#include "Render/PointRenderer.hpp"
#include "Core/Shader.hpp"
#include <cstring>

PointRenderer::PointRenderer()  {}

PointRenderer::~PointRenderer() {
    if (m_vbo) glDeleteBuffers(1, &m_vbo);
    if (m_vao) glDeleteVertexArrays(1, &m_vao);
}

void PointRenderer::initGL() {
    if (m_initialized) return;
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), m_vertices.data(), GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
    glBindVertexArray(0);

    m_initialized = true;
}

void PointRenderer::setPoints(const std::vector<glm::vec2>& pts) {
    m_vertices.clear();
    m_vertices.reserve(pts.size());
    for (auto &p : pts) m_vertices.push_back(Vertex{p, glm::vec3(0.0f,1.0f,0.0f)});  
    update();
}

void PointRenderer::addPoint(const glm::vec2& p, const glm::vec3& color) {
    m_vertices.push_back(Vertex{p, color});
    
    update();
}

void PointRenderer::clear() {
    m_vertices.clear();
    if (m_initialized) {
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);
    }
}

void PointRenderer::draw() {
    if (m_vertices.empty()) return;
    if (m_shader) m_shader->use();
    glBindVertexArray(m_vao);
    glPointSize(8);
    glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(m_vertices.size()));
    glBindVertexArray(0);
}

void PointRenderer::update()
{
    initGL();
    // 简单策略：重新上传全部点
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), m_vertices.data(), GL_DYNAMIC_DRAW);
    glBindVertexArray(0);
}
