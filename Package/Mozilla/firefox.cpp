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

#include "firefox.h"

Firefox::Firefox() :
    Mozilla("Firefox", "14.0")
{
}


void Firefox::build(NSIS *installer, Version version)
{
    isError = false;

    QString src(loadResource(":NSIS/Firefox/main.nsh"));
    src.replace("${Version}", version);

    QString prefs(getPrefs());

    if (prefs.isEmpty()) {
        src += loadResource(":NSIS/Firefox/deleteprefs.nsh");
    } else {
        src += loadResource(":NSIS/Firefox/writeprefs.nsh");
        writePrefsFile(prefs);
    }

    if (!autoUpdate) {
        src += loadResource(":NSIS/Firefox/uninstallmaintenanceservice.nsh");
    }

    if (!isError) {
        download(version);
    }

    if (!isError) {
        createInstallerIni();
        installer->build(
                    objectName(),
                    getOutputFile(),
                    NSIS::None,
                    70,
                    QStringList("firefox.exe"),
                    tempFiles,
                    src
                    );
    }

    cleanup();
}


QString Firefox::getPrefs()
{
    autoUpdate = true;
    QString prefs(Mozilla::getPrefs());

    prefs += setOption("Use automatic update", (setOptionCallback) &Firefox::setAutoUpdate);
    prefs += setOption("Disk cache size", integer, "browser.cache.disk.capacity");
    prefs += setOption("Ask for download directory", boolean_inverted, "browser.download.useDownloadDir");
    if (!getConfig("Show upgrade page", true).toBool()) {
        prefs += "lockPref(\"browser.startup.homepage_override.mstone\", \"ignore\");\r\n";
    }

    return prefs;
}


QString Firefox::setAutoUpdate(QString cmdTemplate, QVariant value)
{
    autoUpdate = value.toBool();
    return cmdTemplate.arg("app.update.enabled").arg(autoUpdate ? "true" : "false");
}
