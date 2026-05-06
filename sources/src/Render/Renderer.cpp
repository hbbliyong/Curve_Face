#include "Render/Renderer.hpp"

#include "Render/PointRenderer.hpp"
#include "Render/LineRenderer.hpp"
#include "Render/SurfaceRenderer.hpp"
#include "Render/BezierCurveRenderer.h"
#include <iostream>
#include <QDebug>
#include<Core/Shader.hpp>
Renderer::Renderer()
{
	m_initialized = false;
	m_viewportWidth = 800;
	m_viewportHeight = 600;
	m_rendererMode = RendererMode::CLEAR;

}

Renderer::~Renderer()
{
	cleanup();
}

bool Renderer::initialize()
{
	if (m_initialized) return true;

	if (!gladLoadGL()) {
		qDebug() << "Failed to initialize GLAD in Renderer";
		return false;
	}

	std::cout << "OpenGL Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "OpenGL Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

	glClearColor(0.2f, 0.3f, 0.4f, 1.0f);

	m_shader = std::make_shared<Shader>("assets/shaders/Base.vert", "assets/shaders/Base.frag");

	// 创建并初始化子渲染器
	m_renderer = std::make_unique<PointRenderer>();
	m_renderer->setShader(m_shader.get());
	m_renderer->initGL();

	m_initialized = true;
	qDebug() << "Renderer initialized successfully (split renderers)";
	return true;
}

void Renderer::render()
{
	if (!m_initialized) return;
	glClear(GL_COLOR_BUFFER_BIT);
	m_renderer->draw();
}

void Renderer::resize(int width, int height)
{
	m_viewportWidth = width;
	m_viewportHeight = height;
	glViewport(0, 0, width, height);
}

void Renderer::setPoints(const std::vector<glm::vec2>& points)
{}

void Renderer::setCurrentPoint(const glm::vec2& point)
{}

void Renderer::setDrawingMode(RendererMode mode)
{
	m_rendererMode = mode;
	updateDrawMode();
}

void Renderer::beginBatch()
{
	// 每一帧开始前，清空上一帧的数据
	cleanup();
}

void Renderer::addPoint(const glm::vec2& position, const glm::vec3& color)
{
	m_pointVertices.emplace_back(glm::vec3(position, 0.0f), color);
}

//void Renderer::addLine(const glm::vec2& start, const glm::vec2& end, const glm::vec3& color)
//{
//    // 从当前批次点构建控制点集合（原先用 m_pointVertices）
//    std::vector<glm::vec2> controlPoints;
//    controlPoints.reserve(m_pointVertices.size());
//    std::transform(m_pointVertices.begin(), m_pointVertices.end(), std::back_inserter(controlPoints),
//        [](const Vertex& v) { return v.pos; });
//
//    // 如果传入的 start/end 应作为控制点的一部分，按需插入：
//    // controlPoints.insert(controlPoints.begin(), start);
//    // controlPoints.push_back(end);
//
//    // 用 BezierCurve 类生成采样点（将贝塞尔逻辑封装到数学类）
//    BezierCurve bez(controlPoints);
//    auto bezierPoints = bez.sample(100); // 采样数可暴露为参数
//
//    // 把采样点转换为渲染用顶点
//    m_lineVertices.clear();
//    m_lineVertices.reserve(bezierPoints.size());
//    for (const auto& p : bezierPoints) {
//        m_lineVertices.push_back(Vertex{ p, color });
//    }
//}

void Renderer::endBatch()
{
	// 所有图元添加完毕，上传数据到GPU并绘制
	flush();
}

void Renderer::flush()
{
	//m_renderer->setDatas(m_pointVertices);
	m_renderer->draw();
}

void Renderer::cleanup()
{
	m_pointVertices.clear();
	m_initialized = false;
}


void Renderer::updateDrawMode()
{
	cleanup();
	// 根据模式委托给子渲染器
	switch (m_rendererMode) {
	case RendererMode::DRAW_POINT:
		m_renderer = std::make_unique<PointRenderer>();
		break;
	case RendererMode::DRAW_LINE:
	case RendererMode::DRAW_POLYLINE:
		m_renderer = std::make_unique<LineRenderer>();
		break;
	case RendererMode::DRAW_BERIZE_CURVE:
		m_renderer = std::make_unique<BezierCurveRenderer>();
		break;
	case RendererMode::DRAW_BSPLINE_SURFACE:
	{
		m_renderer = std::make_unique<SurfaceRenderer>();
		//std::vector<float> datas = { 0, 0, 1000, 0, 0, 1000, 1000, 0, 1000, 1000, 0, 1000, 100, 0, 900, 100, 0, 900, 1000, 0, 900, 1000, 0, 900, 100, 100, 900, 100, 100, 900, 1000, 100, 900, 1000, 100, 900, 0, 100, 1000, 0, 100, 1000, 1000, 100, 1000, 1000, 100, 1000, 0, 0, 1000, 100.00000000000001, 0, 900, 100.00000000000001, 100, 900, 0, 100, 1000, 1000, 0, 1000, 1000, 0, 900, 1000, 100, 900, 1000, 100, 1000 };
		const std::vector<float> datas = {
			// 前表面
			-1.0f, -1.0f,  1.0f,  // 0: 左下前
			 1.0f, -1.0f,  1.0f,  // 1: 右下前
			 1.0f,  1.0f,  1.0f,  // 2: 右上前
			-1.0f,  1.0f,  1.0f,  // 3: 左上前

			// 后表面
			-1.0f, -1.0f, -1.0f,  // 4: 左下后
			 1.0f, -1.0f, -1.0f,  // 5: 右下后
			 1.0f,  1.0f, -1.0f,  // 6: 右上后
			-1.0f,  1.0f, -1.0f   // 7: 左上后
		};
		std::vector<glm::vec3> m_surfaceVertices(datas.size() / 3);
		for (size_t i = 0; i < datas.size() / 3; i++)
		{
			m_surfaceVertices[i] = glm::vec3(datas[i * 3], datas[i * 3 + 1], datas[i * 3 + 2]);
		}

		//std::vector<unsigned int> m_surfaceIndices = { 4, 6, 3, 4, 3, 1, 8, 10, 7, 8, 7, 5, 12, 14, 11, 12, 11, 9, 0, 2, 15, 0, 15, 13, 18, 16, 17, 16, 18, 19, 22, 20, 21, 20, 22, 23 };
		const std::vector<uint32_t> m_surfaceIndices = {
			// 前表面
			0, 1, 2,   0, 2, 3,
			// 右表面
			1, 5, 6,   1, 6, 2,
			// 后表面
			5, 4, 7,   5, 7, 6,
			// 左表面
			4, 0, 3,   4, 3, 7,
			// 上表面
			3, 2, 6,   3, 6, 7,
			// 下表面
			4, 5, 1,   4, 1, 0
		};
		dynamic_cast<SurfaceRenderer*>(m_renderer.get())->setMesh(m_surfaceVertices, m_surfaceIndices);
	}
	break;
	/* case RendererMode::DRAW_BSPLINE_CURVE:

case RendererMode::DRAW_BERIZE_SURFACE:

case RendererMode::DRAW_POLYLINE: */

	default:
		m_renderer = std::make_unique<PointRenderer>();
		break;
	}
	m_renderer->setShader(m_shader.get());
}

