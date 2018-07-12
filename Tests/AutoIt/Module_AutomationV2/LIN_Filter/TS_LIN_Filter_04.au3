#cs
;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_LIN_Filter_04
; TestCases			:		Check for effectiveness - [A message ID range entered in the filter list ]
; Test Data			:
; Test Setup		:		1."Invoke dialog box for filter configuration"
							2. Add Filter in filter list.
							3. double click on the second column item.
							4. A combo box with items Stop and pass will appear.

Expected Results  : 		Stop and Pass filter should be available in the combo box

#ce
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start :TS_LIN_Filter_04.au3****"&@CRLF)


Local $FilterTypeTxt1="",$FilterTypeTxt2=""

_launchApp()

WinActivate($WIN_BUSMASTER)

if winexists($WIN_BUSMASTER) then

	_createConfig("cfxLINFilter_04")																	; Create New Configuration
	_SetViewToolBarLIN()																				; Select LIN Tool Bar to view.
	_linDriverSelection()

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

consolewrite("$FilterTypeTxt1:"&$FilterTypeTxt1&@CRLF)
consolewrite("$FilterTypeTxt2:"&$FilterTypeTxt2&@CRLF)

if $FilterTypeTxt1="Stop" and  $FilterTypeTxt2="Pass" Then									; Validate the result
	_WriteResult("Pass","TS_LIN_Filter_04")
	ConsoleWrite("Pass"&@CRLF)
Else
	_WriteResult("Fail","TS_LIN_Filter_04")
	ConsoleWrite("Fail"&@CRLF)
EndIf



$isAppNotRes=_CloseApp()																		; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_LIN_Filter_04")
EndIf

ConsoleWrite("****End :TS_LIN_Filter_04.au3****"&@CRLF)
ConsoleWrite(@CRLF)


