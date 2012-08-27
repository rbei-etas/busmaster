;=== Test Cases/Test Data ===
; Critical (C):		Y
; TestCase No.:		TC_03
; Test Cases:		Node Simultion - ADD
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
#Include <GuiTreeView.au3>
#Include <GuiListView.au3>
#include <globalFunc.au3>
#Include <GuiToolBar.au3>
#Include <GuiListBox.au3>

winactivate("BUSMASTER",3)
if WinExists("BUSMASTER") Then
	sleep(2000)
	WinMenuSelectItem("BUSMASTER","","&Configure","&Hardware Interface","&Simulation")
	_loadConfig("SiSysTest")																	;load cfx file
	_loadDatabase("SimCAN")																		;load dbf file
	send("!fdc")
	_associateDB("&File","SimCAN.dbf")															;associate dbf
	sleep(2000)
	$Count=2
	WinMenuSelectItem("BUSMASTER","","&Configure","&Tx Messages")								; configure Tx messages
	sleep(1000)
	if (ControlCommand("BUSMASTER","",17000,"IsChecked")=0) Then
		ControlCommand("BUSMASTER","",17000,"Check")											; check 'Autoupdate' check box
	EndIf
	_txMSG("Cyclic",$Count)
	ControlClick("BUSMASTER","",1015)															; Close Tx Window
	sleep(2000)
	WinMenuSelectItem("BUSMASTER","","&Configure","&Simulated Systems")   						;select Configure->Simulated Systems
	$pos=ControlGetPos("BUSMASTER","","[CLASS:SysTreeView32; INSTANCE:1]")
	MouseClick("right",$pos[0]+30,$pos[1]+55)							 						; select new sim
	send("n")
	send(@DesktopDir&"\testCANSim.sim")
	send("{ENTER}")
	sleep(1000)
	if winexists("New Simulated system Configuration Filename...") Then
		controlclick("New Simulated system Configuration Filename...","","[CLASS:Button; INSTANCE:1]")
	EndIf
	MouseClick("right",$pos[0]+50,$pos[1]+65)
	send("a")											 										; select add node
	sleep(1000)
	if WinExists("Node Details") Then
		controlsettext("Node Details","","[CLASS:Edit; INSTANCE:1]","TestNode")	 				; enter node name
		send("{ENTER}")
	EndIf
	sleep(1000)
	if WinExists("BUSMASTER") Then
		controlclick("BUSMASTER","","[CLASS:Button; INSTANCE:3]")								; Click Add File
		sleep(1000)
		if WinExists("Select BUSMASTER Source Filename...") Then
			sleep(1000)
			send(@DesktopDir&"\addCANHandlers")
			send("{ENTER}")
		EndIf
