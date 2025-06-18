QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++20

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    algorithm.cpp \
    drawer.cpp \
    funcs.cpp \
    funcs_list.cpp \
    main.cpp \
    mainwindow.cpp \
    mygraphicsscene.cpp \
    point.cpp

HEADERS += \
    algorithm.h \
    drawer.h \
    funcs.h \
    funcs_list.h \
    mainwindow.h \
    mygraphicsscene.h \
    point.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
