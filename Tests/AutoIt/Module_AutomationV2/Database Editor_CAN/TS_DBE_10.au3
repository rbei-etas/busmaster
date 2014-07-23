;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_DBE_10
; TestCases			:		Delete signal descriptions
; Test Data			:		
; Test Setup		:		1. Select "File->Database->Open..."
;~ 							2. Select "DBForSTS.dbf" database from the Open file dialog box
;~ 							3. Select "MESSAGE1" from the left view
;~ 							4. Select signal "Signal_4"
;~ 							5. Select "TRUE" signal description
;~ 							6. Select "Delete Desc..." button
;~ 							7. Confirm deletion by selecting "Yes""

; Expected Results  : 		1.After step3,Should disable "Delete Desc..." button
;~ 							2.After step4,Should disable "Delete Desc..." button
;~ 							3.After step6,Should display a delete confirmation
;~ 							4.After step7,the selected signal description should be deleted after confirmation"
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_DBE_10.au3****"&@CRLF)

_launchApp()
WinActivate($WIN_BUSMASTER,3)
if winexists($WIN_BUSMASTER) then
	_openCANDB("testDBE_10")																			; Open DBF file
	$delDescBTNStatus1=ControlCommand($WIN_BUSMASTER,"",$BTN_DelSigDesc_DBEditor,"IsEnabled")			; Fetch the status of 'New Desc' button
	ConsoleWrite("$delDescBTNStatus1 :" & $delDescBTNStatus1 &@CRLF)
	$sigDetlvhwd=controlgethandle($WIN_BUSMASTER,"",$LVC_SigDet_DBeditor)								; Get handle of signal details list view
	_GUICtrlListView_ClickItem($sigDetlvhwd,0)															; Click on the first item in the Signal details list view
	$delDescBTNStatus2=ControlCommand($WIN_BUSMASTER,"",$BTN_DelSigDesc_DBEditor,"IsEnabled")			; Fetch the status of 'New Desc' button
	ConsoleWrite("$newDescBTNStatus2 :" & $delDescBTNStatus2 &@CRLF)
	sleep(500)
	$DelSignalDescRes=_DelSignalDesc()																	; Delete signal desc
	$sigDesclvhwd=controlgethandle($WIN_BUSMASTER,"",$LVC_SigDesc_SigDesc)								; Get handle of signal details list view
	$signalDescCount=_GUICtrlListView_GetItemCount($sigDesclvhwd)										; Fetch the count of signal desc items
	consolewrite("$signalDescCount : " & $signalDescCount & @Crlf)
EndIf
if $DelSignalDescRes=1 and $signalDescCount=0 Then
	_WriteResult("Pass","TS_DBE_10")
Else
	_WriteResult("Fail","TS_DBE_10")
EndIf


$isAppNotRes=_CloseApp()																				; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_DBE_10")
EndIf

ConsoleWrite("****End : TS_DBE_10.au3****"&@CRLF)
ConsoleWrite(@CRLF)