;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_DBE_01
; TestCases			:		Addition of message
; Test Data			:		-
; Test Setup		:		1. Create new database.
;~ 							2. Right click on root item to get a popup menu.
;~ 							3. Click on the 'New Message' menu item.
;~ 							4. Add the following message details: 'Message Name' as 'Msg1', 'Message ID' as '0x15', 'Message Length' as '8'.
;~ 							5. Click 'OK' in 'Message Details' dialog.

; Expected Results  : 		1. The 'New Message' menu item should be in enabled state and the remaining two menu items should be in disabled state in popup menu.
;~ 							2. After step3,a 'Messase Details' dialog with empty fields for name, id and 0 for length will be displayed.
;~ 							3. After step5,the message 'Msg1' gets added to NewDBFile.dbf which can be noticed in left pane tree view as child item.
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("***********DBEditor Script Execution Started************"&@CRLF)
ConsoleWrite("****Start : TS_DBE_01.au3****"&@CRLF)


_launchApp()																					; Check if the App is open else launch it

ProcessWait("BUSMASTER.exe")																	; Pauses script execution until a given process exists.
sleep(1000)

WinActivate($WIN_BUSMASTER,3)
if winexists($WIN_BUSMASTER) then
	_createConfig("cfxDBE_01")																	; Create New Configuration
	_createCANDB("testDBE_01")																	; Create New Database File
	sleep(1500)
	_DBmessage("n")																				; Select 'New Message' from right click menu
	sleep(1000)
	$msgNameHWD=controlgethandle($WIN_Msg_Details,"",$TXT_MsgName_MsgDetails)					; Fetch Msg Name Edit control handle
	$msgNametxt=_GUICtrlEdit_GetText($msgNameHWD)												; Fetch msg Name Text
	$msgIDHWD=controlgethandle($WIN_Msg_Details,"",$TXT_MsgID_MsgDetails)						; Fetch Msg ID Edit control handle
	$msgIDtxt=_GUICtrlEdit_GetText($msgIDHWD)													; Fetch msg ID Text
	$msglenHWD=controlgethandle($WIN_Msg_Details,"",$TXT_MsgLen_MsgDetails)						; Fetch Msg len Edit control handle
	$msglentxt=_GUICtrlEdit_GetText($msglenHWD)													; Fetch msg len Text
	ConsoleWrite("$msgNametxt :" &$msgNametxt & @crlf)
	ConsoleWrite("$msgIDtxt :" &$msgIDtxt & @crlf)
	ConsoleWrite("$msglentxt :" &$msglentxt & @crlf)
	_addMsgDetails("Msg1",15,8)																	; Add the message details
	$msgNameTreeView=ControlTreeView ($WIN_BUSMASTER,"",$TVC_DBFile_DBEditor,"GetText","#0|#0")	; Fetch the name of the first message
	ConsoleWrite("$msgNameTreeView :" &$msgNameTreeView & @crlf)
EndIf
if $msgNametxt="" and  $msgIDtxt="" and $msglentxt=0 and $msgNameTreeView="Msg1" then			; Write the status of the testcase
	_WriteResult("Pass","TS_DBE_01")
Else
	_WriteResult("Fail","TS_DBE_01")
EndIf

$isAppNotRes=_CloseApp()																		; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_DBE_01")
EndIf



ConsoleWrite("****End : TS_DBE_01.au3****"&@CRLF)
ConsoleWrite(@CRLF)