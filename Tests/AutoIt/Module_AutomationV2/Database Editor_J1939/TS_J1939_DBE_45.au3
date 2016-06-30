;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_J1939_DBE_45
; TestCases			:		"Testing of database modification."
; Test Data			:
; Test Setup		:		"1. Open the database
;~ 							2. Modify the Message Attributes, Signals, Signal Attributes and Signal Descriptors.
;~ 							3. Save the changes and close the editor.
;~ 							4. Open the saved database again.
;~ 							5. Modify the fileds and close the database with out saving.
;~ 							5. Open the database again."

; Expected Results  : 		"1. The changes will be persisted in the file only if it is saved."

; Critical (C)		:		Y
; TestCase No.		:		TS_J1939_DBE_46
; TestCases			:		Database serialization
; Test Data			:
; Test Setup		:		1. Continue from above test case TS_J1939_DBE_45.
;~ 							2. Try to close the database using menu option 'J1939 -> Database -> Close' or by by clicking close button in the Database Editor window.
;~ 							3.Click 'Yes' in message box.

; Expected Results  : 		1. After step2, a message 'Do you want to save changes to the database file?' should be displayed with 'Yes' and 'No' options.
;~ 							After step3,the Database Editor window gets closed successfully.

;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_J1939_DBE_45.au3****"&@CRLF)


_launchApp()

WinActivate($WIN_BUSMASTER,3)

Local $SaveDBWin=0

If winexists($WIN_BUSMASTER) then

	_createJ1939DB("testJ1939DBE_45")																	; Create New Database File

	;------Maximize Child Window DatabaseEditor - J1939---------------
	_Maximize_childWindow("DatabaseEditor - J1939")
	sleep(1000)


	_J1939DBmessage("n")																				; Select 'New Message' from right click menu

	_addMsgDetails("Msg1",15,8)

	ControlClick($WIN_DBEditor_J1939,"",$BTN_NewSignal_DBEditor)										; click on new signal button

	_addSigDetails("int","Signal1",32,0,0)																; Add the signal details

	WinMenuSelectItem($WIN_BUSMASTER,"",$J1939Menu,$databaseMenu,$closeMenu)								; Select CAN->Database->Close

	if WinWaitActive($WIN_BUSMASTER,$saveDBtxt,2) Then
		ControlClick($WIN_BUSMASTER,"",$BTN_Yes_SaveDB)													; Click on 'Yes' button
		$SaveDBWin=1
		sleep(1000)
	EndIf

	WinMenuSelectItem($WIN_BUSMASTER,"",$J1939Menu,$databaseMenu,$openMenu)

	$DirPath = _OutputDataPath()																		; Set the DirPath to saved dbf file path
	if WinWaitActive($WIN_SelJ1939_DBFile,"",5) Then
		ControlSend($WIN_SelJ1939_DBFile,"",$TXT_FileName_OpenDB,$DirPath&"\testJ1939DBE_45.DBF")				; Set Filename
		sleep(1000)
		$cDB=ControlClick($WIN_SelJ1939_DBFile,"",$BTN_OpenInst_OpenDB) 									; Click on open button
		sleep(1000)
	EndIf

	$sigDetlvhwd=controlgethandle($WIN_BUSMASTER,"",$LVC_SigDet_DBeditor)								; Get handle of signal details list view
	$SignalCount=_GUICtrlListView_GetItemCount($sigDetlvhwd)											; Fetch the signal count

	WinMenuSelectItem($WIN_BUSMASTER,"",$J1939Menu,$databaseMenu,$closeMenu)								; Select CAN->Database->Close

EndIf

ConsoleWrite("$SaveDBWin: "&$SaveDBWin&@CRLF)
ConsoleWrite("$SignalCount: "&$SignalCount&@CRLF)

if $SaveDBWin=1 and $SignalCount=1 Then
	_WriteResult("Pass","TS_J1939_DBE_45")
Else
	_WriteResult("Fail","TS_J1939_DBE_45")
EndIf

$isAppNotRes=_CloseApp()																				; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_J1939_DBE_45")
EndIf

ConsoleWrite("****End : TS_J1939_DBE_45.au3****"&@CRLF)
ConsoleWrite(@CRLF)