;
		consolewrite("click item"&@CRLF)
		sleep(2000)
		$nodeHWD=ControlGetHandle("BUSMASTER","","[CLASS:SysTreeView32; INSTANCE:1]")	; Get tree view control handle
		$item1HWD=_GUICtrlTreeView_GetNext($nodeHWD,"")									; Get handle of first item
		$incHWD=_GUICtrlTreeView_GetFirstChild($nodeHWD,$item1HWD)
		_GUICtrlTreeView_ClickItem($nodeHWD,$incHWD,"Right")							; Right click on First Header
		sleep(500)
		send("d")																		; Delete
		sleep(1000)
		if WinExists("BUSMASTER") Then
			send("{ENTER}")																; Click  'Yes'
		EndIf
		$item1HWD=_GUICtrlTreeView_GetNext($nodeHWD,"")									; Get handle of first item
		_GUICtrlTreeView_ClickItem($nodeHWD,$item1HWD,"Right")							; Click on Include Handler
		send("a")																		; Add a new handler
		sleep(1000)
		if WinExists("Enter Header File") Then
			ControlSetText("Enter Header File","",1049,Chr(34)&"Windows.h"&Chr(34))		; Enter Header File
			controlclick("Enter Header File","",1051)									; Click Browse Button
			sleep(1000)
			if WinExists("Select Header File") Then
				controlclick("Select Header File","",2)									; Click Cancel button
			EndIf
			controlclick("Enter Header File","",1042)									; Click 'OK'
		EndIf
		send("^s")
		$item2HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item1HWD)					; Get handle of second item
		_GUICtrlTreeView_ClickItem($nodeHWD,$item2HWD,"Right")							; Click on Message Handler
		send("a")																		; Add a new msg
		sleep(1000)
		if winexists("Add Message Handler") then										; enter the msg contents
			controlsettext("Add Message Handler","",1155,51)							; Enter Msg ID
			controlclick("Add Message Handler","",1167)									; Click OK
		EndIf
		$item1HWD=_GUICtrlTreeView_GetNext($nodeHWD,"")									; Get handle of first item
		$item2HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item1HWD)					; Get handle of second item
		_GUICtrlTreeView_ClickItem($nodeHWD,$item2HWD,"Right")							; Click on Message Handler
		send("a")																		; Add a new msg
		sleep(1000)
		if winexists("Add Message Handler") then										; enter the msg contents
			controlcommand("Add Message Handler","",1172,"Check")						; Check Msg ID range radio button
			ControlSetText("Add Message Handler","",1160,51)							; Enter Msg ID range 'From'
			ControlSetText("Add Message Handler","",1161,55)							; Enter Msg ID range 'To'
			controlclick("Add Message Handler","",1167)									; Click 'OK'
		EndIf
		$item1HWD=_GUICtrlTreeView_GetNext($nodeHWD,"")									; Get handle of first item
		$item2HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item1HWD)					; Get handle of second item
		_GUICtrlTreeView_ClickItem($nodeHWD,$item2HWD,"Right")							; Click on Message Handler
		send("a")																		; Add a new msg
		sleep(1000)
		if winexists("Add Message Handler") then										; enter the msg contents
			controlcommand("Add Message Handler","",1173,"Check")						; Check 'All Rx Msgs' radio button
			controlclick("Add Message Handler","",1167)									; Click 'OK'
		EndIf
		$item1HWD=_GUICtrlTreeView_GetNext($nodeHWD,"")									; Get handle of first item
		$item2HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item1HWD)					; Get handle of second item
		_GUICtrlTreeView_ClickItem($nodeHWD,$item2HWD,"Right")							; Click on Message Handler
		send("a")																		; Add a new msg
		sleep(1000)
		if winexists("Add Message Handler") then										; enter the msg contents
			$dbMsgListHWD=controlgethandle("Add Message Handler","",1169)				; fetch DB Message List Handler
			controlcommand("Add Message Handler","",1170,"Check")						; Check 'All Rx Msgs' radio button
			_GUICtrlListBox_ClickItem($dbMsgListHWD,0)									; Click on First item
			sleep(500)
			controlclick("Add Message Handler","",1167)									; Click 'OK'
		EndIf


		; Add Time Handler
		$nodeHWD=ControlGetHandle("BUSMASTER","","[CLASS:SysTreeView32; INSTANCE:1]")
		$item1HWD=_GUICtrlTreeView_GetNext($nodeHWD,"")
		$item2HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item1HWD)					; Get handle of second item
		_GUICtrlTreeView_ClickItem($nodeHWD,$item2HWD)
		$item3HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item2HWD)					; Get handle of third item
		_GUICtrlTreeView_ClickItem($nodeHWD,$item3HWD,"Right")
		send("a")																		; Add a timer
		sleep(1000)
		if winexists("Define New Timer Handler") Then
			controlsettext("Define New Timer Handler","",1044,"TimeHandlerName")		; Add time handler name
			controlclick("Define New Timer Handler","",1)								; Click 'OK'
		EndIf
		$nodeHWD=ControlGetHandle("BUSMASTER","","[CLASS:SysTreeView32; INSTANCE:1]")
		$item1HWD=_GUICtrlTreeView_GetNext($nodeHWD,"")
		$item2HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item1HWD)					; Get handle of second item
		_GUICtrlTreeView_ClickItem($nodeHWD,$item2HWD)
		$item3HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item2HWD)					; Get handle of third item
		_GUICtrlTreeView_ClickItem($nodeHWD,$item3HWD)
		$timeHWD=_GUICtrlTreeView_GetFirstChild($nodeHWD,$item3HWD)
		_GUICtrlTreeView_ClickItem($nodeHWD,$timeHWD,"Left","",2)
		sleep(1000)																		; Function description
		ControlSend("BUSMASTER","",59664,"STCAN_MSG sMsg;"&@CRLF)
		ControlSend("BUSMASTER","",59664,"sMsg.m_unMsgID = 0x55;              // Message ID"&@CRLF)
		ControlSend("BUSMASTER","",59664,"sMsg.m_ucEXTENDED = FALSE;           // Standard Message type"&@CRLF)
		ControlSend("BUSMASTER","",59664,"sMsg.m_ucRTR = FALSE;                // Not RTR type"&@CRLF)
		ControlSend("BUSMASTER","",59664,"sMsg.m_ucDLC = 8;                    // Length is 8 Bytes"&@CRLF)
		ControlSend("BUSMASTER","",59664,"sMsg.m_sWhichBit.m_aulData[0] = 10;  // Lower 4 Bytes"&@CRLF)
		ControlSend("BUSMASTER","",59664,"sMsg.m_sWhichBit.m_aulData[1] = 20;  // Upper 4 Bytes"&@CRLF)
		ControlSend("BUSMASTER","",59664,"sMsg.m_ucChannel = 1;                // First CAN channel"&@CRLF)
		ControlSend("BUSMASTER","",59664,"SendMsg(sMsg);"&@CRLF)
		ControlSend("BUSMASTER","",59664,"Trace"&"("&Chr(34)&"Time Check Handler"&Chr(34)&");")
		send("^s")
		$nodeHWD=ControlGetHandle("BUSMASTER","","[CLASS:SysTreeView32; INSTANCE:1]")
		$item1HWD=_GUICtrlTreeView_GetNext($nodeHWD,"")
		$item2HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item1HWD)					; Get handle of second item
		_GUICtrlTreeView_ClickItem($nodeHWD,$item2HWD)
		$item3HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item2HWD)					; Get handle of third item
		_GUICtrlTreeView_ClickItem($nodeHWD,$item3HWD)
		$item4HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item3HWD)					; Get handle of Fourth item
		_GUICtrlTreeView_ClickItem($nodeHWD,$item4HWD,"Right")
		send("a")																		; Add a Key hander
		sleep(1000)
		if WinExists("Enter Key Value") Then
			send("a")																	; enter key 'a'
			ControlClick("Enter Key Value","",3)										; Click 'Apply'
			sleep(1000)
			send("b")																	; enter key 'b'
			ControlClick("Enter Key Value","",1)										; Click 'Apply'
		EndIf
		$nodeHWD=ControlGetHandle("BUSMASTER","","[CLASS:SysTreeView32; INSTANCE:1]")
		$item1HWD=_GUICtrlTreeView_GetNext($nodeHWD,"")
		$item2HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item1HWD)					; Get handle of second item
		_GUICtrlTreeView_ClickItem($nodeHWD,$item2HWD)
		$item3HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item2HWD)					; Get handle of third item
		_GUICtrlTreeView_ClickItem($nodeHWD,$item3HWD)
		$item4HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item3HWD)					; Get handle of Fourth item
		_GUICtrlTreeView_ClickItem($nodeHWD,$item4HWD)
		$keyHWD=_GUICtrlTreeView_GetFirstChild($nodeHWD,$item4HWD)
		_GUICtrlTreeView_ClickItem($nodeHWD,$keyHWD,"Left","",2)
		sleep(2000)
		ControlSend("BUSMASTER","",59664,"Trace"&"("&Chr(34)&"Key 'a' is pressed"&Chr(34)&");")
		send("^s")
		$item1HWD=_GUICtrlTreeView_GetNext($nodeHWD,"")
		$item2HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item1HWD)					; Get handle of second item
		_GUICtrlTreeView_ClickItem($nodeHWD,$item2HWD)
		$item3HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item2HWD)					; Get handle of third item
		_GUICtrlTreeView_ClickItem($nodeHWD,$item3HWD)
		$item4HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item3HWD)					; Get handle of Fourth item
		_GUICtrlTreeView_ClickItem($nodeHWD,$item4HWD)
		$item5HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item4HWD)					; Get handle of Fifth item
		_GUICtrlTreeView_ClickItem($nodeHWD,$item5HWD,"Right")
		send("a")																		; add error handler
		sleep(1000)
		if WinExists("Select Error Handler(s)") Then
			ControlCommand("Select Error Handler(s)","",1155,"Check")					; select error active
			ControlCommand("Select Error Handler(s)","",1156,"Check")					; select error passive
			ControlCommand("Select Error Handler(s)","",1157,"Check")					; select warning limit
			ControlCommand("Select Error Handler(s)","",1158,"Check")					; select Bus Off
			ControlCommand("Select Error Handler(s)","",1159,"Check")					; select error Frame
			controlclick("Select Error Handler(s)","",1176)								;click 'OK'
		EndIf
		$item1HWD=_GUICtrlTreeView_GetNext($nodeHWD,"")
		$item2HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item1HWD)					; Get handle of second item
		_GUICtrlTreeView_ClickItem($nodeHWD,$item2HWD)
		$item3HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item2HWD)					; Get handle of third item
		_GUICtrlTreeView_ClickItem($nodeHWD,$item3HWD)
		$item4HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item3HWD)					; Get handle of Fourth item
		_GUICtrlTreeView_ClickItem($nodeHWD,$item4HWD)
		$item5HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item4HWD)					; Get handle of Fifth item
		_GUICtrlTreeView_ClickItem($nodeHWD,$item5HWD,"Right")
		send("d")																		; Delete error handlers
		sleep(1000)
		if WinExists("Delete Error Handler(s)") Then
			ControlCommand("Delete Error Handler(s)","",1155,"UnCheck")					; Uncheck Error active
			controlclick("Delete Error Handler(s)","",1176)								; click 'OK'
		EndIf
		$item1HWD=_GUICtrlTreeView_GetNext($nodeHWD,"")
		$item2HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item1HWD)					; Get handle of second item
		_GUICtrlTreeView_ClickItem($nodeHWD,$item2HWD)
		$item3HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item2HWD)					; Get handle of third item
		_GUICtrlTreeView_ClickItem($nodeHWD,$item3HWD)
		$item4HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item3HWD)					; Get handle of Fourth item
		_GUICtrlTreeView_ClickItem($nodeHWD,$item4HWD)
		$item5HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item4HWD)					; Get handle of Fifth item
		_GUICtrlTreeView_ClickItem($nodeHWD,$item5HWD)
		$errHWD=_GUICtrlTreeView_GetFirstChild($nodeHWD,$item5HWD)
		_GUICtrlTreeView_ClickItem($nodeHWD,$errHWD,"Left","",2)
		sleep(2000)
		ControlSend("BUSMASTER","",59664,"STCAN_MSG sMsg;"&@CRLF)
		ControlSend("BUSMASTER","",59664,"sMsg.m_unMsgID = 0x55;              // Message ID"&@CRLF)
		ControlSend("BUSMASTER","",59664,"sMsg.m_ucEXTENDED = FALSE;           // Standard Message type"&@CRLF)
		ControlSend("BUSMASTER","",59664,"sMsg.m_ucRTR = FALSE;                // Not RTR type"&@CRLF)
		ControlSend("BUSMASTER","",59664,"sMsg.m_ucDLC = 8;                    // Length is 8 Bytes"&@CRLF)
		ControlSend("BUSMASTER","",59664,"sMsg.m_sWhichBit.m_aulData[0] = 10;  // Lower 4 Bytes"&@CRLF)
		ControlSend("BUSMASTER","",59664,"sMsg.m_sWhichBit.m_aulData[1] = 20;  // Upper 4 Bytes"&@CRLF)
		ControlSend("BUSMASTER","",59664,"sMsg.m_ucChannel = 1;                // First CAN channel"&@CRLF)
		ControlSend("BUSMASTER","",59664,"SendMsg(sMsg);")
		send("^s")
		$item1HWD=_GUICtrlTreeView_GetNext($nodeHWD,"")
		$item2HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item1HWD)					; Get handle of second item
		_GUICtrlTreeView_ClickItem($nodeHWD,$item2HWD)
		$item3HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item2HWD)					; Get handle of third item
		_GUICtrlTreeView_ClickItem($nodeHWD,$item3HWD)
		$item4HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item3HWD)					; Get handle of Fourth item
		_GUICtrlTreeView_ClickItem($nodeHWD,$item4HWD)
		$item5HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item4HWD)					; Get handle of Fifth item
		_GUICtrlTreeView_ClickItem($nodeHWD,$item5HWD)
		$item6HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item5HWD)					; Get handle of Sixth item
		_GUICtrlTreeView_ClickItem($nodeHWD,$item6HWD,"Right")
		send("a")																		; Add DLL handler
		sleep(2000)
		if winexists("Add DLL Handler(s)") Then
			ControlCommand("Add DLL Handler(s)","",1164,"Check")						; Check Load
			ControlCommand("Add DLL Handler(s)","",1165,"Check")						; Check Unload
			controlclick("Add DLL Handler(s)","",1178)									; click 'OK'
		endif
		$item1HWD=_GUICtrlTreeView_GetNext($nodeHWD,"")
		$item2HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item1HWD)					; Get handle of second item
		_GUICtrlTreeView_ClickItem($nodeHWD,$item2HWD)
		$item3HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item2HWD)					; Get handle of third item
		_GUICtrlTreeView_ClickItem($nodeHWD,$item3HWD)
		$item4HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item3HWD)					; Get handle of Fourth item
		_GUICtrlTreeView_ClickItem($nodeHWD,$item4HWD)
		$item5HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item4HWD)					; Get handle of Fifth item
		_GUICtrlTreeView_ClickItem($nodeHWD,$item5HWD)
		$item6HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item5HWD)					; Get handle of Sixth item
		_GUICtrlTreeView_ClickItem($nodeHWD,$item6HWD)
		$dllHWD=_GUICtrlTreeView_GetFirstChild($nodeHWD,$item6HWD)
		_GUICtrlTreeView_ClickItem($nodeHWD,$dllHWD,"Left","",2)
		sleep(2000)
		ControlSend("BUSMASTER","",59664,"Trace("&Chr(34)&"DLL _Load"&Chr(34)&");")
		send("^s")
		$item1HWD=_GUICtrlTreeView_GetNext($nodeHWD,"")
		$item2HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item1HWD)					; Get handle of second item
		_GUICtrlTreeView_ClickItem($nodeHWD,$item2HWD)
		$item3HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item2HWD)					; Get handle of third item
		_GUICtrlTreeView_ClickItem($nodeHWD,$item3HWD)
		$item4HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item3HWD)					; Get handle of Fourth item
		_GUICtrlTreeView_ClickItem($nodeHWD,$item4HWD)
		$item5HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item4HWD)					; Get handle of Fifth item
		_GUICtrlTreeView_ClickItem($nodeHWD,$item5HWD)
		$item6HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item5HWD)					; Get handle of Sixth item
		_GUICtrlTreeView_ClickItem($nodeHWD,$item6HWD)
		$dllHWD=_GUICtrlTreeView_GetFirstChild($nodeHWD,$item6HWD)
		$undllHWD=_GUICtrlTreeView_GetNextChild($nodeHWD,$dllHWD)
		_GUICtrlTreeView_ClickItem($nodeHWD,$undllHWD,"Left","",2)
		sleep(2000)
		ControlSend("BUSMASTER","",59664,"Trace("&Chr(34)&"DLL _UnLoad"&Chr(34)&");")
		send("^s")
		$item1HWD=_GUICtrlTreeView_GetNext($nodeHWD,"")
		$item2HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item1HWD)					; Get handle of second item
		_GUICtrlTreeView_ClickItem($nodeHWD,$item2HWD)
		$item3HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item2HWD)					; Get handle of third item
		_GUICtrlTreeView_ClickItem($nodeHWD,$item3HWD)
		$item4HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item3HWD)					; Get handle of Fourth item
		_GUICtrlTreeView_ClickItem($nodeHWD,$item4HWD)
		$item5HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item4HWD)					; Get handle of Fifth item
		_GUICtrlTreeView_ClickItem($nodeHWD,$item5HWD)
		$item6HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item5HWD)					; Get handle of Sixth item
		_GUICtrlTreeView_ClickItem($nodeHWD,$item6HWD)
		$item7HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item6HWD)					; Get handle of Seventh item
		_GUICtrlTreeView_ClickItem($nodeHWD,$item7HWD,"Right")
		send("a")																		; add a utility handler
		Sleep(1000)
		if WinExists("Add Utility Function") Then
			controlcommand("Add Utility Function","",1029,"SelectString","int")
			ControlSetText("Add Utility Function","",1030,"_Addint x, int y)")			; check for validation
			send("{ENTER}")
			if winexists("BUSMASTER") Then
				send("{ENTER}")
			EndIf
			ControlSetText("Add Utility Function","",1030,"_Add(int x, int y")			; check for validation
			send("{ENTER}")
			if winexists("BUSMASTER") Then
				send("{ENTER}")
			EndIf
			ControlSetText("Add Utility Function","",1030,"_Add(int x, int y)")			; enter function name
			send("{ENTER}")
		EndIf
		$item1HWD=_GUICtrlTreeView_GetNext($nodeHWD,"")
		$item2HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item1HWD)					; Get handle of second item
		_GUICtrlTreeView_ClickItem($nodeHWD,$item2HWD)
		$item3HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item2HWD)					; Get handle of third item
		_GUICtrlTreeView_ClickItem($nodeHWD,$item3HWD)
		$item4HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item3HWD)					; Get handle of Fourth item
		_GUICtrlTreeView_ClickItem($nodeHWD,$item4HWD)
		$item5HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item4HWD)					; Get handle of Fifth item
		_GUICtrlTreeView_ClickItem($nodeHWD,$item5HWD)
		$item6HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item5HWD)					; Get handle of Sixth item
		_GUICtrlTreeView_ClickItem($nodeHWD,$item6HWD)
		$item7HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item6HWD)					; Get handle of Seventh item
		_GUICtrlTreeView_ClickItem($nodeHWD,$item7HWD)
		$utilHWD=_GUICtrlTreeView_GetFirstChild($nodeHWD,$item7HWD)
		_GUICtrlTreeView_ClickItem($nodeHWD,$utilHWD,"Left","",2)
		sleep(2000)
		ControlSend("BUSMASTER","",59664,"int sum = 0;"&@CRLF)							; enter the function
		controlSend("BUSMASTER","",59664,"sum = x+y;",1&@CRLF)
		controlSend("BUSMASTER","",59664,@CRLF)
		ControlSend("BUSMASTER","",59664,"return sum;"&@CRLF)
		send("^s")
		$item1HWD=_GUICtrlTreeView_GetNext($nodeHWD,"")
		$item2HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item1HWD)					; Get handle of second item
		_GUICtrlTreeView_ClickItem($nodeHWD,$item2HWD)
		$item3HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item2HWD)					; Get handle of third item
		_GUICtrlTreeView_ClickItem($nodeHWD,$item3HWD)
		$item4HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item3HWD)					; Get handle of Fourth item
		_GUICtrlTreeView_ClickItem($nodeHWD,$item4HWD)
		$item5HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item4HWD)					; Get handle of Fifth item
		_GUICtrlTreeView_ClickItem($nodeHWD,$item5HWD)
		$item6HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item5HWD)					; Get handle of Sixth item
		_GUICtrlTreeView_ClickItem($nodeHWD,$item6HWD)
		$item7HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item6HWD)					; Get handle of Seventh item
		_GUICtrlTreeView_ClickItem($nodeHWD,$item7HWD)
		$item8HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item7HWD)					; Get handle of Eighth item
		_GUICtrlTreeView_ClickItem($nodeHWD,$item8HWD,"Left","",2)						; Double click on the Global Variable Node
		ControlSend("BUSMASTER","",59664,"int x;"&@CRLF)							; enter the function
		controlSend("BUSMASTER","",59664,"int y;",1&@CRLF)
		sleep(1000)
		send("^s")
		sleep(1000)
		$item1HWD=_GUICtrlTreeView_GetNext($nodeHWD,"")									; Get handle of first item
		$item2HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item1HWD)					; Get handle of second item
		$MSG1HWD=_GUICtrlTreeView_GetFirstChild($nodeHWD,$item2HWD)
		_GUICtrlTreeView_ClickItem($nodeHWD,$msg1HWD,"Left","",2)					    ; Click on First Header
		sleep(1000)
		$editPos=ControlGetPos("BUSMASTER", "", 59664)									; Get position of richEdit control
		MouseClick("right",$editPos[0]+30,$editPos[1]+55)
		send("f")																		; Select 'insert function'
		sleep(1000)
		if winexists("Insert Function") then
			$funcLBHWD=controlgethandle("Insert Function","",1068)							; Get handle of the list box
			_GUICtrlListBox_ClickItem($funcLBHWD,0)											; Click on the 1st item
			send("{ENTER}")
		endif
		MouseClick("right",$editPos[0]+30,$editPos[1]+55)
		send("m")																		; Select 'insert message'
		sleep(1000)
		if winexists("Dialog") Then
			ControlCommand("Dialog","",1028,"Check")									; Check the checkbox
			send("{ENTER}")																; Click on select button
		EndIf
		MouseClick("right",$editPos[0]+30,$editPos[1]+55)
		send("s")																		; Select 'insert signal'
		sleep(1000)
		if winexists("Dialog") Then
			ControlCommand("Dialog","",1028,"Check")									; Check the checkbox
			send("{ENTER}")																; Click on select button
		EndIf
		send("^s")
		send("^a")
		send("{DELETE}")
		send("^s")
		send("^{F4}")															; Close the C file window
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
		controlclick("BUSMASTER","Enable All Handlers",1015)					; click Enable All andler
	EndIf
	send("{F2}")																; Connect
	send("a")
	sleep(6000)
	$cntToolhWd=ControlGetHandle("BUSMASTER","",128)
	_GUICtrlToolbar_ClickIndex($cntToolhWd,4)									; Disconnect
	sleep(1000)
	controlclick("BUSMASTER","Disable All Handlers",1015)						; click Disable All Handler
	$hdlDetailsHWD=ControlGetHandle("BUSMASTER","",1226)
	_GUICtrlListView_ClickItem($hdlDetailsHWD,0)								; Click 1st item in the 'Handler Details' list view
	sleep(500)
	controlclick("BUSMASTER","Enable Handler",1014)								; click Enable Handler
	sleep(500)
	send("{F2}")																; Connect
	sleep(3000)
	_GUICtrlToolbar_ClickIndex($cntToolhWd,4)									; Disconnect
	sleep(500)
	_GUICtrlListView_ClickItem($hdlDetailsHWD,0)								; Click 1st item in the 'Handler Details' list view
	sleep(1000)
	controlclick("BUSMASTER","Disable Handler",1014)							; click Disable Handler
	sleep(1000)
	_GUICtrlListView_ClickItem($hdlDetailsHWD,1)								; Click 2nd item in the 'Handler Details' list view
	sleep(500)
	controlclick("BUSMASTER","Enable Handler",1014)								; click Enable Handler
	sleep(500)
	send("{F2}")																; Connect
	sleep(3000)
	_GUICtrlToolbar_ClickIndex($cntToolhWd,4)									; Disconnect
	sleep(500)
	_GUICtrlListView_ClickItem($hdlDetailsHWD,1)								; Click 2st item in the 'Handler Details' list view
	sleep(1000)
	controlclick("BUSMASTER","Disable Handler",1014)							; click Disable Handler
	sleep(1000)
	_GUICtrlListView_ClickItem($hdlDetailsHWD,2)								; Click 3rd item in the 'Handler Details' list view
	sleep(500)
	controlclick("BUSMASTER","Enable Handler",1014)								; click Enable Handler
	sleep(500)
	send("{F2}")																; Connect
	sleep(1000)
	send("a")
	sleep(3000)
	_GUICtrlToolbar_ClickIndex($cntToolhWd,4)									; Disconnect
	sleep(500)
	_GUICtrlListView_ClickItem($hdlDetailsHWD,2)								; Click 3rd item in the 'Handler Details' list view
	sleep(500)
	controlclick("BUSMASTER","Disable Handler",1014)							; click Disable Handler
	sleep(1000)
	_GUICtrlListView_ClickItem($hdlDetailsHWD,3)								; Click 4th item in the 'Handler Details' list view
	sleep(500)
	controlclick("BUSMASTER","Enable Handler",1014)								; click Enable Handler
	sleep(500)
	send("{F2}")																; Connect
	sleep(3000)
	_GUICtrlToolbar_ClickIndex($cntToolhWd,4)									; Disconnect
	sleep(500)
	_GUICtrlListView_ClickItem($hdlDetailsHWD,3)								; Click 4th item in the 'Handler Details' list view
	sleep(500)
	Controlclick("BUSMASTER","Disable Handler",1014)							; click Disable Handler
	sleep(1000)
	$node1HWD=ControlGetHandle("BUSMASTER","","[CLASS:SysTreeView32; INSTANCE:1]")	; Get tree view control handle
	$item1HWD=_GUICtrlTreeView_GetNext($node1HWD,"")
	_GUICtrlTreeView_ClickItem($node1HWD,$item1HWD,"Right")						; Right click on sim file
	sleep(1000)
	send("b")																	; Select 'Build All' from the context menu
	sleep(1000)
	$item1HWD=_GUICtrlTreeView_GetNext($node1HWD,"")
	_GUICtrlTreeView_ClickItem($node1HWD,$item1HWD,"Right")						; Right click on sim file
	sleep(1000)
	send("l")																	; Select 'Load All' from the context menu
	sleep(1000)
	$item1HWD=_GUICtrlTreeView_GetNext($node1HWD,"")
	_GUICtrlTreeView_ClickItem($node1HWD,$item1HWD,"Right")						; Right click on sim file
	sleep(1000)
	send("u")																	; Select 'Unload All' from the context menu
	sleep(1000)
	$item1HWD=_GUICtrlTreeView_GetNext($node1HWD,"")
	_GUICtrlTreeView_ClickItem($node1HWD,$item1HWD,"Right")						; Right click on sim file
	sleep(1000)
	send("l")																	; Select 'Load All' from the context menu
	sleep(1000)
	$item1HWD=_GUICtrlTreeView_GetNext($node1HWD,"")
	_GUICtrlTreeView_ClickItem($node1HWD,$item1HWD,"Right")						; Right click on sim file
	sleep(1000)
	send("r")																	; Select 'Remove All Load' from the context menu
	sleep(1000)
	if winexists("BUSMASTER") Then
		send("{ENTER}")															; Click 'OK' fromd the unload dll error pop up
	EndIf
	$item1HWD=_GUICtrlTreeView_GetNext($node1HWD,"")
	_GUICtrlTreeView_ClickItem($node1HWD,$item1HWD,"Right")						; Right click on sim file
	sleep(1000)
	send("u")																	; Select 'Unload All' from the context menu
	sleep(1000)
	$item1HWD=_GUICtrlTreeView_GetNext($node1HWD,"")
	_GUICtrlTreeView_ClickItem($node1HWD,$item1HWD,"Right")						; Right click on sim file
	sleep(1000)
	send("b")																	; Select 'Build All' from the context menu
	sleep(1000)
	_GUICtrlToolbar_ClickIndex($cntToolhWd,4)									; Connect
	$item1HWD=_GUICtrlTreeView_GetNext($node1HWD,"")
	_GUICtrlTreeView_ClickItem($node1HWD,$item1HWD,"Right")						; Right click on sim file
	sleep(1000)
	send("l")																	; Select 'Load All' from the context menu
	sleep(1000)
	$prntHWD=_GUICtrlTreeView_GetItemHandle($node1HWD,0)
	_GUICtrlTreeView_ClickItem($node1HWD,$prntHWD,"Right")					    ; Right click on First parent itemr
	sleep(1000)
	send("d")																	; Select 'Delete All Systems' from context menu
	sleep(1000)
	if winexists("BUSMASTER") Then
		send("{ENTER}")															; Click 'OK' from the unload dll error pop up
	EndIf
	sleep(1000)
	send("!fgs")																; Save Config
EndIf
