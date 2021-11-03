QT       += core gui quick bluetooth opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = MSensor_Monitor
TEMPLATE = app

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    sourse/dialog2.cpp \
    sourse/Data_Convert.cpp \
    sourse/Protocol.cpp \
    sourse/characteristicinfo.cpp \
    sourse/device.cpp \
    sourse/deviceinfo.cpp \
    sourse/geometryengine.cpp \
    sourse/m_connect.cpp \
    sourse/main.cpp \
    sourse/mainwindow.cpp \
    sourse/miwidget.cpp \
    sourse/my_messagebox.cpp \
    sourse/objectgl.cpp \
    sourse/qcustomplot.cpp \
    sourse/serviceinfo.cpp

HEADERS += \
    header/dialog2.h \
    header/Data_Convert.h \
    header/Protocol.h \
    header/characteristicinfo.h \
    header/device.h \
    header/deviceinfo.h \
    header/geometryengine.h \
    header/m_connect.h \
    header/mainwindow.h \
    header/miwidget.h \
    header/my_messagebox.h \
    header/objectgl.h \
    header/qcustomplot.h \
    header/serviceinfo.h \
    sourse/m_connect.h

FORMS += \
    sourse/dialog2.ui \
    sourse/m_connect.ui \
    sourse/mainwindow.ui

LIBS += -lOpengl32
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resoure/Resources.qrc \
    resoure/shaders.qrc \
    resoure/textures.qrc \


DISTFILES +=

RC_FILE = resoure/logo.rc
