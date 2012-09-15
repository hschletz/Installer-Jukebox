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

#include <QFileInfo>
#include "application.h"
#include "infrarecorder.h"
#include "Downloader/sourceforge.h"


InfraRecorder::InfraRecorder() :
    Package("InfraRecorder", "0.53")
{
}


void InfraRecorder::build(NSIS *installer, Version version)
{
    isError = false;
    download(version);
    if (!isError) {
        QString src(
                    loadResource(":NSIS/InfraRecorder/main.nsh").replace(
                        "${Installer}",
                        QFileInfo(tempFiles.first()).fileName()
                        )
                    );
        QString header;
        if (getConfig("Set version", false).toBool()) {
            header = loadResource(":NSIS/InfraRecorder/header.nsh");
            src += loadResource(":NSIS/InfraRecorder/setversion.nsh");
        }

        installer->build(
                    objectName(),
                    getOutputFile(),
                    NSIS::None,
                    16,
                    QStringList("InfraRecorder.exe"),
                    tempFiles,
                    src,
                    header
                    );
    }
    cleanup();
}


void InfraRecorder::download(Version version)
{
    QString file(
                Sourceforge::getProjectFile(
                    "infrarecorder",
                    QString("InfraRecorder/%1/ir%2.exe").arg(version).arg(version.stripDots()),
                    Application::getTmpDir()
                    )
                );
    if (file.isEmpty()) {
        isError = true;
    } else {
        tempFiles << file;
    }
}
