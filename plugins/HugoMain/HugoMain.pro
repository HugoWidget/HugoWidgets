QT += core gui widgets
DEFINES += HU_DISABLE_INSTALLER
TEMPLATE = lib
CONFIG += plugin
CONFIG += c++2b
TARGET = HugoMain

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
include(../../deps/Shared.pri)

INCLUDEPATH += ../../deps/
INCLUDEPATH += ../../deps/HugoUtils/include/

SOURCES += \
    hugomainplugin.cpp \
    hugomainwidget.cpp

HEADERS += \
    functionpagebase.h \
    hugomainplugin.h \
    hugomainwidget.h

FORMS += \