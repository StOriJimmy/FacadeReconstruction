MODULE_NAME = GUI

QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.9 #needed for opencv ONLY when compiling on macosx > 10.10

QMAKE_CXXFLAGS += -stdlib=libc++
QMAKE_CXXFLAGS += -std=c++11
QMAKE_CXXFLAGS -= -std=c++0x

# Include shared options
include (../code.pri)

# Input

# Module headers
HEADERS += common.hpp \
    engine.hpp \
    mainwindow.hpp \
    gl_photo.hpp \
    gl_trackball.hpp \
    photo_widget.hpp \
    trackball_widget.hpp \
    datastore.hpp \
    user_input.hpp \
    back_end.hpp \
    sketch_selection_dialog.hpp \
    edge_selection_dialog.hpp \
    panzoomer.hpp \


# Module sources
SOURCES += main.cpp \
    engine.cpp \
    mainwindow.cpp \
    gl_photo.cpp \
    gl_trackball.cpp \
    photo_widget.cpp \
    trackball_widget.cpp \
    datastore.cpp \
    user_input.cpp \
    back_end.cpp \
    sketch_selection_dialog.cpp \
    edge_selection_dialog.cpp \
    panzoomer.cpp \


# Module resources
RESOURCES += resources.qrc

#### LIBS:

## OPENCV IN WINDOWS (REPLACE THE PATH IF NECESSARY)
#win32{
#INCLUDEPATH += C:/opencv/build/include
#}

##Lua related stuff
##LUA_DIR = /usr/local/Cellar/lua/5.1.4
#LUA_DIR = ../libs/LUA/lua5.1
#INCLUDEPATH += $$LUA_DIR/include

##### We compile lua statically in order to better debug problems
#!win32{
#LIBS += -llua
#LIBS += -L$$LUA_DIR/lib
#}
#win32{
#LIBS += -llua5.1
#LIBS += -L$$LUA_DIR/lib
#}


#CUTELOGGER stuff

#DEFINES += CUTELOGGER_LIBRARY

INCLUDEPATH += $$QTLOG_DIR/include

SOURCES += $$QTLOG_DIR/src/Logger.cpp
SOURCES += $$QTLOG_DIR/src/AbstractAppender.cpp
SOURCES += $$QTLOG_DIR/src/AbstractStringAppender.cpp
SOURCES += $$QTLOG_DIR/src/ConsoleAppender.cpp
SOURCES += $$QTLOG_DIR/src/FileAppender.cpp

HEADERS += $$QTLOG_DIR/include/Logger.h
HEADERS += $$QTLOG_DIR/include/CuteLogger_global.h
HEADERS += $$QTLOG_DIR/include/AbstractAppender.h
HEADERS += $$QTLOG_DIR/include/AbstractStringAppender.h
HEADERS += $$QTLOG_DIR/include/ConsoleAppender.h
HEADERS += $$QTLOG_DIR/include/FileAppender.h

win32 {
    SOURCES += $$QTLOG_DIR/src/OutputDebugAppender.cpp
    HEADERS += $$QTLOG_DIR/include/OutputDebugAppender.h
}

#utils
INCLUDEPATH += ../Utils
HEADERS += ../Utils/utils.hpp
HEADERS += ../Utils/glutils.hpp
HEADERS += ../Utils/vcgutils.hpp
#HEADERS += ../Utils/cvutils.hpp
HEADERS += ../Utils/spline.hpp
SOURCES += ../Utils/utils.cpp
SOURCES += ../Utils/glutils.cpp
SOURCES += ../Utils/vcgutils.cpp
#SOURCES += ../Utils/cvutils.cpp

##vcg lib related stuff
INCLUDEPATH += $$VCG_DIR
SOURCES += $$VCG_DIR/wrap/ply/plylib.cpp
SOURCES += $$VCG_DIR/wrap/gui/trackball.cpp
SOURCES += $$VCG_DIR/wrap/gui/trackmode.cpp

LIBS += -lCore

##symbian {
##    MMP_RULES += EXPORTUNFROZEN
##    TARGET.UID3 = 0xE8FB3D8D
##    TARGET.CAPABILITY =
##    TARGET.EPOCALLOWDLLDATA = 1
##    addFiles.sources = CuteLogger.dll
##    addFiles.path = !:/sys/bin
##    DEPLOYMENT += addFiles
##}

##unix:!symbian {
##    maemo5 {
##        target.path = /opt/usr/lib
##    } else {
##        target.path = /usr/lib
##    }
##    INSTALLS += target
##}


##QLEAP_DIR = ../libs/qleap/src/QLeapGui

##SOURCES += $$QLEAP_DIR/QLeap.cpp
##SOURCES += $$QLEAP_DIR/QLeapController.cpp
##SOURCES += $$QLEAP_DIR/QLeapListener.cpp
##SOURCES += $$QLEAP_DIR/QLeapMapper.cpp

##HEADERS += $$QLEAP_DIR/QLeap.h
##HEADERS += $$QLEAP_DIR/QLeapController.h
##HEADERS += $$QLEAP_DIR/QLeapController_p.h
##HEADERS += $$QLEAP_DIR/QLeapGui.h
##HEADERS += $$QLEAP_DIR/QLeapGuiExport.h
##HEADERS += $$QLEAP_DIR/QLeapListener.h
##HEADERS += $$QLEAP_DIR/QLeapListener_p.h
##HEADERS += $$QLEAP_DIR/QLeapMapper.h

#LEAP_DIR = ../libs/LeapSDK
#INCLUDEPATH += $$LEAP_DIR/include
#LIBS += -L$$LEAP_DIR/lib
#LIBS += -lLeap

#QMAKE_POST_LINK = "install_name_tool -change @loader_path/libLeap.dylib @executable_path/../libs/LeapSDK/lib/libLeap.dylib ../bin/prototype07"
