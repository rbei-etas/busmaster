;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_CANSigWatch_07
; TestCases			:		"Invoking configuration window through ICON"
; Test Data			:        cfxCANSigWatch_07.cfx
; Test Setup		:		1. Open Signal Watch window.
;                           2. Close the window by clicking on X mark
;                           3. Click on the icon "Add/Delete Signals to watch window."
; Expected Results  : 		After step the signal configure window will be closed.
;                           2. After step 3 the "Signal watch List" will appear.
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_CANSigWatch_07.au3****"&@CRLF)

_launchApp()

WinActivate($WIN_BUSMASTER,3)
if winexists($WIN_BUSMASTER) then
	_loadConfig("cfxCANSigWatch_07.cfx")																		; Load Configuration
	Sleep(500)
	 _AssociateCANDB("CANSigWatch_DB_02.dbf")                                                                  ;Associate a database
	_SignalWatchMenu("CAN",$SigWatchConfigMenu)
	 Sleep(500)
	WinClose($WIN_SigWatchConfig)
	Sleep(1000)
	$ConfigureToolhWd=ControlGetHandle($WIN_BUSMASTER,"",$ToolBar_Configure)					; Get the handle of Add/Delete signals to the signal watch window toolbar
	_GUICtrlToolbar_ClickIndex($ConfigureToolhWd,7)                                             ;Click on Add/Delete signals to the signal watch window toolbar
	If WinExists($WIN_SigWatchConfig) Then
		_WriteResult("Pass","TS_CANSigWatch_07")
    Else
	    _WriteResult("Fail","TS_CANSigWatch_07")
    EndIf

EndIf

$isAppNotRes=_CloseApp()																					; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_CANSigWatch_07")
EndIf



ConsoleWrite("****End : TS_CANSigWatch_07.au3****"&@CRLF)
ConsoleWrite(@CRLF)



