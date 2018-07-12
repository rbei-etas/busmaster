;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_DBE_07
; TestCases			:		Database Usage in Busmaster
; Test Data			:
; Test Setup		:		1. Associate a Database
;~ 							2. Select menu option 'File -> Database -> Disassociate…'.
;~ 							3. Select database file in list and click on 'Dissociate'.

; Expected Results  : 		1. After step2, A dialog 'Dissociate Database' will be displayed.
;~ 								After step3 a message box 'Dissociation will clear signal watch list.
;~ 								Do you want to clear the list?' will be displayed.
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_DBE_07.au3****"&@CRLF)

_launchApp()

WinActivate($WIN_BUSMASTER,3)
if winexists($WIN_BUSMASTER) then

	_AssociateCANDB("testDBE_06")									; Associate DB
	$Res=_DissociateCANDB(0)										; Dissociate DB
	ControlClick($WIN_Dissociate_CANDB,"",$BTN_Cancel_Dis_CANDB)	; Click on 'Cancel' button in Dissociate database window
	ConsoleWrite("$Res : " & $Res &@CRLF)

EndIf

if $Res=1 Then
	_WriteResult("Pass","TS_DBE_07")
Else
	_WriteResult("Fail","TS_DBE_07")
EndIf


$isAppNotRes=_CloseApp()											; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_DBE_07")
EndIf

ConsoleWrite("****End : TS_DBE_07.au3****"&@CRLF)
ConsoleWrite(@CRLF)