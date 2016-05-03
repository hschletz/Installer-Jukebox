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
#include <QStringList>
#include "application.h"
#include "downloader.h"
#include "vlcmediaplayer.h"

VlcMediaPlayer::VlcMediaPlayer() :
    Package("VLC Media Player", "2.2.2")
{
}


void VlcMediaPlayer::build(NSIS *installer, Version version)
{
    isError = false;

    // The name of the installer provided by download()
    QString installerFile(QString("vlc-%1-win32.exe").arg(version));

    // Main installation routine
    QString src(loadResource(":NSIS/VlcMediaPlayer/main.nsh"));
    src.replace("${VLCinstaller}", installerFile);

    // Compose list of blocking processes. If the Mozilla plugin is enabled,
    // this includes supported browsers.
    QStringList blockingProcesses("vlc.exe");

    if (getConfig("Install Mozilla plugin", true).toBool()) {
        blockingProcesses << browsers();
    } else {
        // Unregister and delete Mozilla plugin
        src.append(loadResource(":NSIS/VlcMediaPlayer/uninstallMozillaPlugin.nsh").replace("${Version}", version));
    }

    if (getConfig("Install ActiveX plugin", true).toBool()) {
        // Force closing all IE windows
        src.prepend(loadResource(":NSIS/VlcMediaPlayer/blockOnIE.nsh"));
    } else {
        // Unregister and delete ActiveX plugin
        src.append(loadResource(":NSIS/VlcMediaPlayer/uninstallActiveXPlugin.nsh"));
    }

    installerFile = download(version);

    if (!isError) {
        isError = !installer->build(
                    objectName(),
                    getOutputFile(),
                    NSIS::None, // Installer is already LZMA compressed
                    100,
                    blockingProcesses,
                    QStringList(installerFile),
                    src
                    );
    }
    cleanup();
}

QString VlcMediaPlayer::download(Version version)
{
    QString url("http://download.videolan.org/pub/videolan/vlc/%1/win32/vlc-%1-win32.exe");
    QString target(
                Downloader::get(
                    url.arg(version),
                    Application::getTmpDir()
                    )
                );
    if (target.isEmpty()) {
        isError = true;
    } else {
        tempFiles << target;
    }
    return target;
}
