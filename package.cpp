/*
 * Copyright (c) 2012 Holger Schletz <holger.schletz@web.de>
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QDirIterator>
#include <QFile>
#include <QFileInfo>
#include "application.h"
#include "command.h"
#include "installer.h"
#include "package.h"
#include "Package/adobereader.h"
#include "Package/Mozilla/firefox.h"
#include "Package/flashplayerplugin.h"
#include "Package/flashplayeractivex.h"
#include "Package/infrarecorder.h"
#include "Package/javaruntimeenvironment8.h"
#include "Package/libreoffice.h"
#include "Package/ocsinventoryngagent.h"
#include "Package/telegramdesktop.h"
#include "Package/Mozilla/thunderbird.h"
#include "Package/vlcmediaplayer.h"

Package::Package(const char *name, const char *minVersion) :
    QObject(0),
    minVersion(minVersion)
{
    setObjectName(name);
}


Package::~Package()
{
    cleanup();
}


QMap<QString, Package *> Package::getPackages()
{
    QMap<QString, Package *> packages;
    QString packageName;
    Package *package;

    // Iterate all section names from the config file ("General" is skipped automatically)
    foreach (packageName, Application::getConfigGroups()) {
        // Generate matching subclass for every section found
        package = 0;
        if (packageName.compare("Adobe Acrobat Reader DC", Qt::CaseInsensitive) == 0) {
            package = new AdobeReader();
        } else if (packageName.compare("Firefox", Qt::CaseInsensitive) == 0) {
            package = new Firefox();
        } else if (packageName.compare("Flash Player Plugin", Qt::CaseInsensitive) == 0) {
            package = new FlashPlayerPlugin();
        } else if (packageName.compare("Flash Player ActiveX", Qt::CaseInsensitive) == 0) {
            package = new FlashPlayerActiveX();
        } else if (packageName.compare("InfraRecorder", Qt::CaseInsensitive) == 0) {
            package = new InfraRecorder();
        } else if (packageName.compare("Java Runtime Environment 8", Qt::CaseInsensitive) == 0) {
            package = new JavaRuntimeEnvironment8();
        } else if (packageName.compare("LibreOffice", Qt::CaseInsensitive) == 0) {
            package = new LibreOffice();
        } else if (packageName.compare("OCS Inventory NG agent", Qt::CaseInsensitive) == 0) {
            package = new OcsInventoryNgAgent();
        } else if (packageName.compare("Telegram Desktop", Qt::CaseInsensitive) == 0) {
            package = new TelegramDesktop();
        } else if (packageName.compare("Thunderbird", Qt::CaseInsensitive) == 0) {
            package = new Thunderbird();
        } else if (packageName.compare("VLC Media Player", Qt::CaseInsensitive) == 0) {
            package = new VlcMediaPlayer();
        } else if (!packageName.startsWith("installer_")) { // "installer_*" are special sections, not a package
            qDebug() << "Unknown package:" << packageName;
        }

        // Add to list
        if (package) {
            packages[packageName] = package;
            qDebug() << "Add package:" << packageName;
        }
    }
    return packages;
}


Version Package::getMinVersion()
{
    return minVersion;
}


void Package::build(QString version)
{
    if (getOutputFile().isEmpty()) {
        return;
    }
    // Get installer and call appropriate overload depending on installer type
    Installer *installer = Installer::getInstaller();
    QString type = installer->metaObject()->className();
    if (type == "NSIS") {
        build((NSIS *) installer, version);
    } else {
        Application::critical(tr("Invalid installer: %1").arg(type));
    }
    delete installer;
}


void Package::build(NSIS *installer, Version version)
{
    Q_UNUSED(version);

    // Default implementation - override to actually build anything
    qDebug() << installer->metaObject()->className() << "not implemented for this package";
    Application::critical(tr("No NSIS installer available for this package."));
}


QString Package::getOutputFile()
{
    // Get "Output file" option from the corresponding section (objectName() returns package name)
    QString outputFile = getConfig("Output file").toString();
    if (outputFile.isEmpty()) {
        qDebug() << "Output file not configured!";
        Application::critical(tr("Output file is not configured!"));
        return QString();
    }
    qDebug() << "Output file:" << outputFile;

    QFileInfo fileInfo(outputFile);
    if (fileInfo.isDir()) {
        qDebug() << "Error: is a directory!";
        Application::critical(tr("Configured output file is invalid!"));
        return QString();
    }

    if (fileInfo.exists()) {
        qDebug() << "File exists";
        if (Application::question(
                    tr("The file '%1' already exists.\n\nModified:\t%2\nSize:\t%3 KiB\n\nOverwrite?")
                    .arg(outputFile)
                    .arg(fileInfo.lastModified().toLocalTime().toString(Qt::DefaultLocaleLongDate))
                    .arg(fileInfo.size() >> 10),
                    QMessageBox::Yes | QMessageBox::No,
                    QMessageBox::No
                    )
                == QMessageBox::No
        ) {
            qDebug() << "Aborted.";
            return QString();
        } else {
            qDebug() << "Delete old file.";
            QFile::remove(outputFile);
        }
    }

    return outputFile;
}


QString Package::loadResource(QString name)
{
    QFile resource(name);
    if (!resource.open(QIODevice::ReadOnly)) {
        throw; // Internal error, invalid resource (should not happen in production)
    }
    return QTextStream(&resource).readAll();
}


QStringList Package::browsers()
{
    QStringList browsers;
    browsers << "firefox.exe";
    browsers << "chrome.exe";
    browsers << "opera.exe";
    browsers << "seamonkey.exe";
    return browsers;
}


void Package::cleanup()
{
    if (Application::keepTempFiles() == Application::Never ||
            (Application::keepTempFiles() == Application::OnError && !isError)
            ) {
        qDebug() << "Cleanup requested by config";
        QString file;
        foreach (file, tempFiles) {
            rmPath(file);
        }
        tempFiles.clear();
    }
}


bool Package::rmPath(QString path)
{
    /* Since rmdir() is not static, a QDir object must be instantiated. For
     * performance reasons this is made static so that it is instantiated only
     * once instead on every recursion. */
    static QDir dirObject;
    bool success;

    if (QFileInfo(path).isDir()) {
        QDirIterator iterator(path, QDir::AllEntries | QDir::Hidden | QDir::NoDotAndDotDot);
        success = true;
        while (success && iterator.hasNext()) {
            success = rmPath(iterator.next());
        }
        if (success) {
            success = dirObject.rmdir(path);
        }
    } else {
        success = QFile::remove(path);
    }
    if (!success) {
        Application::critical(tr("Could not delete '%1'. Aborting").arg(path));
    }
    return success;
}


bool Package::extract7zArchive(QString archive)
{
    QString program = Application::getConfig("7z extraction command", "7z").toString();
    QString args = Application::getConfig("7z extraction args", "x -o%TARGETDIR% %ARCHIVE%").toString();
    qDebug() << "7z extraction command:" << program + " " + args;

    if (!args.contains("%TARGETDIR%") || !args.contains("%ARCHIVE%")) {
        Application::critical(tr("Invalid 7z extraction arguments: '%1'").arg(args));
        return false;
    }

    Command::cmdSpec cmdSpec;
    cmdSpec.program = program;
    cmdSpec.args = args.split(" ", QString::SkipEmptyParts);
    cmdSpec.args.replaceInStrings("%TARGETDIR%", Application::getTmpDir());
    cmdSpec.args.replaceInStrings("%ARCHIVE%", archive);

    Command command;
    command.commands << cmdSpec;
    command.closeOnSuccess = true;
    command.exec();
    return !command.hasErrors();
}


QVariant Package::getConfig(const QString &key, const QVariant &defaultValue)
{
    return Application::getConfig(objectName() + "/" + key, defaultValue);
}
