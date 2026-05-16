QT += core gui

CONFIG += c++17

include(QXlsx/QXlsx.pri)
INCLUDEPATH += $$PWD/Shared
HEADERS += \
    $$PWD/Shared/flowlayout.h \
    $$PWD/Shared/sidebar.h \

SOURCES += \
    $$PWD/Shared/flowlayout.cpp \
    $$PWD/Shared/sidebar.cpp \


