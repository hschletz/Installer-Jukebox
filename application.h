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
#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>
#include <QMessageBox>
#include <QSettings>
#include <QString>
#include <QStringList>
#include <QVariant>

/**
 * @brief The main application object with access to global configuration and some helper methods.
 */
class Application : public QApplication
{
    Q_OBJECT
public:

    /**
     * @brief When to keep temporary files (downloaded stuff etc.)
     *
     * The default is 'Never'. The other values are mostly useful for
     * development.
     */
    enum tmpFileCleanup {
        Never, /**< Full cleanup after building the package. */
        OnError, /**< Keep files if an error occurs, clean up otherwise */
        Always /**< Keep all files unconditionally. */
    };

    /**
     * @brief Default constructor.
     *
     * This constructor performs basic initialization like parsing the config
     * file.
     * @param argc
     * @param argv
     * @exception int Suggested exit code if a fatal
     * error occurs, like a missing config file or invalid essential options.
     */
    explicit Application(int &argc, char **argv);

    /**
     * @brief Display a message box of type 'critical'.
     * @param message Message to display
     */
    static void critical(QString message);

    /**
     * @brief Display a message box of type 'information'.
     * @param message Message to display
     */
    static void information(QString message);

    /**
     * @brief Display a message box of type 'question'.
     * @param message Message to display
     * @param buttons Buttons to display
     * @param defaultButton Button to pre-select
     * @return QMessageBox::StandardButton Button which was pressed by the user
     */
    static QMessageBox::StandardButton question(
            const QString &message,
            QMessageBox::StandardButtons buttons = QMessageBox::Ok,
            QMessageBox::StandardButton defaultButton = QMessageBox::NoButton
            );

    /**
     * @brief Get a value from the configuration file
     * @param key QSettings-style key
     * @param defaultValue Value to return if no value is present in the config file
     * @return QVariant Value
     */
    static QVariant getConfig(const QString &key, const QVariant &defaultValue = QVariant());

    /**
     * @brief Get all keys from a specific group
     * @param group
     * @return QStringList
     */
    static QStringList getConfigGroupKeys(QString group);

    /**
     * @brief Return a list of all sections in the config file
     * @return QStringList
     */
    static QStringList getConfigGroups();

    /**
     * @brief Get working directory for temporary files
     * @return QString
     */
    static QString getTmpDir();

    /**
     * @brief Get configured cleanup of temporary files
     * @return tmpFileCleanup
     */
    static tmpFileCleanup keepTempFiles();

private:

    /**
     * @brief Global application configuration
     */
    QSettings *config;

    /**
     * @brief Global working directory
     */
    QString tmpDir;

    /**
     * @brief When to keep temporary files
     */
    tmpFileCleanup tmpFileHandling;
};
#endif // APPLICATION_H
