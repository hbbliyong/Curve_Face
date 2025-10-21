#pragma once

#include <glad/glad.h>
#include <memory>
#include <glm/glm.hpp>

struct Vertex {
    glm::vec2 position;
    glm::vec3 color; // 可选，增加颜色属性
};

struct Line {
    Vertex start, end;
    Line(Vertex s, Vertex e) : start(s), end(e) {}
};

class Shader;
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

    // 数据更新接口
    void setPoints(const std::vector<glm::vec2>& points);
    void setLines(const std::vector<Line>& lines);
    void setCurrentPoint(const glm::vec2& point);
    void setDrawingMode(bool continuous);
    void beginBatch();
    void addPoint(const glm::vec2& position, const glm::vec3& color = { 1.0f, 1.0f, 1.0f });
    void addLine(const glm::vec2& start, const glm::vec2& end, const glm::vec3& color = { 1.0f, 1.0f, 1.0f });
    void endBatch();
    void flush(); // 执行实际的绘制命令
    // 资源清理
    void cleanup();
    std::vector<glm::vec2> generatorBezierByRecursion(std::vector<glm::vec2>& controlPoints, int segmentCount);
    std::vector<glm::vec2> generatorBezierByIterative(std::vector<glm::vec2>& controlPoints, int segmentCount);
private:
    // OpenGL 对象句柄
    GLuint m_VAO, m_VBO;
    std::shared_ptr< Shader> m_shader;
    // 渲染状态
    bool m_initialized;
    int m_viewportWidth, m_viewportHeight;


    // 渲染数据
    std::vector<glm::vec2> m_points;
    std::vector<Line> m_lines;
    glm::vec2 m_currentPoint;
    bool m_continuousMode;
  

    std::vector<Vertex> m_pointVertices;
    std::vector<Vertex> m_lineVertices;

    // 对应的OpenGL对象
    GLuint m_pointVAO, m_pointVBO;
    GLuint m_lineVAO, m_lineVBO;
};