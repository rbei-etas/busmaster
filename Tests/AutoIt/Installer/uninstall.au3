; === Description ===
; This test checks whether the uninstaller is usable and has expected behavior.
; Unexpected behavior would be crashes, problems in copying files, etc.

; Execute Uninstaller
Run(@ProgramFilesDir & "\BUSMASTER\uninst.exe")
if @error Then
	ConsoleWriteError("Uninstaller didn't run" & @CRLF)
	Exit
EndIf

; BUSMASTER Uninstall: Confirmation
WinWaitActive("BUSMASTER Uninstall: Confirmation", "", 15)
if @error Then
	ConsoleWriteError("Confirmation didn't activate" & @CRLF)
	Exit
EndIf
send("!u") ; Uninstall
WinWaitClose("BUSMASTER Uninstall: Confirmation", "", 3)
if @error Then
	ConsoleWriteError("Confirmation didn't close" & @CRLF)
	Exit
EndIf

; BUSMASTER Uninstall: Uninstalling
WinWaitActive("BUSMASTER Uninstall: Uninstalling", "", 3)
if @error Then
	ConsoleWriteError("Uninstalling didn't activate" & @CRLF)
	Exit
EndIf
WinWaitClose("BUSMASTER Uninstall: Uninstalling", "", 3)
if @error Then
	ConsoleWriteError("Uninstalling didn't close" & @CRLF)
	Exit
EndIf

; BUSMASTER Uninstall: Completed
WinWaitActive("BUSMASTER Uninstall: Completed", "", 3)
if @error Then
	ConsoleWriteError("Uninstall didn't complete" & @CRLF)
	Exit
EndIf
send("!c") ; Close
WinWaitClose("BUSMASTER Uninstall: Completed", "", 3)
if @error Then
	ConsoleWriteError("Completed didn't close" & @CRLF)
	Exit
EndIf
