#-------------------------------------------------
#
# Project created by QtCreator 2017-08-15T16:37:43
#
#-------------------------------------------------

QT       += core gui
QMAKE_CXXFLAGS += -std=c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Reco_3
TEMPLATE = app

INCLUDEPATH += /usr/local/include/opencv2

LIBS += -L/usr/local/lib
LIBS += -lopencv_core
LIBS += -lopencv_imgcodecs
LIBS += -lopencv_imgproc
LIBS += -lopencv_highgui
LIBS += -lopencv_ml
LIBS += -lopencv_video
LIBS += -lopencv_videoio
LIBS += -lopencv_features2d
LIBS += -lopencv_calib3d
LIBS += -lopencv_objdetect
LIBS += -lopencv_flann


# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    videowindow.cpp \
    preprocessing.cpp \
    player.cpp \



HEADERS += \
        mainwindow.h \
    videowindow.h \
    constants.h \
    preprocessing.h \
    player.h \
    constants.h \



FORMS += \
        mainwindow.ui \
    videowindow.ui

DISTFILES += \
    graphics/i_next.png \
    graphics/i_pause.png \
    graphics/i_play.png \
    graphics/i_previous.png \
    graphics/i_rewind_b.png \
    graphics/i_rewind_f.png \
    graphics/i_stop.png \


RESOURCES += \
    icons.qrc
