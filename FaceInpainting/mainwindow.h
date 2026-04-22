#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ZZListener.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class ConfigWidget;
class LogWidget;
class CustomImageView;
class HThumbnailList;
class VThumbnailList;
class ProcessThread;
class MainWindow : public QMainWindow, ZZListener
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    //响应监听事件
    void RespondMessage(int message) override;

protected:
    bool InitWidget();

protected slots:
    void OnProcessThreadFinished();

private:
    ConfigWidget* m_pConfigWidget;
    LogWidget* m_pLogWidget;
    CustomImageView* m_pImageView;
    HThumbnailList* m_pHThumList;
    VThumbnailList* m_pVThumList;
    ProcessThread* m_pRunProcess;
};
#endif // MAINWINDOW_H
