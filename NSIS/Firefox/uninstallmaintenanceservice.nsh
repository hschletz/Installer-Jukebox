    # The documented registry keys for preventing installation of the Mozilla
    # Maintenance Service do not work with silent installations. The service
    # must be uninstalled manually if present.

    # Error flag will be set if the key does not exist. This is not an actual
    # error and the previous state of the error flag should be preserved.
    ${If} ${Errors}
        ReadRegStr $0 HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\MozillaMaintenanceService" "UninstallString"
        SetErrors
    ${Else}
        ReadRegStr $0 HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\MozillaMaintenanceService" "UninstallString"
        ClearErrors
    ${EndIf}

    ${If} $0 != ''
        ExecWait '$0 /S'
    ${EndIf}
