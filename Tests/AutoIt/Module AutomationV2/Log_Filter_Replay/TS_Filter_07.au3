;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_Filter_07
; TestCases			:		Applying display Filter
; Test Data			:
; Test Setup		:		1. Continue from the previous test case.
;~ 							2. Configure TX messages such that it will fall under the message ID and range or the filter.
;~ 							3. Disable display filtering if it is enabled.
;~ 							4. Connect the tool and start message transmission.
;~ 							5. Check for the message that are falling in the filter range and ID list.
;~ 							6. Now enable display filter button in the toolbar.
;~ 							7. Clear message display.
;~ 							8. Now check again for the message that are falling in the filter range and ID list.

; Expected Results  : 		1. After step 5, all messages that are configured during step 2 will be displayed.
;~ 							2. After step 8, as the filter is enabled, the messages that are falling with in filter range and ID will not be displayed in Message Display.
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_Filter_07.au3****"&@CRLF)


_launchApp()																	; Check if the App is open else launch it

Local $MsgWinItemCount1=0,$MsgWinItemCount2=0
Dim $Msg1[10]=["","","","","","","","","",""],$Msg2[10]=["","","","","","","","","",""]
if winexists($WIN_BUSMASTER) then

	_loadConfig("cfxFilter_07")													; Load Cfx file

;~ 	_En_Dis_FilterDisplay()														; Disable Message Filter for display

	_ConnectDisconnect()														; Connect the tool

	_TransmitMsgsMenu()															; Transmit normal blocks
	sleep(3000)

	_ConnectDisconnect()														; Disconnect the tool
	sleep(2000)

	_EnableOverwriteMode()														; Enable overwrite mode
	sleep(1000)

	_DisableJ1939Win()															; If J1939 Msg Window is active then disable it
	sleep(2000)

;~ 	_EnableHex()																; Enable Hex display mode

	$MsgWinItemCount1=_GetCANMsgWinItemCount()									; Fetch the item count in the msg window

	if $MsgWinItemCount1=2 Then
		$Msg1=_GetMsgWinCANInfo(0)												; Fetch the first msg info
	EndIf

	if $Msg1[1]="Tx" and $Msg1[4]=0021 Then										; Validate the result
		$withoutMsgFilter="True"
	Else
		$withoutMsgFilter="False"
	EndIf

	_En_Dis_FilterDisplay()														; Enable Message Filter for display

	_ConnectDisconnect()														; Connect the tool

	_TransmitMsgsMenu()															; Transmit normal blocks
	sleep(3000)

	_ConnectDisconnect()														; Disconnect the tool
	Sleep(2000)

	_DisableJ1939Win()															; If J1939 Msg Window is active then disable it
	sleep(2000)

	$MsgWinItemCount2=_GetCANMsgWinItemCount()									; Fetch the item count in the msg window

	if $MsgWinItemCount2=1 Then
		$Msg2=_GetMsgWinCANInfo(0)												; Fetch the first msg info
	EndIf

	if $Msg2[1]="Rx" and $Msg2[4]=0021 Then										; Validate the result
		$withMsgFilter="True"
	Else
		$withMsgFilter="False"
	EndIf

EndIf
consolewrite("$MsgWinItemCount1:"&$MsgWinItemCount1&@CRLF)
consolewrite("$MsgWinItemCount2:"&$MsgWinItemCount2&@CRLF)
consolewrite("$Msg1[1]:"&$Msg1[1]&@CRLF)
consolewrite("$Msg1[4]:"&$Msg1[4]&@CRLF)
consolewrite("$Msg2[1]:"&$Msg2[1]&@CRLF)
consolewrite("$Msg2[4]:"&$Msg2[4]&@CRLF)
ConsoleWrite("$withoutMsgFilter=" &$withoutMsgFilter&@CRLF)
ConsoleWrite("$withMsgFilter=" &$withMsgFilter&@CRLF)

if $withoutMsgFilter="True" and $withMsgFilter="True" Then						; Validate the result
	_WriteResult("Pass","TS_Filter_07")
Else
	_WriteResult("Fail","TS_Filter_07")
EndIf

$isAppNotRes=_CloseApp()														; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_Filter_07")
EndIf



ConsoleWrite("****End : TS_Filter_07.au3****"&@CRLF)
ConsoleWrite(@CRLF)


