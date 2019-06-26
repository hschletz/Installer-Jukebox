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
    Package("Adobe Acrobat Reader DC", "1800920044")
{
}

void AdobeReader::build(NSIS *installer, Version version)
{
    isError = false;
    download(version);
    if (!isError) {
        QStringList files(originalInstaller);

        // Compose options for command line
        QString transformFile(getConfig("Transform").toString());
        QString transformOption;
        if (!transformFile.isEmpty()) {
            transformOption = "TRANSFORMS=\"$PLUGINSDIR\\%1\"";
            files << transformFile;
        }
        qDebug() << "Transform: " << transformFile;

        // Compose main installation routine
        QString src(loadResource(":NSIS/AdobeReader/main.nsh"));
        src.replace("${Installer}", QFileInfo(originalInstaller).fileName());
        src.replace("${Options}", transformOption.arg(QFileInfo(transformFile).fileName()));

        installer->build(
                    objectName(),
                    getOutputFile(),
                    NSIS::None,
                    800,
                    QStringList("AcroRd32.exe"),
                    files,
                    src,
                    ""
                    );
    }
    cleanup();
}

void AdobeReader::download(Version version)
{
    QString language(getConfig("Language", "en_US").toString());
    qDebug()<<"language:"<<language;

    QString urlVersion(version.stripDots());
    if (urlVersion.startsWith("20")) {
        urlVersion = urlVersion.mid(2);
    }
    QString url("https://ardownload2.adobe.com/pub/adobe/reader/win/AcrobatDC/%1/AcroRdrDC%1_%2.exe");
    originalInstaller = Downloader::get(
                url
                .arg(urlVersion)
                .arg(language),
                Application::getTmpDir()
                );
    if (originalInstaller.isEmpty()) {
        isError = true;
        return;
    }
    tempFiles << originalInstaller;
}
