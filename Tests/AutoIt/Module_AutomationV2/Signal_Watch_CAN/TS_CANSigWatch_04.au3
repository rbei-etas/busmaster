;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_CANSigWatch_04
; TestCases			:		"Signal watch window menu item testing"
; Test Data			:
; Test Setup		:		1. Continue from the Testcase TS_SigWatch_03
;~ 							2. Right Click on the list control of Signal watch window.
;~ 							3. Click the menu item clear.
;~ 							4. Observe the contents of the signal watch window.
;~ 							5.Stop the CANvas transmisson.
;~ 							6. Right click on the signal watch window and select the menu item Clear.

; Expected Results  : 		1. If the signal watch window have some signal in it a menu containg menu item "Clear" will be appeared.
;~ 							2. After step 4  All the elements in the signal watch window will be deleted.
;~ 							3. Again signal watch window will show the currently configured message values after 1 second.
;~ 							4. After step 6 All the elements in the signal watch window will be deleted.
;~ 							5. The signal watch window will be on this status indefinetly
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_CANSigWatch_04.au3****"&@CRLF)

_launchApp()

WinActivate($WIN_BUSMASTER,3)

Local $Clear=0,$aClear=0,$aDisconnect=0
if winexists($WIN_BUSMASTER) then

	_loadConfig("cfxCANSigWatch_04")																		; Load Configuration

	_ConnectDisconnect()																					; Connect

	_TransmitMsgsMenu()																						; Transmit CAN messages
	sleep(1000)

	_SignalWatchMenu($CAN,$SigWatchActivateMenu)															; Open signal watch window

	$HWD=ControlGetHandle($WIN_SigWatch&$CAN,"",$LVC_Value_SigWatch)										; Fetch the handle of Signal watch window
	$ItemCount=_GUICtrlListView_GetItemCount($HWD)															; Fetch the count of items

	if $ItemCount=1 Then

		_GUICtrlListView_ClickItem($HWD,0,"Right")															; Right click on the signal watch window
		sleep(500)

		send("c")																							; select clear

		$MsgStdLil=_GetSigWatchWinInfo($CAN,0)																; Fetch the signal watch window info immidiately after clearing the signal

		if $MsgStdLil[1]="" and $MsgStdLil[2]="" and $MsgStdLil[3]="" and $MsgStdLil[4]="" Then
			$Clear=1
		EndIf

		sleep(1000)

		$MsgStdLil1=_GetSigWatchWinInfo($CAN,0)																; Fetch the signal watch window info after clearing the signal

		if $MsgStdLil1[1]="MsgStdLil" and $MsgStdLil1[2]="Sigstd1" and $MsgStdLil1[3]=0.000 and $MsgStdLil1[4]=0 Then
			$aClear=1
		EndIf

		_ConnectDisconnect()																				; Disconnect

		sleep(1000)

		_GUICtrlListView_ClickItem($HWD,0,"Right")															; Right click on the signal watch window
		sleep(500)

		send("c")																							; select clear

		$MsgStdLil2=_GetSigWatchWinInfo($CAN,0)																	; Fetch the signal watch window info after clearing the signal

		if $MsgStdLil2[1]="" and $MsgStdLil2[2]="" and $MsgStdLil2[3]="" and $MsgStdLil2[4]="" Then
			$aDisconnect=1
		EndIf

	EndIf

	winclose($WIN_SigWatch)																					; Close the signal watch window
EndIf
ConsoleWrite("$ItemCount : "&$ItemCount&@CRLF)
ConsoleWrite("$Clear : "&$Clear&@CRLF)
ConsoleWrite("$aClear : "&$aClear&@CRLF)
ConsoleWrite("$aDisconnect : "&$aDisconnect&@CRLF)

if $Clear=1 and $aClear=1 and $aDisconnect=1 Then
	_WriteResult("Pass","TS_CANSigWatch_04")
Else
	_WriteResult("Fail","TS_CANSigWatch_04")
EndIf

$isAppNotRes=_CloseApp()																					; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_CANSigWatch_04")
EndIf



ConsoleWrite("****End : TS_CANSigWatch_04.au3****"&@CRLF)
ConsoleWrite(@CRLF)
