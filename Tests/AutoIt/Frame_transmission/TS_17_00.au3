;=== Test Cases/Test Data ===
; Feature:			Frame Transmission
; Critical (C):		Y
; Test Cases:		Configure->Tx Messages
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
#Include <Excel.au3>
#include<globalFunc.au3>
Global $oExcel
Global $crashRes
;Open the test specification file.
$sFilePath1 = "D:\Projects\BusMaster\AutoIt\Test_Run\STS_BUSMASTER_Tx_Window.xls" ;This file should already exist
Global $oExcel = _ExcelBookOpen($sFilePath1)
If @error = 1 Then
    MsgBox(0, "Error!", "Unable to Create the Excel Object")
    Exit
ElseIf @error = 2 Then
    MsgBox(0, "Error!", "File does not exist")
    Exit
EndIf
_ExcelSheetActivate($oExcel, "Test Spec-Report")

#include "IsAppCrash.au3"
#include "TS_17_001.au3"
#include "TS_17_002.au3"
#include "TS_17_003.au3"
#include "TS_17_004.au3"
#include "TS_17_005.au3"
#include "TS_17_006.au3"
#include "TS_17_007.au3"
#include "TS_17_008.au3"
#include "TS_17_009.au3"
#include "TS_17_010.au3"
#include "TS_17_011.au3"
#include "TS_17_012.au3"
#include "TS_17_013.au3"
#include "TS_17_014.au3"
#include "TS_17_015.au3"
#include "TS_17_016.au3"
#include "TS_17_018.au3"
#include "TS_17_020.au3"
#include "TS_17_021.au3"
#include "TS_17_022.au3"
#include "TS_17_023.au3"
#include "TS_17_025.au3"
#include "TS_17_026.au3"
#include "TS_17_029.au3"
#include "TS_17_030.au3"
