#-------------------------------------------------
#
# Project created by QtCreator 2017-08-15T16:37:43
#
#-------------------------------------------------

QT       += core gui
QMAKE_CXXFLAGS += -std=c++11 -pthread

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Reco_3
TEMPLATE = app

INCLUDEPATH += /usr/local/include/opencv2
INCLUDEPATH += /home/siobhan/UJ/Masters/Libs/dlib-19.4
INCLUDEPATH += /usr/local/include/opencv

unix {
    CONFIG += link_pkgconfig
    LIBS += -pthread
    PKGCONFIG += opencv
    PKGCONFIG += x11
}



#LIBS += -L/usr/local/lib -lopencv_core -lopencv_imgcodecs -lopencv_imgproc -lopencv_highgui -lopencv_ml -lopencv_video
#LIBS += -lopencv_videoio -lopencv_features2d -lopencv_calib3d -lopencv_objdetect -lopencv_flann -lopencv_objdetect
#LIBS += -lopencv_photo -lopencv_stitching -lopencv_superres -lopencv_videostab


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
    preprocessing.cpp \
    facedetector.cpp \
    qualityassessment.cpp \
    averageface.cpp \
    ../../../UJ/Masters/Libs/dlib-19.4/dlib/all/source.cpp \
    peopletracker.cpp \
    common/common.cpp \
    gui/mainwindow.cpp \
    gui/videolabel.cpp \
    gui/videowindow.cpp \
    gui/recowindow.cpp \
    common/face.cpp \
    reco/facerecognition.cpp \
    gui/dbcreationwindow.cpp



HEADERS += \
    preprocessing.h \
    facedetector.h \
    qualityassessment.h \
    averageface.h \
    peopletracker.h \
    common/common.h \
    gui/videolabel.h \
    gui/videowindow.h \
    gui/recowindow.h \
    gui/mainwindow.h \
    common/face.h \
    reco/facerecognition.h \
    gui/dbcreationwindow.h



FORMS += \
        mainwindow.ui \
    videowindow.ui \
    recowindow.ui \
    dbcreationwindow.ui

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
