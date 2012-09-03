The Installer Jukebox
=====================

What is it?

The Installer Jukebox generates fully automated and customized installers for
several Windows applications, suitable for automatic and unattended deployment,
or just for frequent reinstallation for personal use. Supported packages are:

- Adobe Reader
- Firefox
- Flash Player ActiveX
- Flash Player Plugin
- Java Runtime Environment 6
- LibreOffice
- OCS Inventory NG agent
- Opera
- Thunderbird
- VLC Media Player


How it works

The original installer gets downloaded at runtime directly from the official
download site or an official mirror - no redistribution from shady origins as
sometimes found on the web. Then some NSIS source code is generated and compiled
that bundles the original installer and performs the unattended installation and
customization of the package.


Requirements

- Qt SDK (http://qt-project.org) for compiling the application
- QuaZIP (http://quazip.sourceforge.net)
- NSIS (http://nsis.sourceforge.net) for generating installers
- For VLC Media Player: a 7z extraction utility, like http://7-zip.org

While the generated installers are only useful in a Windows or WINE environment,
the Installer Jukebox itself can natively run under any OS supported by Qt and
NSIS - Windows, Linux and MacOS.


Compiling

Change to the Installer Jukebox source directory and run:

    qmake
    make release

That's it! This will generate the InstallerJukebox either in the same directory
or under release/, depending on the environment. It can be moved to any
location.

If you have Qt Creator installed, you can also use that to open
InstallerJukebox.pro and build and run the application from there.


Configuration

The application needs a configuration file "InstallerJukebox.ini" in the same
directory as the executable file. A template file
"InstallerJukebox.template.ini" is provided - rename or copy it to
"InstallerJukebox.ini". Open that file in an editor and follow the instructions
in the file.


Proxy support

The application uses the system's default proxy settings, as documented at
http://qt-project.org/doc/qt-4.8/qnetworkproxyfactory.html#systemProxyForQuery


Generating installers

Start the application, select the package (and the version number if required)
and click "Build". If everything went fine, you now have a customized installer
in the configured location.


Running the installer

Just start the installer like any other installer. It will only show an
introduction page and then perform the installation without further questions,
according to your configuration.
For fully automated, non-interactive installations start the installer from the
command line with the /S switch:

    install-MyPackage.exe /S

You won't see any activity at a first glance - the installer will run completely
hidden and terminate itself once it's finished.



License

The Installer Jukebox is released under a revised BSD license. You can find the
full license in
the LICENSE file in the same directory that contains this file.


Third party code

The installer Jukebox contains a bundled copy of the nsProcess plugin v1.6
(http://nsis.sourceforge.net/NsProcess_plugin).
