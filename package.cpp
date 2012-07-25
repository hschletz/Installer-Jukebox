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
#include <QFileInfo>
#include "application.h"
#include "installer.h"
#include "package.h"
#include "Package/flashplayerplugin.h"
#include "Package/flashplayeractivex.h"

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
        if (packageName.compare("Flash Player Plugin", Qt::CaseInsensitive) == 0) {
            package = new FlashPlayerPlugin();
        } else if (packageName.compare("Flash Player ActiveX", Qt::CaseInsensitive) == 0) {
            package = new FlashPlayerActiveX();
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
    QString outputFile = Application::getConfig(objectName() + "/Output file").toString();
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
            QFile::remove(file);
        }
        tempFiles.clear();
    }
}
