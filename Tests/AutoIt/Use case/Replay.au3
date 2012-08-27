; Test Case Number:	-
; Feature:			Replay
; Critical (C):		Y
; Test Cases:		-
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===

Func _SetOneFolderUp()
	$CurrentDirPath = @ScriptDir
	ConsoleWrite($CurrentDirPath&@CRLF)
	$arrStrings = StringSplit($CurrentDirPath, "\")

	ConsoleWrite("Size  = "&$arrStrings[0]&@CRLF)
	ConsoleWrite("Value = "&$arrStrings[$arrStrings[0]]&@CRLF)

	$CntRemove = stringlen($arrStrings[$arrStrings[0]]) + 1
	$TrimmedPath = StringTrimRight($CurrentDirPath, $CntRemove)

	ConsoleWrite("Trimmed  = "&$TrimmedPath&@CRLF)
	return $TrimmedPath
EndFunc

WinActivate("BUSMASTER")
Dim $DataPath
if WinExists("BUSMASTER") Then
	sleep(2000)
	WinMenuSelectItem("BUSMASTER","","&Configure","&Replay")   ; select Configure-->Replay
	if winexists("Replay Configuration") Then
		ControlClick("Replay Configuration","",1262)			; Click Add button
		sleep(1000)
		if WinExists("Select Replay File Name") Then
			$DataPath = _SetOneFolderUp()
			send($DataPath&"\EmptyLogFile.log")					; Enter file name
			send("{ENTER}")										; click enter
		EndIf	
		Controlclick("Replay Configuration","OK","[CLASS:Button; INSTANCE:9]")	; Click OK
	EndIf
	send("{F2}")												; select connect
	sleep(2000)
	send("{ESC}")												; select disconnect
EndIf

if WinExists("BUSMASTER") Then
	sleep(2000)
	WinMenuSelectItem("BUSMASTER","","&Configure","&Replay")   ; select Configure-->Replay
	if winexists("Replay Configuration") Then
		
		ControlClick("Replay Configuration","",4)			; Click Delete button		
		sleep(1000)
		Send("!y")			; Click Yes on popup dialog
		
		ControlClick("Replay Configuration","",1262)			; Click Add button
		sleep(1000)
		if WinExists("Select Replay File Name") Then
			$DataPath = _SetOneFolderUp()
			send($DataPath&"\CanLog.log")						; Enter file name
			send("{ENTER}")										; click enter
		EndIf
		ControlCommand("Replay Configuration","",1014,"Check")	; Checkinteractive replay checkbox			
		Controlclick("Replay Configuration","OK","[CLASS:Button; INSTANCE:9]")	; Click OK
	EndIf
	sleep(500)
	send("{F2}")												; select connect
	send("!urg")												; select Functions -> Replay -> Go
	Sleep(1000)
	send("!urt")												; select Functions -> Replay -> Stop
	Sleep(1000)
	send("!urs")												; select Functions -> Replay -> Step
	Sleep(500)
	send("!urk")												; select Functions -> Replay -> Skip
	send("!urg")												; select Functions -> Replay -> Go
	Sleep(500)
	send("{ESC}")												; select disconnect
	Sleep(500)
	WinMenuSelectItem("BUSMASTER","","&Configure","&Replay")     ; select Configure-->Replay
	Sleep(500)
	ControlCommand("Replay Configuration","",1009,"UnCheck")	; click recorded delay
	ControlSetText("Replay Configuration","",1010,3)			; enter msg delay
	ControlCommand("Replay Configuration","",1013,"Check")		; select replay mode->cyclic
	ControlSetText("Replay Configuration","",1011,3)			;enter cyclic delay
	ControlCommand("Replay Configuration","",1266,"SelectString","Tx") ;select msg type
	Controlclick("Replay Configuration","OK","[CLASS:Button; INSTANCE:9]")	; Click OK
	Sleep(1000)
	send("{F2}")												; select connect
	send("!urg")												; select Functions -> Replay -> Go
	sleep(2000)
	send("{ESC}")												; select disconnect
	sleep(500)
	WinMenuSelectItem("BUSMASTER","","&Configure","&Replay")	; select Configure-->Replay
	sleep(1000)
	ControlCommand("Replay Configuration","",1266,"SelectString","All")	;select msg type
	ControlCommand("Replay Configuration","",1014,"UnCheck")			;Uncheck interactive replay checkbox
	Controlclick("Replay Configuration","OK","[CLASS:Button; INSTANCE:9]")	; Click OK
	sleep(1000)
	send("{F2}")								; select connect
	sleep(2000)
	send("{ESC}")								; select disconnect
EndIf