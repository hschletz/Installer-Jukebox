    # Uninstall previous versions

    # Iterate over Uninstall keys and check for matching GUID.
    StrCpy $0 0
    ${Do}
        EnumRegKey $1 HKLM 'Software\Microsoft\Windows\CurrentVersion\Uninstall' $0
        ${If} $1 == ''
            # No more subkeys. Abort loop.
            ${ExitDo}
        ${EndIf}

        # JRE6 installations look like {26A24AE4-039D-4CA4-87B4-2F832160xxxx}
        # JRE7 installations look like {26A24AE4-039D-4CA4-87B4-2F032170xxxx}
        StrCpy $2 $1 33
        ${If} $2 == '{26A24AE4-039D-4CA4-87B4-2F832160'
            # Found JRE6. Uninstall.
            ExecWait 'msiexec /qn /x $1'
            # Old versions before Update 10 may not be uninstalled automatically
            # by later JRE6 updates, so there may be more than 1 JRE6 version
            # installed. Keep iterating to purge all possible incarnations.
        ${EndIf}
        ${If} $2 == '{26A24AE4-039D-4CA4-87B4-2F032170'
            # Found JRE7. Uninstall.
            ExecWait 'msiexec /qn /x $1'
        ${EndIf}
        IntOp $0 $0 + 1
    ${Loop}
