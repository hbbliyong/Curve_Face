#pragma once

#include <glad/glad.h>
#include <memory>
#include <glm/glm.hpp>

#include "RendererMode.hpp"
#include "IRenderer.hpp"

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

    void setCurrentPoint(const glm::vec2& point);
    void setDrawingMode(RendererMode mode);
    void beginBatch();
    void addPoint(const glm::vec2& position, const glm::vec3& color = { 1.0f, 1.0f, 1.0f });

    void endBatch();
    void flush(); // 执行实际的绘制命令
    // 资源清理
    void cleanup();


private:
    void updateDrawMode();
private:

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
    RendererMode m_rendererMode;
	std::unique_ptr<IRenderer> m_renderer;
};