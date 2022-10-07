#define EXPORT_DEV
#include "VideoGLExport.h"
#include "VideoGL.h"

GLHandle API_EXPORT CreatGLHandle()
{
    return new VideoGL;
}

int API_EXPORT InitGLHandele(GLHandle gl_handle,const uint64_t &width,const uint64_t &height)
{
    VideoGL *video_gl = (VideoGL *)gl_handle;
    return video_gl->InitVideoGL(width,height);
}

bool API_EXPORT GLHandleShowVideo(GLHandle gl_handle, const uint64_t &width,const uint64_t &height,uint8_t * data)
{
    VideoGL *video_gl = (VideoGL*)gl_handle;
    video_gl->setWidth(width);
    video_gl->setHeight(height);
    video_gl->setData(data,width*height*4);
    return video_gl->VideoShow(video_gl->getWidth(),video_gl->getHeight(),video_gl->getData());
}