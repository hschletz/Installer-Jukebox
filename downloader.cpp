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
#include <QDateTime>
#include <QDebug>
#include <QFileInfo>
#include <QNetworkRequest>
#include <QProgressBar>
#include "application.h"
#include "downloader.h"


Downloader::Downloader(QString sourceUrl, QString targetDir, QString filename, QString userAgent) :
    QObject(0),
    userAgent(userAgent),
    dialog(NULL),
    progressLabel(tr("Downloading %1 (%2 KiB/s)"))
{
    if (targetDir.isEmpty()) {
        return;
    }

    qDebug() << "Download:" << sourceUrl;

    // Extract filename from URL if necessary
    if (filename.isEmpty()) {
        filename = QFileInfo(sourceUrl).fileName();
    }

    // Compose target path
    target = targetDir + "/" + filename;
    qDebug() << "Target:" << target;

    // Show confirmation dialog if target exists
    if(QFile::exists(target))
    {
        qDebug() << "File already exists";
        QFileInfo fileInfo(target);
        switch(Application::question(
                   tr("The file '%1' already exists.\n\nModified:\t%2\nSize:\t%3 KiB\n\nShould it be downloaded again?")
                   .arg(target)
                   .arg(fileInfo.lastModified().toLocalTime().toString(Qt::DefaultLocaleLongDate))
                   .arg(fileInfo.size() >> 10),
                   QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel,
                   QMessageBox::Cancel
                   )
               ) {
        case QMessageBox::Yes:
            qDebug() << "Re-download.";
            // url and target non-empty
            break;
        case QMessageBox::No:
            qDebug() << "Reuse file.";
            // url empty, target non-empty
            return;
        default: // Cancel
            qDebug() << "Aborted.";
            // url and target empty;
            target.clear();
            return;
        }
    }
    url = sourceUrl;
}


Downloader::Downloader(QString url, QString target, QString userAgent) :
    QObject(0),
    url(url),
    target(target),
    userAgent(userAgent),
    dialog(NULL),
    progressLabel(tr("Downloading %1 (%2 KiB/s)"))
{
}


Downloader::~Downloader()
{
    if (dialog) {
        delete dialog;
    }
}


QString Downloader::get(QString sourceUrl, QString targetDir, QString filename, QString userAgent)
{
    Downloader downloader(sourceUrl, targetDir, filename, userAgent);
    return downloader.getFile();
}


QString Downloader::getFile()
{
    if (target.isEmpty()) {
        return QString();
    } else if (url.isEmpty()) {
        return target;
    } else {
        return download(10);
    }
}


QString Downloader::download(int maxRedirects)
{
    file = new QFile(target);
    if(!file->open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        qDebug() << "File open error:" << file->errorString();
        Application::critical(tr("Could not open '%1' for writing. Error was:\n\n%2").arg(target).arg(file->errorString()));
        delete file;
        file = NULL;
        return QString();
    }

    dialog = new QProgressDialog;
    QProgressBar *bar = new QProgressBar(dialog);
    bar->setFormat("%v/%m KiB");
    dialog->setBar(bar); // Will also delete bar together with dialog

    downloadAborted = false;
    downloadComplete = false;
    startTime = QTime::currentTime();

    QNetworkRequest request(url);
    request.setRawHeader("User-Agent", userAgent.toAscii());
    reply = net.get(request);
    connect(reply, SIGNAL(finished()), this, SLOT(finish()));
    connect(reply, SIGNAL(readyRead()), this, SLOT(receive()));
    connect(reply, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(update(qint64, qint64)));
    connect(dialog, SIGNAL(canceled()), this, SLOT(cancel()));

    dialog->exec(); // Synchronous request, returns when download finishes

    if (downloadComplete) {
        return target;
    } else if (redirectUrl.isValid()) {
        return redirectFiltered(redirectUrl.toString(), maxRedirects);
    } else {
        return QString();
    }
}


QString Downloader::redirectFiltered(QString redirectUrl, int maxRedirects)
{
    if (maxRedirects > 0) {
        maxRedirects--;
        qDebug() << "Redirected to" << redirectUrl;
        qDebug() << maxRedirects << "redirects left.";
        return redirect(redirectUrl, maxRedirects);
    } else {
        qDebug() << "Download error: Redirect limit exceeded!";
        Application::critical(tr("Download aborted: too many HTTP redirects."));
        return QString();
    }
}


QString Downloader::redirect(QString redirectUrl, int maxRedirects)
{
    Downloader downloader(redirectUrl, target, userAgent);
    return downloader.download(maxRedirects);
}


void Downloader::finish()
{
    if(downloadAborted)
    {
        qDebug() << "Download aborted by user.";
        if(file)
        {
            file->close();
            file->remove();
        }
    } else {
        receive(); // Ensure no outstanding data
        file->close();
        redirectUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
        if(reply->error())
        {
            qDebug() << "Download error.";
            Application::critical(tr("Download failed: %1").arg(reply->errorString()));
        } else if (redirectUrl.isNull()) {
            qDebug() << "Download complete," << (file->size() >> 10) << "KiB.";
            downloadComplete = true;
        }
    }
    dialog->hide();
    reply->deleteLater();
    reply = NULL;
    delete file;
    file = NULL;
}


void Downloader::receive()
{
    if(file) {
        file->write(reply->readAll());
    }
}


void Downloader::update(qint64 bytesReceived, qint64 bytesTotal)
{
    if(downloadAborted) {
        return;
    }
    // Calculate average download rate (from bytes/ms to KiB/s)
    dialog->setLabelText(progressLabel.arg(url).arg(bytesReceived / startTime.elapsed() * 1000 / 1024));
    dialog->setMaximum(bytesTotal >> 10); // division by 1024
    dialog->setValue(bytesReceived >> 10);

}


void Downloader::cancel()
{
    downloadAborted = true;
    reply->abort();
}
