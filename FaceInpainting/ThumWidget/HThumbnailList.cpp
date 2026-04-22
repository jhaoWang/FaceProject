#include "HThumbnailList.h"
#include <QLayout>
#include <QFileDialog>
#include <QScrollBar>
#include "LitImgItemWidget.h"

HThumbnailList::HThumbnailList(QWidget *parent)
    : QWidget(parent)
    , m_pListWidget(nullptr)
    , m_bIsRunOnce(false)
{
    if(false == InitWidget())
    {
        throw std::bad_alloc();
    }
}

HThumbnailList::~HThumbnailList()
{
}

bool HThumbnailList::InitWidget()
{
    //初始化变量
    m_pListWidget = new QListWidget(this);
    m_pListWidget->horizontalScrollBar()->setStyleSheet("QScrollBar{background:transparent; height:5px; margin:0px 0px 0px 0px;}"
                                                        "QScrollBar::handle{background:rgba(223, 223, 225, 200); border:0px; border-radius:5px; margin:0px 0px 0px 0px;}"
                                                        "QScrollBar::handle:hover{background:lightgray;}"
                                                        "QScrollBar::handle:pressed{background:rgba(200, 200, 200, 255);}"
                                                        "QScrollBar::sub-page{background:transparent;}"
                                                        "QScrollBar::add-page{background:transparent;}"
                                                        "QScrollBar::up-arrow{background:transparent;}"
                                                        "QScrollBar::down-arrow{background:transparent;}"
                                                        "QScrollBar::sub-line{background:transparent; height:0px;}"
                                                        "QScrollBar::add-line{background:transparent; height:0px;}");
    m_pListWidget->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    m_pListWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_pListWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_pListWidget->setFlow(QListView::LeftToRight);
    m_pListWidget->setViewMode(QListView::ViewMode::ListMode);
    m_pListWidget->setSelectionMode(QListView::SingleSelection);
    m_pListWidget->setFocusPolicy(Qt::NoFocus);
    m_pListWidget->setSelectionRectVisible(false);
    m_pListWidget->setHorizontalScrollMode(QListView::ScrollPerPixel);
    m_pListWidget->setDragEnabled(false);
    m_pListWidget->setSpacing(4);

    //初始化布局
    QVBoxLayout* pListLayout = new QVBoxLayout();
    pListLayout->setSpacing(0);
    pListLayout->setContentsMargins(0,0,0,0);
    pListLayout->addWidget(m_pListWidget);
    this->setLayout(pListLayout);

    //初始化信号槽
    connect(m_pListWidget,&QListWidget::currentRowChanged,
            this,&HThumbnailList::OnCurrentRowChanged);

    return true;
}

void HThumbnailList::addImage(const QString& strPath)
{
    //初始化缩略图元素窗口
    QImage qImage(strPath);
    addImage(qImage);
}

void HThumbnailList::addImage(QImage& qImage)
{
    LitImgItemWidget *pImgItemWidget = new LitImgItemWidget();
    pImgItemWidget->setImage(qImage);
    //定义QListWidgetItem对象
    QListWidgetItem *pItem = new QListWidgetItem();
    pItem->setSizeHint(QSize(this->height() + 20,this->height() - 10));
    m_pListWidget->addItem(pItem);
    m_pListWidget->setItemWidget(pItem,pImgItemWidget);
    m_listImage.push_back(qImage);
}

void HThumbnailList::addImages(QList<QImage>& qImages)
{
    for(QImage& img : qImages)
    {
        addImage(img);
    }
}

void HThumbnailList::openFolder()
{
    //定义文件对话框类
    QFileDialog *fileDialog = new QFileDialog(this);
    //定义文件对话框标题
    fileDialog->setWindowTitle(tr("OpenImageFolder"));
    //设置默认文件路径
    fileDialog->setDirectory(".");
    //设置文件过滤器
    fileDialog->setNameFilter(tr("Images(*.png *.jpg *.jpeg *.bmp)"));
    //设置可以选择多个文件,默认为只能选择一个文件QFileDialog::ExistingFiles
    fileDialog->setFileMode(QFileDialog::ExistingFiles);
    //设置视图模式
    fileDialog->setViewMode(QFileDialog::Detail);

    if(fileDialog->exec())
    {
        clearAllImages();
        m_strImagePaths = fileDialog->selectedFiles();
        for(auto strImagePath : m_strImagePaths)
        {
            addImage(strImagePath);
        }
    }
}

void HThumbnailList::clearAllImages()
{
    m_pListWidget->clear();
    m_strImagePaths.clear();
    m_listImage.clear();
}

void HThumbnailList::nextImage()
{
    int nCurrRow = m_pListWidget->currentRow();
    if(nCurrRow == m_listImage.size()-1)
    {
        nCurrRow = -1;
    }
    //m_bIsRunOnce = true;
    m_pListWidget->setCurrentRow(nCurrRow + 1);
}

void HThumbnailList::lastImage()
{
    int nCurrRow = m_pListWidget->currentRow();
    if(nCurrRow == 0)
    {
        nCurrRow = m_pListWidget->count();
    }
    //m_bIsRunOnce = true;
    m_pListWidget->setCurrentRow(nCurrRow - 1);
}

QImage HThumbnailList::getNextImage()
{
    this->nextImage();
    return getCurrentImage();
}

QImage HThumbnailList::getLastImage()
{
    this->lastImage();
    return getCurrentImage();
}

//获取当前图像
QImage HThumbnailList::getCurrentImage()
{
    int nCurrRow = m_pListWidget->currentRow();
    if(nCurrRow < 0 || nCurrRow > m_listImage.length())
    {
        return QImage();
    }
    return m_listImage[nCurrRow];
}

void HThumbnailList::OnCurrentRowChanged(int nCurRow)
{
    if(nCurRow < 0 || nCurRow > m_listImage.size())
        return;
    QImage curImage = m_listImage[nCurRow];
    emit SigSelectImageChanged(curImage);
    //    if(true == m_bIsRunOnce)
    //    {
    //        m_bIsRunOnce = false;
    //    }
    //    else
    //    {
    //        if(nCurRow < 0 || nCurRow > m_listImage.size())
    //            return;
    //        emit SigSelectImageChanged(m_listImage[nCurRow]);
    //    }
}
