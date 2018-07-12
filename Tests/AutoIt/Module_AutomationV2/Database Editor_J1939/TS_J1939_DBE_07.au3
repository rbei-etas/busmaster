;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_J1939_DBE_07
; TestCases			:		Database Usage in Busmaster
; Test Data			:
; Test Setup		:		1. Associate a Database
;~ 							2. Select menu option 'J1939 -> Database -> Disassociate…'.
;~ 							3. Select database file in list and click on 'Dissociate'.

; Expected Results  : 		1. After step2, A dialog 'Dissociate Database' will be displayed.
;~ 								After step3 a message box 'Dissociation will clear signal watch list.
;~ 								Do you want to clear the list?' will be displayed.
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_J1939_DBE_07.au3****"&@CRLF)

_launchApp()

WinActivate($WIN_BUSMASTER,3)
if winexists($WIN_BUSMASTER) then
	_AssociateJ1939DB("testJ1939DBE_06")						; Associate DB
	_DissociateJ1939DB(0)										; Dissociate DB
	$Res=0
	Opt("WinTextMatchMode", 1)
	if winexists("",$disDBtxt1) Then
		$Res=1
		consolewrite("Dissociation will clear Signal Watch List window exists" & @CRLF)
		WinActivate("",$disDBtxt)
		send("{ENTER}")					  									; Click on 'yes' button in "Dissociation will clear Signal Watch List." warning message
	EndIf
	$disDBlbHWD=ControlGetHandle($WIN_Dissociate_CANDB,"",$LSTB_DBFile_Dis_CANDB)			; Fetch the handle of dissociate Db list box
	$assocDBCount=_GUICtrlListBox_GetCount($disDBlbHWD)						; Fetch the count of items in dissociate DB window
	ConsoleWrite("$assocDBCount : " & $assocDBCount &@CRLF)
	ControlClick($WIN_Dissociate_CANDB,"",$BTN_Cancel_Dis_CANDB)						; Click on 'Cancel' button in Dissociate database window
	ConsoleWrite("$Res : " & $Res &@CRLF)

EndIf
;~ WinWaitActive($WIN_BUSMASTER,$disDBtxt,4)
if $Res=1 Then
	_WriteResult("Pass","TS_J1939_DBE_07")
Else
	_WriteResult("Fail","TS_J1939_DBE_07")
EndIf


$isAppNotRes=_CloseApp()										; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_J1939_DBE_07")
EndIf

ConsoleWrite("****End : TS_J1939_DBE_07.au3****"&@CRLF)
ConsoleWrite(@CRLF)