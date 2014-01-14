;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_J1939_DBE_09
; TestCases			:		Database Usage in Busmaster
; Test Data			:		
; Test Setup		:		1. Continue from above test case TS_J1939_DBE_08.
;~ 							2. Select the 'NewDBFile.dbf' file and click on 'Open' button in the dialog.
;~ 							3. Select menu option 'J1939 -> Database -> Disassociate…' to open the 'Dissociate Database' dialog.

; Expected Results  : 		1. The 'Dissociate Database' dialog should contain the 'NewDBFile.dbf' file in list which indicates that it is currently associated database.
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_J1939_DBE_09.au3****"&@CRLF)

_launchApp()

WinActivate($WIN_BUSMASTER,3)
if winexists($WIN_BUSMASTER) then
	_loadConfig("cfxJ1939DBE_09")														; Load config file
	_DissociateJ1939DBMenu()															; Dissociate DB
	WinWaitActive($WIN_Dissociate_CANDB,"",3)
	$disDBlbHWD=ControlGetHandle($WIN_Dissociate_CANDB,"",$LSTB_DBFile_Dis_CANDB)		; Fetch the handle of dissociate Db list box
	$assocDBFilePath=_GUICtrlListBox_GetText($disDBlbHWD,0)								; Fetch the Associated DB file path from list box
	ConsoleWrite("$assocDBFilePath :" & $assocDBFilePath & @CRLF)
	$assocDBFName=StringRight($assocDBFilePath,19)										; Fetch the dbf file name
	ConsoleWrite("$assocDBFName :" & $assocDBFName & @CRLF)
	ControlClick($WIN_Dissociate_CANDB,"",$BTN_Cancel_Dis_CANDB)						; Click on 'Cancel' button in Dissociate database window
EndIf
if $assocDBFName="testJ1939DBE_06.dbf" Then
	_WriteResult("Pass","TS_J1939_DBE_09")
Else
	_WriteResult("Fail","TS_J1939_DBE_09")
EndIf


$isAppNotRes=_CloseApp()																; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_J1939_DBE_09")
EndIf

ConsoleWrite("****End : TS_J1939_DBE_09.au3****"&@CRLF)
ConsoleWrite(@CRLF)
