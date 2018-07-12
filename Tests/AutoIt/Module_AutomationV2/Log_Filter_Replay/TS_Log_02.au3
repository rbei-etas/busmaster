;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_Log_02
; TestCases			:		Functionality test - Logging
; Test Data			:
; Test Setup		:		1. Configure 4 log files such that each will have different time mode and numeric mode.
;~ 							2. Select Enable logging during Connect/Disconnect option.
;~ 							3. Connect the tool.
;~ 							4. Check the log button status in the tool bar.
;~ 							5. Start sending messages using TX blocks.
;~ 							6. Disconnect the tool.
;~ 							7. Check the log button status.
;~ 							8. Now open all configured log files and check the file content against their configuration.

; Expected Results  : 		1. After step 4, the log button will be in pressed state.
;~ 							2. After step 7, Log button will be in unpressed state.
;~ 							3. The content of log files will match with the configuration in terms of numeric mode and time mode.
;==========================================================================Test Procedure =========================================================================
ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_Log_02.au3****"&@CRLF)


_launchApp()

Local $File1_NumMode="", $File1_TimeMode="",$File2_NumMode="", $File2_TimeMode="",$File3_NumMode="",$File3_TimeMode=""
Dim $HexMsgID[10]=[1,2,3,4,5,6,7,8,9,10],$DecMsgID[10]=[1,2,3,4,5,6,7,8,9,10]

