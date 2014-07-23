;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_CANSigWatch_11
; TestCases			:		"Invoking configuration window through ICON"
; Test Data			:        cfxCANSigWatch_11.cfx
; Test Setup		:		1. Open Signal Watch window.
;                           2.Add some signals to signal watch list
;                           3. Click on the button "Cancel"
;                           4. Again open the configuratuion window
;                           5. observe the signals in the signal watch list
; Expected Results  :      .After the step 2 Configuration dialog will be closed.
;                          2. After step 4 the configuraion window will not show the newly added signals.
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_CANSigWatch_11.au3****"&@CRLF)

_launchApp()
WinActivate($WIN_BUSMASTER,3)
if winexists($WIN_BUSMASTER) then
	_loadConfig("cfxCANSigWatch_11.cfx")																		; Load Configuration
	Sleep(500)
     _AssociateCANDB("CANSigWatch_DB_02.dbf")
	_SignalWatchMenu("CAN",$SigWatchConfigMenu)
	Sleep(1000)
	ControlCommand($WIN_BUSMASTER,"",$COMBO_MsgList_SigWatch,"SetCurrentSelection",0)
	Sleep(500)
	$HWD=ControlGetHandle($WIN_SigWatchConfig,"",$LVC_SigList_SigWatch)										; Fetch the handle of Signal watch list
	_GUICtrlListView_ClickItem($HWD,0)																    ;  click on the first signal of signal watch window
	sleep(500)
	ControlClick($WIN_SigWatchConfig,"",$BTN_Add_SigWatch)                                              ;click on Add button
    Sleep(2000)
    $handle1=ControlGetHandle($WIN_SigWatchConfig,"",$LVC_SigWatchList_SigWatch)                        ;Get the handdle of signal watch list
	Sleep(500)
	$SigCount1=_GUICtrlListView_GetItemCount($handle1)                                                  ;Get the no of signals added to signal watch list
	ConsoleWrite("Item count before" & $SigCount1 & @CRLF)
	ControlClick($WIN_SigWatchConfig,"",$BTN_Cancel_SigWatch)                                           ;Click on cancel button
	_SignalWatchMenu("CAN",$SigWatchConfigMenu)
	Sleep(1000)
	$handle=ControlGetHandle($WIN_SigWatch&$CAN,"",$LVC_SigWatchList_SigWatch)
	Sleep(500)
	$SigCount=_GUICtrlListView_GetItemCount($handle)                                                    ;Get the no of signals added to signal watch list after clicking cancel
	ConsoleWrite("Item count" & $SigCount & @CRLF)
	If($SigCount1=1 And $SigCount=0) Then
		_WriteResult("Pass","TS_CANSigWatch_11")
    Else
	   _WriteResult("Fail","TS_CANSigWatch_11")
    EndIf
EndIf
$isAppNotRes=_CloseApp()																					; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_CANSigWatch_11")
EndIf



ConsoleWrite("****End : TS_CANSigWatch_11.au3****"&@CRLF)
ConsoleWrite(@CRLF)



