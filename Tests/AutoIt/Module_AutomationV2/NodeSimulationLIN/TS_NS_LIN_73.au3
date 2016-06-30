#cs
;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_NS_LIN_73
; TestCases			:		Checking for Error slave no response
; Test Data			:
; Test Setup		:		NO slave response sent from one node.



; Expected Results  :		"A message defined in the Trace("<Message>") statement for OnError_Slave_No_Response() handler should be displayed in the trace Window.
;==========================================================================Test Procedure =========================================================================
#ce
;#include <IncludeFiles.au3>
ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_NS_LIN_73.au3****"&@CRLF)

_launchApp()
ProcessWait("BUSMASTER.exe")

Global $infoTrace[10]=["","","","","","","","","",""]
Global $ErrFunCount=0,$traceInfoErrHndler=0,$FirstMsg=0



WinActivate($WIN_BUSMASTER)

if winexists($WIN_BUSMASTER) then
	sleep(500)

	_loadConfig("cfxLINNS_73")															    ; Load the  Configuration
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

		_AddNewNodeNS("TestNode73","LINCPP73",$TVC_Handlers_CPP)							;Create new Node with new CPP file

		$TVHWD=_GetWinHndlrNSTV()															; Fetch the handle of tree view
		ControlTreeView ( $WIN_LINNodesim, "", $TVHWD, "Select" ,"#0|#1")
		Send("{ENTER}")

		sleep(1000)
		;_AddMsgHandler(3,"","")															; Add key handler for "a"
		;_AddTimeHandler("Timer1","1000")
		_AddErrHandler(3)

		sleep(1000)

		$ErrHndlrHWD=_GetCANHandlerHWD(2,5)													; Fetch the handle of the key handler item
		_GUICtrlTreeView_ClickItem($TVHWD, $ErrHndlrHWD, "Left") 							;  click on Time handler item in the tree view
		$ErrFunCount = _GUICtrlTreeView_GetChildCount($TVHWD, $ErrHndlrHWD)
		ConsoleWrite("$TimeFunCount="&$ErrFunCount)
		if $ErrFunCount=1 Then
			for $i=0 to $ErrFunCount-1
				ControlTreeView ( $WIN_LINNodesim, "", $TVHWD, "Select" ,"#0|#5|#"&$i)
				sleep(1000)
				ControlSend($WIN_BUSMASTER, "", $TXT_TimeHandler_CPPFile, ""   & @CRLF)														; Write in the editor of Node sim
				ControlSend($WIN_BUSMASTER, "", $TXT_TimeHandler_CPPFile, "Trace(""Error Slave No Response"");"   & @CRLF)						; Write in the editor of Node sim

			Next
		endif


		_SaveAndCloseFunEditor()																	; save and Close Function editor

		WinActivate($WIN_BUSMASTER)
		_ConnectDisconnect_LIN()																	; Connect LIN
		Sleep(4000)
		Send("a")
		Sleep(2000)



		_ConnectDisconnect_LIN()																	; Disconnect LIN
		sleep(2000)


		$CountTraceInfo= _GetTraceWinItemCount()													; Count info in trace Window.
		ConsoleWrite("$CountTraceInfo="&$CountTraceInfo&@CRLF)

		if $CountTraceInfo >1 Then
			$infoTrace = _GetTraceWinInfo(0)														; Checking the trace messages
			ConsoleWrite("$infoTrace="&$infoTrace&@CRLF)
		EndIf


		if $infoTrace="Error Slave No Response" Then
			$traceInfoErrHndler = 1
		EndIf

		$rCount=_GetLINMsgWinItemCount()															;count no. of items in msgwindow
		$CountCol=_GetLINMsgWinColCount()															;count no. of columns in msgwindow
		ConsoleWrite("$rCount="&$rCount&@CRLF)
		ConsoleWrite("$Count1="&$CountCol&@CRLF)

		Local $Msg0 = ObjCreate("System.Collections.ArrayList") 									; create object
		Local $Msg1 = ObjCreate("System.Collections.ArrayList") 									; create object



		if $rCount = 1 Then


			$Msg0=_GetMsgWinInfo($LVC_CID_LINMsgWin,0,10)															; Fetch the first row data in the msg window

			if $Msg0.Item(0) <>"" and $Msg0.Item(1)="0x0" and $Msg0.Item(2)="Error - Slave Not Responding" and $Msg0.Item(3)="" and $Msg0.Item(4)=1 and $Msg0.Item(6)= 0x014 and $Msg0.Item(7)="" and $Msg0.Item(8)="" Then			; Compare the Direction, Channel and Msg ID of the first row
				$FirstMsg=1
				ConsoleWrite("$FirstMsg = "& $FirstMsg &@CRLF)
			Endif


		EndIf
	EndIf


EndIf


	ConsoleWrite("$ErrFunCount="&$ErrFunCount&@CRLF)
	ConsoleWrite("$traceInfoErrHndler="&$traceInfoErrHndler&@CRLF)
	ConsoleWrite("$rCount="&$rCount&@CRLF)
	ConsoleWrite("$FirstMsg="&$FirstMsg&@CRLF)



	If $ErrFunCount = 1  and $rCount=1 and $traceInfoErrHndler=1 and $FirstMsg=1 Then
		_WriteResult("Pass","TS_NS_LIN_73")
		ConsoleWrite("PASS")
	Else
	   _WriteResult("Fail","TS_NS_LIN_73")
	   ConsoleWrite("FAIL")
   EndIf

$isAppNotRes=_CloseApp()																	    ; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_NS_LIN_73")
EndIf

ConsoleWrite("****End : TS_NS_LIN_73.au3****"&@CRLF)
ConsoleWrite(@CRLF)

