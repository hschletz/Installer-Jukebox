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
#include "javaruntimeenvironment7.h"
#include "manualdownload.h"


JavaRuntimeEnvironment7::JavaRuntimeEnvironment7() :
    Package("Java Runtime Environment 7", "15")
{
}


void JavaRuntimeEnvironment7::build(NSIS *installer, Version version)
{
    isError = false;

    download(version);
    if (!isError) {
        QString header;
        QString src(loadResource(":NSIS/JavaRuntimeEnvironment7/main.nsh"));
        src.replace("${Installer}", QFileInfo(tempFiles.first()).fileName());

        if (!getConfig("Use automatic update", true).toBool()) {
            src += loadResource(":NSIS/JavaRuntimeEnvironment7/disableautoupdate.nsh");
        }
        if (!getConfig("Use Quickstarter", true).toBool()) {
            header += "!include 'WinVer.nsh'\n";
            src += loadResource(":NSIS/JavaRuntimeEnvironment7/disablequickstarter.nsh");
        }

        installer->build(
                    objectName(),
                    getOutputFile(),
                    NSIS::Lzma,
                    120,
                    browsers(),
                    tempFiles,
                    src,
                    header
                    );
    }
    cleanup();
}


void JavaRuntimeEnvironment7::download(Version version)
{
    QString filename(
                ManualDownload::getFile(
                    QString("jre-7u%1-windows-i586.exe").arg(version),
                    "http://www.java.com/inc/BrowserRedirect1.jsp",
                    "You need to download the 32 bit offline installer."
                    )
                );
    if (filename.isEmpty()) {
        isError = true;
        return;
    }
    tempFiles << filename;
}
