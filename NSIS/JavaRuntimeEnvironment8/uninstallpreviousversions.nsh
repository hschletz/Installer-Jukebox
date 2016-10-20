    # Uninstall previous versions

    # Iterate over Uninstall keys and check for matching GUID.
    ${If} ${RunningX64}
        StrCpy $3 'Software\Wow6432Node\Microsoft\Windows\CurrentVersion\Uninstall'
    ${Else}
        StrCpy $3 'Software\Microsoft\Windows\CurrentVersion\Uninstall'
    ${EndIf}
    StrCpy $0 0
    ${Do}
        EnumRegKey $1 HKLM $3 $0
        ${If} $1 == ''
            # No more subkeys. Abort loop.
            ${ExitDo}
        ${EndIf}

        # Check for old versions with 2-digit version number
        StrCpy $2 $1 31
        ${If} $2 == '{26A24AE4-039D-4CA4-87B4-2F8321'
            # Found JRE. Check Version.
            StrCpy $2 $1 35
            ${If} $2 != '{26A24AE4-039D-4CA4-87B4-2F8321${VersionMajor}0${VersionMinor}'
                # Found different version. Uninstall.
                ExecWait 'msiexec /qn /x $1'
            ${EndIf}
        ${EndIf}
        # Check for old versions with 3-digit version number
        StrCpy $2 $1 30
        ${If} $2 == '{26A24AE4-039D-4CA4-87B4-2F321'
            # Found JRE. Check Version.
            StrCpy $2 $1 35
            ${If} $2 != '{26A24AE4-039D-4CA4-87B4-2F321${VersionMajor}0${VersionMinor}'
                # Found different version. Uninstall.
                ExecWait 'msiexec /qn /x $1'
            ${EndIf}
        ${EndIf}
        IntOp $0 $0 + 1
    ${Loop}
