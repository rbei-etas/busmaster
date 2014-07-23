;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_CANSigWatch_01
; TestCases			:		"Configuration saving of signal watch Configuration window"
; Test Data			:
; Test Setup		:		1. Add Signals to signal watch list.
;~ 							2. Note down the signals in the signal watch list.
;~ 							3. Save the CANvas configuration(File -> Configuration ->Save).
;~ 							4. Close and open CANvas.
;~ 							5. open the signal configuration window and check the signal watch list.
;~ 							6. Close the configuration window.

; Expected Results  : 		1. After step 5 the signal watch list will contain all the signals of step 2
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_CANSigWatch_01.au3****"&@CRLF)

_launchApp()

WinActivate($WIN_BUSMASTER,3)

Local $Signal1="",$Signal2=""
if winexists($WIN_BUSMASTER) then

	_loadConfig("cfxCANSigWatch_02")																		; Load Configuration

	_AssociateCANDB("StdExtDB.dbf")																			; Associate DB
	Sleep(1000)
	_AssociateCANDB("CANSigWatch_DB_02.dbf")
     Sleep(1000)
	 _SignalWatchMenu("CAN",$SigWatchConfigMenu)
	 Sleep(1000)
     $MsgListHWD=ControlGetHandle($WIN_SigWatchConfig,"",$COMBO_MsgList_SigWatch)						; Fetch the handle of Message List combo box
	 $MsgCount=_GUICtrlComboBox_GetCount($MsgListHWD)
	 ConsoleWrite("No of messages before" & $MsgCount & @CRLF)
	 winclose($WIN_SigWatchConfig)
	  _DissociateCANDB(1)
	  if WinWaitActive("",$disDBtxt,3) Then
		WinActivate("",$disDBtxt)
		ControlClick($WIN_BUSMASTER,"",6)	                                                        ; Click on 'yes' button in "Dissociation will clear Signal Watch List." warning message
        Sleep(1000)
	   EndIf

	   _DissociateCANDB(0)
		Sleep(500)
		ConsoleWrite("At second dissociate function" & @CRLF)
		if WinWaitActive("",$disDBtxt,3) Then
			ConsoleWrite("in if statement" & @CRLF)
		WinActivate("",$disDBtxt)
		ControlClick($WIN_BUSMASTER,"",6)		  									; Click on 'yes' button in "Dissociation will clear Signal Watch List." warning message
		ConsoleWrite("In control click" & @CRLF)
		sleep(1000)
	   EndIf

	_SignalWatchMenu("CAN",$SigWatchConfigMenu)
	 Sleep(1000)

	 If WinWaitActive($WIN_BUSMASTER,"There is no active database.",3) Then
		 ConsoleWrite("Error dialog" & @CRLF)
		 $Winexists=1
		 ControlClick($WIN_BUSMASTER,"",$BTN_OK_SigWatch)
	 EndIf

 EndIf

