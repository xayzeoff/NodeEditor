QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    connector.cpp \
    dragwidget.cpp \
    graphicsview.cpp \
    ifelsewidget.cpp \
    lineitem.cpp \
    main.cpp \
    mainwindow.cpp \
    node.cpp \
    nodeeditor.cpp \
    rightframe.cpp

HEADERS += \
    connector.h \
    dragwidget.h \
    graphicsview.h \
    ifelsewidget.h \
    lineitem.h \
    mainwindow.h \
    node.h \
    nodeeditor.h \
    rightframe.h \
    semantictree.hpp

FORMS += \
    mainwindow.ui \
    rightframe.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

RC_ICONS = iconka.ico
