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
#include <QQueue>
#include <QString>
#include <QStringList>

namespace Ui {
class Command;
}

/**
 * @brief %Command execution dialog
 *
 * This is a modal dialog that executes one or more commands and displays the
 * output. It also provides a "Cancel" button to terminate the command if
 * possible.
 *
 * Commands are queued in the "commands" member variable. Populate this with one
 * or more cmdSpec entries and call the exec() slot to start execution. The
 * commands are executed in order. Execution is aborted on the first error or if
 * the user cancels the running command.
 *
 * If all commands ran without error, the optional successMessage is displayed.
 */
class Command : public QDialog
{
    Q_OBJECT

public:

    /**
     * @brief Specification for a single command
     */
    struct cmdSpec {
        /**
         * @brief Constructor
         * @param program
         * @param args
         * @param input
         */
        cmdSpec(QString program=QString(), QStringList args=QStringList(), QString input=QString()) :
            program(program),
            args(args),
            input(input)
        {
        }
        QString program; /**< The name of the file to execute */
        QStringList args; /**< %Command line arguments */
        QString input; /**< Data to pass to the running command via STDIN */
    };

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
     * @brief %Command queue
     */
    QQueue<cmdSpec> commands;

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
     * @brief Run next command in the queue (if there is one)
     */
    void runCommand();

    /**
     * @brief UI elements
     */
    Ui::Command *ui;

    /**
     * @brief Currently executing command
     */
    cmdSpec currentCommand;

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
