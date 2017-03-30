#cs
;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_LIN_Filter_16
; TestCases			:		Check for UnDefined message
; Test Data			:
; Test Setup		:		1. Invoke dialog box for filter configuration via LIN->Filters
							2. Enter ID of an undefined message not occuring in the filter list
							3. Select Add button"
Expected Results  : 		1. Filter list should be updated

#ce
;==========================================================================Test Procedure =========================================================================
ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start :TS_LIN_Filter_16.au3****"&@CRLF)
$addedFilterDetails=""

_launchApp()
WinActivate($WIN_BUSMASTER)

$noOfFiltersAfterDeleting=3
$noOfFiltersAfterPressingCancel=0
$noOfFiltersAfterPressingOK=3

if winexists($WIN_BUSMASTER) then
	_loadConfig("cfxLINFilter_12")                                                                ; Load the existing configuration
	_SetViewToolBarLIN()
	Sleep(1000)

	_AppFilterMenu_LIN()					                                                      ; Select Configure->Log
	if winexists($WIN_Config_filter_LIN) then
		Sleep(1000)
	    $hWnd=controlgethandle($WIN_Config_filter_LIN,"",$LVC_FilterDetails_ConfigFilter)		  ; Get handle of filter details selection list view
		_GUICtrlListView_ClickItem($hWnd, 0, "left", False, 2)                                    ; Select the first item from the filter detail list
		Sleep(1000)
		ControlClick($WIN_Config_filter_LIN,"",$Edit_ID_ConfigFilter,"Left",1)                    ; Click on the msd id edit box
		Send("11")                                                                                ; Enter the msd id
		send("{ENTER}")
		$addedFilterDetails=_GUICtrlListView_GetItemTextString($hWnd,0)                           ; Get the content from the first row in filter detail list
		consolewrite("$addedFilterDetails:"&$addedFilterDetails&@CRLF)
		ControlClick($WIN_Config_filter_LIN,"",$BTN_OK_ConfigFilter)							  ; Click on OK button
	EndIf

EndIf

   if ($addedFilterDetails="11||||Rx|All|" )then											          ; Validate the result
		_WriteResult("Pass","TS_LIN_Filter_16")
        ConsoleWrite("Pass"&@CRLF)
   Else
		_WriteResult("Fail","TS_LIN_Filter_16")
		ConsoleWrite("Fail"&@CRLF)
   EndIf



$isAppNotRes=_CloseApp()																		; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_LIN_Filter_16")
EndIf

ConsoleWrite("****End : TS_LIN_Filter_16.au3****"&@CRLF)
ConsoleWrite(@CRLF)


