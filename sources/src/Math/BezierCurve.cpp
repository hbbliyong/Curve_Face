#include "Math/BezierCurve.hpp"

#include <algorithm>

BezierCurve::BezierCurve(const std::vector<glm::vec2> &controlPoints)
    : m_controlPoints(controlPoints) {}

void BezierCurve::setControlPoints(const std::vector<glm::vec2> &controlPoints)
{
    m_controlPoints = controlPoints;
}

std::vector<glm::vec2> BezierCurve::sample(int segmentCount) const
{
    return BezierCurve::sample(m_controlPoints, segmentCount);
}

std::vector<glm::vec2> BezierCurve::sample(const std::vector<glm::vec2> &controlPoints, int segmentCount)
{
    std::vector<glm::vec2> result;
    if (controlPoints.empty() || segmentCount <= 0)
        return result;

    // ��ֻ��һ�����Ƶ㣬ֱ�ӷ���
    if (controlPoints.size() == 1)
    {
        result.push_back(controlPoints.front());
        return result;
    }

    result.reserve(segmentCount + 1);
    std::vector<glm::vec2> temp;

    for (int i = 0; i <= segmentCount; ++i)
    {
        float t = static_cast<float>(i) / static_cast<float>(segmentCount);
        // ���� de Casteljau
        temp = controlPoints;
        for (size_t level = 1; level < controlPoints.size(); ++level)
        {
            for (size_t j = 0; j + level < controlPoints.size(); ++j)
            {
                temp[j] = temp[j] * (1.0f - t) + temp[j + 1] * t;
            }
        }
        result.push_back(temp.front());
    }
    return result;
}

std::vector<glm::vec2> BezierCurve::generatorBezierByRecursion(std::vector<glm::vec2>& controlPoints, int segmentCount)
{
    if (controlPoints.size() < 3)return std::vector<glm::vec2>();
    int divCount = 100;
    float span = 1.0f / divCount;
    std::vector<glm::vec2> points(divCount);
    std::function<glm::vec2(std::vector<glm::vec2>&, float)> computerBezierPoint = [&computerBezierPoint](std::vector<glm::vec2>& cps, float t)->glm::vec2 {
        if (cps.size() == 1)return cps[0];
        std::vector<glm::vec2> newCps(cps.size() - 1);
        for (size_t i = 0; i < cps.size() - 1; i++)
        {
            newCps[i] = cps[i] * (1 - t) + cps[i + 1] * t;
        }
        return  computerBezierPoint(newCps, t);
        };
    for (size_t i = 0; i < divCount; i++)
    {
        points[i] = computerBezierPoint(controlPoints, i * span);
    }
    return points;
}

//de Casteljau算法
std::vector<glm::vec2> BezierCurve::generatorBezierByIterative(std::vector<glm::vec2>& controlPoints, int segmentCount)
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

    for (size_t i = 0; i <= segmentCount; i++)
    {
        float t = static_cast<float>(i) / segmentCount;

        // 复制控制点到临时数组
        tempPoints = controlPoints;

        for (size_t j = 1; j < controlPoints.size(); j++)
        {
            for (size_t k = 0; k < controlPoints.size() - j; k++)
            {
                tempPoints[k] = tempPoints[k] * (1.0f - t) + tempPoints[k + 1] * t;
            }
        }
        points.push_back(tempPoints[0]);
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