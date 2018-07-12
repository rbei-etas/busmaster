;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_DBE_30
; TestCases			:		Addition of message signals
; Test Data			:
; Test Setup		:		1. Continue from above test case TS_DBE_29.
;~ 							2. Click on 'New Signal...' button in 'Signal details' groupbox to get 'Signal Details' dialog.
;~ 							3. Enter 'Name:' as 'Signal2', select 'Type' as 'int'.
;~ 							4. Set 'Length' as '8' and try to get the focus out of 'Length' edit box.
;~ 							5. Change 'Byte Order' to 'Motorola'.
;~ 							6. Click 'OK' in 'Signal Details' dialog.

; Expected Results  : 		1. After step6, a message 'Duplicate signal start bit value' will be displayed. This is because 'signal 1' is configured with same 'Byte Index' and 'start bit'.

; Critical (C)		:		Y
; TestCase No.		:		TS_DBE_31
; TestCases			:		Addition of message signals
; Test Data			:
; Test Setup		:		1. Continue from above test case TS_DBE_30.
;~ 							2. Click 'OK' message box.
;~ 							3. Set 'Byte Index' to 2.
;~ 							4. Set 'Start bit' to 0.
;~ 							5. Click 'OK' in 'Signal Details' dialog.

; Expected Results  : 		1. After step5, the dialog gets closed and the 'Signal2' details gets populated in list control in 'Signal Details' groupbox with 'Byte order' as 'Motorola'.

; Critical (C)		:		Y
; TestCase No.		:		TS_DBE_32
; TestCases			:		Addition of message signals
; Test Data			:
; Test Setup		:		1. Continue from above test case TS_DBE_31.
;~ 							2. Click on 'New Signal...' button in 'Signal details' groupbox to get 'Signal Details' dialog.
;~ 							3. Enter 'Name:' as 'Signal3', select 'Type' as 'int'.
;~ 							4. Set signal 'Length' as '50'.
;~ 							5. Click 'OK' in 'Signal Details' dialog.
;~ 							6.Click 'OK' message box.
;~ 							7. Set signal 'length' to 48.
;~ 							8. Click 'OK' in 'Signal Details' dialog.

; Expected Results  : 		1. After step5, a message 'End bit of signal is out of message length' will be displayed. This is because 'Signal2' has only 6 bytes of message length(48 bits) which it can occupy.
;~ 							After step8, the dialog gets closed and the 'Signal3' details gets populated in list control in 'Signal Details' groupbox with 'Byte order' as 'Intel'.

; Critical (C)		:		Y
; TestCase No.		:		TS_DBE_33
; TestCases			:		Manipulation of message signals
; Test Data			:
; Test Setup		:		1. Continue from above test case TS_DBE_32.
;~ 							2. Select 'signal 3' in 'signal detsils' group box and click on 'Edit Signal...' button.
;~ 							3.Set signal 'Length' to '24' bits.
;~ 							4. Click 'OK' in 'Signal Details' dialog.

; Expected Results  : 		1. After step2, 'Message Details' dialog should be displayed with 'Signal 3' details.
;~ 							After step4, the dialog gets closed and the 'Signal3' length should be set to '24' bits in the 'Signal Details' groupbox.

; Critical (C)		:		Y
; TestCase No.		:		TS_DBE_34
; TestCases			:		Manipulation of message signals
; Test Data			:
; Test Setup		:		1. Continue from above test case TS_DBE_33.
;~ 							2. Right click on any signal in the list control of 'Signal Details' groupbox.

; Expected Results  : 		1. After step2, a popup menu with following menu items shouls be displayed: 'New Signal…', 'Edit Signal…', 'Delete Signal' and 'New Desc...'.

; Critical (C)		:		Y
; TestCase No.		:		TS_DBE_35
; TestCases			:		"Validation of signal descriptor and value"
; Test Data			:
; Test Setup		:		"1. Select a message in tree view
;~ 							2. Select a signal and Right Click on the Signal details list
;~ 							3. Select "New Descriptor" from displayed pop-up menu.
;~ 							4. Select OK button in the dialog"

; Expected Results  : 		"Should Warn user for invalid descriptor."

;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_DBE_30.au3****"&@CRLF)

_launchApp()

WinActivate($WIN_BUSMASTER,3)

Local $DupStartBit=0,$Sig2=0,$InvEndBit=0,$Sig3=0, $sigDetlvhwd=0,$sigName="",$sigLen=0,$sigType="",$sigMaxVal=0,$sigMinVal=0,$sigByteOrder="",$sig3Name="",$sig3ByteOrder="",$sig3Len=0,$PopUpMenu=0,$NullSigDesc=0
Dim $GetPopUp_Text[5]=["","","","",""]
If winexists($WIN_BUSMASTER) then

