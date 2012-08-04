
    # Delete autostart key. The key might not exist, in which case
    # DeleteRegValue would set the error flag. This is not an actual error and
    # the previous state of the error flag should be preserved.
    ${If} ${Errors}
        DeleteRegValue HKLM 'Software\Microsoft\Windows\CurrentVersion\Run' 'Adobe ARM'
        SetErrors
    ${Else}
        DeleteRegValue HKLM 'Software\Microsoft\Windows\CurrentVersion\Run' 'Adobe ARM'
        ClearErrors
    ${EndIf}
