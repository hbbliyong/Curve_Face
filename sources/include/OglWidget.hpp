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

	void setRender(std::shared_ptr<Renderer> ptr);


	void clearAll();          // 
	void setDrawingMode(RendererMode mode); //
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
	// ����ת������������ �� OpenGL��һ������
	glm::vec2 windowToOpenGL(const QPoint& windowPoint);

	// ���Ƹ�������
	void drawPoint(const glm::vec2& p, float size = 5.0f);
	void drawLine(const glm::vec2& start, const glm::vec2& end, float width = 2.0f);
	void drawPreviewLine(); // ����ʵʱԤ����

private:
	std::shared_ptr<Renderer> m_renderer;

	// ��������
	std::vector<glm::vec2> m_points;     // ������ȷ���ĵ�
	std::vector<Line> m_lines;       // ������ȷ�����߶�
	glm::vec2 m_currentPoint;           // ��ǰ���λ�ã�����Ԥ����
	bool m_hasStartPoint;           // �Ƿ���ѡ�����
	bool m_isDrawing;               // �Ƿ����ڻ���
	bool m_continuousMode;         // ��������ģʽ

	// ����ת��
	QPointF windowToNDC(const QPoint& windowPoint);
	glm::vec2 qtPointToRendererPoint(const QPointF& qtPoint);

	// ���ݹ���
	void updateRendererData();

	std::vector<QPointF> m_qtPoints;     // �洢Qt�����
	std::vector<QLineF> m_qtLines;       // �洢Qt�����߶�
	QPointF m_currentQtPoint;            // ��ǰ���λ�ã�Qt���꣩

};