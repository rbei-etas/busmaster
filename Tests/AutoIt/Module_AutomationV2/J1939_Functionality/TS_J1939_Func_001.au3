;===============================Test Cases/Test Data ========================================
; Critical (C)		:		Y
; TestCase No.		:		TS_J1939_Func_001
; TestCases			:		Tx message window and Trace window
; Test Data			:		cfxNSJ1939_01.cfx
; Test Setup		:	 1.Start Application
;                        2.Select "File ->  Configuration ->Load " menu option
;                        3  Select the configuration with the specified name from the open file dialog box and
;                        4. Click on Load button-
;                        5.Goto J1939->Activate.
;                        6.Goto J1939->database.
;                        7.Select database from the "Select Active Database Filename" dialog box
;                        8. Select Function-->Go Online.
;                        9. Go to view->Transmit window


; Expected Results  :   After step4, DIL j1939 query success message get displayed in trace window.
;                       After step5, J1939 transmit window dialog gets opened. With transport protocol function and Network management group box.
;
;================================Test Procedure =======================================================
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_J1939_Func_001.au3****"&@CRLF)

  _launchApp()
	WinActivate($WIN_BUSMASTER)
	Local $Trace1="",$Trace7=""
	if winexists($WIN_BUSMASTER) Then
																																		;launch the application
		_loadConfig("cfx_J1939Func_01.cfx")

;~ 		_ConnectDisconnect()																											; Connect

		_ActivateJ1939()                                                                                                               ;Activate J1939 Message window

        _AssocJ1939DB("J1939DB.dbf")                                                                                                    ;Associate DB

        $MsgCount=_GetTraceWinItemCount()																								; Fetch the Trace window msg count
	    If $MsgCount>=7 Then
			$Trace1=_GetTraceWinInfo(0)
		    $Trace2=_GetTraceWinInfo(1)
		    $Trace3=_GetTraceWinInfo(2)
	        $Trace4=_GetTraceWinInfo(3)
		    $Trace5=_GetTraceWinInfo(4)
		    $Trace6=_GetTraceWinInfo(5)
		    $Trace7=_GetTraceWinInfo(6)
		    $Trace8=_GetTraceWinInfo(7)
		EndIf

		ConsoleWrite("$Trace1 : " & $Trace1 & @CRLF)
		ConsoleWrite("$Trace7 : " & $Trace7 & @CRLF)

		_DeactJ1939()																													; Deactivate J1939

;~ 		_ConnectDisconnect()																											; Disconnect

		If($Trace1="Querying DIL.J1939 ..." And $Trace7="DIL.J1939 network started...") Then
			_WriteResult("Pass","STS_J1939_001")
        Else
	       _WriteResult("Fail","STS_J1939_001")
       EndIf
    EndIf

$isAppNotRes=_CloseApp()															; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","STS_J1939_001")
EndIf


ConsoleWrite("****End : STS_J1939_001****"&@CRLF)
ConsoleWrite(@CRLF)





