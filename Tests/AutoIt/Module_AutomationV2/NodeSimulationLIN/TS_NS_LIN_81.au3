#cs
;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_NS_LIN_81
; TestCases			:		Multiple KeyHandler-Event for key handler for an alphabet and  "*" key
; Test Data			:		Define an event handler for an alphabet and * keys
; Test Setup		:		"Edit the event handler OnKey_a(unsigned char KeyValue) and OnKey_All(unsigned char KeyValue). Execute the key handler after building the node.

; Expected Results  :		"Only OnKey_a(unsigned char KeyValue) handler shoule get executed as individual key handler will take priority over the * key."
;==========================================================================Test Procedure =========================================================================
#ce
;#include <IncludeFiles.au3>
ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_NS_LIN_81.au3****"&@CRLF)

_launchApp()
ProcessWait("BUSMASTER.exe")

Global $infoTrace[10]=["","","","","","","","","",""]
Global $KeyFunCount=0,$traceInfoMsgHndler=0,$FirstMsg=0



WinActivate($WIN_BUSMASTER)

if winexists($WIN_BUSMASTER) then
	sleep(500)

	_loadConfig("cfxLINNS_63")															    ; Load the  Configuration
	_SetViewToolBarLIN()																	; Select LIN Tool Bar to view.
	_linDriverSelection()																; Select LIN driver selection from xml configuration File.
	sleep(1000)
	_openLINClusterConfig()
	if winexists($WIN_ClustrConfigLIN) then
		ControlCommand($WIN_ClustrConfigLIN, "",$chckBoxEnableMaster, "Check", "") 					; Select Check box for enable Master mode.
		Sleep(1000)
		ControlClick($WIN_ClusterConfiguration,"",$BTN_OK_ClusterConfig)
	EndIf
	sleep(1000)

	_LINNodeSimConfigMenu()																	; Open Node Sim from menu
	sleep(500)

	Opt("WinDetectHiddenText", 0)
	Opt("WinSearchChildren", 1)
	Opt("WinTitleMatchMode", 1)
	WinWait("Configure Simulated Systems - LIN Bus")

	if winexists("Configure Simulated Systems - LIN Bus") then

		_AddNewNodeNS("TestNode81","LINCPP81",$TVC_Handlers_CPP)							;Create new Node with new CPP file

		$TVHWD=_GetWinHndlrNSTV()															; Fetch the handle of tree view
		ControlTreeView ( $WIN_LINNodesim, "", $TVHWD, "Select" ,"#0|#1")
		Send("{ENTER}")

		sleep(1000)
		;_AddMsgHandler(3,"","")															; Add key handler for "a"
		;_AddTimeHandler("Timer1","1000")
		_AddKeyHandler("a")
		_AddKeyHandler("*")

		sleep(1000)

		$KeyHndlrHWD=_GetCANHandlerHWD(2,4)													; Fetch the handle of the key handler item
		_GUICtrlTreeView_ClickItem($TVHWD, $KeyHndlrHWD, "Left") 							;  click on Time handler item in the tree view
		$KeyFunCount = _GUICtrlTreeView_GetChildCount($TVHWD, $KeyHndlrHWD)
		ConsoleWrite("$TimeFunCount="&$KeyFunCount)
		if $KeyFunCount=2 Then
			for $i=0 to $KeyFunCount-1
				ControlTreeView ( $WIN_LINNodesim, "", $TVHWD, "Select" ,"#0|#4|#"&$i)
				sleep(1000)
				ControlSend($WIN_BUSMASTER, "", $TXT_TimeHandler_CPPFile, ""   & @CRLF)														; Write in the editor of Node sim
				ControlSend($WIN_BUSMASTER, "", $TXT_TimeHandler_CPPFile, "Trace(""Key Pressed_"&$i+1&"=%c"""&",KeyValue);"   & @CRLF)						; Write in the editor of Node sim
				sleep(1000)
				ControlSend($WIN_BUSMASTER, "", $TXT_TimeHandler_CPPFile, "STLIN_MSG sMsgM1;" & @CRLF)										; Write in the editor of Node sim
				sleep(1000)
				ControlSend($WIN_BUSMASTER, "", $TXT_TimeHandler_CPPFile, "sMsgM1.messagetype = 0;    " & @CRLF)			; Write in the editor of Node sim
				sleep(1000)
				ControlSend($WIN_BUSMASTER, "", $TXT_TimeHandler_CPPFile, "sMsgM1.checksumtype = 0; " & @CRLF)					; Write in the editor of Node sim
				sleep(1000)
				ControlSend($WIN_BUSMASTER, "", $TXT_TimeHandler_CPPFile, "sMsgM1.dlc = 8; "& @CRLF)							; Write in the editor of Node sim
				sleep(1000)
				ControlSend($WIN_BUSMASTER, "", $TXT_TimeHandler_CPPFile, "sMsgM1.id = "&$i+1&";  " & @CRLF)	; Write in the editor of Node sim
				sleep(1000)
				ControlSend($WIN_BUSMASTER, "", $TXT_TimeHandler_CPPFile, "sMsgM1.data[1] = 10;  " & @CRLF)	; Write in the editor of Node sim
				sleep(1000)
				ControlSend($WIN_BUSMASTER, "", $TXT_TimeHandler_CPPFile, "sMsgM1.data[7] = 20; " & @CRLF)	; Write in the editor of Node sim
				sleep(1000)
				ControlSend($WIN_BUSMASTER, "", $TXT_TimeHandler_CPPFile, "sMsgM1.cluster = 1;" & @CRLF)									; Write in the editor of Node sim
				sleep(1000)
				ControlSend($WIN_BUSMASTER, "", $TXT_TimeHandler_CPPFile, "SendMsg(sMsgM1);" & @CRLF)											; Write in the editor of Node sim
				sleep(1000)
				;ControlSend($WIN_LINNodesim, "", $TXT_EventHandler_CPPFile, "Trace(""MsgId-8 Transmitted"");" & @CRLF)							; Write in the editor of Node sim
			Next
		endif


		_SaveAndCloseFunEditor()																	; save and Close Function editor

		WinActivate($WIN_BUSMASTER)
		_ConnectDisconnect_LIN()																	; Connect LIN
		Send("a")
		Sleep(2000)



		_ConnectDisconnect_LIN()																	; Disconnect LIN
		sleep(2000)


		$CountTraceInfo= _GetTraceWinItemCount()													; Count info in trace Window.
		ConsoleWrite("$CountTraceInfo="&$CountTraceInfo&@CRLF)

		if $CountTraceInfo =1 Then
			For $i=0 to $CountTraceInfo-1
				$infoTrace[$i] = _GetTraceWinInfo($i)
				ConsoleWrite("$infoTrace="&$infoTrace[$i]&@CRLF)
			Next
		EndIf

		if $infoTrace[0]="Key Pressed_1=a" Then
			$traceInfoMsgHndler = 1
		EndIf

		$rCount=_GetLINMsgWinItemCount()																				;count no. of items in msgwindow
		$CountCol=_GetLINMsgWinColCount()																					;count no. of columns in msgwindow
		ConsoleWrite("$rCount="&$rCount&@CRLF)
		ConsoleWrite("$Count1="&$CountCol&@CRLF)

		Local $Msg0 = ObjCreate("System.Collections.ArrayList") 														; create object
		Local $Msg1 = ObjCreate("System.Collections.ArrayList") 														; create object



		if $rCount = 1 Then


			$Msg0=_GetMsgWinInfo($LVC_CID_LINMsgWin,0,10)															; Fetch the first row data in the msg window

			if $Msg0.Item(0) <>" " and $Msg0.Item(1)="0x1" and $Msg0.Item(2)="LIN Message" and $Msg0.Item(3)="Tx" and $Msg0.Item(4)=1 and $Msg0.Item(5)=8 and $Msg0.Item(6)= 0x001 and $Msg0.Item(7)="00 0A 00 00 00 00 00 14" and $Msg0.Item(8)="0x20 (""Enhanced"")" Then			; Compare the Direction, Channel and Msg ID of the first row
				$FirstMsg=1
				ConsoleWrite("$FirstMsg = "& $FirstMsg &@CRLF)
			Endif


		EndIf
	EndIf


EndIf

	ConsoleWrite("$KeyFunCount="&$KeyFunCount&@CRLF)
	ConsoleWrite("$traceInfoMsgHndler="&$traceInfoMsgHndler&@CRLF)
	ConsoleWrite("$rCount="&$rCount&@CRLF)
	ConsoleWrite("$FirstMsg="&$FirstMsg&@CRLF)



	If $KeyFunCount = 2  and $rCount=1 and $traceInfoMsgHndler=1 and $FirstMsg=1 Then
		_WriteResult("Pass","TS_NS_LIN_81")
		ConsoleWrite("PASS")
	Else
	   _WriteResult("Fail","TS_NS_LIN_81")
	   ConsoleWrite("FAIL")
   EndIf

$isAppNotRes=_CloseApp()																	    ; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_NS_LIN_81")
EndIf

ConsoleWrite("****End : TS_NS_LIN_81.au3****"&@CRLF)
ConsoleWrite(@CRLF)