;~ 	_CreateConfig("cfxCANDB_30.cfx")																	; Create Configuration

	_openCANDB("testDBE_30.DBF")																		; Open the DB
	;------Maximize Child Window DatabaseEditor - CAN---------------
	_Maximize_childWindow("DatabaseEditor - CAN")
	sleep(1000)
	ControlClick($WIN_DBEditor_CAN,"",$BTN_NewSignal_DBEditor)											; Click on 'New Signal'

	if WinWaitActive($WIN_Signal_Details,"",3) Then
		ControlCommand($WIN_Signal_Details, "", $TXT_SigType_SigDetails, "SelectString", "int") 		; Select signal type from the combobox
		sleep(500)
		ControlSetText($WIN_Signal_Details, "", $TXT_SigName_SigDetails, "Sig2") 						; Add signal name
		sleep(500)
		ControlSetText($WIN_Signal_Details, "", $TXT_SigLen_SigDetails, 8) 								; enter signal length
		sleep(500)
		send("{TAB}")
		sleep(500)
		ControlCommand ($WIN_Signal_Details, "", $RBTN_Motorola_SigDetails, "Check")					; Select Motorola byte order
		sleep(500)
		ControlClick($WIN_Signal_Details, "", $BTN_OK_SigDetails) 										; Click 'OK' button
		if WinWaitActive("",$TXT_DupStartBit_BM,3) Then
			$DupStartBit=1
			ControlClick("",$TXT_DupStartBit_BM,$BTN_DupStartBit_BM)									; Click on OK button
			sleep(500)
		EndIf

		ControlSetText($WIN_Signal_Details, "", $TXT_ByteIndex_SigDetails, 1)				 			; Byte Index
		sleep(500)
		ControlClick($WIN_Signal_Details, "", $BTN_OK_SigDetails) 										; Click 'OK' button
		sleep(500)
	EndIf

	$sigDetlvhwd=controlgethandle($WIN_BUSMASTER,"",$LVC_SigDet_DBeditor)					; Get handle of signal details list view
	$sigName=_GUICtrlListView_GetItemText($sigDetlvhwd,1,0)									; Fetch the signal name
	$sigLen=_GUICtrlListView_GetItemText($sigDetlvhwd,1,3)									; Fetch the signal length
	$sigType=_GUICtrlListView_GetItemText($sigDetlvhwd,1,4)									; Fetch the signal signal type
	$sigMaxVal=_GUICtrlListView_GetItemText($sigDetlvhwd,1,5)								; Fetch the signal Max value
	$sigMinVal=_GUICtrlListView_GetItemText($sigDetlvhwd,1,6)								; Fetch the signal Min value
	$sigByteOrder=_GUICtrlListView_GetItemText($sigDetlvhwd,1,10)							; Fetch the signal byte order

	ConsoleWrite("$DupStartBit : "&$DupStartBit&@CRLF)
	consolewrite("$sigName="&$sigName&@CRLF)
	consolewrite("$sigLen="&$sigLen&@CRLF)
	consolewrite("$sigType="&$sigType&@CRLF)
	consolewrite("$sigMaxVal="&$sigMaxVal&@CRLF)
	consolewrite("$sigMinVal="&$sigMinVal&@CRLF)
	consolewrite("$sigByteOrder="&$sigByteOrder&@CRLF)

	if $DupStartBit=1 and $sigName="Sig2" and $sigLen=8 and $sigType="int" and $sigMaxVal="7F" and $sigMinVal=80 and $sigByteOrder="Motorola" Then
		$Sig2=1
	EndIf

	ControlClick($WIN_DBEditor_CAN,"",$BTN_NewSignal_DBEditor)											; Click on 'New Signal'

	if WinWaitActive($WIN_Signal_Details,"",3) Then
		ControlCommand($WIN_Signal_Details, "", $TXT_SigType_SigDetails, "SelectString", "int") 		; Select signal type from the combobox
		sleep(500)
		ControlSetText($WIN_Signal_Details, "", $TXT_SigName_SigDetails, "Sig3") 						; Add signal name
		sleep(500)
		ControlSetText($WIN_Signal_Details, "", $TXT_SigLen_SigDetails, 50) 							; enter signal length
		sleep(500)
		send("{TAB}")
		sleep(500)
		ControlCommand ($WIN_Signal_Details, "", $RBTN_Motorola_SigDetails, "Check")					; Select Motorola byte order
		sleep(500)
		ControlClick($WIN_Signal_Details, "", $BTN_OK_SigDetails) 										; Click 'OK' button
		if WinWaitActive("",$TXT_InvEndBit_BM,3) Then
			$InvEndBit=1
			ControlClick("",$TXT_InvEndBit_BM,$BTN_InvEndBit_BM)										; Click on OK button
			sleep(500)
		EndIf
		ControlSetText($WIN_Signal_Details, "", $TXT_ByteIndex_SigDetails, 7)				 			; Byte Index
		sleep(500)
		ControlSetText($WIN_Signal_Details, "", $TXT_SigLen_SigDetails, 48) 							; enter valid signal length
		sleep(500)
		ControlClick($WIN_Signal_Details, "", $BTN_OK_SigDetails) 										; Click 'OK' button
		sleep(500)
	EndIf

	$sig3Name=_GUICtrlListView_GetItemText($sigDetlvhwd,2,0)											; Fetch the signal name
	$sig3ByteOrder=_GUICtrlListView_GetItemText($sigDetlvhwd,2,10)										; Fetch the signal byte order

	ConsoleWrite("$InvEndBit : "&$InvEndBit&@CRLF)
	consolewrite("$sig3Name="&$sig3Name&@CRLF)
	consolewrite("$sig3ByteOrder="&$sig3ByteOrder&@CRLF)

	_GUICtrlListView_ClickItem($sigDetlvhwd,2)															; Select the signal 'Sig3'
	sleep(500)
	ControlClick($WIN_DBEditor_CAN,"",$BTN_EditSig_DBEditor)											; Click on 'Edit Signal'
	If WinWaitActive($WIN_Signal_Details,"",3) Then
		ControlSetText($WIN_Signal_Details, "", $TXT_SigLen_SigDetails, 24) 							; modify signal length
		sleep(500)
		ControlClick($WIN_Signal_Details, "", $BTN_OK_SigDetails) 										; Click 'OK' button
		sleep(500)
	EndIf
	$sig3Len=_GUICtrlListView_GetItemText($sigDetlvhwd,2,3)												; Fetch the signal length

	if $InvEndBit=1 and $sig3Name="Sig3" and $sig3ByteOrder="Motorola" and $sig3Len=24 Then
		$Sig3=1
	EndIf

	_GUICtrlListView_ClickItem($sigDetlvhwd,2,"right")
	sleep(500)

	$GetPopUp_Text = _GetPopUpMenuText()

	If $GetPopUp_Text[0]=5 Then
		if $GetPopUp_Text[1]="New Signal..." and $GetPopUp_Text[2]="Edit Signal..." and $GetPopUp_Text[3]="Delete Signal" and $GetPopUp_Text[5]="&New Desc..." Then
			$PopUpMenu=1
		EndIf
	EndIf
	send("{ESC}")

	ConsoleWrite($GetPopUp_Text[0]&@Crlf)
	ConsoleWrite($GetPopUp_Text[1]&@Crlf)
	ConsoleWrite($GetPopUp_Text[2]&@Crlf)
	ConsoleWrite($GetPopUp_Text[3]&@Crlf)
	ConsoleWrite($GetPopUp_Text[4]&@Crlf)
	ConsoleWrite($GetPopUp_Text[5]&@Crlf)

	ConsoleWrite($Sig2&@Crlf)
	ConsoleWrite($Sig3&@Crlf)
	ConsoleWrite($PopUpMenu&@Crlf)

	ControlClick($WIN_DBEditor_CAN,"",$BTN_SigDes_DBEditor)											; Click on 'New Desc' button
	if WinWaitActive($WIN_Signal_Desc,"",10) Then
		ControlClick($WIN_Signal_Desc, "", $BTN_OK_SigDesc) 										; Click 'OK'
		if WinWaitActive("",$TXT_EmptySigDesc_BM,3) Then
			$NullSigDesc=1
			send("{ESC}")																			; Close the warning dialog
		EndIf
		send("{ESC}")																				; Close the signal desc window
	EndIf
EndIf

if $Sig2=1 and $Sig3=1 and $PopUpMenu=1 and $NullSigDesc=1 Then
	_WriteResult("Pass","TS_DBE_30")
Else
	_WriteResult("Fail","TS_DBE_30")
EndIf

	_ActivatechildWindow($WIN_DBEditor_CAN1)
	WinClose($WIN_DBEditor_CAN1)
	if WinWaitActive($WIN_BUSMASTER,$saveDBtxt,2) Then
		ControlClick($WIN_BUSMASTER,"",$BTN_No_SaveDB)
	EndIf

$isAppNotRes=_CloseApp()																				; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_DBE_30")
EndIf

ConsoleWrite("****End : TS_DBE_30.au3****"&@CRLF)
ConsoleWrite(@CRLF)