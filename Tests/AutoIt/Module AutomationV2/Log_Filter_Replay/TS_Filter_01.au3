;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_Filter_01
; TestCases			:		Validity of filter list for message window
; Test Data			:
; Test Setup		:		1. Invoke dialog box for filter configuration
;~ 							2. Enter ID of a message to update the filter list
;~ 							3. Confirm by pressing OK button
;~ 							4. Enable message filter
;~ 							5. Send the same message across CAN bus

; Expected Results  : 		"The message ID shouldn't appear in the message window"
;==========================================================================Test Procedure =========================================================================


ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_Filter_01.au3****"&@CRLF)


_launchApp()

WinActivate($WIN_BUSMASTER)
Local $MsgWinItemCount=0,$Msg1[5]=["","","","",""]
if winexists($WIN_BUSMASTER) then
	_loadConfig("cfxFilter_01")													; Load Configuration

	_AddFilter("Stop","[0x15]Msg1","","","Std","Non RTR","Tx",1)				; Configure Filter details
	sleep(1000)

	_MsgDisplayMenu()															; Open Message Display dialog

	_AddFiltertoMsgDisp()														; Add the filter to Message display
	sleep(1000)

	_En_Dis_FilterDisplay()														; Enable filters for message display

	_ConnectDisconnect()														; Connect the tool

	_TransmitMsgsMenu()															; Transmit normal blocks
	sleep(3000)

	_ConnectDisconnect()														; Disconnect the tool

	Sleep(2000)
	_DisableJ1939Win()															; If J1939 Msg Window is active then disable it

	_EnableHex()																; Enable Hex display mode

	sleep(1200)

	$MsgWinItemCount=_GetCANMsgWinItemCount()									; Fetch the item count in the msg window

	if $MsgWinItemCount>0 Then
		$Msg1=_GetMsgWinCANInfo(0)													; Fetch the first msg info
	EndIf
EndIf
ConsoleWrite("$Msg1[2]:"&$Msg1[2]&@CRLF)
ConsoleWrite("$Msg1[1]:"&$Msg1[1]&@CRLF)
ConsoleWrite("$Msg1[4]:"&$Msg1[4]&@CRLF)

if $Msg1[2]=2 and $Msg1[1]="Rx" and $Msg1[4]=0x015 Then							; Validate the result
	_WriteResult("Pass","TS_Filter_01")
Else
	_WriteResult("Fail","TS_Filter_01")
EndIf

$isAppNotRes=_CloseApp()														; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_Filter_01")
EndIf

ConsoleWrite("****End : TS_Filter_01.au3****"&@CRLF)
ConsoleWrite(@CRLF)