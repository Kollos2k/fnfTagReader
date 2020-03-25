QT       += core gui sql printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
RC_ICONS = logo.ico
# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    classes/Holder.cpp \
    classes/configdb.cpp \
    classes/scanner.cpp \
    importwindow/ImportWindow.cpp \
    infowindow/InfoWindow.cpp \
    main.cpp \
    mainwindow/TableModelGroupChips.cpp \
    mainwindow/TableModelGroupChipsProxy.cpp \
    mainwindow/TableModelGroupChipsRow.cpp \
    mainwindow/TableModelKnownChips.cpp \
    mainwindow/TableModelKnownChipsProxy.cpp \
    mainwindow/TableModelKnownChipsRow.cpp \
    mainwindow/TableModelSeenChips.cpp \
    mainwindow/TableModelSeenChipsProxy.cpp \
    mainwindow/TableModelSeenChipsRow.cpp \
    mainwindow/mainwindow.cpp

HEADERS += \
    classes/Holder.h \
    classes/configdb.h \
    classes/scanner.h \
    importwindow/ImportWindow.h \
    infowindow/InfoWindow.h \
    mainwindow/TableModelGroupChips.h \
    mainwindow/TableModelGroupChipsProxy.h \
    mainwindow/TableModelGroupChipsRow.h \
    mainwindow/TableModelKnownChips.h \
    mainwindow/TableModelKnownChipsProxy.h \
    mainwindow/TableModelKnownChipsRow.h \
    mainwindow/TableModelSeenChips.h \
    mainwindow/TableModelSeenChipsProxy.h \
    mainwindow/TableModelSeenChipsRow.h \
    mainwindow/mainwindow.h \
    libs/header/rr/tagReader.h

FORMS += \
    importwindow/ImportWindow.ui \
    infowindow/InfoWindow.ui \
    mainwindow/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


INCLUDEPATH += $$PWD/libs/header
DEPENDPATH += $$PWD/libs/header

#win32: LIBS += -L$$PWD/libs/link/rr/ -lEasyPOD

INCLUDEPATH += $$PWD/libs/link/rr
DEPENDPATH += $$PWD/libs/link/rr

win32: LIBS += -lws2_32 -lwsock32

#win32: LIBS += -L$$PWD/libs/link/boost/ -lboost_filesystem-mgw73-mt-x32-1_72
#win32: LIBS += -L$$PWD/libs/link/boost/ -lboost_system-mgw73-mt-x32-1_72
#win32: LIBS += -L$$PWD/libs/link/boost/ -lboost_chrono-mgw73-mt-x32-1_72
#win32: LIBS += -L$$PWD/libs/link/boost/ -lboost_date_time-mgw73-mt-x32-1_72
#win32: LIBS += -L$$PWD/libs/link/boost/ -lboost_thread-mgw73-mt-x32-1_72

RESOURCES += \
    images.qrc
