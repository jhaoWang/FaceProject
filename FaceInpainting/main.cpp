#include "mainwindow.h"
#include <QApplication>
#include <QStyleFactory>
#include "LogWidget/ZZLogMessage.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QStyle *style = QStyleFactory::create("fusion");
    a.setStyle(style);

    ZZLogMessage::Instance()->installMessageHandler();
    QDEBUG("启动日志系统");
    MainWindow w;
    w.show();
    return a.exec();
}
