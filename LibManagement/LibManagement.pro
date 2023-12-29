QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addbook.cpp \
    addmember.cpp \
    bookslist.cpp \
    borrowbook.cpp \
    editbook.cpp \
    editmembers.cpp \
    loginscreen.cpp \
    main.cpp \
    mainmenu.cpp \
    returnbook.cpp \
    splashscreen.cpp \
    startscreen.cpp \
    viewmembers.cpp

HEADERS += \
    addbook.h \
    addmember.h \
    bookslist.h \
    borrowbook.h \
    editbook.h \
    editmembers.h \
    loginscreen.h \
    mainmenu.h \
    returnbook.h \
    splashscreen.h \
    startscreen.h \
    viewmembers.h

FORMS += \
    addbook.ui \
    addmember.ui \
    bookslist.ui \
    borrowbook.ui \
    editbook.ui \
    editmembers.ui \
    loginscreen.ui \
    mainmenu.ui \
    returnbook.ui \
    splashscreen.ui \
    startscreen.ui \
    viewmembers.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Pictures.qrc

DISTFILES += \
    pics/LibTechLogo.png \
    pics/loginscren.png
