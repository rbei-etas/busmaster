;=================================================================== Test Cases/Test Data =========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_Replay_01
; TestCases			:		OK and Cancel functionality
; Test Data			:		
; Test Setup		:		1. Select Replay configuration dialog.
;~ 							2. Remove all entries in that.
;~ 							3. Select Cancel button to cancel changes.
;~ 							4. Select the dialog again and check the entries in that.
;~ 							5. Delete all items again and select OK to save changes.
;~ 							6. Repeat step 4.

; Expected Results  : 		1. After step 3, the replay file list will remain unchanged as the changes done in step 2 is canceled.
;~ 							2. After step 6, the replay list will be empty as changes done in step 5 is saved.
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_Replay_01.au3****"&@CRLF)


_launchApp()
if winexists($WIN_BUSMASTER) then
	_loadConfig("cfxReplay_01")																				; Load Configuration

	_CANReplayMenu()																						; Open configure replay dialog

	_DELReplayFile()																						; Delete replay file

	ControlClick($WIN_CANReplayConfig,"",$BTN_Cancel_CANReplayConfig)										; Click on 'Cancel' button

	_CANReplayMenu()																						; Open configure replay dialog

	$Count1=ControlListView ($WIN_CANReplayConfig,"",$LVC_ReplayFile_CANReplayConfig,"GetItemCount")		; Fetch the count of files
	$FilePath1=ControlListView ($WIN_CANReplayConfig,"",$LVC_ReplayFile_CANReplayConfig,"GetText",0,0)		; Fetch the file path

	ConsoleWrite("$Count1 : "&$Count1&@CRLF)
	ConsoleWrite("$FilePath1 : "&$FilePath1&@CRLF)

	$DelBTNState1=ControlCommand($WIN_CANReplayConfig,"",$BTN_DELFile_CANReplayConfig,"IsEnabled")			; Fetch the state of the Delete button

	ConsoleWrite("$DelBTNState1 : "&$DelBTNState1&@CRLF)

	if $Count1=1 and $DelBTNState1=1 Then
		$CancelFunc="Yes"
	else
		$CancelFunc="No"
	EndIf
	ConsoleWrite("$CancelFunc : "&$CancelFunc&@CRLF)

	_DELReplayFile()																						; Delete replay file

	ControlClick($WIN_CANReplayConfig,"",$BTN_OK_CANReplayConfig)											; Click on 'OK' button

	_CANReplayMenu()																						; Open configure replay dialog

	ControlClick($WIN_CANReplayConfig,"",$BTN_Cancel_CANReplayConfig)										; Click on 'Cancel' button

	$Count2=ControlListView ($WIN_CANReplayConfig,"",$LVC_ReplayFile_CANReplayConfig,"GetItemCount")		; Fetch the count of files
	$FilePath2=ControlListView ($WIN_CANReplayConfig,"",$LVC_ReplayFile_CANReplayConfig,"GetText",0,0)		; Fetch the file path

	ConsoleWrite("$Count2 : "&$Count2&@CRLF)
	ConsoleWrite("$FilePath2 : "&$FilePath2&@CRLF)

	$DelBTNState2=ControlCommand($WIN_CANReplayConfig,"",$BTN_DELFile_CANReplayConfig,"IsEnabled")			; Fetch the state of the Delete button

	ConsoleWrite("$DelBTNState2 : "&$DelBTNState2&@CRLF)

	if $Count2=0 and $DelBTNState2=0 Then
		$OKFunc="Yes"
	else
		$OKFunc="No"
	EndIf

	ConsoleWrite("$OKFunc : "&$OKFunc&@CRLF)
EndIf

if $CancelFunc="Yes" and $OKFunc="Yes" Then
	_WriteResult("Pass","TS_Replay_01")
Else
	_WriteResult("Fail","TS_Replay_01")
EndIf

$isAppNotRes=_CloseApp()																					; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_Replay_01")
EndIf

ConsoleWrite("****End : TS_Replay_01.au3****"&@CRLF)
ConsoleWrite(@CRLF)






