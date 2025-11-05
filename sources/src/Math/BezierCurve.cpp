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