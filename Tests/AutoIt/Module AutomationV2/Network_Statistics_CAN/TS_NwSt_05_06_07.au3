;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_NwSt_05
; TestCases			:		Network statistics window verification
; Test Data			:
; Test Setup		:		1. Continue from the aboce test case.
;~ 							2. Start transmitting messages.
;~ 							3. Verify the Transmitted message statistics.

; Expected Results  : 		1. Total = Messages Transmitted.
;~ 							2. Standard [Total] = Total Std msg transmitted.
;~ 							3. Standard [msg\s]= Total msgs transmitted per second.
;~ 							4. Extended [Total] = Total extended msg.
;~ 							5. Extended [Msg/s] = Total extended msgs per second

; TestCase No.		:		TS_NwSt_06
; TestCases			:		Network statistics window verification
; Test Data			:
; Test Setup		:		1. Continue from the aboce test case.
;~ 							2. Verify the Load, Peak, Average Load

; Expected Results  : 		The Load, peak load and average load of respective channels is displayed in network statistics window

; TestCase No.		:		TS_NwSt_07
; TestCases			:		Network statistics Update.
; Test Data			:
; Test Setup		:		1. Continue from the above test case
;~ 							2. Close the Network statistics window.
;~ 							3. Activate the network statistics window.
;~ 							4. Observe the Network statistics.

; Expected Results  : 		1. After step 4 the network statistics will show the updated network statistics.
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_NwSt_05/06/07.au3****"&@CRLF)


_launchApp()

