#include "Render/Renderer.hpp"
#include "Core/Shader.hpp"

// 引入新子渲染器实现
#include "Render/PointRenderer.hpp"
#include "Render/LineRenderer.hpp"
#include "Render/SurfaceRenderer.hpp"
#include "Math/BezierCurve.hpp"
#include <iostream>
#include <QDebug>

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

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

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
{
}

void Renderer::setLines(const std::vector<Line>& lines)
{
}

void Renderer::setCurrentPoint(const glm::vec2& point)
{
}

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
    m_pointVertices.emplace_back(position,color);
}

void Renderer::addLine(const glm::vec2& start, const glm::vec2& end, const glm::vec3& color)
{
    // 从当前批次点构建控制点集合（原先用 m_pointVertices）
    std::vector<glm::vec2> controlPoints;
    controlPoints.reserve(m_pointVertices.size());
    std::transform(m_pointVertices.begin(), m_pointVertices.end(), std::back_inserter(controlPoints),
        [](const Vertex& v) { return v.pos; });

    // 如果传入的 start/end 应作为控制点的一部分，按需插入：
    // controlPoints.insert(controlPoints.begin(), start);
    // controlPoints.push_back(end);

    // 用 BezierCurve 类生成采样点（将贝塞尔逻辑封装到数学类）
    BezierCurve bez(controlPoints);
    auto bezierPoints = bez.sample(100); // 采样数可暴露为参数

    // 把采样点转换为渲染用顶点
    m_lineVertices.clear();
    m_lineVertices.reserve(bezierPoints.size());
    for (const auto& p : bezierPoints) {
        m_lineVertices.push_back(Vertex{ p, color });
    }
}

void Renderer::endBatch()
{
    // 所有图元添加完毕，上传数据到GPU并绘制
    flush();
}

void Renderer::flush()
{
    m_renderer->setDatas(m_pointVertices);
	m_renderer->draw();
}

void Renderer::cleanup()
{
    m_pointVertices.clear();
    m_initialized = false;
}
//de Casteljau算法
std::vector<glm::vec2> Renderer::generatorBezierByIterative(std::vector<glm::vec2>& controlPoints, int segmentCount)
{
    if (controlPoints.size() < 2) {
        return std::vector<glm::vec2>(); // 至少需要2个控制点
    }

    if (segmentCount <= 0) {
        // 动态计算合适的段数：基于控制点数量和曲线复杂度
        segmentCount = static_cast<int>(controlPoints.size() * 10);
    }

    std::vector<glm::vec2> points;
    points.reserve(segmentCount + 1); // 预分配空间

    // 使用迭代de Casteljau算法替代递归[2,4](@ref)
    std::vector<glm::vec2> tempPoints = controlPoints;

    for (size_t i = 0; i <=segmentCount; i++)
    {
        float t = static_cast<float>(i) / segmentCount;

        // 复制控制点到临时数组
        tempPoints = controlPoints;

        for (size_t j = 1; j < controlPoints.size(); j++)
        {
            for (size_t k = 0; k < controlPoints.size()-j; k++)
            {
                tempPoints[k] = tempPoints[k] * (1.0f - t) + tempPoints[k + 1] * t;
            }
        }
        points.push_back(tempPoints[0]);
    }
    return points;
}

void Renderer::updateDrawMode()
{
	cleanup();
    // 根据模式委托给子渲染器
    switch (m_rendererMode) {
    case RendererMode::DRAW_POINT:
        m_renderer=std::make_unique<PointRenderer>();
        break;
    case RendererMode::DRAW_LINE:
        m_renderer = std::make_unique<LineRenderer>();
		break;
    /*case RendererMode::DRAW_POLYLINE:
    case RendererMode::DRAW_BERIZE_CURVE:
    case RendererMode::DRAW_BSPLINE_CURVE:
       
    case RendererMode::DRAW_BERIZE_SURFACE:
    case RendererMode::DRAW_BSPLINE_SURFACE:
    case RendererMode::DRAW_POLYLINE: */
        
    default:
        m_renderer = std::make_unique<PointRenderer>();
        break;
    }
	m_renderer->setShader(m_shader.get());
}

std::vector<glm::vec2> Renderer::generatorBezierByRecursion(std::vector<glm::vec2>& controlPoints, int segmentCount)
{
    if (controlPoints.size() < 3)return std::vector<glm::vec2>();
    int divCount = 100;
    float span = 1.0f / divCount;
    std::vector<glm::vec2> points(divCount);
    std::function<glm::vec2(std::vector<glm::vec2>&,float)> computerBezierPoint = [&computerBezierPoint](std::vector<glm::vec2>& cps, float t)->glm::vec2 {
        if (cps.size() == 1)return cps[0];
        std::vector<glm::vec2> newCps(cps.size() - 1);
        for (size_t i = 0; i < cps.size()-1; i++)
        {
            newCps[i] = cps[i] * (1 - t) + cps[i + 1] * t;
        }
      return  computerBezierPoint(newCps, t);
        };
    for (size_t i = 0; i < divCount; i++)
    {
        points[i]=computerBezierPoint(controlPoints, i * span);
    }
    return points;
}

//计算弧长
std::vector<float> precomputeArcLengths(const std::vector<glm::vec2>& points) {
    std::vector<float> arcLengths(points.size());
    arcLengths[0] = 0.0f;

    for (size_t i = 1; i < points.size(); ++i) {
        float segmentLength = glm::length(points[i] - points[i - 1]);
        arcLengths[i] = arcLengths[i - 1] + segmentLength;
    }

    return arcLengths;
}
// 预计算组合数
std::vector<float> precomputeBinomials(int n) {
    std::vector<float> binom(n + 1);
    binom[0] = 1.0f;
    for (int i = 1; i <= n; ++i) {
        binom[i] = binom[i - 1] * (n - i + 1) / i;
    }
    return binom;
}

// 使用伯恩斯坦基函数的实现:如果你需要处理高阶贝塞尔曲线或大量计算，可以考虑以下进阶优化
std::vector<glm::vec2> generatorBezierBernstein(const std::vector<glm::vec2>& controlPoints, int segmentCount) {
    size_t n = controlPoints.size() - 1;
    auto binoms = precomputeBinomials(n);

    std::vector<glm::vec2> points;
    points.reserve(segmentCount + 1);

    for (int i = 0; i <= segmentCount; ++i) {
        float t = static_cast<float>(i) / segmentCount;
        glm::vec2 point(0.0f);

        for (size_t j = 0; j <= n; ++j) {
            float basis = binoms[j] * std::pow(t, j) * std::pow(1 - t, n - j);
            point += controlPoints[j] * basis;
        }

        points.push_back(point);
    }

    return points;
}