;=== Test Cases/Test Data ===
; Module			:		DBEditor
; Critical (C)		:		Y
; TestCase No.		:		1
; TestCases			:		Addition of message
; Test Strategy		:		Black Box
; Test Data			:		-
; === Test Procedure ===

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("***********DBEditor Script Execution Started************"&@CRLF)
ConsoleWrite("****Start : TS_DBE_01.au3****"&@CRLF)

ProcessWait("BUSMASTER.exe")											; Pauses script execution until a given process exists.
sleep(1000)

WinActivate($WIN_BUSMASTER,3)
if winexists($WIN_BUSMASTER) then
	_createConfig("cfxDBE_01")												; Create New Configuration
	_createCANDB("testDBE_01")												; Create New Database File
	sleep(1500)
	_DBmessage("n")															; Select 'New Message' from right click menu
	sleep(1000)
	$msgNameHWD=controlgethandle($WIN_Msg_Details,"",$TXT_MsgName_MsgDetails)			; Fetch Msg Name Edit control handle
	$msgNametxt=_GUICtrlEdit_GetText($msgNameHWD)							; Fetch msg Name Text
	$msgIDHWD=controlgethandle($WIN_Msg_Details,"",$TXT_MsgID_MsgDetails)				; Fetch Msg ID Edit control handle
	$msgIDtxt=_GUICtrlEdit_GetText($msgIDHWD)								; Fetch msg ID Text
	$msglenHWD=controlgethandle($WIN_Msg_Details,"",$TXT_MsgLen_MsgDetails)				; Fetch Msg len Edit control handle
	$msglentxt=_GUICtrlEdit_GetText($msglenHWD)								; Fetch msg len Text
	ConsoleWrite("$msgNametxt :" &$msgNametxt & @crlf)
	ConsoleWrite("$msgIDtxt :" &$msgIDtxt & @crlf)
	ConsoleWrite("$msglentxt :" &$msglentxt & @crlf)
	_addMsgDetails("Msg1",15,8)												; Add the message details
	$msgNameTreeView=ControlTreeView ($WIN_BUSMASTER,"",$TVC_DBFile_DBEditor,"GetText","#0|#0")		; Fetch the name of the first message
	ConsoleWrite("$msgNameTreeView :" &$msgNameTreeView & @crlf)
EndIf
if $msgNametxt="" and  $msgIDtxt="" and $msglentxt=0 and $msgNameTreeView="Msg1" then		; Write the status of the testcase
	_ExcelWriteCell($oExcel, "Pass", 6, 2)
Else
	_ExcelWriteCell($oExcel, "Fail", 6, 2)
EndIf
ConsoleWrite("****End : TS_DBE_01.au3****"&@CRLF)
ConsoleWrite(@CRLF)