;==========================================================================Test Procedure =========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_MSGWIN_021
; TestCases			:		Message window display configuration.
; Test Data			:
; Test Setup		:		1. Continue from previous test case.
;                           2. Open message display configuration dialog by menu option 'Configure -> Message display'.
;                           3. Click on 'DB' tab in 'Configure message display - CAN' dialog and observe the contents.


; Expected Results  : 		  After step3, the 'Configure message display - CAN' dialog should contain all the
;                             database messages listed with 'Message ID' and 'Message Text' as column headers.
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_MSGWIN_021.au3****"&@CRLF)
Local $textexpctd[5]=["m4","ms1","m3","m2"]
local $msgcount=0,$dbMsg=0,$cnt=0

_launchApp()

WinActivate($WIN_BUSMASTER)

if winexists($WIN_BUSMASTER) then

	_loadConfig("cfxCANMsgWin_21.cfx")
	;$text1=_GetDBConfiMsgDisp(0)
	$msgCount= _GetDBConfigMsgDispCount()
  ; ConsoleWrite("id" & $text1[0] & @CRLF)
   ;ConsoleWrite("text" & $text1[1] & @CRLF)
   ConsoleWrite("msgcount" & $msgCount & @CRLF)
	if $msgcount=4 then
    for $i=0 to $msgcount-1
		$text1=_GetDBConfiMsgDisp($i)
		ConsoleWrite("$text1" & $text1[1] & @CRLF)
		if $text1[0]="1" and $text1[1]="m4"   Then
			$cnt = $cnt + 1
		EndIf
		if $text1[0]="12" and $text1[1]="ms1"   Then
			$cnt = $cnt + 1
		EndIf
		if $text1[0]="2" and $text1[1]="m3"   Then
			$cnt = $cnt + 1
		EndIf
		if $text1[0]="44" and $text1[1]="m2"   Then
			$cnt = $cnt + 1
		EndIf

	Next
	ConsoleWrite("cnt" & $cnt & @CRLF)
	if $cnt=4 Then
		$dbmsg=1
	EndIf
	endif
   ;If($text1[0]=2 And $text1[1]="m3" And $msgCount=4) Then
   If($dbMsg=1 And $msgCount=4) Then
	   _WriteResult("Pass","TS_MSGWIN_21")
	Else
	   _WriteResult("Fail","TS_MSGWIN_21")
	EndIf

EndIf
$isAppNotRes=_CloseApp()																				; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_MSGWIN_21")
EndIf

ConsoleWrite("****End : TS_MSGWIN_021.au3****"&@CRLF)
ConsoleWrite(@CRLF)

