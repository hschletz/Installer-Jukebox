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
#include "flashplayerplugin.h"
#include "downloader.h"
#include "application.h"

FlashPlayerPlugin::FlashPlayerPlugin() :
    Package("Flash Player Plugin")
{
}

void FlashPlayerPlugin::build(NSIS *installer, Version version)
{
    Q_UNUSED(version);

    isError = false;
    download();
    if (!isError) {
        QStringList blockingProcesses(browsers());
        blockingProcesses.removeAll("chrome.exe");
        isError = !installer->build(
                    objectName(),
                    getOutputFile(),
                    NSIS::Zlib,
                    40,
                    blockingProcesses,
                    tempFiles,
                    loadResource(":NSIS/FlashPlayerPlugin/main.nsh")
                    );
    }
    cleanup();
}

void FlashPlayerPlugin::download()
{
    QString target(
        Downloader::get(
            "http://fpdownload.macromedia.com/pub/flashplayer/current/licensing/win/install_flash_player_12_plugin.exe",
            Application::getTmpDir()
        )
    );
    if (target.isEmpty()) {
        isError = true;
    } else {
        tempFiles << target;
    }
}
