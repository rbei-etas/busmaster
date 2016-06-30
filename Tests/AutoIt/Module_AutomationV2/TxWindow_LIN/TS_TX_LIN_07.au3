;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_TX_LIN_06
; TestCases			:		Transmitting Master frame
; Test Data			:
; Test Setup		:		1.Add frames to TX list by Selecting frames from available LIN frames list.
;							2..Configure  Key Trigger for Transmission(out of A-Z,a-z,0-9) for both header and response frame by enabling checkBox provided.
;							3.Enter key trigger say 'a' for all the frames
;							4..Connect.
;							5.Press key a

; Expected Results  : 		1.After step2,Frames already selected and available in the TX Frame list should be displayed in the Tx window
;							2.The selected frames along with necessary information should be set to the controller for transmission.
;							After step5, frames should get transmitted once.
;							After  pressing the key configured ('a') configured frames should be transmitted again

;==========================================================================Test Procedure =========================================================================
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_TX_LIN_07****"&@CRLF)

Local $Timedelay=0,$VerifyMsg=1,$FirstMsg=0,$FirstMsgTime=0,$SecondMsgTime=0,$TimeDelay=0,$ErrMsg=0,$SecondMsg=0,$Tm2=0,$Tm1=0

_launchApp()

WinActivate($WIN_BUSMASTER)
Local $Time_cyclic=0,$a=0
if winexists($WIN_BUSMASTER) then


	_createConfig("TS_TX_LIN_07")
	_SetViewToolBarLIN()																	 		; Select LIN Tool Bar to view.
	sleep(1000)
	;_linDriverSelectionXml()																 		; Select LIN driver selection from xml configuration File.
	_linDriverSelection()
	;_linDriverSelectionXml()																		; Select LIN driver selection from xml configuration File.
	sleep(1000)
	sleep(1000)
	_openLINClusterConfig()
	WinWaitActive($WIN_ClustrConfigLIN)
	if winexists($WIN_ClustrConfigLIN) then
		sleep(1000)
		_importLDF("TestLIN2_1.ldf")
		sleep(1000)
		ControlCommand($WIN_ClustrConfigLIN, "",$chckBoxEnableMaster, "Check", "") 					; Select Check box for enable Master mode.
		Sleep(1000)
		ControlClick($WIN_ClusterConfiguration,"",$BTN_OK_ClusterConfig)
		sleep(1000)
		_TxMsgMenuLIN()
		_ConfigTXWinDetails(0,"db",1,1,"","Master","","","a")										; $Rownum,$HSCaseNo("db","ndb"),$messageid/Index(For dbMessage),chnl,datalength,Messageype,RTR(Yes/No),Repetition,Key
		_ConfigTXWinDetails(1,"db",1,1,"","Slave","","","")										; $Rownum,$HSCaseNo("db","ndb"),$messageid/Index(For dbMessage),chnl,datalength,Messageype,RTR(Yes/No),Repetit

		_CloseTxWindowArg("LIN")																	; Close Tx window
		_ConnectDisconnect_LIN()
		sleep(1000)
		Send("a")
		;sleep(3000)
		;Send("b")
		;sleep(3000)
		_ConnectDisconnect_LIN()
		;_CloseTxWindowArg("LIN")																	; Close Tx window
	EndIf

	;_DisableOverwriteMode()
	_EnableOverwriteMode()
	$rCount=_GetLINMsgWinItemCount()																;count no. of items in msgwindow
	$CountCol=_GetLINMsgWinColCount()																;count no. of columns in msgwindow
	ConsoleWrite("$rCount="&$rCount&@CRLF)
	ConsoleWrite("$Count1="&$CountCol&@CRLF)


	Local $Msg0 = ObjCreate("System.Collections.ArrayList") 										; create object
	Local $Msg1 = ObjCreate("System.Collections.ArrayList") 										; create object

	if $rCount = 1 Then
		$Msg0=_GetMsgWinInfo($LVC_CID_LINMsgWin,0,10)												; Fetch the first row data in the msg window
		;$Msg1=_GetMsgWinInfo($LVC_CID_LINMsgWin,1,10)
		sleep(2000)
		if $Msg0.Item(0) <>" " and $Msg0.Item(1)="Frame1" and $Msg0.Item(2)="LIN Message" and $Msg0.Item(3)="Tx" and $Msg0.Item(4)=1 and $Msg0.Item(5)=4 and $Msg0.Item(6)= 0x001 and $Msg0.Item(7)="F3 FF FF FF" and $Msg0.Item(8)="0x4A (""Enhanced"")" Then			; Compare the Direction, Channel and Msg ID of the first row
			$FirstMsg=1
		Endif

	EndIf


EndIf

	ConsoleWrite("$FirstMsg = "& $FirstMsg &@CRLF)
	;ConsoleWrite("$SecondMsg = "& $SecondMsg &@CRLF)


	If($FirstMsg=1 ) Then
		_WriteResult("Pass","TS_TX_LIN_07")
	Else
		_WriteResult("Fail","TS_TX_LIN_07")
	EndIf


	$isAppNotRes=_CloseApp()																		; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_TX_LIN_07")
EndIf

ConsoleWrite("****End : TS_TX_LIN_07.au3****"&@CRLF)
ConsoleWrite(@CRLF)