#include <stdio.h>

#include "VideoExport.h"

int main()
{
	AVHandle av_handle = CreatAVHandle();
	int ret = InitAVHandle(av_handle, "/home/sixhero/Downloads/Hole.mp4");
	int width = GetAVHandleWidth(av_handle);
	int height = GetAVHandleHeight(av_handle);
	uint8_t* buff = new uint8_t[width * height * 4];
	int* bufflen = new int;
	while (true)
	{
		AVHandleDecode(av_handle, buff, bufflen);
	}
	printf("VideoPlay");
	return 0;
}