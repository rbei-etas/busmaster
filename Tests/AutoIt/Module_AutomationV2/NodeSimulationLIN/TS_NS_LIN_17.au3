#cs
;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_NS_LIN_17
; TestCases			:		Configuring a Node usage
; Test Data			:
; Test Setup		:		1.Select Lin->Node simulation -> Configure
							2.Active window
							3.Click on Tool to add new node.
							4.Give input for node name in input box and click on brows button.
							5.Provide new cpp file file name and click on Ok button.
							6.Select the created node and click on configure button in tool bar.
							7.Change node name and .cpp file attached.
							8.Click on "OK" button.


; Expected Results  : 		1.After step 7,Node details window should get closed.
							2.Node name and .cpp file configured are changed.
;==========================================================================Test Procedure =========================================================================
#ce
;#include <IncludeFiles.au3>
ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_NS_LIN_14.au3****"&@CRLF)

_launchApp()
ProcessWait("BUSMASTER.exe")
Global $text=0,$cppFilename ="LINCPP17",$cppFilenameToConfig="LINCPP17_config",$childNode="",$NewcppFilecreated=0,$NodeNameToConfig="TestNode17_config"


WinActivate($WIN_BUSMASTER)

if winexists($WIN_BUSMASTER) then
	sleep(500)

	_loadConfig("cfxLINNS_17")															       ; Create new Configuration
	;_SetViewToolBarLIN()																	   ; Select LIN Tool Bar to view.
	;_linDriverSelectionXml()																   ; Select LIN driver selection from xml configuration File.
	sleep(1000)

	_LINNodeSimConfigMenu()																		; Open Node Sim from menu
	sleep(2000)
	$outputdataPath =_OutputDataPath()
	Opt("WinDetectHiddenText", 0)
	Opt("WinSearchChildren", 1)
	Opt("WinTitleMatchMode", 1)
	WinWait("Configure Simulated Systems - LIN Bus")

	if winexists("Configure Simulated Systems - LIN Bus") then

		_RightClickonChildItemSelect("C")															;Right Click on created Node and select "Config"

		_NodeConfigure($NodeNameToConfig,$cppFilenameToConfig)
		;_SaveAndCloseFunEditor()
		sleep(1000)

		$childNode= _GetChildItemDetailsNS()
		sleep(2000)
		ConsoleWrite("$childNode="&$childNode&@CRLF)

		$CPPFilePath = _OutputDataPath()																; Fetch the path of the output data folder
		$pathFrCpp = $CPPFilePath&"\"&$cppFilenameToConfig&".cpp"
		ConsoleWrite("$pathFrCpp="&$pathFrCpp&@CRLF)
		If FileExists($pathFrCpp) Then
			$NewcppFilecreated =1
		Endif
		ConsoleWrite("$NewcppFilecreated="&$NewcppFilecreated&@CRLF)
	EndIf

EndIf


	If $childNode = $NodeNameToConfig and $NewcppFilecreated=1 Then
		_WriteResult("Pass","TS_NS_LIN_17")
		ConsoleWrite("PASS")
	Else
	   _WriteResult("Fail","TS_NS_LIN_17")
	   ConsoleWrite("FAIL")
	EndIf

$isAppNotRes=_CloseApp()																	    ; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_NS_LIN_17")
EndIf

ConsoleWrite("****End : TS_NS_LIN_17.au3****"&@CRLF)
ConsoleWrite(@CRLF)

