;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_TX_LIN_16
; TestCases			:		Transmitting Master frame
; Test Data			:
; Test Setup		:		1.Select LIN->Transmit->Configure.
;							2.Click on Add mesageDropdown in Tx Frame List.Select any message from dropdown.
;							3.Select frames from the list for both Master and slave.
;							4.Change the data Byte for slave and Transmit.

; Expected Results  : 		1. After step4, in Message Window,the messge will be transmited with the updated databyte.
;==========================================================================Test Procedure =========================================================================
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_TX_LIN_16****"&@CRLF)

Local $FirstMsg=0,$SecondMsg=0
Local $DtaByteValue[8]=["8","7","6","5","4","3","2","1"]

_launchApp()

WinActivate($WIN_BUSMASTER)
Local $Time_cyclic=0,$a=0
if winexists($WIN_BUSMASTER) then


	;_createConfig("TS_TX_LIN_09")
	_loadConfig("cfx_TS_TX_LIN_16")
	_SetViewToolBarLIN()																	 		; Select LIN Tool Bar to view.
	sleep(1000)
	;_linDriverSelectionXml()																 		; Select LIN driver selection from xml configuration File.
	_linDriverSelection()

	_DisableOverwriteMode()
	;_linDriverSelectionXml()																		; Select LIN driver selection from xml configuration File.
	sleep(1000)
	sleep(1000)
	_openLINClusterConfig()
	WinWaitActive($WIN_ClustrConfigLIN)
	if winexists($WIN_ClustrConfigLIN) then
		sleep(1000)
		ControlCommand($WIN_ClustrConfigLIN, "",$chckBoxEnableMaster, "Check", "") 					; Select Check box for enable Master mode.
		Sleep(1000)
		ControlClick($WIN_ClusterConfiguration,"",$BTN_OK_ClusterConfig)
		sleep(1000)
	EndIf
		_TxMsgMenuLIN()
		Sleep(1000)
		_SelectRowTxWindow(1)
		for $i=1 to 4
			_updateDataByte("",$i,$DtaByteValue[$i-1])
		Next
		_SelectRowTxWindow(3)
		for $i=1 to 8
			_updateDataByte("",$i,$DtaByteValue[$i-1])
		Next

		_CloseTxWindowArg("LIN")
		_ConnectDisconnect_LIN()																		; Connect
		sleep(1000)
		_ConnectDisconnect_LIN()																		; DisConnect

	$rCount=_GetLINMsgWinItemCount()																; count no. of items in msgwindow
	$CountCol=_GetLINMsgWinColCount()																; count no. of columns in msgwindow

	ConsoleWrite("$rCount = "&$rCount&@CRLF)
	Local $Msg0 = ObjCreate("System.Collections.ArrayList") 										; create object
	Local $Msg1 = ObjCreate("System.Collections.ArrayList") 										; create object


	if $rCount = 2 Then
		$Msg0=_GetMsgWinInfo($LVC_CID_LINMsgWin,0,10)												; Fetch the first row data in the msg window
		for $j=0 to 8
			ConsoleWrite("$Msg0="&$Msg0.Item($j)&@CRLF)
		Next
		$Msg1=_GetMsgWinInfo($LVC_CID_LINMsgWin,1,10)												; Fetch the first row data in the msg window
		sleep(2000)
		;if $Msg0.Item(0) <>" " and $Msg0.Item(1)="Frame1" and $Msg0.Item(2)="Error - Slave Not Responding" and $Msg0.Item(4)=1 And $Msg0.Item(6)= 0x001 Then			; Check the error message,Slave is not configured.
		if $Msg0.Item(0) <>" " And $Msg0.Item(1)="Frame1" And $Msg0.Item(7)="08 07 06 05" And $Msg0.Item(8)="0x24 (""Enhanced"")"  Then			; Check the error message,Slave is not configured.
			$FirstMsg=1
		Endif
		if $Msg1.Item(0) <>" " And $Msg1.Item(1)="0x12" And $Msg1.Item(7)="08 07 06 05 04 03 02 01" And $Msg1.Item(8)="0x49 (""Enhanced"")" Then			; Compare the Direction, Channel and Msg ID of the first row
			$SecondMsg=1
		Endif

	EndIf

EndIf

	ConsoleWrite("$FirstMsg = "&$FirstMsg&@CRLF)
	ConsoleWrite("$SecondMsg = "&$SecondMsg&@CRLF)


	If($FirstMsg=1 And $SecondMsg=1) Then
		_WriteResult("Pass","TS_TX_LIN_16")
	Else
		_WriteResult("Fail","TS_TX_LIN_16")
	EndIf


	$isAppNotRes=_CloseApp()																		; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_TX_LIN_16")
EndIf

ConsoleWrite("****End : TS_TX_LIN_16.au3****"&@CRLF)
ConsoleWrite(@CRLF)