; === Description ===
; This test checks whether the installer is usable and has expected behavior.
; Unexpected behavior would be crashes, problems in copying files, etc.

$InstallerPath = "../../../Installation Setup"

; Search for the latest installer
$Search = FileFindFirstFile($InstallerPath & "/BUSMASTER_Installer*.exe");
if $Search = -1 Then
	ConsoleWrite("Installer file not found" & @CRLF);
	Exit
EndIf
$Installer = ""
While 1
	$Next = FileFindNextFile($Search)
	if @error Then ExitLoop

	$Installer = $Next
WEnd
FileClose($Search)

; Execute installer
ConsoleWrite("Using " & $Installer & " ..." & @CRLF);
Run($InstallerPath & "/" & $Installer)
if @error Then
	ConsoleWriteError("Installer didn't run" & @CRLF);
	Exit
EndIf

; BUSMASTER Setup: License Agreement
WinWaitActive("BUSMASTER Setup: License Agreement", "", 15)
if @error Then
	ConsoleWriteError("License Agreement didn't activate" & @CRLF);
	Exit
EndIf
send("!A") ; I Agree
WinWaitClose("BUSMASTER Setup: License Agreement", "", 3)
if @error Then
	ConsoleWriteError("License Agreement didn't close" & @CRLF);
	Exit
EndIf

; BUSMASTER Setup: Installation Options
;WinWaitActive("BUSMASTER Setup: Installation Options", "", 3)
;if @error Then
;	ConsoleWriteError("Installation Options didn't activate" & @CRLF);
;	Exit
;EndIf
;send("!N") ; Next
;WinWaitClose("BUSMASTER Setup: Installation Options", "", 3)
;if @error Then
;	ConsoleWriteError("Installation Options didn't close" & @CRLF);
;	Exit
;EndIf

; BUSMASTER Setup: Installation Folder
WinWaitActive("BUSMASTER Setup: Installation Folder", "", 3)
if @error Then
	ConsoleWriteError("Installation Folder didn't activate" & @CRLF);
	Exit
EndIf
send("!I") ; Install
WinWaitClose("BUSMASTER Setup: Installation Folder", "", 3)
if @error Then
	ConsoleWriteError("Installation Folder didn't close" & @CRLF);
	Exit
EndIf

; BUSMASTER Setup: Installing
WinWaitActive("BUSMASTER Setup: Installing", "", 3)
if @error Then
	ConsoleWriteError("Installing didn't activate" & @CRLF);
	Exit
EndIf
WinWaitClose("BUSMASTER Setup: Installing", "", 60)
if @error Then
	ConsoleWriteError("Installing didn't close" & @CRLF);
	Exit
EndIf

; BUSMASTER Setup: Completed
WinWaitActive("BUSMASTER Setup: Completed", "", 3)
if @error Then
	ConsoleWriteError("Completed didn't activate" & @CRLF);
	Exit
EndIf
send("!C") ; Close
WinWaitClose("BUSMASTER Setup: Completed", "", 3)
if @error Then
	ConsoleWriteError("Completed didn't close" & @CRLF);
	Exit
EndIf
