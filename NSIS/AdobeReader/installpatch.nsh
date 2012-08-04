
    # Install a MSP file if necessary

    ${If} $REGKEY == ''
        # No previous installation. MSI has just been installed. Install MSP too.
        StrCpy $0 TRUE
    ${Else}
        # Install MSP only if its version is newer than the installed version.
        ReadRegStr $1 HKLM $REGKEY 'DisplayVersion'
        ${VersionCompare} $1 '${MspVersion}' $2
        ${If} $2 = 2
            StrCpy $0 TRUE
        ${Else}
            StrCpy $0 FALSE
        ${EndIf}
    ${EndIf}

    ${If} $0 == TRUE
        ExecWait 'msiexec /p "$PLUGINSDIR\${MspFile}" /qn'
    ${EndIf}
