#-------------------------------------------------
#
# Project created by QtCreator 2012-07-07T13:51:55
#
#-------------------------------------------------

CONFIG += debug_and_release

QT       += core gui network

TARGET = InstallerJukebox
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp \
    installer.cpp \
    Installer/nsis.cpp \
    package.cpp \
    Package/flashplayerplugin.cpp \
    Package/flashplayeractivex.cpp \
    downloader.cpp \
    application.cpp \
    command.cpp \
    Downloader/sourceforge.cpp

HEADERS  += dialog.h \
    installer.h \
    Installer/nsis.h \
    package.h \
    Package/flashplayerplugin.h \
    Package/flashplayeractivex.h \
    downloader.h \
    application.h \
    command.h \
    Downloader/sourceforge.h

FORMS    += dialog.ui \
    command.ui

OTHER_FILES += \
    NSIS/main.nsi \
    NSIS/FlashPlayerActiveX/main.nsh \
    NSIS/FlashPlayerPlugin/main.nsh \
    InstallerJukebox.ini \
    InstallerJukebox.template.ini \
    README.txt

RESOURCES += \
    NSIS/nsis.qrc
