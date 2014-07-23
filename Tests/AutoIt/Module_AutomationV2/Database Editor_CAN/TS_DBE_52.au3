;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_DBE_52
; TestCases			:		Message Details
; Test Data			:
; Test Setup		:		1. Continue from above test case TS_DBE_51.
;~ 							2. Right click on a message 'NMH_Motor_01' in left pane tree view in 'Database Editor - CAN' window which shows a popup menu.
;~ 							3. Click on 'Edit Message' in the popup menu to get 'Message Details' dialog.
;~ 							4. Set the 'Message Length' to 6 by editing or using spin control.
;~ 							5. Click on 'OK' in 'Message Details' dialog.
;~ 							6.Click 'No' in the message box.
;~ 							7.Click 'Yes' in the message box.

; Expected Results  : 		1. After step5, a warning message 'You have signals defined for this message for previous message length.
;~ 							Reducing message length will delete redundent signals and associated description defined for this message. Do you want to delete these signals?' will be displayed.

;~ 							2.After step6, the message should continue to hold its previous message length of 8 and no loss to signals should happen. This can be checked in 'Message and Signal Information' groupbox in right pane of 'Database Editor -CAN' window.

;~ 							3.After step7, the message will loose its signals and signal descriptors information which fall under the last 2 bytes of the message. This can be checked in 'Message and Signal Information' groupbox in right pane of 'Database Editor -CAN' window.

;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_DBE_52.au3****"&@CRLF)

_launchApp()

WinActivate($WIN_BUSMASTER,3)

Local $msglentxt=0, $SignalCount=1

If winexists($WIN_BUSMASTER) then

	_openCANDB("testDBE_52.DBF")																		; Open the DB

	$DBTreeVwHWD = ControlGetHandle($WIN_DBEditor_CAN, "", $TVC_DBFile_DBEditor) 						; Fetch tree view control handle
	$childHWD = _GUICtrlTreeView_GetNext($DBTreeVwHWD, 0)
	_GUICtrlTreeView_ClickItem($DBTreeVwHWD, $childHWD, "Right") 										; Right Click on the msg node
	Send("e")
	sleep(750)

	_addMsgDetails("msg1",33,6)																			; Modify message details

	if WinWaitActive("",$TXT_ReduceMsgLen_BM,3) Then
		ControlClick($WIN_BUSMASTER,$TXT_ReduceMsgLen_BM,$BTN_NoReduceMsgLen_BM)						; Click on 'No' button
	EndIf

	$childHWD = _GUICtrlTreeView_GetNext($DBTreeVwHWD, 0)
	_GUICtrlTreeView_ClickItem($DBTreeVwHWD, $childHWD, "Right") 										; Right Click on the msg node
	Send("e")																							; Right click on the first message and select edit
	sleep(750)

	$msglenHWD=controlgethandle($WIN_Msg_Details,"",$TXT_MsgLen_MsgDetails)								; Fetch Msg len Edit control handle
	$msglentxt=_GUICtrlEdit_GetText($msglenHWD)															; Fetch msg len Text

	ControlSetText($WIN_Msg_Details, "", $TXT_MsgLen_MsgDetails, 6) 									; Modify the msg len
	ControlClick($WIN_Msg_Details, "", $BTN_OK_MsgDetails) 												; Click 'OK' button

	if WinWaitActive("",$TXT_ReduceMsgLen_BM,3) Then
		ControlClick($WIN_BUSMASTER,$TXT_ReduceMsgLen_BM,$BTN_YesReduceMsgLen_BM)						; Click on 'Yes' button
		sleep(1000)
	EndIf

	$sigDetlvhwd=controlgethandle($WIN_BUSMASTER,"",$LVC_SigDet_DBeditor)								; Get handle of signal details list view
	$SignalCount=_GUICtrlListView_GetItemCount($sigDetlvhwd)											; Fetch the signal count

EndIf

ConsoleWrite("$msglentxt :" &$msglentxt & @crlf)
ConsoleWrite("$SignalCount: "&$SignalCount&@CRLF)

if $msglentxt=8 and $SignalCount=0 Then
	_WriteResult("Pass","TS_DBE_52")
Else
	_WriteResult("Fail","TS_DBE_52")
EndIf

$isAppNotRes=_CloseApp()																				; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_DBE_52")
EndIf

ConsoleWrite("****End : TS_DBE_52.au3****"&@CRLF)
ConsoleWrite(@CRLF)