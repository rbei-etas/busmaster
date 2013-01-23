;=== Test Cases/Test Data ===
; Module			:		Filters
; Critical (C)		:		Y
; TestCase No.		:		TS_Filter_06
; TestCases			:		Check for Pass filter mode - Message filter
; Test Strategy		:		Black Box
; Test Data			:		-
; === Test Procedure ===

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_Filter_06.au3****"&@CRLF)

_launchApp()																; Check if the App is open else launch it

if winexists($mWin) then
	_createConfig("cfxFilter_06")												; Create New Configuration

	_createCANDB("testFilter_06")												; Create New Database File
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

	_AssociateCANDB("testFilter_06.dbf")										; Associate DB

	_hdWareSelectKvaser()														; Select Kvaser Hardware
	 sleep(1000)

	_TxMsgMenu()																; Open Tx window from the menu

	_AddMsgBlock()																; Add a msg block

	_EnableAutoUpdate()															; Enable autoupdate

	ControlCommand($mWin,"",$msgIDNameComboctrlID,"SetCurrentSelection",0)		; Add the DB msgs to the Tx message list

	_AddMsg2TxList()															; Click on Add button

	_CloseTxWindow()															; Close Tx window

	_AppFilterMenu()															; Open Configure Filter dialog

	_AddFilter("Pass","[0x15]Msg1","Std","Non RTR","Tx",1)						; Configure Filter details
	sleep(1000)
	_MsgDisplayMenu()															; Open Message Display dialog

	_AddFiltertoMsgDisp()														; Add the filter to Message display

	_Enable_DisableFilterDispMenu()												; Enable filters for message display

	_ConfigureCANLog("Log_Filter_06","System","ALL","Decimal","Append","","","True")	; Configure Logging details

	_EnableFilterLog()																; Enable Filter during logging
	sleep(1500)

	_ConnectDisconnect()															; Connect the tool

	_TransmitMsgsMenu()																; Transmit normal blocks
	sleep(3000)

	_ConnectDisconnect()															; Disconnect the tool

	Sleep(2000)
	_DisableJ1939Win()																; If J1939 Msg Window is active then disable it

	_DisableOverwriteMode()															; Disable Overwrite Mode
	Sleep(2000)


	$hwd= ControlGetHandle($mWin,"",$msgWinlv)										; Fetch the Msg Window Handle
	$MsgEntryTx=_GUICtrlListView_GetItemTextString($hwd,0)							; Fetch the Msg window first item text
	$MsgEntryRx=_GUICtrlListView_GetItemText($hwd,1,2)								; Fetch the Msg window second item text
	ConsoleWrite("$MsgEntryTx:"&$MsgEntryTx&@CRLF)
	ConsoleWrite("$MsgEntryRx:"&$MsgEntryRx&@CRLF)
	$AfterTrim_MsgEntryTx=StringTrimLeft($MsgEntryTx,1)								; Remove the first character
	$AfterRepl_MsgEntryTx=StringReplace($AfterTrim_MsgEntryTx,"|"," ")				; Replace "|" with " "(Space)
	ConsoleWrite("$AfterRepl_MsgEntryRx:"&$AfterRepl_MsgEntryTx&@CRLF)
	$MsgWinTrim=StringMid ($AfterRepl_MsgEntryTx,1,18)								; Extract the first 18 charater from the trimmed string
	$GetLogFile_Path=_SetOneFolderUp()												; Fetch the Log file path
	ConsoleWrite("$GetLogFile_Path:"&$GetLogFile_Path&@CRLF)
	$Read_LogFirstEnter=FileReadLine ($GetLogFile_Path & "\Log_Filter_06.log",16)		; Read the 16th line from the Log file
	consolewrite("@error :"&@error&@CRLF)
	consolewrite("$Read_LogFirstEnter:"&$Read_LogFirstEnter&@CRLF)
	$LogTrim=StringMid ($Read_LogFirstEnter,1,18)													; Extract the first 18 charater
	ConsoleWrite("$MsgWinTrim	: "&$MsgWinTrim&@CRLF)
	ConsoleWrite("$LogTrim	: "&$LogTrim&@CRLF)
EndIf

if $MsgWinTrim=$LogTrim and $MsgEntryRx="Tx" Then											; Validate the result
	_ExcelWriteCell($oExcel, "Pass", 11, 2)
Else
	_ExcelWriteCell($oExcel, "Fail", 11, 2)
EndIf

ConsoleWrite("****End : TS_Filter_06.au3****"&@CRLF)
ConsoleWrite(@CRLF)