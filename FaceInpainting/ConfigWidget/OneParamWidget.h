#ifndef ONEPARAMWIDGET_H
#define ONEPARAMWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>

class OneParamWidget : public QWidget
{
    Q_OBJECT
public:
    explicit OneParamWidget(QString paramName, QWidget *parent = nullptr);
    QImage GetQImage();
    void Reset();

protected:
    bool InitWidget();

protected slots:
    void OnSigLoadImageBtnClicked(bool clicked);
private:
    QString m_strParamName;
    QLabel* m_pTitleLabel;
    QLineEdit *m_pLineEdit;
    QPushButton* m_pLoadImageBtn;
    QImage m_qImage;
};

#endif // ONEPARAMWIDGET_H
