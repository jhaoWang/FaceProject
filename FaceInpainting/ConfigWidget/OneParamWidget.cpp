#include "OneParamWidget.h"
#include <QLayout>
#include <QFileDialog>
#include <QMessageBox>
#include "ZZListener.h"

#define OneParamWidgetHeiht (28)

OneParamWidget::OneParamWidget(QString paramName, QWidget *parent)
    : QWidget{parent}
    , m_strParamName(paramName)
    , m_pTitleLabel(Q_NULLPTR)
    , m_pLineEdit(Q_NULLPTR)
    , m_pLoadImageBtn(Q_NULLPTR)
{
    if(false == InitWidget())
    {
        throw std::bad_alloc();
    }
}

QImage OneParamWidget::GetQImage()
{
    return m_qImage;
}

void OneParamWidget::Reset()
{
    m_pLineEdit->clear();
}

bool OneParamWidget::InitWidget()
{
    QFont font("Microsoft YaHei", 8);
    font.setBold(true);

    m_pTitleLabel = new QLabel(this);
    m_pTitleLabel->setText(m_strParamName);
    m_pTitleLabel->setFont(font);

    m_pLineEdit = new QLineEdit(this);
    m_pLineEdit->setFixedHeight(OneParamWidgetHeiht);

    m_pLoadImageBtn = new QPushButton(this);
    m_pLoadImageBtn->setText(tr("load"));
    m_pLoadImageBtn->setFixedSize(80, OneParamWidgetHeiht);
    m_pLoadImageBtn->setIconSize(QSize(24,24));
    m_pLoadImageBtn->setIcon(QIcon(":/Resouce/icon/loadimg.png"));
    connect(m_pLoadImageBtn, &QPushButton::clicked, this, &OneParamWidget::OnSigLoadImageBtnClicked);

    QHBoxLayout* pTitleLayout = new QHBoxLayout(this);
    pTitleLayout->setContentsMargins(0,0,0,0);
    pTitleLayout->setSpacing(0);
    pTitleLayout->addWidget(m_pTitleLabel);
    pTitleLayout->addStretch();
    QWidget* pTilteWidget = new QWidget(this);
    pTilteWidget->setLayout(pTitleLayout);

    QHBoxLayout* pParamLayout = new QHBoxLayout(this);
    pParamLayout->setContentsMargins(0,0,0,0);
    pParamLayout->setSpacing(0);
    pParamLayout->addWidget(m_pLineEdit);
    pParamLayout->addSpacing(8);
    pParamLayout->addWidget(m_pLoadImageBtn);
    QWidget* pParamWidget = new QWidget(this);
    pParamWidget->setLayout(pParamLayout);

    QVBoxLayout* pMainLayout = new QVBoxLayout(this);
    pMainLayout->setContentsMargins(0,0,0,0);
    pMainLayout->setSpacing(0);
    pMainLayout->addWidget(pTilteWidget);
    pMainLayout->addWidget(pParamWidget);

    this->setLayout(pMainLayout);
    return true;
}

void OneParamWidget::OnSigLoadImageBtnClicked(bool clicked)
{
    qDebug() << "加载图片" << this->m_strParamName;
    QString strImgPath = QFileDialog::getOpenFileName(this, tr("Select Image"), ".", tr("Images(*.jpg *.png *.bmp)"));
    if(strImgPath.isEmpty())
    {
        return;
    }
    m_qImage = QImage(strImgPath);
    m_pLineEdit->setText(strImgPath);
    if(m_qImage.isNull()) //加载图像
    {
        QMessageBox::information(this, tr("Error"), tr("Load Image Failed!"));
        //delete loadImg;
        return;
    }
    ListenerManger::Instance()->notify(ZHUZHAO_UPDATE_SRCIMAGE);
}
