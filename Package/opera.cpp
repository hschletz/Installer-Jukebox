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
#include <QFileInfo>
#include <QTextStream>
#include "application.h"
#include "downloader.h"
#include "opera.h"

Opera::Opera() :
    Package("Opera", "12.00")
{
}


void Opera::build(NSIS *installer, Version version)
{
    // Load main script and construct installer command line.
    QString src(loadResource(":NSIS/Opera/main.nsh"));
    QString installerOptions;
    QTextStream stream(&installerOptions);
    stream << " /desktopshortcut " << getConfig("Desktop shortcut", true).toBool();
    stream << " /quicklaunchshortcut " << getConfig("Quick launch shortcut", true).toBool();
    stream << " /startmenushortcut " << getConfig("Start menu shortcut", true).toBool();
    stream << " /allusers " << !getConfig("Install for current user only", false).toBool();
    stream << " /singleprofile " << getConfig("Single profile", false).toBool();
    stream << " /setdefaultbrowser " << getConfig("Set default browser", true).toBool();
    qDebug() <<"Installer options:" << installerOptions;
    src.replace("${Opera}", QString("Opera_%1_int_Setup.exe").arg(version.stripDots()));
    src.replace("${InstallerOptions}", installerOptions);

    QStringList files;
    QString extraHeader;

    // Install customized speed dial.
    QString speedDial(getConfig("Speed dial template").toString());
    if (!speedDial.isEmpty()) {
        qDebug() << "Using speed dial template:" << speedDial;
        files << speedDial;
        src += loadResource(":NSIS/Opera/speeddial.nsh").replace("${SpeedDial}", QFileInfo(speedDial).fileName());
        extraHeader += loadResource(":NSIS/Opera/speeddial_header.nsh");
    }

    // Customize configuration files
    src += setOptionNSIS("Use password manager", boolean, "User Prefs", "Enable Wand");
    src += setOptionNSIS("Use phishing filter", boolean, "User Prefs", "Enable Trust Rating");
    src += setOptionNSIS("Disk cache size", integer, "Disk Cache", "Size");
    src += setOptionNSIS("Use automatic update", boolean_inverted, "User Prefs", "Disable Opera Package AutoUpdate");
    src += setOptionNSIS("Local hostnames", stringlist, "User Prefs", "IntranetHosts");
    src += setOptionNSIS("Hostname expansion", boolean, "Network", "Enable HostName Expansion");
    src += setOptionNSIS("Search network neighborhood", boolean, "Network", "Check Local HostName");
    src += setOptionNSIS("Enable Do Not Track Header", boolean, "Network", "Enable Do Not Track Header");
    src += setOptionNSIS("Mail handler", &Opera::setMailHandlerNSIS);
    src += setOptionNSIS("Proxy configuration script", &Opera::setProxyScriptNSIS);

    // Construct application path depending on configuration.
    if (getConfig("Install for current user only", false).toBool()) {
        src.replace("${OperaDir}", "$LOCALAPPDATA\\Programs\\Opera");
    } else {
        src.replace("${OperaDir}", "$PROGRAMFILES32\\Opera");
    }

    // Download and build the package.
    isError = false;
    download(version);
    if (!isError) {
        files << tempFiles;
        installer->build(
                    objectName(),
                    getOutputFile(),
                    NSIS::None,
                    50,
                    QStringList("opera.exe"),
                    files,
                    src,
                    extraHeader
                    );
    }
    cleanup();
}


void Opera::download(Version version)
{
    QString url("http://get.opera.com/pub/opera/win/%1/int/Opera_%1_int_Setup.exe");
    QString target(
                Downloader::get(
                    url.arg(version.stripDots()),
                    Application::getTmpDir()
                    )
                );
    if (target.isEmpty()) {
        isError = true;
    } else {
        tempFiles << target;
    }
}


QString Opera::setOptionNSIS(QString name, optionType type, QString section, QString key)
{
    // Return NSIS code for both forms ("name" and "name*).
    return
            setOptionNSIS(defaults, section, key, getConfig(name), type)
            + setOptionNSIS(fixed, section, key, getConfig(name + "*"), type);
}


QString Opera::setOptionNSIS(QString name, setOptionNSIScallback callback)
{
    return (this->*callback)(defaults, getConfig(name)) + (this->*callback)(fixed, getConfig(name + "*"));
}


QString Opera::setOptionNSIS(iniFile file, QString section, QString key, QVariant value, optionType type)
{
    QString cmd;
    if (value.isValid()) {
        cmd = "    WriteINIStr '%2' '%3' '%4' '%1'\n"; // WriteINIStr file section key value
        // Replace %1 with value
        switch (type) {
        case string:
            cmd = cmd.arg(value.toString());
            break;
        case integer:
            cmd = cmd.arg(value.toInt());
            break;
        case boolean:
            cmd = cmd.arg(value.toBool());
            break;
        case boolean_inverted:
            cmd = cmd.arg(!value.toBool());
            break;
        case stringlist:
            // Convert space-separated string into a comma-separated string
            cmd = cmd.arg(value.toString().split(" ", QString::SkipEmptyParts).join(","));
            break;
        default:
            throw; // Programming error, should never happen
        }
    } else {
        // Option was not configured. Delete it explicitly.
        cmd = "    DeleteINIStr '%1' '%2' '%3'\n"; // DeleteINIStr file section key
    }

    return cmd.arg(file == fixed ? "$SYSDIR\\operaprefs_fixed.ini" : "${OperaDir}\\operaprefs_default.ini")
            .arg(section).arg(key);
}


QString Opera::setMailHandlerNSIS(iniFile file, QVariant value)
{
    QString handler(value.toString());
    if (handler.isEmpty()) {
        return setOptionNSIS(file, "Mail", "External Application", QVariant(), string)
                + setOptionNSIS(file, "Mail", "Handler", QVariant(), integer);
    } else if (handler.compare("builtin", Qt::CaseInsensitive) == 0) {
        return setOptionNSIS(file, "Mail", "External Application", QVariant(), string)
                + setOptionNSIS(file, "Mail", "Handler", 1, integer);
    } else if (handler.compare("system", Qt::CaseInsensitive) == 0) {
        return setOptionNSIS(file, "Mail", "External Application", QVariant(), string)
                + setOptionNSIS(file, "Mail", "Handler", 3, integer);
    } else {
        return setOptionNSIS(file, "Mail", "External Application", handler, string)
                + setOptionNSIS(file, "Mail", "Handler", 2, integer);
    }
}


QString Opera::setProxyScriptNSIS(iniFile file, QVariant value)
{
    if (value.isValid()) {
        return setOptionNSIS(file, "Proxy", "Use Automatic Proxy Configuration", 1, integer)
                + setOptionNSIS(file, "Proxy", "Automatic Proxy Configuration URL", value, string);
    } else {
        return setOptionNSIS(file, "Proxy", "Use Automatic Proxy Configuration", QVariant(), integer)
                + setOptionNSIS(file, "Proxy", "Automatic Proxy Configuration URL", QVariant(), string);
    }
}
