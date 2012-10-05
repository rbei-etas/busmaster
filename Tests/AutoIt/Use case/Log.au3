; Test Case Number:	TS_Log_01
; Feature:			Log File
; Critical (C):		Y
; Test Cases:		-
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
#include<globalFunc.au3>
#Include <GuiToolBar.au3>

WinActivate("BUSMASTER",3)
if WinExists("BUSMASTER") then
	_loadConfig("configLog.cfx")
	_loadDatabase("dblog")
	sleep(2000)
	send("!FDC")
	_associateDB("&File","dblog.dbf")
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
	; check if 'Autoupdate' check box is enabled
	if (ControlCommand("BUSMASTER","",17000,"IsChecked")=0) Then
		ControlCommand("BUSMASTER","",17000,"Check")
	EndIf
	_txMSG("Cyclic",$Count)
	ControlClick("BUSMASTER","",1015,"left")					; Click on close button
	WinMenuSelectItem("BUSMASTER","","&Configure","L&og")
	if winexists("Configure Logging for CAN") Then
		ControlClick("Configure Logging for CAN","&Add",1303)
		sleep(1000)
		ControlCommand("Configure Logging for CAN","",1092,"Check")			; Check Enable/Disable Logging during tool Connect/Disconnect
		sleep(1000)
		ControlClick("Configure Logging for CAN","&Add",1303)
		sleep(1000)
		ControlCommand("Configure Logging for CAN","",1101,"SelectString","Relative")
		sleep(1000)
		ControlCommand("Configure Logging for CAN","",1081,"Check")				; select decimal mode
		sleep(2000)
		ControlCommand("Configure Logging for CAN","",1351,"Check")				; select overwrite
		sleep(2000)
		ControlClick("Configure Logging for CAN","&Add",1303)
		sleep(1000)
		ControlCommand("Configure Logging for CAN","",1101,"SelectString","Absolute")
		sleep(1000)
		ControlCommand("Configure Logging for CAN","",1116,"Check")
		sleep(1000)
		ControlSetText("Configure Logging for CAN","",1095,32)
		ControlCommand("Configure Logging for CAN","",1117,"Check")
		sleep(500)
		ControlSetText("Configure Logging for CAN","",1096,37)
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
	$cntToolhWd=ControlGetHandle("BUSMASTER","",128)												; Get handle of tollbar
	_GUICtrlToolbar_ClickIndex($cntToolhWd,4)														; Click on 'Connect' icon
	sleep(500)
	WinMenuSelectItem("BUSMASTER","","F&unctions","&Transmit","&Normal Blocks")
	sleep(10000)
	_GUICtrlToolbar_ClickIndex($cntToolhWd,4)
EndIf