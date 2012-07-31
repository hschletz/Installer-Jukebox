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

#ifndef OPERA_H
#define OPERA_H

#include <QString>
#include <QVariant>
#include "package.h"

/**
 * @brief %Opera package
 */
class Opera : public Package
{

public:

    /**
     * @brief Constructor
     */
    Opera();

    void build(NSIS *installer, Version version);

private:

    /**
     * @brief Datatypes for INI file options
     */
    enum optionType {
        string, /**< A string. It it contains commas, it must be quoted, otherwise it is invalid. */
        integer, /**< An integer. */
        boolean, /**< A boolean. */
        boolean_inverted, /**< A boolean, but its value gets inverted. Useful to avoid double negation logic. */
        stringlist /**< A list of strings, separated by one or more blanks. */
    };

    /**
     * @brief Type of %Opera INI file
     */
    enum iniFile {
        defaults, /**< ${OperaDir}\\operaprefs_default.ini, settings can be overridden by user */
        fixed /**< $SYSDIR%\\operaprefs_fixed.ini, settings can not be overridden */
    };

    /**
     * @brief Callback for setting options in an Opera INI file
     */
    typedef QString(Opera::*setOptionNSIScallback)(iniFile, QVariant);

    /**
     * @brief Download installer
     * @param version Version
     */
    void download(Version version);

    /**
     * @brief Set/delete an option in both INI files
     * @param name Option name in the %Installer Jukebox config file
     * @param type Value's datatype
     * @param section Section name in Opera's config files
     * @param key Key name in Opera's config files
     * @return QString NSIS command lines
     */
    QString setOptionNSIS(QString name, optionType type, QString section, QString key);

    /**
     * @brief Set/delete an option in both INI files using a callback method
     *
     * This is used for complex options that cannot be translated directly from
     * the configuration file, for example if 1 Option influences multiple Opera
     * settings or the value needs transformation.
     * @param name Option name in the %Installer Jukebox config file
     * @param callback Method that gets called with an INI file type and the option value, returning NSIS code
     * @return QString NSIS command lines
     */
    QString setOptionNSIS(QString name, setOptionNSIScallback callback);

    /**
     * @brief Set/delete an option in a particular INI file
     * @param file Type of INI file
     * @param section Section name in Opera's config files
     * @param key Key name in Opera's config files
     * @param value Value to set
     * @param type Value's datatype
     * @return QString
     */
    QString setOptionNSIS(iniFile file, QString section, QString key, QVariant value, optionType type);

    /**
     * @brief Callback for setting the mailto: handler
     * @param file
     * @param value
     * @return QString
     */
    QString setMailHandlerNSIS(iniFile file, QVariant value);

    /**
     * @brief Callback for setting a proxy configuration script
     * @param file
     * @param value
     * @return QString
     */
    QString setProxyScriptNSIS(iniFile file, QVariant value);
};

#endif // OPERA_H
