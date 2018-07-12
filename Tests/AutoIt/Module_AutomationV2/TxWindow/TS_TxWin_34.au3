;==================================================Test Cases/Test Data ===============================================================
; Critical (C)		:	Y
; TestCase No.		:	TS_TxWin_34
; TestCases			:	"Storing Window Coordinates
; Test Data			:	TS_TxWin_34.cfx
; Test Setup		:	1. Start the application.
;                       2. Create a new Configuration file by selecting File -> Configuration -> New.
;                       3. Check the default window position of following windows.
;                       Signal Watch
;                       Network Statistics
;                       4. Resize all windows and change the location.
;                       5. Save the configuration file as Position.cfx.
;                       6. Now select a new configuration file by selecting File -> Configuration -> New.
;                       7. Check the window positions.
;                       8. Continue from the previous test case.
;                       9. Load the configuration file saved in the previous test case step 3.
;                       10. Check the window positions.

; Expected Results  :  After step 7, all windows will be moved to default position and size as mentioned in the previous test case.
;                      All windows will be properly placed in to the default window position and state.
;                      After step10 all windows will be positioned as per the placement changes made in step 4 of previous test case. Window size and state will also be restored.

;==================================================Test Procedure ==========================================================================
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_TxWin_34.au3****"&@CRLF)
    _launchApp()
	WinActivate($WIN_BUSMASTER)
	if winexists($WIN_BUSMASTER) Then
 		_createConfigTest("TS_TxWin_34.cfx")
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
            WinMenuSelectItem($WIN_BUSMASTER,"",$fileMenu,$saveConfig)
		  WinMenuSelectItem($WIN_BUSMASTER,"",$fileMenu,$Exit)								          ; Select File->Exit from menu
	      sleep(2000)
	     if WinWaitActive("",$SaveChangesConfigTXT,2) Then									          ; wait for save configuration dialog
			ControlClick("",$SaveChangesConfigTXT,$BTN_Yes_SaveConfig)						           ; Click on No button
		EndIf
		 _HandleSaveSimSysWin()
	EndIf
	_launchApp()
	WinActivate($WIN_BUSMASTER)
	if winexists($WIN_BUSMASTER) Then
		 WinMenuSelectItem($WIN_BUSMASTER,"",$CANMenu,$CANNWStatsMenu)                                  ; Select CAN->Network Statistics
		 Sleep(500)
		 WinMenuSelectItem($WIN_BUSMASTER,"",$CANMenu,$CANSigWatchMenu,$SigWatchActivateMenu)            ;Select CAN->Signal Watch->Activate
		  Sleep(500)
		 $posAft=WinGetPos($WIN_NW_Stats)                                                                ;Get the pisition of the Network Statistics window
		 sleep(500)
		$posAft1=WinGetPos($WIN_SignalWatch)                                                             ;Get the position of Signal watch window
		 ConsoleWrite("$posAft[0]" & $posAft[0] & @CRLF)
		  ConsoleWrite("$posAft[1]" & $posAft[1] & @CRLF)

		  ConsoleWrite("$posAft1[0]" & $posAft1[0] & @CRLF)
		  ConsoleWrite("$posAft1[1]" & $posAft1[1] & @CRLF)
		  Sleep(1000)

		  If($newPos[0]=$posAft[0] And $newPos[1]=$posAft[1]) Then                                       ;Check if previous location and current location of Network statistics window are same
			  $NwStat=1
		  Else
			  $NwStat=0
		  EndIf
		  If($newPos1[0]=$posAft1[0] And $newPos1[1]=$posAft1[1]) Then                                  ;Check if previous location and current location of Signal Watch window are same
			  $SigWatch=1
		  Else
			  $SigWatch=0
		  EndIf
		  If($NwStat=1 And $SigWatch=1) Then
			_WriteResult("Pass","TS_Tx_034")
		Else
		   _WriteResult("Fail","TS_Tx_034")
	    EndIf

EndIf
$isAppNotRes=_CloseApp()														                         ; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_Tx_034")
EndIf

ConsoleWrite("****End : TS_TxWin_34.au3****"&@CRLF)
ConsoleWrite(@CRLF)

