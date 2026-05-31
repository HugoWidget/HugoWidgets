QT += core gui widgets
DEFINES += HU_DISABLE_INSTALLER
TEMPLATE = lib
CONFIG += plugin
CONFIG += c++2b
TARGET = HugoWidgetsPlugin

CONFIG(debug, debug|release) {
    DESTDIR  = ../../src/debug/plugins/HugoWidget
} else {
    DESTDIR  = ../../src/release/plugins/HugoWidget
}

CONFIG(release, debug|release): LIBS += -L../../src/release -lwecore
CONFIG(debug, debug|release): LIBS += -L../../src/debug/ -lwecore
LIBS += -luser32 -lshell32 -lversion -lws2_32

!isEmpty(target.path): INSTALLS += target

include(../../deps/WECore/WECore.pri)
include(../../deps/HugoUtils.pri)

INCLUDEPATH += ../../deps/
INCLUDEPATH += ../../deps/HugoUtils/include/


SOURCES += \
    HugoPages/HFreezeApiEx.cpp \
    HugoPages/HFreezeNone.cpp \
    HugoPages/HugoLaunchToolPage.cpp \
    HugoPages/HugoPasswordPage.cpp \
    HugoPages/HugoProtectPage.cpp \
    HugoPages/QDiskButton.cpp \
    HugoPages/HugoFreezePage.cpp \
    HugoPages/HugoMountPage.cpp \
    HugoWidgetsPlugin.cpp \

HEADERS += \
    ../HugoMain/FunctionPageBase.h \
    HugoPages/HFreezeNone.h \
    HugoPages/HugoLaunchToolPage.h \
    HugoPages/HugoPasswordPage.h \
    HugoPages/HugoProtectPage.h \
    HugoPages/QDiskButton.h \
    HugoPages/HFreezeApiEx.h \
    HugoPages/HugoFreezePage.h \
    HugoPages/HugoMountPage.h \
    HugoWidgetsPlugin.h \

FORMS += \
    HugoPages/HugoLaunchToolPage.ui \
    HugoPages/HugoFreezePage.ui \
    HugoPages/HugoMountPage.ui \