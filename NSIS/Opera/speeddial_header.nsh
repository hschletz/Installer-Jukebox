!insertmacro GetFileName
!insertmacro Locate

Function deleteSpeedDial
    Delete $R9 # $R9 contains the full path of a matching file
    Push 0     # Return value
FunctionEnd
