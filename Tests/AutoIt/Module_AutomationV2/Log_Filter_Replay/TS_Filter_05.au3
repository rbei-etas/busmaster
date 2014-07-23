;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_Filter_05
; TestCases			:		Check for stop filter mode - Message filter
; Test Data			:
; Test Setup		:		1. Invoke dialog box for filter configuration
;~ 							2. Specify a filter list
;~ 							3. Select stop filter mode
;~ 							4. Confirm by selecting OK button
;~ 							5. Enable message display / logging
;~ 							6. Send a some messages across CAN bus, which shall contain a few messages from the filter list

; Expected Results  : 		"Those messages occuring in the filter list shouldn't be found in the message display window / log file"
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_Filter_05.au3****"&@CRLF)

_launchApp()																		; Check if the App is open else launch it

WinActivate($WIN_BUSMASTER)

Local $MsgWinItemCount=0,$MsgWinFilter=0,$LogFilter=0
Dim $Msg1[10]=[1,2,3,4,5,6,7,8,9,10]

if winexists($WIN_BUSMASTER) then
	_loadConfig("cfxFilter_05")														; Create New Configuration

	_AddFilter("Stop","[0x15]Msg1","","","Std","Non RTR","Tx",1)					; Configure Filter details
	sleep(1000)

	_MsgDisplayMenu()																; Open Message Display dialog

	_AddFiltertoMsgDisp()															; Add the filter to Message display

	_En_Dis_FilterDisplay()															; Enable filters for message display

	_ConfigureCANLog("Log_Filter_05","System","ALL","Decimal","Append","","","True"); Configure Logging details

	_EnableFilterLog()																; Enable Filter during logging
	sleep(1500)

	_ConnectDisconnect()															; Connect the tool

	_TransmitMsgsMenu()																; Transmit normal blocks
	sleep(3000)

	_ConnectDisconnect()															; Disconnect the tool

	Sleep(2000)
	_DisableJ1939Win()																; If J1939 Msg Window is active then disable it

	$MsgWinItemCount=_GetCANMsgWinItemCount()										; Fetch the item count in the msg window
	consolewrite("$MsgWinItemCount :"&$MsgWinItemCount&@CRLF)

	if $MsgWinItemCount=1 Then
		$Msg1=_GetMsgWinCANInfo(0)													; Fetch the first msg info
	EndIf

	$GetLogFile_Path=_OutputDataPath()												; Fetch the Log file path
	ConsoleWrite("$GetLogFile_Path:"&$GetLogFile_Path&@CRLF)
	if _FileCountLines($GetLogFile_Path & "\Log_Filter_05.log")>=19 Then
		$Read_LogFirstMsg=FileReadLine ($GetLogFile_Path & "\Log_Filter_05.log",16)	; Read the 16th line from the Log file
		$Read_LogSecMsg=FileReadLine ($GetLogFile_Path & "\Log_Filter_05.log",17)	; Read the 17th line from the Log file
		$Read_LogThrMsg=FileReadLine ($GetLogFile_Path & "\Log_Filter_05.log",18)	; Read the 18th line from the Log file
		$Read_LogFourthMsg=FileReadLine ($GetLogFile_Path & "\Log_Filter_05.log",19); Read the 19th line from the Log file
		consolewrite("@error :"&@error&@CRLF)
		consolewrite("$Read_LogFirstMsg:"&$Read_LogFirstMsg&@CRLF)
		consolewrite("$Read_LogSecMsg:"&$Read_LogSecMsg&@CRLF)
		consolewrite("$Read_LogThrMsg:"&$Read_LogThrMsg&@CRLF)
		consolewrite("$Read_LogFourthMsg:"&$Read_LogFourthMsg&@CRLF)
		$FirstMsg=StringSplit($Read_LogFirstMsg," ")
		$SecMsg=StringSplit($Read_LogSecMsg," ")
		$ThirdMsg=StringSplit($Read_LogThrMsg," ")
		$FourthMsg=StringSplit($Read_LogFourthMsg," ")
	EndIf
EndIf

if $Msg1[4]="0x015" Then
	$MsgWinFilter=1
EndIf

if $FirstMsg[4]=0021 and $SecMsg[4]=0021 and $ThirdMsg[4]=0021 and $FourthMsg[4]=0021 Then
	$LogFilter=1
EndIf

consolewrite("$MsgWinFilter:"&$MsgWinFilter&@CRLF)
consolewrite("$LogFilter:"&$LogFilter&@CRLF)
consolewrite("$Msg1[4]:"&$Msg1[4]&@CRLF)
consolewrite("$FirstMsg[4]:"&$FirstMsg[4]&@CRLF)
consolewrite("$SecMsg[4]:"&$SecMsg[4]&@CRLF)
consolewrite("$ThirdMsg[4]:"&$ThirdMsg[4]&@CRLF)
consolewrite("$FourthMsg[4]:"&$FourthMsg[4]&@CRLF)

if $MsgWinFilter=1 and $LogFilter=1 Then													; Validate the result
	_WriteResult("Pass","TS_Filter_05")
Else
	_WriteResult("Fail","TS_Filter_05")
EndIf

$isAppNotRes=_CloseApp()															; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_Filter_05")
EndIf

ConsoleWrite("****End : TS_Filter_05.au3****"&@CRLF)
ConsoleWrite(@CRLF)