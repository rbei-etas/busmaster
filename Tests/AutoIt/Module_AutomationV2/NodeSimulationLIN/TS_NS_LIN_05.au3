#cs
;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_NS_LIN_05
; TestCases			:		Adding a Node in Node SImulation
; Test Data			:
; Test Setup		:		1.Select Lin->Node simulation -> Configure
							2.Active window
							3.Click on Tool to add new node.
							4.Give input for node name in input box and click on brows button.
							5.Provide new cpp file file name and click on Ok button.


; Expected Results  : 		1.After step 3,"Node Details" window should be displayed .
							2.After step 5,New node should be created under root node with cpp file provided.
;==========================================================================Test Procedure =========================================================================
#ce
;#include <IncludeFiles.au3>
ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_NS_LIN_05.au3****"&@CRLF)

_launchApp()
ProcessWait("BUSMASTER.exe")
Global $cppFileName = "NewNodeLIN_NewcppFile",$nodeName="TestNodeLIN",$WinNodeDetailsDisplayed=0,$NewcppFilecreated=0,$nodeCount=0


WinActivate($WIN_BUSMASTER)

if winexists($WIN_BUSMASTER) then
	sleep(2000)
	_createConfig("cfxLINNS_05")															 ; Create new Configuration
	;_SetViewToolBarLIN()																	 ; Select LIN Tool Bar to view.

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
			ControlSend($WIN_NodeDetails,"",$TXT_NodeName_NodeDetails,$nodeName)			   	; Enter Filename
			sleep(1000)
			if $cppFileName = "" Then
				ControlClick($WIN_NodeDetails,"",$BTN_OK_NodeDetails) 							; Click on OK button
				sleep(1000)
			Else
				$CPPFilePath = _OutputDataPath()																; Fetch the path of the output data folder
				ControlClick($WIN_NodeDetails,"",$BTN_Brows_NodeDetails)										; Click on the "Add New File" button
				;WinWaitActive($WIN_AddCPPFile,"",5)															; Wait for the window to get active
				Sleep(1000)
				if WinExists($WinselectFile) Then																; If window exits then enter the file name
					ConsoleWrite("Select File -----")
					ControlSend($WinselectFile,"",$TXT_FileName_AddCPPFile,$CPPFilePath&"\"&$CppFileName)   	; Enter Filename
					sleep(1000)
					ControlClick($WinselectFile,"",$BTN_Open_AddCPPFile) 										; Click on Open button
					sleep(1000)

				EndIf
			EndIf
			sleep(500)
			ControlClick($WIN_NodeDetails,"",$BTN_OK_NodeDetails) 									; Click on OK button
		EndIf
	sleep(1000)
	Send("!{F4}")

	;_GUICtrlTreeView_Expand($TVHWD,"",True)													; Expand the tree view items
	;sleep(1000)

	ControlFocus ( $WIN_BUSMASTER, "", $SimTVHWD )
	$childNode=ControlTreeView ( $WIN_BUSMASTER, "", $SimTVHWD,"GetText","#0|#0" )						; Select the node
	;$nodeCount = _GUICtrlTreeView_GetCount($SimFileHWD)
	;$childNode = _GUICtrlTreeView_GetNextChild($SimTVHWD, $SimFileHWD)
	ConsoleWrite("$childNode="&$childNode&@CRLF)
	$nodeCount =_GUICtrlTreeView_GetCount($SimTVHWD)
	ConsoleWrite("$nodeCount="&$nodeCount&@CRLF)

	$pathFrCpp = $CPPFilePath&"\"&$cppFileName&".cpp"
	ConsoleWrite("$pathFrCpp="&$pathFrCpp&@CRLF)
		If FileExists($pathFrCpp) Then
			$NewcppFilecreated =1
		Endif
		ConsoleWrite("$NewcppFilecreated="&$NewcppFilecreated&@CRLF)
	EndIf

EndIf


	If $WinNodeDetailsDisplayed=1 and $NewcppFilecreated=1 and $childNode="TestNodeLIN" and $nodeCount =2 Then
		_WriteResult("Pass","TS_NS_LIN_05")
		ConsoleWrite("PASS")
	Else
	   _WriteResult("Fail","TS_NS_LIN_05")
	   ConsoleWrite("FAIL")
	EndIf

$isAppNotRes=_CloseApp()																	; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_NS_LIN_05")
EndIf

ConsoleWrite("****End : TS_NS_LIN_05.au3****"&@CRLF)
ConsoleWrite(@CRLF)

