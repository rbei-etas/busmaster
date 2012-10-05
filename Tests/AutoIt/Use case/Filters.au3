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

WinActivate("BUSMASTER",3)
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
	;-- Stop Filter
	if WinExists("Configure Filter List") Then
		for $i=1 to 19
			ControlClick("Configure Filter List","",1228)								; Click on Add button
		Next
		$hWnd = ControlGetHandle("Configure Filter List","","[CLASS:SysListView32; INSTANCE:1]")
		$sigDetPos=_GUICtrlListView_GetItemPosition($hWnd, 0)							; Get position of Signal details list view control
		_GUICtrlListView_Scroll($hWnd, 25, 60)											; Scroll the List view
		sleep(2000)
		for $i=1 to 19
			ControlClick("Configure Filter List","",1002)								; Click on Delete button
		next
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
		ControlClick("Configure Filter List","",1256)										;Click on 'OK' button
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

	;--pass filter

	WinMenuSelectItem("BUSMASTER","","&Configure","&App Filters")						; Select Configure->App Filters
	sleep(1000)
	if WinExists("Configure Filter List") Then
		; Click on delete button
		ControlClick("Configure Filter List","",1002)
		sleep(500)

		; Click on add button
		ControlClick("Configure Filter List","",1228)
		sleep(1000)
		$hWnd = ControlGetHandle("Configure Filter List","","[CLASS:SysListView32; INSTANCE:1]")
		$sigDetPos=_GUICtrlListView_GetItemPosition($hWnd, 0)										; Get position of Signal details list view control
		ControlClick("Configure Filter List","",1000,"Left",2,$sigDetPos[0]+100,$sigDetPos[1])		; Double click on the 'Filter Type'
		sleep(500)
		send("{DOWN}")
		sleep(500)
		send("{ENTER}")																		; Select "Pass" filter
		ControlCommand("Configure Filter List","",1004,"SetCurrentSelection",0)				;Msg ID
		ControlCommand("Configure Filter List","",1010,"SetCurrentSelection","All")				;Msg Frame Type
		ControlCommand("Configure Filter List","",1011,"SetCurrentSelection","All")				;Direction
		ControlClick("Configure Filter List","",3)											;Add to list
		_GUICtrlToolbar_ClickIndex($cntToolhWd,4)


		ControlCommand("Configure Filter List","",1254,"Check")								;Check range from radio button
		ControlSetText("Configure Filter List","",1041,51)
		ControlSetText("Configure Filter List","",1042,55)
		ControlCommand("Configure Filter List","",1009,"SetCurrentSelection",2)				;Msg ID type
		ControlCommand("Configure Filter List","",1010,"SetCurrentSelection",0)				;Msg Frame Type
		ControlCommand("Configure Filter List","",1011,"SetCurrentSelection",0)				;Direction
		ControlCommand("Configure Filter List","",1012,"SetCurrentSelection",1)				;Channel Combo
		ControlClick("Configure Filter List","",3)											;Add to list
		ControlCommand("Configure Filter List","",1253,"Check")								;Check ID radio button
		ControlCommand("Configure Filter List","",1004,"SetCurrentSelection",0)				;Msg ID
		ControlCommand("Configure Filter List","",1010,"SetCurrentSelection","Non RTR")				;Msg Frame Type
		ControlCommand("Configure Filter List","",1011,"SelectString","Rx")					;Direction
		ControlCommand("Configure Filter List","",1012,"SetCurrentSelection","2")				;Channel Combo
		ControlCommand("Configure Filter List","",1012,"SetCurrentSelection","2")				;Channel Combo
		ControlClick("Configure Filter List","",3)											;Add to list
		ControlCommand("Configure Filter List","",1004,"SetCurrentSelection",1)				;Msg ID
		ControlCommand("Configure Filter List","",1010,"SetCurrentSelection","All")				;Msg Frame Type
		ControlCommand("Configure Filter List","",1011,"SetCurrentSelection",2)				;Direction
		ControlCommand("Configure Filter List","",1012,"SetCurrentSelection","All")				;Channel Combo
		ControlClick("Configure Filter List","",3)											;Add to list
		ControlClick("Configure Filter List","",1256)										;Click on 'OK' button
	EndIf

	WinMenuSelectItem("BUSMASTER","","&Configure","&Message Display")						;select Configure->msg display
	ControlClick("Configure Message Display- CAN","","[CLASS:Button; INSTANCE:2]")
	if winexists("Filter Selection Dialog") Then
		$fltSelhWd=controlgethandle("Filter Selection Dialog","","[CLASS:SysListView32; INSTANCE:1]")
		_GUICtrlListView_ClickItem($fltSelhWd,0)
		ControlClick("Filter Selection Dialog","&Add","[CLASS:Button; INSTANCE:1]")
		sleep(1000)
		ControlClick("Filter Selection Dialog","",1)
		sleep(1000)
	EndIf
	ControlClick("Configure Message Display- CAN","",1)


	WinMenuSelectItem("BUSMASTER","","&Configure","L&og")									; Select 'Log' menu
	sleep(1000)
	ControlClick("Configure Logging for CAN","&Add",1303)
	sleep(1000)
	ControlCommand("Configure Logging for CAN","",1092,"Check")								; Check Enable/Disable Logging during tool Connect/Disconnect
	sleep(1000)
	controlclick("Configure Logging for CAN","",1121)										; Click on "Filter" button
	sleep(1000)
	if winexists("Filter Selection Dialog") Then
		controlclick("Filter Selection Dialog","","[CLASS:Button; INSTANCE:2]")
		sleep(500)
	EndIf
	controlclick("Configure Logging for CAN","",1121)
	sleep(1000)
	if winexists("Filter Selection Dialog") Then
		controlclick("Filter Selection Dialog","",32946)									; Click on 'Add All' button
		sleep(500)
		controlclick("Filter Selection Dialog","",1)										; Click on 'OK' button
	EndIf
	ControlClick("Configure Logging for CAN","&OK",1)

	; Replay

	WinMenuSelectItem("BUSMASTER","","&Configure","&Replay")   ; select Configure-->Replay
	if winexists("Replay Configuration") Then
		ControlClick("Replay Configuration","",1262)			; Click Add button
		sleep(1000)
		if WinExists("Select Replay File Name") Then
			$DataPath = _SetOneFolderUp()
			send($DataPath&"\CanLog.log")						; Enter file name
			send("{ENTER}")										; click enter
		EndIf
		ControlClick("Replay Configuration","",1002)			; Click Filter button
		if winexists("Filter Selection Dialog") Then
			controlclick("Filter Selection Dialog","",32946)									; Click on 'Add All' button
			sleep(500)
			controlclick("Filter Selection Dialog","",1)										; Click on 'OK' button
		EndIf
		sleep(1000)
		ControlClick("Replay Configuration","",1)												; Click on 'OK' button
	EndIf

	$cntToolhWd=ControlGetHandle("BUSMASTER","",128)
	_GUICtrlToolbar_ClickIndex($cntToolhWd,4)
	WinMenuSelectItem("BUSMASTER","","F&unctions","&Transmit","&Normal Blocks")
	sleep(5000)
	WinMenuSelectItem("BUSMASTER","","F&unctions","Enable &Filter","&Display")
	sleep(3000)
	_GUICtrlToolbar_ClickIndex($cntToolhWd,4)
	sleep(1000)
	WinMenuSelectItem("BUSMASTER","","&Configure","&App Filters")						;select Configure->App Filters
	sleep(1000)
	$fltDethWd=controlgethandle("Configure Filter List","","[CLASS:SysListView32; INSTANCE:2]")  ; Get handle of list details control
	_GUICtrlListView_ClickItem($fltDethWd,0,"","",2)											; click on first item
	ControlCommand("Configure Filter List","",1254,"Check")								;Check range from radio button
	ControlSetText("Configure Filter List","",1041,51)
	ControlSetText("Configure Filter List","",1042,55)
	ControlCommand("Configure Filter List","",1009,"SetCurrentSelection","Exd")				;Msg ID type
	ControlCommand("Configure Filter List","",1010,"SetCurrentSelection","RTR")				;Msg Frame Type
	ControlCommand("Configure Filter List","",1011,"SetCurrentSelection","Tx")				;Direction
	ControlCommand("Configure Filter List","",1012,"SetCurrentSelection",1)				;Channel Combo
	ControlClick("Configure Filter List","",1256)										;Click on 'OK' button
	sleep(1000)
	$cntToolhWd=ControlGetHandle("BUSMASTER","",128)
	_GUICtrlToolbar_ClickIndex($cntToolhWd,4)
	WinMenuSelectItem("BUSMASTER","","F&unctions","&Transmit","&Normal Blocks")
	sleep(5000)
	WinMenuSelectItem("BUSMASTER","","F&unctions","Enable &Filter","&Display")
	sleep(3000)
	_GUICtrlToolbar_ClickIndex($cntToolhWd,4)
	sleep(1000)
	WinMenuSelectItem("BUSMASTER","","&Configure","&App Filters")						;select Configure->App Filters
	sleep(1000)
	$fltDethWd=controlgethandle("Configure Filter List","","[CLASS:SysListView32; INSTANCE:2]")  ; Get handle of list details control
	_GUICtrlListView_ClickItem($fltDethWd,1,"","",2)											; click on first item
	ControlCommand("Configure Filter List","",1253,"Check")								;Check range from radio button
	ControlCommand("Configure Filter List","",1009,"SetCurrentSelection","Exd")				;Msg ID type
	ControlCommand("Configure Filter List","",1010,"SetCurrentSelection","RTR")				;Msg Frame Type
	ControlCommand("Configure Filter List","",1011,"SetCurrentSelection","Tx")				;Direction
	ControlCommand("Configure Filter List","",1012,"SetCurrentSelection",1)				;Channel Combo
	ControlClick("Configure Filter List","",1256)										;Click on 'OK' button
	sleep(1000)
	$cntToolhWd=ControlGetHandle("BUSMASTER","",128)
	_GUICtrlToolbar_ClickIndex($cntToolhWd,4)
	WinMenuSelectItem("BUSMASTER","","F&unctions","&Transmit","&Normal Blocks")
	sleep(5000)
	WinMenuSelectItem("BUSMASTER","","F&unctions","Enable &Filter","&Display")
	sleep(3000)
	_GUICtrlToolbar_ClickIndex($cntToolhWd,4)
	sleep(1000)
	send("!FGS")								; Save configuration
EndIf

