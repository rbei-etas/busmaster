#cs
;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_NS_LIN_46
; TestCases			:		Addding message handler function selecting "All Rx messge"
; Test Data			:
; Test Setup		:		1. open the function editor for a node and right click message handler.Select "Add" option.
							2.Select Radio button for "All Rx Messages".
							3.Click on "Apply".
							4."A defined message sent across the Linbus. Execute the message Handler"


; Expected Results  :		"The generic message handler message should be displayed for all the messages transmitted over the Linbus."
;==========================================================================Test Procedure =========================================================================
#ce
;#include <IncludeFiles.au3>
ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_NS_LIN_46.au3****"&@CRLF)

_launchApp()
ProcessWait("BUSMASTER.exe")
;Global $RemoveDialogBox = 0,$childNode="",$CheckBoxNode1="",$CheckBoxNode2="",$CheckLoadednode=0
Global $infoTrace[4]=["","","",""]
Global $MsgFunCount=0,$traceInfoMsgHndler=0,$SecndMsg=0,$FirstMsg=0,$Thirdmsg=0,$FourthMsg=0,$FifthMsg=0,$SixthMsg=0,$rCount=0


WinActivate($WIN_BUSMASTER)

if winexists($WIN_BUSMASTER) then
	sleep(500)

	_loadConfig("cfxLINNS_46")															    ; Load the  Configuration
	_SetViewToolBarLIN()																	; Select LIN Tool Bar to view.
	_linDriverSelection()																; Select LIN driver selection from xml configuration File.
	sleep(1000)

	_openLINClusterConfig()
	if winexists($WIN_ClustrConfigLIN) then
		ControlCommand($WIN_ClustrConfigLIN, "",$chckBoxEnableMaster, "Check", "") 					; Select Check box for enable Master mode.
		Sleep(1000)
		ControlClick($WIN_ClusterConfiguration,"",$BTN_OK_ClusterConfig)
	EndIf

	_LINNodeSimConfigMenu()																	; Open Node Sim from menu
	sleep(500)

	Opt("WinDetectHiddenText", 0)
	Opt("WinSearchChildren", 1)
	Opt("WinTitleMatchMode", 1)
	WinWait("Configure Simulated Systems - LIN Bus")

	if winexists("Configure Simulated Systems - LIN Bus") then

		_AddNewNodeNS("TestNode46_2","LINCPP46_2",$TVC_Handlers_CPP)						;Create new Node with new CPP file

		$TVHWD=_GetWinHndlrNSTV()															; Fetch the handle of tree view
		ControlTreeView ( $WIN_LINNodesim, "", $TVHWD, "Select" ,"#0|#1")
		Send("{ENTER}")

		sleep(1000)
		_AddMsgHandler(3,"","")																; Add msg handler with msg-id-8
		sleep(1000)

		$msgHndlrHWD=_GetCANHandlerHWD(2,2)													; Fetch the handle of the Msg handler item
		_GUICtrlTreeView_ClickItem($TVHWD, $msgHndlrHWD, "Left") 							;  click on Msg handler item in the tree view
		$MsgFunCount = _GUICtrlTreeView_GetChildCount($TVHWD, $msgHndlrHWD)

		if $MsgFunCount=1 Then
			ControlTreeView ( $WIN_LINNodesim, "", $TVHWD, "Select" ,"#0|#2|#"&0)
			sleep(1000)
			ControlSend($WIN_BUSMASTER, "", $TXT_EventHandler_CPPFile, ""   & @CRLF)														; Write in the editor of Node sim
			ControlSend($WIN_BUSMASTER, "", $TXT_EventHandler_CPPFile, "Trace(""All Rx Message"");"   & @CRLF)								; Write in the editor of Node sim
			ControlSend($WIN_BUSMASTER, "", $TXT_EventHandler_CPPFile, "STLIN_MSG sMsgM1;" & @CRLF)										; Write in the editor of Node sim
			ControlSend($WIN_BUSMASTER, "", $TXT_EventHandler_CPPFile, "sMsgM1.messagetype = 0;    // Slave Response" & @CRLF)				; Write in the editor of Node sim
			ControlSend($WIN_BUSMASTER, "", $TXT_EventHandler_CPPFile, "sMsgM1.checksumtype = 0; // Classic" & @CRLF)						; Write in the editor of Node sim
			ControlSend($WIN_BUSMASTER, "", $TXT_EventHandler_CPPFile, "sMsgM1.dlc = 10; // DLC"& @CRLF)							; Write in the editor of Node sim
			ControlSend($WIN_BUSMASTER, "", $TXT_EventHandler_CPPFile, "sMsgM1.id = 0x12;  // Message Identifier" & @CRLF)			; Write in the editor of Node sim
			ControlSend($WIN_BUSMASTER, "", $TXT_EventHandler_CPPFile, "sMsgM1.data[1] = 10;  // Lower 4 Bytes" & @CRLF)	; Write in the editor of Node sim
			ControlSend($WIN_BUSMASTER, "", $TXT_EventHandler_CPPFile, "sMsgM1.data[7] = 20;  // Upper 4 Bytes" & @CRLF)	; Write in the editor of Node sim
			ControlSend($WIN_BUSMASTER, "", $TXT_EventHandler_CPPFile, "sMsgM1.cluster = 1;" & @CRLF)									; Write in the editor of Node sim
			ControlSend($WIN_BUSMASTER, "", $TXT_EventHandler_CPPFile, "SendMsg(sMsgM1);" & @CRLF)											; Write in the editor of Node sim
			;ControlSend($WIN_LINNodesim, "", $TXT_EventHandler_CPPFile, "Trace(""MsgId-8 Transmitted"");" & @CRLF)							; Write in the editor of Node sim
		endif


		_SaveAndCloseFunEditor()																	; save and Close Function editor

		WinActivate($WIN_BUSMASTER)
		_ConnectDisconnect_LIN()																	; Connect LIN
		Sleep(4000)


		;$CountMsgWindowLIN=_GetLINMsgWinItemCount()																					;count no. of columns in msgwindow

		;_TxMsgMenuLIN()
		;Send("a")
		;sleep(1000)
		;Send("b")
		;sleep(1000)
		;Send("c")
		;sleep(1000)
		_ConnectDisconnect_LIN()																	; Disconnect LIN
		sleep(1000)


		$CountTraceInfo= _GetTraceWinItemCount()													; Count info in trace Window.
		ConsoleWrite("$CountTraceInfo="&$CountTraceInfo&@CRLF)

		if $CountTraceInfo >4 Then
			For $i=0 to 3
				$infoTrace[$i] = _GetTraceWinInfo($i)
				ConsoleWrite("$infoTrace="&$infoTrace[$i]&@CRLF)
			Next
		EndIf

		if $infoTrace[0]="All Rx Message" and $infoTrace[1] = "All Rx Message" and $infoTrace[2]="All Rx Message" and $infoTrace[3]="All Rx Message"  Then
			$traceInfoMsgHndler = 1
		EndIf

		$rCount=_GetLINMsgWinItemCount()																				;count no. of items in msgwindow
		$Count1=_GetLINMsgWinColCount()																					;count no. of columns in msgwindow
		ConsoleWrite("$rCount="&$rCount&@CRLF)
		ConsoleWrite("$Count1="&$Count1&@CRLF)

		Local $Msg0 = ObjCreate("System.Collections.ArrayList") 														; create object
		Local $Msg1 = ObjCreate("System.Collections.ArrayList") 														; create object
		Local $Msg2 = ObjCreate("System.Collections.ArrayList") 														; create object
		Local $Msg3 = ObjCreate("System.Collections.ArrayList") 														; create object
		Local $Msg4 = ObjCreate("System.Collections.ArrayList") 														; create object
		Local $Msg5 = ObjCreate("System.Collections.ArrayList") 														; create object
		Local $Msg = ObjCreate("System.Collections.ArrayList") 														; create object


		if $rCount = 5 Then


			$Msg0=_GetMsgWinInfo($LVC_CID_LINMsgWin,0,10)															; Fetch the first row data in the msg window
			$Msg1=_GetMsgWinInfo($LVC_CID_LINMsgWin,1,10)															; Fetch the second row data in the msg window
			$Msg2=_GetMsgWinInfo($LVC_CID_LINMsgWin,2,10)															; Fetch the second row data in the msg window
			$Msg3=_GetMsgWinInfo($LVC_CID_LINMsgWin,3,10)															; Fetch the second row data in the msg window
			$Msg4=_GetMsgWinInfo($LVC_CID_LINMsgWin,4,10)															; Fetch the second row data in the msg window

			sleep(2000)

			for $i=0 to $rCount-1
				$Msg=_GetMsgWinInfo($LVC_CID_LINMsgWin,$i,10)
				for $j=0 to $Count1-2
					$Value =$Msg.Item($j)
					ConsoleWrite("$Msg[" & $i &"] : "& $Value &@CRLF)
				Next
			Next

			if $Msg0.Item(0) <>" " and $Msg0.Item(1)="0x3" and $Msg0.Item(2)="LIN Message" and $Msg0.Item(3)="Tx" and $Msg0.Item(4)=1 and $Msg0.Item(5)=8 and $Msg0.Item(6)= 0x003 and $Msg0.Item(7)="00 0A 00 00 00 00 00 14" and $Msg0.Item(8)="0xDE (""Enhanced"")" Then			; Compare the Direction, Channel and Msg ID of the first row
				$FirstMsg=1
				ConsoleWrite("$FirstMsg = "& $FirstMsg &@CRLF)
			Endif

			if $Msg1.Item(0) <>"" and $Msg1.Item(1)="0x4" and $Msg1.Item(2)="LIN Message" and $Msg1.Item(3)="Tx" and $Msg1.Item(4)=1 and $Msg1.Item(5)=8 and $Msg1.Item(6)=0x004 and $Msg1.Item(7)="00 0A 00 00 00 00 00 14" and $Msg1.Item(8)="0x1D (""Enhanced"")" Then			; Compare the Direction, Channel and Msg ID of the first row
				$SecndMsg=1
				ConsoleWrite("$SecndMsg = "& $Thirdmsg &@CRLF)
			EndIf

			if $Msg2.Item(0) <>"" and $Msg2.Item(1)="0x8" and $Msg2.Item(2)="LIN Message" and $Msg2.Item(3)="Tx" and $Msg2.Item(4)=1 and $Msg2.Item(5)=8 and $Msg2.Item(6)=0x008 and $Msg2.Item(7)="00 0A 00 00 00 00 00 14" and $Msg2.Item(8)="0xD9 (""Enhanced"")" Then			; Compare the Direction, Channel and Msg ID of the first row
				$Thirdmsg=1
				ConsoleWrite("$Thirdmsg = "& $FifthMsg &@CRLF)
			EndIf
			if $Msg3.Item(0) <>"" and $Msg3.Item(1)="0x10" and $Msg3.Item(2)="LIN Message" and $Msg3.Item(3)="Tx" and $Msg3.Item(4)=1 and $Msg3.Item(5)=8 and $Msg3.Item(6)=0x010 and $Msg3.Item(7)="00 0A 00 00 00 00 00 14" and $Msg3.Item(8)="0x91 (""Enhanced"")" Then			; Compare the Direction, Channel and Msg ID of the first row
				$FourthMsg=1
				ConsoleWrite("$FourthMsg = "& $SixthMsg &@CRLF)
			EndIf
			if $Msg4.Item(0) <>"" and $Msg4.Item(1)="0x12" and $Msg4.Item(2)="LIN Message" and $Msg4.Item(3)="Tx" and $Msg4.Item(4)=1 and $Msg4.Item(5)=8 and $Msg4.Item(6)=0x012 and $Msg4.Item(7)="00 0A 00 00 00 00 00 14" and $Msg4.Item(8)="0x4F (""Enhanced"")" Then			; Compare the Direction, Channel and Msg ID of the first row
				$FifthMsg=1
				ConsoleWrite("$FifthMsg = "& $FourthMsg &@CRLF)
			EndIf

		EndIf
	EndIf


