#cs
;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_NS_LIN_79
; TestCases			:		Execute Time handler
; Test Data			:		1.Time name = TimerName
							2.Timer Value=2000 msec
; Test Setup		:		1.Open a file.
							2.Add key handler for the key 'a'  as

							STLIN_MSG sMsg;
							sMsg.m_ucMsgTyp = 0; // Master Response
							sMsg.m_ucChksumTyp = 0; // Classic
							sMsg.m_ucDataLen = 8;
							sMsg.m_ucMsgID = 0x12;
							sMsg.m_sWhichBit.m_aulData[0] = 10;  // Lower 4 Bytes
							sMsg.m_sWhichBit.m_aulData[1] = 20;  // Upper 4 Bytes
							sMsg.m_ucChannel = 1;

							// Send the message
							SendMsg(sMsg);
							3.Attach the file to any node
							4.Build and load the DLL
							5.Set the message window as active window
							6.Press key 'a'


; Expected Results  :		When key  'a' is pressed first time a message of message id 100 will send from the application. This message TX should be displayed on the message window.
;==========================================================================Test Procedure =========================================================================
#ce
;#include <IncludeFiles.au3>
ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_NS_LIN_79.au3****"&@CRLF)
If(@OSVersion <> "WIN_7") Then
	$tmOut = 0
Else
	$tmOut = 1000
EndIf
_launchApp()
ProcessWait("BUSMASTER.exe")

Global $infoTrace[10]=["","","","","","","","","",""]
Global $KeyFunCount=0,$traceInfoMsgHndler=0,$FirstMsg=0



WinActivate($WIN_BUSMASTER)

