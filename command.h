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
#ifndef COMMAND_H
#define COMMAND_H

#include <QDialog>
#include <QProcess>
#include <QString>
#include <QStringList>

namespace Ui {
class Command;
}

/**
 * @brief %Command execution dialog
 *
 * This is a modal dialog that executes a command and displays its output. It
 * also provides a "Cancel" buton to terminate the command if possible.
 * Set the command, args, input and successMessage members and call the exec()
 * slot to use it.
 */
class Command : public QDialog
{
    Q_OBJECT

public:

    /**
     * @brief Default constructor
     * @param parent
     */
    explicit Command(QWidget *parent = 0);

    /**
     * @brief Destructor
     *
     */
    ~Command();

    /**
     * @brief Return error status
     * @return bool
     */
    bool hasErrors();

    /**
     * @brief The name of the file to execute
     */
    QString command;

    /**
     * @brief %Command line arguments
     */
    QStringList args;

    /**
     * @brief Data to pass to the running command via STDIN
     */
    QString input;

    /**
     * @brief Message to display on success
     */
    QString successMessage;

public slots:

    /**
     * @brief Display dialog and start command
     * @return int
     */
    int exec();

    /**
     * @brief Send input to process
     *
     */
    void passInput();

    /**
     * @brief Display command output
     */
    void printStdOut();

    /**
     * @brief Display status message, close dialog if there is no output
     * @param exitCode
     * @param exitStatus
     */
    void finish(int exitCode, QProcess::ExitStatus exitStatus);

    /**
     * @brief Error actions
     * @param error
     */
    void handleError(QProcess::ProcessError error);

private slots:

    /**
     * @brief Terminate process or close dialog, depending on process status
     */
    void on_button_clicked();

private:

    /**
     * @brief UI elements
     */
    Ui::Command *ui;

    /**
     * @brief Process object
     */
    QProcess process;

    /**
     * @brief Error status
     */
    bool isError;
};

#endif // COMMAND_H
