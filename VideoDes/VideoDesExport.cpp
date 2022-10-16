#define EXPORT_DEV
#include "VideoDes/VideoDesExport.h"
#include "VideoDes.h"

AVHandle API_EXPORT CreatAVHandle()
{
	return new VideoDes;
}

int API_EXPORT InitAVHandle(AVHandle av_handle, std::string source_url)
{
	VideoDes *video_des = (VideoDes *)av_handle;
	if (!source_url.empty())
	{
		video_des->setSourceUrl(source_url);
	}
	return video_des->InitVideoDes();
}

int API_EXPORT GetAVHandleWidth(AVHandle av_handle)
{
	VideoDes *video_des = (VideoDes *)av_handle;
	return video_des->getVideoWidth();
}

int API_EXPORT GetAVHandleHeight(AVHandle av_handle)
{
	VideoDes *video_des = (VideoDes *)av_handle;
	return video_des->getVideoHeight();
}

bool API_EXPORT AVHandleGetVideoData(AVHandle av_handle, uint8_t *databuff, int64_t *bufflen, int64_t *pts)
{
	VideoDes *video_des = (VideoDes *)av_handle;
	VideoData *video_data = video_des->GetVideoData();
	if (video_data == nullptr)
	{
		return false;
	}
	memcpy(databuff, video_data->_data, video_data->size);
	*bufflen = video_data->size;
	*pts = video_data->pts;
	delete video_data;
	return true;
}
