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

#include <QFile>
#include <QSettings>
#include <QTextStream>
#include "application.h"
#include "downloader.h"
#include "mozilla.h"


Mozilla::Mozilla(const char *name, const char *minVersion) :
    Package(name, minVersion)
{
}


void Mozilla::download(Version version)
{
    QString url("https://download.mozilla.org/?product=%1-%2-SSL&os=win64&lang=%3");
    QString target(
                Downloader::get(
                    url
                    .arg(objectName().toLower())
                    .arg(version)
                    .arg(getConfig("Language", "en-US").toString()),
                    Application::getTmpDir(),
                    QString("%1 Setup %2.exe").arg(objectName()).arg(version)
                    )
                );
    if (target.isEmpty()) {
        isError = true;
    } else {
        tempFiles << target;
    }
}


void Mozilla::createInstallerIni()
{
    QString iniFileName(Application::getTmpDir() + "/mozilla.ini");
    QSettings iniFile(iniFileName, QSettings::IniFormat);

    iniFile.setValue("Install/DesktopShortcut", getConfig("Desktop shortcut", true).toBool());
    iniFile.setValue("Install/QuickLaunchShortcut", getConfig("Quick launch shortcut", true).toBool());
    iniFile.setValue("Install/StartMenuShortcuts", getConfig("Start menu shortcut", true).toBool());

    // Check option with and without asterisk.
    QVariant autoUpdate(getConfig("Use automatic update*"));
    if (!autoUpdate.isValid()) {
        autoUpdate = getConfig("Use automatic update", true);
    }
    iniFile.setValue("Install/MaintenanceService", autoUpdate.toBool());

    tempFiles << iniFileName;
}


QString Mozilla::getPrefs()
{
    QString prefs;
    prefs += setOption("Proxy configuration script", &Mozilla::setProxyScript);
    prefs += setOption("Enable geolocation", boolean, "geo.enabled");
    return prefs;
}


QString Mozilla::setOption(QString name, optionType type, QString jsName)
{
    QString cmd;
    QVariant value(getConfig(name + "*"));
    if (value.isValid()) {
        cmd = "lockPref(\"%1\", %2);\r\n";
    } else {
        value = getConfig(name);
        if (value.isValid()) {
            cmd = "defaultPref(\"%1\", %2);\r\n";
        } else {
            return QString();
        }
    }

    cmd = cmd.arg(jsName);

    switch (type) {
    case string:
        cmd = cmd.arg(quoteString(value.toString()));
        break;
    case integer:
        cmd = cmd.arg(value.toInt());
        break;
    case boolean:
        cmd = cmd.arg(value.toBool() ? "true" : "false");
        break;
    case boolean_inverted:
        cmd = cmd.arg(value.toBool() ? "false" : "true");
        break;
    default:
        throw; // Programming error, should never happen
    }

    return cmd;
}


QString Mozilla::setOption(QString name, setOptionCallback callback)
{
    QString cmdTemplate;
    QVariant value(getConfig(name + "*"));
    if (value.isValid()) {
        cmdTemplate = "lockPref(\"%1\", %2);\r\n";
    } else {
        value = getConfig(name);
        if (value.isValid()) {
            cmdTemplate = "defaultPref(\"%1\", %2);\r\n";
        } else {
            return QString();
        }
    }
    return (this->*callback)(cmdTemplate, value);
}


QString Mozilla::quoteString(QString string)
{
    return "\"" + string.replace("\"", "\\\"") + "\"";
}


void Mozilla::writePrefsFile(QString prefs)
{
    // Write prefs to mozilla.cfg
    QString fileName(Application::getTmpDir() + "/mozilla.cfg");
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        // Prepend content with required initial comment line
        QTextStream(&file) << "//\r\n" << prefs;
        file.close();
        tempFiles << fileName;

        // Write local-settings.js with static content from resource
        fileName = Application::getTmpDir() + "/local-settings.js";
        file.setFileName(fileName);
        if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
            QTextStream(&file) << loadResource(":NSIS/Mozilla/local-settings.js");
            file.close();
            tempFiles << fileName;
        } else {
            isError = true;
            Application::critical(tr("Error opening '%1'").arg(fileName));
        }
    } else {
        isError = true;
        Application::critical(tr("Error opening '%1'").arg(fileName));
    }
}


QString Mozilla::setProxyScript(QString cmdTemplate, QVariant value)
{
    return
            cmdTemplate.arg("network.proxy.type").arg(2) +
            cmdTemplate.arg("network.proxy.autoconfig_url").arg(quoteString(value.toString()));
}
