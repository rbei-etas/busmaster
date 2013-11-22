;===========Main Script==============
; Framework			: Modular Driven Framework
; Test Strategy		: Black Box
; Critical 			: Y
;===========Main Script==============

; Include all the necessary Library .au3 files,Global Function, Variable Declaration and AppCrash check

#include <Data_DeclarationModV2.au3>
#include <Excel.au3>
#include <GuiStatusBar.au3>
#include <globalFunc4ModuleV2.au3>												; Global Function
#include <GuiComboBoxEx.au3>
#include <GuiTreeView.au3>
#include <GuiListView.au3>
#include <GuiToolBar.au3>
#include <GuiMenu.au3>
#Include <GuiEdit.au3>
#Include <GuiListBox.au3>
#include <GuiMenu.au3>
#include <File.au3>
#include <Date.au3>
#include <IE.au3>
#include <GuiTab.au3>
#include <GuiButton.au3>


Global $oExcel,$FilePath,$AppExePath
Global $crashRes, $lConfig, $cConfig, $channelCount, $time

; $BusMasterExeFPath has to be updated everytime before running the script on a new version.
$BusMasterExeFPath=@ProgramFilesDir&"\BUSMASTER_v1.9.0"

$FilePath = @ScriptDir&"\Result\TestRunResults.xlsx" ; This file should already exist
if FileExists($FilePath) Then
	Global $oExcel = _ExcelBookOpen($FilePath) ; Open the Excel file
	If @error = 1 Then
		MsgBox(0, "Error!", "Unable to Create the Excel Object")
		Exit
	ElseIf @error = 2 Then
		MsgBox(0, "Error!", "File does not exist")
		Exit
	EndIf
	_ExcelSheetActivate($oExcel, "1.9.0")
EndIf

; If output folder exists in the script directory then delete it
$CurrentDirPath = @ScriptDir
if fileexists($CurrentDirPath & "\" & "Output")=1 Then
	DirRemove($CurrentDirPath & "\" & "Output",1)
EndIf

;~ ; Copy below Node simulation files from installed Busmaster folder to the script directory to avoid build,load errors
FileCopy ( $BusMasterExeFPath & "\GCCDLLMakeTemplate_CAN", @ScriptDir, 1)
DirCreate( @ScriptDir & "\SimulatedSystems")
DirCopy ( $BusMasterExeFPath & "\SimulatedSystems", @ScriptDir& "\SimulatedSystems", 1)
FileCopy ( $BusMasterExeFPath & "\GCCDLLMakeTemplate_J1939", @ScriptDir, 1)


$oIE = _IECreate()											; Create an Internet Explorer Browser Window
consolewrite("object " & $oIE & @CRLF)
_IEAction($oIE, "invisible")								; Sets an object state to hidden

; Launch the application by calling _launchApp() function from "globalFunc.au3" library file
_launchApp()

; Run all the module parent scripts
 #include <Database Editor_CAN\DBEditor.au3>
 #include <Log_Filter_Replay\Log_Filter_Replay.au3>
 #include <TxWindow\TxWindow.au3>
 #include <NodeSimulation_CAN\NodeSimulationCAN.au3>
 #include <Database Editor_J1939\DBEditor_J1939.au3>
 #include <Log_J1939\Log_J1939.au3>
 #include <Network_Statistics_CAN\Network_Statistics_CAN.au3>
 #include <Message_Window_CAN\Message_Window_CAN.au3>
 #include <Signal_Watch_CAN\Signal_Watch_CAN.au3>
 #include <NodeSimulation_J1939\NodeSimulationJ1939.au3>
 #include <J1939_Functionality\J1939Functionality.au3>
 #include <Message_Window_J1939\Message_WindowJ1939.au3>
 #include <UserConfiguration\UserConfiguration.au3>
_IEAction($oIE, "visible")									; Sets an object state to visible
_IEDocWriteHTML($oIE, $sHTML)								; Replaces the HTML for the entire document.

 consolewrite("object aft " & $oIE & @CRLF)

 ; Save the HTML File
_SaveHTMLReport()

