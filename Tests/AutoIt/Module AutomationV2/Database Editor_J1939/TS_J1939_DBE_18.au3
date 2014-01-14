;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_J1939_DBE_18
; TestCases			:		"Add a dupliacte signal to message in database"
; Test Data			:
; Test Setup		:		1. Select a message in tree view.
;~ 							2. Right Click on the signal list box
;~ 							3. Select  "New Signal" from the pop-up menu.
;~ 							4. Key in Duplicate Signal Name

; Expected Results  : 		"Should Warn the user for invalid Signal Name"

; Critical (C)		:		Y
; TestCase No.		:		TS_J1939_DBE_22
; TestCases			:		"Validation of signal byte number and signal start bit"
; Test Data			:
; Test Setup		:		"1. Select a message in tree view.
;~ 							2. Right Click on the signal list box
;~ 							3. Select  "New Signal" from the pop-up menu.
;~ 							4. Key in same combination of  Byte No. and Start Bit values as the previous signal."

; Expected Results  : 		"Should not allow the user to accept these value."

; Critical (C)		:		Y
; TestCase No.		:		TS_J1939_DBE_23
; TestCases			:		"Validation of signal start bit"
; Test Data			:
; Test Setup		:		"1. Select a message in tree view.
;~ 							2. Right Click on the signal list box
;~ 							3. Select  "New Signal" from the pop-up menu.
;~ 							4. Key in values specified in the test data
;~ 							5. Repeat the test case with next test data"

; Expected Results  : 		"1. Should not allow the User to enter "-".
;~ 							2. Should Warn the user for invalid input.Valid range should be 0-7"
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_J1939_DBE_18.au3****"&@CRLF)

_launchApp()

WinActivate($WIN_BUSMASTER,3)

Local $DupSignalName="Doesn't Exists",$DupSignalSBit="Doesn't Exists",$InvStartBit="Doesn't Exists"
if winexists($WIN_BUSMASTER) then

	_CreateConfig("cfxJ1939DB_18.cfx")																; Create Configuration

	_openJ1939DB("testJ1939DBE_18.DBF")																; Open the DB

	ControlClick($WIN_DBEditor_J1939,"",$BTN_NewSignal_DBEditor)									; Click on 'New Signal'

	_addSigDetails("int","Sig1",32,4,0)																; Add a signal with same signal name as the first one to generate duplicate signal name warning messsage

	if WinWaitActive("",$TXT_DupSigName_BM,3) Then
		$DupSignalName="Exists"
		ControlClick("",$TXT_DupSigName_BM,$BTN_OKDupSigName_BM)									; Click on 'OK' button
		sleep(500)
	EndIf

	_addSigDetails("int","Sig2",32,0,0)																; Add a signal with same byte index to generate duplicate start bit value warning messsage

	if WinWaitActive("",$TXT_DupStartBit_BM,3) Then
		$DupSignalSBit="Exists"
		ControlClick("",$TXT_DupStartBit_BM,$BTN_DupStartBit_BM)									; Click on 'OK' button
		sleep(500)
	EndIf

	_addSigDetails("int","Sig3",32,0,8)																; Add a signal with invalid start bit to generate invalid start bit warning messsage

	if WinWaitActive("",$TXT_InvStartBit,3) Then
		$InvStartBit="Exists"
		ControlClick("",$TXT_InvStartBit,$BTN_OKInvStartBit_BM)										; Click on 'OK' button
		sleep(500)
		ControlSetText($WIN_Signal_Details, "", $TXT_StartBit_SigDetails, 0) 						; set Start Bit as 0
		sleep(500)
		ControlSetText($WIN_Signal_Details, "", $TXT_ByteIndex_SigDetails, 4) 						; set byte index to 4
		sleep(500)
		ControlClick($WIN_Signal_Details, "", $BTN_OK_SigDetails) 									; Click 'OK' button
	EndIf

EndIf

ConsoleWrite("$DupSignalName : "&$DupSignalName&@CRLF)
ConsoleWrite("$DupSignalSBit : "&$DupSignalSBit&@CRLF)
ConsoleWrite("$InvStartBit : "&$InvStartBit&@CRLF)

if $DupSignalName="Exists" and $DupSignalSBit="Exists" and $InvStartBit="Exists" Then
	_WriteResult("Pass","TS_J1939_DBE_18")
Else
	_WriteResult("Fail","TS_J1939_DBE_18")
EndIf


$isAppNotRes=_CloseApp()																				; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_J1939_DBE_18")
EndIf

ConsoleWrite("****End : TS_J1939_DBE_18.au3****"&@CRLF)
ConsoleWrite(@CRLF)