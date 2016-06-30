#cs
;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_NS_LIN_10
; TestCases			:		Deleting a Node
; Test Data			:
; Test Setup		:		1.Select Lin->Node simulation -> Configure
							2.Active window
							3.Click on Tool to add new node.
							4.Give input for node name in input box and click on "OK" button.
							5.Select the created node.
							6.click on Remove node from tool bar.
							7. Save configuration and close the application.
							8. Start application and make sure the previous configuration is loaded.
							9.Check node simulation list .


; Expected Results  : 		1.After step 9,The node should be removed in list.
;==========================================================================Test Procedure =========================================================================
#ce
;#include <IncludeFiles.au3>
ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_NS_LIN_10.au3****"&@CRLF)

_launchApp()
ProcessWait("BUSMASTER.exe")
Global $cppFileName = "NewNodeLIN_NewcppFile",$nodeName="TestNodeLIN",$WinNodeDetailsDisplayed=0,$NewcppFilecreated=0


WinActivate($WIN_BUSMASTER)

if winexists($WIN_BUSMASTER) then
	sleep(2000)
	;_loadConfig("cfxLINNS_10")															       ; Create new Configuration
	_createConfig("cfxLINNS_10")
	_SetViewToolBarLIN()																	   ; Select LIN Tool Bar to view.

	_linDriverSelection()																   ; Select LIN driver selection from xml configuration File.
	sleep(1000)

	_LINNodeSimConfigMenu()
	sleep(2000)
	Opt("WinDetectHiddenText", 0)
	Opt("WinSearchChildren", 1)
	Opt("WinTitleMatchMode", 1)
	WinWait("Configure Simulated Systems - LIN Bus")
	if winexists("Configure Simulated Systems - LIN Bus") then
		ConsoleWrite("Nodesim Lin window exists")
		$SimTVHWD= ControlGetHandle($WIN_BUSMASTER, "",$TVC_Handlers_CPP)						; Fetch the handle of tree view
		$SimFileHWD=_GUICtrlTreeView_GetItemHandle($SimTVHWD,"")								; Fetch the first item handle

		_GUICtrlTreeView_ClickItem($SimTVHWD,$SimFileHWD, "right") 								; Right click on the DB path item
		sleep(800)
		send("a")																				; Select "Add" from the right click menu
		sleep(1000)
		if WinWaitActive($WIN_NodeDetails,"",5) Then
			$WinNodeDetailsDisplayed =1
			ControlSend($WIN_NodeDetails,"",$TXT_NodeName_NodeDetails,$nodeName)			    ; Enter Filename
			sleep(1000)
			ControlClick($WIN_NodeDetails,"",$BTN_OK_NodeDetails)						        ; Click on OK button
			sleep(1000)
			;ControlClick($WIN_NodeDetails,"",$BTN_OK_NodeDetails) 							    ; Click on OK button
		EndIf
	sleep(1000)
	ControlFocus ( $WIN_BUSMASTER, "", $SimTVHWD )
	$childNode=ControlTreeView ( $WIN_BUSMASTER, "", $SimTVHWD,"GetText","#0|#0" )			    ; Select the node
	;$nodeCount = _GUICtrlTreeView_GetCount($SimFileHWD)
	;$childNode = _GUICtrlTreeView_GetNextChild($SimTVHWD, $SimFileHWD)
	ConsoleWrite("$childNode="&$childNode&@CRLF)
	$nodeCount =_GUICtrlTreeView_GetCount($SimTVHWD)
	ConsoleWrite("$nodeCount="&$nodeCount&@CRLF)

	$SimTVHWD= ControlGetHandle($WIN_BUSMASTER, "",$TVC_Handlers_CPP)						; Fetch the handle of tree view
	$SimFileHWD=_GUICtrlTreeView_GetItemHandle($SimTVHWD,"")								; Fetch the first item handle
	;$hndle=_GUICtrlTreeView_GetFirstChild($SimTVHWD, $SimFileHWD)
	_GUICtrlTreeView_ClickItem($SimTVHWD,$SimFileHWD, "right") 									    ; Right click on the First child node
	sleep(800)
	send("R")																				    ; Select "Remove" from the right click menu
	sleep(1000)
	send("Y")
	EndIf
	_saveConfig()
EndIf

	_CloseApp()																				    ; Close the app
	sleep(2000)
	_launchApp()
	ProcessWait("BUSMASTER.exe")
	WinActivate($WIN_BUSMASTER)
	if winexists($WIN_BUSMASTER) then
		;_loadConfig("cfxLINNS_10")
		_loadConfigFileAnyloc(_OutputDataPath()&"\"&"cfxLINNS_10.cfx")
		sleep(1000)
		_LINNodeSimConfigMenu()
		sleep(2000)
		Opt("WinDetectHiddenText", 0)
		Opt("WinSearchChildren", 1)
		Opt("WinTitleMatchMode", 1)
		WinWait("Configure Simulated Systems - LIN Bus")
		if winexists("Configure Simulated Systems - LIN Bus") then
			ControlFocus ( $WIN_BUSMASTER, "", $SimTVHWD )


			$SimTVHWD1= ControlGetHandle($WIN_BUSMASTER, "",$TVC_Handlers_CPP)					; Fetch the handle of tree view
			$SimFileHWD1=_GUICtrlTreeView_GetItemHandle($SimTVHWD,"")							; Fetch the first item handle

			$childNode1=ControlTreeView ( $WIN_BUSMASTER, "", $SimTVHWD1,"GetText","#0|#0" )	; Check the existance of the removed node
			;$nodeCount = _GUICtrlTreeView_GetCount($SimFileHWD)
			;$childNode = _GUICtrlTreeView_GetNextChild($SimTVHWD, $SimFileHWD)
			ConsoleWrite("$childNode1="&$childNode1&@CRLF)
			$nodeCount1 =_GUICtrlTreeView_GetCount($SimTVHWD1)
			ConsoleWrite("$nodeCount1="&$nodeCount1&@CRLF)
		EndIf
	EndIf

	ConsoleWrite("$WinNodeDetailsDisplayed="&$WinNodeDetailsDisplayed&@CRLF)
	ConsoleWrite("$childNode="&$childNode&@CRLF)
	ConsoleWrite("$nodeCount="&$nodeCount&@CRLF)
	ConsoleWrite("$childNode1="&$childNode1&@CRLF)
	ConsoleWrite("$nodeCount1="&$nodeCount1&@CRLF)

	If $WinNodeDetailsDisplayed=1 and $childNode="TestNodeLIN" and  $nodeCount=2 and $childNode1="" and $nodeCount1=1 Then
		_WriteResult("Pass","TS_NS_LIN_10")
		ConsoleWrite("PASS")
	Else
	   _WriteResult("Fail","TS_NS_LIN_10")
	   ConsoleWrite("FAIL")
	EndIf

$isAppNotRes=_CloseApp()																	    ; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_NS_LIN_10")
EndIf

ConsoleWrite("****End : TS_NS_LIN_10.au3****"&@CRLF)
ConsoleWrite(@CRLF)