EndIf

	ConsoleWrite("$MsgFunCount="&$MsgFunCount&@CRLF)
	ConsoleWrite("$traceInfoMsgHndler="&$traceInfoMsgHndler&@CRLF)
	ConsoleWrite("$rCount="&$rCount&@CRLF)
	ConsoleWrite("$FirstMsg="&$FirstMsg&@CRLF)
	ConsoleWrite("$SecndMsg="&$SecndMsg&@CRLF)
	ConsoleWrite("$Thirdmsg="&$Thirdmsg&@CRLF)
	ConsoleWrite("$FourthMsg="&$FourthMsg&@CRLF)
	ConsoleWrite("$FifthMsg="&$FifthMsg&@CRLF)
	;ConsoleWrite("$SixthMsg="&$SixthMsg&@CRLF)



	If $MsgFunCount = 1   and $traceInfoMsgHndler=1 and $rCount=5 and $FirstMsg=1 and $SecndMsg=1 and $Thirdmsg=1 and $FourthMsg=1 and $FifthMsg=1  Then
		_WriteResult("Pass","TS_NS_LIN_46")
		ConsoleWrite("PASS")
	Else
	   _WriteResult("Fail","TS_NS_LIN_46")
	   ConsoleWrite("FAIL")
   EndIf

$isAppNotRes=_CloseApp()																	    ; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_NS_LIN_46")
EndIf

ConsoleWrite("****End : TS_NS_LIN_46.au3****"&@CRLF)
ConsoleWrite(@CRLF)

