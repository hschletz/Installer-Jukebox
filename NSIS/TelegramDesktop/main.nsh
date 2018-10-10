# Telegram Desktop

# Force installation in Program folder instead of user's roaming profile
ExecWait '"$PLUGINSDIR\${Installer}" /VERYSILENT /NORESTART /DIR="$PROGRAMFILES32\Telegram Desktop"'


# Make desktop shortcut and start menu entry global.
# The Copy+Delete operation ensures correct ACL (Rename would keep the user's ACL).

Var /GLOBAL desktopUser
Var /GLOBAL startMenuUser
StrCpy $desktopUser $DESKTOP
StrCpy $startMenuUser $SMPROGRAMS

SetShellVarContext all

${IfNot} ${FileExists} "$DESKTOP\Telegram.lnk"
    CopyFiles /SILENT "$desktopUser\Telegram.lnk" "$DESKTOP\Telegram.lnk"
${EndIf}
Delete /REBOOTOK "$desktopUser\Telegram.lnk"

${IfNot} ${FileExists} "$SMPROGRAMS\Telegram Desktop"
    CopyFiles /SILENT "$startMenuUser\Telegram Desktop" "$SMPROGRAMS\Telegram Desktop"
${EndIf}
Delete /REBOOTOK "$startMenuUser\Telegram Desktop"
