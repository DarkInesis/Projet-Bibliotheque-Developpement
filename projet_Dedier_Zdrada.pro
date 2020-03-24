#-------------------------------------------------
#
# Project created by QtCreator 2012-10-23T11:26:13
#
#-------------------------------------------------

QT       += core gui opengl widgets

TARGET = Projet_bibliotheque
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

# ajout des libs au linker
win32 {
    win32-msvc* {
        LIBS     += opengl32.lib glu32.lib
    } else {
        LIBS     += -lopengl32 -lglu32
    }
}
else {
        LIBS     += -lGL -lGLU
}

SOURCES += \
    main.cpp \
    maze.cpp \
    myglwidget.cpp \
    wall.cpp \
    webcam.cpp \
    window.cpp

INCLUDEPATH +=$$(OPENCV_DIR)\..\..\include


LIBS += -L$$(OPENCV_DIR)\lib \
    -lopencv_core420 \
    -lopencv_highgui420 \
    -lopencv_imgproc420 \
    -lopencv_imgcodecs420 \
    -lopencv_videoio420 \
    -lopencv_features2d420 \
    -lopencv_calib3d420  \
    -lopencv_objdetect420

FORMS += \
    window.ui

HEADERS += \
    cell.h \
    draw_functions.h \
    maze.h \
    myglwidget.h \
    wall.h \
    webcam.h \
    window.h

QT += widgets
