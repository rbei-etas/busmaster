#cs
;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_NS_LIN_36
; TestCases			:		Adding a message handler for the selected message Id and connect
; Test Data			:
; Test Setup		:		1. open the function editor for a node and right click message handler.Select "Add" option.
							2.Select Radio button for Message id .
							3.Give message id in input box.
							4.Click on "Apply".
							5.Select the node and Connect.
							"A defined message sent across the Linbus. Execute the message Handler"


; Expected Results  :		"The generic message handler message should be displayed for selected message ID transmitted over the Linbus."
;==========================================================================Test Procedure =========================================================================
#ce
;#include <IncludeFiles.au3>
ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_NS_LIN_36.au3****"&@CRLF)

_launchApp()
ProcessWait("BUSMASTER.exe")
;Global $RemoveDialogBox = 0,$childNode="",$CheckBoxNode1="",$CheckBoxNode2="",$CheckLoadednode=0
Global $infoTrace[4]=["","","",""]
Global $MsgFunCount=0,$traceInfoMsgHndler=0,$SecndMsg=0,$FirstMsg=0


WinActivate($WIN_BUSMASTER)

if winexists($WIN_BUSMASTER) then
	sleep(500)

	_loadConfig("cfxLINNS_36")															    ; Load the  Configuration
	_SetViewToolBarLIN()																	; Select LIN Tool Bar to view.
	_linDriverSelection()																; Select LIN driver selection from xml configuration File.
	sleep(1000)
	WinMenuSelectItem($WIN_BUSMASTER,"",$CANMenu,$CANMsgWinMenu,"&Activate")			; To deactivate CAn Message Window
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

		_AddNewNodeNS("TestNode36_2","LINCPP36_2",$TVC_Handlers_CPP)						;Create new Node with new CPP file

		$TVHWD=_GetWinHndlrNSTV()															; Fetch the handle of tree view
		ControlTreeView ( $WIN_LINNodesim, "", $TVHWD, "Select" ,"#0|#1")
		Send("{ENTER}")

		sleep(1000)
		_AddMsgHandler(1,8,"")																; Add msg handler with msg-id-8
		sleep(1000)

		$msgHndlrHWD=_GetCANHandlerHWD(2,2)													; Fetch the handle of the Msg handler item
		_GUICtrlTreeView_ClickItem($TVHWD, $msgHndlrHWD, "Right") 							; Right click on Busevent handler item in the tree view
		$MsgFunCount = _GUICtrlTreeView_GetChildCount($TVHWD, $msgHndlrHWD)

		if $MsgFunCount=1 Then
			ControlTreeView ( $WIN_LINNodesim, "", $TVHWD, "Select" ,"#0|#2|#"&0)
			sleep(1000)
			ControlSend($WIN_BUSMASTER, "", $TXT_EventHandler_CPPFile, ""   & @CRLF)							; Write in the editor of Node sim
			ControlSend($WIN_BUSMASTER, "", $TXT_EventHandler_CPPFile, "Trace(""Message Handler-MId-8 received"");"   & @CRLF)							; Write in the editor of Node sim
			ControlSend($WIN_BUSMASTER, "", $TXT_EventHandler_CPPFile, "STLIN_MSG sMsgM1;" & @CRLF)										; Write in the editor of Node sim
			ControlSend($WIN_BUSMASTER, "", $TXT_EventHandler_CPPFile, "sMsgM1.messagetype = 0;    // Slave Response" & @CRLF)				; Write in the editor of Node sim
			ControlSend($WIN_BUSMASTER, "", $TXT_EventHandler_CPPFile, "sMsgM1.checksumtype = 0; // Classic" & @CRLF)						; Write in the editor of Node sim
			ControlSend($WIN_BUSMASTER, "", $TXT_EventHandler_CPPFile, "sMsgM1.dlc = 8; // DLC"& @CRLF)							; Write in the editor of Node sim
			ControlSend($WIN_BUSMASTER, "", $TXT_EventHandler_CPPFile, "sMsgM1.id = 0x10;  // Message Identifier" & @CRLF)			; Write in the editor of Node sim
			ControlSend($WIN_BUSMASTER, "", $TXT_EventHandler_CPPFile, "sMsgM1.data[1] = 10;  // Lower 4 Bytes" & @CRLF)	; Write in the editor of Node sim
			ControlSend($WIN_BUSMASTER, "", $TXT_EventHandler_CPPFile, "sMsgM1.data[7] = 20;  // Upper 4 Bytes" & @CRLF)	; Write in the editor of Node sim
			ControlSend($WIN_BUSMASTER, "", $TXT_EventHandler_CPPFile, "sMsgM1.cluster = 1;" & @CRLF)									; Write in the editor of Node sim
			ControlSend($WIN_BUSMASTER, "", $TXT_EventHandler_CPPFile, "SendMsg(sMsgM1);" & @CRLF)											; Write in the editor of Node sim
			ControlSend($WIN_BUSMASTER, "", $TXT_EventHandler_CPPFile, "Trace(""Message Handler-MId-10 transmitted"");" & @CRLF)							; Write in the editor of Node sim
		endif


		_SaveAndCloseFunEditor()																	; save and Close Function editor

		WinActivate($WIN_BUSMASTER)
		_ConnectDisconnect_LIN()																	; Connect LIN
		Sleep(4000)
		_ConnectDisconnect_LIN()																	; Disconnect LIN

		$CountTraceInfo= _GetTraceWinItemCount()													; Count info in trace Window.
		ConsoleWrite("$CountTraceInfo="&$CountTraceInfo&@CRLF)

		if $CountTraceInfo >=2 Then
			For $i=0 to $CountTraceInfo-1
				$infoTrace[$i] = _GetTraceWinInfo($i)
				ConsoleWrite("$infoTrace="&$infoTrace[$i]&@CRLF)
			Next
		EndIf

		if $infoTrace[0]="Message Handler-MId-8 received" and $infoTrace[1] = "Message Handler-MId-10 transmitted"  Then
			$traceInfoMsgHndler = 1
		EndIf

		$rCount=_GetLINMsgWinItemCount()																				;count no. of items in msgwindow
		$Count1=_GetLINMsgWinColCount()																					;count no. of columns in msgwindow
		ConsoleWrite("$rCount="&$rCount&@CRLF)
		ConsoleWrite("$Count1="&$Count1&@CRLF)

		Local $Msg8 = ObjCreate("System.Collections.ArrayList") 														; create object
		Local $Msg10 = ObjCreate("System.Collections.ArrayList") 														; create object

		if $rCount <>0 Then


			$Msg8=_GetMsgWinInfo($LVC_CID_LINMsgWin,0,10)															; Fetch the first row data in the msg window
			$Msg10=_GetMsgWinInfo($LVC_CID_LINMsgWin,1,10)															; Fetch the second row data in the msg window
			sleep(2000)

			for $i=0 to $Count1-2
				$Value8 =$Msg8.Item($i)
				$Value10 =$Msg10.Item($i)
				ConsoleWrite("$Msg8[" & $i &"] : "& $Value8 &@CRLF)
				ConsoleWrite("$Msg10[" & $i &"] : "& $Value10 &@CRLF)
				;ConsoleWrite("$Msg_1[" & $i &"] : "& $msg1 &@CRLF)
			next

			if $Msg8.Item(0) <>" " and $Msg8.Item(1)="0x8" and $Msg8.Item(2)="LIN Message" and $Msg8.Item(3)="Tx" and $Msg8.Item(4)=1 and $Msg8.Item(5)=8 and $Msg8.Item(6)= 0x008 and $Msg8.Item(7)="00 0A 00 00 00 00 00 14" and $Msg8.Item(8)="0xD9 (""Enhanced"")" Then			; Compare the Direction, Channel and Msg ID of the first row
				$FirstMsg=1
				ConsoleWrite("$FirstMsg = "& $FirstMsg &@CRLF)
			Else
				$FirstMsg=0
				ConsoleWrite("$FirstMsg = "& $FirstMsg &@CRLF)
			EndIf
			if $Msg10.Item(0) <>"" and $Msg10.Item(1)="0x10" and $Msg10.Item(2)="LIN Message" and $Msg10.Item(3)="Tx" and $Msg10.Item(4)=1 and $Msg10.Item(5)=8 and $Msg10.Item(6)=0x010 and $Msg10.Item(7)="00 0A 00 00 00 00 00 14" and $Msg10.Item(8)="0x91 (""Enhanced"")" Then			; Compare the Direction, Channel and Msg ID of the first row
				$SecndMsg=1
				ConsoleWrite("$SecndMsg = "& $SecndMsg &@CRLF)
			Else
				$SecndMsg=0
				ConsoleWrite("$SecndMsg = "& $SecndMsg &@CRLF)
			EndIf
		EndIf
	EndIf


EndIf

	ConsoleWrite("$MsgFunCount="&$MsgFunCount&@CRLF)
	ConsoleWrite("$traceInfoMsgHndler="&$traceInfoMsgHndler&@CRLF)
	ConsoleWrite("$FirstMsg="&$FirstMsg&@CRLF)
	ConsoleWrite("$SecndMsg="&$SecndMsg&@CRLF)


	If $MsgFunCount = 1   and $traceInfoMsgHndler=1 and $FirstMsg=1 and $SecndMsg=1 Then
		_WriteResult("Pass","TS_NS_LIN_36")
		ConsoleWrite("PASS")
	Else
	   _WriteResult("Fail","TS_NS_LIN_36")
	   ConsoleWrite("FAIL")
   EndIf

$isAppNotRes=_CloseApp()																	    ; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_NS_LIN_36")
EndIf

ConsoleWrite("****End : TS_NS_LIN_31.au3****"&@CRLF)
ConsoleWrite(@CRLF)

