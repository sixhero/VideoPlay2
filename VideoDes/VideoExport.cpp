#define EXPORT_DEV
#include "VideoExport.h"
#include "VideoDes.h"

AVHandle API_EXPORT CreatAVHandle()
{
	return new VideoDes;
}

int API_EXPORT InitAVHandle(AVHandle av_handle, std::string source_url)
{
	VideoDes* video_des = (VideoDes*)av_handle;
	if (!source_url.empty())
	{
		video_des->setSourceUrl(source_url);
	}
	return video_des->InitVideoDes();
}

int API_EXPORT GetAVHandleWidth(AVHandle av_handle)
{
	VideoDes* video_des = (VideoDes*)av_handle;
	return video_des->getVideoWidth();
}

int API_EXPORT GetAVHandleHeight(AVHandle av_handle)
{
	VideoDes* video_des = (VideoDes*)av_handle;
	return video_des->getVideoHeight();
}

int API_EXPORT AVHandleDecode(AVHandle av_handle, const uint8_t* databuff, const int* bufflen)
{
	VideoDes* video_des = (VideoDes*)av_handle;
	return video_des->AVDecode();
}



