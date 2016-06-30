;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_NwSt_03
; TestCases			:		-
; Test Data			:
; Test Setup		:		1. Activate the network statistics window.
;~ 							2. Connect the Busmaster to the Bus.
;~ 							3. observe the values in window
;~ 							4. Start sending messages.
;~ 							5. Observe the Network statistics values.

; Expected Results  : 		1. After step 2 the network statistics will be initialised to zero.
;~ 							2. After step5,the network statistics values will be updated for every second

; TestCase No.		:		TS_NwSt_04
; TestCases			:		Network statistics initialisation with Disconnect\Connect sequence.
; Test Data			:
; Test Setup		:		1. Continue from the above test case .
;~ 							2. Disconnect the Busmaster from the Bus.obserrve the network statistics values.
;~ 							3. Again Connect the Busmaster to the bus.

; Expected Results  : 		1. After step 2 the network ststistics will not change.
;~ 							2. After step3 network statistics will initialised to zero.
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_NwSt_03/04.au3****"&@CRLF)


_launchApp()

WinActivate($WIN_BUSMASTER,3)
Local $bTransmission=0, $aTransmission=0,$aDisconnect=0

if winexists($mWin) then
	_loadConfig("cfx_NwSt_03")											; Load Configuration

	_CANNWStatsMenu()															; Open Network Statistics winodow

	;_ConnectDisconnect()														; Connect
	sleep(1500)

	$TotalMsg=_GetNWStatsInfo(0)												; Fetch the total message info
	consolewrite("$TotalMsg[0]: "&$TotalMsg[0]&@CRLF)
	consolewrite("$TotalMsg[1]: "&$TotalMsg[1]&@CRLF)

	$MsgPSec=_GetNWStatsInfo(1)													; Fetch the messages per second info
	consolewrite("$MsgPSec[0]: "&$MsgPSec[0]&@CRLF)
	consolewrite("$MsgPSec[1]: "&$MsgPSec[1]&@CRLF)

	if $TotalMsg[0]="Messages[Total]" and $TotalMsg[1]=0 and $MsgPSec[0]="Messages[Msg/s]" and $MsgPSec[1]=0 Then
		$bTransmission=1
	EndIf

	;_TransmitMsgsMenu()															; Transmit CAN messages
	_ConnectDisconnect()
	sleep(1500)
	sleep(3000)

	$TotalMsg_Trans1=_GetNWStatsInfo(0)											; Fetch the total message info
	consolewrite("$TotalMsg_Trans1[0]: "&$TotalMsg_Trans1[0]&@CRLF)
	consolewrite("$TotalMsg_Trans1[1]: "&$TotalMsg_Trans1[1]&@CRLF)
	;sleep(1000)
	sleep(3000)

	$TotalMsg_Trans2=_GetNWStatsInfo(0)											; Fetch the total message info after 1 sec
	consolewrite("$TotalMsg_Trans2[0]: "&$TotalMsg_Trans2[0]&@CRLF)
	consolewrite("$TotalMsg_Trans2[1]: "&$TotalMsg_Trans2[1]&@CRLF)
	sleep(1000)
	_ConnectDisconnect()														; Disconnect
	sleep(1500)

	$TotalMsg_Trans3=_GetNWStatsInfo(0)											; Fetch the total message info after disconnect
	consolewrite("$TotalMsg_Trans3[0]: "&$TotalMsg_Trans3[0]&@CRLF)
	consolewrite("$TotalMsg_Trans3[1]: "&$TotalMsg_Trans3[1]&@CRLF)

	_CANNWStatsMenu()															; Open Network Statistics winodow

	;if $TotalMsg_Trans2[1] > $TotalMsg_Trans1[1] Then
	if ($TotalMsg_Trans2[1] - $TotalMsg_Trans1[1])>0 Then
		$aTransmission=1
	EndIf

	;if $TotalMsg_Trans3[1] > $TotalMsg_Trans2[1] Then
	if ($TotalMsg_Trans3[1] - $TotalMsg_Trans2[1])>0 Then
		$aDisconnect=1
	EndIf

	WinClose($WIN_NW_Stats)														; Close network statistics window

EndIf

consolewrite("$bTransmission: "&$bTransmission&@CRLF)
consolewrite("$aTransmission: "&$aTransmission&@CRLF)
consolewrite("$aDisconnect: "&$aDisconnect&@CRLF)

if $bTransmission=1 and $aTransmission=1 Then
	_WriteResult("Pass","TS_NwSt_03")
Else
	_WriteResult("Fail","TS_NwSt_03")
EndIf

if $aDisconnect=1 Then
	_WriteResult("Pass","TS_NwSt_04")
Else
	_WriteResult("Fail","TS_NwSt_04")
EndIf

$isAppNotRes=_CloseApp()

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_NwSt_03")
	_WriteResult("Warning","TS_NwSt_04")
EndIf



ConsoleWrite("****End : TS_NwSt_03/04.au3****"&@CRLF)
ConsoleWrite(@CRLF)
