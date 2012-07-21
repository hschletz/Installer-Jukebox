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
#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QFile>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include <QProgressDialog>
#include <QString>
#include <QTime>

/**
 * @brief A class that downloads files, showing a progress bar
 *
 * To download a file, just call the static get() method.
 */
class Downloader : public QObject
{
    Q_OBJECT

public:

    /**
     * @brief Constructor
     *
     * It is not recommended to instantiate an object directly. Use get()
     * instead.
     * @param url
     * @param target
     */
    explicit Downloader(QString url, QString target, QString userAgent);

    /**
     * @brief Destructor
     */
    ~Downloader();

    /**
     * @brief Download a file (recommended method)
     *
     * Arguments are assumed to be valid - validate before calling this!
     * If the target file already exists, a confirmation dialog is displayed.
     *
     * @param url URL to download
     * @param targetDir Directory to store downloaded file. If empty, returns immediately
     * @param filename Name for downloaded file. If empty, filename is extracted from URL.
     * @param userAgent User-Agent header to use. If empty, QNetworkRequest's default is used.
     * @return QString Full path of downloaded file, or empty on error
     */
    static QString get(QString url, QString targetDir, QString filename="", QString userAgent="");

    /**
     * @brief Download a file (internal method)
     * @return QString Full path of downloaded file, or empty on error/abort
     */
    QString download();

public slots:

    /**
     * @brief Finish download, clean up
     */
    void finish();

    /**
     * @brief Write received data to file
     */
    void receive();

    /**
     * @brief Update progess dialog
     * @param bytesReceived
     * @param bytesTotal
     */
    void update(qint64 bytesReceived, qint64 bytesTotal);

    /**
     * @brief Abort download
     */
    void cancel();

private:

    /**
     * @brief URL
     */
    QString url;

    /**
     * @brief Full path to target file
     */
    QString target;

    /**
     * @brief User agent to sent in HTTP request
     */
    QString userAgent;

    /**
     * @brief Progress dialog
     */
    QProgressDialog *dialog;

    /**
     * @brief Label of progress dialog
     */
    QString progressLabel;

    /**
     * @brief Target file
     */
    QFile *file;

    /**
     * @brief Flag to indicate aborted download
     */
    bool downloadAborted;

    /**
     * @brief Timestamp of download begin
     */
    QTime startTime;

    /**
     * @brief Network access manager
     */
    QNetworkAccessManager net;

    /**
     * @brief Network reply
     */
    QNetworkReply *reply;

    /**
     * @brief Flag to indicate download success
     */
    bool downloadComplete;
};

#endif // DOWNLOADER_H
