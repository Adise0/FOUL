Set oWS = WScript.CreateObject("WScript.Shell")
Set oFS = CreateObject("Scripting.FileSystemObject")

' Confirm
answer = MsgBox("Are you sure you want to uninstall FOUL?", vbYesNo)
If answer = vbNo Then WScript.Quit

' Get install path from registry
sInstallPath = oWS.RegRead("HKLM\SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\FOUL\InstallPath")

' Remove Start Menu shortcut
sLinkFile = oWS.SpecialFolders("StartMenu") & "\FOUL.lnk"
If oFS.FileExists(sLinkFile) Then oFS.DeleteFile sLinkFile

' Remove install folder
If oFS.FolderExists(sInstallPath) Then oFS.DeleteFolder sInstallPath, True

' Remove registry entry
oWS.RegDelete "HKLM\SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\FOUL\"

MsgBox "FOUL has been uninstalled."
