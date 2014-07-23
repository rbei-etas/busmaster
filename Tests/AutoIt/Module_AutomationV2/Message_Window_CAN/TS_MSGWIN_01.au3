;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_MSGWIN_01
; TestCases			:		Message Window Format
; Test Data			:		
; Test Setup		:		Start the Busmaster application.

; Expected Results  : 		1. Message window should appear in the front as an active window.
;~ 							2. Message window list should contain following columns by default - "Time, Tx/Rx, Channel, Msg Type, ID, Message, DLC, DataByte(s)"
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_MSGWIN_01.au3****"&@CRLF)

_launchApp()

WinActivate($WIN_BUSMASTER,3)
Local $MsgWinColCount=0,$ColTime="",$ColDir="",$ColChl="",$ColMsgType="",$ColMsgID="",$ColMsg="",$ColDLC="",$ColDB=""
if winexists($mWin) then
	
	_loadConfig("cfxCANMsgWin_01")													; Load Configuration
	
	$MsgWinColCount=_GetCANMsgWinColCount()											; Fetch the Column count of the message window
	
	if $MsgWinColCount=9 Then
		
		$HWD=_GetCANMsgWinHWD()
		$SecColH=_GUICtrlListView_GetColumn($HWD,1)										; Fetch the 2nd column header text
		$ColTime=StringStripWS($SecColH[5],2)											; Remove trailing spaces from 2nd column header text
		
		$ThirdColH=_GUICtrlListView_GetColumn($HWD,2)									; Fetch the 3rd column header text
		$ColDir=StringStripWS($ThirdColH[5],2)											; Remove trailing spaces from 3rd column header text
		
		$FourthColH=_GUICtrlListView_GetColumn($HWD,3)									; Fetch the 4th column header text
		$ColChl=StringStripWS($FourthColH[5],2)											; Remove trailing spaces from 4th column header text
		
		$FifthColH=_GUICtrlListView_GetColumn($HWD,4)									; Fetch the 5th column header text
		$ColMsgType=StringStripWS($FifthColH[5],2)										; Remove trailing spaces from 5th column header text
		
		$SixthColH=_GUICtrlListView_GetColumn($HWD,5)									; Fetch the 6th column header text
		$ColMsgID=StringStripWS($SixthColH[5],2)										; Remove trailing spaces from 6th column header text
		
		$SeventhColH=_GUICtrlListView_GetColumn($HWD,6)									; Fetch the 7th column header text
		$ColMsg=StringStripWS($SeventhColH[5],2)										; Remove trailing spaces from 7th column header text
		
		$EightColH=_GUICtrlListView_GetColumn($HWD,7)									; Fetch the 8th column header text
		$ColDLC=StringStripWS($EightColH[5],2)											; Remove trailing spaces from 8th column header text
		
		$NinthColH=_GUICtrlListView_GetColumn($HWD,8)									; Fetch the 9th column header text
		$ColDB=StringStripWS($NinthColH[5],2)											; Remove trailing spaces from 9th column header text
		
	EndIf

EndIf

consolewrite("$MsgWinColCount: "&$MsgWinColCount&@CRLF)
consolewrite("$ColTime: "&$ColTime&@CRLF)
consolewrite("$ColDir: "&$ColDir&@CRLF)
consolewrite("$ColChl: "&$ColChl&@CRLF)
consolewrite("$ColMsgType: "&$ColMsgType&@CRLF)
consolewrite("$ColMsg: "&$ColMsg&@CRLF)
consolewrite("$ColDLC: "&$ColDLC&@CRLF)
consolewrite("$ColDB: "&$ColDB&@CRLF)

if $ColTime="Time" and $ColDir="Tx/Rx" and $ColChl="Channel" and  $ColMsgType="Msg Type" and $ColMsg="Message" and $ColDLC="DLC" and $ColDB="Data Byte(s)" Then
	_WriteResult("Pass","TS_MSGWIN_01")
Else
	_WriteResult("Fail","TS_MSGWIN_01")
EndIf

$isAppNotRes=_CloseApp()																				; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_MSGWIN_01")
EndIf



ConsoleWrite("****End : TS_MSGWIN_01.au3****"&@CRLF)
ConsoleWrite(@CRLF)
