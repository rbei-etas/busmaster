;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_DBE_11
; TestCases			:		"Delete Signal"
; Test Data			:		
; Test Setup		:		"1. Open Database editor
;~ 							2. Select message "Test".
;~ 							3. Select signal ""Delete Signal" button or menu.
;~ 							4. Select OK to confirm deletion"

; Expected Results  : 		"The signal will be deleted from the list"
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_DBE_11.au3****"&@CRLF)

_launchApp()

WinActivate($WIN_BUSMASTER,3)
if winexists($WIN_BUSMASTER) then
	_openCANDB("testDBE_11")														    ; Open DBF file
	$sigDetlvhwd=controlgethandle($WIN_BUSMASTER,"",$LVC_SigDet_DBeditor)				; Get handle of signal details list view
	_GUICtrlListView_ClickItem($sigDetlvhwd,0)											; Click on the first item in the Signal details list view
	controlclick($WIN_BUSMASTER,"",$BTN_DeleteSig_DBEditor)								; Click Yes button
	sleep(1000)
	if winexists($DLG_Busmaster,$sigDelmsgtxt) Then
		ControlClick($DLG_Busmaster,"",$BTN_Yes_BM)										; Click on 'Yes' button
		consolewrite("Win Exists"&@Crlf)
	EndIf
	$signalCount=_GUICtrlListView_GetItemCount($sigDetlvhwd)							; Fetch the signal count
	consolewrite("$signalCount : " & $signalCount & @Crlf)
EndIf
if $signalCount=0 Then
	_WriteResult("Pass","TS_DBE_11")
Else
	_WriteResult("Fail","TS_DBE_11")
EndIf


$isAppNotRes=_CloseApp()																; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_DBE_11")
EndIf

ConsoleWrite("****End : TS_DBE_11.au3****"&@CRLF)
ConsoleWrite(@CRLF)