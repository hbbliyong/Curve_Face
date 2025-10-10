#pragma once

 #include <glad/glad.h>
 
class Renderer
{
public:
    Renderer();
    ~Renderer();
    
    // 初始化渲染器（必须在有效的OpenGL上下文中调用）
    bool initialize();
    
    // 渲染场景
    void render();
    
    // 处理窗口大小变化
    void resize(int width, int height);
    
    // 资源清理
    void cleanup();

private:
    // 编译着色器辅助函数
    GLuint compileShader(GLenum type, const char* source);
    GLuint createShaderProgram(const char* vertexSource, const char* fragmentSource);
    bool checkShaderCompileStatus(GLuint shader);
    bool checkProgramLinkStatus(GLuint program);
    
    // OpenGL 对象句柄
    GLuint m_VAO, m_VBO, m_shaderProgram;
    
    // 渲染状态
    bool m_initialized;
    int m_viewportWidth, m_viewportHeight;
};