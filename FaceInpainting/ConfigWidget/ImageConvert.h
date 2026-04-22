#ifndef MATTOQIMAGE_H
#define MATTOQIMAGE_H

#include <QImage>
#include <opencv2/opencv.hpp>
#include <QDebug>

/**
 * @brief 工具类：OpenCV Mat 和 Qt QImage 相互转换
 * 支持格式：
 *  - CV_8UC1  <->  QImage::Format_Grayscale8
 *  - CV_8UC3  <->  QImage::Format_RGB888
 *  - CV_8UC4  <->  QImage::Format_ARGB32
 */
class MatToQImage
{
public:
    /**
     * @brief cv::Mat 转 QImage
     * @param mat 输入的 OpenCV Mat（支持 8位 单通道/3通道/4通道）
     * @return 转换后的 QImage，失败返回空图像
     */
    static QImage matToQImage(const cv::Mat& mat)
    {
        // 空 Mat 直接返回空图像
        if (mat.empty()) {
            qWarning() << "matToQImage: 输入 Mat 为空！";
            return QImage();
        }

        switch (mat.type()) {
        // 8位灰度图
        case CV_8UC1: {
            QImage image(mat.data,
                         mat.cols,
                         mat.rows,
                         mat.step,
                         QImage::Format_Grayscale8);
            // 深拷贝，避免共享内存导致崩溃
            return image.copy();
        }

        // 8位3通道 (BGR格式 OpenCV默认)
        case CV_8UC3: {
            cv::Mat rgbMat;
            // OpenCV BGR -> Qt RGB
            cv::cvtColor(mat, rgbMat, cv::COLOR_BGR2RGB);
            QImage image(rgbMat.data,
                         rgbMat.cols,
                         rgbMat.rows,
                         rgbMat.step,
                         QImage::Format_RGB888);
            return image.copy();
        }

        // 8位4通道 (带透明通道)
        case CV_8UC4: {
            QImage image(mat.data,
                         mat.cols,
                         mat.rows,
                         mat.step,
                         QImage::Format_ARGB32);
            return image.copy();
        }

        // 不支持的格式
        default: {
            qWarning() << "matToQImage: 不支持的 Mat 格式：" << mat.type();
            return QImage();
        }
        }
    }

    /**
     * @brief QImage 转 cv::Mat
     * @param image 输入的 Qt QImage
     * @return 转换后的 OpenCV Mat，失败返回空 Mat
     */
    static cv::Mat qImageToMat(const QImage& image)
    {
        // 空图像直接返回
        if (image.isNull()) {
            qWarning() << "qImageToMat: 输入 QImage 为空！";
            return cv::Mat();
        }

        QImage convertedImage;
        switch (image.format()) {
        // 灰度图
        case QImage::Format_Grayscale8: {
            return cv::Mat(image.height(),
                           image.width(),
                           CV_8UC1,
                           const_cast<uchar*>(image.bits()),
                           image.bytesPerLine()).clone();
        }

        // RGB3通道
        case QImage::Format_RGB888: {
            cv::Mat mat(image.height(),
                        image.width(),
                        CV_8UC3,
                        const_cast<uchar*>(image.bits()),
                        image.bytesPerLine());
            // Qt RGB -> OpenCV BGR
            cv::cvtColor(mat, mat, cv::COLOR_RGB2BGR);
            return mat.clone();
        }

        // ARGB4通道
        case QImage::Format_ARGB32:
        case QImage::Format_RGBA8888: {
            return cv::Mat(image.height(),
                           image.width(),
                           CV_8UC4,
                           const_cast<uchar*>(image.bits()),
                           image.bytesPerLine()).clone();
        }

        // 其他格式统一转成 RGB888 再转换
        default: {
            convertedImage = image.convertToFormat(QImage::Format_RGB888);
            return qImageToMat(convertedImage);
        }
        }
    }
};

#endif // MATTOQIMAGE_H
