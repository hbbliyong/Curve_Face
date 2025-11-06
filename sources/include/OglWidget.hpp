#pragma once
#include "Render/Renderer.hpp"
#include "RendererMode.hpp"
#include  <QOpenGLWidget>
#include <QMouseEvent>
#include <glm/glm.hpp>

class OglWidget :public QOpenGLWidget {
	Q_OBJECT
public:
	explicit OglWidget(QWidget* parent = nullptr);
	~OglWidget();

	void clearAll();          // 
	void setDrawingMode(RendererMode mode); //
signals:
	void pointSelected(const QPointF& point);

protected:
	void initializeGL() override;
	void paintGL() override;
	void resizeGL(int w, int h) override;

	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
private:

	glm::vec2 windowToOpenGL(const QPoint& windowPoint);

	void drawPreviewLine(); 

private:
	std::shared_ptr<Renderer> m_renderer;

	std::vector<glm::vec2> m_points;     
	std::vector<Line> m_lines;      
	glm::vec2 m_currentPoint;         
	bool m_hasStartPoint;          
	bool m_isDrawing;               
	bool m_continuousMode;         

	QPointF windowToNDC(const QPoint& windowPoint);
	glm::vec2 qtPointToRendererPoint(const QPointF& qtPoint);

	std::vector<QPointF> m_qtPoints;     
	std::vector<QLineF> m_qtLines;       
	QPointF m_currentQtPoint;            
};