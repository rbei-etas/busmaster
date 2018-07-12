;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_J1939_DBE_02
; TestCases			:		"Add a signal to message in database"
; Test Data			:		-
; Test Setup		:		1. Select a message in tree view.
;~ 							2. Right Click on the signal list box
;~ 							3. Select  "New Signal" from the pop-up menu.
;~ 							4. Key in New Valid Signal details
;~ 							5. Select OK button

; Expected Results  : 		Should update the signal list with entered signal name
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_J1939_DBE_02.au3****"&@CRLF)

_launchApp()																				; Check if the App is open else launch it

if winexists($WIN_BUSMASTER) then
	_openJ1939DB("testJ1939DBE_02")
	$sigDetlvPos=ControlGetPos($WIN_BUSMASTER,"",$LVC_SigDet_DBeditor)						; Get handle of signal details list view
	MouseClick("right",$sigDetlvPos[0]+150,$sigDetlvPos[1]+100)								; Right Click on the Signal details list view
	send("{DOWN}")																			; Select 'New Signal' menu
	sleep(1500)
	send("{ENTER}")
	_addSigDetails("int","Signal1",32,0,0)													; Add the signal details
	$sigDetlvhwd=controlgethandle($WIN_BUSMASTER,"",$LVC_SigDet_DBeditor)					; Get handle of signal details list view
	$sigName=_GUICtrlListView_GetItemText($sigDetlvhwd,0,0)									; Fetch the signal name
	$sigByteInd=_GUICtrlListView_GetItemText($sigDetlvhwd,0,1)								; Fetch the Byte Index
	$sigBitNo=_GUICtrlListView_GetItemText($sigDetlvhwd,0,2)								; Fetch the Bit No.
	$sigLen=_GUICtrlListView_GetItemText($sigDetlvhwd,0,3)									; Fetch the signal length
	$sigType=_GUICtrlListView_GetItemText($sigDetlvhwd,0,4)									; Fetch the signal type
	$sigMaxVal=_GUICtrlListView_GetItemText($sigDetlvhwd,0,5)								; Fetch the signal Max value
	$sigMinVal=_GUICtrlListView_GetItemText($sigDetlvhwd,0,6)								; Fetch the signal Min value

	_ActivatechildWindow($WIN_DBEditor_J19391)
	WinClose($WIN_DBEditor_J19391)
	if WinWaitActive($WIN_BUSMASTER,$saveDBtxt,2) Then
		ControlClick($WIN_BUSMASTER,"",$BTN_No_SaveDB)
	EndIf

	consolewrite("$sigName="&$sigName&@CRLF)
	consolewrite("$sigLen="&$sigLen&@CRLF)
	consolewrite("$sigType="&$sigType&@CRLF)
	consolewrite("$sigByteInd="&$sigByteInd&@CRLF)
	consolewrite("$sigBitNo="&$sigBitNo&@CRLF)
	consolewrite("$sigMaxVal="&$sigMaxVal&@CRLF)
	consolewrite("$sigMinVal="&$sigMinVal&@CRLF)
EndIf
if $sigName="Signal1" and $sigLen=32 and $sigType="int" and $sigByteInd=0 and $sigBitNo=0 and $sigMaxVal="7FFFFFFF" and  $sigMinVal=80000000 Then
	_WriteResult("Pass","TS_J1939_DBE_02")
Else
	_WriteResult("Fail","TS_J1939_DBE_02")
EndIf

$isAppNotRes=_CloseApp()																		; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_J1939_DBE_02")
EndIf



ConsoleWrite("****End : TS_J1939_DBE_02.au3****"&@CRLF)
ConsoleWrite(@CRLF)