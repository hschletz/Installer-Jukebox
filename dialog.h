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
#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QMap>
#include <QSettings>
#include "package.h"

namespace Ui {
class Dialog;
}

/**
 * @brief The main dialog.
 */
class Dialog : public QDialog
{
    Q_OBJECT

public:

    /**
     * @brief Default constructor
     */
    explicit Dialog();

    /**
     * @brief Destructor
     */
    ~Dialog();

private slots:

    /**
     * @brief Set visibility of version widget depending on selected package
     * @param arg1 Selected package
     */
    void on_packageName_currentIndexChanged(const QString &arg1);

    /**
     * @brief Start building a package according to settings.
     */
    void on_buildButton_clicked();

private:

    /**
     * @brief The UI elements.
     */
    Ui::Dialog *ui;

    /**
     * @brief Map of Packages
     * The constructor populates this with all packages enabled in the
     * configuration. The key is the package name, the value is an instance of
     * the corresponding Package-derived class.
     */
    QMap<QString, Package*> packages;
};

#endif // DIALOG_H
