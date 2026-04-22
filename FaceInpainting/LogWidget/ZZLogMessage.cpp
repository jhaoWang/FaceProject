#include "ZZLogMessage.h"
#include <QMutex>
#include <QDateTime>
#include <QCoreApplication>
#include <QFile>
#include <QDir>
#include <QTextStream>
/**************************************************
 *一个日志的单例实现
 * 负责将QT的日志打印重定向到页面一个控件上
 * 并将日志的打印信息保存到本地磁盘
 **************************************************/

//接收调试信息的函数
void outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    static QMutex mutex;
    QMutexLocker lock(&mutex);

    QString text;
    QString MessageColor = "red";
    switch(type)
    {
        //如果是debug信息，那么直接打印至应用程序输出，然后退出本函数
        //    case QtDebugMsg:
        //        std::cout << msg.toStdString() << std::endl;
        //        return ;
    //debug信息
    case QtDebugMsg:
    {
        text = QString("[Debug]");
        MessageColor = "darkgray";
        break;
    }
    //如果是警告，则继续执行后面的数据处理
    case QtWarningMsg:
    {
        text = QString("[Warning]");
        MessageColor = "darkorange";
        break;
    }
    case QtCriticalMsg:
    {
        text = QString("[Error]");
        MessageColor = "red";
        break;
    }
    case QtFatalMsg:
    {
        text = QString("[Fatal]");
        MessageColor = "red";
        break;
    }
    default:
    {
        text = QString("[Default]");
        MessageColor = "red";
        break;
    }
    }
    //获取单例
    ZZLogMessage *instance = ZZLogMessage::Instance();
    //消息打印时间
    QString current_date_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString current_date = QString("[%1]")
                               .arg(current_date_time);
    //调试信息
    QString message = QString("%1%2%3")
                          .arg(current_date)
                          .arg(text)
                          .arg(msg);
    //将调试信息格式化成html格式
    QString messageHtml = QString("<font color=%1>" + message + "</font>").arg(MessageColor);

    //将调试信息写入文件
    QFile file(instance->logPath() + instance->logName());
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream text_stream(&file);
    text_stream << message << "\r\n";
    file.flush();
    file.close();
    //将处理好的调试信息发送出去
    emit instance->sigDebugStrData(message);
    //将处理成 html 的调试信息发送出去
    emit instance->sigDebugHtmlData(messageHtml);
    //检查文件是否达到了指定大小
    if(file.size() < 1024*1024)
    {
        return ;
    }
    //log达到了限制值则将名字更改，防止文件越来越大
    for(int loop = 1; loop < 100; ++loop)
    {
        QString fileName = QString("%1/log_%2.txt").arg(instance->logPath()).arg(loop);
        QFile logfile(fileName);
        if(logfile.size() < 4)
        {
            file.rename(fileName);
            return;
        }
    }
}

//MyDebug单例
ZZLogMessage* ZZLogMessage::m_pLogInstance = nullptr;
ZZLogMessage* ZZLogMessage::Instance()
{
    if(!m_pLogInstance)
    {
        QMutex muter;
        QMutexLocker clocker(&muter);

        if(!m_pLogInstance)
        {
            m_pLogInstance = new ZZLogMessage();
        }
    }
    return m_pLogInstance;
}
//安装消息器
void ZZLogMessage::installMessageHandler()
{
    qInstallMessageHandler(outputMessage);
}
//卸载消息器
void ZZLogMessage::uninstallMessageHandler()
{
    qInstallMessageHandler(nullptr);
}
//在程序的运行目录下建立日志文件路径
QString ZZLogMessage::logPath()
{
    QString current_date_file_name = QDateTime::currentDateTime().toString("yyyy-MM-dd");
    QDir dir(QString("log/%1").arg(current_date_file_name));
    if(!dir.exists())
    {
        dir.mkpath("./");
    }
    return dir.path() + "/" ;
}

QString ZZLogMessage::logName()
{
    return "log.txt";
}

ZZLogMessage::ZZLogMessage(QObject *parent) : QObject(parent)
{
}

ZZLogMessage::~ZZLogMessage()
{
}
