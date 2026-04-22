#ifndef CONFIGWIDGET_H
#define CONFIGWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>

class OneParamWidget;
class ConfigWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ConfigWidget(QWidget *parent = nullptr);
    void GetPhotometricStereoParams(QList<QImage>& srcImages);

protected slots:
    void OnResetBtnClicked(bool clicked);
    void OnRunOnceBtnClicked(bool clicked);

protected:
    bool InitWidget();


private:
    QLabel *mLabel_Title;
    QPushButton *mButton_run;
    QList<OneParamWidget*> m_ParamWidgetList;
    QPushButton *mButton_reset;
};

#endif // CONFIGWIDGET_H
