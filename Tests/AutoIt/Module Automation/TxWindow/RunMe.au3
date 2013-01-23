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

$sFilePath1= "D:\Busmaster file\TxWindow\STS_BUSMASTER_TxWindow.xls"
  $oExcel = _ExcelBookOpen($sFilePath1,1)
 _ExcelSheetActivate($oExcel,"Beta1")
  WinActivate("Microsoft Excel - STS_BUSMASTER_TxWindow.xls")
  _ExcelSheetActivate($oExcel,4)

#include "D:\Module Automation\TxWindow\Txwindow.au3"