WinActivate($WIN_BUSMASTER,3)
Local $TotalMsgs=0, $TotalErrors=0,$Total_TxSTDMsgs=0,$TxSTDMsgsPSec=0,$Total_TxEXTMsgs=0,$TxEXTMsgsPSec=0,$Load=0,$pLoad=0,$avLoad=0
Local $uTotalMsgs=0, $uTotalErrors=0,$uTotal_TxSTDMsgs=0,$uTxSTDMsgsPSec=0,$uTotal_TxEXTMsgs=0,$uTxEXTMsgsPSec=0,$uLoad=0,$upLoad=0,$uavLoad=0
if winexists($mWin) then
	_loadConfig("cfx_NwSt_05")													; Load Configuration

	_CANNWStatsMenu()															; Open Network Statistics window

	_ConnectDisconnect()														; Connect
	sleep(1500)

	_TransmitMsgsMenu()															; Transmit CAN messages
	sleep(3000)

	_ConnectDisconnect()														; Disconnect
	sleep(1500)

	; Fetch Total Messages row info
	$TotalMsgs_Parameter=_GetNWStatsInfo(0)
 	if $TotalMsgs_Parameter[0]="Messages[Total]" and $TotalMsgs_Parameter[1]>14000 and $TotalMsgs_Parameter[2]>14000 Then
		$TotalMsgs=1
	EndIf

	; Fetch Total Error Messages row info
	$TotalErrs_Parameter=_GetNWStatsInfo(2)
	if $TotalErrs_Parameter[0]="Errors[Total]" and $TotalErrs_Parameter[1]=0 and $TotalErrs_Parameter[2]=0 Then
		$TotalErrors=1
	EndIf

	; Fetch Transmitted Total STD Messages row info
	$Tx_STDTotal_Parameter=_GetNWStatsInfo(9)
	if $Tx_STDTotal_Parameter[0]="Standard[Total]" and $Tx_STDTotal_Parameter[1]>7000 Then
		$Total_TxSTDMsgs=1
	EndIf

	; Fetch Transmitted STD Messages per sec row info
	$Tx_STDMsgsPSec_Parameter=_GetNWStatsInfo(10)
	if $Tx_STDMsgsPSec_Parameter[0]="Standard[Msg/s]" and $Tx_STDMsgsPSec_Parameter[1]>4000 Then
		$TxSTDMsgsPSec=1
	EndIf

	; Fetch Transmitted Total EXT Messages row info
	$Tx_EXTTotal_Parameter=_GetNWStatsInfo(11)
	if $Tx_EXTTotal_Parameter[0]="Extended[Total]" and $Tx_EXTTotal_Parameter[1]>7000 Then
		$Total_TxEXTMsgs=1
	EndIf

	; Fetch Transmitted EXT Messages per sec row info
	$Tx_EXTMsgsPSec_Parameter=_GetNWStatsInfo(12)
	if $Tx_EXTMsgsPSec_Parameter[0]="Extended[Msg/s]" and $Tx_EXTMsgsPSec_Parameter[1]>4000 Then
		$TxEXTMsgsPSec=1
	EndIf

	; Fetch Load row info
	$Load_Parameter=_GetNWStatsInfo(4)
	$Load_Channel1=StringStripWS($Load_Parameter[1],8)
	$Load_Channel2=StringStripWS($Load_Parameter[2],8)
	if $Load_Parameter[0]="Load" and $Load_Channel1="99.99%" and $Load_Channel2="99.99%" Then
		$Load=1
	EndIf

	; Fetch Peak Load row info
	$pLoad_Parameter=_GetNWStatsInfo(5)
	$pLoad_Channel1=StringStripWS($pLoad_Parameter[1],8)
	$pLoad_Channel2=StringStripWS($pLoad_Parameter[2],8)
	if $pLoad_Parameter[0]="PeakLoad" and $pLoad_Channel1="99.99%" and $pLoad_Channel2="99.99%" Then
		$pLoad=1
	EndIf

	; Fetch Average Load row info
	$avLoad_Parameter=_GetNWStatsInfo(6)
	$avLoad_Channel1=StringStripWS($avLoad_Parameter[1],8)
	$avLoad_Channel2=StringStripWS($avLoad_Parameter[2],8)
	if $avLoad_Parameter[0]="AverageLoad" and $avLoad_Channel1>="25%" and $avLoad_Channel2>="25%" Then
		$avLoad=1
	EndIf

	_CANNWStatsMenu()															; Close Network Statistics window

	_CANNWStatsMenu()															; Open Network Statistics window

	; Fetch Total Messages row info
	$uTotalMsgs_Parameter=_GetNWStatsInfo(0)
 	if $uTotalMsgs_Parameter[0]=$TotalMsgs_Parameter[0] and $uTotalMsgs_Parameter[1]=$TotalMsgs_Parameter[1] and $uTotalMsgs_Parameter[2]=$TotalMsgs_Parameter[2] Then
		$uTotalMsgs=1
	EndIf

	; Fetch Total Error Messages row info
	$uTotalErrs_Parameter=_GetNWStatsInfo(2)
	if $uTotalErrs_Parameter[0]=$TotalErrs_Parameter[0] and $uTotalErrs_Parameter[1]=$TotalErrs_Parameter[1] and $uTotalErrs_Parameter[2]=$TotalErrs_Parameter[2] Then
		$uTotalErrors=1
	EndIf

	; Fetch Transmitted Total STD Messages row info
	$uTx_STDTotal_Parameter=_GetNWStatsInfo(9)
	if $uTx_STDTotal_Parameter[0]=$Tx_STDTotal_Parameter[0] and $uTx_STDTotal_Parameter[1]=$Tx_STDTotal_Parameter[1] and $uTx_STDTotal_Parameter[2]=$Tx_STDTotal_Parameter[2] Then
		$uTotal_TxSTDMsgs=1
	EndIf

	; Fetch Transmitted STD Messages per sec row info
	$uTx_STDMsgsPSec_Parameter=_GetNWStatsInfo(10)
	if $uTx_STDMsgsPSec_Parameter[0]=$Tx_STDMsgsPSec_Parameter[0] and $uTx_STDMsgsPSec_Parameter[1]=$Tx_STDMsgsPSec_Parameter[1] and $uTx_STDMsgsPSec_Parameter[2]=$Tx_STDMsgsPSec_Parameter[2] Then
		$uTxSTDMsgsPSec=1
	EndIf

	; Fetch Transmitted Total EXT Messages row info
	$uTx_EXTTotal_Parameter=_GetNWStatsInfo(11)
	if $uTx_EXTTotal_Parameter[0]=$Tx_EXTTotal_Parameter[0] and $uTx_EXTTotal_Parameter[1]=$Tx_EXTTotal_Parameter[1] and $uTx_EXTTotal_Parameter[2]=$Tx_EXTTotal_Parameter[2] Then
		$uTotal_TxEXTMsgs=1
	EndIf

	; Fetch Transmitted EXT Messages per sec row info
	$uTx_EXTMsgsPSec_Parameter=_GetNWStatsInfo(12)
	if $uTx_EXTMsgsPSec_Parameter[0]=$Tx_EXTMsgsPSec_Parameter[0] and $uTx_EXTMsgsPSec_Parameter[1]=$Tx_EXTMsgsPSec_Parameter[1] and $uTx_EXTMsgsPSec_Parameter[2]=$Tx_EXTMsgsPSec_Parameter[2] Then
		$uTxEXTMsgsPSec=1
	EndIf

	; Fetch Load row info
	$uLoad_Parameter=_GetNWStatsInfo(4)
	$uLoad_Channel1=StringStripWS($uLoad_Parameter[1],8)
	$uLoad_Channel2=StringStripWS($uLoad_Parameter[2],8)
	if $uLoad_Parameter[0]=$Load_Parameter[0] and $uLoad_Channel1=$Load_Channel1 and $uLoad_Channel2=$Load_Channel2 Then
		$uLoad=1
	EndIf

	; Fetch Peak Load row info
	$upLoad_Parameter=_GetNWStatsInfo(5)
	$upLoad_Channel1=StringStripWS($upLoad_Parameter[1],8)
	$upLoad_Channel2=StringStripWS($upLoad_Parameter[2],8)
	if $upLoad_Parameter[0]=$pLoad_Parameter[0] and $upLoad_Channel1=$pLoad_Channel1 and $upLoad_Channel2=$pLoad_Channel2 Then
		$upLoad=1
	EndIf

	; Fetch Average Load row info
	$uavLoad_Parameter=_GetNWStatsInfo(6)
	$uavLoad_Channel1=StringStripWS($uavLoad_Parameter[1],8)
	$uavLoad_Channel2=StringStripWS($uavLoad_Parameter[2],8)
	if $uavLoad_Parameter[0]=$avLoad_Parameter[0] and $uavLoad_Channel1=$avLoad_Channel1 and $uavLoad_Channel2=$avLoad_Channel2 Then
		$uavLoad=1
	EndIf

	_CANNWStatsMenu()															; Close Network Statistics window

