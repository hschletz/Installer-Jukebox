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

#ifndef MANUALDOWNLOAD_H
#define MANUALDOWNLOAD_H

#include <QDialog>
#include <QFileDialog>


namespace Ui {
class ManualDownload;
}

/**
 * @brief %Dialog for selecting a manually downloaded installer
 *
 * Some installers cannot be downloaded automatically either because the
 * download requires some sort of authentication (like accepting an EULA on the
 * website) or because the URL contains some unpredictable component that
 * changes with every release. This dialog prompts the user to download the
 * installer manually (a hyperlink to open the download site in the default
 * browser is provided for convenience) and supply the path to the downloaded
 * file.
 *
 * To use this dialog, simply call the static getFile() method.
 */
class ManualDownload : public QDialog
{
    Q_OBJECT

protected:
    /**
     * @brief Constructor
     * @param filename Name of the installer to download and prompt for
     * @param downloadSite URL of the download site (as far as it is predictable and freely accessible)
     * @param extraText Extra information to display below the main prompt text
     */
    explicit ManualDownload(QString filename, QString downloadSite, QString extraText);

    /**
     * @brief Destructor
     */
    ~ManualDownload();

public:

    /**
     * @brief Prompt for downloaded file
     * @param filename Name of the installer to download and prompt for
     * @param downloadSite URL of the download site (as far as it is predictable and freely accessible)
     * @param extraText Extra information to display below the main prompt text
     * @return QString Full path to file, empty if dialog was aborted
     */
    static QString getFile(QString filename, QString downloadSite, QString extraText = QString());

private slots:

    /**
     * @brief Handler for "Choose File" button - open file selector dialog
     */
    void on_button_clicked();

private:

    /**
     * @brief Main dialog
     */
    Ui::ManualDownload *ui;

    /**
     * @brief File selector dialog
     */
    QFileDialog fileDialog;

};

#endif // MANUALDOWNLOAD_H
