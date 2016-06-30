#cs
;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_NS_LIN_88
; TestCases			:		DLL Handler
; Test Data			:
; Test Setup		:		1.open function editor and select DLL handler.
							2.Right click on it and select "ADD".
							3.Select load check box in the "All DLL handler window".
							4.Save the node and connect.

; Expected Results  :		"The DLL handlers will be executed displaying the Trace message when it triggers.
;==========================================================================Test Procedure =========================================================================
#ce
;#include <IncludeFiles.au3>
ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_NS_LIN_88.au3****"&@CRLF)

_launchApp()
ProcessWait("BUSMASTER.exe")

Global $infoTrace[10]=["","","","","","","","","",""]
Global $DLLFunCount=0,$traceInfoMsgHndler=0



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

		_AddNewNodeNS("TestNode88","LINCPP88",$TVC_Handlers_CPP)							;Create new Node with new CPP file

		$TVHWD=_GetWinHndlrNSTV()															; Fetch the handle of tree view
		ControlTreeView ( $WIN_LINNodesim, "", $TVHWD, "Select" ,"#0|#1")
		Send("{ENTER}")

		sleep(1000)
		_AddDLLHandler(1)

		$DLLHndlrHWD=_GetCANHandlerHWD(2,6)													; Fetch the handle of the key handler item
		_GUICtrlTreeView_ClickItem($TVHWD, $DLLHndlrHWD, "Left") 							;  click on Time handler item in the tree view
		$DLLFunCount = _GUICtrlTreeView_GetChildCount($TVHWD, $DLLHndlrHWD)
		ConsoleWrite("$TimeFunCount="&$DLLFunCount)
		if $DLLFunCount=1 Then
				ControlTreeView ( $WIN_LINNodesim, "", $TVHWD, "Select" ,"#0|#6|#0")
				sleep(1000)
				ControlSend($WIN_BUSMASTER, "", $TXT_TimeHandler_CPPFile, ""   & @CRLF)														; Write in the editor of Node sim
				ControlSend($WIN_BUSMASTER, "", $TXT_TimeHandler_CPPFile, "Trace(""DLL handler"");" & @CRLF)								; Write in the editor of Node sim
		endif


		_SaveAndCloseFunEditor()																	; save and Close Function editor

		WinActivate($WIN_BUSMASTER)
		_ConnectDisconnect_LIN()																	; Connect LIN
		;Send("a")
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

		if $infoTrace[0]="DLL handler" Then
			$traceInfoMsgHndler = 1
		EndIf

		$rCount=_GetLINMsgWinItemCount()																				;count no. of items in msgwindow
		$CountCol=_GetLINMsgWinColCount()																					;count no. of columns in msgwindow
		ConsoleWrite("$rCount="&$rCount&@CRLF)
		ConsoleWrite("$Count1="&$CountCol&@CRLF)

		Local $Msg0 = ObjCreate("System.Collections.ArrayList") 														; create object
		Local $Msg1 = ObjCreate("System.Collections.ArrayList") 														; create object


	EndIf


EndIf

	ConsoleWrite("$DLLFunCount="&$DLLFunCount&@CRLF)
	ConsoleWrite("$traceInfoMsgHndler="&$traceInfoMsgHndler&@CRLF)
	;ConsoleWrite("$rCount="&$rCount&@CRLF)
	;ConsoleWrite("$FirstMsg="&$FirstMsg&@CRLF)



	If $DLLFunCount = 1   and $traceInfoMsgHndler=1 Then
		_WriteResult("Pass","TS_NS_LIN_88")
		ConsoleWrite("PASS")
	Else
	   _WriteResult("Fail","TS_NS_LIN_88")
	   ConsoleWrite("FAIL")
   EndIf

$isAppNotRes=_CloseApp()																	    ; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_NS_LIN_88")
EndIf

ConsoleWrite("****End : TS_NS_LIN_88.au3****"&@CRLF)
ConsoleWrite(@CRLF)

