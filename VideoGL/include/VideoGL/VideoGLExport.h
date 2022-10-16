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
    /// @brief 生成一个GLHandle句柄
    /// @return GLHandle句柄
    GLHandle API_EXPORT CreatGLHandle();

    /// @brief 初始化GLHandle句柄
    /// @param gl_handle GLHandle句柄
    /// @param width 渲染图像的宽度
    /// @param height 渲染图像的高度
    /// @return 初始化的状态值0成功，其他负数失败码
    int API_EXPORT InitGLHandele(GLHandle gl_handle,const uint64_t &width,const uint64_t &height);

    /// @brief 通过GLHandle对输入的RGBA(RGB32)数据进行渲染
    /// @param gl_handle GLhandle句柄
    /// @param width 图像数据的宽度
    /// @param height 图像数据的高度
    /// @param data 图像数据
    /// @return 
    bool API_EXPORT GLHandleShowVideo(GLHandle gl_handle, const uint64_t &width,const uint64_t &height,uint8_t * data);

}

#endif