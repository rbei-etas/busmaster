;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_J1939_DBE_24
; TestCases			:		"Validation of signal scale factor"
; Test Data			:
; Test Setup		:		"1. Select a message in tree view
;~ 							2. Right Click on the signal list box
;~ 							3. Select  "New Signal" from the pop-up menu.
;~ 							4. Key in value 10 in signal scale factor field and Select OK button"

; Expected Results  : 		"Should accept the accept the value successfully."

; Critical (C)		:		Y
; TestCase No.		:		TS_J1939_DBE_25
; TestCases			:		"Signal length edit box and spin control Limit"
; Test Data			:
; Test Setup		:		"1. Open Database editor
;~ 							2. Add message with length 8bytes.
;~ 							3. Select "Add Signal" button or menu.
;~ 							4. Add signal name and select the type as "unsigned int".
;~ 							5. select signal length as 64bit by entering in edit box or using signal control."

; Expected Results  : 		"The edit box should accept the signal length as 64bit."

; Critical (C)		:		Y
; TestCase No.		:		TS_J1939_DBE_26
; TestCases			:		"Add >31 bit Signal"
; Test Data			:
; Test Setup		:		1. Open Database editor
;~ 							2. Add message with length 8bytes.
;~ 							3. Select "Add Signal" button or menu.
;~ 							4. Add signal name and select the type as "unsigned int".
;~ 							5. select signal length as 64bit by entering in edit box or using signal control.
;~ 							6. Select OK"
;~ 							7.Repeat the test case above for signed int

; Expected Results  : 		"The signal "Test64bit" will be added with maximum and minimum default value as 7FFFFFFFFFFFFFFF and 0."
;~ 							After step7,"The signal "Test64bit" will be added with maximum and minimum default value as 7FFFFFFFFFFFFF and -80000000 respectively."

; Critical (C)		:		Y
; TestCase No.		:		TS_J1939_DBE_28
; TestCases			:		Addition of message signals
; Test Data			:
; Test Setup		:		1. Open existing database.
;~ 							2. Select 'Msg1' in left tree view so that its details are seen in 'Message and Signal information' group box in right pane.
;~ 							3. Click on 'New Signal...' button in 'Signal details' groupbox.
;~ 							4. Enter 'Name:' as 'Signal1', select 'Type' as 'int'.
;~ 							5. Set 'Length' as '100' and try to get the focus out of 'Length' edit box.

; Expected Results  : 		1. After step3, 'Signal Details' dialog will appear.
;~ 							2. After step5, a message box 'Please enter an interger between 1 and 64' will be displayed.
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_J1939_DBE_24.au3****"&@CRLF)

_launchApp()

WinActivate($WIN_BUSMASTER,3)

Local $InvalidSigLen="Doesn't Exists",$sigName="",$sigLen=0,$sigFactor=0
If winexists($WIN_BUSMASTER) then

	_CreateConfig("cfxJ1939DB_24.cfx")																	; Create Configuration

	_openJ1939DB("testj1939DBE_24.DBF")																		; Open the DB

	ControlClick($WIN_DBEditor_J1939,"",$BTN_NewSignal_DBEditor)											; Click on 'New Signal'

	if WinWaitActive($WIN_Signal_Details,"",3) Then
		ControlCommand($WIN_Signal_Details, "", $TXT_SigType_SigDetails, "SelectString", "unsigned int") 		; Select signal type from the combobox
		sleep(500)
		ControlSetText($WIN_Signal_Details, "", $TXT_SigName_SigDetails, "Sig1") 						; Add signal name
		sleep(500)
		ControlSetText($WIN_Signal_Details, "", $TXT_SigLen_SigDetails, 100) 							; enter signal length
		sleep(500)
		send("{TAB}")
		If WinWaitActive("",$TXT_InvSignalLen_BM,3) Then
			$InvalidSigLen="Exists"
			ControlClick("",$TXT_InvSignalLen_BM,$BTN_InvSignalLen_BM)									; Click on 'OK' button
		EndIf
		sleep(500)
		ControlSetText($WIN_Signal_Details, "", $TXT_SigLen_SigDetails, 64) 							; enter signal length
		sleep(500)
		ControlSetText($WIN_Signal_Details, "", $EDIT_Factor_SigDetails, 10) 							; enter scale factor
		sleep(500)
		ControlClick($WIN_Signal_Details, "", $BTN_OK_SigDetails) 										; Click 'OK' button
		sleep(250)
	EndIf

	$sigDetlvhwd=controlgethandle($WIN_BUSMASTER,"",$LVC_SigDet_DBeditor)					; Get handle of signal details list view
	$sigName=_GUICtrlListView_GetItemText($sigDetlvhwd,0,0)									; Fetch the signal name
	$sigLen=_GUICtrlListView_GetItemText($sigDetlvhwd,0,3)									; Fetch the signal length
	$sigFactor=_GUICtrlListView_GetItemText($sigDetlvhwd,0,8)								; Fetch the signal scale factor
	$sigMaxVal=_GUICtrlListView_GetItemText($sigDetlvhwd,0,5)								; Fetch the signal Max value
	$sigMinVal=_GUICtrlListView_GetItemText($sigDetlvhwd,0,6)								; Fetch the signal Min value

EndIf

ConsoleWrite("$InvalidSigLen : "&$InvalidSigLen&@CRLF)
consolewrite("$sigName="&$sigName&@CRLF)
consolewrite("$sigLen="&$sigLen&@CRLF)
consolewrite("$sigFactor="&$sigFactor&@CRLF)
consolewrite("$sigMaxVal="&$sigMaxVal&@CRLF)
consolewrite("$sigMinVal="&$sigMinVal&@CRLF)

if $InvalidSigLen="Exists" and $sigName="Sig1" and $sigLen=64 and $sigMaxVal="FFFFFFFFFFFFFFFF" and $sigMinVal=0 and $sigFactor=10.000000 Then
	_WriteResult("Pass","TS_J1939_DBE_24")
Else
	_WriteResult("Fail","TS_J1939_DBE_24")
EndIf


$isAppNotRes=_CloseApp()																				; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_J1939_DBE_24")
EndIf

ConsoleWrite("****End : TS_J1939_DBE_24.au3****"&@CRLF)
ConsoleWrite(@CRLF)