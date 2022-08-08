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
	int *bufflen = new int;
	while (true)
	{
// AVHandleDecode(av_handle, buff, bufflen);
#ifdef __WIN32
		Sleep(3);
#elif __linux__
		sleep(3);
#endif
	}
	printf("VideoPlay");
	return 0;
}