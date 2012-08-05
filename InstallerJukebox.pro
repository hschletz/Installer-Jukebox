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
    Downloader/sourceforge.cpp \
    version.cpp \
    Package/vlcmediaplayer.cpp \
    Package/opera.cpp \
    Package/adobereader.cpp \
    Package/libreoffice.cpp

HEADERS  += dialog.h \
    installer.h \
    Installer/nsis.h \
    package.h \
    Package/flashplayerplugin.h \
    Package/flashplayeractivex.h \
    downloader.h \
    application.h \
    command.h \
    Downloader/sourceforge.h \
    version.h \
    Package/vlcmediaplayer.h \
    Package/opera.h \
    Package/adobereader.h \
    Package/libreoffice.h

FORMS    += dialog.ui \
    command.ui

OTHER_FILES += \
    NSIS/main.nsi \
    NSIS/FlashPlayerActiveX/main.nsh \
    NSIS/FlashPlayerPlugin/main.nsh \
    InstallerJukebox.ini \
    InstallerJukebox.template.ini \
    README.txt \
    NSIS/VlcMediaPlayer/main.nsh \
    NSIS/VlcMediaPlayer/blockOnIE.nsh \
    NSIS/Opera/main.nsh \
    NSIS/Opera/speeddial.nsh \
    NSIS/Opera/speeddial_header.nsh \
    NSIS/AdobeReader/main.nsh \
    NSIS/AdobeReader/header.nsh \
    NSIS/AdobeReader/installpatch.nsh \
    NSIS/AdobeReader/disableupdater.nsh \
    NSIS/LibreOffice/main.nsh

RESOURCES += \
    NSIS/nsis.qrc
