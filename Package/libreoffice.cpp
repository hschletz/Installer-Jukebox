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
#include <QStringList>
#include "application.h"
#include "downloader.h"
#include "libreoffice.h"


LibreOffice::LibreOffice() :
    Package("LibreOffice", "3.5.5")
{
}


void LibreOffice::build(NSIS *installer, Version version)
{
    isError = false;
    download(version);
    if (!isError) {
        QStringList msiOptions("ADDLOCAL=ALL");
        QStringList removeComponents;

        if (!getConfig("Use automatic update", true).toBool()) {
            removeComponents << "gm_o_Onlineupdate";
        }

        if (!removeComponents.isEmpty()) {
            msiOptions << "REMOVE=" + removeComponents.join(",");
        }

        qDebug() << "MSI options:" << msiOptions;

        QString src(loadResource(":NSIS/LibreOffice/main.nsh"));
        src.replace("${MsiFile}", QFileInfo(tempFiles.first()).fileName());
        src.replace("${MsiOptions}", msiOptions.join(" "));

        installer->build(
                    objectName(),
                    getOutputFile(),
                    NSIS::Zlib,
                    800,
                    QStringList() << "soffice.exe" << "soffice.bin",
                    tempFiles,
                    src
                    );
    }
    cleanup();
}


void LibreOffice::download(Version version)
{
    QString url("http://download.documentfoundation.org/libreoffice/stable/%1/win/x86/LibO_%1_Win_x86_install_multi.msi");
    QString target(Downloader::get(url.arg(version), Application::getTmpDir()));
    if (target.isEmpty()) {
        isError = true;
    } else {
        tempFiles << target;
    }
}
