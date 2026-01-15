#include "MainWindow.h"
#include <QApplication>
#include <QStyleFactory>
#include <QDir>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // 设置应用程序信息
    app.setApplicationName("学生成绩管理系统");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("Qt课程设计");
    
    // 设置样式
    app.setStyle(QStyleFactory::create("Fusion"));
    
    // 创建主窗口
    MainWindow window;
    window.show();
    
    return app.exec();
}
