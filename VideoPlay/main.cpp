#include <stdio.h>

#include "VideoExport.h"

int main()
{
	AVHandle av_handle = CreatAVHandle();
	int ret = InitAVHandle(av_handle, "D:/Download/阿拉涅的虫笼.mp4");
	int width = GetAVHandleWidth(av_handle);
	int height = GetAVHandleHeight(av_handle);
	uint8_t* buff = new uint8_t[width * height * 4];
	int* bufflen = new int;
	while (true)
	{
		//AVHandleDecode(av_handle, buff, bufflen);
		_sleep(3);
	}
	printf("VideoPlay");
	return 0;
}