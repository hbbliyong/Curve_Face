#pragma once
#include "Renderer.hpp"
#include  <QOpenGLWidget>
#include <QMouseEvent>
#include <glm/glm.hpp>



class OglWidget :public QOpenGLWidget {
	Q_OBJECT
public:
	explicit OglWidget(QWidget* parent = nullptr);
	~OglWidget();

	void setRender(std::shared_ptr<Renderer> ptr);

	// 控制函数
	void clearAll();          // 清除所有点和线
	void setDrawingMode(bool continuous); // 设置绘制模式
signals:
	void pointSelected(const QPointF& point);
	//void lineDrawn(const QPointF& start, const QPointF& end);

protected:
	void initializeGL() override;
	void paintGL() override;
	void resizeGL(int w, int h) override;

	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
private:
	// 坐标转换：窗口坐标 → OpenGL归一化坐标
	glm::vec2 windowToOpenGL(const QPoint& windowPoint);

	// 绘制辅助函数
	void drawPoint(const glm::vec2& p, float size = 5.0f);
	void drawLine(const glm::vec2& start, const glm::vec2& end, float width = 2.0f);
	void drawPreviewLine(); // 绘制实时预览线

private:
	std::shared_ptr<Renderer> m_renderer;

	// 绘制数据
	std::vector<glm::vec2> m_points;     // 所有已确定的点
	std::vector<Line> m_lines;       // 所有已确定的线段
	glm::vec2 m_currentPoint;           // 当前鼠标位置（用于预览）
	bool m_hasStartPoint;           // 是否已选择起点
	bool m_isDrawing;               // 是否正在绘制
	bool m_continuousMode;         // 连续绘制模式

	// 坐标转换
	QPointF windowToNDC(const QPoint& windowPoint);
	glm::vec2 qtPointToRendererPoint(const QPointF& qtPoint);

	// 数据管理
	void updateRendererData();

	std::vector<QPointF> m_qtPoints;     // 存储Qt坐标点
	std::vector<QLineF> m_qtLines;       // 存储Qt坐标线段
	QPointF m_currentQtPoint;            // 当前鼠标位置（Qt坐标）

};