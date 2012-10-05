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
WinActivate("BUSMASTER",3)
WinWait("BUSMASTER","",2)
if WinActive("MCNet") Then
	ControlClick("MCNet","","[CLASS:Button; INSTANCE:1]")
EndIf
if WinWaitActive("BUSMASTER","",5) then
Send("!ct")														; Configure -> Tx Messages
sleep(2000)
EndIf
