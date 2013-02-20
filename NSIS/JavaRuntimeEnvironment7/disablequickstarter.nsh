    # Remove Quick starter on pre-Vista systems (it's never installed on Vista and later)
    ${If} ${AtMostWinXP}
        ExecWait '"$PROGRAMFILES32\Java\jre7\bin\jqs.exe" -unregister'
    ${EndIf}

