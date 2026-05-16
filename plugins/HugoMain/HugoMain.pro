QT += core gui widgets
DEFINES+=HU_DISABLE_INSTALLER
TEMPLATE = lib
CONFIG += plugin
CONFIG += c++2b
TARGET = HugoMain

LIBS += -luser32 -lshell32

win32 {
    CONFIG(debug, debug|release) {
        DESTDIR  = ../../src/debug/plugins/HugoWidget
    } else {
        DESTDIR  = ../../src/release/plugins/HugoWidget
    }
}
win32:CONFIG(release, debug|release): LIBS += -L../../src/release -lwecore
else:win32:CONFIG(debug, debug|release): LIBS += -L../../src/debug/ -lwecore
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target

include(../../deps/WECore/WECore.pri)
include(../../deps/HugoUtils.pri)
include(../../deps/Shared.pri)

INCLUDEPATH += ../../deps/
INCLUDEPATH += ../../deps/HugoUtils/include/

LIBS+=-lversion
LIBS+=-lws2_32
SOURCES += \
    HugoPages/HFreezeApiEx.cpp \
    HugoPages/HFreezeNone.cpp \
    HugoPages/QDiskButton.cpp \
    HugoPages/hugofreezepage.cpp \
    HugoPages/hugomountpage.cpp \
    hugomainplugin.cpp \
    hugomainwidget.cpp

HEADERS += \
    HugoPages/QDiskButton.h \
    HugoPages/hfreezeapiex.h \
    HugoPages/HFreezeNone.h \
    HugoPages/hugofreezepage.h \
    HugoPages/hugomountpage.h \
    functionpagebase.h \
    hugomainplugin.h \
    hugomainwidget.h

FORMS += \
    HugoPages/hugofreezepage.ui \
    HugoPages/hugomountpage.ui