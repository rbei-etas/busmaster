;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_J1939_DBE_38
; TestCases			:		"Validation of signal descriptor value"
; Test Data			:
; Test Setup		:		"1. Select the signal which has descriptor defined.
;~ 							2. Select "Add Desc" from displayed pop-up menu.
;~ 							3. Key in values specified in the test data"

; Expected Results  : 		"Should Warn the user for duplicate entry and should focus to the signal value edit control"

; Critical (C)		:		Y
; TestCase No.		:		TS_J1939_DBE_39
; TestCases			:		"Validation of signal descriptor and value"
; Test Data			:
; Test Setup		:		"1. Select a message in tree view.
;~ 							2.Chose signal type "Boolean"
;~ 							3. Right Click on the Signal Descriptor list select New and key in valid Descriptor and value
;~ 							4.Key in values as shown in test data
;~ 							5.Repeat the same test case with other specified values"

; Expected Results  : 		"Should warn the user for invalid signal value."

; Critical (C)		:		Y
; TestCase No.		:		TS_J1939_DBE_42
; TestCases			:		Signal Value Descriptors options
; Test Data			:
; Test Setup		:		Right click on the signal descriptor 'SigDesc' in 'Signal Description' list.

; Expected Results  : 		1. After step2, a popup message with following menu items should be displayed: 'Edit Desc…' and 'Delete Desc'.

;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_J1939_DBE_38.au3****"&@CRLF)

_launchApp()

WinActivate($WIN_BUSMASTER,3)

Local $DupSigDesc=0,$InvalidSigVal=0,$PopUpMenu=0

If winexists($WIN_BUSMASTER) then

	_CreateConfig("cfxj1939DB_38.cfx")																	; Create Configuration

	_openJ1939DB("testj1939DBE_38.DBF")																		; Open the DB

	;------Maximize Child Window DatabaseEditor - J1939---------------
	_Maximize_childWindow("DatabaseEditor - J1939")
	sleep(1000)

	$sigDetlvhwd=controlgethandle($WIN_BUSMASTER,"",$LVC_SigDet_DBeditor)								; Get handle of signal details list view
	_GUICtrlListView_ClickItem($sigDetlvhwd,0)
	sleep(500)

	ControlClick($WIN_DBEditor_J1939,"",$BTN_SigDes_DBEditor)												; Click on 'New Desc'

	_addSigDesc("ON",1)																					; Add signal description with same values

	if WinWaitActive("",$TXT_DupSigDesc_BM,3) Then														; if duplicate signal desc warning mesage is active then
		$DupSigDesc=1
		ControlClick($WIN_BUSMASTER,$TXT_DupSigDesc_BM,$BTN_OKDupSigDesc_BM)										; Click on ok button
	EndIf

	ControlSetText($WIN_Signal_Desc, "", $TXT_SigDescName_SigDesc, "Test") 								; Enter signal desc name

	ControlSetText($WIN_Signal_Desc, "", $TXT_SigDescVal_SigDesc, -1) 									; Enter -1 as signal desc value

	ControlClick($WIN_Signal_Desc, "", $BTN_OK_SigDesc) 												; Click 'OK'

	if WinWaitActive("",$TXT_InvSigValRange_BM,3) Then													; if invalid signal value window is active then
		$InvalidSigVal=1
		ControlClick($WIN_BUSMASTER,$TXT_InvSigValRange_BM,$BTN_OKInvSigValRange_BM)					; Click on Ok button
	EndIf

	send("{ESC}")																						; Close the value descriptor and value dialog
	sleep(500)

	$sigDesclvhwd=controlgethandle($WIN_BUSMASTER,"",$LVC_SigDesc_SigDesc)								; Get handle of signal description list view
	_GUICtrlListView_ClickItem($sigDesclvhwd,0,"Right")													; right click on the first signal descriptor
	sleep(500)

	$GetPopUp_Text = _GetPopUpMenuText()

	If $GetPopUp_Text[0]=2 Then
		if $GetPopUp_Text[1]="&Edit Desc..." and $GetPopUp_Text[2]="&Delete Desc" Then
			$PopUpMenu=1
		EndIf
	EndIf
	send("{ESC}")

	ConsoleWrite($GetPopUp_Text[0]&@Crlf)
	ConsoleWrite($GetPopUp_Text[1]&@Crlf)
	ConsoleWrite($GetPopUp_Text[2]&@Crlf)

EndIf

ConsoleWrite("$DupSigDesc: "&$DupSigDesc&@CRLF)
ConsoleWrite("$InvalidSigVal: "&$InvalidSigVal&@CRLF)
ConsoleWrite("$PopUpMenu: "&$PopUpMenu&@CRLF)

if  $DupSigDesc=1 and $InvalidSigVal=1 and $PopUpMenu=1 Then
	_WriteResult("Pass","TS_J1939_DBE_38")
Else
	_WriteResult("Fail","TS_J1939_DBE_38")
EndIf

$isAppNotRes=_CloseApp()																				; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_J1939_DBE_38")
EndIf

ConsoleWrite("****End : TS_J1939_DBE_38.au3****"&@CRLF)
ConsoleWrite(@CRLF)