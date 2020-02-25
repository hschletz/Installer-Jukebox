
    # Copy the given template (XCU file)
    ${IfNot} ${Errors}
        StrCpy $0 '$PROGRAMFILES64\LibreOffice\share\registry\data\org\openoffice'
        CreateDirectory $0
        CopyFiles /SILENT '$PLUGINSDIR\${ConfigTemplate}' $0
    ${EndIf}
