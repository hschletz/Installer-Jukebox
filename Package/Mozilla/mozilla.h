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

#ifndef MOZILLA_H
#define MOZILLA_H

#include "package.h"


/**
 * @brief Base class for %Mozilla packages
 *
 * Subclasses can use the provided methods for unified access to the %Mozilla
 * download area and preferences system.
 */
class Mozilla : public Package
{
public:

    /**
     * @brief Datatypes for preferences
     */
    enum optionType {
        string, /**< A string. */
        integer, /**< An integer. */
        boolean, /**< A boolean. */
        boolean_inverted /**< A boolean, but its value gets inverted. Useful to avoid double negation logic. */
    };

    /**
     * @brief Callback for setting options
     *
     * To pass a subclass method as a callback to setOption(), the pointer must
     * be explicitly cast to setOptionCallback.
     *
     * The first argument is a template string. %1 should be replaced with a
     * %Mozilla preferences name, %2 with an appropriate value. Use
     * quoteString() for string types. The second argument is the value from the
     * config file.
     *
     * The callback must return valid code to be inserted into the preferences
     * file, i.e. it must contain only 0 or more incarnations of the template
     * string. JS comments and blank lines are OK, too.
     */
    typedef QString(Mozilla::*setOptionCallback)(QString, QVariant);

protected:

    /**
     * @brief Constructor
     *
     * @param name passed to Package constructor
     * @param minVersion passed to Package constructor
     */
    Mozilla(const char *name, const char *minVersion);

    /**
     * @brief Download installer
     * @param version %Package version
     */
    void download(Version version);

    /**
     * @brief Generate code for preferences file
     *
     * The base implemantation provides support for common options available in
     * all Mozilla products:
     *
     * - Proxy configuration script
     *
     * Subclasses should extend this function with package specific options.
     * @return QString
     */
    virtual QString getPrefs();

    /**
     * @brief Set/delete preferences
     * @param name Option name in the %Installer Jukebox config file
     * @param type Value's datatype
     * @param jsName Option name in %Mozilla config system
     * @return JS code to append to %Mozilla prefs file
     */
    QString setOption(QString name, optionType type, QString jsName);

    /**
     * @brief Set/delete preferences using a callback method
     * @param name Option name in the %Installer Jukebox config file
     * @param callback Callback method
     * @return JS code to append to %Mozilla prefs file
     */
    QString setOption(QString name, setOptionCallback callback);

    /**
     * @brief Quote a string argument for insertion into the preferences file
     * @param string Raw string
     * @return QString Quoted and escaped string
     */
    QString quoteString(QString string);

    /**
     * @brief Create temporary files to be included in package
     *
     * This function generates 2 files in the working directory. The build()
     * implementation is responsible for putting them in the right location on
     * the target system:
     *
     * - local-settings.js goes to APPDIR/defaults/pref
     * - mozilla.cfg goes to APPDIR
     *
     * @param prefs Content of preferences file. The required initial comment line is prepended.
     */
    void writePrefsFile(QString prefs);

private:

    /**
     * @brief Callback for setting a proxy script
     * @param cmdTemplate
     * @param value
     * @return QString
     */
    QString setProxyScript(QString cmdTemplate, QVariant value);

};

#endif // MOZILLA_H
