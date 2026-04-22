#ifndef ZZLOGMESSAGE_H
#define ZZLOGMESSAGE_H

#include <QObject>
#include <QDebug>

/****************************************************************
 * LogMessage类
 * 负责这个界面的调试日志信息打印
****************************************************************/

//日志输出宏
#define QDEBUG(message)     qDebug()<<" [FUNCTION]: "<<__FUNCTION__<<" [LINE]: "<<__LINE__<<" [LOG]: " << message;
#define QWARNING(message)   qWarning()<<" [FUNCTION]: "<<__FUNCTION__<<" [LINE]: "<<__LINE__<<" [LOG]: " << message;
#define QCRITICAL(message)  qCritical()<<" [FUNCTION]: "<<__FUNCTION__<<" [LINE]: "<<__LINE__<<" [LOG]: " << message;
#define QFATAL(message)     qFatal()<<" [FUNCTION]: "<<__FUNCTION__<<" [LINE]: "<<__LINE__<<" [LOG]: " << message;

//单例设计模式实现日志功能类
class ZZLogMessage : public QObject
{
    Q_OBJECT
public:
    static  ZZLogMessage* Instance();
    void installMessageHandler();
    void uninstallMessageHandler();
    QString logPath();
    QString logName();

private:
    explicit ZZLogMessage(QObject *parent = nullptr);
    ~ZZLogMessage();
    static ZZLogMessage* m_pLogInstance;//全局唯一

signals:
    void sigDebugStrData(const QString &);
    void sigDebugHtmlData(const QString &);
};

#endif // ZZLOGMESSAGE_H
