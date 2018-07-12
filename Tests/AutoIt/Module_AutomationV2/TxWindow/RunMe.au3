;===========Main Script==============
; Test Cases		: Testcase 1 -Testcase 17
; Test Strategy		: Black Box
; Critical 			: Y
;===========Main Script==============

#Include <Excel.au3>
#Include <GuiStatusBar.au3>
#include <globalFunc_SmokeTest.au3>
#Include <GuiComboBoxEx.au3>
#Include <GuiListView.au3>
#Include <GuiToolBar.au3>
#Include <GuiMenu.au3>
;~ #include <Data_Declaration.au3>

Global $oExcel
Global $crashRes, $lConfig, $cConfig,$channelCount,$time,$mWin


;~ $FilePath = "D:\Module Automation\Result\TestRunResults.xls" ; This file should already exist
;~ Global $oExcel = _ExcelBookOpen($FilePath) ; Open the Excel file


;~ #include "D:\Module Automation\TxWindow\Txwindow.au3"
#include "D:\Module Automation\Node simulation\NodeSimulation.au3"
