#include <stdio.h>
#ifdef __WIN32
#include <Windows.h>
#elif __linux__
#include <unistd.h>
#endif

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include "VideoDesExport.h"
#include "VideoGLExport.h"

int main()
{
	/// @brief 快速日志句柄
	std::shared_ptr<spdlog::logger> m_logger;
	m_logger = spdlog::stdout_color_mt("VideoPlay");
    m_logger->info("VideoPlay对象初始化 ");

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

		if(!GLHandleShowVideo(gl_handle,width,height,buff))
		{
			m_logger->info("退出渲染程序");
			break;
		}

#ifdef __WIN32
		if (pts > 0)
		{
			//Sleep(pts-3);
		}
#elif __linux__
		//睡眠微妙（秒，1000×毫秒，1000×微妙）
		usleep(pts*1000-2000);//正常应该减去程序运行的耗时，暂时未作
#endif
		pts_start = pts_end;
	}
	m_logger->info("退出程序");
	return 0;
}