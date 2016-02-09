    # Store installation directory for postinst scripts
    Var /GLOBAL VlcDir
    StrCpy $VlcDir "$PROGRAMFILES\VideoLAN\VLC"

    ExecWait '"$PLUGINSDIR\${VLCinstaller}" /S'
