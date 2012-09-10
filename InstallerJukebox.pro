#-------------------------------------------------
#
# Project created by QtCreator 2012-07-07T13:51:55
#
#-------------------------------------------------

CONFIG += debug_and_release

QT       += core gui network

TARGET = InstallerJukebox
TEMPLATE = app

LIBS += -lquazip

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
    Package/libreoffice.cpp \
    Package/Mozilla/mozilla.cpp \
    Package/Mozilla/thunderbird.cpp \
    Package/Mozilla/firefox.cpp \
    Package/ocsinventoryngagent.cpp \
    Package/javaruntimeenvironment6.cpp \
    manualdownload.cpp

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
    Package/libreoffice.h \
    Package/Mozilla/mozilla.h \
    Package/Mozilla/thunderbird.h \
    Package/Mozilla/firefox.h \
    Package/ocsinventoryngagent.h \
    Package/javaruntimeenvironment6.h \
    manualdownload.h

FORMS    += dialog.ui \
    command.ui \
    manualdownload.ui

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
    NSIS/LibreOffice/main.nsh \
    NSIS/LibreOffice/configtemplate.nsh \
    NSIS/Thunderbird/main.nsh \
    NSIS/Thunderbird/deleteprefs.nsh \
    NSIS/Thunderbird/writeprefs.nsh \
    NSIS/Mozilla/local-settings.js \
    NSIS/Thunderbird/uninstalltestpilot.nsh \
    NSIS/Thunderbird/accountconfig.nsh \
    NSIS/Firefox/main.nsh \
    NSIS/Firefox/deleteprefs.nsh \
    NSIS/Firefox/writeprefs.nsh \
    NSIS/Firefox/uninstallmaintenanceservice.nsh \
    NSIS/OcsInventoryNgAgent/main.nsh \
    NSIS/OcsInventoryNgAgent/installcert.nsh \
    NSIS/JavaRuntimeEnvironment6/main.nsh \
    NSIS/JavaRuntimeEnvironment6/disablequickstarter.nsh \
    NSIS/JavaRuntimeEnvironment6/disableautoupdate.nsh \
    NSIS/OcsInventoryNgAgent/installplugin.nsh

RESOURCES += \
    NSIS/nsis.qrc
