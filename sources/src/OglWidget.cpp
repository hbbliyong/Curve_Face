#include "OglWidget.hpp"
#include "Renderer.hpp"

OglWidget::OglWidget(QWidget* parent) :
    QOpenGLWidget(parent)
{

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
    m_renderer->render();

}

void OglWidget::resizeGL(int w, int h) {
    m_renderer->resize(w, h);
}

void OglWidget::setRender(std::shared_ptr<Renderer> ptr) {
   // render = ptr;
}