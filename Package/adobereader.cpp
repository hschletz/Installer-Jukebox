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
#include <QFileInfo>
#include "adobereader.h"
#include "application.h"
#include "downloader.h"

AdobeReader::AdobeReader() :
    Package("Adobe Reader", "10.1.3")
{
}


void AdobeReader::build(NSIS *installer, Version version)
{
    isError = false;
    download(version);
    if (!isError) {
        // Compose options for msiexec command line
        QStringList msiOptions;
        if (getConfig("Suppress reboot", false).toBool()) {
            msiOptions << "REBOOT=ReallySuppress";
        }
        if (getConfig("Accept EULA", false).toBool()) {
            msiOptions << "EULA_ACCEPT=YES";
        }
        if (!getConfig("Desktop shortcut", true).toBool()) {
            msiOptions << "DISABLEDESKTOPSHORTCUT=1";
        }
        if (getConfig("Set default PDF viewer", false).toBool()) {
            msiOptions << "IW_DEFAULT_VERB=READ";
            msiOptions << "LEAVE_PDFOWNERSHIP=NO";
        } else {
            msiOptions << "LEAVE_PDFOWNERSHIP=YES";
        }
        if (!getConfig("Install synchronizer", true).toBool()) {
            msiOptions << "SYNCHRONIZER=NO";
        }
        if (!getConfig("Use automatic update", true).toBool()) {
            msiOptions << "DISABLE_ARM_SERVICE_INSTALL=1";
        }
        qDebug() << "MSI options:" << msiOptions;

        // Compose main installation routine
        Version msiVersion(version.truncate(2).pad(2));
        QString src(loadResource(":NSIS/AdobeReader/main.nsh"));
        src.replace("${VersionMajor}", msiVersion.part(1));
        src.replace("${VersionMinor}", msiVersion.part(2));
        src.replace("${MsiFile}", QFileInfo(msiFile).fileName());
        src.replace("${MsiOptions}", msiOptions.join(" "));

        // Compose patch installation if necessary
        QString header;
        if (!mspFiles.isEmpty()) {
            header = loadResource(":NSIS/AdobeReader/header.nsh");
            QPair<Version, QString> mspFile;
            foreach (mspFile, mspFiles) {
                src += loadResource(":NSIS/AdobeReader/installpatch.nsh")
                        .replace("${MspVersion}", mspFile.first)
                        .replace("${MspFile}", QFileInfo(mspFile.second).fileName())
                        ;
            }
        }

        // Add code to disable updater if requested
        if (!getConfig("Use automatic update", true).toBool()) {
            src += loadResource(":NSIS/AdobeReader/disableupdater.nsh");
        }

        installer->build(
                    objectName(),
                    getOutputFile(),
                    NSIS::Zlib,
                    300,
                    browsers() << "AcroRd32.exe",
                    tempFiles,
                    src,
                    header
                    );
    }
    cleanup();
}


void AdobeReader::download(Version version)
{
    // MSI version always has 2 parts (10.0, 10.1...)
    Version msiVersion;
    if (version.part(1).toInt() >= 11) {
        msiVersion = version.pad(2).pad(3, "00");
    } else {
        msiVersion = version.truncate(2).pad(2);
    }
    qDebug() << "MSI version" << msiVersion.toString();

    QString language(getConfig("Language", "en_US").toString());
    qDebug()<<"language:"<<language;

    // Download MSI file.
    QString msiUrl("http://ardownload.adobe.com/pub/adobe/reader/win/%1/%2/%4/AdbeRdr%3_%4.msi");
    msiFile = Downloader::get(
                msiUrl
                .arg(msiVersion.truncate(2).replace(2, "x"))
                .arg(msiVersion.pad(3))
                .arg(msiVersion.pad(3).stripDots())
                .arg(language),
                Application::getTmpDir()
                );
    if (msiFile.isEmpty()) {
        isError = true;
        return;
    }
    tempFiles << msiFile;

    // If version has 3 parts, download MSP file.
    if (version.numParts() < 3) {
        return;
    }
    QString mspUrl("http://ardownload.adobe.com/pub/adobe/reader/win/%1/%2/misc/AdbeRdrUpd%3.msp");
    QString mspFile = Downloader::get(
                    mspUrl
                    .arg(msiVersion.replace(2, "x"))
                    .arg(version)
                    .arg(version.stripDots()),
                    Application::getTmpDir()
                    );
    if (mspFile.isEmpty()) {
        isError = true;
    } else {
        // In the past, there have been releases where more than 1 MSP was
        // required for a fresh up-to-date installation. For this reason,
        // the patches are stored in a list.
        // The dependency tracking is not implemented yet (it will should it
        // ever become necessary again), but the rest of the code is already
        // prepared for multiple patches.
        mspFiles << qMakePair(version, mspFile);
        tempFiles << mspFile;
    }
}
