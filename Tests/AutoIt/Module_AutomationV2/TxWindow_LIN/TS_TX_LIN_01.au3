;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_TX_LIN_01
; TestCases			:		Add Frames to Configure Transmission messages - Response
; Test Data			:
; Test Setup		:		1. Import LDF file
;							2. Select LIN->Transmit->Configure.
;							3. Select frames for transmission from dropdown Box provided in Tx frame List.(Add Message).
;							4. Connect

; Expected Results  : 		After step1, "Import LDF" window will open.
;							A dialog will open to select and associate a database.
;							And it should allow to select  LDF files of extension .ldf only.

;							After step2,a window should be displayed with the list of frames as given.
;							Option to select the messages (Add Messge) interested should be provided in Tx Frame List..The below list view will be displayed.
;							a.Tx Frame List
;							b.DataView List
;							c.Signal Details

;							After step3,The window should display the following information in the window for each frame:Message Name ,FrameId,Channel,Frame Type, Data Length.Repetition,key.
;							DtaByte for the selected frame will be displayed in DataByte list view.
;							The TX Frame list in the TX Window should be updated with the selected frames.
;							After step5, response should transmitted.
;==========================================================================Test Procedure =========================================================================
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_TX_LIN_01****"&@CRLF)

Local $LDFFilepath1=0,$TxFrameListColCount=0,$SigDtlsColCount=0,$dataByteCount=0,$LDFFilePathCC=""
Local $txtColEXPCTD[8]=["Message Name","Frame Id","Channel","Data Length","Message Type","RTR","Repetition (ms)","Key"]
Local $txtColSigDtlsEXPCTD[4]=["Signal Name","Raw Value","Physical Value","Unit"]
local $txtColdtabyteVwEXPCTD[9]=["index","00","01","02","03","04","05","06","07"]

_launchApp()

WinActivate($WIN_BUSMASTER)
Local $Time_cyclic=0,$a=0
if winexists($WIN_BUSMASTER) then

   _createConfig("cfxTXWinLIN")																		; Create New  Configuration

	_openLINClusterConfig()

	;1. After step1, "Cluster Configuration" window will open.

	if winexists($WIN_ClustrConfigLIN) then															; Check cluster config window is opened
			$clstrWinLinOpened = 1
			_importLDF("TestLIN2_1.ldf")															; Import LDF file in Cluster config
			sleep(1000)


			$LDFFilePathCC = ControlGetText($WIN_ClustrConfigLIN, "", $LDFFilePath)
			ConsoleWrite("$LDFFilePathCC="&$LDFFilePathCC&@CRLF)
			ConsoleWrite("----------=" &@CRLF)

			;$PathLDF=
			if $LDFFilePathCC = _TestDataPath()&"\TestLIN2_1.ldf" Then
				$LDFFilepath1 = 1																		; Check LDF File Path
			EndIf
			Sleep(1000)
			ControlClick($WIN_ClusterConfiguration,"",$BTN_OK_ClusterConfig)
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

			$txtColDbMsg=_GUICtrlListView_GetItemText($LVhWndTx, 0)
			ConsoleWrite("$txtColDbMsg="&$txtColDbMsg&@CRLF)

	EndIf

Endif

	ConsoleWrite("$LDFFilepath1= " & $LDFFilepath1 & @CRLF)
	ConsoleWrite("$TxFrameListColCount= " & $TxFrameListColCount & @CRLF)
	ConsoleWrite("$SigDtlsColCount= " & $SigDtlsColCount & @CRLF)
	ConsoleWrite("$dataByteCount= " & $dataByteCount & @CRLF)
	ConsoleWrite("$txtColDbMsg= " & $txtColDbMsg & @CRLF)



	If($LDFFilepath1=1 And $TxFrameListColCount=8 And $SigDtlsColCount=4 And $dataByteCount=9 And $txtColDbMsg="[Add Message]") Then
		_WriteResult("Pass","TS_TX_LIN_01")
	Else
		_WriteResult("Fail","TS_TX_LIN_01")
	EndIf


	$isAppNotRes=_CloseApp()														; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_TX_LIN_01")
EndIf

ConsoleWrite("****End : TS_TX_LIN_01.au3****"&@CRLF)
ConsoleWrite(@CRLF)