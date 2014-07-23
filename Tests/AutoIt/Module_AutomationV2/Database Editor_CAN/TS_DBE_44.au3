;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_DBE_44
; TestCases			:		"Testing new Database Create, Save, Save As and close"
; Test Data			:
; Test Setup		:		"1. Select New from File -> Database menu.
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
ConsoleWrite("****Start : TS_DBE_44.au3****"&@CRLF)
_launchApp()

WinActivate($WIN_BUSMASTER,3)

Local $DBEditor="Doesn't Exists"

If winexists($WIN_BUSMASTER) then

	_CreateConfig("cfxCANDB_44.cfx")																	; Create Configuration

	_createCANDB("testDBE_44")																			; Create New Database File

	_DBmessage("n")																						; Select 'New Message' from right click menu

	_addMsgDetails("Msg1",15,8)

	ControlClick($WIN_DBEditor_CAN,"",$BTN_NewSignal_DBEditor)											; click on new signal button

	_addSigDetails("int","Signal1",32,0,0)																; Add the signal details

	WinMenuSelectItem($WIN_BUSMASTER,"",$CANMenu,$databaseMenu,$saveMenu)								; Select CAN->Database->Save
	sleep(250)

	WinMenuSelectItem($WIN_BUSMASTER,"",$CANMenu,$databaseMenu,$DBSaveAsMenu)							; Select CAN->Database->Save As
	$DBFolderPath = _OutputDataPath()																	; Set the DirPath to save the dbf file
	if WinWaitActive($WIN_DBSaveAs,"",3) Then
		ControlSend($WIN_DBSaveAs,"",$Edit_FName_SaveAsDB,$DBFolderPath&"\SaveAstestDBE_44")   			; Set Filename
		sleep(1000)
		$cDB=ControlClick($WIN_DBSaveAs,"",$BTN_SaveInst_SaveAsDB,"left") 								; Click on Save button
		sleep(1000)
	EndIf

	WinMenuSelectItem($WIN_BUSMASTER,"",$CANMenu,$databaseMenu,$closeMenu)								; Select CAN->Database->Close

	if WinExists($WIN_DBEditor_CAN) Then
		$DBEditor="Exists"
	EndIf
EndIf

ConsoleWrite("$DBEditor: "&$DBEditor&@CRLF)


if FileExists($DBFolderPath&"\testDBE_44.dbf") and FileExists($DBFolderPath&"\SaveAstestDBE_44.dbf") and $DBEditor="Doesn't Exists" Then
	_WriteResult("Pass","TS_DBE_44")
Else
	_WriteResult("Fail","TS_DBE_44")
EndIf

$isAppNotRes=_CloseApp()																				; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_DBE_44")
EndIf

ConsoleWrite("****End : TS_DBE_44.au3****"&@CRLF)
ConsoleWrite(@CRLF)