#include "Render/LineRenderer.hpp"
#include "Core/Shader.hpp"
#include <cstring>

LineRenderer::LineRenderer()  {}
LineRenderer::~LineRenderer() {
    if (m_vbo) glDeleteBuffers(1, &m_vbo);
    if (m_vao) glDeleteVertexArrays(1, &m_vao);
}

void LineRenderer::initGL() {
    if (m_initialized) return;
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    m_initialized = true;
}

void LineRenderer::setPolyline(const std::vector<glm::vec2>& pts, const glm::vec3& color) {
    m_vertices.clear();
    m_vertices.reserve(pts.size());
    for (auto &p : pts) m_vertices.push_back(Vertex{p, color});
    initGL();
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), m_vertices.data(), GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
    glBindVertexArray(0);
}

void LineRenderer::clear() {
    m_vertices.clear();
    if (m_initialized) {
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);
    }
}

void LineRenderer::draw() {
    if (m_vertices.empty()) return;
    if (m_shader) m_shader->use();
    glBindVertexArray(m_vao);
    glDrawArrays(GL_LINE_STRIP, 0, static_cast<GLsizei>(m_vertices.size()));
    glBindVertexArray(0);
}

void LineRenderer::update()
{
}
