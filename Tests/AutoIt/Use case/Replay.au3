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

Dim $DataPath
if WinExists("BUSMASTER") Then
	sleep(2000)
	WinMenuSelectItem("BUSMASTER","","&Configure","&Replay")   ; select Configure-->Replay
	if winexists("Replay Configuration") Then
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
	send("{F2}")												; select connect
	sleep(2000)
	send("{ESC}")												; select disconnect
	WinMenuSelectItem("BUSMASTER","","&Configure","&Replay")     ; select Configure-->Replay
	ControlCommand("Replay Configuration","",1009,"UnCheck")	; click recorded delay
	ControlSetText("Replay Configuration","",1010,3)			; enter msg delay
	ControlCommand("Replay Configuration","",1013,"Check")		; select replay mode->cyclic
	ControlSetText("Replay Configuration","",1011,3)			;enter cyclic delay
	ControlCommand("Replay Configuration","",1266,"SelectString","Tx") ;select msg type
	Controlclick("Replay Configuration","OK","[CLASS:Button; INSTANCE:9]")	; Click OK
	send("{F2}")												; select connect
	sleep(2000)
	send("{ESC}")												; select disconnect
	WinMenuSelectItem("BUSMASTER","","&Configure","&Replay")	; select Configure-->Replay
	ControlCommand("Replay Configuration","",1266,"SelectString","All")	;select msg type
	ControlCommand("Replay Configuration","",1014,"UnCheck")			;Uncheck interactive replay checkbox
	Controlclick("Replay Configuration","OK","[CLASS:Button; INSTANCE:9]")	; Click OK
	send("{F2}")								; select connect
	sleep(2000)
	send("{ESC}")								; select disconnect
EndIf