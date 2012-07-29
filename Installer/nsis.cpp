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
#include <QDir>
#include <QFile>
#include <QTextStream>
#include "application.h"
#include "nsis.h"

NSIS::NSIS() :
    Installer()
{
}


NSIS::~NSIS()
{
    cleanup();
}


bool NSIS::build(
        QString name,
        QString outputFile,
        Compression compression,
        uint minSpaceRequired,
        QStringList blockingProcesses,
        QStringList files,
        QString script,
        QString extraHeader
        )
{
    // Get NSIS template
    QFile main(":/NSIS/main.nsi");
    main.open(QIODevice::ReadOnly);
    QString source = QTextStream(&main).readAll();

    // Enable nonstandard plugins
    source.replace("${extraPlugins}", Application::getTmpDir());

    // Substitute constants. This has the same effect as defining them on the
    // command line.
    source.replace("${installerName}", name);
    source.replace("${outputFile}", outputFile);
    source.replace("${minSpaceRequired}", QString::number(minSpaceRequired));

    // Compose "Name" command
    QString nameCmd("Name \"%1\"");
    if (name.contains("&")) {
        // Add second argument with doubled ampersands.
        nameCmd += " \"";
        nameCmd += name;
        nameCmd += "\"";
        nameCmd.replace("&", "&&");
    }
    source.replace("### INSTALLER_NAME ###", nameCmd.arg(name));

    // Set compression
    switch (compression) {
    case NSIS::None:
        source.replace("### COMPRESSION ###", "SetCompress off");
        break;
    case NSIS::Zlib:
        source.replace("### COMPRESSION ###", "SetCompressor /SOLID zlib");
        break;
    case NSIS::Bzip2:
        source.replace("### COMPRESSION ###", "SetCompressor /SOLID bzip2");
        break;
    case NSIS::Lzma:
        source.replace("### COMPRESSION ###", "SetCompressor /SOLID lzma");
        break;
    default:
        throw; // invalid method call, should never happen in production
    }

    // Force termination of configured processes.
    QStringList processes;
    foreach (QString process, blockingProcesses) {
        processes << QString("!insertmacro blockOnProcess \"%1\"").arg(process);
    }
    source.replace("### BLOCK_ON_PROCESSES ###", processes.join("\n    "));

    // Add configured files.
    QStringList fileCommands;
    foreach (QString file, files) {
        fileCommands << QString("File \"%1\"").arg(QDir::toNativeSeparators(file));
    }
    source.replace("### FILES ###", fileCommands.join("\n    "));

    // Insert extra headers
    source.replace("### EXTRA_HEADER ###", extraHeader);

    // Insert installation script fragment.
    source.replace("### INSTALL_PACKAGE ###", script);

    // Copy System plugin if configured.
    systemPluginCopied = false;
    QString systemPlugin(Application::getConfig("installer_NSIS/System Plugin").toString());
    if (!systemPlugin.isEmpty()) {
        systemPluginCopied = QFile::copy(systemPlugin, Application::getTmpDir() + "/System.dll");
        if (!systemPluginCopied) {
            Application::critical(
                        tr("The file '%1' could not be copied to the working directory. Check the configuration.")
                        .arg(systemPlugin));
            return false;
        }
    }

    // Copy nsProcess plugin if necessary.
    nsProcessPluginCopied = false;
    if (!blockingProcesses.isEmpty()) {
        nsProcessPluginCopied = QFile::copy(":NSIS/plugins/nsProcess.dll", Application::getTmpDir() + "/nsProcess.dll");
        if (!nsProcessPluginCopied) {
            Application::critical(
                        tr("The nsProcess plugin could not be copied to the working directory. Check the configuration.")
                        );
            return false;
        }
    }

    // Run makensis on generated script
    command->commands << Command::cmdSpec(
                             Application::getConfig("installer_NSIS/makensis", "makensis").toString(),
                             QStringList() << "-V2" <<"-", // display only warnings and errors, get source from STDIN
                             source
                             );
    command->successMessage = tr("The installer has been built successfully. "
                                 "It can be found at '%1'").arg(outputFile);
    command->exec();

    cleanup();

    return !command->hasErrors();
}


void NSIS::cleanup()
{
    if (systemPluginCopied) {
        QFile::remove(Application::getTmpDir() + "/System.dll");
        systemPluginCopied = false;
    }
    if (nsProcessPluginCopied) {
        QFile::remove(Application::getTmpDir() + "/nsProcess.dll");
        nsProcessPluginCopied = false;
    }
}
