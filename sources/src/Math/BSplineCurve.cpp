#include "Math/BSPlineCurve.hpp"

BSplineCurve::BSplineCurve(std::vector<glm::vec2>& points, int k):
	control_points(points),degree(k){
		//初始化节点向量m=n+k+1,m节点数量,n是控制点数量，k为阶数
		int m = control_points.size() + degree + 1;
		for (size_t i = 0; i < m; i++)
		{
			if (i < degree) {
				knot_vector.push_back(0);
			}
			else if (i < control_points.size()) {
				knot_vector.push_back((i - degree) / (float)(control_points.size() - degree));
			}
			else {
				knot_vector.push_back(1);
			}
		}
	}

	float BSplineCurve::BasicFunction(int i, int k, float u) {
		if (k == 0) {
			if (u >= knot_vector[i] && u < knot_vector[i + 1]) {
				return 1;
			}
			else {
				return 0;
			}
		}

		float a = 0, b = 0;
		if (knot_vector[i + k] - knot_vector[i] != 0) {
			a = (u - knot_vector[i]) / float(knot_vector[i + k] - knot_vector[i]);
		}
		if (knot_vector[i + k + 1] - knot_vector[i + 1] != 0) {
			b = (knot_vector[i + k + 1] - u) / float(knot_vector[i + k + 1] - knot_vector[i + 1]);
		}
		return a * BasicFunction(i, k - 1, u) + b * BasicFunction(i + 1, k - 1, u);
	}

	glm::vec2 BSplineCurve::calcuatePoint(float u) {
		glm::vec2 res(0, 0);

		for (size_t i = 0; i < control_points.size(); i++)
		{
			float basis = BasicFunction(i, degree, u);
			res+= control_points[i] * basis;
		}
		return res;
	}

	void BSplineCurve::draw() {

	}