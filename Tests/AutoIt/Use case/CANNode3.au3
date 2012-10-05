;=== Test Cases/Test Data ===
; Critical (C):		Y
; TestCase No.:		TC_03
; Test Cases:		Node Simultion
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

WinActivate("BUSMASTER", 3)
if WinExists("BUSMASTER") Then
	sleep(2000)
;~ 	WinMenuSelectItem("BUSMASTER","","&Configure","&Simulated Systems")   ;select Configure->Simulated Systems
	$pos=ControlGetPos("BUSMASTER","","[CLASS:SysTreeView32; INSTANCE:1]")
	MouseClick("right",$pos[0]+30,$pos[1]+55)							 ; select new sim
	send("n")
	$NodeSimPath = _SetOneFolderUp()
	send($NodeSimPath&"\testCANSim.sim")
	send("{ENTER}")
	sleep(1000)
	if winexists("New Simulated system Configuration Filename...") Then
		controlclick("New Simulated system Configuration Filename...","","[CLASS:Button; INSTANCE:1]")
	EndIf
	MouseClick("right",$pos[0]+50,$pos[1]+65)
	send("a")											 ; select add node
	sleep(1000)
	if WinExists("Node Details") Then
		controlsettext("Node Details","","[CLASS:Edit; INSTANCE:1]","TestNode")	 ; enter node name
		send("{ENTER}")
	EndIf
	sleep(1000)
	if WinExists("BUSMASTER") Then
		controlclick("BUSMASTER","","[CLASS:Button; INSTANCE:3]")				; Click Add File
		sleep(1000)
		if WinExists("Select BUSMASTER Source Filename...") Then
			sleep(1000)
			send($NodeSimPath&"\CANtestinsim")
			send("{ENTER}")
		EndIf
		sleep(1000)
		send("^{F4}")															; Close C file window
		controlclick("BUSMASTER","Edit File...",1002)							; click edit file
		send("^{F4}")															; Close C file window
		sleep(500)
		controlclick("BUSMASTER","Build...",1003)								; click Build
		sleep(500)
		controlclick("BUSMASTER","Build and Load",1004)							; click Build and load
		sleep(500)
		controlclick("BUSMASTER","UnLoad",1005)									; Unload
		sleep(500)
		controlclick("BUSMASTER","Build and Load",1004)							; click Build and load
		sleep(500)
		controlclick("BUSMASTER","Enable Handler",1014)							; click Enable Handler
		sleep(500)
		controlclick("BUSMASTER","Enable All Handlers",1015)					; click Enable All Handler
		sleep(500)
		controlclick("BUSMASTER","Disable Handler",1014)						; click Disable Handler
		sleep(500)
		controlclick("BUSMASTER","Disable All Handlers",1015)					; click Disable All Handler
		sleep(1000)
		controlclick("BUSMASTER","Enable All Handlers",1015)					; click Enable Handler
	EndIf
	send("{F2}")
	send("a")
	sleep(15000)
	send("{ESC}")
EndIf
