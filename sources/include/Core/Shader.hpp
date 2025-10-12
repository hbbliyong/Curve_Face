#pragma once

#include <glad/glad.h>
#include <string>
#include <glm/glm.hpp> // 如果需要使用glm数学库

class Shader
{
public:
    // 程序ID
    unsigned int ID;
    
    // 构造函数读取并构建着色器
    Shader(const char* vertexPath, const char* fragmentPath);
    // 析构函数释放资源
    ~Shader();
    
    // 使用/激活程序
    void use();
    
    // Utility uniform 函数
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    
    // 如果需要使用glm数学库，可以添加以下函数
    void setVec2(const std::string &name, const glm::vec2 &value) const;
    void setVec3(const std::string &name, const glm::vec3 &value) const;
    void setVec4(const std::string &name, const glm::vec4 &value) const;
    void setMat2(const std::string &name, const glm::mat2 &mat) const;
    void setMat3(const std::string &name, const glm::mat3 &mat) const;
    void setMat4(const std::string &name, const glm::mat4 &mat) const;

private:
    // 辅助函数，用于检查编译和链接错误
    void checkCompileErrors(unsigned int shader, std::string type);

    // 编译着色器辅助函数
    GLuint compileShader(GLenum type, const char* source);
    GLuint createShaderProgram(const char* vertexSource, const char* fragmentSource);
    bool checkShaderCompileStatus(GLuint shader);
    bool checkProgramLinkStatus(GLuint program);
};