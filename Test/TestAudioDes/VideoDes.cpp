#include "VideoDes.h"
#include <iostream>
VideoDes::VideoDes()
{
    //初始化日志库
    m_logger = spdlog::stdout_color_mt("VideoDes");
    m_logger->info("VideoDes对象初始化 ");
    // setSourceUrl("D:/Download/阿拉涅的虫笼.mp4");
}

std::string VideoDes::getSourceUrl()
{
    return m_source_url;
}
void VideoDes::setSourceUrl(const std::string url)
{
    m_source_url = url;
}
int VideoDes::getVideoWidth() const
{
    return m_video_width;
}
int VideoDes::getVideoHeight() const
{
    return m_video_height;
}
int VideoDes::InitVideoDes()
{
    m_logger->info("初始化解码器");
    m_logger->info("设置优化参数，对视频源设置固有属性 ");
    //设置优化参数，对视频源设置固有属性
    av_dict_set(&m_av_dictionary, "rtsp_transpot", "+udp+tcp", 0);
    av_dict_set(&m_av_dictionary, "rtsp_flage", "+prefer_tcp", 0);
    av_dict_set(&m_av_dictionary, "buffer_size", "10240000", 0);
    av_dict_set(&m_av_dictionary, "max_delay", "500", 0);
    av_dict_set(&m_av_dictionary, "stimeout", "2000", 0);

    if (getSourceUrl().empty())
    {
        m_logger->error("数据源地址为空，返回值-1");
        return -1;
    }
    int ret = 0;

    //打开数据源地址
    ret = avformat_open_input(&m_av_format_context, m_source_url.c_str(), nullptr, &m_av_dictionary);
    if (ret < 0)
    {
        m_logger->error(std::string("数据源地址打开失败 ") + av_make_error_string(errbuf, AV_ERROR_MAX_STRING_SIZE, ret));
        return -2;
    }

    m_logger->info(std::string("数据源地址:") + getSourceUrl());

    //查找视频源的流信息
    ret = avformat_find_stream_info(m_av_format_context, nullptr);
    if (ret < 0)
    {
        m_logger->error(std::string("查找视频源流信息失败 ") + av_make_error_string(errbuf, AV_ERROR_MAX_STRING_SIZE, ret));
        return -3;
    }

    //获取视频流索引
    for (int i = 0; i < m_av_format_context->nb_streams; i++)
    {
        if (m_av_format_context->streams[i]->codecpar->codec_type == AVMediaType::AVMEDIA_TYPE_VIDEO)
        {
            m_video_index = i;
            m_logger->info(std::string("获取到视频流数据:index=") + std::to_string(i));
            continue;
        }
        if (m_av_format_context->streams[i]->codecpar->codec_type == AVMediaType::AVMEDIA_TYPE_AUDIO)
        {
            m_audio_index = i;
            m_logger->info(std::string("获取到音频流数据:index=") + std::to_string(i));
            continue;
        }
    }

    //寻找视频流解码器
    if (m_video_index != -1)
    {
        m_av_video_codec = avcodec_find_decoder(m_av_format_context->streams[m_video_index]->codecpar->codec_id);
        if (m_av_video_codec == nullptr)
        {
            m_logger->warn("寻找视频流解码器失败,不存在视频流");
        }
        else
        {
            //初始化视频解码器上下文
            m_av_video_code_context = avcodec_alloc_context3(m_av_video_codec);

            //将视频流信息绑定
            ret = avcodec_parameters_to_context(m_av_video_code_context, m_av_format_context->streams[m_video_index]->codecpar);
            if (ret < 0)
            {
                m_logger->error(std::string("绑定视频流数据失败 ") + av_make_error_string(errbuf, AV_ERROR_MAX_STRING_SIZE, ret));
                return -4;
            }

            //打开视频解码器
            ret = avcodec_open2(m_av_video_code_context, m_av_video_codec, nullptr);
            if (ret < 0)
            {
                m_logger->error(std::string("打开视频解码器失败 ") + av_make_error_string(errbuf, AV_ERROR_MAX_STRING_SIZE, ret));
                return -5;
            }

            //设置视频转换参数
            m_video_sws_context = sws_getContext(
                m_av_video_code_context->width,
                m_av_video_code_context->height,
                m_av_video_code_context->pix_fmt,
                m_av_video_code_context->width,
                m_av_video_code_context->height,
                AV_PIX_FMT_RGB32,
                SWS_BILINEAR,
                nullptr, nullptr, nullptr);
        }
        m_video_width = m_av_video_code_context->width;
        m_video_height = m_av_video_code_context->height;
    }

    //寻找音频流解码器
    if (m_audio_index != -1)
    {
        m_av_audio_codec = avcodec_find_decoder(m_av_format_context->streams[m_audio_index]->codecpar->codec_id);
        if (m_av_audio_codec == nullptr)
        {
            m_logger->warn("寻找音频流解码器失败,不存在音频流");
        }
        else
        {
            //初始化音频解码上下文
            m_av_audio_code_context = avcodec_alloc_context3(m_av_audio_codec);

            //将音频流信息进行绑定
            ret = avcodec_parameters_to_context(m_av_audio_code_context, m_av_format_context->streams[m_audio_index]->codecpar);
            if (ret < 0)
            {
                m_logger->error(std::string("绑定音频流数据信息失败 ") + av_make_error_string(errbuf, AV_ERROR_MAX_STRING_SIZE, ret));
                return -4;
            }

            //打开音频解码器
            ret = avcodec_open2(m_av_audio_code_context, m_av_audio_codec, nullptr);
            if (ret < 0)
            {
                m_logger->error(std::string("打开音频解码器失败 ") + av_make_error_string(errbuf, AV_ERROR_MAX_STRING_SIZE, ret));
                return -5;
            }

            //设置音频转换参数
            m_audio_swr_context = swr_alloc();
            swr_alloc_set_opts(
                m_audio_swr_context,
                m_av_audio_code_context->ch_layout.nb_channels,
                AVSampleFormat::AV_SAMPLE_FMT_S16,
                m_av_audio_code_context->sample_rate,
                m_av_audio_code_context->ch_layout.nb_channels,
                m_av_audio_code_context->sample_fmt,
                m_av_audio_code_context->sample_rate,
                0, nullptr);

            // swr_alloc_set_opts
            // ret = swr_alloc_set_opts2(
            //     &m_audio_swr_context,
            //     &m_av_audio_code_context->ch_layout,
            //     AVSampleFormat::AV_SAMPLE_FMT_S16,
            //     m_av_audio_code_context->sample_rate,
            //     &m_av_audio_code_context->ch_layout,
            //     m_av_audio_code_context->sample_fmt,
            //     m_av_audio_code_context->sample_rate,
            //     0, nullptr);
            if (ret != 0)
            {
                m_logger->error(std::string("设置音频转化参数失败 ") + av_make_error_string(errbuf, AV_ERROR_MAX_STRING_SIZE, ret));
                return -6;
            }

            ret = swr_init(m_audio_swr_context);
            if (ret != 0)
            {
                m_logger->error(std::string("初始化音频转化参数失败 ") + av_make_error_string(errbuf, AV_ERROR_MAX_STRING_SIZE, ret));
                return -7;
            }
        }
    }

    //初始化数据
    m_av_frame_src = av_frame_alloc();
    m_av_frame_dest = av_frame_alloc();
    m_av_packet = av_packet_alloc();

    //启动解码线程
    decode_thread = std::thread(&VideoDes::Decoder, this);
    decode_thread.detach(); //分离线程
    return 0;
}

