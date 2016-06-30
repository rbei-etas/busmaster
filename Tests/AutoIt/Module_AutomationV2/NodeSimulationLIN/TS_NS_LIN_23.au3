#cs
;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_NS_LIN_23
; TestCases			:		Enable All nodes and connect
; Test Data			:
; Test Setup		:		1.Select Lin->Node simulation -> Configure
							2.Active window
							3.Create two nodes under root node.
							4.Enable all nodes.
							5.right click on root node.
							6.Select "build All".
							7.Connect busmaster.


; Expected Results  : 		Enabling/Disabling Nodes should be done only on Check/Uncheck the checkbox associated with the node. Only Enabled nodes are loaded on connect.
;==========================================================================Test Procedure =========================================================================
#ce
;#include <IncludeFiles.au3>
ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_NS_LIN_23.au3****"&@CRLF)

_launchApp()
ProcessWait("BUSMASTER.exe")
Global $RemoveDialogBox = 0,$childNode="",$CheckBoxNode1="",$CheckBoxNode2="",$CheckLoadednode=0
Global $infoTrace[10] = ["","","","","","","","","",""]

WinActivate($WIN_BUSMASTER)

if winexists($WIN_BUSMASTER) then
	sleep(500)

	_loadConfig("cfxLINNS_23")															       ; Create new Configuration
	_SetViewToolBarLIN()																	   ; Select LIN Tool Bar to view.
	_linDriverSelection()																   ; Select LIN driver selection from xml configuration File.
	sleep(1000)

	_LINNodeSimConfigMenu()																		; Open Node Sim from menu
	sleep(500)
	$outputdataPath =_OutputDataPath()
	Opt("WinDetectHiddenText", 0)
	Opt("WinSearchChildren", 1)
	Opt("WinTitleMatchMode", 1)
	WinWait("Configure Simulated Systems - LIN Bus")

	if winexists("Configure Simulated Systems - LIN Bus") then

		;_RightClickonRootNode("a")																				;Right Click on created Node and select "Config"


		_SelectNodesCheckBox("LIN Bus|Node1")
		_SelectNodesCheckBox("LIN Bus|Node2")

		$CheckBoxNode1=_ValidateNodecheckBox(0)
		$CheckBoxNode2=_ValidateNodecheckBox(1)

		;$CheckBoxNode1  = _SelectNodesCheckBox(0)												; Select check box foe the first node
		;$CheckBoxNode2  = _SelectNodesCheckBox(1)												; Select check box foe the Second node

		ConsoleWrite("$CheckBoxNode1="&$CheckBoxNode1&@CRLF)
		ConsoleWrite("$CheckBoxNode2="&$CheckBoxNode2&@CRLF)
		sleep(1000)

		_RightClickonRootNode("B")																; Right click on root node and Build All
		sleep(1000)

		_ConnectDisconnect_LIN()																; Connect LIN
		Sleep(2000)
		_ConnectDisconnect_LIN()																; Disconnect LIN

		$CountTraceInfo= _GetTraceWinItemCount()												; Count info in trace Window.
		ConsoleWrite("$CountTraceInfo="&$CountTraceInfo&@CRLF)

		if $CountTraceInfo =6 Then
			For $i=0 to $CountTraceInfo-1
				$infoTrace[$i] = _GetTraceWinInfo($i)
				ConsoleWrite("$infoTrace="&$infoTrace[$i]&@CRLF)
			Next
		EndIf

		if $infoTrace[0] = "Node1-Load" and $infoTrace[1] = "Node2-Load" and $infoTrace[2] = "Node1-Connect" and $infoTrace[3] = "Node2-Connect" and $infoTrace[4] = "Node1-Unload" and $infoTrace[5] = "Node2-Unload"  Then
			$CheckLoadednode = 1
		EndIf
	EndIf

EndIf
	ConsoleWrite("$CheckBoxNode1="&$CheckBoxNode1&@CRLF)
	ConsoleWrite("$CheckBoxNode2="&$CheckBoxNode2&@CRLF)
	ConsoleWrite("$CheckLoadednode="&$CheckLoadednode&@CRLF)

	If $CheckBoxNode1 = True and $CheckBoxNode2= True and $CheckLoadednode=1  Then
		_WriteResult("Pass","TS_NS_LIN_23")
		ConsoleWrite("PASS")
	Else
	   _WriteResult("Fail","TS_NS_LIN_23")
	   ConsoleWrite("FAIL")
	EndIf

$isAppNotRes=_CloseApp()																	    ; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_NS_LIN_23")
EndIf

ConsoleWrite("****End : TS_NS_LIN_23.au3****"&@CRLF)
ConsoleWrite(@CRLF)

