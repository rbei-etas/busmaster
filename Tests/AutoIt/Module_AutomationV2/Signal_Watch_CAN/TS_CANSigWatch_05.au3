;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_CANSigWatch_05
; TestCases			:		"Signal watch window updation"
; Test Data			:
; Test Setup		:		1. Activate Signal watch window and start message transmitting.
;~ 							2. open the signal watch window configuratuin dialog.
;~ 							3. Delete some signals on right side.
;~ 							4. click on cancel button.
;~ 							5. clear the signal watch window
;~ 							6. observe the signal watch window.
;~ 							7.Activate Signal watch window and start message transmitting.
;~ 							8. open the signal watch window configuratuin dialog.
;~ 							9. Add some more signals on right side.
;~ 							10. click on ok button.
;~ 							11. observe the signal watch window.
;~ 							12. Activate Signal watch window and start message transmitting.
;~ 							13. open the signal watch window configuratuin dialog.
;~ 							14. Add some more signals on right side.
;~ 							15. click on cancel button.
;~ 							16. observe the signal watch window

; Expected Results  : 		1.After step 6 the signal watch window will show all the previous message including the deleted messages also.
;~ 							2.after step 11 After Step 5 the signal watch window will show the newly added signals also.
;~ 							3.After Step 16 the signal watch window will not show the added signals.
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_CANSigWatch_05.au3****"&@CRLF)

_launchApp()																								; Check if the App is open else launch it

local $ItemCount1=0,$ItemCount2=0,$ItemCount3=0,$ItemCount4=0

if winexists($WIN_BUSMASTER) then

	_loadConfig("cfxCANSigWatch_05.cfx")																		; Load Configuration

	_ConnectDisconnect()																					; Connect

	_TransmitMsgsMenu()																						; Transmit CAN messages
	sleep(1000)

	_SignalWatchMenu($CAN,$SigWatchActivateMenu)															; Open signal watch window

	$ItemCount1=_GetSigWatchWinItemCount()																	; Fetch the count of items

	_SignalWatchMenu($CAN,$SigWatchConfigMenu)																; Open configure signal watch window

	ControlClick($WIN_SigWatchConfig,"",$BTN_DelAll_SigWatch)												; Click on "Delete All" button

	ControlClick($WIN_SigWatchConfig,"",$BTN_Cancel_SigWatch)												; Click on "Cancel" button

	$HWD=ControlGetHandle($WIN_SigWatch&$CAN,"",$LVC_Value_SigWatch)										; Fetch the handle of Signal watch window
	_GUICtrlListView_ClickItem($HWD,0,"Right")																; Right click on the signal watch window
	sleep(500)

	send("{DOWN}")																							; select Clear from the pop up menu
	send("{ENTER}")																							; Press Enter
	sleep(1000)

	$ItemCount2=_GetSigWatchWinItemCount()																	; Fetch the count of items

	_SignalWatchMenu($CAN,$SigWatchConfigMenu)																; Open configure signal watch window

	ControlListView($WIN_SigWatchConfig,"",$LVC_SigList_SigWatch,"Select",0)								; Select the first item in the config signal watch
	sleep(250)

	ControlClick($WIN_SigWatchConfig,"",$BTN_Add_SigWatch)													; Click on "Add" button

	ControlClick($WIN_SigWatchConfig,"",$BTN_OK_SigWatch)													; Click on "OK" button
	sleep(1000)

	$ItemCount3=_GetSigWatchWinItemCount()																	; Fetch the count of items

	_SignalWatchMenu($CAN,$SigWatchConfigMenu)																; Open configure signal watch window

	ControlListView($WIN_SigWatchConfig,"",$LVC_SigList_SigWatch,"Select",0)								; Select the first item in the config signal watch
	sleep(250)

	ControlClick($WIN_SigWatchConfig,"",$BTN_Add_SigWatch)													; Click on "Add" button

	ControlClick($WIN_SigWatchConfig,"",$BTN_Cancel_SigWatch)												; Click on "Cancel" button

	$ItemCount4=_GetSigWatchWinItemCount()																	; Fetch the count of items

	ConsoleWrite("$ItemCount1 : "&$ItemCount1&@CRLF)
	ConsoleWrite("$ItemCount2 : "&$ItemCount2&@CRLF)
	ConsoleWrite("$ItemCount3 : "&$ItemCount3&@CRLF)
	ConsoleWrite("$ItemCount4 : "&$ItemCount4&@CRLF)

	_ConnectDisconnect()																					; Disconnect

	winclose($WIN_SigWatch&$CAN)																			; Close the signal watch window

EndIf

if $ItemCount1=2 and $ItemCount2=2 and $ItemCount3=3 and $ItemCount4=3 Then
	_WriteResult("Pass","TS_CANSigWatch_05")
Else
	_WriteResult("Fail","TS_CANSigWatch_05")
EndIf

$isAppNotRes=_CloseApp()																					; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_CANSigWatch_05")
EndIf



ConsoleWrite("****End : TS_CANSigWatch_05.au3****"&@CRLF)
ConsoleWrite(@CRLF)
