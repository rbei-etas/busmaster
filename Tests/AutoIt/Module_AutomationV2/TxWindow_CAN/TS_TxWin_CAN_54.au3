;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_TxWin_CAN_54
; TestCases			:		Checking the Deleting of single message from Tx Message List during transmission.
; Test Data			:
; Test Setup		:		1. The tx Windows view( Tx message list) will contain the following  columns.
							;~ Tx rame List-
							;~ A. messaeg name
							;~ B. Frame Id
							;~ C.Chanel
							;~ D. DataLength
							;~ E.Message type
							;~ F. RTR
							;~ G.Repetition
							;~ Dta byte View-
							;~ A Index Name and columns from 00 to 07.
							;~ Signal Details View-
							;~ Signal name,Raw Value,Physical Value and Unit

; 							Expected Results  :After step 1 Tx Window should contain all the specified column in List.

;==========================================================================Test Procedure =========================================================================
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_TxWin_CAN_54****"&@CRLF)


Local $txtColEXPCTD[8]=["Message Name","Frame Id","Channel","Data Length","Message Type","RTR","Repetition (ms)","Key"]
Local $txtColSigDtlsEXPCTD[4]=["Signal Name","Raw Value","Physical Value","Unit"]
local $txtColdtabyteVwEXPCTD[9]=["index","00","01","02","03","04","05","06","07"]
Local $TxFrameListColCount=0,$SigDtlsColCount=0,$dataByteCount=0

_launchApp()

WinActivate($WIN_BUSMASTER)
Local $Time_cyclic=0,$a=0
if winexists($WIN_BUSMASTER) then
    _loadConfig("TS_TxWin_CAN_19")
	sleep(500)

	sleep(1000)
	_TxMsgMenu()																								; Select CAN->Transmit->Configure menu
	sleep(1000)
	$LVhWndTx=_GetTxWindHWD()																					; Get Handle of Tx Window

	for $i=0 to 7
		$txtCol=_GUICtrlListView_GetColumn($LVhWndTx, $i)
		ConsoleWrite("$txtCol="&$txtCol[5]&@CRLF)																; $txtCol[5] - Column header text
		if $txtCol[5]=$txtColEXPCTD[$i] Then
			$TxFrameListColCount=$TxFrameListColCount+1
		EndIf
	Next

	$HWD1=ControlGetHandle($WIN_BUSMASTER,"",$LVC_TxWin_dtaView)													; Fetch the handle of signal details
	for $i=0 to 8
		$txtColdtabyteVwActl=_GUICtrlListView_GetColumn($HWD1, $i)
		ConsoleWrite("$txtColDataByteViewDtls="&$txtColdtabyteVwActl[5]&@CRLF)										; $txtCol[5] - Column header text for data Byte View List
		if $txtColdtabyteVwActl[5]=$txtColdtabyteVwEXPCTD[$i] Then
			$dataByteCount=$dataByteCount+1
		EndIf
	Next


	$HWD=ControlGetHandle($WIN_BUSMASTER,"",$LVC_TxWin_SigDtls)													; Fetch the handle of signal details
	for $i=0 to 3
		$txtColSigDtlsActl=_GUICtrlListView_GetColumn($HWD, $i)
		ConsoleWrite("$txtColSigDtls="&$txtColSigDtlsActl[5]&@CRLF)												; $txtCol[5] - Column header text for Signal Col List
		if $txtColSigDtlsActl[5]=$txtColSigDtlsEXPCTD[$i] Then
			$SigDtlsColCount=$SigDtlsColCount+1
		EndIf
	Next



EndIf
	ConsoleWrite("$TxFrameListColCount="&$TxFrameListColCount&@CRLF)
	ConsoleWrite("$SigDtlsColCount="&$SigDtlsColCount&@CRLF)
	ConsoleWrite("$dataByteCount="&$dataByteCount&@CRLF)


	If ($TxFrameListColCount=8 And $SigDtlsColCount=4 And $dataByteCount=9) Then
		_WriteResult("Pass","TS_TxWin_CAN_54")
	Else
		_WriteResult("Fail","TS_TxWin_CAN_54")
	EndIf


	$isAppNotRes=_CloseApp()														; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_TxWin_CAN_54")
EndIf

ConsoleWrite("****End : TS_TxWin_CAN_54.au3****"&@CRLF)
ConsoleWrite(@CRLF)