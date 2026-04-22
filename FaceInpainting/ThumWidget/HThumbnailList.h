#ifndef HTHUMBNAILLIST_H
#define HTHUMBNAILLIST_H

#include <QWidget>
#include <QListWidget>
#include <QList>
#include <QImage>

class HThumbnailList: public QWidget
{
    Q_OBJECT
public:
    HThumbnailList(QWidget* parent = nullptr);
    ~HThumbnailList();
    bool InitWidget();

    //添加单张图像
    void addImage(const QString& strPath);
    //添加单张图像
    void addImage(QImage& qImage);
    //添加多张图像
    void addImages(QList<QImage>& qImages);
    //打开图像文件夹
    void openFolder();
    //清除所有图像
    void clearAllImages();
    //获取下张图像
    void nextImage();
    //获取上张图像
    void lastImage();
    //获取下张图像
    QImage getNextImage();
    //获取上张图像
    QImage getLastImage();
    //获取当前图像
    QImage getCurrentImage();

signals:
    void SigSelectImageChanged(QImage& qImage);
protected slots:
    void OnCurrentRowChanged(int nCurRow);

private:
    QListWidget* m_pListWidget;
    QStringList m_strImagePaths;//存储缩略图读取的图像的绝对路径
    QList<QImage> m_listImage;//存储图像列表
    bool m_bIsRunOnce;//标识是手动选中图像，还是next图像引起的图像列表变化
};

#endif // HTHUMBNAILLIST_H
