#include "OglWidget.hpp"
#include "Renderer.hpp"

OglWidget::OglWidget(QWidget* parent) :
    QOpenGLWidget(parent), m_hasStartPoint(false), m_isDrawing(false),
    m_continuousMode(false)
{
    //启用鼠标跟踪，实时获取鼠标位置
    setMouseTracking(true);
}

OglWidget::~OglWidget()
{

}

void OglWidget::initializeGL()
{
       // 创建渲染器实例
    m_renderer = std::make_shared<Renderer>();
    
    // 初始化渲染器
    if (!m_renderer->initialize()) {
        qDebug() << "Failed to initialize renderer";
        return;
    }
    
    qDebug() << "MyOpenGLWidget initialized successfully";
}

void OglWidget::paintGL()
{
  //  m_renderer->render();
    glClear(GL_COLOR_BUFFER_BIT);

    m_renderer->beginBatch(); // 开始新一帧的批次

    for (const auto& point : m_qtPoints) {
        glm::vec2 p = qtPointToRendererPoint(point);
        m_renderer->addPoint(glm::vec2(p.x, p.y), glm::vec3(0.0f, 1.0f, 0.0f));
    }

    // 将需要绘制的所有点和线添加到批处理器中
    for (const auto& line : m_qtLines) {
        // 转换坐标并添加到批次
        glm::vec2 start = qtPointToRendererPoint(line.p1());
        glm::vec2 end = qtPointToRendererPoint(line.p2());
        m_renderer->addLine(glm::vec2(start.x, start.y), glm::vec2(end.x, end.y), glm::vec3(1.0f, 1.0f, 1.0f));
    }
   

    m_renderer->endBatch(); // 结束批次，触发批量绘制
}

void OglWidget::resizeGL(int w, int h) {
    m_renderer->resize(w, h);
}

void OglWidget::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        QPointF ndcPoint = windowToNDC(event->pos());

        if (!m_hasStartPoint) {
            // 开始新线段
            m_qtPoints.push_back(ndcPoint);
            m_hasStartPoint = true;
            m_isDrawing = true;
          //  emit pointSelected(ndcPoint);
        }
        else {
            // 完成当前线段
            m_qtPoints.push_back(ndcPoint);
            if (m_qtPoints.size() >= 2) {
                QPointF start = m_qtPoints[m_qtPoints.size() - 2];
                QPointF end = m_qtPoints[m_qtPoints.size() - 1];
                m_qtLines.push_back(QLineF(start, end));
                //emit lineDrawn(start, end);
            }

            if (!m_continuousMode) {
                m_hasStartPoint = false;
                m_isDrawing = false;
            }
        }

        update();
    }
}

void OglWidget::mouseMoveEvent(QMouseEvent* event)
{
}

void OglWidget::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::RightButton && m_isDrawing) {
        // 右键取消当前绘制
        if (m_continuousMode && !m_qtPoints.empty()) {
            m_qtPoints.pop_back();
        }
        m_hasStartPoint = false;
        m_isDrawing = false;
        update();
    }
}

glm::vec2 OglWidget::windowToOpenGL(const QPoint& windowPoint)
{
    return glm::vec2();
}

void OglWidget::drawPoint(const glm::vec2& p, float size)
{
}

void OglWidget::drawLine(const glm::vec2& start, const glm::vec2& end, float width)
{
}

void OglWidget::drawPreviewLine()
{
}

QPointF OglWidget::windowToNDC(const QPoint& windowPoint)
{
    // 将窗口坐标转换为归一化设备坐标 [-1, 1]
    float x = (2.0f * windowPoint.x()) / width() - 1.0f;
    float y = 1.0f - (2.0f * windowPoint.y()) / height(); // Y轴翻转
    return QPointF(x, y);
}

glm::vec2 OglWidget::qtPointToRendererPoint(const QPointF& qtPoint)
{
    return glm::vec2(static_cast<float>(qtPoint.x()),
        static_cast<float>(qtPoint.y()));
}

void OglWidget::updateRendererData()
{
}

void OglWidget::setRender(std::shared_ptr<Renderer> ptr) {
   // render = ptr;
}

void OglWidget::clearAll()
{
    m_qtPoints.clear();
    m_qtLines.clear();
    m_hasStartPoint = false;
    m_isDrawing = false;
    update();
}

void OglWidget::setDrawingMode(bool continuous)
{
    m_continuousMode = continuous;
    m_renderer->setDrawingMode(continuous);
}

//void OglWidget::lineDrawn(const QPointF& start, const QPointF& end)
//{
//}
