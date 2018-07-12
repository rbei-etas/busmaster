;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_Filter_04
; TestCases			:		Check for GUI - Message Filter
; Test Data			:
; Test Setup		:		1."Invoke dialog box for filter configuration"
;~ 							2. Add Filter in filter list.
;~ 							3. double click on the second column item.
;~ 							4. A combo box with items Stop and pass will appear

; Expected Results  : 		Stop and Pass filter should be available in the combo box
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_Filter_04.au3****"&@CRLF)

_launchApp()

Local $FilterTypeTxt1="",$FilterTypeTxt2=""
if winexists($WIN_BUSMASTER) then
	_loadConfig("cfxFilter_04")																; Create New Configuration

	_AppFilterMenu()																		; Open Configure Filter dialog

	ControlClick($WIN_Config_filter,"",$BTN_AddFilter_ConfigFilter)							; Click on 'Add' filter button
	sleep(1000)

	$hWnd = ControlGetHandle($WIN_Config_filter,"",$LVC_FilterList_ConfigFilter)
	$FilterTypeTxt1=_GUICtrlListView_GetItemText($hWnd,0,1)									; Fetch Stop filter type Text
	ConsoleWrite(@CRLF & $FilterTypeTxt1 & @CRLF)
	$FilTypePos=_GUICtrlListView_GetItemPosition($hWnd, 0)									; Get position of Signal details list view control
	ControlClick($WIN_Config_filter,"",1000,"Left",2,$FilTypePos[0]+100,$FilTypePos[1])		; Double click on the 'Filter Type'
	sleep(500)
	send("{DOWN}")
	sleep(500)
	send("{ENTER}")																			; Select Pass filter
	sleep(500)
	$FilterTypeTxt2=_GUICtrlListView_GetItemText($hWnd,0,1)									; Fetch Pass filter type text
	ConsoleWrite(@CRLF & $FilterTypeTxt2 & @CRLF)
	ControlClick($WIN_Config_filter,"",$BTN_OK_ConfigFilter)								; Click on OK button
	sleep(500)
EndIf

if $FilterTypeTxt1="Stop" and  $FilterTypeTxt2="Pass" Then									; Validate the result
	_WriteResult("Pass","TS_Filter_04")
Else
	_WriteResult("Fail","TS_Filter_04")
EndIf

$isAppNotRes=_CloseApp()																	; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_Filter_04")
EndIf

ConsoleWrite("****End : TS_Filter_04.au3****"&@CRLF)
ConsoleWrite(@CRLF)
