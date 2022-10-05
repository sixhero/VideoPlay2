#include <stdio.h>
#ifdef __WIN32
#include <Windows.h>
#elif __linux__
#include <unistd.h>
#endif

#include "VideoDesExport.h"
#include "VideoGLExport.h"

#ifndef STB_IMAGE_IMPLEMENTATION
    #define STB_IMAGE_IMPLEMENTATION
    #include "stb_image.h"
#endif

int main()
{
	AVHandle av_handle = CreatAVHandle();
	#ifdef __WIN32
	int ret = InitAVHandle(av_handle, "D:/Download/阿拉涅的虫笼.mp4");
	#else
	//int ret = InitAVHandle(av_handle, "/home/sixhero/Downloads/Hole.mp4");
	int ret = InitAVHandle(av_handle, "/home/sixhero/Downloads/AlaNie.mp4");

	#endif
	int width = GetAVHandleWidth(av_handle);
	int height = GetAVHandleHeight(av_handle);
	uint8_t *buff = new uint8_t[width * height * 4];
	int64_t bufflen;
	int64_t pts_start;
	int64_t pts_end;
	int64_t pts;

	//获取屏幕尺寸
	

	//OpenGL模块
	GLHandle gl_handle = CreatGLHandle();
	InitGLHandele(gl_handle,width,height);
	//获取第一帧视频
	while (!AVHandleGetVideoData(av_handle, buff, &bufflen, &pts_start))
	{
#ifdef __WIN32
		Sleep(20);
#elif __linux__
		usleep(3);
#endif
	}

	while (true)
	{
		AVHandleGetVideoData(av_handle, buff, &bufflen, &pts_end);

		pts = pts_end - pts_start;

		GLHandleShowVideo(gl_handle,width,height,buff);

#ifdef __WIN32
		if (pts > 0)
		{
			//Sleep(pts-3);
		}
#elif __linux__
		usleep(pts*1000-2000);//减运行误差
#endif
		pts_start = pts_end;
	}
	printf("VideoPlay");
	return 0;
}