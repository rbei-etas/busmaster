;=== Test Cases/Test Data ===
; Critical (C):		Y
; TestCase No.:		-
; Test Cases:		Filters
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
#Include <GuiListView.au3>
#Include <globalFunc.au3>
#Include <GuiTab.au3>
#Include <GuiToolBar.au3>


if WinExists("BUSMASTER") Then
	if winexists("Hardware Selection") Then
		ControlClick("Hardware Selection","&Select","[CLASS:Button; INSTANCE:2]")
		sleep(1000)
		ControlClick("Hardware Selection","&Select","[CLASS:Button; INSTANCE:2]")
		sleep(1000)
		ControlClick("Hardware Selection","&OK","[CLASS:Button; INSTANCE:4]")
		sleep(1000)
	EndIf
	_loadConfig("12FLTTest")					;load cfx file
	_loadDatabase("12AutoitTest")				;load dbf file
	send("!fdc")
	_associateDB("&File","12AutoitTest.dbf")	;associate dbf
	WinMenuSelectItem("BUSMASTER","","&Configure","&Hardware Interface","&Simulation")
	sleep(1000)
	WinMenuSelectItem("BUSMASTER","","&Configure","&Hardware Interface","&Kvaser CAN") ;select device
	sleep(2000)
	if winexists("Hardware Selection") Then
		sleep(1000)
		ControlClick("Hardware Selection","&Select","[CLASS:Button; INSTANCE:2]")
		sleep(1000)
		ControlClick("Hardware Selection","&Select","[CLASS:Button; INSTANCE:2]")
		sleep(1000)
		ControlClick("Hardware Selection","&OK","[CLASS:Button; INSTANCE:4]")
		sleep(1000)
	EndIf
	sleep(2000)
	WinMenuSelectItem("BUSMASTER","","&Configure","&App Filters")						;select Configure->App Filters
	sleep(1000)
	if WinExists("Configure Filter List") Then
		ControlClick("Configure Filter List","",1228)
		ControlClick("Configure Filter List","",1002)
		ControlClick("Configure Filter List","",1228)
		sleep(500)
		$flthWd=controlgethandle("Configure Filter List","","[CLASS:SysListView32; INSTANCE:1]")
		_GUICtrlListView_ClickItem($flthWd,0,"","",2)										;Click on the Filter
		send("Test")																		;change the filter name
		ControlCommand("Configure Filter List","",1004,"SetCurrentSelection",0)				;Msg ID
		ControlCommand("Configure Filter List","",1010,"SetCurrentSelection",2)				;Msg Frame Type
		ControlCommand("Configure Filter List","",1011,"SetCurrentSelection",2)				;Direction
		ControlClick("Configure Filter List","",3)											;Add to list
		;ControlCommand("Configure Filter List","",1004,"SetCurrentSelection",1)				;Msg ID
		ControlCommand("Configure Filter List","",1254,"Check")								;Check range from radio button
		ControlSetText("Configure Filter List","",1041,51)
		ControlSetText("Configure Filter List","",1042,55)
		ControlCommand("Configure Filter List","",1009,"SetCurrentSelection",2)				;Msg ID type
		ControlCommand("Configure Filter List","",1010,"SetCurrentSelection",1)				;Msg Frame Type
		ControlCommand("Configure Filter List","",1011,"SetCurrentSelection",1)				;Direction
		ControlCommand("Configure Filter List","",1012,"SetCurrentSelection",1)				;Channel Combo
		ControlClick("Configure Filter List","",3)											;Add to list
		$fltDethWd=controlgethandle("Configure Filter List","","[CLASS:SysListView32; INSTANCE:2]")  ; Get handle of list details control
		_GUICtrlListView_ClickItem($fltDethWd,0,"","",2)											; click on first item
		ControlClick("Configure Filter List","",1250)											; delete an item
		sleep(1000)
		ControlClick("Configure Filter List","",1251)											; delete all item
		sleep(1000)
		if winexists("BUSMASTER") Then
			sleep(1000)
			ConsoleWrite("delete all")
			ControlClick("BUSMASTER","&Yes","[CLASS:Button; INSTANCE:1]")
		EndIf
		ControlCommand("Configure Filter List","",1253,"Check")								;Check ID radio button
		ControlCommand("Configure Filter List","",1004,"SetCurrentSelection",0)				;Msg ID
		ControlCommand("Configure Filter List","",1010,"SetCurrentSelection","Non RTR")				;Msg Frame Type
		ControlCommand("Configure Filter List","",1011,"SelectString","Rx")					;Direction
		ControlCommand("Configure Filter List","",1012,"SetCurrentSelection","2")				;Channel Combo
		ControlClick("Configure Filter List","",3)											;Add to list
		ControlCommand("Configure Filter List","",1004,"SetCurrentSelection",1)				;Msg ID
		ControlCommand("Configure Filter List","",1010,"SetCurrentSelection","All")				;Msg Frame Type
		ControlCommand("Configure Filter List","",1011,"SetCurrentSelection",2)				;Direction
		ControlCommand("Configure Filter List","",1012,"SetCurrentSelection","All")				;Channel Combo
		ControlClick("Configure Filter List","",3)											;Add to list
		ControlClick("Configure Filter List","",1256)
	EndIf
	WinMenuSelectItem("BUSMASTER","","&Configure","&Tx Messages")							; configure Tx messages
	sleep(1000)
	if winexists("BUSMASTER") Then
		ControlClick("BUSMASTER","Add","[CLASS:Button; INSTANCE:14]")						; Add a block
