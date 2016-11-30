;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_J1939_DBE_44
; TestCases			:		"Testing new Database Create, Save, Save As and close"
; Test Data			:
; Test Setup		:		"1. Select New from J1939 -> Database menu.
;~ 							2. From the database editor add few database messages. Give different attribute values to these messages like Message Length, Type.
;~ 							3. Add Signals & Signals descriptors for these messages.
;~ 							4. Save the database by selecting Save from File -> Database.
;~ 							5. Remove the last message from the database.
;~ 							5. Save the database as CRH_0018_1 & CRH_0018_2 by selecting "File -> Database -> Save As...".
;~ 							6. Close the editor."

; Expected Results  : 		"1. The editor will create new database file on selection of New.
;~ 							2. The editor will save the changes in to the file while saving.
;~ 							3. The editor will save the file with the given name while selecting Save As option.
;~ 							4. The editor will be closed with out any exceptions"

;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_J1939_DBE_44.au3****"&@CRLF)

_launchApp()

WinActivate($WIN_BUSMASTER,3)

Local $DBEditor="Doesn't Exists"

If winexists($WIN_BUSMASTER) then

	_CreateConfig("cfxJ1939DB_44.cfx")																	; Create Configuration

	_createJ1939DB("testJ1939DBE_44")																	; Create New Database File

	;------Maximize Child Window DatabaseEditor - J1939---------------
	_Maximize_childWindow("DatabaseEditor - J1939")
	sleep(1000)


	_J1939DBmessage("n")																				; Select 'New Message' from right click menu

	_addMsgDetails("Msg1",15,8)

	ControlClick($WIN_DBEditor_J1939,"",$BTN_NewSignal_DBEditor)										; click on new signal button

	_addSigDetails("int","Signal1",32,0,0)																; Add the signal details

	WinRibbonSelectItem($WIN_BUSMASTER,"",$Tool,$J1939DBMenuTool,$saveMenu)								; Select J1939->Database->Save
	sleep(250)

	WinRibbonSelectItem($WIN_BUSMASTER,"",$Tool,$J1939DBMenuTool,$closeMenuDBEditor)							; Select J1939->Database->Close

	if WinExists($WIN_DBEditor_J1939) Then
		$DBEditor="Exists"
	EndIf

	$DBFolderPath = _OutputDataPath()																	; Set the DirPath to save the dbf file
EndIf

ConsoleWrite("$DBEditor: "&$DBEditor&@CRLF)


if FileExists($DBFolderPath&"\testJ1939DBE_44.dbf") and $DBEditor="Doesn't Exists" Then
	_WriteResult("Pass","TS_J1939_DBE_44")
Else
	_WriteResult("Fail","TS_J1939_DBE_44")
EndIf

$isAppNotRes=_CloseApp()																				; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_J1939_DBE_44")
EndIf

ConsoleWrite("****End : TS_J1939_DBE_44.au3****"&@CRLF)
ConsoleWrite(@CRLF)