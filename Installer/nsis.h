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
#ifndef NSIS_H
#define NSIS_H

#include <QString>
#include <QStringList>
#include "installer.h"

/**
 * @brief %NSIS builder
 */
class NSIS : public Installer
{
    Q_OBJECT

public:

    /**
     * @brief Compression type
     *
     * The best choice of compression depends on the package content. If it is
     * already efficiently compressed, choosing a high compression ratio may
     * have a negative effect on the output file size.
     */
    enum Compression {
        None, /**< No compression. Best choice for very efficiently compressed content. */
        Zlib, /**< Zlib compression. Very fast and good choice for most files. */
        Bzip2, /**< Bzip2 compression. Slower than Zlib, but sometimes better compression. */
        Lzma /**< LZMA compression. Fast and very good compression for less efficiently compressed files. */
    };

/**
 * @brief Constructor
 */
    NSIS();

    /**
     * @brief Build the package
     *
     * @param name %Package name, substitutes ${installerName} in the template
     * @param outputFile Full path to output file, substitutes ${outputFile} in the template
     * @param compression Compression type
     * @param minSpaceRequired Minimum required disk space (in MB). Rule of thumb: 4 times the package size.
     * @param blockingProcesses List of process names that will be forced to terminate before installation.
     * @param files List of files to include in the package
     * @param script %NSIS code fragment that does the actual installation
     * @return bool Success
     */
    bool build(QString name,
               QString outputFile,
               Compression compression,
               uint minSpaceRequired,
               QStringList blockingProcesses,
               QStringList files,
               QString script
               );

    /**
     * @brief Destructor
     */
    ~NSIS();

private:

    /**
     * @brief Clean up temporary files created by build()
     */
    void cleanup();

    /**
     * @brief Flag indicating necessary cleanup of System.dll
     */
    bool systemPluginCopied;

    /**
     * @brief Flag indicating necessary cleanup of nsProcess.dll
     */
    bool nsProcessPluginCopied;
};

#endif // NSIS_H
