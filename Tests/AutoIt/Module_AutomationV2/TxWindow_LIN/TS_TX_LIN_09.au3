;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_TX_LIN_09
; TestCases			:		Transmitting Master frame
; Test Data			:
; Test Setup		:		1.Add frames to TX list by Selecting frames from available LIN frames list.

;							2.Configure  Key Trigger for Transmission(out of A-Z,a-z,0-9) by checking checkBox provided.
;							3.Configure Repetition time (say 2000 ms) for a frame by enabling checkBox.
;							   Enter key trigger say 'b' for the first frame
;							4.Connect.
;							5.Press key b

; Expected Results  : 		After step1,Frames already selected and available in the TX Frame list should be added to TX window.User can select messages for transmission with the help of checkBoxex
;							After step5,frames configured , should get transmitted according to Repetition cycle value specified.(Frames should get transmitted for a interval of 2000ms as specified).
;							Once user presses b, frame configured for that particular key should get transmitted.
;==========================================================================Test Procedure =========================================================================
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_TX_LIN_09****"&@CRLF)

Local $Timedelay=0,$VerifyMsg=1,$FirstMsg=0,$FirstMsgTime=0,$SecondMsgTime=0,$TimeDelay=0,$ErrMsg=0,$SecondMsg=0,$Tm2=0,$Tm1=0
Local $ThirdMsg=0,$ThirdMsgTime=0,$Tm3=0,$TimeDelayOfKeyMsg=0

_launchApp()

WinActivate($WIN_BUSMASTER)
Local $Time_cyclic=0,$a=0
if winexists($WIN_BUSMASTER) then



	_loadConfig("cfx_TS_TX_LIN_09")

	sleep(1000)

	_linDriverSelection()

	_DisableOverwriteModeAll($linMenu)

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
		_TxMsgMenuLIN()
		_ConfigTXWinDetails(0,"db",1,1,"","Master","","2000","a")									; $Rownum,$HSCaseNo("db","ndb"),$messageid/Index(For dbMessage),chnl,datalength,Messageype(For Lin master/Slave),RTR(Yes/No),Repetition,Key
		_ConfigTXWinDetails(1,"db",1,1,"","Slave","","2000","a")									; $Rownum,$HSCaseNo("db","ndb"),$messageid/Index(For dbMessage),chnl,datalength,Messageype(For Lin master/Slave),RTR(Yes/No),Repetition,Key

		_CloseTxWindowArg("LIN")																	; Close Tx window
		_ConnectDisconnect_LIN()
		Sleep(1000)
		;$rCount=_GetLINMsgWinItemCount()

		;sleep(3000)
		;WinActivate($WIN_BUSMASTER,"")
		;$rCount=_GetLINMsgWinItemCount()
		Send("a")
		;sleep(100)
		_ConnectDisconnect_LIN()
		;_CloseTxWindowArg("LIN")																	; Close Tx window
	EndIf


	;_EnableOverwriteMode()
	$rCount=_GetLINMsgWinItemCount()																;count no. of items in msgwindow
	$CountCol=_GetLINMsgWinColCount()																;count no. of columns in msgwindow
	ConsoleWrite("$rCount="&$rCount&@CRLF)
	ConsoleWrite("$Count1="&$CountCol&@CRLF)


	Local $Msg0 = ObjCreate("System.Collections.ArrayList") 										; create object
	Local $Msg1 = ObjCreate("System.Collections.ArrayList") 										; create object

	if $rCount = 3 Then
		$Msg0=_GetMsgWinInfo($LVC_CID_LINMsgWin,0,10)												; Fetch the first row data in the msg window
		$Msg1=_GetMsgWinInfo($LVC_CID_LINMsgWin,1,10)
		$Msg2=_GetMsgWinInfo($LVC_CID_LINMsgWin,2,10)
		sleep(2000)
		if $Msg0.Item(0) <>" " and $Msg0.Item(1)="Frame1" and $Msg0.Item(2)="LIN Message" and $Msg0.Item(3)="Tx" and $Msg0.Item(4)=1 and $Msg0.Item(5)=4 and $Msg0.Item(6)= 0x001 and $Msg0.Item(7)="F3 FF FF FF" and $Msg0.Item(8)="0x4A (""Enhanced"")" Then			; Compare the Direction, Channel and Msg ID of the first row
			$FirstMsg=1
			$FirstMsgTime=StringSplit($Msg0.Item(0),":")
			$Tm1=$FirstMsgTime[3]
			ConsoleWrite("$Tm1 - "&$Tm1&@CRLF)
		Endif
		if $Msg1.Item(0) <>" " and $Msg1.Item(1)="Frame1" and $Msg1.Item(2)="LIN Message" and $Msg1.Item(3)="Tx" and $Msg1.Item(4)=1 and $Msg1.Item(5)=4 and $Msg1.Item(6)= 0x001 and $Msg1.Item(7)="F3 FF FF FF" and $Msg1.Item(8)="0x4A (""Enhanced"")" Then			; Compare the Direction, Channel and Msg ID of the first row
			$SecondMsg=1
			$SecondMsgTime=StringSplit($Msg1.Item(0),":")
			$Tm2=$SecondMsgTime[3]
			ConsoleWrite("$Tm2 - "&$Tm2&@CRLF)
		Endif
		if $Msg2.Item(0) <>" " and $Msg2.Item(1)="Frame1" and $Msg2.Item(2)="LIN Message" and $Msg2.Item(3)="Tx" and $Msg2.Item(4)=1 and $Msg2.Item(5)=4 and $Msg2.Item(6)= 0x001 and $Msg2.Item(7)="F3 FF FF FF" and $Msg2.Item(8)="0x4A (""Enhanced"")" Then			; Compare the Direction, Channel and Msg ID of the first row
			$ThirdMsg=1
			$ThirdMsgTime=StringSplit($Msg2.Item(0),":")
			$Tm3=$ThirdMsgTime[3]
			ConsoleWrite("$Tm3 - "&$Tm3&@CRLF)
		Endif

	EndIf

	;if ($Tm2-$Tm1)=2 Then
	;	$TimeDelay=1
	;EndIf
	if ($Tm3-$Tm1)=2 Then
		$TimeDelayOfKeyMsg=1
	EndIf

EndIf

	ConsoleWrite("$FirstMsg = "& $FirstMsg &@CRLF)
	ConsoleWrite("$SecondMsg = "& $SecondMsg &@CRLF)
	ConsoleWrite("$TimeDelay = "& $TimeDelay &@CRLF)
	ConsoleWrite("$ThirdMsg = "& $ThirdMsg &@CRLF)
	ConsoleWrite("$TimeDelayOfKeyMsg = "& $TimeDelayOfKeyMsg &@CRLF)


	;If($FirstMsg=1 And $SecondMsg=1 And $TimeDelay=1 And $ThirdMsg=1 And $TimeDelayOfKeyMsg=1) Then
	If($FirstMsg=1 And $SecondMsg=1 And  $ThirdMsg=1 And $TimeDelayOfKeyMsg=1) Then
		_WriteResult("Pass","TS_TX_LIN_09")
	Else
		_WriteResult("Fail","TS_TX_LIN_09")
	EndIf


	$isAppNotRes=_CloseApp()																		; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_TX_LIN_09")
EndIf

ConsoleWrite("****End : TS_TX_LIN_09.au3****"&@CRLF)
ConsoleWrite(@CRLF)