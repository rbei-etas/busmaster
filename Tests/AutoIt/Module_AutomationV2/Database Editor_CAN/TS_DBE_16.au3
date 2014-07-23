;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_DBE_16
; TestCases			:		Message Details
; Test Data			:
; Test Setup		:		1. Create new message.
;~ 							2. Check if 'Message Name' edit box  and 'Message Length' are in 'editable state'.
;~ 							3. Try increasing the 'Message Length' by spin control up button continuously.
;~ 							4. Try decreasing the 'Message Length' by spin control down button continuously.

; Expected Results  : 		1. The 'Message Name' editbox and  'Message Length'are in editable state.
;~ 							2.After step3,the 'Message Length' will reach a max value of 8 and there after it should ignore the spin control clicks.
;~ 							3.After step4,the 'Message Length' will reach a min value of 0 and there after it should ignore the spin control clicks.

; Critical (C)		:		Y
; TestCase No.		:		TS_DBE_15
; TestCases			:		Add new Message to database
; Test Data			:
; Test Setup		:		"1.Select the root item in the tree view
;~ 							2. right click and select "New Message" menu"
;~ 							3.Enter Duplicate Message Name in message name  field
;~ 							4. Repeat the same test for Message ID field."
;~ 							5.Enter Duplicate Message ID in message id field. Select Ok button in the dialog"

; Expected Results  : 		After step2,"User should be able to key in data for the input fields in "Message Details" dialog."
;~ 							After step5,Should Warn the user for duplicate entry."
;~ 							Focus should move back to the Message ID field

; Critical (C)		:		Y
; TestCase No.		:		TS_DBE_17
; TestCases			:		Message Details
; Test Data			:
; Test Setup		:		1. Continue from above test case TS_DBE_16.
;~ 							2. Manually enter a value of 16 in 'Message Length' edit box.
;~ 							3. Click 'OK' in 'Message Details' dialog.

; Expected Results  : 		1. After step3, a message box showing 'Please enter integer between 0 and 8' should be shown.
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_DBE_16.au3****"&@CRLF)

_launchApp()

WinActivate($WIN_BUSMASTER,3)

Local $DupMsgName="Doesn't Exists",$DupMsgID="Doesn't Exists",$ValMsgID="Doesn't Exists"
if winexists($WIN_BUSMASTER) then

	_CreateConfig("cfxCANDB_16.cfx")																; Create Configuration

	_createCANDB("testCANDB16")																		; Create a CAN DB

	_DBmessage("n")																					; Select 'New Message' from right click menu

	WinWaitActive($WIN_Msg_Details,"",3)
	if WinExists($WIN_Msg_Details) Then
		ControlSetText($WIN_Msg_Details, "", $TXT_MsgName_MsgDetails,"Msg1")						; Enter the msg name
		sleep(500)
		ControlSetText($WIN_Msg_Details, "", $TXT_MsgID_MsgDetails,1) 								; Enter the msg ID
		sleep(500)
		ControlClick($WIN_Msg_Details, "", $TXT_MsgLen_MsgDetails)									; Click on Msg Length edit box
		sleep(500)
		send("{UP 10}")																				; Press the Down key 10 times
		sleep(500)
		ControlClick($WIN_Msg_Details, "", $TXT_MsgLen_MsgDetails)									; Click on Msg Length edit box
		$MsgLenAftSpinUp=ControlGetText($WIN_Msg_Details,"",$TXT_MsgLen_MsgDetails)					; Fetch the Msg Length name after pressing spin up control 10 times
		sleep(500)
		send("{DOWN 10}")																			; Press the Down key 10 times
		sleep(500)
		$MsgLenAftSpinDown=ControlGetText($WIN_Msg_Details,"",$TXT_MsgLen_MsgDetails)				; Fetch the Msg Length name after pressing spin down control 10 times
		sleep(500)
		ControlSetText($WIN_Msg_Details, "", $TXT_MsgLen_MsgDetails, 8) 							; Enter the msg len
		sleep(500)
		ControlClick($WIN_Msg_Details, "", $BTN_OK_MsgDetails) 										; Click 'OK' button
		sleep(500)
	EndIf

	_DBmessage("n")																					; Select 'New Message' from right click menu
	WinWaitActive($WIN_Msg_Details,"",3)
	If WinExists($WIN_Msg_Details) Then
		ControlSetText($WIN_Msg_Details, "", $TXT_MsgName_MsgDetails,"Msg1")						; Enter the msg name
		sleep(500)
		ControlSetText($WIN_Msg_Details, "", $TXT_MsgID_MsgDetails,1) 								; Enter the msg ID
		sleep(500)
		ControlClick($WIN_Msg_Details, "", $BTN_OK_MsgDetails) 										; Click 'OK' button
		If WinWaitActive("",$TXT_DupMsgName_BM,3) Then
			$DupMsgName="Exists"
			ControlClick($WIN_BUSMASTER, $TXT_DupMsgName_BM, $BTN_OKDupMsgName_BM)					; Click 'OK' button
			sleep(500)
		EndIf
		ControlSetText($WIN_Msg_Details, "", $TXT_MsgName_MsgDetails,"Msg2")						; Enter the msg name
		sleep(500)
		ControlClick($WIN_Msg_Details, "", $BTN_OK_MsgDetails) 										; Click 'OK' button
		If WinWaitActive("",$TXT_DupMsgID_BM,3) Then
			$DupMsgID="Exists"
			ControlClick($WIN_BUSMASTER, $TXT_DupMsgID_BM, $BTN_OKDupMsgID_BM)						; Click 'OK' button
			sleep(500)
		EndIf
		ControlSetText($WIN_Msg_Details, "", $TXT_MsgID_MsgDetails,2)								; Enter the Msg ID
		sleep(500)
		ControlSetText($WIN_Msg_Details, "", $TXT_MsgLen_MsgDetails, 16) 							; Enter the msg len
		sleep(500)
		ControlClick($WIN_Msg_Details, "", $BTN_OK_MsgDetails) 										; Click 'OK' button
		If WinWaitActive("",$TXT_MsgIDValidation_BM,3) Then
			$ValMsgID="Exists"
			ControlClick($WIN_BUSMASTER, $TXT_MsgIDValidation_BM, $BTN_MsgIDValidation_BM)			; Click 'OK' button
			sleep(500)
		EndIf
		ControlSetText($WIN_Msg_Details, "", $TXT_MsgLen_MsgDetails, 8) 							; Enter the msg len
		sleep(500)
		ControlClick($WIN_Msg_Details, "", $BTN_OK_MsgDetails) 										; Click 'OK' button
		sleep(500)
	EndIf

	ConsoleWrite("$MsgLenAftSpinUp :"&$MsgLenAftSpinUp&@CRLF)
	ConsoleWrite("$MsgLenAftSpinDown :"&$MsgLenAftSpinDown&@CRLF)
	ConsoleWrite("$DupMsgName :"&$DupMsgName&@CRLF)
	ConsoleWrite("$DupMsgID :"&$DupMsgID&@CRLF)
	ConsoleWrite("$ValMsgID :"&$ValMsgID&@CRLF)

EndIf


if $MsgLenAftSpinUp=8 and $MsgLenAftSpinDown=0 and $DupMsgName="Exists" and $DupMsgID="Exists" and $ValMsgID="Exists" Then
	_WriteResult("Pass","TS_DBE_16")
Else
	_WriteResult("Fail","TS_DBE_16")
EndIf


$isAppNotRes=_CloseApp()																				; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_DBE_16")
EndIf

ConsoleWrite("****End : TS_DBE_16.au3****"&@CRLF)
ConsoleWrite(@CRLF)