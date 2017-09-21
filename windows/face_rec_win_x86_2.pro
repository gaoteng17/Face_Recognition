#-------------------------------------------------
#
# Project created by QtCreator 2017-09-07T17:05:52
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = face_rec_win_x86_2
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    logindialog.cpp \
    infodialog.cpp

HEADERS  += mainwindow.h \
    logindialog.h \
    infodialog.h \
    train.h

FORMS    += \
    logindialog.ui \
    mainwindow.ui \
    infodialog.ui


INCLUDEPATH += E:\opencv_build1\install\include\opencv \
        E:\opencv_build1\install\include\opencv2 \
        E:\opencv_build1\install\include

LIBS += -LE:\opencv_build1\install\x86\vc14\lib \
        -lopencv_aruco330d  \
        -lopencv_bgsegm330d \
        -lopencv_bioinspired330d    \
        -lopencv_calib3d330d    \
        -lopencv_ccalib330d     \
        -lopencv_core330d       \
        -lopencv_datasets330d   \
        -lopencv_dnn330d        \
        -lopencv_dpm330d        \
        -lopencv_face330d       \
        -lopencv_features2d330d \
        -lopencv_flann330d      \
        -lopencv_fuzzy330d      \
        -lopencv_highgui330d    \
        -lopencv_imgcodecs330d  \
        -lopencv_imgproc330d    \
        -lopencv_img_hash330d   \
        -lopencv_line_descriptor330d    \
        -lopencv_ml330d         \
        -lopencv_objdetect330d  \
        -lopencv_optflow330d    \
        -lopencv_phase_unwrapping330d   \
        -lopencv_photo330d      \
        -lopencv_plot330d       \
        -lopencv_reg330d        \
        -lopencv_rgbd330d       \
        -lopencv_saliency330d   \
        -lopencv_shape330d      \
        -lopencv_stereo330d     \
        -lopencv_stitching330d  \
        -lopencv_structured_light330d   \
        -lopencv_superres330d           \
        -lopencv_surface_matching330d   \
        -lopencv_text330d               \
        -lopencv_tracking330d           \
        -lopencv_video330d              \
        -lopencv_videoio330d            \
        -lopencv_videostab330d          \
        -lopencv_xfeatures2d330d           \
        -lopencv_ximgproc330d           \
        -lopencv_xobjdetect330d         \
        -lopencv_xphoto330d             \
