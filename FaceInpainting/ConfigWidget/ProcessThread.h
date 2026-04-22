#ifndef PROCESSTHREAD_H
#define PROCESSTHREAD_H

#include <QThread>
#include <QImage>

class ProcessThread : public QThread
{
public:
    ProcessThread();
    void SetPhotometricStereoParams(QList<QImage>& srcImages);
    void GetResultImages(QList<QImage>& dstImages);

protected:
    void run() override;
    QImage createOcclusionImage(const QImage& rgbImage, const QImage& maskImage);

private:
    volatile bool m_bIsStop;
    QList<QImage> m_srcImages;
    QList<QImage> m_dstImages;

};

#endif // PROCESSTHREAD_H
