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

#include <QDebug>
#include "application.h"
#include "manualdownload.h"
#include "ui_manualdownload.h"


ManualDownload::ManualDownload(QString filename, QString downloadSite, QString extraText) :
    QDialog(0),
    ui(new Ui::ManualDownload)
{
    ui->setupUi(this);
    ui->label->setText(
                ui->label->text().arg(filename).arg(downloadSite).arg(extraText)
                );
    ui->button->setFocus();

    fileDialog.setOption(QFileDialog::DontUseNativeDialog, true);
    fileDialog.setFileMode(QFileDialog::ExistingFile);
    fileDialog.setReadOnly(true);
    fileDialog.setDirectory(Application::getTmpDir());
    fileDialog.setNameFilter(filename);
    fileDialog.selectFile(filename);
}


ManualDownload::~ManualDownload()
{
    delete ui;
}


QString ManualDownload::getFile(QString filename, QString downloadSite, QString extraText)
{
    ManualDownload dialog(filename, downloadSite, extraText);
    if (dialog.exec() == Accepted) {
        qDebug() << "File selected:" << dialog.ui->filename->text();
        return dialog.ui->filename->text();
    } else {
        qDebug() << "File dialog aborted.";
        return QString();
    }
}


void ManualDownload::on_button_clicked()
{
    if (fileDialog.exec() == Accepted) {
        ui->filename->setText(fileDialog.selectedFiles().first());
    }
}
