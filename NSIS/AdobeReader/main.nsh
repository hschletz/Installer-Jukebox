# Adobe Reader

    Var /GLOBAL REGKEY
    Var /GLOBAL INSTALLMSI

    # Block on open IE windows because of browser plugin
    !insertmacro blockOnIE

    # Skip MSI installation if matching version is already installed.

    # Loop through all subkeys. $0 contains registry enumerator.
    StrCpy $0 0
    ${Do}
        EnumRegKey $1 HKLM 'Software\Microsoft\Windows\CurrentVersion\Uninstall' $0
        ${If} $1 == ''
            # No more subkeys. Abort loop and enable installation of MSI and MSPs.
            StrCpy $REGKEY ''
            StrCpy $INSTALLMSI TRUE
            ${ExitDo}
        ${EndIf}

        # MSI installations start with '{'.
        StrCpy $2 $1 1
        ${If} $2 == '{'
            # Compare beginning of 'DisplayName' and numeric version info
            StrCpy $REGKEY 'Software\Microsoft\Windows\CurrentVersion\Uninstall\$1'
            ReadRegStr   $2 HKLM $REGKEY 'DisplayName'
            ReadRegDWORD $3 HKLM $REGKEY 'VersionMajor'
            ReadRegDWORD $4 HKLM $REGKEY 'VersionMinor'
            StrCpy $2 $2 12
            ${If} $2 == 'Adobe Reader'
            ${AndIf} $3 = ${VersionMajor}
            ${AndIf} $4 = ${VersionMinor}
                # MSI of the same version already installed. Skip MSI
                # installation and abort loop. $REGKEY will point to the
                # installation info.
                StrCpy $INSTALLMSI FALSE
                ${ExitDo}
            ${EndIf}
        ${EndIf}
        IntOp $0 $0 + 1
    ${Loop}

    # Install MSI if required.
    ${If} $INSTALLMSI == TRUE
        ExecWait 'msiexec /i "$PLUGINSDIR\${MsiFile}" /qn IGNORE_INTEROP_REQS=YES ${MsiOptions}'
    ${EndIf}
