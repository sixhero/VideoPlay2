#ifndef _VIDEOEXPORT
#define _VIDEOEXPORT
#pragma once
#include <string>
#ifdef WIN32
#ifdef EXPORT_DEV
#define API_EXPORT __declspec( dllexport )
#else
#define API_EXPORT __declspec( dllimport )
#endif
#else
#define API_EXPORT 
#endif

 /// @brief 解码器句柄
typedef void * AVHandle;

extern "C"
{
	/// @brief 创建解码器句柄
	/// @return 解码器句柄
	AVHandle API_EXPORT CreatAVHandle();

	/// @brief 初始化解码器句柄
	/// @param av_handle 解码器句柄
	/// @param source_url 数据源地址
	/// @return 0 成功，其他负值 失败码
	int API_EXPORT InitAVHandle(AVHandle av_handle, std::string source_url = "");

	/// @brief 获取解码器的图像宽度
	/// @param av_handle 解码器句柄
	/// @return 解码器的图像宽度
	int API_EXPORT GetAVHandleWidth(AVHandle av_handle);

	/// @brief 获取解码器的图像高度
	/// @param av_handle 解码器句柄
	/// @return 解码器的图像高度
	int API_EXPORT GetAVHandleHeight(AVHandle av_handle);

	/// @brief 获取队列缓存的视频数据
	/// @param av_handle 
	/// @param databuff 数据缓冲，由调用者提供
	/// @param bufflen 数据缓冲大小，由调用者提供
	/// @return 是否成功获取到视频数据
	bool API_EXPORT AVHandleGetVideoData(AVHandle av_handle, uint8_t* databuff, int64_t* bufflen,int64_t *pts);


}

#endif