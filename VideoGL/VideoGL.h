#ifndef _VIDEO_GL_
#define _VIDEO_GL_

#include <thread>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <stdint.h>
#define STRING(x) #x
#define SHADER(x) "" STRING(x)

class VideoGL
{
public:
    VideoGL();
    /// @brief 初始化上下文的信息
    /// @return 返回 0成功，-1错误码
    int InitVideoGL(const uint64_t &width,const uint64_t &height);

    /// @brief 退出渲染器，释放资源
    /// @return 
    void ExitVideoGL();

    uint64_t getHeight();
    uint64_t getWidth();
    void setHeight(const uint64_t &height);
    void setWidth(const uint64_t &width);

    void setData(const uint8_t * data,const uint64_t & data_length);
    uint8_t * getData();


    /// @brief 渲染图像视频的函数
    /// @param width 数据宽度
    /// @param height 数据高度
    /// @param data 数据内存块
    void VideoShow(const uint64_t &width,const uint64_t &height,const uint8_t* data);
    

private:
	/// @brief 快速日志句柄
	std::shared_ptr<spdlog::logger> m_logger;
    /// @brief glfw的窗口高度
    uint64_t m_height;
    /// @brief glfw的窗口宽度
    uint64_t m_width;

    //
    uint8_t *m_data = nullptr;


    // //VideoGL画面渲染线程
    // std::thread gl_thread;

    // void GLShower();


private:
    /// @brief 创建OpenGL窗口的上下文指针
    GLFWwindow *m_glfw_window = nullptr;
    /// @brief 顶点渲染器
    GLuint m_vertex_shader;
    /// @brief 片段渲染器
    GLuint m_fragment_shader;
    /// @brief 最后渲染的OpenGL程序
    GLuint m_shader_program;
    /// @brief 顶点缓冲对象
    GLuint m_VBO;
    /// @brief 顶点数组对象
    GLuint m_VAO;
    /// @brief 元素缓冲对象
    GLuint m_EBO;
    /// @brief 2D纹理对象
    GLuint m_texture_2D;
private:
    /// @brief 顶点渲染器坐标设置
    const float vertices[4 * 8] = {
        //渲染的位置            颜色                纹理坐标
        1.0f,1.0f,0.0f,        1.0f,1.0f,1.0f,     1.0f,0.0f, 
        1.0f,-1.0f,0.0f,       1.0f,1.0f,1.0f,     1.0f,1.0f, 
        -1.0f,-1.0f,0.0f,      1.0f,1.0f,1.0f,     0.0f,1.0f, 
        -1.0f,1.0f,0.0f,       1.0f,1.0f,1.0f,     0.0f,0.0f, 
    };

    /// @brief 顶点着色器源码
    const char* vertex_shader_source = (char *)SHADER(\
        #version 330 core\n
        layout(location = 0) in vec3 aPos;
        layout(location = 1) in vec3 aColor;
        layout(location = 2) in vec2 aTexCoord;

        out vec3 ourColor;
        out vec2 TexCoord;

        void main()
        {
            gl_Position = vec4(aPos, 1.0);
            ourColor = aColor;
            TexCoord = aTexCoord;
        }
    );
    /// @brief 片段着色器片段
    const char* fragment_shader_source = (char *)SHADER(\
        #version 330 core\n
        out vec4 FragColor;

        in vec3 ourColor;
        in vec2 TexCoord;

        uniform sampler2D ourTexture;

        void main()
        {
            FragColor = texture(ourTexture, TexCoord);
        }
    );
};

#endif