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

#ifndef VLCMEDIAPLAYER_H
#define VLCMEDIAPLAYER_H

#include "Installer/nsis.h"
#include "package.h"

/**
 * @brief VLC Media Player package
 */
class VlcMediaPlayer : public Package
{
public:

    /**
     * @brief Constructor
     */
    VlcMediaPlayer();

    void build(NSIS *installer, Version version);

private:

    /**
     * @brief Download and build a customized installer
     *
     * The VLC installer offers almost no command line options to control the
     * installation. Fortunately, its NSIS source code is available in the 7z
     * download. It can be patched and rebuilt with the required functionality.
     *
     * This modified installer still receives a wrapper for the generic
     * functionality.
     */
    void download(Version version, Installer *installer);

    /**
     * @brief Patch the NSIS source code
     *
     * This function replaces strings in the NSIS source code, but only if it
     * is really present and unambiguous. This is a precaution for changes in
     * the source code which would otherwise cause silent failure.
     * @param source Reference to source code (will be modified)
     * @param before String to replace
     * @param after Replacement
     * @return bool Success
     */
    bool patch(QString &source, QString before, QString after);
};

#endif // VLCMEDIAPLAYER_H
