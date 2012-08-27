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
#include <QVariant>

/**
 * @brief A class that downloads files, showing a progress bar
 *
 * To download a file, just call the static get() method.
 */
class Downloader : public QObject
{
    Q_OBJECT

protected:

    /**
     * @brief Constructs downloader, generating target path
     *
     * This constructor generates the download target from targetDir and
     * filename. If the target exists, the user is prompted for appropriate
     * action. See get() for description of arguments.
     * @param sourceUrl
     * @param targetDir
     * @param filename
     * @param userAgent
     */
    Downloader(QString sourceUrl, QString targetDir, QString filename, QString userAgent);

    /**
     * @brief Constructs downloader with already known target path
     *
     * The target is overwritten unconditinally without prompting the user.
     * @param url
     * @param target
     * @param userAgent
     */
    explicit Downloader(QString url, QString target, QString userAgent);

    /**
     * @brief Destructor
     */
    virtual ~Downloader();

public:

    /**
     * @brief Download a file (public method)
     *
     * Arguments are assumed to be valid - validate before calling this!
     * If the target file already exists, a confirmation dialog is displayed.
     *
     * @param sourceUrl URL to download
     * @param targetDir Directory to store downloaded file. If empty, download aborts immediately
     * @param filename Name for downloaded file. If empty, filename is extracted from URL.
     * @param userAgent User-Agent header to use. If empty, QNetworkRequest's default is used.
     * @return QString Full path of downloaded file, or empty on error
     */
    static QString get(QString sourceUrl, QString targetDir, QString filename="", QString userAgent="");

protected:

    /**
     * @brief Initiate download
     *
     * This checks url and target and initiates the download if necessary.
     * @return QString Full path of downloaded file, or empty on error/abort
     */
    QString getFile();

    /**
     * @brief Download a file (internal method)
     * @param maxRedirects Maximum number of HTTP redirects before download gets aborted.
     * @return QString Full path of downloaded file, or empty on error/abort
     */
    QString download(int maxRedirects);

protected slots:

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

protected:

    /**
     * @brief Handle redirects with filter
     *
     * This method is called whenever a redirect is detected. The default
     * implementation redirects unconditionally unless maxRedirects is 0 or
     * less. Subclasses can override this with extra checks or modification on
     * the URL.
     * @param redirectUrl Redirect target
     * @param maxRedirects Maximum redirects left
     * @return QString Full path of downloaded file, or empty on error/abort
     */
    virtual QString redirectFiltered(QString redirectUrl, int maxRedirects);

    /**
     * @brief Perform redirects unconditionally
     *
     * This is called by redirectFiltered() to perform the actual redirect.
     * Subclasses can override this to instantiate their own class.
     * @param redirectUrl Redirect target
     * @param maxRedirects Maximum redirects left
     * @return QString Full path of downloaded file, or empty on error/abort
     */
    virtual QString redirect(QString redirectUrl, int maxRedirects);

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

private:

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

    /**
     * @brief Redirection target (if any)
     */
    QVariant redirectUrl;

};

#endif // DOWNLOADER_H
