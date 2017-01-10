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
#include "downloader.h"
#include "flashplayeractivex.h"

FlashPlayerActiveX::FlashPlayerActiveX() :
    Package("Flash Player ActiveX", "24.0.0.194")
{
}


void FlashPlayerActiveX::build(NSIS *installer, Version version)
{
    Q_UNUSED(version);

    isError = false;
    download(version);
    if (!isError) {
        installer->build(
                    objectName(),
                    getOutputFile(),
                    NSIS::Zlib,
                    40,
                    QStringList(),
                    tempFiles,
                    loadResource(":NSIS/FlashPlayerActiveX/main.nsh").replace(
                        "${Installer}",
                        QFileInfo(tempFiles.first()).fileName())
                    );
    }
    cleanup();
}


void FlashPlayerActiveX::download(Version version)
{
    QString target(
        Downloader::get(
            QString("http://fpdownload.adobe.com/get/flashplayer/pdc/%1/install_flash_player_ax.exe")
                    .arg(version),
            Application::getTmpDir()
        )
    );
    if (target.isEmpty()) {
        isError = true;
    } else {
        tempFiles << target;
    }
}
