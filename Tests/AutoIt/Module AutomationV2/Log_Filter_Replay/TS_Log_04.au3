;=== Test Cases/Test Data ===
; Module			:		Logging
; Critical (C)		:		Y
; TestCase No.		:		TS_Log_04
; TestCases			:		Functionality test - Trigger
; Test Strategy		:		Black Box
; Test Data			:		-
; === Test Procedure ===

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_Log_04.au3****"&@CRLF)

_launchApp()
if winexists($WIN_BUSMASTER) then
	_createConfig("cfxLog_04")												; Create New Configuration

	_createCANDB("testLog_04")												; Create New Database File
	sleep(1500)

	_DBmessage("n")																; Select 'New Message' from right click menu
	sleep(1000)

	_addMsgDetails("Msg1",50,8)													; Add the message details

	$sigDetlvPos=ControlGetPos($WIN_BUSMASTER,"",$LVC_SigDet_DBeditor)						; Get handle of signal details list view
	MouseClick("right",$sigDetlvPos[0]+150,$sigDetlvPos[1]+100)				; Right Click on the Signal details list view
	send("{DOWN}")															; Select 'New Signal' menu
	sleep(1500)
	send("{ENTER}")

	_addSigDetails("int","Signal1",64,0,0)										; Add the signal details

	_DBmessage("n")																; Select 'New Message' from right click menu
	sleep(1000)

	_addMsgDetails("Msg2",100,8)													; Add the message details

	$sigDetlvPos=ControlGetPos($WIN_BUSMASTER,"",$LVC_SigDet_DBeditor)						; Get handle of signal details list view
	MouseClick("right",$sigDetlvPos[0]+150,$sigDetlvPos[1]+100)				; Right Click on the Signal details list view
	send("{DOWN}")															; Select 'New Signal' menu
	sleep(1500)
	send("{ENTER}")

	_addSigDetails("int","Signal2",64,0,0)										; Add the signal details

	_DBmessage("n")																; Select 'New Message' from right click menu
	sleep(1000)

	_addMsgDetails("Msg3",150,8)													; Add the message details

	$sigDetlvPos=ControlGetPos($WIN_BUSMASTER,"",$LVC_SigDet_DBeditor)						; Get handle of signal details list view
	MouseClick("right",$sigDetlvPos[0]+150,$sigDetlvPos[1]+100)				; Right Click on the Signal details list view
	send("{DOWN}")															; Select 'New Signal' menu
	sleep(1500)
	send("{ENTER}")

	_addSigDetails("int","Signal3",64,0,0)										; Add the signal details


	_DBmessage("n")																; Select 'New Message' from right click menu
	sleep(1000)

	_addMsgDetails("Msg4",200,8)													; Add the message details

	$sigDetlvPos=ControlGetPos($WIN_BUSMASTER,"",$LVC_SigDet_DBeditor)						; Get handle of signal details list view
	MouseClick("right",$sigDetlvPos[0]+150,$sigDetlvPos[1]+100)				; Right Click on the Signal details list view
	send("{DOWN}")															; Select 'New Signal' menu
	sleep(1500)
	send("{ENTER}")

	_addSigDetails("int","Signal4",64,0,0)										; Add the signal details

	_DBmessage("n")																; Select 'New Message' from right click menu
	sleep(1000)

	_addMsgDetails("Msg5",250,8)													; Add the message details

	$sigDetlvPos=ControlGetPos($WIN_BUSMASTER,"",$LVC_SigDet_DBeditor)						; Get handle of signal details list view
	MouseClick("right",$sigDetlvPos[0]+150,$sigDetlvPos[1]+100)				; Right Click on the Signal details list view
	send("{DOWN}")															; Select 'New Signal' menu
	sleep(1500)
	send("{ENTER}")

	_addSigDetails("int","Signal5",64,0,0)										; Add the signal details

	_DBmessage("n")																; Select 'New Message' from right click menu
	sleep(1000)

	_addMsgDetails("Msg6",300,8)													; Add the message details

	$sigDetlvPos=ControlGetPos($WIN_BUSMASTER,"",$LVC_SigDet_DBeditor)						; Get handle of signal details list view
	MouseClick("right",$sigDetlvPos[0]+150,$sigDetlvPos[1]+100)				; Right Click on the Signal details list view
	send("{DOWN}")															; Select 'New Signal' menu
	sleep(1500)
	send("{ENTER}")

	_addSigDetails("int","Signal6",64,0,0)										; Add the signal details

	_saveCloseCANDB()

	_AssociateCANDB("testLog_04.dbf")										; Associate DB

	_hdWareSelectKvaser()														; Select Kvaser Hardware
	 sleep(1000)

	_TxMsgMenu()																; Open Tx window from the menu

	_AddMsgBlock()																; Add a msg block

	_EnableAutoUpdate()															; Enable autoupdate

	ControlCommand($WIN_BUSMASTER,"",$COMB_MsgID_ConfigTX,"SetCurrentSelection",0)		; Add the DB msgs to the Tx message list

	_AddMsg2TxList()															; Click on Add button

	ControlCommand($WIN_BUSMASTER,"",$COMB_MsgID_ConfigTX,"SetCurrentSelection",1)		; Add the DB msgs to the Tx message list

	_AddMsg2TxList()															; Click on Add button

	ControlCommand($WIN_BUSMASTER,"",$COMB_MsgID_ConfigTX,"SetCurrentSelection",2)		; Add the DB msgs to the Tx message list

	_AddMsg2TxList()															; Click on Add button

	ControlCommand($WIN_BUSMASTER,"",$COMB_MsgID_ConfigTX,"SetCurrentSelection",3)		; Add the DB msgs to the Tx message list

	_AddMsg2TxList()															; Click on Add button

	ControlCommand($WIN_BUSMASTER,"",$COMB_MsgID_ConfigTX,"SetCurrentSelection",4)		; Add the DB msgs to the Tx message list

	_AddMsg2TxList()															; Click on Add button

	ControlCommand($WIN_BUSMASTER,"",$COMB_MsgID_ConfigTX,"SetCurrentSelection",5)		; Add the DB msgs to the Tx message list

	_AddMsg2TxList()															; Click on Add button

	_CloseTxWindow()															; Close Tx window

	_ConfigureCANLog("Log_04_Sample","System","ALL","Hex","Overwrite",100,200,"False")	; Configure Logging details

	_ConnectDisconnect()															; Connect the tool

	_TransmitMsgsMenu()																; Transmit normal blocks
	sleep(5000)

	_ConnectDisconnect()															; Disconnect the tool

	$GetLogFile_Path=_SetOneFolderUp()												; Fetch the Log file path
	ConsoleWrite("$GetLogFile_Path:"&$GetLogFile_Path&@CRLF)

	$Msg100Rx=FileReadLine ($GetLogFile_Path & "\Log_04_Sample.log",16)				; Read the 16th line from the Log file
	consolewrite("$Msg100Rx :"&$Msg100Rx&@CRLF)
	$HexMsgID100Rx=StringSplit($Msg100Rx," ")
	consolewrite("$HexMsgID_Dir :"&$HexMsgID100Rx[2]&@CRLF)							; Fetch the Direction
	consolewrite("$HexMsgID100 :"&$HexMsgID100Rx[4]&@CRLF)							; Fetch the Hex Msg ID

	$Msg150Tx=FileReadLine ($GetLogFile_Path & "\Log_04_Sample.log",17)				; Read the 17th line from the Log file
	consolewrite("$Msg150Tx :"&$Msg150Tx&@CRLF)
	$HexMsgID150Tx=StringSplit($Msg150Tx," ")
	consolewrite("$HexMsgID_Dir :"&$HexMsgID150Tx[2]&@CRLF)							; Fetch the Direction
	consolewrite("$HexMsgID150 :"&$HexMsgID150Tx[4]&@CRLF)							; Fetch the Hex Msg ID

	$Msg150Rx=FileReadLine ($GetLogFile_Path & "\Log_04_Sample.log",18)				; Read the 18th line from the Log file
	consolewrite("$Msg150Rx :"&$Msg150Rx&@CRLF)
	$HexMsgID150Rx=StringSplit($Msg150Rx," ")
	consolewrite("$HexMsgID_Dir :"&$HexMsgID150Rx[2]&@CRLF)							; Fetch the Direction
	consolewrite("$HexMsgID150 :"&$HexMsgID150Rx[4]&@CRLF)							; Fetch the Hex Msg ID

	$Msg200Tx=FileReadLine ($GetLogFile_Path & "\Log_04_Sample.log",19)				; Read the 19th line from the Log file
	consolewrite("$Msg200Tx :"&$Msg200Tx&@CRLF)
	$HexMsgID200Tx=StringSplit($Msg200Tx," ")
	consolewrite("$HexMsgID_Dir :"&$HexMsgID200Tx[2]&@CRLF)							; Fetch the Direction
	consolewrite("$HexMsgID200 :"&$HexMsgID200Tx[4]&@CRLF)							; Fetch the Hex Msg ID

	$Msg200Rx=FileReadLine ($GetLogFile_Path & "\Log_04_Sample.log",20)				; Read the 20th line from the Log file
	consolewrite("$Msg200Tx :"&$Msg200Rx&@CRLF)
	$HexMsgID200Rx=StringSplit($Msg200Rx," ")
	consolewrite("$HexMsgID_Dir :"&$HexMsgID200Rx[2]&@CRLF)							; Fetch the Direction
	consolewrite("$HexMsgID200 :"&$HexMsgID200Rx[4]&@CRLF)							; Fetch the Hex Msg ID

	$EndOfFile=FileReadLine ($GetLogFile_Path & "\Log_04_Sample.log",21)				; Read the 21th line from the Log file
	consolewrite("$EndOfFile :"&$EndOfFile&@CRLF)
	$FindEOF=StringSplit($EndOfFile," ")
	consolewrite("$FindEOF:"&$FindEOF[1]&@CRLF)										; Fetch the Direction
