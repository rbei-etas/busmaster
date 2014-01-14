;=================================================================== Test Cases/Test Data =========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_Replay_18
; TestCases			:		Using the Filter Attributes
; Test Data			:		
; Test Setup		:		1. Open 'Configure Filters' dialog by menu option 'Configure -> App Filters'.
;~ 							2. Make a filter with the following attributes:
;~ 							3. Choose 'ID' radio box in 'Filter Attributes' group box.
;~ 							4. Select an 'ID' from the combobox or manually enter it.
;~ 							5. Choose 'ID Type' as 'std'.
;~ 							6. Choose 'Frame' as 'All'.
;~ 							7. Choose 'Direction' as 'Tx'.
;~ 							8. Choose 'Channel' as 'All'.
;~ 							9. Add the filter by clicking on Add button and close the 'Configure Filters' dialog by clicking on 'OK'.
;~ 							10. Open 'Configure Replay dialog' and add a new replay file.
;~ 							11. Choose the confiured filter by opening 'Filter Selection dialog'.
;~ 							12. Now click on connect to get the 'Replay Window' and observe the message entries in it. 

; Expected Results  : 		1. The chosen ID in step 4 along with filter attributes choosen should not be present in 'Replay Window'.
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_Replay_18.au3****"&@CRLF)


_launchApp()

Dim $FirstMsg[10],$SecondMsg[10],$ThirdMsg[10],$FourthMsg[10],$FifthMsg[10],$SixthMsg[10]

if winexists($WIN_BUSMASTER) then

	_loadConfig("cfxReplay_18")																	; Load Configuration

	_CANReplayMenu()																			; Open configure replay dialog

	_ADDReplayFile("Replay_18")																	; Add a replay file
	sleep(1000)

	ControlCommand($WIN_CANReplayConfig,"",$CHKB_Interactive_CANReplayConfig,"Check")			; Select Interactive replay

	_AddReplayFilters()																			; Add filter for replaying

	ControlClick($WIN_CANReplayConfig,"",$BTN_OK_CANReplayConfig)								; Click on 'OK' button

	_ReplayFilterMenu()																			; Select CAN->Replay->Enable Filters

	_DisableOverwriteMode()																		; Disable overwrite mode

;~ 	_EnableHex()																				; Enable Hex mode

	_ConnectDisconnect()																		; Connect the tool
	Sleep(1000)

	_CANReplayOptionsMenu($CANReplayGoMenu)														; Select "Go" from menu
	sleep(5000)

	_ConnectDisconnect()																		; Disconnect the tool
	sleep(1000)

	$MsgWinCount=_GetCANMsgWinItemCount()														; Fetch the Replay Msg window count
	consolewrite("$MsgWinCount :"&$MsgWinCount&@CRLF)

	$FirstMsg[4]=0
	$SecondMsg[4]=0
	$ThirdMsg[4]=0
	$FourthMsg[4]=0
	$FifthMsg[4]=0
	$SixthMsg[4]=0

	$MsgIDs=0
	$MsgDir=0
	$MsgCh=0

	$FirstMsg[2]=0
	$SecondMsg[2]=0
	$ThirdMsg[2]=0
	$FourthMsg[2]=0
	$FifthMsg[2]=0
	$SixthMsg[2]=0

	if $MsgWinCount=6 Then

		$FirstMsg=_GetMsgWinCANInfo(0)
		$SecondMsg=_GetMsgWinCANInfo(1)
		$ThirdMsg=_GetMsgWinCANInfo(2)
		$FourthMsg=_GetMsgWinCANInfo(3)
		$FifthMsg=_GetMsgWinCANInfo(4)
		$SixthMsg=_GetMsgWinCANInfo(5)

	EndIf

	consolewrite("$FirstMsg[4] :"&$FirstMsg[4]&@CRLF)
	consolewrite("$FirstMsg[2] :"&$FirstMsg[2]&@CRLF)
	consolewrite("$SecondMsg[4] :"&$SecondMsg[4]&@CRLF)
	consolewrite("$SecondMsg[2] :"&$SecondMsg[2]&@CRLF)
	consolewrite("$ThirdMsg[4] :"&$ThirdMsg[4]&@CRLF)
	consolewrite("$ThirdMsg[2] :"&$ThirdMsg[2]&@CRLF)
	consolewrite("$FourthMsg[4] :"&$FourthMsg[4]&@CRLF)
	consolewrite("$FourthMsg[2] :"&$FourthMsg[2]&@CRLF)
	consolewrite("$FifthMsg[4] :"&$FifthMsg[4]&@CRLF)
	consolewrite("$FifthMsg[2] :"&$FifthMsg[2]&@CRLF)
	consolewrite("$FifthMsg[1] :"&$FifthMsg[1]&@CRLF)
	consolewrite("$SixthMsg[4] :"&$SixthMsg[4]&@CRLF)
	consolewrite("$SixthMsg[2] :"&$SixthMsg[2]&@CRLF)
	consolewrite("$SixthMsg[1] :"&$SixthMsg[1]&@CRLF)

	if $FirstMsg[4]="0x100" and $SecondMsg[4]="0x100" and $ThirdMsg[4]="0x100" and $FourthMsg[4]="0x100" and $FifthMsg[4]="0x200" and $SixthMsg[4]="0x200" Then
		$MsgIDs=1
	EndIf

	if $FirstMsg[2]=1 and $SecondMsg[2]=2 and $ThirdMsg[2]=1 and $FourthMsg[2]=2 and $FifthMsg[2]=1 and  $SixthMsg[2]=2 Then
		$MsgDir=1
	EndIf

	if $FifthMsg[1]="Rx" and  $SixthMsg[1]="Tx" Then
		$MsgCh=1
	EndIf

	consolewrite("$MsgIDs :"&$MsgIDs&@CRLF)
	consolewrite("$MsgDir :"&$MsgDir&@CRLF)
	consolewrite("$MsgCh :"&$MsgCh&@CRLF)

	if $MsgIDs=1 and $MsgDir=1  and $MsgCh=1 Then
		_WriteResult("Pass","TS_Replay_18")
	Else
		_WriteResult("Fail","TS_Replay_18")
	EndIf

EndIf

$isAppNotRes=_CloseApp()																			; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_Replay_18")
EndIf


ConsoleWrite("****End : TS_Replay_18.au3****"&@CRLF)
ConsoleWrite(@CRLF)
