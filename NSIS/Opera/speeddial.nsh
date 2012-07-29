
    # Use own speed dial as template for new profiles
    ${GetFileName} '${SpeedDial}' $0
    DetailPrint "Using '$0' as speed dial for new profiles."

    # The template is placed in the defaults directory.
    CopyFiles /SILENT '$PLUGINSDIR\$0' '${OperaDir}\defaults\standard_speeddial.ini'

    # Some subdirectories of 'locale' and 'region' contain a speed dial template
    # which takes precedence over the default one. Delete them all.
    ${Locate} '${OperaDir}\locale' '/L=F /M=standard_speeddial.ini /G=1' deleteSpeedDial
    ${Locate} '${OperaDir}\region' '/L=F /M=standard_speeddial.ini /G=1' deleteSpeedDial
