;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_Filter_02
; TestCases			:		Validity of filter list for logging
; Test Data			:
; Test Setup		:		1. Invoke dialog box for filter configuration
;~ 							2. Enter ID of a message to update the filter list
;~ 							3. Confirm by pressing OK button
;~ 							4. Enable logging
;~ 							5. Enable log filter
;~ 							6. Send the same message across CAN bus

; Expected Results  : 		"The message ID shouldn't appear in the log file"
;==========================================================================Test Procedure =========================================================================


ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_Filter_02.au3****"&@CRLF)

_launchApp()																		; Check if the App is open else launch it

WinActivate($WIN_BUSMASTER)
Dim $FirstMsg[13]=[1,2,3,4,5,6,7,8,9,10,11,12,13],$SecMsg[13]=[1,2,3,4,5,6,7,8,9,10,11,12,13]

if winexists($WIN_BUSMASTER) then
	_loadConfig("cfxFilter_02")														; Load Configuration

	_AddFilter("Stop","[0x15]Msg1","","","Std","Non RTR","Tx",1)					; Configure Filter details
	sleep(1000)

	_DisableJ1939Win()																; If J1939 Msg Window is active then disable it
	sleep(2000)

	_ConfigureCANLog("Log_Filter_02","System","ALL","Decimal","Append","","","True"); Configure Logging details

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

	$GetLogFile_Path=_OutputDataPath()												; Fetch the Log file path
	ConsoleWrite("$GetLogFile_Path:"&$GetLogFile_Path&@CRLF)
	if _FileCountLines($GetLogFile_Path & "\Log_Filter_02.log")>=17 Then
		$Read_LogFirstEnter=FileReadLine ($GetLogFile_Path & "\Log_Filter_02.log",16)	; Read the 16th line from the Log file
		$Read_LogSecEnter=FileReadLine ($GetLogFile_Path & "\Log_Filter_02.log",17)		; Read the 17th line from the Log file
		consolewrite("@error :"&@error&@CRLF)
		consolewrite("$Read_LogFirstEnter:"&$Read_LogFirstEnter&@CRLF)
		consolewrite("$Read_LogSecEnter:"&$Read_LogSecEnter&@CRLF)
		$FirstMsg=StringSplit($Read_LogFirstEnter," ")
		$SecMsg=StringSplit($Read_LogSecEnter," ")
	EndIf
EndIf
consolewrite("$FirstMsg[2]:"&$FirstMsg[2]&@CRLF)
consolewrite("$SecMsg[2]:"&$SecMsg[2]&@CRLF)


if $FirstMsg[2]="Rx" and $SecMsg[2]="Rx" Then														; Validate the result
	_WriteResult("Pass","TS_Filter_02")
Else
	_WriteResult("Fail","TS_Filter_02")
EndIf
$isAppNotRes=_CloseApp()															; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_Filter_02")
EndIf


ConsoleWrite("****End : TS_Filter_02.au3****"&@CRLF)
ConsoleWrite(@CRLF)