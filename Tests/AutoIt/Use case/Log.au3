; Test Case Number:	TS_Log_01
; Feature:			Log File
; Critical (C):		Y
; Test Cases:		-
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
#include<globalFunc.au3>
if WinExists("BUSMASTER") then
	_loadConfig("UseCase1.cfx")
	_loadDatabase("AutoitTest")
	_associateDB("&File","AutoitTest.dbf")
	sleep(1000)
	WinMenuSelectItem("BUSMASTER","","&Configure","&Hardware Interface","&Kvaser CAN")
	sleep(1000)
	if winexists("Hardware Selection") Then
		ControlClick("Hardware Selection","&Select","[CLASS:Button; INSTANCE:2]")
		sleep(1000)
		ControlClick("Hardware Selection","&Select","[CLASS:Button; INSTANCE:2]")
		sleep(1000)
		ControlListView("Hardware Selection","","SysListView322","GetItemCount")
		ControlClick("Hardware Selection","&OK","[CLASS:Button; INSTANCE:4]")
		sleep(1000)
	EndIf
	sleep(1000)
	$Count=2
	WinMenuSelectItem("BUSMASTER","","&Configure","&Tx Messages")
	sleep(1000)
	_txMSG("Cyclic",$Count)
	ControlClick("BUSMASTER","","[CLASS:Button; INSTANCE:23]","left")
	sleep(1000)
	if winexists("BUSMASTER","Do you want to save changes?") Then
		ControlClick("BUSMASTER","","[CLASS:Button; INSTANCE:2]","left")
	EndIf
	WinMenuSelectItem("BUSMASTER","","&Configure","L&og")
	if winexists("Configure Logging for CAN") Then
		ControlClick("Configure Logging for CAN","&Add","")
		sleep(1000)
		ControlCommand("Configure Logging for CAN","",1092,"Check")
		sleep(1000)
		ControlCommand("Configure Logging for CAN","",1101,"SelectString","Relative")
		sleep(1000)
		ControlCommand("Configure Logging for CAN","",1101,"SelectString","Absolute")
		sleep(1000)
		ControlCommand("Configure Logging for CAN","",1081,"Check")
		sleep(1000)
		ControlCommand("Configure Logging for CAN","",1082,"Check")
		sleep(1000)
		ControlCommand("Configure Logging for CAN","",1351,"Check")
		sleep(1000)
		ControlCommand("Configure Logging for CAN","",1350,"Check")
		sleep(1000)
		controlclick("Configure Logging for CAN","",1121)
		sleep(1000)
		if winexists("Filter Selection Dialog") Then
			controlclick("Filter Selection Dialog","","[CLASS:Button; INSTANCE:2]")
			sleep(500)
		EndIf
		controlclick("Configure Logging for CAN","",1121)
		sleep(1000)
		if winexists("Filter Selection Dialog") Then
			controlclick("Filter Selection Dialog","","[CLASS:Button; INSTANCE:3]")
			sleep(500)
		EndIf
		ControlClick("Configure Logging for CAN","&OK","[CLASS:Button; INSTANCE:14]")
	EndIf
	sleep(500)
	WinMenuSelectItem("BUSMASTER","","F&unctions","&Connect")
	sleep(500)
	WinMenuSelectItem("BUSMASTER","","F&unctions","&Transmit","&Normal Blocks")
	sleep(300000)
	WinMenuSelectItem("BUSMASTER","","F&unctions","D&isconnect	ESC")
EndIf