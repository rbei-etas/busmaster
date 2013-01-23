;=== Test Cases/Test Data ===
; Module			:		Filters
; Critical (C)		:		Y
; TestCase No.		:		TS_Filter_01
; TestCases			:		Provision to enter message ID for filtering
; Test Strategy		:		Black Box
; Test Data			:		-
; === Test Procedure ===

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("***********Filter Script Execution Started************"&@CRLF)
ConsoleWrite("****Start : TS_Filter_01.au3****"&@CRLF)

WinActivate($mWin,3)
if winexists($mWin) then
	_createConfig("cfxFilter_01")												; Create New Configuration

	_createCANDB("testFilter_01")												; Create New Database File
	sleep(1500)

	_DBmessage("n")																; Select 'New Message' from right click menu
	sleep(1000)

	_addMsgDetails("Msg1",15,8)													; Add the message details

	$sigDetlvPos=ControlGetPos($mWin,"",$sigDetlvInst1)						; Get handle of signal details list view
	MouseClick("right",$sigDetlvPos[0]+150,$sigDetlvPos[1]+100)				; Right Click on the Signal details list view
	send("{DOWN}")															; Select 'New Signal' menu
	sleep(1500)
	send("{ENTER}")

	_addSigDetails("int","Signal1",32,0,0)										; Add the signal details

	_saveCloseCANDB()

	_AssociateCANDB("testFilter_01.dbf")										; Associate DB

	_hdWareSelectKvaser()														; Select Kvaser Hardware
	 sleep(1000)

	_TxMsgMenu()																; Open Tx window from the menu

	_AddMsgBlock()																; Add a msg block

	_EnableAutoUpdate()															; Enable autoupdate

	ControlCommand($mWin,"",$msgIDNameComboctrlID,"SetCurrentSelection",0)		; Add the DB msgs to the Tx message list

	_AddMsg2TxList()															; Click on Add button

	_CloseTxWindow()															; Close Tx window

	_AppFilterMenu()															; Open Configure Filter dialog

	_AddFilter("Stop","[0x15]Msg1","Std","Non RTR","Tx",1)						; Configure Filter details
	sleep(1000)
	_MsgDisplayMenu()															; Open Message Display dialog

	_AddFiltertoMsgDisp()														; Add the filter to Message display

	_Enable_DisableFilterDispMenu()											; Enable filters for message display

	_ConnectDisconnect()														; Connect the tool

	_TransmitMsgsMenu()															; Transmit normal blocks
	sleep(3000)

	_ConnectDisconnect()														; Disconnect the tool

	Sleep(2000)
	_DisableJ1939Win()															; If J1939 Msg Window is active then disable it

	sleep(2000)
	$hwd= ControlGetHandle($mWin,"",$msgWinlv)									; Fetch handle of Msg window list view
	$count=_GUICtrlListView_GetItemCount($hwd)									; Fetch the number of items in the list view
	ConsoleWrite("$count=" &$count&@CRLF)
	$dir=_GUICtrlListView_GetItemText($hwd, 0,2)								; Fetch the direction of the msg
	ConsoleWrite("$dir=" &$dir&@CRLF)
	$decMsgID=_GUICtrlListView_GetItemText($hwd, 0,5)							; Fetch the Dec msg ID
	ConsoleWrite("$decMsgID=" &$decMsgID&@CRLF)
	$hexVal=hex($decMsgID)														; Convert the Dec msg ID to Hex
	ConsoleWrite("$hexVal=" &$hexVal&@CRLF)
EndIf

if $count=1 and $dir="Rx" and $hexVal=00000015 Then								; Validate the result
	_ExcelWriteCell($oExcel, "Pass", 6, 2)
Else
	_ExcelWriteCell($oExcel, "Fail", 6, 2)
EndIf

ConsoleWrite("****End : TS_Filter_01.au3****"&@CRLF)
ConsoleWrite(@CRLF)