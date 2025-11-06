#pragma once
#include "IRenderer.hpp"
#include <glad/glad.h>
#include <vector>
#include "Math/BezierCurve.hpp"
class Shader;
class BezierCurveRenderer : public IRenderer {
	 public:
		BezierCurveRenderer();
		virtual ~BezierCurveRenderer() override;
		virtual void initGL() override;
		virtual void setShader(Shader* shader) override { m_shader = shader; }
		virtual void setControlPoints(const std::vector<glm::vec2>& controlPoints);
		virtual void clear() override;
		virtual void draw() override;
private:
	virtual void update() override;
private:
	Shader* m_shader{ nullptr };
	GLuint m_vao{ 0 }, m_vbo{ 0 };
	bool m_initialized{ false };
	std::vector<Vertex> m_drawDatas;
	BezierCurve m_berizeCrve;
};
