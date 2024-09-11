QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    browser/plaintext.cpp \
    file/fileread.cpp \
    file/jsonreader.cpp \
    operation/generatefilelist.cpp \
    operation/listoperation.cpp \
    operation/progress.cpp \
    window/mainwindow.cpp \
    main.cpp \

HEADERS += \
    browser/plaintext.h \
    file/fileread.h \
    file/jsonreader.h \
    operation/generatefilelist.h \
    operation/listoperation.h \
    operation/progress.h \
    window/mainwindow.h

FORMS += \
    window/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
