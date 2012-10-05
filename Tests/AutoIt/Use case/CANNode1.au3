;=== Test Cases/Test Data ===
; Critical (C):		Y
; TestCase No.:		TC_03
; Test Cases:		Node Simultion - ADD
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
#include <GuiTreeView.au3>
#include <GuiListView.au3>
#include <globalFunc.au3>
#include <GuiToolBar.au3>
#include <GuiListBox.au3>

WinActivate("BUSMASTER", 3)

If WinExists("BUSMASTER") Then
	Sleep(2000)
	WinMenuSelectItem("BUSMASTER","","&Configure","&Hardware Interface","&Simulation")
	_loadConfig("SiSysTest") ;load cfx file
	_loadDatabase("SimCAN") ;load dbf file
	Send("!fdc")
	_associateDB("&File", "SimCAN.dbf") ;associate dbf
	Sleep(2000)
	$Count = 2
	WinMenuSelectItem("BUSMASTER", "", "&Configure", "&Tx Messages") ; configure Tx messages
	Sleep(1000)
	If (ControlCommand("BUSMASTER", "", 17000, "IsChecked") = 0) Then
		ControlCommand("BUSMASTER", "", 17000, "Check") ; check 'Autoupdate' check box
	EndIf
	_txMSG("Cyclic", $Count)
	ControlClick("BUSMASTER", "", 1015) ; Close Tx Window
	Sleep(2000)
	WinMenuSelectItem("BUSMASTER", "", "&Configure", "&Simulated Systems") ;select Configure->Simulated Systems
	$pos = ControlGetPos("BUSMASTER", "", "[CLASS:SysTreeView32; INSTANCE:1]")
	ConsoleWrite("Inside Simulated system window")
	MouseClick("right", $pos[0] + 30, $pos[1] + 55) ; select new sim
	Send("n")
	Send(@DesktopDir & "\testCANSim.sim")
	Send("{ENTER}")
	Sleep(1000)
	If WinExists("New Simulated system Configuration Filename...") Then
		ControlClick("New Simulated system Configuration Filename...", "", "[CLASS:Button; INSTANCE:1]")
	EndIf
	MouseClick("right", $pos[0] + 50, $pos[1] + 65)
	Send("a") ; select add node
	Sleep(1000)
	If WinExists("Node Details") Then
		ControlSetText("Node Details", "", "[CLASS:Edit; INSTANCE:1]", "TestNode") ; enter node name
		Send("{ENTER}")
	EndIf
	Sleep(1000)
	If WinExists("BUSMASTER") Then
		ControlClick("BUSMASTER", "", "[CLASS:Button; INSTANCE:3]") ; Click Add File
		Sleep(1000)
		If WinExists("Select BUSMASTER Source Filename...") Then
			Sleep(1000)
			Send(@DesktopDir & "\addCANHandlers")
			Send("{ENTER}")
		EndIf
		;
		ConsoleWrite("click item" & @CRLF)
		Sleep(2000)
		$nodeHWD = ControlGetHandle("BUSMASTER", "", "[CLASS:SysTreeView32; INSTANCE:1]") ; Get tree view control handle
		$item1HWD = _GUICtrlTreeView_GetNext($nodeHWD, "") ; Get handle of first item
		$incHWD = _GUICtrlTreeView_GetFirstChild($nodeHWD, $item1HWD)
		_GUICtrlTreeView_ClickItem($nodeHWD, $incHWD, "Right") ; Right click on First Header
		Sleep(500)
		Send("d") ; Delete
		Sleep(1000)
		If WinExists("BUSMASTER") Then
			Send("{ENTER}") ; Click  'Yes'
		EndIf
		$item1HWD = _GUICtrlTreeView_GetNext($nodeHWD, "") ; Get handle of first item
		_GUICtrlTreeView_ClickItem($nodeHWD, $item1HWD, "Right") ; Click on Include Handler
		Send("a") ; Add a new handler
		Sleep(1000)
		If WinExists("Enter Header File") Then
			ControlSetText("Enter Header File", "", 1049, Chr(34) & "Windows.h" & Chr(34)) ; Enter Header File
			ControlClick("Enter Header File", "", 1051) ; Click Browse Button
			Sleep(1000)
			If WinExists("Select Header File") Then
				ControlClick("Select Header File", "", 2) ; Click Cancel button
			EndIf
			ControlClick("Enter Header File", "", 1042) ; Click 'OK'
		EndIf
		Send("^s")
		$item1HWD = _GUICtrlTreeView_GetNext($nodeHWD, "") ; Get handle of first item
		$item21HWD = _GUICtrlTreeView_GetNextSibling($nodeHWD, $item1HWD) ; Get handle of second item
		_GUICtrlTreeView_ClickItem($nodeHWD, $item21HWD, "Right") ; Click on Bus Event Handler
		Send("a") ; Select add from right click context menu
		Sleep(1000)
		If WinExists("Add Bus Events Handler(s)") Then
			ControlCommand("Add Bus Events Handler(s)", "", 4011, "Check", "") ; Check connect
			ControlCommand("Add Bus Events Handler(s)", "", 4012, "Check", "") ; Check Disconnect
			ControlClick("Add Bus Events Handler(s)", "", 1) ; Click on 'OK' button
		EndIf
		$item1HWD = _GUICtrlTreeView_GetNext($nodeHWD, "") ; Get handle of first item
		$item21HWD = _GUICtrlTreeView_GetNextSibling($nodeHWD, $item1HWD) ; Get handle of second item
		_GUICtrlTreeView_ClickItem($nodeHWD, $item21HWD)
		$item2HWD = _GUICtrlTreeView_GetNextSibling($nodeHWD, $item21HWD) ; Get handle of third item
		_GUICtrlTreeView_ClickItem($nodeHWD, $item2HWD, "Right") ; Click on Message Handler
		Send("a") ; Add a new msg
		Sleep(1000)
		If WinExists("Add Message Handler") Then ; enter the msg contents
			ControlSetText("Add Message Handler", "", 1155, 51) ; Enter Msg ID
			ControlClick("Add Message Handler", "", 1167) ; Click OK
		EndIf
		$item1HWD = _GUICtrlTreeView_GetNext($nodeHWD, "") ; Get handle of first item
		$item21HWD = _GUICtrlTreeView_GetNextSibling($nodeHWD, $item1HWD) ; Get handle of second item
		_GUICtrlTreeView_ClickItem($nodeHWD, $item21HWD)
		$item2HWD = _GUICtrlTreeView_GetNextSibling($nodeHWD, $item21HWD) ; Get handle of third item
		_GUICtrlTreeView_ClickItem($nodeHWD, $item2HWD, "Right") ; Click on Message Handler
		Send("a") ; Add a new msg
		Sleep(1000)
		If WinExists("Add Message Handler") Then ; enter the msg contents
			ControlCommand("Add Message Handler", "", 1172, "Check") ; Check Msg ID range radio button
			ControlSetText("Add Message Handler", "", 1160, 51) ; Enter Msg ID range 'From'
			ControlSetText("Add Message Handler", "", 1161, 55) ; Enter Msg ID range 'To'
			ControlClick("Add Message Handler", "", 1167) ; Click 'OK'
		EndIf
		$item1HWD = _GUICtrlTreeView_GetNext($nodeHWD, "") ; Get handle of first item
		$item21HWD = _GUICtrlTreeView_GetNextSibling($nodeHWD, $item1HWD) ; Get handle of second item
		_GUICtrlTreeView_ClickItem($nodeHWD, $item21HWD)
		$item2HWD = _GUICtrlTreeView_GetNextSibling($nodeHWD, $item21HWD) ; Get handle of third item
		_GUICtrlTreeView_ClickItem($nodeHWD, $item2HWD, "Right") ; Click on Message Handler
		Send("a") ; Add a new msg
		Sleep(1000)
		If WinExists("Add Message Handler") Then ; enter the msg contents
			ControlCommand("Add Message Handler", "", 1173, "Check") ; Check 'All Rx Msgs' radio button
			ControlClick("Add Message Handler", "", 1167) ; Click 'OK'
		EndIf
		$item1HWD = _GUICtrlTreeView_GetNext($nodeHWD, "") ; Get handle of first item
		$item21HWD = _GUICtrlTreeView_GetNextSibling($nodeHWD, $item1HWD) ; Get handle of second item
		_GUICtrlTreeView_ClickItem($nodeHWD, $item21HWD)
		$item2HWD = _GUICtrlTreeView_GetNextSibling($nodeHWD, $item21HWD) ; Get handle of third item
		_GUICtrlTreeView_ClickItem($nodeHWD, $item2HWD, "Right") ; Click on Message Handler
		Send("a") ; Add a new msg
		Sleep(1000)
		If WinExists("Add Message Handler") Then ; enter the msg contents
			$dbMsgListHWD = ControlGetHandle("Add Message Handler", "", 1169) ; fetch DB Message List Handler
			ControlCommand("Add Message Handler", "", 1170, "Check") ; Check 'Message Name radio button
			_GUICtrlListBox_ClickItem($dbMsgListHWD, 0) ; Click on First item
			Sleep(500)
			ControlClick("Add Message Handler", "", 1167) ; Click 'OK'
		EndIf


		; Add Time Handler
		$nodeHWD = ControlGetHandle("BUSMASTER", "", "[CLASS:SysTreeView32; INSTANCE:1]")
		$item1HWD = _GUICtrlTreeView_GetNext($nodeHWD, "")
		$item21HWD = _GUICtrlTreeView_GetNextSibling($nodeHWD, $item1HWD) ; Get handle of second item
		_GUICtrlTreeView_ClickItem($nodeHWD, $item21HWD) ; Click on Bus Event Handle
		$item2HWD = _GUICtrlTreeView_GetNextSibling($nodeHWD, $item21HWD) ; Get handle of third item
		_GUICtrlTreeView_ClickItem($nodeHWD, $item2HWD)
		$item3HWD = _GUICtrlTreeView_GetNextSibling($nodeHWD, $item2HWD) ; Get handle of fourth item
		_GUICtrlTreeView_ClickItem($nodeHWD, $item3HWD, "Right")
		Send("a") ; Add a timer
		Sleep(1000)
		If WinExists("Define New Timer Handler") Then
			ControlSetText("Define New Timer Handler", "", 1044, "TimeHandlerName") ; Add time handler name
			ControlClick("Define New Timer Handler", "", 1) ; Click 'OK'
		EndIf
		$nodeHWD = ControlGetHandle("BUSMASTER", "", "[CLASS:SysTreeView32; INSTANCE:1]")
		$item1HWD = _GUICtrlTreeView_GetNext($nodeHWD, "")
		$item21HWD = _GUICtrlTreeView_GetNextSibling($nodeHWD, $item1HWD) ; Get handle of second item
		_GUICtrlTreeView_ClickItem($nodeHWD, $item21HWD)
		$item2HWD = _GUICtrlTreeView_GetNextSibling($nodeHWD, $item21HWD) ; Get handle of third item
		_GUICtrlTreeView_ClickItem($nodeHWD, $item2HWD)
		$item3HWD = _GUICtrlTreeView_GetNextSibling($nodeHWD, $item2HWD) ; Get handle of Fourth item
		_GUICtrlTreeView_ClickItem($nodeHWD, $item3HWD)
		$timeHWD = _GUICtrlTreeView_GetFirstChild($nodeHWD, $item3HWD)
		_GUICtrlTreeView_ClickItem($nodeHWD, $timeHWD, "Left", "", 2)
		Sleep(1000) ; Function description
		ControlSend("BUSMASTER", "", 59664, "STCAN_MSG sMsg;" & @CRLF)
		ControlSend("BUSMASTER", "", 59664, "sMsg.m_unMsgID = 0x55;              // Message ID" & @CRLF)
		ControlSend("BUSMASTER", "", 59664, "sMsg.m_ucEXTENDED = FALSE;           // Standard Message type" & @CRLF)
		ControlSend("BUSMASTER", "", 59664, "sMsg.m_ucRTR = FALSE;                // Not RTR type" & @CRLF)
		ControlSend("BUSMASTER", "", 59664, "sMsg.m_ucDLC = 8;                    // Length is 8 Bytes" & @CRLF)
		ControlSend("BUSMASTER", "", 59664, "sMsg.m_sWhichBit.m_aulData[0] = 10;  // Lower 4 Bytes" & @CRLF)
		ControlSend("BUSMASTER", "", 59664, "sMsg.m_sWhichBit.m_aulData[1] = 20;  // Upper 4 Bytes" & @CRLF)
		ControlSend("BUSMASTER", "", 59664, "sMsg.m_ucChannel = 1;                // First CAN channel" & @CRLF)
		ControlSend("BUSMASTER", "", 59664, "SendMsg(sMsg);" & @CRLF)
		ControlSend("BUSMASTER", "", 59664, "Trace" & "(" & Chr(34) & "Time Check Handler" & Chr(34) & ");")
		Send("^s")
		$nodeHWD = ControlGetHandle("BUSMASTER", "", "[CLASS:SysTreeView32; INSTANCE:1]")
		$item1HWD = _GUICtrlTreeView_GetNext($nodeHWD, "")
		$item21HWD = _GUICtrlTreeView_GetNextSibling($nodeHWD, $item1HWD) ; Get handle of second item
		_GUICtrlTreeView_ClickItem($nodeHWD, $item21HWD)
		$item2HWD = _GUICtrlTreeView_GetNextSibling($nodeHWD, $item21HWD) ; Get handle of third item
		_GUICtrlTreeView_ClickItem($nodeHWD, $item2HWD)
		$item3HWD = _GUICtrlTreeView_GetNextSibling($nodeHWD, $item2HWD) ; Get handle of Fourth item
		_GUICtrlTreeView_ClickItem($nodeHWD, $item3HWD)
		$item4HWD = _GUICtrlTreeView_GetNextSibling($nodeHWD, $item3HWD) ; Get handle of fifth item
		_GUICtrlTreeView_ClickItem($nodeHWD, $item4HWD, "Right")
		Send("a") ; Add a Key hander
		Sleep(1000)
		If WinExists("Enter Key Value") Then
			Send("a") ; enter key 'a'
			ControlClick("Enter Key Value", "", 1) ; Click 'Apply'
			Sleep(1000)
			Send("b") ; enter key 'b'
			ControlClick("Enter Key Value", "", 1) ; Click 'Apply'
		EndIf
		$nodeHWD = ControlGetHandle("BUSMASTER", "", "[CLASS:SysTreeView32; INSTANCE:1]")
		$item1HWD = _GUICtrlTreeView_GetNext($nodeHWD, "")
		$item21HWD = _GUICtrlTreeView_GetNextSibling($nodeHWD, $item1HWD) ; Get handle of second item
		_GUICtrlTreeView_ClickItem($nodeHWD, $item21HWD)
		$item2HWD = _GUICtrlTreeView_GetNextSibling($nodeHWD, $item21HWD) ; Get handle of third item
		_GUICtrlTreeView_ClickItem($nodeHWD, $item2HWD)
		$item3HWD = _GUICtrlTreeView_GetNextSibling($nodeHWD, $item2HWD) ; Get handle of Fourth item
		_GUICtrlTreeView_ClickItem($nodeHWD, $item3HWD)
		$item4HWD = _GUICtrlTreeView_GetNextSibling($nodeHWD, $item3HWD) ; Get handle of fifth item
		_GUICtrlTreeView_ClickItem($nodeHWD, $item4HWD)
		$keyHWD = _GUICtrlTreeView_GetFirstChild($nodeHWD, $item4HWD)
		_GUICtrlTreeView_ClickItem($nodeHWD, $keyHWD, "Left", "", 2)
		Sleep(2000)
		ControlSend("BUSMASTER", "", 59664, "Trace" & "(" & Chr(34) & "Key 'a' is pressed" & Chr(34) & ");")
		Send("^s")
		$item1HWD = _GUICtrlTreeView_GetNext($nodeHWD, "")
		$item21HWD = _GUICtrlTreeView_GetNextSibling($nodeHWD, $item1HWD) ; Get handle of second item
		_GUICtrlTreeView_ClickItem($nodeHWD, $item21HWD)
		$item2HWD = _GUICtrlTreeView_GetNextSibling($nodeHWD, $item21HWD) ; Get handle of third item
		_GUICtrlTreeView_ClickItem($nodeHWD, $item2HWD)
		$item3HWD = _GUICtrlTreeView_GetNextSibling($nodeHWD, $item2HWD) ; Get handle of Fourth item
		_GUICtrlTreeView_ClickItem($nodeHWD, $item3HWD)
		$item4HWD = _GUICtrlTreeView_GetNextSibling($nodeHWD, $item3HWD) ; Get handle of fifth item
		_GUICtrlTreeView_ClickItem($nodeHWD, $item4HWD)
		$item5HWD = _GUICtrlTreeView_GetNextSibling($nodeHWD, $item4HWD) ; Get handle of sixth item
		_GUICtrlTreeView_ClickItem($nodeHWD, $item5HWD, "Right")
		Send("a") ; add error handler
		Sleep(1000)
		If WinExists("Select Error Handler(s)") Then
			ControlCommand("Select Error Handler(s)", "", 1155, "Check") ; select error active
			ControlCommand("Select Error Handler(s)", "", 1156, "Check") ; select error passive
			ControlCommand("Select Error Handler(s)", "", 1157, "Check") ; select warning limit
			ControlCommand("Select Error Handler(s)", "", 1158, "Check") ; select Bus Off
			ControlCommand("Select Error Handler(s)", "", 1159, "Check") ; select error Frame
			ControlClick("Select Error Handler(s)", "", 1176) ;click 'OK'
		EndIf
		$item1HWD = _GUICtrlTreeView_GetNext($nodeHWD, "")
		$item21HWD = _GUICtrlTreeView_GetNextSibling($nodeHWD, $item1HWD) ; Get handle of second item
		_GUICtrlTreeView_ClickItem($nodeHWD, $item21HWD)
		$item2HWD = _GUICtrlTreeView_GetNextSibling($nodeHWD, $item21HWD) ; Get handle of third item
		_GUICtrlTreeView_ClickItem($nodeHWD, $item2HWD)
		$item3HWD = _GUICtrlTreeView_GetNextSibling($nodeHWD, $item2HWD) ; Get handle of Fourth item
		_GUICtrlTreeView_ClickItem($nodeHWD, $item3HWD)
		$item4HWD = _GUICtrlTreeView_GetNextSibling($nodeHWD, $item3HWD) ; Get handle of fifth item
		_GUICtrlTreeView_ClickItem($nodeHWD, $item4HWD)
		$item5HWD = _GUICtrlTreeView_GetNextSibling($nodeHWD, $item4HWD) ; Get handle of sixth item
		_GUICtrlTreeView_ClickItem($nodeHWD, $item5HWD, "Right")
		Send("d") ; Delete error handlers
		Sleep(1000)
		If WinExists("Delete Error Handler(s)") Then
			ControlCommand("Delete Error Handler(s)", "", 1155, "UnCheck") ; Uncheck Error active
			ControlClick("Delete Error Handler(s)", "", 1176) ; click 'OK'
		EndIf
		$item1HWD = _GUICtrlTreeView_GetNext($nodeHWD, "")
		$item21HWD = _GUICtrlTreeView_GetNextSibling($nodeHWD, $item1HWD) ; Get handle of second item
		_GUICtrlTreeView_ClickItem($nodeHWD, $item21HWD)
		$item2HWD = _GUICtrlTreeView_GetNextSibling($nodeHWD, $item21HWD) ; Get handle of third item
		_GUICtrlTreeView_ClickItem($nodeHWD, $item2HWD)
		$item3HWD = _GUICtrlTreeView_GetNextSibling($nodeHWD, $item2HWD) ; Get handle of Fourth item
		_GUICtrlTreeView_ClickItem($nodeHWD, $item3HWD)
		$item4HWD = _GUICtrlTreeView_GetNextSibling($nodeHWD, $item3HWD) ; Get handle of fifth item
		_GUICtrlTreeView_ClickItem($nodeHWD, $item4HWD)
		$item5HWD = _GUICtrlTreeView_GetNextSibling($nodeHWD, $item4HWD) ; Get handle of sixth item
		_GUICtrlTreeView_ClickItem($nodeHWD, $item5HWD)
		$errHWD = _GUICtrlTreeView_GetFirstChild($nodeHWD, $item5HWD)
		_GUICtrlTreeView_ClickItem($nodeHWD, $errHWD, "Left", "", 2)
		Sleep(2000)
		ControlSend("BUSMASTER", "", 59664, "STCAN_MSG sMsg;" & @CRLF)
		ControlSend("BUSMASTER", "", 59664, "sMsg.m_unMsgID = 0x55;              // Message ID" & @CRLF)
		ControlSend("BUSMASTER", "", 59664, "sMsg.m_ucEXTENDED = FALSE;           // Standard Message type" & @CRLF)
		ControlSend("BUSMASTER", "", 59664, "sMsg.m_ucRTR = FALSE;                // Not RTR type" & @CRLF)
		ControlSend("BUSMASTER", "", 59664, "sMsg.m_ucDLC = 8;                    // Length is 8 Bytes" & @CRLF)
		ControlSend("BUSMASTER", "", 59664, "sMsg.m_sWhichBit.m_aulData[0] = 10;  // Lower 4 Bytes" & @CRLF)
		ControlSend("BUSMASTER", "", 59664, "sMsg.m_sWhichBit.m_aulData[1] = 20;  // Upper 4 Bytes" & @CRLF)
		ControlSend("BUSMASTER", "", 59664, "sMsg.m_ucChannel = 1;                // First CAN channel" & @CRLF)
		ControlSend("BUSMASTER", "", 59664, "SendMsg(sMsg);")
		Send("^s")
		$item1HWD = _GUICtrlTreeView_GetNext($nodeHWD, "")
		$item21HWD = _GUICtrlTreeView_GetNextSibling($nodeHWD, $item1HWD) ; Get handle of second item
		_GUICtrlTreeView_ClickItem($nodeHWD, $item21HWD)
		$item2HWD = _GUICtrlTreeView_GetNextSibling($nodeHWD, $item21HWD) ; Get handle of third item
		_GUICtrlTreeView_ClickItem($nodeHWD, $item2HWD)
		$item3HWD = _GUICtrlTreeView_GetNextSibling($nodeHWD, $item2HWD) ; Get handle of Fourth item
		_GUICtrlTreeView_ClickItem($nodeHWD, $item3HWD)
		$item4HWD = _GUICtrlTreeView_GetNextSibling($nodeHWD, $item3HWD) ; Get handle of fifth item
		_GUICtrlTreeView_ClickItem($nodeHWD, $item4HWD)
		$item5HWD = _GUICtrlTreeView_GetNextSibling($nodeHWD, $item4HWD) ; Get handle of sixth item
		_GUICtrlTreeView_ClickItem($nodeHWD, $item5HWD)
		$item6HWD = _GUICtrlTreeView_GetNextSibling($nodeHWD, $item5HWD) ; Get handle of seventh item
		_GUICtrlTreeView_ClickItem($nodeHWD, $item6HWD, "Right")
		Send("a") ; Add DLL handler
		Sleep(2000)
		If WinExists("Add DLL Handler(s)") Then
			ControlCommand("Add DLL Handler(s)", "", 1164, "Check") ; Check Load
			ControlCommand("Add DLL Handler(s)", "", 1165, "Check") ; Check Unload
			ControlClick("Add DLL Handler(s)", "", 1178) ; click 'OK'
		EndIf
		$item1HWD = _GUICtrlTreeView_GetNext($nodeHWD, "")
		$item21HWD = _GUICtrlTreeView_GetNextSibling($nodeHWD, $item1HWD) ; Get handle of second item
		_GUICtrlTreeView_ClickItem($nodeHWD, $item21HWD)
		$item2HWD = _GUICtrlTreeView_GetNextSibling($nodeHWD, $item21HWD) ; Get handle of third item
		_GUICtrlTreeView_ClickItem($nodeHWD, $item2HWD)
		$item3HWD = _GUICtrlTreeView_GetNextSibling($nodeHWD, $item2HWD) ; Get handle of Fourth item
		_GUICtrlTreeView_ClickItem($nodeHWD, $item3HWD)
		$item4HWD = _GUICtrlTreeView_GetNextSibling($nodeHWD, $item3HWD) ; Get handle of fifth item
		_GUICtrlTreeView_ClickItem($nodeHWD, $item4HWD)
		$item5HWD = _GUICtrlTreeView_GetNextSibling($nodeHWD, $item4HWD) ; Get handle of sixth item
		_GUICtrlTreeView_ClickItem($nodeHWD, $item5HWD)
		$item6HWD = _GUICtrlTreeView_GetNextSibling($nodeHWD, $item5HWD) ; Get handle of seventh item
		_GUICtrlTreeView_ClickItem($nodeHWD, $item6HWD)
		$dllHWD = _GUICtrlTreeView_GetFirstChild($nodeHWD, $item6HWD)
		_GUICtrlTreeView_ClickItem($nodeHWD, $dllHWD, "Left", "", 2)
		Sleep(2000)
		ControlSend("BUSMASTER", "", 59664, "Trace(" & Chr(34) & "DLL _Load" & Chr(34) & ");")
		Send("^s")
		$item1HWD = _GUICtrlTreeView_GetNext($nodeHWD, "")
		$item21HWD = _GUICtrlTreeView_GetNextSibling($nodeHWD, $item1HWD) ; Get handle of second item
		_GUICtrlTreeView_ClickItem($nodeHWD, $item21HWD)
		$item2HWD = _GUICtrlTreeView_GetNextSibling($nodeHWD, $item21HWD) ; Get handle of third item
		_GUICtrlTreeView_ClickItem($nodeHWD, $item2HWD)
		$item3HWD = _GUICtrlTreeView_GetNextSibling($nodeHWD, $item2HWD) ; Get handle of Fourth item
		_GUICtrlTreeView_ClickItem($nodeHWD, $item3HWD)
		$item4HWD = _GUICtrlTreeView_GetNextSibling($nodeHWD, $item3HWD) ; Get handle of fifth item
		_GUICtrlTreeView_ClickItem($nodeHWD, $item4HWD)
		$item5HWD = _GUICtrlTreeView_GetNextSibling($nodeHWD, $item4HWD) ; Get handle of sixth item
		_GUICtrlTreeView_ClickItem($nodeHWD, $item5HWD)
		$item6HWD = _GUICtrlTreeView_GetNextSibling($nodeHWD, $item5HWD) ; Get handle of seventh item
		_GUICtrlTreeView_ClickItem($nodeHWD, $item6HWD)
		$dllHWD = _GUICtrlTreeView_GetFirstChild($nodeHWD, $item6HWD)
		$undllHWD = _GUICtrlTreeView_GetNextChild($nodeHWD, $dllHWD)
		_GUICtrlTreeView_ClickItem($nodeHWD, $undllHWD, "Left", "", 2)
		Sleep(2000)
		ControlSend("BUSMASTER", "", 59664, "Trace(" & Chr(34) & "DLL _UnLoad" & Chr(34) & ");")
		Send("^s")
		$item1HWD = _GUICtrlTreeView_GetNext($nodeHWD, "")
		$item21HWD = _GUICtrlTreeView_GetNextSibling($nodeHWD, $item1HWD) ; Get handle of second item
		_GUICtrlTreeView_ClickItem($nodeHWD, $item21HWD)
		$item2HWD = _GUICtrlTreeView_GetNextSibling($nodeHWD, $item21HWD) ; Get handle of third item
		_GUICtrlTreeView_ClickItem($nodeHWD, $item2HWD)
		$item3HWD = _GUICtrlTreeView_GetNextSibling($nodeHWD, $item2HWD) ; Get handle of Fourth item
		_GUICtrlTreeView_ClickItem($nodeHWD, $item3HWD)
		$item4HWD = _GUICtrlTreeView_GetNextSibling($nodeHWD, $item3HWD) ; Get handle of fifth item
		_GUICtrlTreeView_ClickItem($nodeHWD, $item4HWD)
		$item5HWD = _GUICtrlTreeView_GetNextSibling($nodeHWD, $item4HWD) ; Get handle of sixth item
		_GUICtrlTreeView_ClickItem($nodeHWD, $item5HWD)
		$item6HWD = _GUICtrlTreeView_GetNextSibling($nodeHWD, $item5HWD) ; Get handle of seventh item
		_GUICtrlTreeView_ClickItem($nodeHWD, $item6HWD)
		$item7HWD = _GUICtrlTreeView_GetNextSibling($nodeHWD, $item6HWD) ; Get handle of eight item
		_GUICtrlTreeView_ClickItem($nodeHWD, $item7HWD, "Right")
		Send("a") ; add a utility handler
		Sleep(1000)
		If WinExists("Add Utility Function") Then
			ControlCommand("Add Utility Function", "", 1029, "SelectString", "int")
			ControlSetText("Add Utility Function", "", 1030, "_Addint x, int y)") ; check for validation
			Send("{ENTER}")
			If WinExists("BUSMASTER") Then
				Send("{ENTER}")
			EndIf
			ControlSetText("Add Utility Function", "", 1030, "_Add(int x, int y") ; check for validation
			Send("{ENTER}")
			If WinExists("BUSMASTER") Then
				Send("{ENTER}")
			EndIf
			ControlSetText("Add Utility Function", "", 1030, "_Add(int x, int y)") ; enter function name
			Send("{ENTER}")
		EndIf
		$item1HWD = _GUICtrlTreeView_GetNext($nodeHWD, "")
		$item21HWD = _GUICtrlTreeView_GetNextSibling($nodeHWD, $item1HWD) ; Get handle of second item
		_GUICtrlTreeView_ClickItem($nodeHWD, $item21HWD)
		$item2HWD = _GUICtrlTreeView_GetNextSibling($nodeHWD, $item21HWD) ; Get handle of third item
		_GUICtrlTreeView_ClickItem($nodeHWD, $item2HWD)
		$item3HWD = _GUICtrlTreeView_GetNextSibling($nodeHWD, $item2HWD) ; Get handle of Fourth item
		_GUICtrlTreeView_ClickItem($nodeHWD, $item3HWD)
		$item4HWD = _GUICtrlTreeView_GetNextSibling($nodeHWD, $item3HWD) ; Get handle of fifth item
		_GUICtrlTreeView_ClickItem($nodeHWD, $item4HWD)
		$item5HWD = _GUICtrlTreeView_GetNextSibling($nodeHWD, $item4HWD) ; Get handle of sixth item
		_GUICtrlTreeView_ClickItem($nodeHWD, $item5HWD)
		$item6HWD = _GUICtrlTreeView_GetNextSibling($nodeHWD, $item5HWD) ; Get handle of seventh item
		_GUICtrlTreeView_ClickItem($nodeHWD, $item6HWD)
		$item7HWD = _GUICtrlTreeView_GetNextSibling($nodeHWD, $item6HWD) ; Get handle of eight item
		_GUICtrlTreeView_ClickItem($nodeHWD, $item7HWD)
		$utilHWD = _GUICtrlTreeView_GetFirstChild($nodeHWD, $item7HWD)
		_GUICtrlTreeView_ClickItem($nodeHWD, $utilHWD, "Left", "", 2)
		Sleep(2000)
		ControlSend("BUSMASTER", "", 59664, "int sum = 0;" & @CRLF) ; enter the function
		ControlSend("BUSMASTER", "", 59664, "sum = x+y;", 1 & @CRLF)
		ControlSend("BUSMASTER", "", 59664, @CRLF)
		ControlSend("BUSMASTER", "", 59664, "return sum;" & @CRLF)
		Send("^s")
		$item1HWD = _GUICtrlTreeView_GetNext($nodeHWD, "")
		$item21HWD = _GUICtrlTreeView_GetNextSibling($nodeHWD, $item1HWD) ; Get handle of second item
		_GUICtrlTreeView_ClickItem($nodeHWD, $item21HWD)
		$item2HWD = _GUICtrlTreeView_GetNextSibling($nodeHWD, $item21HWD) ; Get handle of third item
		_GUICtrlTreeView_ClickItem($nodeHWD, $item2HWD)
		$item3HWD = _GUICtrlTreeView_GetNextSibling($nodeHWD, $item2HWD) ; Get handle of Fourth item
		_GUICtrlTreeView_ClickItem($nodeHWD, $item3HWD)
		$item4HWD = _GUICtrlTreeView_GetNextSibling($nodeHWD, $item3HWD) ; Get handle of fifth item
		_GUICtrlTreeView_ClickItem($nodeHWD, $item4HWD)
		$item5HWD = _GUICtrlTreeView_GetNextSibling($nodeHWD, $item4HWD) ; Get handle of sixth item
		_GUICtrlTreeView_ClickItem($nodeHWD, $item5HWD)
		$item6HWD = _GUICtrlTreeView_GetNextSibling($nodeHWD, $item5HWD) ; Get handle of seventh item
		_GUICtrlTreeView_ClickItem($nodeHWD, $item6HWD)
		$item7HWD = _GUICtrlTreeView_GetNextSibling($nodeHWD, $item6HWD) ; Get handle of eight item
		_GUICtrlTreeView_ClickItem($nodeHWD, $item7HWD)
		$item8HWD = _GUICtrlTreeView_GetNextSibling($nodeHWD, $item7HWD) ; Get handle of ninth item
		_GUICtrlTreeView_ClickItem($nodeHWD, $item8HWD, "Left", "", 2) ; Double click on the Global Variable Node
		ControlSend("BUSMASTER", "", 59664, "int x;" & @CRLF) ; enter the function
		ControlSend("BUSMASTER", "", 59664, "int y;", 1 & @CRLF)
		Sleep(1000)
		Send("^s")
		Sleep(1000)
		$item1HWD = _GUICtrlTreeView_GetNext($nodeHWD, "")
		$item21HWD = _GUICtrlTreeView_GetNextSibling($nodeHWD, $item1HWD) ; Get handle of second item
		_GUICtrlTreeView_ClickItem($nodeHWD, $item21HWD)
		$item2HWD = _GUICtrlTreeView_GetNextSibling($nodeHWD, $item21HWD) ; Get handle of third item
		$MSG1HWD = _GUICtrlTreeView_GetFirstChild($nodeHWD, $item2HWD)
		_GUICtrlTreeView_ClickItem($nodeHWD, $MSG1HWD, "Left", "", 2) ; Click on First Header
		Sleep(1000)
		$editPos = ControlGetPos("BUSMASTER", "", 59664) ; Get position of richEdit control
		MouseClick("right", $editPos[0] + 30, $editPos[1] + 55)
		Send("f") ; Select 'insert function'
		Sleep(1000)
		If WinExists("Insert Function") Then
			$funcLBHWD = ControlGetHandle("Insert Function", "", 1068) ; Get handle of the list box
			_GUICtrlListBox_ClickItem($funcLBHWD, 0) ; Click on the 1st item
			Send("{ENTER}")
		EndIf
		MouseClick("right", $editPos[0] + 30, $editPos[1] + 55)
		Send("m") ; Select 'insert message'
		Sleep(1000)
		If WinExists("Dialog") Then
			ControlCommand("Dialog", "", 1028, "Check") ; Check the checkbox
			Send("{ENTER}") ; Click on select button
		EndIf
		MouseClick("right", $editPos[0] + 30, $editPos[1] + 55)
		Send("s") ; Select 'insert signal'
		Sleep(1000)
		If WinExists("Dialog") Then
			ControlCommand("Dialog", "", 1028, "Check") ; Check the checkbox
			Send("{ENTER}") ; Click on select button
		EndIf
		Send("^s")
		Send("^a")
		Send("{DELETE}")
		Send("^s")
		Send("^{F4}") ; Close the C file window
		ControlClick("BUSMASTER", "Edit File...", 1002) ; click edit file
		sleep(2000)
		Send("^{F4}") ; Close C file window
		Sleep(500)
		ControlClick("BUSMASTER", "Build...", 1003) ; click Build
		Sleep(500)
		ControlClick("BUSMASTER", "Build and Load", 1004) ; click Build and load
		Sleep(500)
		ControlClick("BUSMASTER", "UnLoad", 1005) ; Unload
		Sleep(500)
		ControlClick("BUSMASTER", "Build and Load", 1004) ; click Build and load
		Sleep(500)
		ControlClick("BUSMASTER", "Enable Handler", 1014) ; click Enable Handler
		Sleep(500)
		ControlClick("BUSMASTER", "Enable All Handlers", 1015) ; click Enable All Handler
		Sleep(500)
		ControlClick("BUSMASTER", "Disable Handler", 1014) ; click Disable Handler
		Sleep(500)
		ControlClick("BUSMASTER", "Disable All Handlers", 1015) ; click Disable All Handler
		Sleep(1000)
		ControlClick("BUSMASTER", "Enable All Handlers", 1015) ; click Enable All andler
	EndIf
	Send("{F2}") ; Connect
	Send("a")
	Sleep(6000)
	$cntToolhWd = ControlGetHandle("BUSMASTER", "", 128)
	_GUICtrlToolbar_ClickIndex($cntToolhWd, 4) ; Disconnect
	Sleep(1000)
	ControlClick("BUSMASTER", "Disable All Handlers", 1015) ; click Disable All Handler
	$hdlDetailsHWD = ControlGetHandle("BUSMASTER", "", 1226)
	_GUICtrlListView_ClickItem($hdlDetailsHWD, 0) ; Click 1st item in the 'Handler Details' list view
	Sleep(500)
	ControlClick("BUSMASTER", "Enable Handler", 1014) ; click Enable Handler
	Sleep(500)
	Send("{F2}") ; Connect
	Sleep(3000)
	_GUICtrlToolbar_ClickIndex($cntToolhWd, 4) ; Disconnect
	Sleep(500)
	_GUICtrlListView_ClickItem($hdlDetailsHWD, 0) ; Click 1st item in the 'Handler Details' list view
	Sleep(1000)
	ControlClick("BUSMASTER", "Disable Handler", 1014) ; click Disable Handler
	Sleep(1000)
	_GUICtrlListView_ClickItem($hdlDetailsHWD, 1) ; Click 2nd item in the 'Handler Details' list view
	Sleep(500)
	ControlClick("BUSMASTER", "Enable Handler", 1014) ; click Enable Handler
	Sleep(500)
	Send("{F2}") ; Connect
	Sleep(3000)
	_GUICtrlToolbar_ClickIndex($cntToolhWd, 4) ; Disconnect
	Sleep(500)
	_GUICtrlListView_ClickItem($hdlDetailsHWD, 1) ; Click 2st item in the 'Handler Details' list view
	Sleep(1000)
	ControlClick("BUSMASTER", "Disable Handler", 1014) ; click Disable Handler
	Sleep(1000)
	_GUICtrlListView_ClickItem($hdlDetailsHWD, 2) ; Click 3rd item in the 'Handler Details' list view
	Sleep(500)
	ControlClick("BUSMASTER", "Enable Handler", 1014) ; click Enable Handler
	Sleep(500)
	Send("{F2}") ; Connect
	Sleep(1000)
	Send("a")
	Sleep(3000)
	_GUICtrlToolbar_ClickIndex($cntToolhWd, 4) ; Disconnect
	Sleep(500)
	_GUICtrlListView_ClickItem($hdlDetailsHWD, 2) ; Click 3rd item in the 'Handler Details' list view
	Sleep(500)
	ControlClick("BUSMASTER", "Disable Handler", 1014) ; click Disable Handler
	Sleep(1000)
	_GUICtrlListView_ClickItem($hdlDetailsHWD, 3) ; Click 4th item in the 'Handler Details' list view
	Sleep(500)
	ControlClick("BUSMASTER", "Enable Handler", 1014) ; click Enable Handler
	Sleep(500)
	Send("{F2}") ; Connect
	Sleep(3000)
	_GUICtrlToolbar_ClickIndex($cntToolhWd, 4) ; Disconnect
	Sleep(500)
	_GUICtrlListView_ClickItem($hdlDetailsHWD, 3) ; Click 4th item in the 'Handler Details' list view
	Sleep(500)
	ControlClick("BUSMASTER", "Disable Handler", 1014) ; click Disable Handler
	Sleep(1000)
	$node1HWD = ControlGetHandle("BUSMASTER", "", "[CLASS:SysTreeView32; INSTANCE:1]") ; Get tree view control handle
	$item1HWD = _GUICtrlTreeView_GetNext($node1HWD, "")
	_GUICtrlTreeView_ClickItem($node1HWD, $item1HWD, "Right") ; Right click on sim file
	Sleep(1000)
	Send("b") ; Select 'Build All' from the context menu
	Sleep(1000)
	$item1HWD = _GUICtrlTreeView_GetNext($node1HWD, "")
	_GUICtrlTreeView_ClickItem($node1HWD, $item1HWD, "Right") ; Right click on sim file
	Sleep(1000)
	Send("l") ; Select 'Load All' from the context menu
	Sleep(1000)
	$item1HWD = _GUICtrlTreeView_GetNext($node1HWD, "")
	_GUICtrlTreeView_ClickItem($node1HWD, $item1HWD, "Right") ; Right click on sim file
	Sleep(1000)
	Send("u") ; Select 'Unload All' from the context menu
	Sleep(1000)
	$item1HWD = _GUICtrlTreeView_GetNext($node1HWD, "")
	_GUICtrlTreeView_ClickItem($node1HWD, $item1HWD, "Right") ; Right click on sim file
	Sleep(1000)
	Send("l") ; Select 'Load All' from the context menu
	Sleep(1000)
	$item1HWD = _GUICtrlTreeView_GetNext($node1HWD, "")
	_GUICtrlTreeView_ClickItem($node1HWD, $item1HWD, "Right") ; Right click on sim file
	Sleep(1000)
	Send("r") ; Select 'Remove All Load' from the context menu
	Sleep(1000)
	If WinExists("BUSMASTER") Then
		Send("{ENTER}") ; Click 'OK' fromd the unload dll error pop up
	EndIf
	$item1HWD = _GUICtrlTreeView_GetNext($node1HWD, "")
	_GUICtrlTreeView_ClickItem($node1HWD, $item1HWD, "Right") ; Right click on sim file
	Sleep(1000)
	Send("u") ; Select 'Unload All' from the context menu
	Sleep(1000)
	$item1HWD = _GUICtrlTreeView_GetNext($node1HWD, "")
	_GUICtrlTreeView_ClickItem($node1HWD, $item1HWD, "Right") ; Right click on sim file
	Sleep(1000)
	Send("b") ; Select 'Build All' from the context menu
	Sleep(1000)
	_GUICtrlToolbar_ClickIndex($cntToolhWd, 4) ; Connect
	$item1HWD = _GUICtrlTreeView_GetNext($node1HWD, "")
	_GUICtrlTreeView_ClickItem($node1HWD, $item1HWD, "Right") ; Right click on sim file
	Sleep(1000)
	Send("l") ; Select 'Load All' from the context menu
	Sleep(1000)
	$prntHWD = _GUICtrlTreeView_GetItemHandle($node1HWD, 0)
	_GUICtrlTreeView_ClickItem($node1HWD, $prntHWD, "Right") ; Right click on First parent itemr
	Sleep(1000)
	Send("d") ; Select 'Delete All Systems' from context menu
	Sleep(1000)
	If WinExists("BUSMASTER") Then
		Send("{ENTER}") ; Click 'OK' from the unload dll error pop up
	EndIf
	Sleep(1000)
	Send("!fgs") ; Save Config
EndIf
