#include <QApplication>
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv); // 初始化应用程序对象，管理整个应用的生命周期

    MainWindow window; // 创建主窗口实例
    window.setWindowTitle("Qt MainWindow 示例"); // 设置窗口标题
    window.resize(800, 600); // 设置初始窗口大小
    window.show(); // 显示窗口

    return app.exec(); // 进入主事件循环，等待用户操作
}