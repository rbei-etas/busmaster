; Test Case Number:	TS_Log_01
; Feature:			Log File
; Critical (C):		Y
; Test Cases:		-
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
#include<globalFunc.au3>
#Include <GuiTreeView.au3>
#Include <GuiToolBar.au3>
#Include <GuiListView.au3>


sleep(2000)
if WinExists("BUSMASTER") then
	WinMenuSelectItem("BUSMASTER","","&Configure","&Hardware Interface","&Simulation")
	_loadConfig("SigDesc")					;load cfx file
	_loadDatabase("DBSigDesc")				;load dbf file
	; Add signal desc
	sleep(2000)
	ControlListView("BUSMASTER","","[CLASS:SysHeader32; INSTANCE:1]","Select",0,0)		;select the 1st item in signal list view
	ControlClick("BUSMASTER","New Desc...",1089)										;click new desc button
	sleep(1000)
	if winexists("Enter Value Descriptor and Value") then
		send("{ENTER}")																	;check the validation for null values
		send("{ENTER}")
		ControlSetText("Enter Value Descriptor and Value","","[CLASS:Edit; INSTANCE:1]","Test1")		;Signal desc name
		ControlSetText("Enter Value Descriptor and Value","","[CLASS:Edit; INSTANCE:2]",10)				;value
		send("{ENTER}")
	EndIf
	sleep(2000)
	ControlListView("BUSMASTER","","[CLASS:SysListView32; INSTANCE:2]","Select",0,0)		;select the 1st item in signal desc list view
	ControlClick("BUSMASTER","Edit Desc...",1090)										; Edit desc
	sleep(2000)
	if winexists("Enter Value Descriptor and Value") then
		ControlSetText("Enter Value Descriptor and Value","","[CLASS:Edit; INSTANCE:1]","Testupdate1")		;Signal desc name
		ControlSetText("Enter Value Descriptor and Value","","[CLASS:Edit; INSTANCE:2]",40)				;value
		send("{ENTER}")
		send("{ENTER}")
		ControlSetText("Enter Value Descriptor and Value","","[CLASS:Edit; INSTANCE:2]",11)
		send("{ENTER}")
	EndIf
;~ 	ControlListView("BUSMASTER","","[CLASS:SysHeader32; INSTANCE:1]","Select",0,0)		;select the 1st item in signal list view
;~ 	ControlClick("BUSMASTER","New Desc...",1089)										;click new desc button
;~ 	sleep(1000)
	;test for duplicate signal desc name
