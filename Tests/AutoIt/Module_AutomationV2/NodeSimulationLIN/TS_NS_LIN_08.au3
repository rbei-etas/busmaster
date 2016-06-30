#cs
;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_NS_LIN_08
; TestCases			:		Adding a Node - cancel button
; Test Data			:
; Test Setup		:		1.Select Lin->Node simulation -> Configure
							2.Active window
							3.Click on Tool to add new node.
							4.click on "CANCEL" button.


; Expected Results  : 		1.After step 3,"Node Details" window should be displayed .
							2.After step 4,New node should be created under root node.
;==========================================================================Test Procedure =========================================================================
#ce
;#include <IncludeFiles.au3>
ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_NS_LIN_08.au3****"&@CRLF)

_launchApp()
ProcessWait("BUSMASTER.exe")
Global $cppFileName = "NewNodeLIN_NewcppFile",$nodeName="TestNodeLIN",$WinNodeDetailsDisplayed=0,$NewcppFilecreated=0


WinActivate($WIN_BUSMASTER)

if winexists($WIN_BUSMASTER) then
	sleep(2000)
	_createConfig("cfxLINNS_08")															 ; Create new Configuration
	_SetViewToolBarLIN()																	 ; Select LIN Tool Bar to view.

	_linDriverSelection()																 ; Select LIN driver selection from xml configuration File.
	sleep(1000)

	_LINNodeSimConfigMenu()
	sleep(2000)
	Opt("WinDetectHiddenText", 0)
	Opt("WinSearchChildren", 1)
	Opt("WinTitleMatchMode", 1)
	WinWait("Configure Simulated Systems - LIN Bus")
	if winexists("Configure Simulated Systems - LIN Bus") then
		ConsoleWrite("Nodesim Lin window exists")
		$SimTVHWD= ControlGetHandle($WIN_BUSMASTER, "",$TVC_Handlers_CPP)							; Fetch the handle of tree view
		$SimFileHWD=_GUICtrlTreeView_GetItemHandle($SimTVHWD,"")								; Fetch the first item handle

		_GUICtrlTreeView_ClickItem($SimTVHWD,$SimFileHWD, "right") 								; Right click on the DB path item
		sleep(800)
		send("a")																				; Select "Add" from the right click menu
		sleep(1000)
		if WinWaitActive($WIN_NodeDetails,"",5) Then
			$WinNodeDetailsDisplayed =1
			ControlSend($WIN_NodeDetails,"",$TXT_NodeName_NodeDetails,$nodeName)			   ; Enter Filename
			sleep(1000)
			ControlClick($WIN_NodeDetails,"",$BTN_Cancel_NodeDetails)						   ; Click on cancel button
			sleep(1000)
			;ControlClick($WIN_NodeDetails,"",$BTN_OK_NodeDetails) 							; Click on OK button
		EndIf
	sleep(1000)
	ControlFocus ( $WIN_BUSMASTER, "", $SimTVHWD )
	$childNode=ControlTreeView ( $WIN_BUSMASTER, "", $SimTVHWD,"GetText","#0|#0" )						; Select the node
	;$nodeCount = _GUICtrlTreeView_GetCount($SimFileHWD)
	;$childNode = _GUICtrlTreeView_GetNextChild($SimTVHWD, $SimFileHWD)
	ConsoleWrite("$childNode="&$childNode&@CRLF)
	$nodeCount =_GUICtrlTreeView_GetCount($SimTVHWD)
	ConsoleWrite("$nodeCount="&$nodeCount&@CRLF)

	EndIf

EndIf


	If $WinNodeDetailsDisplayed=1 and $childNode="" and $nodeCount=1 Then
		_WriteResult("Pass","TS_NS_LIN_08")
		ConsoleWrite("PASS")
	Else
	   _WriteResult("Fail","TS_NS_LIN_08")
	   ConsoleWrite("FAIL")
	EndIf

$isAppNotRes=_CloseApp()																	; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_NS_LIN_08")
EndIf

ConsoleWrite("****End : TS_NS_LIN_08.au3****"&@CRLF)
ConsoleWrite(@CRLF)

