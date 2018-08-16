### Usage note:
# please define:
# MODULE_NAME
# before including this pri

# bin & build dirs
DESTDIR     = ../bin
BUILD_DIR   = ../build/$$MODULE_NAME
UI_DIR      = $$BUILD_DIR/ui
MOC_DIR     = $$BUILD_DIR/moc
OBJECTS_DIR = $$BUILD_DIR/obj
RCC_DIR     = $$BUILD_DIR/rcc

LIBS_DIR = ../libs
# OPENCV_DIR = /usr/local/opt/opencv/
VCG_DIR  = $$LIBS_DIR/vcglib
EIGEN_DIR = $$LIBS_DIR/eigen-git-mirror
QTLOG_DIR = $$LIBS_DIR/cutelogger

# Executable name
TARGET = $$MODULE_NAME

# Base options
TEMPLATE = app
LANGUAGE = C++

# no app bundles for mac
CONFIG-=app_bundle

# STL support is enabled
CONFIG += stl

### QT modules
QT += opengl
QT += xml

# Awful..
win32{
  DEFINES += NOMINMAX
}

# The following define is needed in gcc to remove the asserts
win32-g++:DEFINES += NDEBUG
CONFIG(debug, debug|release) {
  win32-g++:release:DEFINES -= NDEBUG
}

# Lib headers
INCLUDEPATH += .
INCLUDEPATH += ../Core

# Opencv
INCLUDEPATH += $$OPENCV_DIR/include/
DEPENDPATH += $$OPENCV_DIR/include/

# Eigen
INCLUDEPATH += $$EIGEN_DIR
DEPENDPATH += $$EIGEN_DIR

INCLUDEPATH += $$LIBS_DIR/glew/include

LIBS += -L../bin

##opencv related stuff
!win32{
LIBS += -L/usr/local/lib
LIBS += -lopencv_core
LIBS += -lopencv_highgui
LIBS += -lopencv_imgproc
}

## OPENCV IN WINDOWS (REPLACE ACCORDING TO YOUR INSTALLATION IF NECESSARY)
#win32 {
# LIBS += -LC:/opencv/build/x86/vc10/lib
# CONFIG(debug, debug|release) {
#  LIBS += -lopencv_core243d
#  LIBS += -lopencv_imgproc243d
# }
# else {
#  LIBS += -lopencv_core243
#  LIBS += -lopencv_imgproc243
# }
#}

##This flag is needed in order to compile opencv, sorry about that.
#unix:*-g++*: QMAKE_CXXFLAGS += -fpermissive

