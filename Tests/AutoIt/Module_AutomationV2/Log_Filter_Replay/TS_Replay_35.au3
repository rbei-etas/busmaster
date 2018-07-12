;=================================================================== Test Cases/Test Data =========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_Replay_35
; TestCases			:		Using the Filter Attributes
; Test Data			:
; Test Setup		:		1. Open 'Configure Filters' dialog by menu option 'Configure -> App Filters'.
;~ 							2. Make a filter with the following attributes:
;~ 							3. Choose 'Range ' radio box in 'Filter Attributes' group box.
;~ 							4. Assign 'From' and 'To' ID values as 0x1 to 0xA.
;~ 							5. Choose 'ID Type' as 'std'.
;~ 							6. Choose 'Frame' as 'All'.
;~ 							7. Choose 'Direction' as 'Rx'.
;~ 							8. Choose 'Channel' as 'All'.
;~ 							9. Add the filter by clicking on Add button and close the 'Configure Filters' dialog by clicking on 'OK'.
;~ 							10. Open 'Configure Replay dialog' and add a new replay file.
;~ 							11. Choose the confiured filter by opening 'Filter Selection dialog'.
;~ 							12. Now click on connect to get the 'Replay Window' and observe the message entries in it.

; Expected Results  : 		1. The chosen ID range i.e from 0x1 to 0xA in step 4 along with filter attributes choosen should not be present in 'Replay Window'.
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_Replay_35.au3****"&@CRLF)


_launchApp()

Dim $FirstMsg[10],$SecondMsg[10],$ThirdMsg[10],$FourthMsg[10],$FifthMsg[10],$SixthMsg[10],$SeventhMsg[10],$EightMsg[10]

if winexists($WIN_BUSMASTER) then

	_loadConfig("cfxReplay_35")																	; Load Configuration

	_DisableOverwriteMode()																		; Disable overwrite mode

;~ 	_EnableHex()																				; Enable Hex mode

	;_ConnectDisconnect()																		; Connect the tool
	_Connect_CAN_Menu()
	Sleep(1000)

	_CANReplayOptionsMenu($CANReplayGoMenu)														; Select "Go" from menu
	sleep(6000)

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
	$SeventhMsg[4]=0
	$EightMsg[4]=0

	$MsgIDs=0
	$MsgDir=0
	$MsgCh=0

	$FirstMsg[2]=0
	$SecondMsg[2]=0
	$ThirdMsg[2]=0
	$FourthMsg[2]=0
	$FifthMsg[2]=0
	$SixthMsg[2]=0
	$SeventhMsg[2]=0
	$EightMsg[2]=0

	if $MsgWinCount=8 Then

		$FirstMsg=_GetMsgWinCANInfo(0)															; Fetch the msg win first row info
		$SecondMsg=_GetMsgWinCANInfo(1)															; Fetch the msg win second row info
		$ThirdMsg=_GetMsgWinCANInfo(2)															; Fetch the msg win third row info
		$FourthMsg=_GetMsgWinCANInfo(3)															; Fetch the msg win fourth row info
		$FifthMsg=_GetMsgWinCANInfo(4)															; Fetch the msg win fifth row info
		$SixthMsg=_GetMsgWinCANInfo(5)															; Fetch the msg win sixth row info
		$SeventhMsg=_GetMsgWinCANInfo(6)														; Fetch the msg win seventh row info
		$EightMsg=_GetMsgWinCANInfo(7)															; Fetch the msg win eight row info

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
	consolewrite("$SeventhMsg[4] :"&$SeventhMsg[4]&@CRLF)
	consolewrite("$SeventhMsg[2] :"&$SeventhMsg[2]&@CRLF)
	consolewrite("$SeventhMsg[1] :"&$SeventhMsg[1]&@CRLF)
	consolewrite("$EightMsg[4] :"&$EightMsg[4]&@CRLF)
	consolewrite("$EightMsg[2] :"&$EightMsg[2]&@CRLF)
	consolewrite("$EightMsg[1] :"&$EightMsg[1]&@CRLF)

	if $FirstMsg[4]="0x100" and $SecondMsg[4]="0x100" and $ThirdMsg[4]="0x100" and $FourthMsg[4]="0x100" and $FifthMsg[4]="0x150" and $SixthMsg[4]="0x150" and $SeventhMsg[4]="0x200" and $EightMsg[4]="0x200" Then
		$MsgIDs=1
	EndIf
	#cs
	if $FirstMsg[2]=1 and $SecondMsg[2]=2 and $ThirdMsg[2]=2 and $FourthMsg[2]=1 and $FifthMsg[2]=1 and  $SixthMsg[2]=2 and $SeventhMsg[2]=1 and $EightMsg[2]=2 Then
		$MsgDir=1
	EndIf

	if $FifthMsg[1]="Tx" and  $SixthMsg[1]="Rx" and $SeventhMsg[1]="Tx" and  $EightMsg[1]="Rx" Then
		$MsgCh=1
	EndIf
	#ce
	;-------------------updated below to avoid virtual controller issue of Tx and Rx messages
	if $FirstMsg[1] = "Tx" Then
		if $FirstMsg[2]=1 and $SecondMsg[2]=2 and $ThirdMsg[2]=2 and $FourthMsg[2]=1 and $FifthMsg[2]=1 and  $SixthMsg[2]=2 and $SeventhMsg[2]=1 and $EightMsg[2]=2 Then
			$MsgDir=1
		EndIf
	ElseIf $FirstMsg[1] = "Rx" Then
		if $FirstMsg[2]=2 and $SecondMsg[2]=1 and $ThirdMsg[2]=1 and $FourthMsg[2]=2 and $FifthMsg[2]=2 and  $SixthMsg[2]=1 and $SeventhMsg[2]=2 and $EightMsg[2]=1 Then
			$MsgDir=1
		EndIf
	EndIf
	if $FifthMsg[1]="Tx" and  $SixthMsg[1]="Rx" and $SeventhMsg[1]="Tx" and  $EightMsg[1]="Rx" Then
		$MsgCh=1
	ElseIf $FifthMsg[1]="Rx" and  $SixthMsg[1]="Tx" and $SeventhMsg[1]="Rx" and  $EightMsg[1]="Tx" Then
		$MsgCh=1
	EndIf
	;----------------------------------------------------------------------------------------
	consolewrite("$MsgIDs :"&$MsgIDs&@CRLF)
	consolewrite("$MsgDir :"&$MsgDir&@CRLF)
	consolewrite("$MsgCh :"&$MsgCh&@CRLF)

	if $MsgIDs=1 and $MsgDir=1  and $MsgCh=1 Then
		_WriteResult("Pass","TS_Replay_35")
	Else
		_WriteResult("Fail","TS_Replay_35")
	EndIf

EndIf

$isAppNotRes=_CloseApp()																			; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_Replay_35")
EndIf


ConsoleWrite("****End : TS_Replay_35.au3****"&@CRLF)
ConsoleWrite(@CRLF)
