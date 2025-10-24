#include "Renderer.hpp"
#include <QDebug>
#include <iostream>
#include "Core/Shader.hpp"
#include <functional>
Renderer::Renderer()
    : m_VAO(0), m_VBO(0),
    m_initialized(false), m_viewportWidth(0), m_viewportHeight(0), m_rendererMode(RendererMode::DRAW_POINT)
{
   
}

Renderer::~Renderer()
{
    cleanup();
}

bool Renderer::initialize()
{
    if (m_initialized) {
        return true;
    }
    
    // 初始化GLAD
    if (!gladLoadGL()) {
        qDebug() << "Failed to initialize GLAD in Renderer";
        return false;
    }
    
    // 输出OpenGL信息
    std::cout << "OpenGL Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "OpenGL Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    
    // 设置初始OpenGL状态
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    
    // 定义三角形顶点数据
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };
    
    // 创建和配置VAO、VBO
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    glGenVertexArrays(1, &m_pointVAO);
    glGenBuffers(1, &m_pointVBO);

    glGenVertexArrays(1, &m_lineVAO);
    glGenBuffers(1, &m_lineVBO);
  //  m_shader = std::make_shared<Shader>("E:/001Code/002CurveAndFace/build/sources/Debug/assets/shaders/Base.vert", "E:/001Code/002CurveAndFace/build/sources/Debug/assets/shaders/Base.frag");
    m_shader = std::make_shared<Shader>("assets/shaders/Base.vert", "assets/shaders/Base.frag");
    
    m_initialized = true;
    qDebug() << "Renderer initialized successfully";
    return true;
}

void Renderer::render()
{
 /*   if (!m_initialized) {
        qDebug() << "Renderer not initialized!";
        return;
    }
    
    glClear(GL_COLOR_BUFFER_BIT);
    
    m_shader->use();
    glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);*/
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
}

void Renderer::beginBatch()
{
    // 每一帧开始前，清空上一帧的数据
    m_pointVertices.clear();
    m_lineVertices.clear();
}

void Renderer::addPoint(const glm::vec2& position, const glm::vec3& color)
{
    Vertex v;
    v.position = position;
    v.color = color;
    m_pointVertices.push_back(v);
}

void Renderer::addLine(const glm::vec2& start, const glm::vec2& end, const glm::vec3& color)
{
    // 一条线需要两个顶点
    //Vertex v1, v2;
    //v1.position = start; v1.color = color;
    //v2.position = end;   v2.color = color;
    //m_lineVertices.push_back(v1);
    //m_lineVertices.push_back(v2);
    m_lineVertices.clear();
    std::vector<glm::vec2> points;
    std::transform(m_pointVertices.begin(), m_pointVertices.end(), std::back_inserter(points), [](const Vertex& v) {return v.position; });
    
    auto berizePoints = generatorBezierByIterative(points,100);
    std::transform(berizePoints.begin(), berizePoints.end(), std::back_inserter(m_lineVertices), [&color](const glm::vec2& v) {return Vertex{ v,color }; });
}

void Renderer::endBatch()
{
    // 所有图元添加完毕，上传数据到GPU并绘制
    flush();
}

void Renderer::flush()
{
    m_shader->use();
    // 1. 绘制所有线段
    if (!m_lineVertices.empty()) {
        glBindVertexArray(m_lineVAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_lineVBO);
        // 将线段顶点数据一次性上传到GPU
        glBufferData(GL_ARRAY_BUFFER, m_lineVertices.size() * sizeof(Vertex), m_lineVertices.data(), GL_DYNAMIC_DRAW);

        // 位置属性
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        glEnableVertexAttribArray(0);
        // 颜色属性
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // 一次Draw Call绘制所有线段！参数：绘制模式，起始索引，顶点数量
        glDrawArrays(GL_LINE_STRIP, 0, m_lineVertices.size());
        glBindVertexArray(0);
    }

    // 2. 绘制所有点
    if (!m_pointVertices.empty()) {
        glPointSize(10);
        glBindVertexArray(m_pointVAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_pointVBO);
        glBufferData(GL_ARRAY_BUFFER, m_pointVertices.size() * sizeof(Vertex), m_pointVertices.data(), GL_DYNAMIC_DRAW);
        // 位置属性
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        glEnableVertexAttribArray(0);
        // 颜色属性
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glDrawArrays(GL_POINTS, 0, m_pointVertices.size());
        glBindVertexArray(0);
    }
}

void Renderer::cleanup()
{
    if (m_VAO) {
        glDeleteVertexArrays(1, &m_VAO);
        m_VAO = 0;
    }
    if (m_VBO) {
        glDeleteBuffers(1, &m_VBO);
        m_VBO = 0;
    }
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