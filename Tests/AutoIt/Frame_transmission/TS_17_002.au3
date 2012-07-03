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
EndIf
if WinExists("BUSMASTER","Configure Transmission Messages") then
$clickclose=ControlClick("BUSMASTER","Configure Transmission Messages", 1015) 	;1015 is the ID for 'Close' button in 'Configure Transmission Messages' Dialog.
EndIf
