;==================================================Test Cases/Test Data =============================================
; Critical (C)		:	Y
; TestCase No.		:	TS_TxWin_33
; TestCases			:	"Storing Window Coordinates
; Test Data			:	TS_TxWin_01.cfx
; Test Setup		:	1. Start the application.
;                       2. Create a new Configuration file by selecting File -> Configuration -> New.
;                       3. Check the default window position of following windows.
;                       Signal Watch
;                       Network Statistics

; Expected Results  :   After step 3, mentioned window will be placed in default position. All windows will be visible and window state will be normal.
;                       All windows will be placed such that entire window will be inside main frame and the window size will be proportionate to main frame.
;==================================================Test Procedure =======================================================
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_TxWin_33.au3****"&@CRLF)
    _launchApp()
	WinActivate($WIN_BUSMASTER)
	if winexists($WIN_BUSMASTER) Then
 		_createConfigTest("TS_TxWin_33.cfx")
		Sleep(1000)
		 WinMenuSelectItem($WIN_BUSMASTER,"",$CANMenu,$CANNWStatsMenu)                                      ; Select CAN->Network Statistics
		 Sleep(500)
		 $pos=WinGetPos($WIN_NW_Stats)                                                                       ;Get the pisition of the Network Statistics window
		 ConsoleWrite("$pos[0]" & $pos[0] & @CRLF)
		  ConsoleWrite("$pos[1]" & $pos[1] & @CRLF)
		  Sleep(500)
		  WinClose($WIN_NW_Stats)
		  Sleep(1000)
		  WinMenuSelectItem($WIN_BUSMASTER,"",$CANMenu,$CANSigWatchMenu,$SigWatchActivateMenu)             ;Select CAN->Signal Watch->Activate
		  Sleep(500)
		  $pos1=WinGetPos($WIN_SignalWatch)                                                                 ;Get the position of Signal watch window
		  ConsoleWrite("$pos1[0]" & $pos1[0] & @CRLF)
		  ConsoleWrite("$pos1[1]" & $pos1[1] & @CRLF)
		  Sleep(500)
		  WinClose($WIN_SignalWatch)
		  WinMenuSelectItem($WIN_BUSMASTER,"",$fileMenu,$saveConfig)                                        ;Select File->Save to save configuration
		  WinMenuSelectItem($WIN_BUSMASTER,"",$fileMenu,$Exit)								                ; Select File->Exit from menu to close application
	      sleep(2000)
	     if WinWaitActive("",$SaveChangesConfigTXT,2) Then									                ; wait for save configuration dialog
		ControlClick("",$SaveChangesConfigTXT,$BTN_Yes_SaveConfig)						                    ; Click on No button
	     EndIf
	    _HandleSaveSimSysWin()
	EndIf
	_launchApp()                                                                                            ;Launch application
	WinActivate($WIN_BUSMASTER)
	if winexists($WIN_BUSMASTER) Then
 		 _loadConfig("TS_TxWin_33.cfx")
		Sleep(500)
		 WinMenuSelectItem($WIN_BUSMASTER,"",$CANMenu,$CANNWStatsMenu)                                      ; Select CAN->Network Statistics
		 Sleep(500)
		 $Newpos=WinGetPos($WIN_NW_Stats)                                                                   ;Get the pisition of the Network Statistics window
		 ConsoleWrite("$Newpos[0]" & $Newpos[0] & @CRLF)
		  ConsoleWrite("$Newpos[1]" & $Newpos[1] & @CRLF)
		  Sleep(500)
		  WinClose($WIN_NW_Stats)                                                                           ;Close Network statistics window
		  Sleep(1000)
		  WinMenuSelectItem($WIN_BUSMASTER,"",$CANMenu,$CANSigWatchMenu,$SigWatchActivateMenu)               ;Select CAN->Signal Watch->Activate
		  Sleep(500)
		  $Newpos1=WinGetPos($WIN_SignalWatch)                                                                ;Get the position of Signal watch window
		  ConsoleWrite("$Newpos1[0]" & $Newpos1[0] & @CRLF)
		  ConsoleWrite("$Newpos1[1]" & $Newpos1[1] & @CRLF)
		  Sleep(500)
		  WinClose($WIN_SignalWatch)                                                                          ;Close Signal watch window
		If($pos[0]= $Newpos[0]) Then                                                                          ; Check for X cordinate
			$Xcord=1
		Else
			$Xcord=0
		EndIf
		If($pos[1]= $Newpos[1]) Then                                                                         ;Check for Y cordinate
			$Ycord=1
		Else
			$Ycord=0
		EndIf
		If($Xcord=1 And $Ycord=1) Then
			_WriteResult("Pass","TS_Tx_033")
		Else
		   _WriteResult("Fail","TS_Tx_033")
	    EndIf

EndIf
    $isAppNotRes=_CloseApp()														                         ; Close the app

    if $isAppNotRes=1 Then
		_WriteResult("Warning","TS_Tx_033")
    EndIf

ConsoleWrite("****End : TS_TxWin_33.au3****"&@CRLF)
ConsoleWrite(@CRLF)