;~ 		sleep(1000)
;~ 		ControlClick("BUSMASTER","Auto Update","[CLASS:Button; INSTANCE:26]")				; select auto update
;~ 		sleep(1000)
		ControlCommand("BUSMASTER","",1221,"SetCurrentSelection",0)							; select 1st msg
		ControlClick("BUSMASTER","Add &Message","[CLASS:Button; INSTANCE:4]")				; Click add message
		ControlClick("BUSMASTER","Add","[CLASS:Button; INSTANCE:14]")						; Add a block
		ControlCommand("BUSMASTER","",1221,"SetCurrentSelection",1)							; select 2st msg
		ControlClick("BUSMASTER","Add &Message","[CLASS:Button; INSTANCE:4]")				; Click add message
		ControlClick("BUSMASTER","Update","[CLASS:Button; INSTANCE:24]")							;click update
		ControlClick("BUSMASTER","Close","[CLASS:Button; INSTANCE:25]")				; Click add message
	EndIf
	sleep(2000)
	WinMenuSelectItem("BUSMASTER","","&Configure","&Message Display")						;select Configure->msg display
	ControlClick("Configure Message Display- CAN","","[CLASS:Button; INSTANCE:2]")
	if winexists("Filter Selection Dialog") Then
		$fltSelhWd=controlgethandle("Filter Selection Dialog","","[CLASS:SysListView32; INSTANCE:1]")
		_GUICtrlListView_ClickItem($fltSelhWd,0)
		ControlClick("Filter Selection Dialog","&Add","[CLASS:Button; INSTANCE:1]")
		ControlClick("Filter Selection Dialog","D&elete All","[CLASS:Button; INSTANCE:6]")
		ControlClick("Filter Selection Dialog","Add Al&l","[CLASS:Button; INSTANCE:4]")
		ControlClick("Filter Selection Dialog","&OK","[CLASS:Button; INSTANCE:2]")
		$cfgMsgDishWd=controlgethandle("Configure Message Display- CAN","","[CLASS:SysTabControl32; INSTANCE:1]")
		_GUICtrlTab_ClickTab($cfgMsgDishWd,1)
		ControlClick("Configure Message Display- CAN","","[CLASS:Button; INSTANCE:2]")
		send("{ENTER}")
		_GUICtrlTab_ClickTab($cfgMsgDishWd,2)
		ControlClick("Configure Message Display- CAN","","[CLASS:Button; INSTANCE:1]")
		if winexists("Add Message Display Attributes") Then
			ControlSetText("Add Message Display Attributes","","[CLASS:Edit; INSTANCE:1]","junkmsg")
			ControlSetText("Add Message Display Attributes","","[CLASS:Edit; INSTANCE:2]",30)
			send("{ENTER}")
		EndIf
		ControlClick("Configure Message Display- CAN","","[CLASS:Button; INSTANCE:2]")
		if winexists("Edit Message Display Attributes") Then
			ControlSetText("Add Message Display Attributes","","[CLASS:Edit; INSTANCE:1]","junkmsgedit")
			send("{ENTER}")
		EndIf
		_GUICtrlTab_ClickTab($cfgMsgDishWd,3)
		ControlSetText("Configure Message Display- CAN","","[CLASS:Edit; INSTANCE:1]",6000)
		ControlSetText("Configure Message Display- CAN","","[CLASS:Edit; INSTANCE:2]",1000)
		ControlSetText("Configure Message Display- CAN","","[CLASS:Edit; INSTANCE:3]",200)
		ControlClick("Configure Message Display- CAN","Set Default","[CLASS:Button; INSTANCE:2]")
		ControlClick("Configure Message Display- CAN","OK","[CLASS:Button; INSTANCE:4]")
	EndIf
	sleep(2000)
	$cntToolhWd=ControlGetHandle("BUSMASTER","",128)
	_GUICtrlToolbar_ClickIndex($cntToolhWd,4)
	WinMenuSelectItem("BUSMASTER","","F&unctions","&Transmit","&Normal Blocks")
	sleep(5000)
	WinMenuSelectItem("BUSMASTER","","F&unctions","Enable &Filter","&Display")
	sleep(3000)
	_GUICtrlToolbar_ClickIndex($cntToolhWd,4)
EndIf

