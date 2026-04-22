#include "ConfigWidget.h"
#include <QLayout>
#include "OneParamWidget.h"
#include "ZZListener.h"

ConfigWidget::ConfigWidget(QWidget *parent)
    : QWidget{parent}
    , mLabel_Title(Q_NULLPTR)
    , mButton_run(Q_NULLPTR)
    , mButton_reset(Q_NULLPTR)
{
    if(false == InitWidget())
    {
        throw std::bad_alloc();
    }
}

void ConfigWidget::GetPhotometricStereoParams(QList<QImage> &srcImages)
{
    srcImages.clear();
    for(auto pParamWidget : m_ParamWidgetList)
    {
        QImage srcImage = pParamWidget->GetQImage();
        srcImages.push_back(srcImage);
    }
}

void ConfigWidget::OnResetBtnClicked(bool clicked)
{
    for(auto pParamWidget : m_ParamWidgetList)
    {
        pParamWidget->Reset();
    }
    ListenerManger::Instance()->notify(ZHUZHAO_UPDATE_SRCIMAGE);
}

void ConfigWidget::OnRunOnceBtnClicked(bool clicked)
{
    ListenerManger::Instance()->notify(ZHUZHAO_RUNONCE);
}

bool ConfigWidget::InitWidget()
{
    auto CreateOneParam = [&](QString strParamName)->OneParamWidget*
    {
        auto pParamWidget = new OneParamWidget(strParamName, this);
        pParamWidget->setFixedHeight(60);
        m_ParamWidgetList.push_back(pParamWidget);

        return pParamWidget;
    };

    QFont font("Microsoft YaHei", 12);
    font.setBold(true);
    mLabel_Title = new QLabel(this);
    mLabel_Title->setText(tr("Input Select"));
    mLabel_Title->setFixedHeight(36);
    mLabel_Title->setFont(font);

    mButton_reset = new QPushButton(this);
    mButton_reset->setText(tr("Reset"));
    mButton_reset->setFixedSize(90, 28);
    mButton_reset->setIconSize(QSize(24,24));
    mButton_reset->setIcon(QIcon(":/Resouce/icon/reset.png"));
    connect(mButton_reset, &QPushButton::clicked, this, &ConfigWidget::OnResetBtnClicked);

    mButton_run = new QPushButton(this);
    mButton_run->setText(tr("Run"));
    mButton_run->setFixedSize(90, 28);
    mButton_run->setIconSize(QSize(24,24));
    mButton_run->setIcon(QIcon(":/Resouce/icon/runonce.png"));
    connect(mButton_run, &QPushButton::clicked, this, &ConfigWidget::OnRunOnceBtnClicked);

    QHBoxLayout *pLayout_Title = new QHBoxLayout(this);
    pLayout_Title->setContentsMargins(0, 0, 0, 0);
    pLayout_Title->addWidget(mLabel_Title);
    pLayout_Title->addStretch();
    QWidget *pWidget_Title = new QWidget(this);
    pWidget_Title->setLayout(pLayout_Title);

    QHBoxLayout *pLayout_Btn = new QHBoxLayout(this);
    pLayout_Btn->setContentsMargins(0, 0, 0, 0);
    pLayout_Btn->addStretch();
    pLayout_Btn->addWidget(mButton_reset);
    pLayout_Btn->addSpacing(8);
    pLayout_Btn->addWidget(mButton_run);
    QWidget *pWidget_Btn = new QWidget(this);
    pWidget_Btn->setFixedHeight(36);
    pWidget_Btn->setLayout(pLayout_Btn);
    pWidget_Btn->setStyleSheet("background-color:darkgray");

    auto pFirstParamWidget = CreateOneParam(tr("SrcImage"));
    auto pSecondParamWidget = CreateOneParam(tr("MaskImage"));
    auto pThirdParamWidget = CreateOneParam(tr("LmkImage"));

    QVBoxLayout* pTopLayout = new QVBoxLayout(this);
    pTopLayout->setContentsMargins(10,10,10,0);
    pTopLayout->setSpacing(0);
    pTopLayout->addWidget(pWidget_Title);
    pTopLayout->addWidget(pFirstParamWidget);
    pTopLayout->addWidget(pSecondParamWidget);
    pTopLayout->addWidget(pThirdParamWidget);
    QWidget* pTopWidget = new QWidget(this);
    pTopWidget->setLayout(pTopLayout);
    QVBoxLayout* pMainLayout = new QVBoxLayout(this);
    pMainLayout->setContentsMargins(0,0,0,0);
    pMainLayout->setSpacing(0);
    pMainLayout->addWidget(pTopWidget);
    pMainLayout->addStretch();
    pMainLayout->addWidget(pWidget_Btn);

    this->setLayout(pMainLayout);
    return true;
}


