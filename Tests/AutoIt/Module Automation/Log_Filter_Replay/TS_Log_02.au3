;=== Test Cases/Test Data ===
; Module			:		Logging
; Critical (C)		:		Y
; TestCase No.		:		TS_Log_02
; TestCases			:		Functionality test - Logging
; Test Strategy		:		Black Box
; Test Data			:		-
; === Test Procedure ===

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_Log_02.au3****"&@CRLF)


_launchApp()

if winexists($mWin) then

	_createConfig("cfxLogging_02")													; Create New Configuration

	_createCANDB("testLogging_02")												; Create New Database File
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

	_AssociateCANDB("testLogging_02.dbf")										; Associate DB

	_hdWareSelectKvaser()														; Select Kvaser Hardware
	 sleep(1000)

	_TxMsgMenu()																; Open Tx window from the menu

	_AddMsgBlock()																; Add a msg block

	_EnableAutoUpdate()															; Enable autoupdate

	ControlCommand($mWin,"",$msgIDNameComboctrlID,"SetCurrentSelection",0)		; Add the DB msgs to the Tx message list

	_AddMsg2TxList()															; Click on Add button

	_CloseTxWindow()															; Close Tx window

	_ConfigureCANLog("Logtest02_1","System","ALL","Hex","Overwrite","","","False")		; Configure Logging details

	_ConfigureCANLog("Logtest02_2","Absolute","ALL","Decimal","Append","","","False")	; Configure Logging details

	_ConfigureCANLog("Logtest02_3","Relative","ALL","Decimal","Append","","","False")	; Configure Logging details

	_ConnectDisconnect()															; Connect the tool

	$cntToolhWd=ControlGetHandle($mWin,"",$connectToolbarctrlID)						; Get handle of the 'Connect/Disconnect' toolbar

	$CommandID=_GUICtrlToolbar_IndexToCommand($cntToolhWd,9)							; Fetch the CommandID
	consolewrite(@CRLF&"Command ID : " & $CommandID)
	$PressedState=_GUICtrlToolbar_IsButtonChecked($cntToolhWd, $CommandID)						; Retrive the state of the button
	consolewrite(@CRLF&"$PressedState : " & $PressedState)

	_TransmitMsgsMenu()																; Transmit normal blocks
	sleep(3000)

	_ConnectDisconnect()															; Disconnect the tool
	Sleep(2000)

	$CommandID=_GUICtrlToolbar_IndexToCommand($cntToolhWd,9)							; Fetch the CommandID
	consolewrite(@CRLF&"Command ID : " & $CommandID)
	$UnPressedState=_GUICtrlToolbar_IsButtonChecked($cntToolhWd, $CommandID)						; Retrive the state of the button
	consolewrite(@CRLF&"$UnPressedState : " & $UnPressedState)

	if $PressedState=True and $UnPressedState=False Then
		$LogButtonCheck=1
	Else
		$LogButtonCheck=0
	EndIf

	$GetLogFile_Path=_SetOneFolderUp()												; Fetch the Log file path
	ConsoleWrite("$GetLogFile_Path:"&$GetLogFile_Path&@CRLF)

	$Logtest02_1_NumMode=FileReadLine ($GetLogFile_Path & "\Logtest02_1.log",6)		; Read the 6th line from the Log file
	$Logtest02_1_TimeMode=FileReadLine ($GetLogFile_Path & "\Logtest02_1.log",7)	; Read the 7th line from the Log file
	consolewrite("@error :"&@error&@CRLF)
	consolewrite("$Logtest02_1_NumMode:"&$Logtest02_1_NumMode&@CRLF)
	consolewrite("$Logtest02_1_TimeMode:"&$Logtest02_1_TimeMode&@CRLF)

	$Logtest02_2_NumMode=FileReadLine ($GetLogFile_Path & "\Logtest02_2.log",6)		; Read the 6th line from the Log file
	$Logtest02_2_TimeMode=FileReadLine ($GetLogFile_Path & "\Logtest02_2.log",7)	; Read the 7th line from the Log file
	consolewrite("@error :"&@error&@CRLF)
	consolewrite("$Logtest02_2_NumMode:"&$Logtest02_2_NumMode&@CRLF)
	consolewrite("$Logtest02_2_TimeMode:"&$Logtest02_2_TimeMode&@CRLF)

	$Logtest02_3_NumMode=FileReadLine ($GetLogFile_Path & "\Logtest02_3.log",6)		; Read the 6th line from the Log file
	$Logtest02_3_TimeMode=FileReadLine ($GetLogFile_Path & "\Logtest02_3.log",7)	; Read the 7th line from the Log file
	consolewrite("@error :"&@error&@CRLF)
	consolewrite("$Logtest02_3_NumMode:"&$Logtest02_3_NumMode&@CRLF)
	consolewrite("$Logtest02_3_TimeMode:"&$Logtest02_3_TimeMode&@CRLF)

	$File1_NumMode=StringReplace($Logtest02_1_NumMode,"*","")						; Remove String '*'
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

	if $File1_NumMode="HEX" and $File1_TimeMode="SYSTEM MODE" then					; Validate First Log file settings
		$LogFile1Config="Match"
	Else
		$LogFile1Config="No Match"
	EndIf

	if $File2_NumMode="DEC" and $File2_TimeMode="ABSOLUTE MODE" then				; Validate Second Log file settings
		$LogFile2Config="Match"
	Else
		$LogFile2Config="No Match"
	EndIf

	if $File3_NumMode="DEC" and $File3_TimeMode="RELATIVE MODE" then				; Validate Third Log file settings
		$LogFile3Config="Match"
	Else
		$LogFile3Config="No Match"
	EndIf

	consolewrite("$LogButtonCheck :"&$LogButtonCheck&@CRLF)
	consolewrite("$LogFile1Config :"&$LogFile1Config&@CRLF)
	consolewrite("$LogFile2Config :"&$LogFile2Config&@CRLF)
	consolewrite("$LogFile3Config :"&$LogFile3Config&@CRLF)

	$TxMsgFile1=FileReadLine ($GetLogFile_Path & "\Logtest02_1.log",16)				; Read the 16th line from the Log file
	consolewrite("$TxMsgFile1 :"&$TxMsgFile1&@CRLF)
	$HexMsgID=StringSplit($TxMsgFile1," ")
	consolewrite("$HexMsgID :"&$HexMsgID[4]&@CRLF)

	$TxMsgFile2=FileReadLine ($GetLogFile_Path & "\Logtest02_2.log",16)				; Read the 16th line from the Log file
	consolewrite("$TxMsgFile2 :"&$TxMsgFile2&@CRLF)
	$DecMsgID=StringSplit($TxMsgFile2," ")
	consolewrite("$DecMsgID :"&$DecMsgID[4]&@CRLF)
EndIf

if $LogButtonCheck=1 and $LogFile1Config="Match" and $LogFile2Config="Match" and $LogFile3Config="Match" and $HexMsgID[4]=0x015 and $DecMsgID[4]=0021 Then
	_ExcelWriteCell($oExcel, "Pass", 14, 2)
Else
	_ExcelWriteCell($oExcel, "Fail", 14, 2)
EndIf

ConsoleWrite("****End : TS_Log_02.au3****"&@CRLF)
ConsoleWrite(@CRLF)
