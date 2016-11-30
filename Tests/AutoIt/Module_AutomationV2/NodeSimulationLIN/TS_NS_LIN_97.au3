#cs
;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_NS_LIN_97
; TestCases			:		Utility function usage
; Test Data			:
; Test Setup		:		1.repeate the steps from above test case.
							2.Write code for the function as per test data given.Save it.
							3.Use the function in any message handler and save it.
							4.Connect busamster.


; Expected Results  :		1.After step4,The Utility function should be called when message hndler triggers.
;==========================================================================Test Procedure =========================================================================
#ce
;#include <IncludeFiles.au3>
ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_NS_LIN_97.au3****"&@CRLF)

_launchApp()
ProcessWait("BUSMASTER.exe")
;Global $RemoveDialogBox = 0,$childNode="",$CheckBoxNode1="",$CheckBoxNode2="",$CheckLoadednode=0
Global $infoTrace[10]=["","","","","","","","","",""]
Global $UtilFunCount=0,$traceInfoMsgHndler=0,$SecndMsg=0,$FirstMsg=0
local $k=0

WinActivate($WIN_BUSMASTER)

if winexists($WIN_BUSMASTER) then
	sleep(500)

	_loadConfig("cfxLINNS_97")															    ; Load the  Configuration
	_SetViewToolBarLIN()																	; Select LIN Tool Bar to view.
	sleep(1000)
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

		_AddNewNodeNS("TestNode97","LINCPP97",$TVC_Handlers_CPP)						;Create new Node with new CPP file

		$TVHWD=_GetWinHndlrNSTV()															; Fetch the handle of tree view
		ControlTreeView ( $WIN_LINNodesim, "", $TVHWD, "Select" ,"#0|#1")
		Send("{ENTER}")

		sleep(1000)
		_AddMsgHandler(1,8,"")																; Add msg handler with msg-id-8
		sleep(1000)


		$msgHndlrHWD=_GetCANHandlerHWD(2,2)													; Fetch the handle of the Msg handler item
		_GUICtrlTreeView_ClickItem($TVHWD, $msgHndlrHWD, "Left") 							; Right click on Busevent handler item in the tree view
		$MsgFunCount = _GUICtrlTreeView_GetChildCount($TVHWD, $msgHndlrHWD)

		if $MsgFunCount=1 Then
			ControlTreeView ( $WIN_LINNodesim, "", $TVHWD, "Select" ,"#0|#2|#0")
			sleep(1000)
			ControlSend($WIN_BUSMASTER, "", $TXT_EventHandler_CPPFile, ""   & @CRLF)							; Write in the editor of Node sim
			ControlSend($WIN_BUSMASTER, "", $TXT_EventHandler_CPPFile, "int ret=Utils_UtilityFun();"   & @CRLF)							; Write in the editor of Node sim
			ControlSend($WIN_BUSMASTER, "", $TXT_TimeHandler_CPPFile, "Trace(""RetValue=%d"""&",ret);"   & @CRLF)						; Write in the editor of Node sim


		endif

		_AddUtilityFun("int","UtilityFun()")
		Sleep(1000)

		$UtilFunHWD=_GetCANHandlerHWD(2,7)													; Fetch the handle of the Msg handler item
		_GUICtrlTreeView_ClickItem($TVHWD, $msgHndlrHWD, "Left") 							; Right click on Busevent handler item in the tree view
		$UtilFunCount = _GUICtrlTreeView_GetChildCount($TVHWD, $UtilFunHWD)

		if $UtilFunCount=1 Then
			ControlTreeView ( $WIN_LINNodesim, "", $TVHWD, "Select" ,"#0|#7|#0")
			sleep(1000)
			ControlSend($WIN_BUSMASTER, "", $TXT_EventHandler_CPPFile, ""   & @CRLF)							; Write in the editor of Node sim
			ControlSend($WIN_BUSMASTER, "", $TXT_EventHandler_CPPFile, "Trace(""Utility Function triggered"");"   & @CRLF)								; Write in the editor of Node sim
			ControlSend($WIN_BUSMASTER, "", $TXT_TimeHandler_CPPFile, "STLIN_MSG sMsgM1;" & @CRLF)										; Write in the editor of Node sim
			ControlSend($WIN_BUSMASTER, "", $TXT_TimeHandler_CPPFile, "sMsgM1.messagetype = 0;    // master Response" & @CRLF)			; Write in the editor of Node sim
			ControlSend($WIN_BUSMASTER, "", $TXT_TimeHandler_CPPFile, "sMsgM1.checksumtype = 0; // Classic" & @CRLF)					; Write in the editor of Node sim
			ControlSend($WIN_BUSMASTER, "", $TXT_TimeHandler_CPPFile, "sMsgM1.dlc = 8; // DLC"& @CRLF)							; Write in the editor of Node sim
			ControlSend($WIN_BUSMASTER, "", $TXT_TimeHandler_CPPFile, "sMsgM1.id = 4;  // Message Identifier" & @CRLF)	; Write in the editor of Node sim
			ControlSend($WIN_BUSMASTER, "", $TXT_TimeHandler_CPPFile, "sMsgM1.data[1] = 10;  // Lower 4 Bytes" & @CRLF)	; Write in the editor of Node sim
			ControlSend($WIN_BUSMASTER, "", $TXT_TimeHandler_CPPFile, "sMsgM1.data[7] = 20;  // Upper 4 Bytes" & @CRLF)	; Write in the editor of Node sim
			ControlSend($WIN_BUSMASTER, "", $TXT_TimeHandler_CPPFile, "sMsgM1.cluster = 1;" & @CRLF)									; Write in the editor of Node sim
			ControlSend($WIN_BUSMASTER, "", $TXT_TimeHandler_CPPFile, "SendMsg(sMsgM1);" & @CRLF)											; Write in the editor of Node sim
			ControlSend($WIN_BUSMASTER, "", $TXT_TimeHandler_CPPFile, "return 1;" & @CRLF)											; Write in the editor of Node sim

		endif

		_SaveAndCloseFunEditor()																	; save and Close Function editor

		WinActivate($WIN_BUSMASTER)
		_ConnectDisconnect_LIN()																	; Connect LIN
		Sleep(2000)
		_ConnectDisconnect_LIN()																	; Disconnect LIN

		$rCount=_GetLINMsgWinItemCount()

		if $rCount=0 Then
			Do
				;----cleare trace window-----
				MouseClick("right", 880, 449)
				sleep(1000)
				;Send("{RIGHT}")
				sleep(1000)
				Send("{DOWN 2}")
				Send("{ENTER}")
				Sleep(2000)
				;----------------------------
				_ConnectDisconnect_LIN()																	; Connect LIN
				Sleep(500)
				_ConnectDisconnect_LIN()
				$rCount=_GetLINMsgWinItemCount()
				$k = $k + 1
			Until ($rCount = 4 Or $k=10)
		EndIf

		$CountTraceInfo= _GetTraceWinItemCount()													; Count info in trace Window.
		ConsoleWrite("$CountTraceInfo="&$CountTraceInfo&@CRLF)



		if $CountTraceInfo >=4 Then
			For $i=0 to $CountTraceInfo-1
				$infoTrace[$i] = _GetTraceWinInfo($i)
				ConsoleWrite("$infoTrace="&$infoTrace[$i]&@CRLF)
			Next
		EndIf

		if $infoTrace[0]="Utility Function triggered" and $infoTrace[1]="RetValue=1"  Then
			$traceInfoMsgHndler = 1
		EndIf

		$rCount=_GetLINMsgWinItemCount()																				;count no. of items in msgwindow
		$Count1=_GetLINMsgWinColCount()																					;count no. of columns in msgwindow
		ConsoleWrite("$rCount="&$rCount&@CRLF)
		ConsoleWrite("$Count1="&$Count1&@CRLF)

		Local $Msg0 = ObjCreate("System.Collections.ArrayList") 														; create object
		Local $Msg1 = ObjCreate("System.Collections.ArrayList") 														; create object





		if $rCount=5 Then
			_ConnectDisconnect_LIN()																	; Connect LIN
			Sleep(1000)
			_ConnectDisconnect_LIN()
			$rCount=_GetLINMsgWinItemCount()
			;sleep(1000)
		EndIf

		if $rCount>=4 Then


			$Msg0=_GetMsgWinInfo($LVC_CID_LINMsgWin,0,10)															; Fetch the first row data in the msg window
			$Msg1=_GetMsgWinInfo($LVC_CID_LINMsgWin,1,10)															; Fetch the second row data in the msg window
			sleep(2000)

			for $i=0 to $rCount-1
				$Msg=_GetMsgWinInfo($LVC_CID_LINMsgWin,$i,10)
				for $j=0 to $Count1-2
					$Value =$Msg.Item($j)
					ConsoleWrite("$Msg[" & $i &"] : "& $Value &@CRLF)
				Next
			Next


			if $Msg0.Item(0) <>"" and $Msg0.Item(1)="0x8" and $Msg0.Item(2)="LIN Message" and $Msg0.Item(3)="Tx" and $Msg0.Item(4)=1 and $Msg0.Item(5)=8 and $Msg0.Item(6)=0x008 and $Msg0.Item(7)="00 0A 00 00 00 00 00 14" and $Msg0.Item(8)="0xD9 (""Enhanced"")" Then			; Compare the Direction, Channel and Msg ID of the first row
				$FirstMsg=1
				ConsoleWrite("$FirstMsg = "& $FirstMsg &@CRLF)
			Endif

			if $Msg1.Item(0) <>"" and $Msg1.Item(1)="0x4" and $Msg1.Item(2)="LIN Message" and $Msg1.Item(3)="Tx" and $Msg1.Item(4)=1 and $Msg1.Item(5)=8 and $Msg1.Item(6)=0x004 and $Msg1.Item(7)="00 0A 00 00 00 00 00 14" and $Msg1.Item(8)="0x1D (""Enhanced"")" Then			; Compare the Direction, Channel and Msg ID of the first row
				$SecndMsg=1
				ConsoleWrite("$SecndMsg = "& $SecndMsg &@CRLF)
			EndIf
		EndIf
	EndIf


EndIf

	ConsoleWrite("$UtilFunCount="&$UtilFunCount&@CRLF)
	ConsoleWrite("$traceInfoMsgHndler="&$traceInfoMsgHndler&@CRLF)
	ConsoleWrite("$FirstMsg="&$FirstMsg&@CRLF)
	ConsoleWrite("$SecndMsg="&$SecndMsg&@CRLF)


	If $UtilFunCount = 1   and $traceInfoMsgHndler=1 and $FirstMsg=1 and $SecndMsg=1 Then
		_WriteResult("Pass","TS_NS_LIN_97")
		ConsoleWrite("PASS")
	Else
	   _WriteResult("Fail","TS_NS_LIN_97")
	   ConsoleWrite("FAIL")
   EndIf

$isAppNotRes=_CloseApp()																	    ; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_NS_LIN_97")
EndIf

ConsoleWrite("****End : TS_NS_LIN_97.au3****"&@CRLF)
ConsoleWrite(@CRLF)