if winexists($WIN_BUSMASTER) then
	sleep(500)

	_loadConfig("cfxLINNS_63")															    ; Load the  Configuration
	_SetViewToolBarLIN()																	; Select LIN Tool Bar to view.
	_linDriverSelection()																	; Select LIN driver selection from xml configuration File.
	sleep(1000)
	_openLINClusterConfig()
	if winexists($WIN_ClustrConfigLIN) then
		ControlCommand($WIN_ClustrConfigLIN, "",$chckBoxEnableMaster, "Check", "") 			; Select Check box for enable Master mode.
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

		_AddNewNodeNS("TestNode79","LINCPP79",$TVC_Handlers_CPP)							;Create new Node with new CPP file

		$TVHWD=_GetWinHndlrNSTV()															; Fetch the handle of tree view
		ControlTreeView ( $WIN_LINNodesim, "", $TVHWD, "Select" ,"#0|#1")
		Send("{ENTER}")

		sleep(1000)
		;_AddMsgHandler(3,"","")															; Add key handler for "a"
		;_AddTimeHandler("Timer1","1000")
		_AddKeyHandler("a")

		sleep(1000)

		$KeyHndlrHWD=_GetCANHandlerHWD(2,4)													; Fetch the handle of the key handler item
		_GUICtrlTreeView_ClickItem($TVHWD, $KeyHndlrHWD, "Left") 							; click on Time handler item in the tree view
		$KeyFunCount = _GUICtrlTreeView_GetChildCount($TVHWD, $KeyHndlrHWD)
		ConsoleWrite("$TimeFunCount="&$KeyFunCount)
		if $KeyFunCount=1 Then
			for $i=0 to $KeyFunCount-1
				ControlTreeView ( $WIN_LINNodesim, "", $TVHWD, "Select" ,"#0|#4|#"&$i)
				sleep(1000)
				ControlSend($WIN_BUSMASTER, "", $TXT_TimeHandler_CPPFile, ""   & @CRLF)															; Write in the editor of Node sim
				ControlSend($WIN_BUSMASTER, "", $TXT_TimeHandler_CPPFile, "Trace(""Key Pressed=%c"""&",KeyValue);"   & @CRLF)					; Write in the editor of Node sim
				ControlSend($WIN_BUSMASTER, "", $TXT_TimeHandler_CPPFile, "STLIN_MSG sMsgM1;" & @CRLF)											; Write in the editor of Node sim
				ControlSend($WIN_BUSMASTER, "", $TXT_TimeHandler_CPPFile, "sMsgM1.messagetype = 0;    // master Response" & @CRLF)				; Write in the editor of Node sim
				ControlSend($WIN_BUSMASTER, "", $TXT_TimeHandler_CPPFile, "sMsgM1.checksumtype = 0; // Classic" & @CRLF)						; Write in the editor of Node sim
				ControlSend($WIN_BUSMASTER, "", $TXT_TimeHandler_CPPFile, "sMsgM1.dlc = 8; // DLC"& @CRLF)										; Write in the editor of Node sim
				ControlSend($WIN_BUSMASTER, "", $TXT_TimeHandler_CPPFile, "sMsgM1.id = "&$i+1&";  // Message Identifier" & @CRLF)				; Write in the editor of Node sim
				ControlSend($WIN_BUSMASTER, "", $TXT_TimeHandler_CPPFile, "sMsgM1.data[1] = 10;  // Lower 4 Bytes" & @CRLF)						; Write in the editor of Node sim
				ControlSend($WIN_BUSMASTER, "", $TXT_TimeHandler_CPPFile, "sMsgM1.data[7] = 20;  // Upper 4 Bytes" & @CRLF)						; Write in the editor of Node sim
				ControlSend($WIN_BUSMASTER, "", $TXT_TimeHandler_CPPFile, "sMsgM1.cluster = 1;" & @CRLF)										; Write in the editor of Node sim
				ControlSend($WIN_BUSMASTER, "", $TXT_TimeHandler_CPPFile, "SendMsg(sMsgM1);" & @CRLF)											; Write in the editor of Node sim
				;ControlSend($WIN_LINNodesim, "", $TXT_EventHandler_CPPFile, "Trace(""MsgId-8 Transmitted"");" & @CRLF)							; Write in the editor of Node sim
			Next
		endif


		_SaveAndCloseFunEditor()																	; save and Close Function editor

		WinActivate($WIN_BUSMASTER)
		_ConnectDisconnect_LIN()																	; Connect LIN
		Send("a")
		Sleep(4000)



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

		if $infoTrace[0]="Key Pressed=a" Then
			$traceInfoMsgHndler = 1
		EndIf

		$rCount=_GetLINMsgWinItemCount()															;count no. of items in msgwindow
		$CountCol=_GetLINMsgWinColCount()															;count no. of columns in msgwindow
		ConsoleWrite("$rCount="&$rCount&@CRLF)
		ConsoleWrite("$Count1="&$CountCol&@CRLF)

		Local $Msg0 = ObjCreate("System.Collections.ArrayList") 									; create object
		Local $Msg1 = ObjCreate("System.Collections.ArrayList") 									; create object

		if $rCount=2 Then
			_ConnectDisconnect_LIN()																	; Connect LIN
			Send("a")
			Sleep($tmOut)
			_ConnectDisconnect_LIN()
			$rCount=_GetLINMsgWinItemCount()
			sleep(1000)
		EndIf

		if $rCount = 1 Then


			$Msg0=_GetMsgWinInfo($LVC_CID_LINMsgWin,0,10)															; Fetch the first row data in the msg window

			if $Msg0.Item(0) <>" " and $Msg0.Item(1)="0x1" and $Msg0.Item(2)="LIN Message" and $Msg0.Item(3)="Tx" and $Msg0.Item(4)=1 and $Msg0.Item(5)=8 and $Msg0.Item(6)= "0x001" and $Msg0.Item(7)="00 0A 00 00 00 00 00 14" and $Msg0.Item(8)="0x20 (""Enhanced"")" Then			; Compare the Direction, Channel and Msg ID of the first row
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



	If $KeyFunCount = 1  and $rCount=1 and $traceInfoMsgHndler=1 and $FirstMsg=1 Then
		_WriteResult("Pass","TS_NS_LIN_79")
		ConsoleWrite("PASS")
	Else
	   _WriteResult("Fail","TS_NS_LIN_79")
	   ConsoleWrite("FAIL")
   EndIf

$isAppNotRes=_CloseApp()																	    ; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_NS_LIN_79")
EndIf

ConsoleWrite("****End : TS_NS_LIN_79.au3****"&@CRLF)
ConsoleWrite(@CRLF)

