#include "Renderer.hpp"
#include <QDebug>
#include <iostream>

// 着色器源码
const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    void main()
    {
        gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    }
)";

const char* fragmentShaderSource = R"(
    #version 330 core
    out vec4 FragColor;
    void main()
    {
        FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
    }
)";

Renderer::Renderer() 
    : m_VAO(0), m_VBO(0), m_shaderProgram(0), 
      m_initialized(false), m_viewportWidth(0), m_viewportHeight(0)
{
}

Renderer::~Renderer()
{
    cleanup();
}

bool Renderer::initialize()
{
    if (m_initialized) {
        return true;
    }
    
    // 初始化GLAD
    if (!gladLoadGL()) {
        qDebug() << "Failed to initialize GLAD in Renderer";
        return false;
    }
    
    // 输出OpenGL信息
    std::cout << "OpenGL Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "OpenGL Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    
    // 设置初始OpenGL状态
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    
    // 定义三角形顶点数据
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };
    
    // 创建和配置VAO、VBO
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    // 创建着色器程序
    m_shaderProgram = createShaderProgram(vertexShaderSource, fragmentShaderSource);
    if (m_shaderProgram == 0) {
        qDebug() << "Failed to create shader program in Renderer";
        return false;
    }
    
    m_initialized = true;
    qDebug() << "Renderer initialized successfully";
    return true;
}

void Renderer::render()
{
    if (!m_initialized) {
        qDebug() << "Renderer not initialized!";
        return;
    }
    
    glClear(GL_COLOR_BUFFER_BIT);
    
    glUseProgram(m_shaderProgram);
    glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}

void Renderer::resize(int width, int height)
{
    m_viewportWidth = width;
    m_viewportHeight = height;
    glViewport(0, 0, width, height);
}

void Renderer::cleanup()
{
    if (m_VAO) {
        glDeleteVertexArrays(1, &m_VAO);
        m_VAO = 0;
    }
    if (m_VBO) {
        glDeleteBuffers(1, &m_VBO);
        m_VBO = 0;
    }
    if (m_shaderProgram) {
        glDeleteProgram(m_shaderProgram);
        m_shaderProgram = 0;
    }
    
    m_initialized = false;
}

// 着色器编译辅助函数（与之前实现相同）
GLuint Renderer::compileShader(GLenum type, const char* source)
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);
    
    if (!checkShaderCompileStatus(shader)) {
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

GLuint Renderer::createShaderProgram(const char* vertexSource, const char* fragmentSource)
{
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);
    
    if (vertexShader == 0 || fragmentShader == 0) {
        return 0;
    }
    
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    
    if (!checkProgramLinkStatus(program)) {
        glDeleteProgram(program);
        program = 0;
    }
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    return program;
}

bool Renderer::checkShaderCompileStatus(GLuint shader)
{
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        qDebug() << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog;
        return false;
    }
    return true;
}

bool Renderer::checkProgramLinkStatus(GLuint program)
{
    GLint success;
    GLchar infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        qDebug() << "ERROR::PROGRAM::LINKING_FAILED\n" << infoLog;
        return false;
    }
    return true;
}