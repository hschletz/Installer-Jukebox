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
#include <QTextStream>
#include "application.h"
#include "command.h"
#include "ui_command.h"

Command::Command(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Command)
{
    ui->setupUi(this);

    // Set up QProcess object
    process.setProcessChannelMode(QProcess::MergedChannels);
    connect(&process, SIGNAL(started()), this, SLOT(passInput()));
    connect(&process, SIGNAL(readyReadStandardOutput()), this, SLOT(printStdOut()));
    connect(&process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(finish(int, QProcess::ExitStatus)));
    connect(&process, SIGNAL(error(QProcess::ProcessError)), this, SLOT(handleError(QProcess::ProcessError)));
}


Command::~Command()
{
    delete ui;
}


bool Command::hasErrors()
{
    return isError;
}


int Command::exec() {
    if (command.isEmpty()) {
        isError = true; // command must be set up before calling this!
    } else {
        isError = false;
        qDebug() << "Running command:" << command + " " + args.join(" ");
        process.start(command, args);
    }
    return QDialog::exec();
}


void Command::passInput()
{
    QTextStream(&process) << input;
    process.closeWriteChannel(); // Required for some commands to know then to terminate
}


void Command::printStdOut()
{
    ui->output->appendPlainText(process.readAllStandardOutput());
}


void Command::finish(int exitCode, QProcess::ExitStatus exitStatus)
{
    qDebug() << "Command finished. Status:" << exitStatus << "Code:" << exitCode;
    if (exitStatus == QProcess::CrashExit) {
        isError = true;
        Application::critical(tr("%1 crashed!").arg(command));
    } else if (exitCode != 0) {
        isError = true;
        Application::critical(tr("%1 returned with exit code %2").arg(command).arg(exitCode));
    } else if (!successMessage.isEmpty()){
        Application::information(successMessage);
    }

    // Close dialog if there is no output, turn "Cancel" into "Close" otherwise
    if (ui->output->toPlainText().isEmpty()) {
        close();
    } else {
        ui->button->setText(tr("Close"));
    }
}


void Command::handleError(QProcess::ProcessError error)
{
    isError = true;
    qDebug() << "Process error:" << error;
    Application::critical(tr("Starting %1 failed with error %2.").arg(command).arg(error));
}


void Command::on_button_clicked()
{
    if (process.state() == QProcess::Running) {
        isError = true;
        process.terminate();
    } else {
        close();
    }
}
