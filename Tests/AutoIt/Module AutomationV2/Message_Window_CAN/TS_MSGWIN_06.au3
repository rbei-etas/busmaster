#cs ----------------------------------------------------------------------------

 AutoIt Version: 3.3.8.1
 Author:         myName

 Script Function:
	Template AutoIt script.

#ce ----------------------------------------------------------------------------

; Script Start - Add your code below here
ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_MSGWIN_06.au3****"&@CRLF)


WinActivate($WIN_BUSMASTER)
if winexists($WIN_BUSMASTER) then
_loadConfig("cfxCANMsgWin_01")
    if winexists($mWin) then
	   _loadConfig("cfxCANMsgWin_01")													; Load Configuration
		$HWD=_GetCANMsgWinHWD()
		$ColH=_GUICtrlListView_GetColumn($HWD,0)										; Fetch the 1st column header text
        $Coltext=StringStripWS($ColH[5],2)											; Remove trailing spaces from 2nd column header text
        ConsoleWrite("rowtext" & $Coltext & @CRLF)
		If($Coltext="") Then
			_WriteResult("Pass","TS_MSGWIN_06")
        Else
	       _WriteResult("Fail","TS_MSGWIN_06")
	   EndIf
   EndIf

EndIf
$isAppNotRes=_CloseApp()																				; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_MSGWIN_06")
EndIf

ConsoleWrite("****End : TS_MSGWIN_06.au3****"&@CRLF)
ConsoleWrite(@CRLF)





