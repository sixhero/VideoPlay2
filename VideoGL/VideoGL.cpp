#include "VideoGL.h"

/// @brief 窗口信息回调函数
/// @param window 窗口句柄
/// @param width 宽度
/// @param height 高度
void FramebufferSizeCallback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}   

VideoGL::VideoGL()
{
    //初始化日志库
    m_logger = spdlog::stdout_color_mt("VideoGL");
    m_logger->info("VideoGL对象初始化 ");
}
uint64_t VideoGL::getHeight()
{
    return m_height;
}
uint64_t VideoGL::getWidth()
{
    return m_width;
}
void VideoGL::setHeight(const uint64_t &height)
{
    m_height = height;
}
void VideoGL::setWidth(const uint64_t &width)
{
    m_width = width;
}

int VideoGL::InitVideoGL(const uint64_t &width, const uint64_t &height)
{
    m_width = width;
    m_height = height;
    int state = glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //初始化窗口
    m_glfw_window = glfwCreateWindow(843, 480, "VedioGL", NULL, NULL);
    // m_glfw_window = glfwCreateWindow(width,height,"VedioGL",NULL,NULL);

    if (m_glfw_window == nullptr)
    {
        m_logger->error("GLFW的窗口句柄初始化失败");
        glfwTerminate();
        return -1;
    }

    //为窗口绑定上下文
    glfwMakeContextCurrent(m_glfw_window);

    //初始化GLAD组件
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        m_logger->error("初始化GLAD组件失败");
        return -2;
    }

    //设置窗口的尺寸
    glad_glViewport(0, 0, 843, 480);
    // glad_glViewport(0,0,width,height);

    //生成纹理对象，第一个参数为数量，第二个参数为纹理对象数组，存放申请的纹理对象
    glad_glGenTextures(1, &m_texture_2D);
    //绑定纹理类型
    glad_glBindTexture(GL_TEXTURE_2D, m_texture_2D);
    //设置纹理参数
    glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //创建顶点着色器
    m_vertex_shader = glad_glCreateShader(GL_VERTEX_SHADER);
    //绑定着色器源码 第一个参数着色器对象，第二参数指定了传递的源码字符串数量，第三个参数是顶点着色器真正的源码，第四个参数我们先设置为NULL
    glad_glShaderSource(m_vertex_shader, 1, &vertex_shader_source, nullptr);
    //编译顶点着色器
    glad_glCompileShader(m_vertex_shader);

    //检查是否编译成功
    int success;
    char infoLog[512];
    glad_glGetShaderiv(m_vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        //编译失败
        glad_glGetShaderInfoLog(m_vertex_shader, 512, nullptr, infoLog);
        m_logger->error(std::string() + "顶点着色器编译失败-" + infoLog);
        return -3;
    }

    //创建片段着色器
    m_fragment_shader = glad_glCreateShader(GL_FRAGMENT_SHADER);
    //绑定着色器源码
    glad_glShaderSource(m_fragment_shader, 1, &fragment_shader_source, nullptr);
    //编译着色器
    glad_glCompileShader(m_fragment_shader);

    //检查是否编译成功
    glad_glGetShaderiv(m_fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        //编译失败
        glad_glGetShaderInfoLog(m_fragment_shader, 512, nullptr, infoLog);
        m_logger->error(std::string() + "片段着色器编译失败-" + infoLog);
        return -3;
    }

    //创建程序对象
    m_shader_program = glad_glCreateProgram();
    //附加着色器
    glad_glAttachShader(m_shader_program, m_vertex_shader);
    glad_glAttachShader(m_shader_program, m_fragment_shader);
    //链接生成程序对象
    glad_glLinkProgram(m_shader_program);

    //清理着色器对象，
    glad_glDeleteShader(m_vertex_shader);
    glad_glDeleteShader(m_fragment_shader);

    //生成缓冲对象
    glad_glGenBuffers(1, &m_VBO);
    glad_glGenBuffers(1, &m_EBO);
    //生成顶点数组对象
    glad_glGenVertexArrays(1, &m_VAO);

    //绑定缓冲对象，类型为数组缓冲
    glad_glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glad_glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //绑定顶点对象
    glad_glBindVertexArray(m_VAO);

    //绑定元素缓冲对象，绘制的区域
    //GL_ELEMENT_ARRAY_BUFFER类型会被glad_glDrawElements触发
    glad_glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glad_glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //取出要渲染的顶点坐标区域--数据会传入顶点渲染器中
    glad_glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glad_glEnableVertexAttribArray(0);

    //取出纹理坐标
    glad_glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glad_glEnableVertexAttribArray(1);

    //注册窗口变动回调函数--数据会传入顶点渲染器中
    glfwSetFramebufferSizeCallback(m_glfw_window, &FramebufferSizeCallback);

    //初始化要渲染的RGBA数据
    m_data = new uint8_t[m_width * m_height * 4];
    memset(m_data, 0, m_width * m_height * 4);
    return 0;
}

bool VideoGL::VideoShow(const uint64_t &width, const uint64_t &height, const uint8_t *data)
{
    if(m_glfw_window == nullptr)
    {
        return false;
    }
    if (!glfwWindowShouldClose(m_glfw_window))
    {
        ProcessInput(m_glfw_window);
        //生成纹理
        glad_glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

        m_logger->info(std::string("渲染图像") + std::to_string(glad_glGetError()));

        glad_glClearColor(0.5f, 0.5f, 0.5f, 0.5f);
        glad_glClear(GL_COLOR_BUFFER_BIT);

        //激活纹理
        glad_glActiveTexture(GL_TEXTURE0);
        //绑定纹理
        glad_glBindTexture(GL_TEXTURE_2D, m_texture_2D);

        glad_glUseProgram(m_shader_program);
        glad_glBindVertexArray(m_VAO);

        //渲染区域
        glad_glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        //交换缓冲，检查并调用事件
        glfwSwapBuffers(m_glfw_window);
        glfwPollEvents();
        return true;
    }
    else
    {
        //窗口关闭退出
        ExitVideoGL();
        return false;
    }
    return false;
}

void VideoGL::ExitVideoGL()
{
    glad_glDeleteVertexArrays(1, &m_VAO);
    glad_glDeleteBuffers(1, &m_VBO);
    glad_glDeleteBuffers(1, &m_EBO);

    //停止GLFW窗口
    glfwTerminate();

    m_glfw_window = nullptr;

    m_logger->info("退出视频渲染器");
}

void VideoGL::setData(const uint8_t *data, const uint64_t &data_length)
{
    memcpy(m_data, data, data_length);
}

uint8_t *VideoGL::getData()
{
    return m_data;
}

void VideoGL::ProcessInput(GLFWwindow *window)
{
    if(glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}
