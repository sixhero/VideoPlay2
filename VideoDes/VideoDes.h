#ifndef _VIDEODES
#define _VIDEODES
#pragma once

//日志库
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
extern "C"
{
	// ffmpeg库
#include "libavcodec/avcodec.h"
#include "libavdevice/avdevice.h"
#include "libavfilter/avfilter.h"
#include "libavformat/avformat.h"
#include "libavutil/avutil.h"
#include "libswresample/swresample.h"
#include "libswscale/swscale.h"
#include "libavutil/imgutils.h"
};

	// #pragma comment(lib,"avcodec.lib")
	// #pragma comment(lib,"avdevice.lib")
	// #pragma comment(lib,"avfilter.lib")
	// #pragma comment(lib,"avformat.lib")
	// #pragma comment(lib,"avutil.lib")
	// #pragma comment(lib,"swresample.lib")
	// #pragma comment(lib,"swscale.lib")

#include <string>
#include <queue>
#include <thread>

#define MAX_QUEUE_SIZE 8

/// @brief 视频解码后的视频结构体
typedef struct
{
	uint8_t *_data; // RGB图像数据
	int64_t size;	// RGB图像数据大小
	int64_t pts;	//视频显示时间
} VideoData, *_VideoData;

/// @brief 视频解码后的音频结构体
typedef struct
{
	uint8_t *_data; //音频数据
	int64_t size;	//音频数据大小
	int64_t pts;	//音频播放时间
} AudioData, *_AudioData;

/// <summary>
/// 此类的实现目的，只负责解码工作，不向外提供内存数据
/// 不存储其他类型数据
/// </summary>
class VideoDes
{
	///李佳 98年11月份
public:
	VideoDes();

	/// @brief m_source_url的getter
	/// @return m_source_url
	std::string getSourceUrl();

	/// @brief m_source_url的setter
	/// @param url 数据源地址m_source_url
	void setSourceUrl(const std::string url);

	/// @brief 获取视频宽度
	/// @return 视频宽度
	int getVideoWidth() const;

	/// @brief 获取视频高度
	/// @return 视频高度
	int getVideoHeight() const;

	/// @brief 初始化解码环境
	/// @return 0 成功 其他错误值
	int InitVideoDes();

	/// @brief 对数据进行解码
	/// @param databuff 解码成功的数据缓冲
	/// @param bufflen 数据缓冲大小尺寸
	/// @return 负数 解码失败，0 视频数据，1 音频数据
	int AVDecode();

	/// @brief 获取视频队列的数据
	/// @return 返回一个视频数据结构
	VideoData *GetVideoData();

private:
	/// @brief 快速日志句柄
	std::shared_ptr<spdlog::logger> m_logger;

	/// @brief 数据源的地址,外部提供
	std::string m_source_url;

	/// @brief 上下文的类型字典，数据源类型格式等
	AVDictionary *m_av_dictionary = nullptr;

	/// @brief FFmpeg的全局上下文
	AVFormatContext *m_av_format_context = nullptr;

	/// @brief 视频解码器上下文
	AVCodecContext *m_av_video_code_context = nullptr;

	/// @brief 音频解码器上下文
	AVCodecContext *m_av_audio_code_context = nullptr;

	/// @brief 具体的视频解码器
	const AVCodec *m_av_video_codec = nullptr;

	/// @brief 具体的音频解码器
	const AVCodec *m_av_audio_codec = nullptr;

	/// @brief 视频源数据帧
	AVFrame *m_av_frame_src = nullptr;

	/// @brief 指定格式转换后的目的数据帧
	AVFrame *m_av_frame_dest = nullptr;

	/// @brief 视频流索引值
	int8_t m_video_index = -1;

	/// @brief 音频流索引值
	int8_t m_audio_index = -1;

	/// @brief 视频帧转换参数
	SwsContext *m_video_sws_context = nullptr;

	/// @brief 音频帧转换参数
	SwrContext *m_audio_swr_context = nullptr;

	/// @brief 流数据包
	AVPacket *m_av_packet = nullptr;

	/// @brief 视频尺寸宽度
	int m_video_width = 0;

	/// @brief 视频尺寸高度
	int m_video_height = 0;

private:
	/// @brief FFmpeg库函数出错时存放的错误信息
	char errbuf[AV_ERROR_MAX_STRING_SIZE] = {0};

	/// @brief 视频数据队列
	std::queue<VideoData> queue_video_data;

	/// @brief 音频数据队列
	std::queue<AudioData> queue_audio_data;

	/// @brief 数据原解码线程
	std::thread decode_thread;

	/// @brief 视频解码器
	void Decoder();
};

#endif