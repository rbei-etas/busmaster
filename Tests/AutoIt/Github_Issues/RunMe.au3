;===========Main Script==============
; Test Cases		: Github Defects Test
; Test Strategy		: Black Box
; Critical 			: Y
;===========Main Script==============

#include <IsAppCrash.au3>
#Include <Excel.au3>
#include <Functions.au3>
#Include <GuiListView.au3>
#Include <GuiMenu.au3>
#Include <GuiEdit.au3>
#Include <GuiListBox.au3>
#Include <GuiComboBox.au3>

Global $oExcel,$sFilePath1, $sigDescCount1, $sigDescCount, $sigDetCount, $sigDetCount1, $msgCount, $a, $msgCount1,$kvsrHdw
;Open the test specification file.
$sFilePath1 = "D:\Projects\BusMaster\AutoIt\Github_Issues\Github_Issues.xls" ;This file should already exist
$oExcel = _ExcelBookOpen($sFilePath1)
If @error = 1 Then
	MsgBox(0, "Error!", "Unable to Create the Excel Object")
    Exit
Elseif @error = 2 Then
    MsgBox(0, "Error!", "File does not exist")
    Exit
EndIf
_ExcelSheetActivate($oExcel, "Sheet1")

#include "Issue_210.au3"
#include "Issue_93.au3"
#include "Issue_185.au3"
#include "Issue_186.au3"
#include "Issue_180.au3"
#include "Issue_92.au3"
#include "Issue_181.au3"
#include "Issue_79.au3"
#include "Issue_204.au3"
#include "Issue_19.au3"
#include "Issue_63.au3"
#include "Issue_78.au3"
#include "Issue_18.au3"
#include "Issue_149.au3"
#include "Issue_22.au3"
#include "Issue_96.au3"