;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_MSGWIN_06
; TestCases			:		Message Window Format.
; Test Data			:
; Test Setup		:		Start the BUSMASTER application, Activate J1939 using menu option "J1939-> Activate"

; Expected Results  : 		 J1939 Message window should appear in the front as an active window.
;                            2. Message window list should contain following columns by default - "Time, Channel, CAN ID, PGN, PGN Name, Type, Src, Dest, Priority, Tx/Rx, DLC, Databyte(s)"
;==========================================================================Test Procedure =========================================================================
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_MsgWndJ1939_06.au3****"&@CRLF)

    _launchApp()
	WinActivate($WIN_BUSMASTER)
	Local $ColTime="",$Colch="",$ColCANid="",$ColPGN="",$ColPGNname="",$ColType="",$ColSrc="",$ColDest="",$ColPrio="",$ColDir="",$ColDLC="",$ColData=""
	if winexists($WIN_BUSMASTER) Then
		_loadConfig("cfxMsgWin_J1939_06.cfx")
		 _ActivateJ1939()                                                                                      ;Activate J1939 Message window
		 sleep(1000)
		 $HWD=_GetCANMsgWinHWD()
		$SecColH=_GUICtrlListView_GetColumn($HWD,1)										; Fetch the 2nd column header text
		$ColTime=StringStripWS($SecColH[5],2)

		$ThirdColH=_GUICtrlListView_GetColumn($HWD,2)									; Fetch the 3rd column header text
		$Colch=StringStripWS($ThirdColH[5],2)											; Remove trailing spaces from 3rd column header text

		$FourthColH=_GUICtrlListView_GetColumn($HWD,3)									; Fetch the 4th column header text
		$ColCANid=StringStripWS($FourthColH[5],2)											; Remove trailing spaces from 4th column header text

		$FifthColH=_GUICtrlListView_GetColumn($HWD,4)									; Fetch the 5th column header text
		$ColPGN=StringStripWS($FifthColH[5],2)										; Remove trailing spaces from 5th column header text

		$SixthColH=_GUICtrlListView_GetColumn($HWD,5)									; Fetch the 6th column header text
		$ColPGNname=StringStripWS($SixthColH[5],2)										; Remove trailing spaces from 6th column header text

		$SeventhColH=_GUICtrlListView_GetColumn($HWD,6)									; Fetch the 7th column header text
		$ColType=StringStripWS($SeventhColH[5],2)										; Remove trailing spaces from 7th column header text

		$EightColH=_GUICtrlListView_GetColumn($HWD,7)									; Fetch the 8th column header text
		$ColSrc=StringStripWS($EightColH[5],2)											; Remove trailing spaces from 8th column header text

		$NinthColH=_GUICtrlListView_GetColumn($HWD,8)									; Fetch the 9th column header text
		$ColDest=StringStripWS($NinthColH[5],2)											; Remove trailing spaces from 9th column header text

		$TenthColH=_GUICtrlListView_GetColumn($HWD,9)									; Fetch the 10th column header text
		$ColPrio=StringStripWS($TenthColH[5],2)											; Remove trailing spaces from 9th column header text

		$EleventhColH=_GUICtrlListView_GetColumn($HWD,10)									; Fetch the 11th column header text
		$ColDir=StringStripWS($EleventhColH[5],2)											; Remove trailing spaces from 9th column header text

		$TwelthColH=_GUICtrlListView_GetColumn($HWD,11)									; Fetch the 12th column header text
		$ColDLC=StringStripWS($TwelthColH[5],2)											; Remove trailing spaces from 9th column header text

		$ThirteenColH=_GUICtrlListView_GetColumn($HWD,12)									; Fetch the 13th column header text
		$ColData=StringStripWS($ThirteenColH[5],2)


	  ConsoleWrite("$ColTime" & $ColTime & @CRLF)
	  ConsoleWrite("$Colch" & $Colch & @CRLF)
	  ConsoleWrite("$ColCANid" & $ColCANid & @CRLF)
	  ConsoleWrite("$ColPGN" & $ColPGN & @CRLF)
	  ConsoleWrite("$ColPGNname" & $ColPGNname & @CRLF)
	   ConsoleWrite("$ColType" & $ColType & @CRLF)
	    ConsoleWrite("$ColSrc" & $ColSrc & @CRLF)
		ConsoleWrite("$ColDest" & $ColDest & @CRLF)
		ConsoleWrite("$ColPrio" & $ColPrio & @CRLF)
		ConsoleWrite("$ColDir" & $ColDir & @CRLF)
		ConsoleWrite("$ColDLC" & $ColDLC & @CRLF)
		ConsoleWrite("$ColData" & $ColData & @CRLF)

      If($ColTime="Time" And $Colch="Channel" And $ColCANid="CAN ID" And $ColPGN="PGN" And $ColPGNname="PGN Name" And $ColType="Type" And $ColSrc="Src" And $ColDest="Dest" And $ColPrio="Priority" And $ColDir="Tx/Rx" And $ColDLC="DLC" And $ColData="Data Byte(s)") Then
		_WriteResult("Pass","TS_MsgWndJ1939_06")
        Else
	       _WriteResult("Fail","TS_MsgWndJ1939_06")
	   EndIf
EndIf
$isAppNotRes=_CloseApp()															; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_MsgWndJ1939_06")
EndIf

ConsoleWrite("****End : TS_MsgWndJ1939_06****"&@CRLF)
ConsoleWrite(@CRLF)








