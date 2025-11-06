#include "Render/BezierCurveRenderer.h"
#include "Core/Shader.hpp"
#include <algorithm>
BezierCurveRenderer::BezierCurveRenderer()
{
}

BezierCurveRenderer::~BezierCurveRenderer()
{
    if (m_vbo) glDeleteBuffers(1, &m_vbo);
    if (m_vao) glDeleteVertexArrays(1, &m_vao);
}

void BezierCurveRenderer::setControlPoints(const std::vector<glm::vec2>& controlPoints)
{
    m_vertices.clear();
    m_vertices.reserve(controlPoints.size());
    for (auto& p : controlPoints) m_vertices.push_back(Vertex{  });
}


void BezierCurveRenderer::initGL() {
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


void BezierCurveRenderer::clear() {
    m_vertices.clear();
    if (m_initialized) {
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);
    }
}

void BezierCurveRenderer::draw() {
    if (m_drawDatas.empty()) return;
    if (m_shader) m_shader->use();
    glBindVertexArray(m_vao);
    glDrawArrays(GL_LINE_STRIP, 0, static_cast<GLsizei>(m_drawDatas.size()));
    glBindVertexArray(0);
}

void BezierCurveRenderer::update()
{
	if (m_vertices.size() < 3)return;
    initGL();
	std::vector<glm::vec2> controlPoints(m_vertices.size());
    std::transform(m_vertices.begin(), m_vertices.end(), controlPoints.begin(),
		[](const Vertex& v) { return v.pos; });
    m_berizeCrve.setControlPoints(controlPoints);

	std::vector<glm::vec2> bezierPoints = m_berizeCrve.sample(100);
	m_drawDatas.resize(bezierPoints.size());
    std::transform(bezierPoints.begin(), bezierPoints.end(), m_drawDatas.begin(),
        [](const glm::vec2& v) { return Vertex{v,glm::uvec3(0.1f,0.2f,0.3f)}; });

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_drawDatas.size() * sizeof(Vertex), m_drawDatas.data(), GL_DYNAMIC_DRAW);
    glBindVertexArray(0);
}