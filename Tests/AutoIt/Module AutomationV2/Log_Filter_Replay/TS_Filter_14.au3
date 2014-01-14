;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_Filter_14
; TestCases			:		Deleting filter from filter details list
; Test Data			:
; Test Setup		:		1. Select a filter entry from the filter details list.
;~ 							2. Select Delete button.
;~ 							3. Check filter details list.
;~ 							4. Select Delete all button.
;~ 							5. Check for delete confirmation message and select No.
;~ 							6. Check filter details list.
;~ 							7. Repeat steps 4 and 5 but select Yes to delete conformation message.
;~ 							8. Check the list again.

; Expected Results  : 		1. After step 3, the selected filter entry will be removed from the list.
;~ 							2. After step 5, delete conformations message will be posted to the user.
;~ 							3. After step 6, the filter details list will remain unchanged as delete all was canceled.
;~ 							4. After step 8, the filter details list will be empty.
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_Filter_14.au3****"&@CRLF)

_launchApp()																							; Check if the App is open else launch it

WinActivate($WIN_BUSMASTER)

Local $CountAftDel=0,$CountAftDelAllNo=0,$CountAftDelAllYes=0

if winexists($WIN_BUSMASTER) then
	_loadConfig("cfxFilter_14.cfx")																		; Create New Configuration

	_DeleteCANFilterDet(0)																				; Delete the first filter in the list

	_AppFilterMenu()																					; Select CAN->Filter Configuration->Configure

	if winexists($WIN_Config_filter) Then
		$FltSelhWd=controlgethandle($WIN_Config_filter,"",$LVC_FilterDetails_ConfigFilter)				; Get handle of filter selection list view
		$CountAftDel=_GUICtrlListView_GetItemCount($FltSelhWd)											; fetch the item count after deleting a filter

		ControlClick($WIN_Config_filter,"",$BTN_DELAll_Filter)											; Click on 'Delete All' filter button
		WinWaitActive($WIN_BUSMASTER,$TXT_DelAllFilter_BM,3)											; Wait for warning message pop up
		if winexists($WIN_BUSMASTER,$TXT_DelAllFilter_BM) Then
			ControlClick($WIN_BUSMASTER,$TXT_DelAllFilter_BM,$BTN_DelAllFilterNO_BM)					; Click on 'NO' button
			sleep(500)
		EndIf
		$CountAftDelAllNo=_GUICtrlListView_GetItemCount($FltSelhWd)										; fetch the item count after clicking on 'No' button on deleting all filter

		ControlClick($WIN_Config_filter,"",$BTN_DELAll_Filter)											; Click on 'Delete All' filter button
		WinWaitActive($WIN_BUSMASTER,$TXT_DelAllFilter_BM,3)											; Wait for warning message pop up
		if winexists($WIN_BUSMASTER,$TXT_DelAllFilter_BM) Then
			ControlClick($WIN_BUSMASTER,$TXT_DelAllFilter_BM,$BTN_DelAllFilterYes_BM)					; Click on 'NO' button
			sleep(500)
		EndIf
		$CountAftDelAllYes=_GUICtrlListView_GetItemCount($FltSelhWd)									; fetch the item count after clicking on No button on deleting all filter

		ControlClick($WIN_Config_filter,"",$BTN_OK_ConfigFilter)										; Click on OK button
	EndIf
EndIf

ConsoleWrite("$CountAftDel : "&$CountAftDel&@CRLF)
ConsoleWrite("$CountAftDelAllNo : "&$CountAftDelAllNo&@CRLF)
ConsoleWrite("$CountAftDelAllYes : "&$CountAftDelAllYes&@CRLF)

if $CountAftDel=2 and $CountAftDelAllNo=2 and $CountAftDelAllYes=0 Then									; Validate the result
	_WriteResult("Pass","TS_Filter_14")
Else
	_WriteResult("Fail","TS_Filter_14")
EndIf

$isAppNotRes=_CloseApp()																				; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_Filter_14")
EndIf



ConsoleWrite("****End : TS_Filter_14.au3****"&@CRLF)
ConsoleWrite(@CRLF)