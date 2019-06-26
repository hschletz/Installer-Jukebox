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

#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <quazip5/quazip.h>
#include <quazip5/quazipfile.h>
#include "application.h"
#include "downloader.h"
#include "ocsinventoryngagent.h"

OcsInventoryNgAgent::OcsInventoryNgAgent() :
    Package("OCS Inventory NG agent", "2.6.0.0")
{
}


void OcsInventoryNgAgent::build(NSIS *installer, Version version)
{
    isError = false;

    download(version);
    if (!isError) {
        QStringList files(originalInstaller);

        QStringList options(getConfig("Installer options").toString().split(" ", QString::SkipEmptyParts));
        if (!options.contains("/NOSPLASH")) {
            options.prepend("/NOSPLASH");
        }
        if (!options.contains("/S")) {
            options.prepend("/S");
        }
        qDebug() << "Installer options:" << options;

        QString src(loadResource(":NSIS/OcsInventoryNgAgent/main.nsh"));
        src.replace("${Options}", options.join(" "));

        QString cacert(getConfig("CA certificate").toString());
        if (!cacert.isEmpty()) {
            src += loadResource(":NSIS/OcsInventoryNgAgent/installcert.nsh")
                    .replace("${Cert}", QFileInfo(cacert).fileName());
            files << cacert;
        }

        // Iterate over all options starting with "Plugin". Their value will be
        // used as a path to a plugin which gets bundled with the installer.
        QString pluginKey;
        foreach (pluginKey, Application::getConfigGroupKeys(objectName()).filter(QRegExp("^Plugin", Qt::CaseInsensitive)))
        {
            QString plugin(getConfig(pluginKey).toString());
            src += loadResource(":NSIS/OcsInventoryNgAgent/installplugin.nsh")
                    .replace("${Plugin}", QFileInfo(plugin).fileName());
            files << plugin;
        }

        installer->build(
                    objectName(),
                    getOutputFile(),
                    NSIS::Zlib,
                    20,
                    QStringList(),
                    files,
                    src
                    );
    }
    cleanup();
}


void OcsInventoryNgAgent::download(Version version)
{
    QString archive(
                Downloader::get(
                    QString("https://github.com/OCSInventory-NG/WindowsAgent/releases/download/%1/OCS-Windows-Agent-%2.zip")
                    .arg(version)
                    .arg(version.pad(4, "0")),
                    Application::getTmpDir()
                    )
                );
    if (archive.isEmpty()) {
        isError = true;
        return;
    }
    tempFiles << archive;

    QuaZipFile file(
                archive,
                QString("OCS-Windows-Agent-%1/OCS-NG-Windows-Agent-Setup.exe").arg(version.pad(4, "0"))
                );
    if (!file.open(QIODevice::ReadOnly) or file.usize() == -1) {
        Application::critical("Error opening archive!");
        isError = true;
        return;
    }
    qDebug() << "Archive opened.";

    originalInstaller = Application::getTmpDir() + "/OCS-NG-Windows-Agent-Setup.exe";
    QFile target(originalInstaller);
    if (!target.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        Application::critical("Error opening target file!");
        isError = true;
        return;
    }
    qDebug() << "Opened" << originalInstaller;
    tempFiles << originalInstaller;

    if (target.write(file.readAll()) != file.usize()) {
        Application::critical("Error extracting file!");
        isError = true;
        return;
    }
    qDebug() << "Extraction successful.";
}
