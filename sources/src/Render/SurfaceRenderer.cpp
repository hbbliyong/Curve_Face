#include "Render/SurfaceRenderer.hpp"
#include "Core/Shader.hpp"
#include <cstring>

SurfaceRenderer::SurfaceRenderer() {}
SurfaceRenderer::~SurfaceRenderer() {
    if (m_ebo) glDeleteBuffers(1, &m_ebo);
    if (m_vbo) glDeleteBuffers(1, &m_vbo);
    if (m_vao) glDeleteVertexArrays(1, &m_vao);
}

void SurfaceRenderer::initGL() {
    if (m_initialized) return;
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);
    m_initialized = true;
}

void SurfaceRenderer::setMesh(const std::vector<glm::vec3>& vertices, const std::vector<unsigned int>& indices) {
   // m_vertices = vertices;
  //  m_indices = indices;
    initGL();
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(glm::vec3), m_vertices.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), m_indices.data(), GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glBindVertexArray(0);
}

void SurfaceRenderer::clear() {
    m_vertices.clear();
    m_indices.clear();
    if (m_initialized) {
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);
    }
}

void SurfaceRenderer::draw() {
    if (m_indices.empty() || m_vertices.empty()) return;
    if (m_shader) m_shader->use();
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void SurfaceRenderer::update()
{
}
