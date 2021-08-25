QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

Target = SbConverter
TEMPLATE = app
win32 : RC_ICONS += myicon.ico
CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    account.cpp \
    csvio.cpp \
    filesdata.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    account.h \
    csvio.h \
    filesdata.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    sb.qrc
