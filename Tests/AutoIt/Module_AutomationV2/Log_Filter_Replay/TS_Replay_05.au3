;=================================================================== Test Cases/Test Data =========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_Replay_05
; TestCases			:		Functionality - Non Interactive Replay
; Test Data			:		
; Test Setup		:		1. Select a replay file. Select time mode to retain delay and repeat mode to once. Uncheck the interactive option.
;~ 							2. Connect the tool. Check for message transmission.
;~ 							3. Disconnect the tool and check for message transmission or error message

; Expected Results  : 		1. After step 2, Non interactive replay will start its message transmission.
;~ 							2. Non interactive replay will be stopped during disconnect. After step 3, there will not be any message transmission or error messages.
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_Replay_05.au3****"&@CRLF)


_launchApp()

if winexists($WIN_BUSMASTER) then

	_loadConfig("cfxReplay_05")																	; Load Configuration

	_CANReplayMenu()																			; Open configure replay dialog

	_ADDReplayFile("Replay_05")																	; Add a replay file

	ControlClick($WIN_CANReplayConfig,"",$BTN_OK_CANReplayConfig)								; Click on 'OK' button

	_ConnectDisconnect()																		; Connect the tool
	sleep(3000)

	_ConnectDisconnect()																		; Disconnect the tool

	$GetLogFile_Path=_TestDataPath()															; Fetch the Log file path
	$txt=_FileCountLines ($GetLogFile_Path & "\Replay_05.log")
	$TotalLogMsgCount=$txt-2-15																	; Fetch the message count in the log file
	consolewrite("$TotalLogMsgCount :"&$TotalLogMsgCount&@CRLF)

	$MsgWinCount=_GetCANMsgWinItemCount()														; Fetch the Replay Msg window count
	consolewrite("$MsgWinCount :"&$MsgWinCount&@CRLF)
	$FileLoad=0
	if $TotalLogMsgCount=$MsgWinCount Then

		$FirstMsgLog=FileReadLine ($GetLogFile_Path & "\Replay_05.log",16)						; Read the 16th line from the Log file
		$SecondMsgLog=FileReadLine ($GetLogFile_Path & "\Replay_05.log",17)						; Read the 17th line from the Log file
		$ThirdMsgLog=FileReadLine ($GetLogFile_Path & "\Replay_05.log",18)						; Read the 18th line from the Log file
		$FourthMsgLog=FileReadLine ($GetLogFile_Path & "\Replay_05.log",19)						; Read the 19th line from the Log file
		$FifthMsgLog=FileReadLine ($GetLogFile_Path & "\Replay_05.log",20)						; Read the 20th line from the Log file
		$FirstMsgLogTrim=StringTrimRight ($FirstMsgLog,1)										; Remove the space at the end of the line
		$SecondMsgLogTrim=StringTrimRight ($SecondMsgLog,1)
		$ThirdMsgLogTrim=StringTrimRight ($ThirdMsgLog,1)
		$FourthMsgLogTrim=StringTrimRight ($FourthMsgLog,1)
		$FifthMsgLogTrim=StringTrimRight ($FifthMsgLog,1)

		consolewrite("$FirstMsgLogTrim :"&$FirstMsgLogTrim&@CRLF)
		consolewrite("$SecondMsgLogTrim :"&$SecondMsgLogTrim&@CRLF)
		consolewrite("$ThirdMsgLogTrim :"&$ThirdMsgLogTrim&@CRLF)
		consolewrite("$FourthMsgLogTrim :"&$FourthMsgLogTrim&@CRLF)
		consolewrite("$FifthMsgLogTrim :"&$FifthMsgLogTrim&@CRLF)

		$FirstMsgRep=_GetMsgWinCANInfo(0)														; Fetch the first msg entry
		$SecondMsgRep=_GetMsgWinCANInfo(1)														; Fetch the second msg entry
		$ThirdMsgRep=_GetMsgWinCANInfo(2)														; Fetch the third msg entry
		$FourthMsgRep=_GetMsgWinCANInfo(3)														; Fetch the fourth msg entry
		$FifthMsgRep=_GetMsgWinCANInfo(4)														; Fetch the fifth msg entry

		$RepFirstMsg=0
		if $FirstMsgRep[1]="Tx" and $FirstMsgRep[2]=2 and $FirstMsgRep[4]="0x100" Then
			$RepFirstMsg=1
		EndIf

		$RepSecMsg=0
		if $SecondMsgRep[1]="Tx" and $SecondMsgRep[2]=1 and $SecondMsgRep[4]="0x150" Then
			$RepSecMsg=1
		EndIf

		$RepThirdMsg=0
		if $ThirdMsgRep[1]="Tx" and $ThirdMsgRep[2]=2 and $ThirdMsgRep[4]="0x150" Then
			$RepThirdMsg=1
		EndIf

		$RepFourthMsg=0
		if $FourthMsgRep[1]="Tx" and $FourthMsgRep[2]=1 and $FourthMsgRep[4]="0x200" Then
			$RepFourthMsg=1
		EndIf

		$RepFifthMsg=0
		if $FifthMsgRep[1]="Tx" and $FifthMsgRep[2]=2 and $FifthMsgRep[4]="0x200" Then
			$RepFifthMsg=1
		EndIf


		if $RepFirstMsg=1 and $RepSecMsg=1 and $RepThirdMsg=1 and $RepFourthMsg=1 and $RepFifthMsg=1 Then
			$FileLoad=1
		EndIf

		if $FileLoad=1 Then
			_WriteResult("Pass","TS_Replay_05")
		Else
			_WriteResult("Fail","TS_Replay_05")
		EndIf
	Else
		_WriteResult("Fail","TS_Replay_05")
	EndIf
EndIf

$isAppNotRes=_CloseApp()																		; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_Replay_05")
EndIf


ConsoleWrite("****End : TS_Replay_05.au3****"&@CRLF)
ConsoleWrite(@CRLF)