;~ 	if winexists("Enter Value Descriptor and Value") then
;~ 		ControlSetText("Enter Value Descriptor and Value","","[CLASS:Edit; INSTANCE:1]","Testupdate1")		;Signal desc name
;~ 		ControlSetText("Enter Value Descriptor and Value","","[CLASS:Edit; INSTANCE:2]",13)					;value
;~ 		send("{ENTER}")
;~ 		sleep(2000)
;~ 		if WinExists("BUSMASTER") Then
;~ 			ControlClick("BUSMASTER","",2)
;~ 		EndIf
;~ 		sleep(1000)
;~ 		ControlClick("Enter Value Descriptor and Value","",1043)
;~ 	EndIf
	ControlListView("BUSMASTER","","[CLASS:SysListView32; INSTANCE:2]","Select",0,0)
	ControlClick("BUSMASTER","Delete Desc",1091)										; click del desc button.
	if WinExists("BUSMASTER") Then
		ControlClick("BUSMASTER","",7)													; click 'No' button
		send("{ENTER}")																	; Click 'Yes' button
	EndIf
	_associateDB("&File","DBSigDesc.dbf")	;associate dbf
	WinMenuSelectItem("BUSMASTER","","&Configure","&Hardware Interface","&Kvaser CAN") ;select device
	if winexists("Hardware Selection") Then
		ControlClick("Hardware Selection","&Select","[CLASS:Button; INSTANCE:2]")
		sleep(1000)
		ControlClick("Hardware Selection","&Select","[CLASS:Button; INSTANCE:2]")
		ControlClick("Hardware Selection","&OK","[CLASS:Button; INSTANCE:4]")
		sleep(1000)
	EndIf
	sleep(2000)
	WinMenuSelectItem("BUSMASTER","","&Configure","&Waveform Messages")	;select Waveform Messages window
	sleep(1000)
	if winexists("Waveform Selection List") then
		$lvhWd=ControlGetHandle("Waveform Selection List","","[CLASS:SysListView32; INSTANCE:1]") ;get listview handle
		ControlSetText("Waveform Selection List","","[CLASS:Edit; INSTANCE:1]",2)		;enter signal value
		_GUICtrlListView_ClickItem($lvhWd,0)									; select 1st item in listview
		controlclick("Waveform Selection List","","[CLASS:Button; INSTANCE:1]")	;click define
		sleep(1000)
		if winexists("Signal Definer Dialog") then
			ControlCommand("Signal Definer Dialog","","[CLASS:ComboBox; INSTANCE:2]","ShowDropDown")
			sleep(500)
			ControlCommand("Signal Definer Dialog","","[CLASS:ComboBox; INSTANCE:2]","HideDropDown")
			controlsettext("Signal Definer Dialog","",1001,20)				;enter amplitude
			controlsettext("Signal Definer Dialog","",1004,150)			;enter sampling time
			controlsettext("Signal Definer Dialog","",1002,150)			;;enter frequency
			controlclick("Signal Definer Dialog","","[CLASS:Button; INSTANCE:1]")
		EndIf
		_GUICtrlListView_ClickItem($lvhWd,0)
		controlclick("Waveform Selection List","","[CLASS:Button; INSTANCE:1]")
		sleep(1000)
		if winexists("Signal Definer Dialog") then
			sleep(500)
			ControlCommand("Signal Definer Dialog","",1000,"ShowDropDown")
			sleep(500)
			ControlCommand("Signal Definer Dialog","",1000,"HideDropDown")
			sleep(500)
			ControlCommand("Signal Definer Dialog","",1000,"SelectString","Cos Wave") ;select cos wave
			sleep(500)
			controlsettext("Signal Definer Dialog","",1001,20)	; Amplitude
			controlsettext("Signal Definer Dialog","",1004,150)	; Sampling time period
			controlsettext("Signal Definer Dialog","",1002,150)	; Frequency
			controlclick("Signal Definer Dialog","","[CLASS:Button; INSTANCE:1]")
		EndIf
		sleep(2000)
		ControlCommand("Waveform Selection List","","[CLASS:ComboBox; INSTANCE:1]","SetCurrentSelection",1)	;select 2nd message
		ControlSetText("Waveform Selection List","","[CLASS:Edit; INSTANCE:1]",2)
		_GUICtrlListView_ClickItem($lvhWd,0)									;select 1st item in listview
		controlclick("Waveform Selection List","","[CLASS:Button; INSTANCE:1]")
		sleep(1000)
		if winexists("Signal Definer Dialog") then
			ControlCommand("Signal Definer Dialog","",1003,"ShowDropDown")	; Cycles combobox
			sleep(500)
			ControlCommand("Signal Definer Dialog","",1003,"HideDropDown")
			sleep(500)
			ControlCommand("Signal Definer Dialog","",1003,"SelectString","2") ;select cycles as 2
			sleep(500)
			controlsettext("Signal Definer Dialog","",1001,20)
			controlsettext("Signal Definer Dialog","",1004,150)
			controlsettext("Signal Definer Dialog","",1002,150)
			controlclick("Signal Definer Dialog","","[CLASS:Button; INSTANCE:1]")
		EndIf
		_GUICtrlListView_ClickItem($lvhWd,0)
		controlclick("Waveform Selection List","","[CLASS:Button; INSTANCE:1]")
		sleep(1000)
		if winexists("Signal Definer Dialog") then
			sleep(500)
			ControlCommand("Signal Definer Dialog","",1000,"ShowDropDown")
			sleep(500)
			ControlCommand("Signal Definer Dialog","",1000,"HideDropDown")
			sleep(500)
			ControlCommand("Signal Definer Dialog","",1000,"SelectString","Triangular Wave") ;select Triangular Wave 			
			sleep(500)
			controlsettext("Signal Definer Dialog","",1001,20)
			controlsettext("Signal Definer Dialog","",1004,150)
			controlsettext("Signal Definer Dialog","",1002,150)
			controlclick("Signal Definer Dialog","","[CLASS:Button; INSTANCE:1]")
		EndIf
		controlclick("Waveform Selection List","","[CLASS:Button; INSTANCE:2]")
	EndIf
	sleep(1000)
	$cntToolhWd=ControlGetHandle("BUSMASTER","",128)
	_GUICtrlToolbar_ClickIndex($cntToolhWd,4)
	WinMenuSelectItem("BUSMASTER","","F&unctions","&Transmit","&Waveforms")
	sleep(2000)
send("{ESC}")
send("!fdc")
if WinExists("BUSMASTER") Then
	ControlClick("BUSMASTER","",7)
EndIf
$msgWinPos=ControlGetPos("BUSMASTER","","[CLASS:SysListView32; INSTANCE:1]")
MouseClick("left",$msgWinPos[0]+30,$msgWinPos[1]+95,2)					; Edit desc
WinClose("Message Interpretation")
EndIf