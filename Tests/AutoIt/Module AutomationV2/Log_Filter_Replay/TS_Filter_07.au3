;=== Test Cases/Test Data ===
; Module			:		Filters
; Critical (C)		:		Y
; TestCase No.		:		TS_Filter_07
; TestCases			:		Applying display Filter
; Test Strategy		:		Black Box
; Test Data			:		-
; === Test Procedure ===

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_Filter_07.au3****"&@CRLF)

Dim $item[500]
_launchApp()																	; Check if the App is open else launch it

if winexists($WIN_BUSMASTER) then

	_loadConfig("cfxFilter_01")													; Load Cfx file

	_ConnectDisconnect()														; Connect the tool

	_TransmitMsgsMenu()															; Transmit normal blocks
	sleep(3000)

	_ConnectDisconnect()														; Disconnect the tool
	Sleep(2000)

	_EnableOverwriteMode()
	sleep(1000)

	_DisableJ1939Win()															; If J1939 Msg Window is active then disable it
	sleep(2000)

	$hwd= ControlGetHandle($WIN_BUSMASTER,"",$LSTC_CANMsgWin)									; Fetch handle of Msg window list view
	$count=_GUICtrlListView_GetItemCount($hwd)									; Fetch the number of items in the list view
	ConsoleWrite("$count=" &$count&@CRLF)
	$dir1=_GUICtrlListView_ClickItem($hwd,0)									; Select the first column
	ConsoleWrite("$dir1=" &$dir1&@CRLF)
	$txt=_GUICtrlListView_GetItemTextString($hwd,-1)							; Fetch the text of the selected item
	ConsoleWrite("$txt=" &$txt&@CRLF)
	$item=StringSplit ($txt,"|")												; Split the txt to extract direction, MsgID
	ConsoleWrite("$dir: " & $item[3] &@CRLF)
	ConsoleWrite("MsgID: " & $item[6] &@CRLF)
	$hexVal=hex($item[6])														; Convert the Dec msg ID to Hex
	ConsoleWrite("$hexVal=" &$hexVal&@CRLF)

	if $count=2 and $item[3]="Tx" and $hexVal=00000015 Then								; Validate the result
		$withoutMsgFilter="True"
	Else
		$withoutMsgFilter="False"
	EndIf


	_Enable_DisableFilterDispMenu()												; Enable Message Filter for display

	_ConnectDisconnect()														; Connect the tool

	_TransmitMsgsMenu()															; Transmit normal blocks
	sleep(3000)

	_ConnectDisconnect()														; Disconnect the tool
	Sleep(2000)

	_DisableJ1939Win()															; If J1939 Msg Window is active then disable it
	sleep(2000)

	$hwd= ControlGetHandle($WIN_BUSMASTER,"",$LSTC_CANMsgWin)									; Fetch handle of Msg window list view
	$count=_GUICtrlListView_GetItemCount($hwd)									; Fetch the number of items in the list view
	ConsoleWrite("$count=" &$count&@CRLF)
	$dir=_GUICtrlListView_GetItemText($hwd, 0,2)								; Fetch the direction of the msg
	ConsoleWrite("$dir=" &$dir&@CRLF)
	$decMsgID=_GUICtrlListView_GetItemText($hwd, 0,5)							; Fetch the Dec msg ID
	ConsoleWrite("$decMsgID=" &$decMsgID&@CRLF)
	$hexVal=hex($decMsgID)														; Convert the Dec msg ID to Hex
	ConsoleWrite("$hexVal=" &$hexVal&@CRLF)

	if $count=1 and $dir="Rx" and $hexVal=00000015 Then							; Validate the result
		$withMsgFilter="True"
	Else
		$withMsgFilter="False"
	EndIf

EndIf

ConsoleWrite("$withoutMsgFilter=" &$withoutMsgFilter&@CRLF)
ConsoleWrite("$withMsgFilter=" &$withMsgFilter&@CRLF)

if $withoutMsgFilter="True" and $withMsgFilter="True" Then
	_ExcelWriteCell($oExcel, "Pass", 12, 2)
Else
	_ExcelWriteCell($oExcel, "Fail", 12, 2)
EndIf


ConsoleWrite("****End : TS_Filter_07.au3****"&@CRLF)
ConsoleWrite(@CRLF)


