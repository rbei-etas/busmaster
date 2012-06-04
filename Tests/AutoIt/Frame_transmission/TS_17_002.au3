;Test Case Number:	TS_17_002
;Feature:			Frame Transmission
; Critical (C):		Y
; Test Cases:		Closing Tx Window
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===

;#include<globalFunc.au3>											; include global function script.
;#include <IsAppCrash.au3>											; call 'IsAppCrash' function to check for application crash
;#Include <Excel.au3>
global $crashRes, $oExcel
_launchApp()														; check whether the app exists or else launch it.
if $app=0 Then
	Send("!ct")
	sleep(2000)
	if $crashRes=1 then												; If application crash is found then update the result sheet by calling _writeCrashRes function.
	_writeCrashRes(7,10)
	EndIf
EndIf
if WinExists("BUSMASTER","Configure Transmission Messages") then
$clickclose=ControlClick("BUSMASTER","Configure Transmission Messages", 1015) 	;1015 is the ID for 'Close' button in 'Configure Transmission Messages' Dialog.
if $clickclose=0 and ControlCommand("BUSMASTER", "Configure Transmission Messages","", "IsVisible") = 1 Then
	_ExcelWriteCell($oExcel, "Error", 7, 10)
Else
	_ExcelWriteCell($oExcel, "OK", 7, 10)
EndIf
Else
	_ExcelWriteCell($oExcel, "Error", 7, 10)
EndIf
