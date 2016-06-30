#cs
;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_NS_LIN_19
; TestCases			:		Delete All Nodes
; Test Data			:
; Test Setup		:		1.Select Lin->Node simulation -> Configure
							2.Active window
							3.Create two nodes under root node.
							4.Select root node.
							5.Click on remove button in tool bar.
							5.Select "Yes" of confirmation dialog.


; Expected Results  : 		1.After step5,A confirmation message should be displayed to delete all the nodes in the simulated systems with a Yes or No buttons in the dialog box.
							2.All nodes under root should be deleted.
;==========================================================================Test Procedure =========================================================================
#ce
;#include <IncludeFiles.au3>
ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_NS_LIN_19.au3****"&@CRLF)

_launchApp()
ProcessWait("BUSMASTER.exe")
Global $RemoveDialogBox = 0,$childNode=""


WinActivate($WIN_BUSMASTER)

if winexists($WIN_BUSMASTER) then
	sleep(500)

	_loadConfig("cfxLINNS_19")															       ; Create new Configuration
	;_SetViewToolBarLIN()																	   ; Select LIN Tool Bar to view.
	;_linDriverSelectionXml()																   ; Select LIN driver selection from xml configuration File.
	sleep(1000)

	_LINNodeSimConfigMenu()																		; Open Node Sim from menu
	sleep(500)
	$outputdataPath =_OutputDataPath()
	Opt("WinDetectHiddenText", 0)
	Opt("WinSearchChildren", 1)
	Opt("WinTitleMatchMode", 1)
	WinWait("Configure Simulated Systems - LIN Bus")

	if winexists("Configure Simulated Systems - LIN Bus") then

		_RightClickonRootNode("R")																				;Right Click on created Node and select "Config"

		if WinWaitActive("","You are about to Delete all the Nodes",2) Then										; wait for save configuration dialog
			ControlClick($WIN_BUSMASTER,"You are about to Delete all the Nodes","[CLASS:Button; INSTANCE:1]")	; Click on No button
			$RemoveDialogBox =1
		EndIf

		$childNode1= _CountNodeInTVNS()
		sleep(1000)

		if $childNode1= -1 Then
			$childNode =0
		EndIf
		ConsoleWrite("$childNode="&$childNode&@CRLF)
		ConsoleWrite("$RemoveDialogBox="&$RemoveDialogBox&@CRLF)

	EndIf

EndIf


	If $RemoveDialogBox = 1 and $childNode=0 Then
		_WriteResult("Pass","TS_NS_LIN_19")
		ConsoleWrite("PASS")
	Else
	   _WriteResult("Fail","TS_NS_LIN_19")
	   ConsoleWrite("FAIL")
	EndIf

$isAppNotRes=_CloseApp()																	    ; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_NS_LIN_19")
EndIf

ConsoleWrite("****End : TS_NS_LIN_19.au3****"&@CRLF)
ConsoleWrite(@CRLF)

