#pragma once
#include <vector>
#include <glm/glm.hpp>

class BezierCurve {
public:
    BezierCurve() = default;
    BezierCurve(const std::vector<glm::vec2>& controlPoints);

    void setControlPoints(const std::vector<glm::vec2>& controlPoints);
    // 返回采样点（包含两端点），segmentCount >= 1 表示分段数
    std::vector<glm::vec2> sample(int segmentCount = 100) const;

    // 静态便捷接口
    static std::vector<glm::vec2> sample(const std::vector<glm::vec2>& controlPoints, int segmentCount = 100);
    std::vector<glm::vec2> generatorBezierByRecursion(std::vector<glm::vec2>& controlPoints, int segmentCount);
    std::vector<glm::vec2> generatorBezierByIterative(std::vector<glm::vec2>& controlPoints, int segmentCount);
private:
    std::vector<glm::vec2> m_controlPoints;
};