#include "VideoDes.h"

VideoDes::VideoDes()
{
    //初始化日志库
    m_logger = spdlog::stdout_color_mt("VideoDes");
    m_logger->info("对象初始化");
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
    m_logger->info("设置优化参数，对视频源设置固有属性");
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

    m_logger->info(std::string("数据源地址：") + getSourceUrl());

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
    if (m_video_index != 1)
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
            ret = swr_alloc_set_opts2(
                &m_audio_swr_context,
                &m_av_audio_code_context->ch_layout,
                AVSampleFormat::AV_SAMPLE_FMT_S16,
                m_av_audio_code_context->sample_rate,
                &m_av_audio_code_context->ch_layout,
                m_av_audio_code_context->sample_fmt,
                m_av_audio_code_context->sample_rate,
                0, nullptr);
            if (ret != 0)
            {
                m_logger->error(std::string("初始化音频转化参数失败 ") + av_make_error_string(errbuf, AV_ERROR_MAX_STRING_SIZE, ret));
                return -6;
            }
        }
    }

    //初始化数据
    m_av_frame_src = av_frame_alloc();
    m_av_frame_dest = av_frame_alloc();
    m_av_packet = av_packet_alloc();

    m_video_width = m_av_video_code_context->width;
    m_video_height = m_av_video_code_context->height;

    return 0;
}

int VideoDes::AVDecode(const uint8_t *databuff, const int *bufflen)
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
        //发送数据包到视频解码器上下文
        ret = avcodec_send_packet(m_av_video_code_context, m_av_packet);
        if (ret < 0)
        {
            m_logger->warn(std::string("发送数据包到视频解码器上下文失败 ") + av_make_error_string(errbuf, AV_ERROR_MAX_STRING_SIZE, ret));
            ret = -2;
            goto END;
        }

        //获取到图片可用数据
        ret = avcodec_receive_frame(m_av_video_code_context, m_av_frame_src);
        if (ret != 0)
        {
            m_logger->warn(std::string("接收视频解码器输出的数据帧不可用 ") + av_make_error_string(errbuf, AV_ERROR_MAX_STRING_SIZE, ret));
            ret = -3;
            goto END;
        }

        //初始化目的帧绑定数据缓冲
        ret = av_image_fill_arrays(m_av_frame_dest->data, m_av_frame_dest->linesize, databuff, AV_PIX_FMT_RGB32, m_video_width, m_video_height, 4);
        if (ret < 0)
        {
            m_logger->warn(std::string("绑定目的帧数据缓冲失败 ") + av_make_error_string(errbuf, AV_ERROR_MAX_STRING_SIZE, ret));
            ret = -4;
            goto END;
        }

        //视频格式数据转换，转码为RGB32
        ret = sws_scale(m_video_sws_context, m_av_frame_src->data, m_av_frame_src->linesize, 0, m_video_height, m_av_frame_dest->data, m_av_frame_dest->linesize);
        if (ret < 0)
        {
            m_logger->warn(std::string("视频格式数据转换，转码为RGB32失败 ") + av_make_error_string(errbuf, AV_ERROR_MAX_STRING_SIZE, ret));
            ret = -5;
            goto END;
        }
        //视频数据样式
        ret = 0;
        goto END;
    }
    else if (m_av_packet->stream_index == m_audio_index)
    {
    }
    // return -1;

END:
    av_packet_unref(m_av_packet);
    av_frame_unref(m_av_frame_src);
    av_frame_unref(m_av_frame_dest);
    if (ret == 0)
    {
        m_logger->info("提取视频数据成功");
    }
    else if (ret == 1)
    {
        m_logger->info("提取音频数据成功");
    }
    return ret;
}
