QT += core gui widgets qml
CONFIG += c++20
RC_ICONS = icon/we_tp.ico

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/release/ -lwecore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/debug/ -lwecore
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
INCLUDEPATH +=../deps/WECore/include
include( ../deps/WECore/WECore.pri)
include( ../deps/Qlementine.pri)
INCLUDEPATH +=\
    ../deps
SOURCES += \
    lightsystem.cpp \
    lightwidget.cpp \
    main.cpp \
    querymaindialog.cpp
HEADERS += \
    lightsystem.h \
    lightwidget.h \
    querymaindialog.h
FORMS += \
    querymaindialog.ui
RESOURCES += \
    resourse.qrc
