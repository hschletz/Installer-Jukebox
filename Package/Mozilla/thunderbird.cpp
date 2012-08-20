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

#include <QFileInfo>
#include "application.h"
#include "thunderbird.h"

Thunderbird::Thunderbird() :
    Mozilla("Thunderbird", "14.0")
{
}


void Thunderbird::build(NSIS *installer, Version version)
{
    isError = false;

    QStringList files;

    QString src(loadResource(":NSIS/Thunderbird/main.nsh"));
    src.replace("${Version}", version);

    QString prefs(getPrefs());

    if (prefs.isEmpty()) {
        src += loadResource(":NSIS/Thunderbird/deleteprefs.nsh");
    } else {
        src += loadResource(":NSIS/Thunderbird/writeprefs.nsh");
        writePrefsFile(prefs);
    }

    if (!getConfig("Enable Test Pilot", true).toBool()) {
        src += loadResource(":NSIS/Thunderbird/uninstalltestpilot.nsh");
    }

    QString accountConfig(getConfig("Account configuration file").toString());
    if (!accountConfig.isEmpty()) {
        src += loadResource(":NSIS/Thunderbird/accountconfig.nsh")
                .replace("${AccountConfig}", QFileInfo(accountConfig).fileName());
        files << accountConfig;
    }

    if (!isError) {
        download(version);
    }

    if (!isError) {
        files << tempFiles;
        installer->build(
                    objectName(),
                    getOutputFile(),
                    NSIS::None,
                    80,
                    QStringList("thunderbird.exe"),
                    files,
                    src
                    );
    }

    cleanup();
}


QString Thunderbird::getPrefs()
{
    QString prefs(Mozilla::getPrefs());

    prefs += setOption("Use automatic update", boolean, "app.update.enabled");
    prefs += setOption("Allow message cache", (setOptionCallback) &Thunderbird::setDisableCache);
    prefs += setOption("Browser cache size", integer, "browser.cache.disk.capacity");
    prefs += setOption("Show start page", boolean, "mailnews.start_page.enabled");
    prefs += setOption("Display names from address book only", boolean, "mail.showCondensedAddresses");
    prefs += setOption("Request MDN", boolean, "mail.receipt.request_return_receipt_on");
    prefs += setOption("Reply MDN", boolean, "mail.mdn.report.enabled");
    prefs += setOption("Compose HTML messages", boolean, "mail.identity.default.compose_html");
    prefs += setOption("Enable file sharing", boolean, "mail.cloud_files.enabled");
    prefs += setOption("Offer file sharing", boolean, "mail.compose.big_attachments.notify");
    if (!getConfig("Show upgrade page", true).toBool()) {
        prefs += "lockPref(\"mailnews.start_page_override.mstone\", \"ignore\");\r\n";
    }

    return prefs;
}


QString Thunderbird::setDisableCache(QString cmdTemplate, QVariant value)
{
    QString arg(value.toBool() ? "true" : "false");
    return
            cmdTemplate.arg("mail.server.default.autosync_offline_stores").arg(arg) +
            cmdTemplate.arg("mail.server.default.offline_download").arg(arg) +
            cmdTemplate.arg("mailnews.database.global.indexer.enabled").arg(arg) +
            cmdTemplate.arg("mail.ui.show.migration.on.upgrade").arg(arg);
}
