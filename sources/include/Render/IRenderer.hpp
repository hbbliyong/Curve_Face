#pragma once

// 通用渲染器接口，供 Point/Line/Surface 等子渲染器实现
// 目标：统一生命周期与基本操作，便于在上层统一管理与替换
#include<glm/glm.hpp>
class Shader;
struct Vertex {
    glm::vec2 pos;
    glm::vec3 color; // 可选，增加颜色属性
};

class IRenderer {
public:
    IRenderer() =default;
    virtual ~IRenderer() = default;

    // 在有效的 OpenGL 上下文中初始化所需的 GL 对象（VAO/VBO/EBO 等）
    virtual void initGL() = 0;

    // 绑定/切换着色器
    virtual void setShader(Shader* shader) = 0;
    virtual void setDatas(std::vector<Vertex>& vertices) {
		m_vertices = vertices;
		update();
    };

    // 清空内部数据（顶点缓冲等），保持对象可复用
    virtual void clear() = 0;
    virtual void update() = 0;
    // 执行实际绘制
    virtual void draw() = 0;

    // 可选：子类可覆盖以清理额外资源（默认空实现）
    virtual void cleanup() {}
protected:
	std::vector<Vertex> m_vertices;
};