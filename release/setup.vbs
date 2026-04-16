Set oFS = CreateObject("Scripting.FileSystemObject")
Set oWS = WScript.CreateObject("WScript.Shell")

sInstallPath = oFS.GetAbsolutePathName(".")
oWS.RegWrite "HKLM\SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\FOUL\InstallPath", sInstallPath
oWS.RegWrite "HKLM\SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\FOUL\DisplayName", "FOUL"
oWS.RegWrite "HKLM\SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\FOUL\UninstallString", "wscript.exe """ & sInstallPath & "\uninstall.vbs"""
oWS.RegWrite "HKLM\SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\FOUL\DisplayIcon", sInstallPath & "\FOUL.exe"

answer = MsgBox("Create Start Menu shortcut?", vbYesNo)
If answer = vbYes Then


sLinkFile = oWS.SpecialFolders("StartMenu") & "\FOUL.lnk"
Set oLink = oWS.CreateShortcut(sLinkFile)
oLink.TargetPath = sInstallPath & "\FOUL.exe"
oLink.WorkingDirectory = sInstallPath
oLink.IconLocation = sInstallPath & "\FOUL.exe, 0"
oLink.Save

MsgBox "Shortcut created pointing to: " & sInstallPath
End If


