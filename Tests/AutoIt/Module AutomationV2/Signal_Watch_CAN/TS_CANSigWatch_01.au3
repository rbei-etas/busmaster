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

	_createConfig("cfxCANSigWatch_01")																		; Load Configuration

	_AssociateCANDB("StdExtDB.dbf")																			; Associate DB

	_ConfigSignalWatch($CAN,2)																				; Add two message signals to CAN signal watch list

	_saveConfig()																							; Save the configuration

	$isAppNotRes=_CloseApp()																				; Close the app

	_launchApp()																							; Launch the app

	_SignalWatchMenu($CAN,$SigWatchConfigMenu)																; Open Signal Watch configuration window

	$SignalCount=ControlListView($WIN_SigWatchConfig,"",$LVC_SigWatchList_SigWatch,"GetItemCount")			; Fetch the count of no. of items
	ConsoleWrite("$SignalCount: "&$SignalCount&@CRLF)

	if $SignalCount=2 Then
		$Signal1=ControlListView($WIN_SigWatchConfig,"",$LVC_SigWatchList_SigWatch,"GetText", 0)
		$Signal2=ControlListView($WIN_SigWatchConfig,"",$LVC_SigWatchList_SigWatch,"GetText", 1)
	EndIf
	winclose($WIN_SigWatchConfig)																			; Close Signal watch window
EndIf

ConsoleWrite("$Signal1 : "&$Signal1&@CRLF)
ConsoleWrite("$Signal2 : "&$Signal2&@CRLF)

if $Signal1="[0x101]MsgStdLil -> Sigstd1" and $Signal2="[0x102]MsgStdBig -> Sigstd2" Then
	_WriteResult("Pass","TS_CANSigWatch_01")
Else
	_WriteResult("Fail","TS_CANSigWatch_01")
EndIf

$isAppNotRes=_CloseApp()																					; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_CANSigWatch_01")
EndIf



ConsoleWrite("****End : TS_CANSigWatch_01.au3****"&@CRLF)
ConsoleWrite(@CRLF)
