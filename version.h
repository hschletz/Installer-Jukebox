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

#ifndef VERSION_H
#define VERSION_H

#include <QString>
#include <QStringList>
#include <QVariant>

/**
 * @brief A class representing a version number ("1.2.3" etc.)
 */
class Version : public QVariant
{

public:

    /**
     * @brief Construct an empty version object
     */
    Version();

    /**
     * @brief Construct version object from a string
     * @param versionString
     */
    Version(QString versionString);

    /**
     * @brief Return number of components ("a.b.c" => 3)
     * @return uint
     */
    uint numParts();

    /**
     * @brief Return a single part ("a.b.c", 2 => "b")
     * @param position Number of component (starting with 1). If it does not exist, return an empty string.
     * @return QString
     */
    QString part(uint position);

    /**
     * @brief Return truncated version ("a.b.c", 2 => "a.b")
     * @param length New length. If the current length is shorter, the unmodified version is returned.
     * @return Version
     */
    Version truncate(uint length);

    /**
     * @brief Return version padded to the given length ("a.b", 3 => "a.b.0")
     * @param length New length. If the current length is equal or longer, the unmodified version is returned.
     * @param padString String to use as padding. Default: "0".
     * @return Version
     */
    Version pad(uint length, QString padString = "0");

    /**
     * @brief Return version with a component replaced by another string ("a.b.c", 3, "x" => "a.b.x")
     * @param position Number of component (starting with 1) to replace. The component must exist!
     * @param value Replacement string
     * @return Version
     */
    Version replace(uint position, QString value);

    /**
     * @brief Return version number without dots
     * @return QString
     */
    QString stripDots();

    /**
     * @brief Return list of components ("a.b.c" => "a", "b", "c")
     * @return QStringList
     */
    QStringList split();

    /**
     * @brief Convert to string
     */
    operator QString();
};

#endif // VERSION_H
