;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_DBE_14
; TestCases			:		New Database File
; Test Data			:
; Test Setup		:		1. Select menu option 'File -> Database -> New'.
;~ 							2.Enter "NewDBFile" as the name of DBF file to be created and click on 'Save' button.

; Expected Results  : 		1. A 'Save As' dialog should be opened whixh allows the user to enter the name of the new database file.
;~ 							2. After step2, the 'Database Editor - CAN' dialog should be displayed with 'NewDBFile.DBF' as the root item in left pane tree control.
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_DBE_14.au3****"&@CRLF)

_launchApp()

Local $DefDBName=""
if winexists($WIN_BUSMASTER) then

	_CreateConfig("cfxCANDB_14.cfx")																; Create Configuration

	WinMenuSelectItem($WIN_BUSMASTER,"",$CANMenu,$databaseMenu,$newMenu)							; Create a new DB
	WinWaitActive($WIN_NewDBFile,"",3)

	if winexists($WIN_NewDBFile) Then
		$DefDBName=ControlGetText($WIN_NewDBFile,"",$TXT_FileName_NewDB)							; Fetch the Default DB name
		ControlClick($WIN_NewDBFile,"",$BTN_Cancel_NewDB)							 				; Click on Cancel button
	EndIf

EndIf
ConsoleWrite("$DefDBName :"&$DefDBName&@CRLF)

If $DefDBName="New_Database1.DBF" Then
	_WriteResult("Pass","TS_DBE_14")
Else
	_WriteResult("Fail","TS_DBE_14")
EndIf


$isAppNotRes=_CloseApp()																				; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_DBE_14")
EndIf

ConsoleWrite("****End : TS_DBE_14.au3****"&@CRLF)
ConsoleWrite(@CRLF)