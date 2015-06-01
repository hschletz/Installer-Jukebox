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
    Package("VLC Media Player", "2.1.2")
{
}


void VlcMediaPlayer::build(NSIS *installer, Version version)
{
    isError = false;

    // Compose list of blocking processes. If the Mozilla plugin is enabled,
    // this includes supported browsers.
    QStringList blockingProcesses("vlc.exe");
    if (getConfig("Install Mozilla plugin", true).toBool()) {
        blockingProcesses << browsers();
    }

    // The name of the installer provided by download()
    QString installerFile(QString("vlc-%1-win32.exe").arg(version));

    // Main installation routine
    QString src(loadResource(":NSIS/VlcMediaPlayer/main.nsh"));
    src.replace("${VLCinstaller}", installerFile);

    // If the ActiveX plugin is enabled, force closing all IE windows.
    if (getConfig("Install ActiveX plugin", true).toBool()) {
        src.prepend(loadResource(":NSIS/VlcMediaPlayer/blockOnIE.nsh"));
    }

    download(version, installer);
    installerFile.prepend(Application::getTmpDir() + "/");
    tempFiles << installerFile;

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

void VlcMediaPlayer::download(Version version, Installer *installer)
{
    // Clear extracted path if necessary
    QString extractedPath(Application::getTmpDir() + "/vlc-" + version);
    if (QFile::exists(extractedPath)) {
        qDebug() <<extractedPath << "exists.";
        if (Application::question(
                    tr("'%1' exists. Click OK to delete it recursively.").arg(extractedPath),
                    QMessageBox::Ok | QMessageBox::Cancel,
                    QMessageBox::Cancel
                    )
                == QMessageBox::Cancel) {
            qDebug() << "Canceled.";
            isError = true;
            return;
        }
        if (!rmPath(extractedPath)) {
            isError = true;
            return;
        }
    }

    // Download and extract the 7z archive
    QString url("http://get.videolan.org/vlc/%1/win32/vlc-%1-win32.7z");
    // Qt's default User-Agent header leads to a countdown page with HTTP status
    // 200, which only works in a full-featured browser. To get a 302 response,
    // use an arbitrary header not recognized by the site.
    QString target(
                Downloader::get(
                    url.arg(version),
                    Application::getTmpDir(),
                    "",
                    "Installer Jukebox"
                    )
                );
    if (target.isEmpty()) {
        isError = true;
        return;
    }
    tempFiles << target;
    extract7zArchive(target);
    tempFiles << extractedPath;

    // Patch the installer if necessary
    QString src(extractedPath + "/vlc.win32.nsi");
    bool disableActiveX = !getConfig("Install ActiveX plugin", true).toBool();
    bool disableMozilla = !getConfig("Install Mozilla plugin", true).toBool();
    if (disableActiveX or disableMozilla) {
        QFile file(src);
        if (!file.open(QIODevice::ReadWrite)) {
            isError = true;
            return;
        }
        QTextStream stream(&file);
        QString content(stream.readAll());
        if (disableActiveX) {
            if (!patch(content, "!define INSTALL_ACTIVEX", "")) {
                return;
            }
        }
        if (disableMozilla) {
            if (!patch(content, "!define INSTALL_MOZILLA", "")) {
                return;
            }
        }
        // Rewrite file content ant truncate to new length
        stream.seek(0);
        stream << content;
        file.resize(stream.pos());
        file.close();
    }

    // Add makensis to command execution
    installer->addPreprocessCommand(
                Command::cmdSpec(
                    Application::getConfig("installer_NSIS/makensis", "makensis").toString(),
                    QStringList() << "-V1" << src
                    )
                );
}


bool VlcMediaPlayer::patch(QString &source, QString before, QString after)
{
    if (source.count(before) == 1) {
        source.replace(before, after);
        qDebug() << "Replaced" << before << "with" << after;
        return true;
    } else {
        qDebug() << "Installer source code mismatch:" << before;
        Application::critical(tr("Internal error: '%1' does not occur exactly 1 time. "
                                 "Installer Jukebox needs to be fixed.")
                              .arg(before));
        isError = true;
        return false;
    }
}
