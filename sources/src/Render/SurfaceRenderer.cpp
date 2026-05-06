#include "Render/SurfaceRenderer.hpp"
#include "Core/Shader.hpp"
#include <cstring>
#include <ranges>
#include <iostream>
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
	m_vertices.reserve(vertices.size());
	auto x =
		vertices | std::ranges::views::transform([](const glm::vec3& v) { return Vertex{ .pos = v, .color = glm::vec3(1.0f) }; });
	std::ranges::copy(x, std::back_inserter(m_vertices));
	m_indices = indices;
	update();
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
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		std::cout << "GL Error: " << err << std::endl;
	}
	GLint eboBinding = 0;
	glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &eboBinding);
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_indices.size()), GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}

void SurfaceRenderer::update()
{
	if (!m_initialized)
		initGL();
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), m_vertices.data(), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), m_indices.data(), GL_DYNAMIC_DRAW);

	// 位置属性 (location = 0)
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));
	// 颜色属性 (location = 1) —— 如果 shader 使用 color，要启用并设置
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	glBindVertexArray(0);
}
