#cs
;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_LIN_Filter_03
; TestCases			:		Check for effectiveness - [A message ID range entered in the filter list ]
; Test Data			:
; Test Setup		:		"1. Invoke dialog box for filter configuration
							2. Specify a filter range
							3. Confirm by pressing OK button
							4. Enable display/log filter
							5. Send message(s) specified in the range across LIN bus"

Expected Results  : 		"Specified message(s) shouldn't be found in the message display window / log file"

#ce
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start :TS_LIN_Filter_03.au3****"&@CRLF)
Local $Msg = ObjCreate("System.Collections.ArrayList") ; create
Dim $FirstMsg[13]=[1,2,3,4,5,6,7,8,9,10,11,12,13],$SecMsg[13]=[1,2,3,4,5,6,7,8,9,10,11,12,13]
Local $FirstMsg = 0,$msgidCheck=0,$txmsg=0,$endOfFIle=0
Local $FirstMsgInMsgWindow=0,$SecMsginMsgWindow=0


_launchApp()

WinActivate($WIN_BUSMASTER)

if winexists($WIN_BUSMASTER) then

	_loadConfig("cfxLINFilter_03")																	; Load Configuration
	_SetViewToolBarLIN()																			; Select LIN Tool Bar to view.
	_linDriverSelection()


	_AddFilter_LIN("Stop","",2,4,"Tx","All")														; Configure Filter details
	Sleep(2000)
	_MsgDisplayMenuLIN()																			; Open Message Display dialog -LIN


	_AddFiltertoMsgDisp_LIN()													   					; Adds the filter for message display for LIN
	sleep(1000)

	_EnableFilterDispMenuLIN()														  				; Enable filters for message display

	_ConfigureLINLog("LIN_Log_Filter_03","System","ALL","Decimal","Append","","","True")


	_EnableFilterLogLIN()																			; Enable Filter during logging

	_ConnectDisconnect_LIN()
	sleep(1000)
	_ConnectDisconnect_LIN()

	;_DisableOverwriteMode()																		; Disable Overwrite Mode
	Sleep(1000)

	$rCount=_GetLINMsgWinItemCount()																;get the row count


	if $rCount = 2 Then


		$Msg=_GetMsgWinInfo($LVC_CID_LINMsgWin,0,10)												; Fetch the first row data in the msg window
		$Count1=_GetLINMsgWinColCount()
		for $i=0 to $Count1-2
			$Value =$Msg.Item($i)
			ConsoleWrite("$Msg[" & $i &"] : "& $Value &@CRLF)
		next

		for $j=0 to $rCount-1
			$Msg=_GetMsgWinInfo($LVC_CID_LINMsgWin,$j,10)												; Fetch the first row data in the msg window

			if $Msg.Item(0) <>" " and $Msg.Item(1)="Frame1" and $Msg.Item(2)="LIN Message" and $Msg.Item(3)="Tx" and $Msg.Item(4)=1 and $Msg.Item(5)=4 and $Msg.Item(6)="0x001" and $Msg.Item(7)="12 00 00 00" and $Msg.Item(8)="0x2C (""Enhanced"")" Then			; Compare the Direction, Channel and Msg ID of the first row
				$FirstMsgInMsgWindow=1
			EndIf
			if $Msg.Item(0) <>" " and $Msg.Item(1)="0x5" and $Msg.Item(2)="LIN Message" and $Msg.Item(3)="Tx" and $Msg.Item(4)=1 and $Msg.Item(5)=8 and $Msg.Item(6)="0x005" and $Msg.Item(7)="00 00 00 00 00 00 00 00" and $Msg.Item(8)="0x7A (""Enhanced"")" Then			; Compare the Direction, Channel and Msg ID of the first row
				$SecMsginMsgWindow=1
			EndIf
		Next


	EndIf


	$GetLogFile_Path=_OutputDataPath()																; Fetch the Log file path
	ConsoleWrite("$GetLogFile_Path:"&$GetLogFile_Path&@CRLF)

	ConsoleWrite("$GetLogFile_Path:"&$GetLogFile_Path&@CRLF)
	$logcount=_FileCountLines($GetLogFile_Path & "\LIN_Log_Filter_03.log")
	ConsoleWrite("$logcount-----------:"&$logcount&@CRLF)
	if _FileCountLines($GetLogFile_Path & "\LIN_Log_Filter_03.log")=20 Then
		$Read_LogFirstEnter=FileReadLine ($GetLogFile_Path & "\LIN_Log_Filter_03.log",15)			; Read the 16th line from the Log file
		$Read_LogSecEnter=FileReadLine ($GetLogFile_Path & "\LIN_Log_Filter_03.log",16)				; Read the 17th line from the Log file

		$Read_LogSThirdEnter=FileReadLine ($GetLogFile_Path & "\LIN_Log_Filter_03.log",17)			; Read the 18th line from the Log file
		$Read_LogFourthEnter=FileReadLine ($GetLogFile_Path & "\LIN_Log_Filter_03.log",18)			; Read the 19th line from the Log file
		$Read_LogEndDate=FileReadLine ($GetLogFile_Path & "\LIN_Log_Filter_03.log",19)				; Read the 19th line from the Log file
		$Read_EndSession=FileReadLine ($GetLogFile_Path & "\LIN_Log_Filter_03.log",20)				; Read the 19th line from the Log file
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
		$ThrdMsg=StringSplit($Read_LogSThirdEnter," ")
		$FourthMsg=StringSplit($Read_LogFourthEnter," ")

		if $FirstMsg[2]="Tx" and $SecMsg[2]="Tx" and $ThrdMsg[2]="Tx" and $FourthMsg[2]="Tx" Then
			$txMsg =1
		EndIf
		if $FirstMsg[4]="0001" and $SecMsg[4]="0005" and $ThrdMsg[4]="0001" and $FourthMsg[4]="0005" Then
			$msgidCheck =1
		EndIf
	EndIf

EndIf
consolewrite("$FirstMsgInMsgWindow:"&$FirstMsgInMsgWindow&@CRLF)
consolewrite("$SecMsginMsgWindow:"&$SecMsginMsgWindow&@CRLF)
consolewrite("$txMsg:"&$txMsg&@CRLF)
consolewrite("$msgidCheck:"&$msgidCheck&@CRLF)
consolewrite("$endOfFIle:"&$endOfFIle&@CRLF)

if $FirstMsgInMsgWindow=1 and $SecMsginMsgWindow=1 and $txMsg=1 and $msgidCheck=1 and $endOfFIle=1 Then												; Validate the result
	_WriteResult("Pass","TS_LIN_Filter_03")
	ConsoleWrite("Pass"&@CRLF)
Else
	_WriteResult("Fail","TS_LIN_Filter_03")
	ConsoleWrite("Fail"&@CRLF)
EndIf



$isAppNotRes=_CloseApp()																		; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_LIN_Filter_03")
EndIf

ConsoleWrite("****End :TS_LIN_Filter_03.au3****"&@CRLF)
ConsoleWrite(@CRLF)


