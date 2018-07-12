;==========================================================================Test Procedure =========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_MSGWIN_026
; TestCases			:		Message window display configuration.
; Test Data			:
; Test Setup		:		1. Continue from previous test case.
;                           2. Click on 'NDB' tab in 'Configure message display - CAN' dialog and observe the contents.


; Expected Results  : 		The list box contains all the messages which were present in the previously associated databases.
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_MSGWIN_026.au3****"&@CRLF)

_launchApp()

WinActivate($WIN_BUSMASTER)

if winexists($WIN_BUSMASTER) then

	_loadConfig("cfxCANMsgWin_26.cfx")
	$msgCount= _GetDBConfigMsgDispCount()                         ;Get Db message count
	_DissociateCANDB(0)
	;ControlClick($WIN_BUSMASTER,"[CLASS:Button; INSTANCE:2]",1)
	controlclick($WIN_Dissociate_CANDB,"","[CLASS:Button; INSTANCE:2]")							; Click on OK button \
	Sleep(3000)
    $msgCountNonDB=_GetNonDBConfigMsgDispCount()                  ;Get Non Db message count
	ConsoleWrite("$msgCount= " & $msgCount & @CRLF)
	ConsoleWrite("$msgCountNonDb= " & $msgCountNonDb & @CRLF)
	If($msgCount==$msgCountNonDb) Then
		_WriteResult("Pass","TS_MSGWIN_26")
	Else
	   _WriteResult("Fail","TS_MSGWIN_26")
	EndIf

EndIf
$isAppNotRes=_CloseApp()																				; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_MSGWIN_26")
EndIf

ConsoleWrite("****End : TS_MSGWIN_026.au3****"&@CRLF)
ConsoleWrite(@CRLF)



