;===========Main Script==============
; Framework			: Modular Driven Framework
; Test Strategy		: Black Box
; Critical 			: Y
;===========Main Script==============

; Include all the necessary Library .au3 files,Global Function, Variable Declaration and AppCrash check

#include <Data_DeclarationV2.au3>
#include <IsAppCrash.au3>
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



Global $oExcel
Global $crashRes, $lConfig, $cConfig, $channelCount, $time

$FilePath = @ScriptDir&"\Result\TestRunResults.xls" ; This file should already exist
Global $oExcel = _ExcelBookOpen($FilePath) ; Open the Excel file
If @error = 1 Then
	MsgBox(0, "Error!", "Unable to Create the Excel Object")
	Exit
ElseIf @error = 2 Then
	MsgBox(0, "Error!", "File does not exist")
	Exit
EndIf


; Launch the application by calling _launchApp() function from "globalFunc.au3" library file
_launchApp()

#include "D:\Module Automation\Database Editor\DBEditor.au3"
#include "D:\Module Automation\Log_Filter_Replay\Log_Filter_Replay.au3"
