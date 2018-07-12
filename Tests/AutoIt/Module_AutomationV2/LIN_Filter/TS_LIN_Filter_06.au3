#cs
;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_LIN_Filter_06
; TestCases			:		Check for pass filter mode
; Test Data			:
; Test Setup		:		"1. Invoke dialog box for filter configuration
							2. Specify a filter list
							3. Select stop filter mode
							4. Confirm by selecting OK button
							5. Enable message display / logging
							6. Send a some messages across LIN bus, which shall contain a few messages from the filter list"

Expected Results  : 		"Only those messages occuring in the filter list should be found in the message display window / log file"

#ce
;==========================================================================Test Procedure =========================================================================


ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start :TS_LIN_Filter_06.au3****"&@CRLF)
Local $Msg = ObjCreate("System.Collections.ArrayList") ; create
Dim $FirstMsg[13]=[1,2,3,4,5,6,7,8,9,10,11,12,13],$SecMsg[13]=[1,2,3,4,5,6,7,8,9,10,11,12,13]
Local $FirstMsg = 0,$msgidCheck=0,$txmsg=0,$endOfFIle=0
Local $FirstMsgInMsgWindow=0,$SecMsginMsgWindow=0
Local $LoGFiltername = "LIN_Log_Filter_06"

If(@OSVersion <> "WIN_7") Then
	Local $tmout = 1000
Else
	Local $tmout = 0
EndIf

_launchApp()

WinActivate($WIN_BUSMASTER)

if winexists($WIN_BUSMASTER) then

	_loadConfig("cfxLINFilter_06")																	; Load Configuration
	_SetViewToolBarLIN()																			; Select LIN Tool Bar to view.
	_linDriverSelection()


	_AddFilter_LIN("Pass","[0x1]Frame1","","","Tx","All")														; Configure Filter details
	Sleep(2000)
	_MsgDisplayMenuLIN()																			; Open Message Display dialog -LIN


	_AddFiltertoMsgDisp_LIN()													   					; Adds the filter for message display for LIN
	sleep(1000)

	_EnableFilterDispMenuLIN()														  				; Enable filters for message display

	_ConfigureLINLog("LIN_Log_Filter_06","System","ALL","Decimal","Append","","","True")


	_EnableFilterLogLIN()																			; Enable Filter during logging

	_ConnectDisconnect_LIN()
	sleep($tmout)
	_ConnectDisconnect_LIN()

	;_DisableOverwriteMode()																		; Disable Overwrite Mode
	Sleep(1000)

	$rCount=_GetLINMsgWinItemCount()																;get the row count


	if $rCount = 1 Then


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

		Next


	EndIf


	$GetLogFile_Path=_OutputDataPath()																; Fetch the Log file path
	ConsoleWrite("$GetLogFile_Path:"&$GetLogFile_Path&@CRLF)

	ConsoleWrite("$GetLogFile_Path:"&$GetLogFile_Path&@CRLF)
	$logcount=_FileCountLines($GetLogFile_Path &"\"& $LoGFiltername&".log")
	ConsoleWrite("$logcount-----------:"&$logcount&@CRLF)
	if _FileCountLines($GetLogFile_Path &"\"& $LoGFiltername&".log")=20 Then
		$Read_LogFirstEnter=FileReadLine ($GetLogFile_Path &"\"& $LoGFiltername&".log",15)			; Read the 15th line from the Log file
		$Read_LogSecEnter=FileReadLine ($GetLogFile_Path &"\"& $LoGFiltername&".log",16)				; Read the 16th line from the Log file
		$Read_LogThirdEnter=FileReadLine ($GetLogFile_Path &"\"& $LoGFiltername&".log",17)				; Read the 17th line from the Log file
		$Read_LogfourthEnter=FileReadLine ($GetLogFile_Path &"\"& $LoGFiltername&".log",18)				; Read the 18th line from the Log file


		$Read_LogEndDate=FileReadLine ($GetLogFile_Path &"\"& $LoGFiltername&".log",19)				; Read the 19th line from the Log file
		$Read_EndSession=FileReadLine ($GetLogFile_Path &"\"& $LoGFiltername&".log",20)				; Read the 20th line from the Log file

		consolewrite("$Read_LogFirstEnter:"&$Read_LogFirstEnter&@CRLF)
		consolewrite("$Read_LogSecEnter:"&$Read_LogSecEnter&@CRLF)




		$ret1 = StringInStr($Read_LogEndDate,"END DATE AND TIME")


		consolewrite("$ret1:"&$ret1&@CRLF)
		consolewrite("$Read_EndSession:"&$Read_EndSession&@CRLF)


		if $ret1>1 and $Read_EndSession="***[STOP LOGGING SESSION]***" Then
			$endOfFIle = 1
		EndIf

		$FirstMsg=StringSplit($Read_LogFirstEnter," ")
		$SecMsg=StringSplit($Read_LogSecEnter," ")
		$ThirdMsg=StringSplit($Read_LogSecEnter," ")
		$fourMsg=StringSplit($Read_LogSecEnter," ")


		if $FirstMsg[2]="Tx" and $SecMsg[2]="Tx" and $ThirdMsg[2]="Tx" and $fourMsg[2]="Tx" Then
			$txMsg =1
		EndIf
		if $FirstMsg[4]="0001" and $SecMsg[4]="0001"  and $ThirdMsg[4]="0001" and $fourMsg[4]="0001" Then
			$msgidCheck =1
		EndIf
	EndIf

EndIf
consolewrite("$FirstMsgInMsgWindow:"&$FirstMsgInMsgWindow&@CRLF)
;consolewrite("$SecMsginMsgWindow:"&$SecMsginMsgWindow&@CRLF)
consolewrite("$txMsg:"&$txMsg&@CRLF)
consolewrite("$msgidCheck:"&$msgidCheck&@CRLF)
consolewrite("$endOfFIle:"&$endOfFIle&@CRLF)

if $FirstMsgInMsgWindow=1 and $txMsg=1 and $msgidCheck=1 and $endOfFIle=1 Then												; Validate the result
	_WriteResult("Pass","TS_LIN_Filter_06")
	ConsoleWrite("Pass"&@CRLF)
Else
	_WriteResult("Fail","TS_LIN_Filter_06")
	ConsoleWrite("Fail"&@CRLF)
EndIf



$isAppNotRes=_CloseApp()																									; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_LIN_Filter_06")
EndIf

ConsoleWrite("****End : TS_LIN_Filter_06.au3****"&@CRLF)
ConsoleWrite(@CRLF)


