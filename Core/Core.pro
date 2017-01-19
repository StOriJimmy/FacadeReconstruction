#-------------------------------------------------
#
# Project created by QtCreator 2014-04-29T11:07:15
#
#-------------------------------------------------

MODULE_NAME = CurveCore

QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.9 #needed for opencv ONLY when compiling on macosx > 10.10

QMAKE_CXXFLAGS += -std=c++11
QMAKE_CXXFLAGS -= -std=c++0x

# Include shared options
include (../code.pri)

QT       -= gui

TARGET = Core
TEMPLATE = lib
CONFIG += dynamiclib
CONFIG += c++11

SOURCES += \
    Sample.cpp \
    Matrix.cpp \
    Quadric.cpp \
    Triangulate.cpp \
    PolyGTM.cpp \
    main.cpp

HEADERS += \
    Matrix.h \
    Sample.h \
    Quadric.h \
    Triangulate.h \
    PolyGTM.h

CONFIG(debug, debug|release): DEFINES += DEBUG
CONFIG(release, debug|release): DEFINES += RELEASE

QMAKE_CXXFLAGS_RELEASE -= -O
QMAKE_CXXFLAGS_RELEASE -= -O1
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE *= -Ofast

#LIBS += -L$$PWD/../../lib/CurveExtraction/lib/ -lcurves
#LIBS += -L/usr/local/lib -lopencv_calib3d.2.4.8 -lopencv_contrib -lopencv_core -lopencv_features2d -lopencv_flann -lopencv_gpu -lopencv_highgui -lopencv_imgproc -lopencv_legacy.2.4.8 -lopencv_ml -lopencv_nonfree -lopencv_objdetect -lopencv_ocl -lopencv_photo -lopencv_stitching -lopencv_superres -lopencv_video -lopencv_videostab

