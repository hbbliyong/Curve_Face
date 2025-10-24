#pragma once
#include <vector>
#include <glm/glm.hpp>

//https://blog.csdn.net/m0_46465344/article/details/134538069
class BSplineCurve {
public:
	BSplineCurve(std::vector<glm::vec2>& points, int k);
	float BasicFunction(int i, int k, float u);
	
	glm::vec2 calcuatePoint(float u);
	void draw();
private:
	std::vector<glm::vec2> control_points;
	std::vector<float> knot_vector;
	int degree;
};