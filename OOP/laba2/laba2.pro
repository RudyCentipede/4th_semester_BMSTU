QT = core

CONFIG += c++20 cmdline

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        MatrixBase.cpp \
        exception.cpp \
        main.cpp
        concept.h


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    ConstMatrixIterator.h \
    ConstMatrixIterator.hpp \
    Matrix.h \
    Matrix.hpp \
    MatrixBase.h \
    MatrixIterator.h \
    MatrixIterator.hpp \
    MatrixIteratorBase.h \
    MatrixIteratorBase.hpp \
    concept.h \
    exception.h
