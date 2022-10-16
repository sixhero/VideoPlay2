#ifndef _VIDEO_GL_
#define _VIDEO_GL_

#include <thread>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdint.h>
#define STRING(x) #x
#define SHADER(x) "" STRING(x)

/// @brief 窗口信息回调函数
/// @param window 窗口句柄
/// @param width 宽度
/// @param height 高度
void FramebufferSizeCallback(GLFWwindow *window, int width, int height);

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


    /// @brief 渲染图像视频的函数，由于VideoDes设置转换的类型为RGB32,此处为2D纹理绑定的类型为GL_RGBA类型匹配
    /// @param width 数据宽度
    /// @param height 数据高度
    /// @param data 数据内存块
    /// @return 是否显示
    bool VideoShow(const uint64_t &width,const uint64_t &height,const uint8_t* data);
    

private:
	/// @brief 快速日志句柄
	std::shared_ptr<spdlog::logger> m_logger;
    /// @brief 渲染图像高度
    uint64_t m_height;
    /// @brief 渲染图像的宽度
    uint64_t m_width;

    /// @brief 存放RGBA数据进行渲染
    uint8_t *m_data = nullptr;

    // //VideoGL画面渲染线程
    // std::thread gl_thread;

    // void GLShower();
    
    /// @brief 处理GLFW窗口输入事件
    /// @param window GLFW窗口句柄
    void ProcessInput(GLFWwindow *window);


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
    const float vertices[4 * 5] = {
        //渲染的位置                   纹理坐标
        1.0f,1.0f,0.0f,             1.0f,0.0f, 
        1.0f,-1.0f,0.0f,            1.0f,1.0f, 
        -1.0f,-1.0f,0.0f,           0.0f,1.0f, 
        -1.0f,1.0f,0.0f,            0.0f,0.0f, 
    };
    /// @brief 绘制两个三角（合起来为矩形）的渲染区域
    const unsigned int indices[2*3] = {
        0,1,3,
		1,2,3
    };

    const char* vertex_shader_source = (char *)SHADER(\
        #version 330 core\n
        layout(location = 0) in vec3 aPos;
        layout(location = 1) in vec2 aTexCoord;

        out vec2 TexCoord;

        void main()
        {
            gl_Position = vec4(aPos, 1.0);
            TexCoord = aTexCoord;
        }
    );
    /// @brief 片段着色器片段
    const char* fragment_shader_source = (char *)SHADER(\
        #version 330 core\n
        out vec4 FragColor;

        in vec2 TexCoord;

        uniform sampler2D ourTexture;

        void main()
        {
            FragColor = texture(ourTexture, TexCoord);
        }
    );
};

#endif