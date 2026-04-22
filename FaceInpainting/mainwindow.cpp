#include "mainwindow.h"
#include <QLayout>
#include <QSplitter>
#include "ConfigWidget/ConfigWidget.h"
#include "ConfigWidget/ProcessThread.h"
#include "LogWidget/ZZLogWidget.h"
#include "ViewWidget/CustomImageView.h"
#include "ThumWidget/VThumbnailList.h"
#include "ThumWidget/HThumbnailList.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ZZListener()
    , m_pConfigWidget(Q_NULLPTR)
    , m_pLogWidget(Q_NULLPTR)
    , m_pImageView(Q_NULLPTR)
    , m_pHThumList(Q_NULLPTR)
    , m_pVThumList(Q_NULLPTR)
    , m_pRunProcess(Q_NULLPTR)
{
    //设置大小
    this->setMinimumSize(1000,600);
    //设置标题
    this->setWindowTitle(tr("ZhuzhaoGUI - zhouxuan - https://www.roundvision.cc"));

    if(false == InitWidget())
    {
        throw std::bad_alloc();
    }

    //注册事件监听
    ListenerManger::Instance()->registerMessage(
        MESSAGE::ZHUZHAO_UPDATE_SRCIMAGE |
            MESSAGE::ZHUZHAO_UPDATE_RESULTI |
            MESSAGE::ZHUZHAO_RUNONCE
        , this);

    //首次初始化先更新一下图像列表
    ListenerManger::Instance()->notify(ZHUZHAO_UPDATE_SRCIMAGE);
}

MainWindow::~MainWindow() {}

void MainWindow::RespondMessage(int message)
{
    if ((message & MESSAGE::ZHUZHAO_UPDATE_SRCIMAGE) == MESSAGE::ZHUZHAO_UPDATE_SRCIMAGE)
    {
        //刷新输入图像列表
        QList<QImage> srcImages;
        QList<float> Slants;
        QList<float> Tilts;
        m_pConfigWidget->GetPhotometricStereoParams(srcImages);
        m_pVThumList->clearAllImages();
        m_pVThumList->addImages(srcImages);
    }
    if ((message & MESSAGE::ZHUZHAO_UPDATE_RESULTI) == MESSAGE::ZHUZHAO_UPDATE_RESULTI)
    {
        //刷新视觉窗口和结果图像列表
    }
    if ((message & MESSAGE::ZHUZHAO_RUNONCE) == MESSAGE::ZHUZHAO_RUNONCE)
    {
        //单次运行一次
        QList<QImage> srcImages;
        QList<float> Slants;
        QList<float> Tilts;
        m_pConfigWidget->GetPhotometricStereoParams(srcImages);
        m_pRunProcess->SetPhotometricStereoParams(srcImages);
        //start执行子线程
        m_pRunProcess->start();
    }
}

bool MainWindow::InitWidget()
{
    //创建标题
    QFont font("Microsoft YaHei", 20);
    font.setBold(true);

    m_pConfigWidget = new ConfigWidget(this);

    m_pLogWidget = new LogWidget(this);

    m_pImageView = new CustomImageView(this);

    m_pHThumList = new HThumbnailList(this);
    m_pHThumList->setFixedHeight(120);
    m_pVThumList = new VThumbnailList(this);
    m_pVThumList->setFixedWidth(120);
    connect(m_pHThumList,&HThumbnailList::SigSelectImageChanged,m_pImageView,&CustomImageView::OnSendImage);
    connect(m_pVThumList,&VThumbnailList::SigSelectImageChanged,m_pImageView,&CustomImageView::OnSendImage);

    m_pRunProcess = new ProcessThread();
    connect(m_pRunProcess,&ProcessThread::finished,this,&MainWindow::OnProcessThreadFinished);
    //创建布局
    QSplitter* pLeftSplitter = new QSplitter(Qt::Vertical);
    pLeftSplitter->addWidget(m_pConfigWidget);
    pLeftSplitter->addWidget(m_pLogWidget);

    QVBoxLayout* pViewLayout = new QVBoxLayout(this);
    pViewLayout->setContentsMargins(0,0,0,0);
    pViewLayout->addWidget(m_pImageView);
    pViewLayout->addWidget(m_pHThumList);
    pViewLayout->addStretch();

    QHBoxLayout* pRightLayout = new QHBoxLayout(this);
    pRightLayout->setContentsMargins(0,0,0,0);
    pRightLayout->addWidget(m_pVThumList);
    pRightLayout->addLayout(pViewLayout);
    QWidget* pRightWidget = new QWidget(this);
    pRightWidget->setLayout(pRightLayout);

    QSplitter* pMianSplitter = new QSplitter(Qt::Horizontal);
    pMianSplitter->addWidget(pLeftSplitter);
    pMianSplitter->addWidget(pRightWidget);

    QHBoxLayout* pMainLayout = new QHBoxLayout(this);
    pMainLayout->setContentsMargins(0,0,0,0);
    pMainLayout->addWidget(pMianSplitter);
    QWidget* pCenterWidget = new QWidget(this);
    pCenterWidget->setLayout(pMainLayout);

    this->setCentralWidget(pCenterWidget);
    return true;
}

void MainWindow::OnProcessThreadFinished()
{
    QList<QImage> dstImages;
    m_pRunProcess->GetResultImages(dstImages);
    m_pHThumList->clearAllImages();
    m_pHThumList->addImages(dstImages);
}
