#pragma once

#include <QMainWindow>
#include <map>
// 前置声明，避免在头文件中包含大量头文件，加快编译速度
class QAction;
class QMenu;
class QToolBar;
class QTextEdit; // 作为中心部件的示例，后续可替换为您的OpenGL部件

class MainWindow : public QMainWindow
{
    Q_OBJECT // Qt 元对象系统宏，必须用于信号槽机制

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

    // 槽函数，用于响应菜单和工具栏的动作
private slots:
    void newFile();
    void openFile();
    void about();

    void clearAll();
    void drawPoint();
    void drawLine();
    void drawPolyline();
    void drawBerizeCurve();
    void drawBSplineCurve();
    void drawBerizeSurface();
    void drawBSplineSurface();
private:
    // 初始化函数
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();

    // 菜单栏
    QMenu* fileMenu;
    QMenu* helpMenu;

    // 工具栏
    QToolBar* fileToolBar;

    // 动作 (Actions)
    QAction* newAct;
    QAction* openAct;
    QAction* exitAct;
    QAction* aboutAct;

    std::map<std::string, QAction*> mDrawAction;
};