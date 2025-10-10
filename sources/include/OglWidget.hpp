#pragma once
#include "Renderer.hpp"
#include  <QOpenGLWidget>

class OglWidget :public QOpenGLWidget {
	Q_OBJECT
public:
public:
    explicit OglWidget(QWidget* parent = nullptr);
    ~OglWidget();

    void setRender(std::shared_ptr<Renderer> ptr);


protected:
 virtual   void initializeGL() override;
   virtual void paintGL() override;
   virtual void resizeGL(int w, int h) override;


private:
    std::shared_ptr<Renderer> m_renderer; 
};