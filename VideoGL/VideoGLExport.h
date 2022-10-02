#ifndef _VIDEO_GL_EXPORT_
#define _VIDEO_GL_EXPORT_

#ifdef WIN32
#ifdef EXPORT_DEV
#define API_EXPORT __declspec(dllexport)
#else
#define API_EXPORT __declspec(dllimport)
#endif

#else
#define API_EXPORT
#endif

#include <stdint.h>

typedef void *GLHandle;

extern "C"
{
    GLHandle API_EXPORT CreatGLHandle();

    int API_EXPORT InitGLHandele(GLHandle gl_handle,const uint64_t &width,const uint64_t &height);

    bool API_EXPORT GLHandleShowVideo(GLHandle gl_handle, const uint64_t &width,const uint64_t &height,uint8_t * data);

}

#endif