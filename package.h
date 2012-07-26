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
#ifndef PACKAGE_H
#define PACKAGE_H

#include <QMap>
#include <QObject>
#include <QString>
#include <QStringList>
#include "version.h"
#include "Installer/nsis.h"

/**
 * @brief Base class for all packages
 *
 * This class should not be instantiated directly, but always by a subclass. It
 * provides some helper methods useful for all packages.
 */
class Package : public QObject
{
    Q_OBJECT

public:

    /**
     * @brief Constructor
     * @param name %Package name, can later be queried via className()
     * @param %minVersion Minimum supported version (optional)
     */
    explicit Package(const char *name, const char *minVersion=0);

    /**
     * @brief Destructor
     */
    virtual ~Package();

    /**
     * @brief Get a list of all configured packages
     * @return QMap<QString, Package *>
     */
    static QMap<QString, Package *> getPackages();

    /**
     * @brief Get minimum supported version
     *
     * Gets the minimum software version supported by the package class. Older
     * versions might work or not, but are untested and not officially
     * supported.
     *
     * The default implementation returns no version, meaning that the latest
     * version gets downloaded automatically.
     * @return Version
     */
    Version getMinVersion();

    /**
     * @brief Build the package via the configured installer
     */
    void build(QString version);

    /**
     * @brief Build the package via %NSIS
     * @param installer NSIS instance
     * @param version Requested version
     */
    virtual void build(NSIS *installer, Version version);

    /**
     * @brief Get configured output file to build
     *
     * If the file already exists, a confirmation dialog is displayed.
     * @return QString Full path to output file or empty on error/abort.
     */
    QString getOutputFile();

    /**
     * @brief Clean up temporary files according to "Keep temp files" option
     */
    void cleanup();

protected:

    /**
     * @brief Return a resource as string
     * @param name Resource name
     * @return QString Resource content
     * @exception If resource name is invalid (programming error - should never happen in production)
     */
    QString loadResource(QString name);

    /**
     * @brief Return a list of supported browser process names
     *
     * This is useful for the installation of browser plugins. The generated
     * installer can check for running instances of supported browsers and force
     * termination of browsers before proceeding.
     * @return QStringList
     */
    QStringList browsers();

    /**
     * @brief List of temporary files
     *
     * Every temporary file generated during the package building process (by
     * download or other means) should be noted in this list. They will then be
     * cleaned up automatically according to the "Keep temp files" option.
     */
    QStringList tempFiles;

    /**
     * @brief Error flag
     */
    bool isError;

    /**
     * @brief Minimum supported version
     */
    Version minVersion;
};
#endif // PACKAGE_H
