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
#include "application.h"
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QNetworkProxyFactory>

Application::Application(int &argc, char **argv) :
    QApplication(argc, argv)
{
    setApplicationName("Installer Jukebox");

    // All paths are relative to the location of the executable
    QString appPath = applicationDirPath();
    qDebug() << "Application path:" << appPath;
    QDir::addSearchPath("app", appPath);

    // Parse configuration file (required)
    if (!QFileInfo("app:InstallerJukebox.ini").isReadable()) {
        QString configPath = appPath + QDir::separator() + "InstallerJukebox.ini";
        qDebug() << "Cannot read" << configPath;
        critical(tr("Cannot read %1").arg(configPath));
        throw 1;
    }
    config = new QSettings("app:InstallerJukebox.ini", QSettings::IniFormat);

    // Get working directory from config. Must be configured, exist and be writable.
    tmpDir = config->value("Working directory").toString();
    if (tmpDir.isEmpty()) {
        qDebug() << "Working directory not configured!";
        critical(tr("Working directory is not configured!"));
        throw 1;
    }
    QFileInfo fileInfo(tmpDir);
    if (!fileInfo.isDir()) {
        qDebug() << tmpDir << "is not a directory!";
        Application::critical(tr("'%1' is not a directory!").arg(tmpDir));
        throw 1;
    }
    if (!fileInfo.isWritable()) {
        qDebug() << tmpDir << "is not writable!";
        Application::critical(tr("Directory '%1' is not writable!").arg(tmpDir));
        throw 1;
    }
    tmpDir = QDir::cleanPath(fileInfo.absoluteFilePath());
    qDebug() << "tmpDir is" << tmpDir;

    // Parse "Keep temp files" option. Must be valid if configured, default to "Never".
    QString keepTempFiles = config->value("Keep temp files", "never").toString().toLower();
    qDebug() << "Keep temp files:" << keepTempFiles;
    if (keepTempFiles == "never") {
        tmpFileHandling = Never;
    } else if (keepTempFiles == "onerror") {
        tmpFileHandling = OnError;
    } else if (keepTempFiles == "always") {
        tmpFileHandling = Always;
    } else {
        qDebug() << "Invalid!";
        critical(tr("'Keep temp files' setting is invalid!"));
        throw 1;
    }

    // Use system's default proxy configuration
    QNetworkProxyFactory::setUseSystemConfiguration(true);
}


void Application::critical(QString message)
{
    QMessageBox::critical(0, applicationName(), message);
}


void Application::information(QString message)
{
    QMessageBox::information(0, applicationName(), message);
}


QMessageBox::StandardButton Application::question(
        const QString &message,
        QMessageBox::StandardButtons buttons,
        QMessageBox::StandardButton defaultButton
        )
{
    return QMessageBox::question(0, applicationName(), message, buttons, defaultButton);
}


QVariant Application::getConfig(const QString & key, const QVariant & defaultValue)
{
    QVariant value = ((Application *) qApp)->config->value(key, defaultValue);
    if (value.toString().isEmpty()) {
        value = QVariant(defaultValue);
    }
    return value;
}


QStringList Application::getConfigGroups()
{
    return ((Application *) qApp)->config->childGroups();
}


QString Application::getTmpDir()
{
    return ((Application *) qApp)->tmpDir;
}


Application::tmpFileCleanup Application::keepTempFiles()
{
    return ((Application *) qApp)->tmpFileHandling;
}
