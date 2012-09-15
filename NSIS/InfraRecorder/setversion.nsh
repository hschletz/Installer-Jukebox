    # The installer does not set version information in the registry. Do it here.
    ${GetFileVersion} "$PROGRAMFILES32\InfraRecorder\InfraRecorder.exe" $0
    ${IfNot} ${Errors}
        WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\InfraRecorder" "DisplayVersion" $0
    ${EndIf}