FILE *file = fopen("C:\\Users\\sixhero\\Desktop\\tmp\\tmp", "w+");

int VideoDes::AVDecode()
{
    //开始读取数据源的数据包
    int ret = av_read_frame(m_av_format_context, m_av_packet);
    if (ret < 0)
    {
        m_logger->warn(std::string("读取数据包失败 ") + av_make_error_string(errbuf, AV_ERROR_MAX_STRING_SIZE, ret));
        ret = -1;
        goto END;
    }

    //解码视频数据包
    if (m_av_packet->stream_index == m_video_index)
    {
        _VideoData video_data = new VideoData;
        //发送数据包到视频解码器上下文
        ret = avcodec_send_packet(m_av_video_code_context, m_av_packet);
        if (ret < 0)
        {
            m_logger->warn(std::string("发送数据包到视频解码器上下文失败 ") + av_make_error_string(errbuf, AV_ERROR_MAX_STRING_SIZE, ret));
            ret = -2;
            delete video_data;
            goto END;
        }

        //获取到图片可用数据
        ret = avcodec_receive_frame(m_av_video_code_context, m_av_frame_src);
        if (ret != 0)
        {
            m_logger->warn(std::string("接收视频解码器输出的数据帧不可用 ") + av_make_error_string(errbuf, AV_ERROR_MAX_STRING_SIZE, ret));
            ret = -3;
            delete video_data;
            goto END;
        }

        //自定义的视频数据
        //获取图像内存块尺寸
        video_data->size = av_image_get_buffer_size(AV_PIX_FMT_RGB32, m_video_width, m_video_height, 4);
        if (video_data->size < 0)
        {
            m_logger->warn(std::string("获取图像缓冲大小失败 ") + av_make_error_string(errbuf, AV_ERROR_MAX_STRING_SIZE, video_data->size));
            ret = -6;
            delete video_data;
            goto END;
        }
        //初始化图像缓冲内存块
        video_data->_data = (uint8_t *)av_malloc(video_data->size);
        memset(video_data->_data, 0, video_data->size);

        //将pts转换为时间毫秒
        video_data->pts = 1000 * m_av_frame_src->pts * av_q2d(m_av_format_context->streams[m_video_index]->time_base);

        //初始化目的帧绑定数据缓冲
        ret = av_image_fill_arrays(m_av_frame_dest->data, m_av_frame_dest->linesize, video_data->_data, AV_PIX_FMT_RGB32, m_video_width, m_video_height, 4);
        if (ret < 0)
        {
            m_logger->warn(std::string("绑定目的帧数据缓冲失败 ") + av_make_error_string(errbuf, AV_ERROR_MAX_STRING_SIZE, ret));
            ret = -4;
            delete video_data;
            goto END;
        }

        //视频格式数据转换，转码为RGB32
        ret = sws_scale(m_video_sws_context, m_av_frame_src->data, m_av_frame_src->linesize, 0, m_video_height, m_av_frame_dest->data, m_av_frame_dest->linesize);
        if (ret < 0)
        {
            m_logger->warn(std::string("视频格式数据转换,转码为RGB32失败 ") + av_make_error_string(errbuf, AV_ERROR_MAX_STRING_SIZE, ret));
            ret = -5;
            delete video_data;
            goto END;
        }
        //视频数据样式
        queue_video_data.push(video_data);
        ret = 0;
        goto END;
    }
    else if (m_av_packet->stream_index == m_audio_index)
    {
        _AudioData audio_data = new AudioData;
        //发送数据包到视频解码器上下文
        ret = avcodec_send_packet(m_av_audio_code_context, m_av_packet);
        if (ret < 0)
        {
            m_logger->warn(std::string("发送数据包到音频解码器上下文失败 ") + av_make_error_string(errbuf, AV_ERROR_MAX_STRING_SIZE, ret));
            ret = -2;
            delete audio_data;
            goto END;
        }

        //获取到音频帧可用数据
        ret = avcodec_receive_frame(m_av_audio_code_context, m_av_frame_src);
        if (ret != 0)
        {
            m_logger->warn(std::string("接收音频解码器输出的数据帧不可用 ") + av_make_error_string(errbuf, AV_ERROR_MAX_STRING_SIZE, ret));
            ret = -3;
            delete audio_data;
            goto END;
        }
        // //计算输出音频数据的有效长度
        // audio_data->size = av_samples_get_buffer_size(NULL, m_av_frame_src->ch_layout.nb_channels, m_av_frame_src->nb_samples, AV_SAMPLE_FMT_S16, 1);
        // //初始化音频缓冲内存块
        // audio_data->_data = (uint8_t *)av_malloc(audio_data->size);
        // memset(audio_data->_data, 0, audio_data->size);


        audio_data->size = av_samples_alloc(&audio_data->_data,NULL,m_av_frame_src->ch_layout.nb_channels,m_av_frame_src->nb_samples,AV_SAMPLE_FMT_S16, 1);

        //将pts转换为时间毫秒
        audio_data->pts = 1000 * m_av_frame_src->pts * av_q2d(m_av_format_context->streams[m_audio_index]->time_base);

        // //绑定填充要转换的目的帧
        // av_samples_fill_arrays(m_av_frame_dest->data,
        //                        m_av_frame_dest->linesize,
        //                        audio_data->_data,
        //                        m_av_frame_src->ch_layout.nb_channels,
        //                        m_av_frame_src->nb_samples,
        //                        AV_SAMPLE_FMT_S16,
        //                        1);

        //音频编码转换将数据取出
        ret = swr_convert(m_audio_swr_context,
                            &audio_data->_data,
                            audio_data->size/2,
                        //   m_av_frame_dest->data,
                        //   m_av_frame_src->nb_samples,
                          (const uint8_t **)m_av_frame_src->data,
                          m_av_frame_src->nb_samples);
        if (ret < 0)
        {
            m_logger->warn(std::string("音频数据转化，失败 ") + av_make_error_string(errbuf, AV_ERROR_MAX_STRING_SIZE, ret));
            ret = -5;
            delete audio_data;
            goto END;
        }
        if (file)
        {
            fwrite(audio_data->_data, sizeof(uint8_t), audio_data->size, file);
        }
        delete audio_data;
        m_logger->error(std::to_string(m_av_frame_dest->sample_rate) + "--------" + std::to_string(m_av_frame_src->sample_rate));
        // queue_audio_data.push(audio_data);
        ret = 1;
        goto END;
    }
    // return -1;

END:
    av_packet_unref(m_av_packet);
    av_frame_unref(m_av_frame_src);
    // av_frame_unref(m_av_frame_dest);
    if (ret == 0)
    {
        m_logger->info("提取视频数据成功");
    }
    else if (ret == 1)
    {
        m_logger->info("提取音频数据成功");
    }
    else
    {
        m_logger->info(std::string("出现错误，错误码:") + std::to_string(ret));
    }
    return ret;
}

VideoData *VideoDes::GetVideoData()
{
    if (queue_video_data.size() <= 0)
    {
        m_logger->info("获取数据失败");
        return nullptr;
    }
    _VideoData video_data = queue_video_data.front();
    queue_video_data.pop();
    return video_data;
}

void VideoDes::Decoder()
{
    while (true)
    {
        //这个判断方式有点扯淡
        if (queue_video_data.size() < MAX_QUEUE_SIZE)
        {
            AVDecode();
        }
        else
        {
            Sleep(1);
        }
    }
}
