#include "ProcessThread.h"
#include <opencv2/opencv.hpp>
#include "Lafin.h"
#include "ImageConvert.h"


ProcessThread::ProcessThread() {}

void ProcessThread::SetPhotometricStereoParams(QList<QImage> &srcImages)
{
    m_srcImages.clear();
    m_srcImages = srcImages;
}

void ProcessThread::GetResultImages(QList<QImage>& dstImages)
{
    dstImages.clear();
    dstImages = m_dstImages;
}

void ProcessThread::run()
{
    qDebug() << "开始修复";
    //准备输入数据和输出数据
    std::vector<cv::Mat> srcImages;
    std::vector<cv::Mat> dstHeightFieldImg;
    //先将输入参数整理为算法动态库接受的格式
    for(int i = 0; i < m_srcImages.size(); i++)
    {
        cv::Mat srcImage;

        if (i == 0)
        {
            // ✅ 第一张：彩色图，直接转，不转灰度！
            srcImage = MatToQImage::qImageToMat(m_srcImages.at(i));
        }
        else
        {
            // ✅ 其他图：mask / lmk，必须转灰度
            QImage grayImg = m_srcImages.at(i).convertToFormat(QImage::Format_Grayscale8);
            srcImage = MatToQImage::qImageToMat(grayImg);
        }

        srcImages.push_back(srcImage);
    }

    //运行算法
    LaFin_Inpaint(srcImages,dstHeightFieldImg);

    QList<QImage> qdst;

    //将算法结果转换并保存
    for(auto img : dstHeightFieldImg){
        qdst.push_back(MatToQImage::matToQImage(img));
    }

    QImage masked = createOcclusionImage(qdst[0], qdst[1]);
    m_dstImages.push_back(masked);
    m_dstImages.push_back(qdst[2]);
    qDebug() << "修复完成";
}


QImage ProcessThread::createOcclusionImage(const QImage& rgbImage, const QImage& maskImage)
{
    // 1. 确保尺寸一致
    QImage mask = maskImage;
    if (mask.size() != rgbImage.size()) {
        mask = mask.scaled(rgbImage.size(), Qt::IgnoreAspectRatio, Qt::FastTransformation);
    }

    // 2. 创建结果图
    QImage result = rgbImage.copy();
    int w = result.width();
    int h = result.height();

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            // 获取掩码灰度值
            int gray = qGray(mask.pixel(x, y));

            // ===================== 关键：对掩码取反 =====================
            int maskValue = 255 - gray;

            // 取反后 = 0 → 遮挡成黑色
            if (maskValue == 0) {
                result.setPixel(x, y, qRgb(0, 0, 0));
            }
        }
    }

    return result;
}
