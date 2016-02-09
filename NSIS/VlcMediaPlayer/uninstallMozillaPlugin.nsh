    DeleteRegKey HKLM "Software\MozillaPlugins\@videolan.org/vlc,version=${Version}"
    Delete /REBOOTOK "$VlcDir\npvlc.dll"
    Delete /REBOOTOK "$VlcDir\npvlc.dll.manifest"
