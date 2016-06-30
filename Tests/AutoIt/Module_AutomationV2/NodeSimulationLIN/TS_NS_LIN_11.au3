#cs
;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_NS_LIN_11
; TestCases			:		Editing a Node
; Test Data			:
; Test Setup		:		1.Select Lin->Node simulation -> Configure
							2.Active window
							3.Click on Tool to add new node.
							4.Give input for node name in input box and click on brows button.
							5.Provide new cpp file file name and click on Ok button.
							6.close function editor.
							7.Then select the created node and click on edit button in tool bar.
							8.Create function for message handler .Save and close it.


; Expected Results  : 		1.After step 4,A node should be allowed to be edited by opening the CPP file when the toolbar button/menu “Edit” is selected. Function editor should be displayed with no functions created under any handler.
							2.After step7,Node should be updated with new function under mesasge handler.
;==========================================================================Test Procedure =========================================================================
#ce
;#include <IncludeFiles.au3>
ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_NS_LIN_11.au3****"&@CRLF)

_launchApp()
ProcessWait("BUSMASTER.exe")
Global $FunCountmsgHndlr=0


WinActivate($WIN_BUSMASTER)

if winexists($WIN_BUSMASTER) then
	sleep(500)

	_loadConfig("cfxLINNS_11")															       ; Create new Configuration
	;_SetViewToolBarLIN()																	   ; Select LIN Tool Bar to view.
	;_linDriverSelectionXml()																   ; Select LIN driver selection from xml configuration File.
	sleep(1000)

	_LINNodeSimConfigMenu()																		; Open Node Sim from menu
	sleep(2000)

	Opt("WinDetectHiddenText", 0)
	Opt("WinSearchChildren", 1)
	Opt("WinTitleMatchMode", 1)
	WinWait("Configure Simulated Systems - LIN Bus")

	if winexists("Configure Simulated Systems - LIN Bus") then

		_AddNewNodeNS("TestEditNode","LINCPP",$TVC_Handlers_CPP)								;Create new Node with new CPP file

		_RightClickonChildItemSelect("E")

		sleep(1000)

		_AddMsgHandler(1,12,"")
		_SaveAndCloseFunEditor()


		_RightClickonChildItemSelect("E")

		$FunCountmsgHndlr = _CountFunInMsgHndler()

		ConsoleWrite("$FunCountmsgHndlr="&$FunCountmsgHndlr)
		_SaveAndCloseFunEditor()

		;_saveConfig()
	EndIf
EndIf


	If $FunCountmsgHndlr=1 Then
		_WriteResult("Pass","TS_NS_LIN_11")
		ConsoleWrite("PASS")
	Else
	   _WriteResult("Fail","TS_NS_LIN_11")
	   ConsoleWrite("FAIL")
	EndIf

$isAppNotRes=_CloseApp()																	    ; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_NS_LIN_11")
EndIf

ConsoleWrite("****End : TS_NS_LIN_11.au3****"&@CRLF)
ConsoleWrite(@CRLF)

