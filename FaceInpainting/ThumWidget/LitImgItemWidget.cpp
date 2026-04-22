#include "LitImgItemWidget.h"
#include <QLayout>
#include <QPainter>
#include <QPixmap>

#define LITIMG_WIDTH 100
#define LITIMG_HEIGHT 80

LitImgItemWidget::LitImgItemWidget(QWidget *parent)
    : QWidget(parent)
{
    //this->setFixedSize(LITIMG_WIDTH, LITIMG_HEIGHT);
    this->setFocusPolicy(Qt::StrongFocus);
    if(false == InitWidget())
    {
        throw std::bad_alloc();
    }
}

LitImgItemWidget::~LitImgItemWidget()
{
}

bool LitImgItemWidget::InitWidget()
{
    //    //初始化布局
    //    QVBoxLayout* pListLayout = new QVBoxLayout();
    //    pListLayout->setSpacing(0);
    //    pListLayout->setContentsMargins(0,0,0,0);
    //    this->setLayout(pListLayout);
    return true;
}

void LitImgItemWidget::setImage(QImage& qImage)
{
    m_qImg = qImage;
}

void LitImgItemWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    int nDevceWidth = this->width();
    int nDeviceHeight = this->height();
    //1、绘制图像
    if(m_qImg.isNull())
    {
        //如果图像为空，就加载问号图像
        m_qImg = QImage("://Resouce/icon/nullImg.png");
    }
    // QPixmap tempPixmap = QPixmap::fromImage(m_qImg);
    // painter.drawPixmap(2, 2, nDevceWidth-4, nDeviceHeight-4, tempPixmap);
    // //绘制图像外围矩形
    // QPen pen;
    // pen.setColor(QColor(this->hasFocus() ? Qt::red : Qt::black));//设置笔颜色，选中的时候为红色，否则为黑色
    // pen.setWidth(4);//设置笔宽度
    // painter.setPen(pen);//设置为要绘制的笔
    // painter.drawRect(QRect(2,2,nDevceWidth-4,nDeviceHeight-4));

    if (m_qImg.format() != QImage::Format_RGB32) {
        m_qImg = m_qImg.convertToFormat(QImage::Format_RGB32);
        update();
    }

    // 👇👇👇【核心修复】不用 Pixmap 中转，直接绘制 QImage
    painter.drawImage(QRect(2, 2, nDevceWidth-4, nDeviceHeight-4), m_qImg);

    // 绘制外框
    QPen pen;
    pen.setColor(hasFocus() ? Qt::red : Qt::black);
    pen.setWidth(4);
    painter.setPen(pen);
    painter.drawRect(2, 2, nDevceWidth-4, nDeviceHeight-4);
}
