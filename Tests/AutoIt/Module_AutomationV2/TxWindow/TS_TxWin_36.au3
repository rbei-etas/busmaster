;==================================================Test Cases/Test Data ===============================================================
; Critical (C)		:	Y
; TestCase No.		:	TS_TxWin_36
; TestCases			:	"Storing Window Coordinates
; Test Data			:	TS_TxWin36.cfx
; Test Setup		:	 1start the application.
;                        2. Move, resize or change the state of the windows given in the list.
;                        3. Close the application.
;                        4. Check for configuration save dialog.
; Expected Results  :  Application will throw a message while closing if any of the above mentioned window's position or state changes.

;==================================================Test Procedure ==========================================================================
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_TxWin_36.au3****"&@CRLF)
    _launchApp()
	WinActivate($WIN_BUSMASTER)
	if winexists($WIN_BUSMASTER) Then
 		_createConfigTest("TS_TxWin36.cfx")
		Sleep(1000)
		 WinMenuSelectItem($WIN_BUSMASTER,"",$CANMenu,$CANNWStatsMenu)                             ; Select CAN->Network Statistics
		 Sleep(500)
		 WinMenuSelectItem($WIN_BUSMASTER,"",$CANMenu,$CANSigWatchMenu,$SigWatchActivateMenu)       ;Select CAN->Signal Watch->Activate
		  Sleep(500)
		    ;---Updated below scripts to access Network statistics window since it is implemented as child window ----

		Opt("WinDetectHiddenText", 0)
		;$HWD= ControlGetHandle($WIN_NW_Stats,"",$LVC_Details_NW_Stats)
		Opt("WinSearchChildren", 1)
		Opt("WinTitleMatchMode", 1)
		WinWait("Network Statistics")

		;----------------------------------------------------------------------------
		 $pos=WinGetPos($WIN_NW_Stats)                                                               ;Get the pisition of the Network Statistics window
		 sleep(500)
		$pos1=WinGetPos($WIN_SignalWatch)                                                            ;Get the position of Signal watch window
		   ConsoleWrite("$pos[0]" & $pos[0] & @CRLF)
		  ConsoleWrite("$pos[1]" & $pos[1] & @CRLF)

		  ConsoleWrite("$pos1[0]" & $pos1[0] & @CRLF)
		  ConsoleWrite("$pos1[1]" & $pos1[1] & @CRLF)
		  Sleep(1000)
		  WinMove($WIN_NW_Stats,"",10,10)                                                           ;Change location of Network Statistics window by x,y cord value mentioned
		  Sleep(500)
		  $newPos=WinGetPos($WIN_NW_Stats)
		  ConsoleWrite("$newPos[0]" & $newPos[0] & @CRLF)
		  ConsoleWrite("$newPos[1]" & $newPos[1] & @CRLF)
		  Sleep(500)
		   WinMove($WIN_SignalWatch,"",30,20)
		   $newPos1=WinGetPos($WIN_SignalWatch)                                                      ;Change location of Signal watch window by x,y cord value mentioned
		   ConsoleWrite("$newPos1[0]" & $newPos1[0] & @CRLF)
		  ConsoleWrite("$newPos1[1]" & $newPos1[1] & @CRLF)
		   Sleep(1000)
		   WinMenuSelectItem($WIN_BUSMASTER,"",$fileMenu,$Exit)								          ; Select File->Exit from menu
	      sleep(2000)

		if WinWaitActive("",$SaveChangesConfigTXT,2) Then
			 _WriteResult("Pass","TS_Tx_036")
		Else
		   _WriteResult("Fail","TS_Tx_036")
	    EndIf

EndIf
$isAppNotRes=_CloseApp()														                         ; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_Tx_036")
EndIf

ConsoleWrite("****End : TS_TxWin_36.au3****"&@CRLF)
ConsoleWrite(@CRLF)


