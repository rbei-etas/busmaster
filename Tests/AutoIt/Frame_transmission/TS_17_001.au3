;Test Case Number:	TS_17_001
;Feature:			Frame Transmission
; Critical (C):		Y
; Test Cases:		Configure->Tx Messages
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===

;#include<globalFunc.au3>										; include global function script.
;#include <IsAppCrash.au3>										; call 'IsAppCrash' function to check for application crash
;#Include <Excel.au3>
global $crashRes, $oExcel
Run("C:\Program Files\BUSMASTER\BUSMASTER.exe")					; invoke the application and run the Process
WinWait("BUSMASTER","",2)
if $crashRes=1 then
_writeCrashRes(6,10)											; If application crash is found then update the result sheet by calling _writeCrashRes function.
 EndIf
if WinActive("MCNet") Then
	ControlClick("MCNet","","[CLASS:Button; INSTANCE:1]")
EndIf
if WinWaitActive("BUSMASTER","",5) then
Send("!ct")														; Configure -> Tx Messages
sleep(2000)
if $crashRes=1 then												; If application crash is found then update the result sheet by calling _writeCrashRes function.
_writeCrashRes(6,10)
 EndIf
EndIf
if ControlCommand("BUSMASTER", "Configure Transmission Messages","", "IsVisible") = 1 Then		; Checks if the window is visible
	_ExcelWriteCell($oExcel, "OK", 6, 10)
Else
	_ExcelWriteCell($oExcel, "Error", 6, 10)
EndIf