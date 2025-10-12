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

    setCentralWidget(new OglWidget(this)); 

    createActions(); 
    createMenus();   
    createToolBars(); 
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


    openAct = new QAction(tr("open"), this);
    openAct->setShortcut(tr("Ctrl+O"));
    openAct->setStatusTip(tr("opinging"));
    connect(openAct, &QAction::triggered, this, &MainWindow::openFile);

   
    exitAct = new QAction(tr("eqit"), this);
    exitAct->setShortcut(tr("Ctrl+Q"));
    exitAct->setStatusTip(tr("......"));
    connect(exitAct, &QAction::triggered, this, &QWidget::close); 

    
    aboutAct = new QAction(tr("about"), this);
    aboutAct->setStatusTip(tr("about ...."));
    connect(aboutAct, &QAction::triggered, this, &MainWindow::about);
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("open"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addSeparator(); 
    fileMenu->addAction(exitAct);

    
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