;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_CANSigWatch_06
; TestCases			:		"Invoking configuration window"
; Test Data			:        cfxCANSigWatch_06.cfx
; Test Setup		:		1. Open CANvas.
;                           2. Load a Fresh configuration.
;                           3. Click on Menu Item 'Configure->Signal Watch'.
;                           4. Continue from the Test case
;                           5. Load a valid CANvas Database.
;                           6. Click on the menu item 'Configure->Signal watch'.
; Expected Results  : 		A dialog box saying "There is no active database. Please import database" will appear.
;                          2. After step 6 A dialog with title "Signal watch List" will appear.
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_CANSigWatch_06.au3****"&@CRLF)

_launchApp()

WinActivate($WIN_BUSMASTER,3)
if winexists($WIN_BUSMASTER) then
	_loadConfig("cfxCANSigWatch_06.cfx")																		; Load Configuration
	_SignalWatchMenu("CAN",$SigWatchConfigMenu)
	 Sleep(500)
	 If WinWaitActive($WIN_BUSMASTER,"There is no active database.",3) Then
		 ConsoleWrite("Error dialog" & @CRLF)
		 $Winexists=1
		 ControlClick($WIN_BUSMASTER,"",2)
	 Else
		  ConsoleWrite(" No Error dialog" & @CRLF)
		  $Winexists=0
	 EndIf
	 _AssociateCANDB("CANSigWatch_DB_02.dbf")
	 Sleep(1000)
	 _SignalWatchMenu("CAN",$SigWatchConfigMenu)
	If WinExists($WIN_SigWatchConfig) Then
		ConsoleWrite("signal watch window" & @CRLF)
		$SigWatchwin=1
		Sleep(500)
		WinClose($WIN_SigWatchConfig)
	Else
		ConsoleWrite("signal watch window does not exist" & @CRLF)
		$SigWatchwin=0
	EndIf
	If($Winexists=1 And $SigWatchwin=1) Then
		_WriteResult("Pass","TS_CANSigWatch_06")
    Else
	    _WriteResult("Fail","TS_CANSigWatch_06")
    EndIf

EndIf

$isAppNotRes=_CloseApp()																					; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_CANSigWatch_06")
EndIf



ConsoleWrite("****End : TS_CANSigWatch_06.au3****"&@CRLF)
ConsoleWrite(@CRLF)




