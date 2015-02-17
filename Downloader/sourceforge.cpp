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
#include "application.h"
#include "sourceforge.h"

Sourceforge::Sourceforge(QString sourceUrl, QString targetDir, QString filename, QByteArray userAgent) :
    Downloader(sourceUrl, targetDir, filename, userAgent)
{
}


Sourceforge::Sourceforge(QString sourceUrl, QString target, QByteArray userAgent) :
    Downloader(sourceUrl, target, userAgent)
{
}


QString Sourceforge::getProjectFile(
        QString project,
        QString path,
        QString targetDir,
        QString filename
        )
{
    QString url("http://sourceforge.net/projects/%1/files/%2");
    // Qt's default User-Agent header leads to a countdown page with HTTP status
    // 200, which only works in a full-featured browser. To get a 302 response,
    // use an arbitrary header not recognized by the site.
    Sourceforge downloader(url.arg(project).arg(path), targetDir, filename, "Installer Jukebox");
    return downloader.getFile();
}


QString Sourceforge::redirectFiltered(QString redirectUrl, int maxRedirects)
{
    // If the requested file does not exist, we get redirected to the top of the
    // download area instead of receiving a 404 response. Detect this situation
    // and abort.
    if (redirectUrl.endsWith("/files/")) {
        qDebug() << "Error: File does not exist in download area!";
        Application::critical(tr("File not found on server!"));
        return QString();
    } else {
        return Downloader::redirectFiltered(redirectUrl, maxRedirects);
    }
}


QString Sourceforge::redirect(QString redirectUrl, int maxRedirects)
{
    Sourceforge downloader(redirectUrl, target, userAgent);
    return downloader.download(maxRedirects);
}
