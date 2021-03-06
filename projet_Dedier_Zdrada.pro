#-------------------------------------------------
#
# Project created by QtCreator 2012-10-23T11:26:13
#
#-------------------------------------------------

QT       += core gui opengl widgets

TARGET = Projet_bibliotheque
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
    ball.cpp \
    gamewidget.cpp \
    labyrinthe.cpp \
    main.cpp \
    maze.cpp \
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
    ball.h \
    cell.h \
    draw_functions.h \
    gamewidget.h \
    labyrinthe.h \
    maze.h \
    webcam.h \
    window.h

QT += widgets

DISTFILES += \
    res/haarcascade_frontalface_alt.xml \
    res/logo_tse.png \
    res/mercure.jpg \
    resources/haarcascade_eye.xml \
    resources/haarcascade_eye_tree_eyeglasses.xml \
    resources/haarcascade_frontalcatface.xml \
    resources/haarcascade_frontalcatface_extended.xml \
    resources/haarcascade_frontalface_alt.xml \
    resources/logo_tse.png

RESOURCES += \
    res.qrc
