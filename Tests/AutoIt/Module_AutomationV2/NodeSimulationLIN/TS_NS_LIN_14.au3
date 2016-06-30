#cs
;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_NS_LIN_14
; TestCases			:		Compiling a Node
; Test Data			:
; Test Setup		:		1.Select Lin->Node simulation -> Configure
							2.Active window
							3.Click on Tool to add new node.
							4.Give input for node name in input box and click on brows button.
							5.Provide new cpp file file name and click on Ok button.
							6.Select the created node.


; Expected Results  : 		Afters step 6,The toolbar button or the menu “Build” should be enabled.It should be  used to compile the CPP file added for a node. On selecting this button/menu,
							the compilation should happen and the results should be displayed in an output window.The below message should be displayed in out put window.
							message - <path>/<nodename.dll> created successafully.
;==========================================================================Test Procedure =========================================================================
#ce
;#include <IncludeFiles.au3>
ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_NS_LIN_14.au3****"&@CRLF)

_launchApp()
ProcessWait("BUSMASTER.exe")
Global $text=0,$cppFilename ="LINCPP"


WinActivate($WIN_BUSMASTER)

if winexists($WIN_BUSMASTER) then
	sleep(500)

	_loadConfig("cfxLINNS_11")															       ; Create new Configuration
	_SetViewToolBarLIN()																	   ; Select LIN Tool Bar to view.
	_linDriverSelection()																   ; Select LIN driver selection from xml configuration File.
	sleep(1000)

	_LINNodeSimConfigMenu()																		; Open Node Sim from menu
	sleep(2000)
	$outputdataPath =_OutputDataPath()
	Opt("WinDetectHiddenText", 0)
	Opt("WinSearchChildren", 1)
	Opt("WinTitleMatchMode", 1)
	WinWait("Configure Simulated Systems - LIN Bus")

	if winexists("Configure Simulated Systems - LIN Bus") then

		_AddNewNodeNS("TestNode14",$cppFilename,$TVC_Handlers_CPP)									;Create new Node with new CPP file
		;_SaveAndCloseFunEditor()
		sleep(1000)

		_RightClickonChildItemSelect("B")															;Right Click on created Node and select "Build"

		sleep(2000)

		$text1=_OutputWindowText(1)																	; Get the Text from Output Window
		$text2=_OutputWindowText(2)																	; Get the Text from Output Window

		ConsoleWrite("$text1="&$text1&@CRLF)
		ConsoleWrite("$text2="&$text2&@CRLF)

		$Expctdtext1 = $outputdataPath&"\"&$cppFilename&".cpp"
		$Expctdtext2 = $outputdataPath&"\"&$cppFilename&".dll created successfully"

		ConsoleWrite("$Expctdtext1="&$Expctdtext1&@CRLF)
		ConsoleWrite("$Expctdtext2="&$Expctdtext2&@CRLF)


	EndIf
EndIf


	If $text1=$outputdataPath&"\"&$cppFilename&".cpp" and $text2=$outputdataPath&"\"&$cppFilename&".dll created successfully" Then
		_WriteResult("Pass","TS_NS_LIN_14")
		ConsoleWrite("PASS")
	Else
	   _WriteResult("Fail","TS_NS_LIN_14")
	   ConsoleWrite("FAIL")
	EndIf

$isAppNotRes=_CloseApp()																	    ; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_NS_LIN_14")
EndIf

ConsoleWrite("****End : TS_NS_LIN_14.au3****"&@CRLF)
ConsoleWrite(@CRLF)