if winexists($WIN_BUSMASTER) then

	_loadConfig("cfxLogging_02")																; Create New Configuration

	_ConfigureCANLog("Logtest02_1","System","ALL","Hex","Overwrite","","","False")				; Configure Logging details

	_ConfigureCANLog("Logtest02_2","Absolute","ALL","Decimal","Append","","","False")			; Configure Logging details

	_ConfigureCANLog("Logtest02_3","Relative","ALL","Decimal","Append","","","False")			; Configure Logging details

	_ConnectDisconnect()																		; Connect the tool

	$cntToolhWd=ControlGetHandle($WIN_BUSMASTER,"",$ToolBar_Con_Disconnect)						; Get handle of the 'Connect/Disconnect' toolbar

	$CommandID=_GUICtrlToolbar_IndexToCommand($cntToolhWd,$Icon_EnableLog_Index)				; Fetch the CommandID
	consolewrite(@CRLF&"Command ID : " & $CommandID)
	$PressedState=_GUICtrlToolbar_IsButtonChecked($cntToolhWd, $CommandID)						; Retrive the state of the button
	consolewrite(@CRLF&"$PressedState : " & $PressedState)

	_TransmitMsgsMenu()																			; Transmit normal blocks
	sleep(3000)

	_ConnectDisconnect()																		; Disconnect the tool
	Sleep(2000)

	$CommandID=_GUICtrlToolbar_IndexToCommand($cntToolhWd,$Icon_EnableLog_Index)				; Fetch the CommandID
	consolewrite(@CRLF&"Command ID : " & $CommandID)
	$UnPressedState=_GUICtrlToolbar_IsButtonChecked($cntToolhWd, $CommandID)					; Retrive the state of the button
	consolewrite(@CRLF&"$UnPressedState : " & $UnPressedState)

	if $PressedState=True and $UnPressedState=False Then
		$LogButtonCheck=1
	Else
		$LogButtonCheck=0
	EndIf

	$GetLogFile_Path=_OutputDataPath()															; Fetch the Log file path
	ConsoleWrite("$GetLogFile_Path:"&$GetLogFile_Path&@CRLF)

	if _FileCountLines($GetLogFile_Path & "\Logtest02_1.log")>=7 Then
		$Logtest02_1_NumMode=FileReadLine ($GetLogFile_Path & "\Logtest02_1.log",6)					; Read the 6th line from the Log file
		$Logtest02_1_TimeMode=FileReadLine ($GetLogFile_Path & "\Logtest02_1.log",7)				; Read the 7th line from the Log file
		$TxMsgFile1=FileReadLine ($GetLogFile_Path & "\Logtest02_1.log",16)								; Read the 16th line from the Log file
		consolewrite("$TxMsgFile1 :"&$TxMsgFile1&@CRLF)
		$HexMsgID=StringSplit($TxMsgFile1," ")
		consolewrite("$HexMsgID :"&$HexMsgID[4]&@CRLF)
		consolewrite("@error :"&@error&@CRLF)
		consolewrite("$Logtest02_1_NumMode:"&$Logtest02_1_NumMode&@CRLF)
		consolewrite("$Logtest02_1_TimeMode:"&$Logtest02_1_TimeMode&@CRLF)
	EndIf

	if _FileCountLines($GetLogFile_Path & "\Logtest02_2.log")>=7 Then
		$Logtest02_2_NumMode=FileReadLine ($GetLogFile_Path & "\Logtest02_2.log",6)					; Read the 6th line from the Log file
		$Logtest02_2_TimeMode=FileReadLine ($GetLogFile_Path & "\Logtest02_2.log",7)				; Read the 7th line from the Log file
		$TxMsgFile2=FileReadLine ($GetLogFile_Path & "\Logtest02_2.log",16)								; Read the 16th line from the Log file
		consolewrite("$TxMsgFile2 :"&$TxMsgFile2&@CRLF)
		$DecMsgID=StringSplit($TxMsgFile2," ")
		consolewrite("$DecMsgID :"&$DecMsgID[4]&@CRLF)
		consolewrite("@error :"&@error&@CRLF)
		consolewrite("$Logtest02_2_NumMode:"&$Logtest02_2_NumMode&@CRLF)
		consolewrite("$Logtest02_2_TimeMode:"&$Logtest02_2_TimeMode&@CRLF)
	EndIf

	if _FileCountLines($GetLogFile_Path & "\Logtest02_3.log")>=7 Then
		$Logtest02_3_NumMode=FileReadLine ($GetLogFile_Path & "\Logtest02_3.log",6)					; Read the 6th line from the Log file
		$Logtest02_3_TimeMode=FileReadLine ($GetLogFile_Path & "\Logtest02_3.log",7)				; Read the 7th line from the Log file
		consolewrite("@error :"&@error&@CRLF)
		consolewrite("$Logtest02_3_NumMode:"&$Logtest02_3_NumMode&@CRLF)
		consolewrite("$Logtest02_3_TimeMode:"&$Logtest02_3_TimeMode&@CRLF)
	EndIf

	$File1_NumMode=StringReplace($Logtest02_1_NumMode,"*","")									; Remove String '*'
	$File1_TimeMode=StringReplace($Logtest02_1_TimeMode,"*","")

	$File2_NumMode=StringReplace($Logtest02_2_NumMode,"*","")
	$File2_TimeMode=StringReplace($Logtest02_2_TimeMode,"*","")

	$File3_NumMode=StringReplace($Logtest02_3_NumMode,"*","")
	$File3_TimeMode=StringReplace($Logtest02_3_TimeMode,"*","")

	consolewrite("$File1_NumMod:"&$File1_NumMode&@CRLF)
	consolewrite("$File1_TimeMode:"&$File1_TimeMode&@CRLF)

	consolewrite("$File2_NumMod:"&$File2_NumMode&@CRLF)
	consolewrite("$File2_TimeMode:"&$File2_TimeMode&@CRLF)

	consolewrite("$File3_NumMod:"&$File3_NumMode&@CRLF)
	consolewrite("$File3_TimeMode:"&$File3_TimeMode&@CRLF)

	if $File1_NumMode="HEX" and $File1_TimeMode="SYSTEM MODE" then									; Validate First Log file settings
		$LogFile1Config="Match"
	Else
		$LogFile1Config="No Match"
	EndIf

	if $File2_NumMode="DEC" and $File2_TimeMode="ABSOLUTE MODE" then								; Validate Second Log file settings
		$LogFile2Config="Match"
	Else
		$LogFile2Config="No Match"
	EndIf

	if $File3_NumMode="DEC" and $File3_TimeMode="RELATIVE MODE" then								; Validate Third Log file settings
		$LogFile3Config="Match"
	Else
		$LogFile3Config="No Match"
	EndIf

	consolewrite("$LogButtonCheck :"&$LogButtonCheck&@CRLF)
	consolewrite("$LogFile1Config :"&$LogFile1Config&@CRLF)
	consolewrite("$LogFile2Config :"&$LogFile2Config&@CRLF)
	consolewrite("$LogFile3Config :"&$LogFile3Config&@CRLF)

EndIf

if  $LogFile1Config="Match" and $LogFile2Config="Match" and $LogFile3Config="Match" and $HexMsgID[4]=0x015 and $DecMsgID[4]=0021 Then
	_WriteResult("Pass","TS_Log_02")
Else
	_WriteResult("Fail","TS_Log_02")
EndIf

$isAppNotRes=_CloseApp()															; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_Log_02")
EndIf




ConsoleWrite("****End : TS_Log_02.au3****"&@CRLF)
ConsoleWrite(@CRLF)
