#cs
;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_LIN_Filter_02
; TestCases			:		Validity of filter list for message window
; Test Data			:
; Test Setup		:		"1. Invoke dialog box for filter configuration
							2. Enter ID of a message to update the filter list
							3. Confirm by pressing OK button
							4. Enable logging
							5. Enable log filter
							6. Send the same message across LIN bus"

Expected Results  : 		"The message ID shouldn't appear in the log file"

#ce
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start :TS_LIN_Filter_02.au3****"&@CRLF)
Local $Msg = ObjCreate("System.Collections.ArrayList") ; create
Dim $FirstMsg[13]=[1,2,3,4,5,6,7,8,9,10,11,12,13],$SecMsg[13]=[1,2,3,4,5,6,7,8,9,10,11,12,13]
Local $FirstMsg = 0,$msgidCheck=0,$txmsg=0,$endOfFIle=0




_launchApp()

WinActivate($WIN_BUSMASTER)

if winexists($WIN_BUSMASTER) then

	_loadConfig("cfxLINFilter_01")																	; Load Configuration
	_SetViewToolBarLIN()																			; Select LIN Tool Bar to view.
	_linDriverSelection()


	_AddFilter_LIN("Stop","[0x1]Frame1","","","Tx","All")											; Configure Filter details
	Sleep(2000)

	_ConfigureLINLog("LIN_Log_Filter_01","System","ALL","Decimal","Append","","","True")


	_EnableFilterLogLIN()																			; Enable Filter during logging

	_ConnectDisconnect_LIN()
	sleep(3000)
	_ConnectDisconnect_LIN()

	;_DisableOverwriteMode()																		; Disable Overwrite Mode
	Sleep(2000)

	$GetLogFile_Path=_OutputDataPath()																; Fetch the Log file path
	ConsoleWrite("$GetLogFile_Path:"&$GetLogFile_Path&@CRLF)

	ConsoleWrite("$GetLogFile_Path:"&$GetLogFile_Path&@CRLF)
	if _FileCountLines($GetLogFile_Path & "\LIN_Log_Filter_01.log")=20 Then
		$Read_LogFirstEnter=FileReadLine ($GetLogFile_Path & "\LIN_Log_Filter_01.log",15)	; Read the 16th line from the Log file
		$Read_LogSecEnter=FileReadLine ($GetLogFile_Path & "\LIN_Log_Filter_01.log",16)		; Read the 17th line from the Log file

		$Read_LogSThirdEnter=FileReadLine ($GetLogFile_Path & "\LIN_Log_Filter_01.log",17)		; Read the 18th line from the Log file
		$Read_LogFourthEnter=FileReadLine ($GetLogFile_Path & "\LIN_Log_Filter_01.log",18)		; Read the 19th line from the Log file
		$Read_LogEndDate=FileReadLine ($GetLogFile_Path & "\LIN_Log_Filter_01.log",19)		; Read the 19th line from the Log file
		$Read_EndSession=FileReadLine ($GetLogFile_Path & "\LIN_Log_Filter_01.log",20)		; Read the 19th line from the Log file
		;consolewrite("@error :"&@error&@CRLF)
		consolewrite("$Read_LogFirstEnter:"&$Read_LogFirstEnter&@CRLF)
		consolewrite("$Read_LogSecEnter:"&$Read_LogSecEnter&@CRLF)
		consolewrite("$Read_LogSThirdEnter:"&$Read_LogSThirdEnter&@CRLF)
		consolewrite("$Read_LogFourthEnter:"&$Read_LogFourthEnter&@CRLF)



		$ret1 = StringInStr($Read_LogEndDate,"END DATE AND TIME")


		consolewrite("$ret1:"&$ret1&@CRLF)
		consolewrite("$Read_EndSession:"&$Read_EndSession&@CRLF)


		if $ret1>1 and $Read_EndSession="***[STOP LOGGING SESSION]***" Then
			$endOfFIle = 1
		EndIf

		$FirstMsg=StringSplit($Read_LogFirstEnter," ")
		$SecMsg=StringSplit($Read_LogSecEnter," ")
		$ThrdMsg=StringSplit($Read_LogSecEnter," ")
		$FourthMsg=StringSplit($Read_LogSecEnter," ")

		if $FirstMsg[2]="Tx" and $SecMsg[2]="Tx" and $ThrdMsg[2]="Tx" and $FourthMsg[2]="Tx" Then
			$txMsg =1
		EndIf
		if $FirstMsg[4]="0002" and $SecMsg[4]="0002" and $ThrdMsg[4]="0002" and $FourthMsg[4]="0002" Then
			$msgidCheck =1
		EndIf
	EndIf

EndIf
consolewrite("$txMsg:"&$txMsg&@CRLF)
consolewrite("$msgidCheck:"&$msgidCheck&@CRLF)
consolewrite("$endOfFIle:"&$endOfFIle&@CRLF)

if $txMsg=1 and $msgidCheck=1 and $endOfFIle=1 Then												; Validate the result
	_WriteResult("Pass","TS_LIN_Filter_02")
	ConsoleWrite("Pass"&@CRLF)
Else
	_WriteResult("Fail","TS_LIN_Filter_02")
	ConsoleWrite("Fail"&@CRLF)
EndIf



$isAppNotRes=_CloseApp()																		; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_LIN_Filter_02")
EndIf

ConsoleWrite("****End : TS_LIN_Filter_02.au3****"&@CRLF)
ConsoleWrite(@CRLF)


