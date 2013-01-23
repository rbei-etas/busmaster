;=== Test Cases/Test Data ===
; Module			:		Logging
; Critical (C)		:		Y
; TestCase No.		:		TS_Log_03
; TestCases			:		Functionality test - Log Filters
; Test Strategy		:		Black Box
; Test Data			:		-
; === Test Procedure ===

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_Log_03.au3****"&@CRLF)


_launchApp()

if winexists($mWin) then
	_createConfig("cfxLog_03")												; Create New Configuration

	_createCANDB("testLog_03")												; Create New Database File
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

	_AssociateCANDB("testLog_03.dbf")										; Associate DB

	_hdWareSelectKvaser()														; Select Kvaser Hardware
	 sleep(1000)

	_TxMsgMenu()																; Open Tx window from the menu

	_AddMsgBlock()																; Add a msg block

	_EnableAutoUpdate()															; Enable autoupdate

	ControlCommand($mWin,"",$msgIDNameComboctrlID,"SetCurrentSelection",0)		; Add the DB msgs to the Tx message list

	_AddMsg2TxList()															; Click on Add button

	_CloseTxWindow()															; Close Tx window

	_AppFilterMenu()															; Open Configure Filter dialog

	_AddFilter("Stop","[0x15]Msg1","All","All","All","All")						; Configure Filter details
	sleep(1000)

	_ConfigureCANLog("Log_05_Sample","System","ALL","Decimal","Overwrite","","","True")	; Configure Logging details

	_ConnectDisconnect()															; Connect the tool

	_TransmitMsgsMenu()																; Transmit normal blocks
	sleep(3000)

	_EnableFilterLog()																; Enable Filter during logging
	sleep(1500)

	_ConnectDisconnect()															; Disconnect the tool

	Sleep(2000)
	_DisableJ1939Win()																; If J1939 Msg Window is active then disable it

	_DisableOverwriteMode()															; Disable Overwrite Mode
	Sleep(2000)
EndIf
	$txt=_FileCountLines ("D:\BUSMASTERTestData_Autoit\Log_05_Sample.log")
	$LastMsg=FileReadLine("D:\BUSMASTERTestData_Autoit\Log_05_Sample.log",$txt-2)
	$a=stringsplit($LastMsg," ")
	ConsoleWrite("$a[1] : " &$a[1] & @CRLF)
	$totalMsgCount=$txt-2-15
	ConsoleWrite("$totalMsgCount : " & $totalMsgCount & @CRLF)
	$hWd=ControlGetHandle($mWin,"",$msgWinlv)
	$fromMsgWin=_GUICtrlListView_GetItemTextString($hWd,$totalMsgCount-1)
	ConsoleWrite("$a[1] : " & $a[1] & @CRLF)
	ConsoleWrite("$fromMsgWin : " & $fromMsgWin & @CRLF)
	$text=stringsplit($fromMsgWin,"|")
	ConsoleWrite("$text[2]: " &$text[2] & @CRLF)
	$LastMsgWin=_GUICtrlListView_GetItemTextString($hWd,$totalMsgCount)
	ConsoleWrite("$LastMsgWin :" & $LastMsgWin & @CRLF)
	$LastMsgTime=stringsplit($LastMsgWin,"|")
	ConsoleWrite("$LastMsgTime[2] :" &$LastMsgTime[2] & @CRLF)
	if $a[1]=$text[2] and $LastMsgTime[2]<>"" then
		_ExcelWriteCell($oExcel, "Pass", 15, 2)
	Else
		_ExcelWriteCell($oExcel, "Fail", 15, 2)
	EndIf

ConsoleWrite("****End : TS_Log_03.au3****"&@CRLF)
ConsoleWrite(@CRLF)