EndIf

consolewrite("$TotalMsgs: "&$TotalMsgs&@CRLF)
consolewrite("$TotalErrors: "&$TotalErrors&@CRLF)
consolewrite("$Total_TxSTDMsgs: "&$Total_TxSTDMsgs&@CRLF)
consolewrite("$TxSTDMsgsPSec: "&$TxSTDMsgsPSec&@CRLF)
consolewrite("$Total_TxEXTMsgs: "&$Total_TxEXTMsgs&@CRLF)
consolewrite("$TxEXTMsgsPSec: "&$TxEXTMsgsPSec&@CRLF)
consolewrite("$Load: "&$Load&@CRLF)
consolewrite("$pLoad: "&$pLoad&@CRLF)
consolewrite("$avLoad: "&$avLoad&@CRLF)

consolewrite("$uTotalMsgs: "&$uTotalMsgs&@CRLF)
consolewrite("$uTotalErrors: "&$uTotalErrors&@CRLF)
consolewrite("$uTotal_TxSTDMsgs: "&$uTotal_TxSTDMsgs&@CRLF)
consolewrite("$uTxSTDMsgsPSec: "&$uTxSTDMsgsPSec&@CRLF)
consolewrite("$uTotal_TxEXTMsgs: "&$uTotal_TxEXTMsgs&@CRLF)
consolewrite("$uTxEXTMsgsPSec: "&$uTxEXTMsgsPSec&@CRLF)
consolewrite("$uLoad: "&$uLoad&@CRLF)
consolewrite("$upLoad: "&$upLoad&@CRLF)
consolewrite("$uavLoad: "&$uavLoad&@CRLF)

if $TotalMsgs=1 and $TotalErrors=1 and $Total_TxSTDMsgs=1 and  $TxSTDMsgsPSec=1 and $Total_TxEXTMsgs=1 and $TxEXTMsgsPSec=1 Then
	_WriteResult("Pass","TS_NwSt_05")
Else
	_WriteResult("Fail","TS_NwSt_05")
EndIf

if $Load=1 and $pLoad=1 and $avLoad=1 Then
	_WriteResult("Pass","TS_NwSt_06")
Else
	_WriteResult("Fail","TS_NwSt_06")
EndIf

if $uTotalMsgs=1 and $uTotalErrors=1 and $uTotal_TxSTDMsgs=1 and  $uTxSTDMsgsPSec=1 and $uTotal_TxEXTMsgs=1 and $uTxEXTMsgsPSec=1 and $uLoad=1 and $upLoad=1 and $uavLoad=1 Then
	_WriteResult("Pass","TS_NwSt_07")
Else
	_WriteResult("Fail","TS_NwSt_07")
EndIf

$isAppNotRes=_CloseApp()

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_NwSt_05")
	_WriteResult("Warning","TS_NwSt_06")
	_WriteResult("Warning","TS_NwSt_07")
EndIf



ConsoleWrite("****End : TS_NwSt_05/06/07.au3****"&@CRLF)
ConsoleWrite(@CRLF)
