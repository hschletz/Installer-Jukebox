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

#ifndef SOURCEFORGE_H
#define SOURCEFORGE_H

#include "downloader.h"

/**
 * @brief Class for downloading files from sourceforge.net
 *
 * This class takes care of redirection issues and error handling which does not
 * work with the standard Downloader class. Use the static getProjectFile() to
 * download a file.
 */
class Sourceforge : public Downloader
{

public:

    /**
     * @brief Constructor
     * @param sourceUrl
     * @param targetDir
     * @param filename
     * @param userAgent
     */
    Sourceforge(QString sourceUrl, QString targetDir, QString filename, QString userAgent);

    /**
     * @brief Constructor
     * @param sourceUrl
     * @param target
     * @param userAgent
     */
    explicit Sourceforge(QString sourceUrl, QString target, QString userAgent);

    /**
     * @brief Get a file from a project's download area
     *
     * For convenience, the URL is constructed from project and path:
     *
     *     http://sourceforge.net/projects/<project>/files/<path>
     *
     * @param project %Sourceforge project name
     * @param path Path relative to the download area (no leading /)
     * @param targetDir Directory to store downloaded file. If empty, download aborts immediately
     * @param filename Name for downloaded file. If empty, filename is extracted from URL.
     * @return QString Full path of downloaded file, or empty on error
     */
    static QString getProjectFile(QString project, QString path, QString targetDir, QString filename="");

protected:

    /**
     * @brief Handle invalid paths in addition to standard redirect handling
     * @param redirectUrl
     * @param maxRedirects
     * @return QString
     */
    QString redirectFiltered(QString redirectUrl, int maxRedirects);

    /**
     * @brief Instantiante Sourceforge class for redirection
     * @param redirectUrl
     * @param maxRedirects
     * @return QString
     */
    QString redirect(QString redirectUrl, int maxRedirects);

};

#endif // SOURCEFORGE_H
