;=================================================================== Test Cases/Test Data =========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_Replay_02
; TestCases			:		Functionality - Interactive Replay - File Loading
; Test Data			:
; Test Setup		:		1. Select Replay configuration dialog.
;~ 							2. Select a valid log file and select interactive option. Set replay mode to Once and time mode to Retain delay options.
;~ 							3. Select OK to save changes.
;~ 							4. Connect the tool.
;~ 							5. Check for replay window.
;~ 							6. Check the content of the replay window list.

; Expected Results  : 		1. After step 5, replay window will be displayed with the replay file path as window title.
;~ 							2. After step 6, the content of the replay window list will be same as the content of the log file (If there is no mode mismatch)
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_Replay_02.au3****"&@CRLF)


_launchApp()

if winexists($WIN_BUSMASTER) then

	_loadConfig("cfxReplay_02")																	; Load Configuration

	_CANReplayMenu()																			; Open configure replay dialog

	_ADDReplayFile("Replay_02")																	; Add a replay file

	ControlCommand($WIN_CANReplayConfig,"",$CHKB_Interactive_CANReplayConfig,"Check")			; Select Interactive replay

	ControlClick($WIN_CANReplayConfig,"",$BTN_OK_CANReplayConfig)								; Click on 'OK' button

	_ConnectDisconnect()																		; Connect the tool
	sleep(1500)

	$GetLogFile_Path=_TestDataPath()															; Fetch the Log file path
	$txt=_FileCountLines ($GetLogFile_Path & "\Replay_02.log")
	$TotalLogMsgCount=$txt-2-15																	; Fetch the message count in the log file
	consolewrite("$TotalLogMsgCount :"&$TotalLogMsgCount&@CRLF)

	$HWD= ControlGetHandle($WIN_BUSMASTER,"",$LVC_ReplayWin)									; Fetch the handle of replay list view instance
	$RepWinMsgCount=_GUICtrlListView_GetItemCount($HWD)											; Fetch the Replay Msg window count
	consolewrite("$RepWinMsgCount :"&$RepWinMsgCount&@CRLF)
	$FileLoad=0

	if $TotalLogMsgCount=$RepWinMsgCount Then

		$FirstMsgLog=FileReadLine ($GetLogFile_Path & "\Replay_02.log",16)						; Read the 16th line from the Log file
		$SecondMsgLog=FileReadLine ($GetLogFile_Path & "\Replay_02.log",17)						; Read the 17th line from the Log file
		$ThirdMsgLog=FileReadLine ($GetLogFile_Path & "\Replay_02.log",18)						; Read the 18th line from the Log file
		$FourthMsgLog=FileReadLine ($GetLogFile_Path & "\Replay_02.log",19)						; Read the 19th line from the Log file
		$FifthMsgLog=FileReadLine ($GetLogFile_Path & "\Replay_02.log",20)						; Read the 20th line from the Log file
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

		$FirstMsgRep=_GetReplayWinInfo(0)														; Fetch the first msg entry
		$SecondMsgRep=_GetReplayWinInfo(1)														; Fetch the second msg entry
		$ThirdMsgRep=_GetReplayWinInfo(2)														; Fetch the third msg entry
		$FourthMsgRep=_GetReplayWinInfo(3)														; Fetch the fourth msg entry
		$FifthMsgRep=_GetReplayWinInfo(4)														; Fetch the fifth msg entry
		consolewrite("$FirstMsgRep :"&$FirstMsgRep&@CRLF)
		consolewrite("$SecondMsgRep :"&$SecondMsgRep&@CRLF)
		consolewrite("$ThirdMsgRep :"&$ThirdMsgRep&@CRLF)
		consolewrite("$FourthMsgRep :"&$FourthMsgRep&@CRLF)
		consolewrite("$FifthMsgRep :"&$FifthMsgRep&@CRLF)

		if $FirstMsgLogTrim=$FirstMsgRep and $SecondMsgLogTrim=$SecondMsgRep and $ThirdMsgLogTrim=$ThirdMsgRep and $FourthMsgLogTrim=$FourthMsgRep and $FifthMsgLogTrim=$FifthMsgRep Then
			$FileLoad=1
		EndIf
		if $FileLoad=1 Then
			_WriteResult("Pass","TS_Replay_02")
		Else
			_WriteResult("Fail","TS_Replay_02")
		EndIf
	Else
		_WriteResult("Fail","TS_Replay_02")
	EndIf
	_ConnectDisconnect()																		; Disconnect the tool
EndIf

$isAppNotRes=_CloseApp()																		; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_Replay_02")
EndIf

ConsoleWrite("****End : TS_Replay_02.au3****"&@CRLF)
ConsoleWrite(@CRLF)






