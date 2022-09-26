#include <stdio.h>
#ifdef __WIN32
#include <Windows.h>
#elif __linux__
#include <unistd.h>
#endif

#include "VideoExport.h"

int main()
{
	AVHandle av_handle = CreatAVHandle();
	int ret = InitAVHandle(av_handle, "D:/Download/阿拉涅的虫笼.mp4");
	int width = GetAVHandleWidth(av_handle);
	int height = GetAVHandleHeight(av_handle);
	uint8_t *buff = new uint8_t[width * height * 4];
	int64_t bufflen;
	int64_t pts_start;
	int64_t pts_end;
	int64_t pts;
	//获取第一帧视频
	while (!AVHandleGetVideoData(av_handle, buff, &bufflen, &pts_start))
	{
#ifdef __WIN32
		Sleep(20);
#elif __linux__
		sleep(3);
#endif
	}

	while (true)
	{
		// AVHandleGetData(av_handle, buff, bufflen);

		AVHandleGetVideoData(av_handle, buff, &bufflen, &pts_end);
		pts = pts_end - pts_start;

#ifdef __WIN32
		if (pts > 0)
		{
			Sleep(pts);
		}
#elif __linux__
		sleep(3);
#endif
		pts_start = pts_end;
	}
	printf("VideoPlay");
	return 0;
}