EndIf
if $HexMsgID100Rx[2]="Rx" and $HexMsgID150Tx[2]="Tx" and $HexMsgID150Rx[2]="Rx" and $HexMsgID200Tx[2]="Tx" and $HexMsgID200Rx[2]="Rx" Then
	$AllMsgDirection="Success"
Else
	$AllMsgDirection="UnSuccess"
EndIf
if $HexMsgID100Rx[4]=0x100 and $HexMsgID150Tx[4]=0x150 and $HexMsgID150Rx[4]=0x150 and $HexMsgID200Tx[4]=0x200 and $HexMsgID200Rx[4]=0x200 Then
	$AllMsgID="Success"
Else
	$AllMsgID="UnSuccess"
EndIf

consolewrite("$AllMsgDirection:"&$AllMsgDirection&@CRLF)
consolewrite("$AllMsgID:"&$AllMsgID&@CRLF)
consolewrite("$FindEOF:"&$FindEOF[1]&@CRLF)

if $AllMsgDirection="Success" and $AllMsgID="Success" and $FindEOF[1]="***END" Then
	_ExcelWriteCell($oExcel, "Pass", 16, 2)
Else
	_ExcelWriteCell($oExcel, "Fail", 16, 2)
EndIf


ConsoleWrite("****End : TS_Log_04.au3****"&@CRLF)
ConsoleWrite(@CRLF)