#include "MainWindow.h"
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QTextEdit>
#include <QMessageBox>
#include <QFileDialog>
#include "OglWidget.hpp"
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    mDrawWidget = new OglWidget(this);
    setCentralWidget(mDrawWidget);

    createActions(); 
    createMenus();   
   // createToolBars(); 
    createStatusBar(); 
 
    statusBar()->showMessage(tr("hello world"), 2000);
}

MainWindow::~MainWindow()
{
    
}

void MainWindow::createActions()
{  
    newAct = new QAction(tr("New"), this);
    newAct->setShortcut(tr("Ctrl+N")); 
    newAct->setStatusTip(tr("new ....")); 
    connect(newAct, &QAction::triggered, this, &MainWindow::newFile); 


    openAct = new QAction(tr("Open"), this);
    openAct->setShortcut(tr("Ctrl+O"));
    openAct->setStatusTip(tr("opinging"));
    connect(openAct, &QAction::triggered, this, &MainWindow::openFile);

   
    exitAct = new QAction(tr("Eqit"), this);
    exitAct->setShortcut(tr("Ctrl+Q"));
    exitAct->setStatusTip(tr("......"));
    connect(exitAct, &QAction::triggered, this, &QWidget::close); 

    
    aboutAct = new QAction(tr("about"), this);
    aboutAct->setStatusTip(tr("about ...."));
    connect(aboutAct, &QAction::triggered, this, &MainWindow::about);

    auto clearPointAct = new QAction("Clear", this);
    connect(clearPointAct, &QAction::triggered, this, &MainWindow::clearAll);

    auto drawPointAct = new QAction("Draw Point", this);
    connect(drawPointAct, &QAction::triggered, this, &MainWindow::drawPoint);

    auto drawLineAct = new QAction("Draw Line", this);
    connect(drawLineAct, &QAction::triggered, this, &MainWindow::drawLine);

    auto drawPolylineAct = new QAction("Draw Polyline", this);
    connect(drawPolylineAct, &QAction::triggered, this, &MainWindow::drawPolyline);


    auto drawBerizeCurveAct = new QAction("Draw Berize Curve", this);
    connect(drawBerizeCurveAct, &QAction::triggered, this, &MainWindow::drawBerizeCurve);

    auto drawBSplineCurveAct = new QAction("Draw BSpline Curve", this);
    connect(drawBSplineCurveAct, &QAction::triggered, this, &MainWindow::drawBSplineCurve);

    auto drawBerizeSurfaceAct = new QAction("Draw Berize Surface", this);
    connect(drawBerizeSurfaceAct, &QAction::triggered, this, &MainWindow::drawBerizeSurface);

    auto drawBSplineSurfaceAct = new QAction("Draw BSpline Surface", this);
    connect(drawBSplineSurfaceAct, &QAction::triggered, this, &MainWindow::drawBSplineSurface);

    mDrawAction.emplace("Clear", clearPointAct);
    mDrawAction.emplace("DrawPoint", drawPointAct);
    mDrawAction.emplace("DrawLine", drawLineAct);
    mDrawAction.emplace("DrawPolyline", drawPolylineAct);
    mDrawAction.emplace("DrawBerizeCurve", drawBerizeCurveAct);
    mDrawAction.emplace("DrawBSplineCurve", drawBSplineCurveAct);
    mDrawAction.emplace("DrawBerizeSurface", drawBerizeSurfaceAct);
    mDrawAction.emplace("DrawBSplineSurface", drawBSplineSurfaceAct);
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu("File");
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addSeparator(); 
    fileMenu->addAction(exitAct);

    auto clearMenu = menuBar()->addMenu("Clear");
    clearMenu->addAction(mDrawAction["Clear"]);

    auto drawPointMenu = menuBar()->addMenu("Draw Point");
    drawPointMenu->addAction(mDrawAction["DrawPoint"]);

    auto drawLineMenu = menuBar()->addMenu("Draw Line");
    drawLineMenu->addAction(mDrawAction["DrawLine"]);
    drawLineMenu->addAction(mDrawAction["DrawPolyline"]);

    auto drawArcMenu = menuBar()->addMenu("Draw Arc");
    drawArcMenu->addAction(mDrawAction["DrawBerizeCurve"]);
    drawArcMenu->addAction(mDrawAction["DrawBSplineCurve"]);

    auto drawSurfaceMenu = menuBar()->addMenu("Draw Surface");
    drawSurfaceMenu->addAction(mDrawAction["DrawBerizeSurface"]);
    drawSurfaceMenu->addAction(mDrawAction["DrawBSplineSurface"]);

    helpMenu = menuBar()->addMenu(tr("about"));
    helpMenu->addAction(aboutAct);
}

void MainWindow::createToolBars()
{ 
    fileToolBar = addToolBar(tr("createToolBars"));
    fileToolBar->addAction(newAct);
    fileToolBar->addAction(openAct);
}

void MainWindow::createStatusBar()
{  
    statusBar()->showMessage(tr("createStatusBar"));
}


void MainWindow::newFile()
{
    statusBar()->showMessage(tr("new file"), 2000);
}

void MainWindow::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("hello.txt"));
    if (!fileName.isEmpty()) {
       
        statusBar()->showMessage(tr("selected file: %1").arg(fileName), 2000);
    }
}

void MainWindow::about()
{
    QMessageBox::about(this, "about",
        "about");
}

void MainWindow::clearAll()
{
    mDrawWidget->clearAll();
}

void MainWindow::drawPoint()
{
    mDrawWidget->setDrawingMode(RendererMode::DRAW_POINT);
}

void MainWindow::drawLine()
{
    mDrawWidget->setDrawingMode(RendererMode::DRAW_LINE);
}

void MainWindow::drawPolyline()
{
    mDrawWidget->setDrawingMode(RendererMode::DRAW_POINT);
}

void MainWindow::drawBerizeCurve()
{
    mDrawWidget->setDrawingMode(RendererMode::DRAW_BERIZE_CURVE);
}

void MainWindow::drawBSplineCurve()
{
    mDrawWidget->setDrawingMode(RendererMode::DRAW_BSPLINE_CURVE);
}

void MainWindow::drawBerizeSurface()
{
    mDrawWidget->setDrawingMode(RendererMode::DRAW_BERIZE_SURFACE);
}

void MainWindow::drawBSplineSurface()
{
    mDrawWidget->setDrawingMode(RendererMode::DRAW_BSPLINE_SURFACE);
}
