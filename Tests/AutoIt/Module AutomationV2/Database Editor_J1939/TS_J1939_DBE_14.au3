;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_J1939_DBE_14
; TestCases			:		New Database File
; Test Data			:
; Test Setup		:		1. Select menu option '"J1939 -> Database -> New'.
;~ 							2. Enter "NewDBFile" as the name of DBF file to be created and click on 'Save' button.

; Expected Results  : 		1. A 'Save As' dialog should be opened whixh allows the user to enter the name of the new database file.
;~ 							2. After step2, the 'Database Editor - J1939' dialog should be displayed with 'NewDBFile.DBF' as the root item in left pane tree control.
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_J1939_DBE_14.au3****"&@CRLF)

_launchApp()
WinActivate($WIN_BUSMASTER,3)

Local $DefDBName=""
if winexists($WIN_BUSMASTER) then

	_CreateConfig("cfxJ1939DB_14.cfx")																; Create Configuration

	WinMenuSelectItem($WIN_BUSMASTER,"",$J1939Menu,$databaseMenu,$newMenu)							; Create a new DB
	WinWaitActive($WIN_J1939SaveAs,"",3)

	if winexists($WIN_J1939SaveAs) Then
		$DefDBName=ControlGetText($WIN_J1939SaveAs,"",$TXT_FileName_NewDB)								; Fetch the Default DB name
		ControlClick($WIN_J1939SaveAs,"",$BTN_Cancel_NewDB)							 				; Click on Cancel button
	EndIf

EndIf
ConsoleWrite("$DefDBName :"&$DefDBName&@CRLF)

If $DefDBName="New_DatabaseJ19391.DBF" Then
	_WriteResult("Pass","TS_J1939_DBE_14")
Else
	_WriteResult("Fail","TS_J1939_DBE_14")
EndIf


$isAppNotRes=_CloseApp()																				; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_J1939_DBE_14")
EndIf

ConsoleWrite("****End : TS_J1939_DBE_14.au3****"&@CRLF)
ConsoleWrite(@CRLF)