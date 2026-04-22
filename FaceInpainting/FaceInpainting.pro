QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# 包含头文件路径
INCLUDEPATH += $$PWD/

# 链接 lib
LIBS += -L$$PWD/ -lLaFinInfer

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ConfigWidget/ConfigWidget.cpp \
    ConfigWidget/OneParamWidget.cpp \
    ConfigWidget/ProcessThread.cpp \
    LogWidget/ZZLogMessage.cpp \
    LogWidget/ZZLogWidget.cpp \
    ThumWidget/HThumbnailList.cpp \
    ThumWidget/LitImgItemWidget.cpp \
    ThumWidget/VThumbnailList.cpp \
    ViewWidget/CustomGraphicsView.cpp \
    ViewWidget/CustomImageItem.cpp \
    ViewWidget/CustomImageView.cpp \
    ZZListener.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    ConfigWidget/ConfigWidget.h \
    ConfigWidget/ImageConvert.h \
    ConfigWidget/OneParamWidget.h \
    ConfigWidget/ProcessThread.h \
    LogWidget/ZZLogMessage.h \
    LogWidget/ZZLogWidget.h \
    ThumWidget/HThumbnailList.h \
    ThumWidget/LitImgItemWidget.h \
    ThumWidget/VThumbnailList.h \
    ViewWidget/CustomGraphicsView.h \
    ViewWidget/CustomImageItem.h \
    ViewWidget/CustomImageView.h \
    ZZListener.h \
    mainwindow.h

OPENCV_PATH = D:\opencv\opencv\build # 改成你自己的解压路径

INCLUDEPATH += $$OPENCV_PATH/include
INCLUDEPATH += $$OPENCV_PATH/include/opencv2

LIBS += $$OPENCV_PATH/x64/vc16/lib/*.lib
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc
