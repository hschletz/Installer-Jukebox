!addplugindir ${extraPlugins}
!include "MUI2.nsh"
!include "LogicLib.nsh"
!include "FileFunc.nsh"
!include x64.nsh

### COMPRESSION ###

!define MUI_WELCOMEPAGE_TITLE "${installerName}"
!define MUI_WELCOMEPAGE_TEXT "${installerName} will be installed fully automated and customized."
!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_LANGUAGE "English"

!insertmacro GetRoot
!insertMacro DriveSpace

!macro blockOnProcess process
    nsProcess::_FindProcess /NOUNLOAD "${process}"
    Pop $0
    ${While} $0 == 0
        MessageBox MB_OK|MB_ICONINFORMATION "Please quit the program '${process}' and click 'OK' to allow the update of ${installerName}."
        nsProcess::_FindProcess /NOUNLOAD "${process}"
        Pop $0
    ${EndWhile}
    nsProcess::_Unload
!macroend

!macro blockOnIE
    FindWindow $0 "IEFrame"
    ${While} $0 <> 0
        FindWindow $0 "IEFrame"
        MessageBox MB_OK|MB_ICONINFORMATION "Please close all Internet Explorer windows and click 'OK' to allow the update of ${installerName}."
        FindWindow $0 "IEFrame"
    ${EndWhile}
!macroend

ShowInstDetails show

### INSTALLER_NAME ###
OutFile "${outputFile}"
RequestExecutionLevel admin

### EXTRA_HEADER ###

Function .onInit
    # Force admin privileges
    UserInfo::GetAccountType
    Pop $0
    ${If} $0 != "Admin"
        MessageBox MB_OK|MB_ICONEXCLAMATION "Administrator privileges required. Aborting." /SD IDOK
        Abort
    ${EndIf}

    # Prevent multiple instances of this installer
    System::Call 'kernel32::CreateMutexA(i 0, i 0, t "${installerName}") ?e'
    Pop $0
    ${If} $0 != "0"
        MessageBox MB_OK|MB_ICONEXCLAMATION "Another installation of ${installerName} is currently in progress. Aborting." /SD IDOK
        Abort
    ${EndIf}

    # Check for free disk space
    ${GetRoot} "$PROGRAMFILES" $0  # Typically "C:"
    ${DriveSpace} "$0\" "/D=F /S=M" $1 # Free space in MB
    ${If} ${Errors}
        MessageBox MB_OK|MB_ICONEXCLAMATION "Could not check available disk space on drive $0. Aborting." /SD IDOK
        Abort
    ${EndIf}
    ${If} $1 < ${minSpaceRequired}
        MessageBox MB_OK|MB_ICONEXCLAMATION "Not enough free space on drive $0 (${minSpaceRequired} MB required). Aborting." /SD IDOK
        Abort
    ${EndIf}
FunctionEnd

Section
    InitPluginsDir
    SetOutPath $PLUGINSDIR

    ### BLOCK_ON_PROCESSES ###

    ### FILES ###

    ### INSTALL_PACKAGE ###

    ${If} ${Errors}
        MessageBox MB_OK|MB_ICONEXCLAMATION "Installation of ${installerName} failed." /SD IDOK
        SetErrorLevel 2
    ${Else}
        SetErrorLevel 0
    ${EndIf}
SectionEnd

Function .onInstSuccess
    # The temporary PLUGINSDIR will not be deleted if it is still in use as output path.
    # Change output directory to allow proper cleanup.
    SetOutPath $TEMP
FunctionEnd
