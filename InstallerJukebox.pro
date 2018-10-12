CONFIG += debug_and_release

QT       += core gui network widgets

TARGET = InstallerJukebox
TEMPLATE = app

LIBS += -lquazip5

gcc:QMAKE_CXXFLAGS += -fPIC

SOURCES += main.cpp
SOURCES += application.cpp
SOURCES += command.cpp
SOURCES += dialog.cpp
SOURCES += downloader.cpp
SOURCES += installer.cpp
SOURCES += manualdownload.cpp
SOURCES += package.cpp
SOURCES += version.cpp
SOURCES += Downloader/sourceforge.cpp
SOURCES += Installer/nsis.cpp
SOURCES += Package/adobereader.cpp
SOURCES += Package/flashplayeractivex.cpp
SOURCES += Package/flashplayerplugin.cpp
SOURCES += Package/infrarecorder.cpp
SOURCES += Package/javaruntimeenvironment8.cpp
SOURCES += Package/libreoffice.cpp
SOURCES += Package/ocsinventoryngagent.cpp
SOURCES += Package/telegramdesktop.cpp
SOURCES += Package/vlcmediaplayer.cpp
SOURCES += Package/Mozilla/mozilla.cpp
SOURCES += Package/Mozilla/firefox.cpp
SOURCES += Package/Mozilla/thunderbird.cpp

HEADERS += application.h
HEADERS += command.h
HEADERS += dialog.h
HEADERS += downloader.h
HEADERS += package.h
HEADERS += installer.h
HEADERS += manualdownload.h
HEADERS += version.h
HEADERS += Downloader/sourceforge.h
HEADERS += Installer/nsis.h
HEADERS += Package/adobereader.h
HEADERS += Package/flashplayeractivex.h
HEADERS += Package/flashplayerplugin.h
HEADERS += Package/infrarecorder.h
HEADERS += Package/javaruntimeenvironment8.h
HEADERS += Package/libreoffice.h
HEADERS += Package/ocsinventoryngagent.h
HEADERS += Package/telegramdesktop.h
HEADERS += Package/vlcmediaplayer.h
HEADERS += Package/Mozilla/mozilla.h
HEADERS += Package/Mozilla/thunderbird.h
HEADERS += Package/Mozilla/firefox.h

FORMS += command.ui
FORMS += dialog.ui
FORMS += manualdownload.ui

OTHER_FILES += InstallerJukebox.ini
OTHER_FILES += InstallerJukebox.template.ini
OTHER_FILES += README.txt
OTHER_FILES += NSIS/main.nsi
OTHER_FILES += NSIS/AdobeReader/main.nsh
OTHER_FILES += NSIS/Firefox/main.nsh
OTHER_FILES += NSIS/Firefox/deleteprefs.nsh
OTHER_FILES += NSIS/Firefox/writeprefs.nsh
OTHER_FILES += NSIS/FlashPlayerActiveX/main.nsh
OTHER_FILES += NSIS/FlashPlayerPlugin/main.nsh
OTHER_FILES += NSIS/InfraRecorder/main.nsh
OTHER_FILES += NSIS/InfraRecorder/header.nsh
OTHER_FILES += NSIS/InfraRecorder/setversion.nsh
OTHER_FILES += NSIS/JavaRuntimeEnvironment8/main.nsh
OTHER_FILES += NSIS/JavaRuntimeEnvironment8/disableautoupdate.nsh
OTHER_FILES += NSIS/JavaRuntimeEnvironment8/uninstallpreviousversions.nsh
OTHER_FILES += NSIS/LibreOffice/main.nsh
OTHER_FILES += NSIS/LibreOffice/configtemplate.nsh
OTHER_FILES += NSIS/Mozilla/local-settings.js
OTHER_FILES += NSIS/OcsInventoryNgAgent/main.nsh
OTHER_FILES += NSIS/OcsInventoryNgAgent/installcert.nsh
OTHER_FILES += NSIS/OcsInventoryNgAgent/installplugin.nsh
OTHER_FILES += NSIS/Thunderbird/main.nsh
OTHER_FILES += NSIS/Thunderbird/accountconfig.nsh
OTHER_FILES += NSIS/Thunderbird/deleteprefs.nsh
OTHER_FILES += NSIS/Thunderbird/writeprefs.nsh
OTHER_FILES += NSIS/Thunderbird/uninstalltestpilot.nsh
OTHER_FILES += NSIS/VlcMediaPlayer/main.nsh
OTHER_FILES += NSIS/VlcMediaPlayer/blockOnIE.nsh
OTHER_FILES += NSIS/VlcMediaPlayer/uninstallActiveXPlugin.nsh \
OTHER_FILES += NSIS/VlcMediaPlayer/uninstallMozillaPlugin.nsh

RESOURCES += NSIS/nsis.qrc
