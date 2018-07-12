#cs
;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_LIN_Filter_15
; TestCases			:		OK and Cancel functionality
; Test Data			:
; Test Setup		:		1. Invoke Filter configuration dialog by selecting LIN->Filters
							2. Note down the filter list
							3. Delete all filters in the filter list.
							4. Select Cancel button.
							5. Invoke the dialog again.
							6. Check the filter list.
							7. Delete all filters in the filter list.
							8. Select OK.
							9. Invoke the dialog again and check the filter list content.
Expected Results  : 		1. After step 6, the filter list will remain as it was at step 2.
							2. After step 9, the filter list will be empty.

#ce
;==========================================================================Test Procedure =========================================================================
ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start :TS_LIN_Filter_15.au3****"&@CRLF)


_launchApp()
WinActivate($WIN_BUSMASTER)

$noOfFiltersAfterDeleting=3
$noOfFiltersAfterPressingCancel=0
$noOfFiltersAfterPressingOK=3

if winexists($WIN_BUSMASTER) then
	_loadConfig("cfxLINFilter_12")
	_SetViewToolBarLIN()
	Sleep(1000)

	_AppFilterMenu_LIN()					                                                                                ; Select Configure->Log
	if winexists($WIN_Config_filter_LIN) then
	    $hWnd=controlgethandle($WIN_Config_filter_LIN,"",$LVC_FilterList_ConfigFilter)		                                ; Get handle of filter list selection list view
        _GUICtrlListView_ClickItem($hWnd, 0, "left", False, 1)
		$hWndFilterDetails=controlgethandle($WIN_Config_filter_LIN,"",$LVC_FilterDetails_ConfigFilter)		                ; Get handle of filter details selection list view
		Sleep(200)
		controlclick($WIN_Config_filter_LIN,"",$BTN_DELAll_Filter)
		controlclick($WIN_BUSMASTER,"",$BTN_DelAllFilterYes_BM)
		$noOfFiltersAfterDeleting=_GUICtrlListView_GetItemCount($hWndFilterDetails)
		ConsoleWrite("$noOfFiltersAfterDeleting = "& $noOfFiltersAfterDeleting &@CRLF)
		ControlClick($WIN_Config_filter_LIN,"",$BTN_Cancel_ConfigFilter)									                ; Click on cancel button
		Sleep(1000)

		_AppFilterMenu_LIN()					                                                                            ; Select Configure->Log
		 $hWnd=controlgethandle($WIN_Config_filter_LIN,"",$LVC_FilterList_ConfigFilter)		                                ; Get handle of filter details selection list view
        _GUICtrlListView_ClickItem($hWnd, 0, "left", False, 1)
		$hWndFilterDetails=controlgethandle($WIN_Config_filter_LIN,"",$LVC_FilterDetails_ConfigFilter)		                ; Get handle of filter details selection list view
		Sleep(200)
		$noOfFiltersAfterPressingCancel=_GUICtrlListView_GetItemCount($hWndFilterDetails)
		ConsoleWrite("$noOfFiltersAfterPressingCancel = "& $noOfFiltersAfterPressingCancel &@CRLF)
		controlclick($WIN_Config_filter_LIN,"",$BTN_DELAll_Filter)
		controlclick($WIN_BUSMASTER,"",$BTN_DelAllFilterYes_BM)
		ControlClick($WIN_Config_filter_LIN,"",$BTN_OK_ConfigFilter)									                    ; Click on cancel button
		_AppFilterMenu_LIN()					                                                                            ; Select Configure->Log

		 $hWnd=controlgethandle($WIN_Config_filter_LIN,"",$LVC_FilterList_ConfigFilter)		                                ; Get handle of filter details selection list view
        _GUICtrlListView_ClickItem($hWnd, 0, "left", False, 1)
		$hWndFilterDetails=controlgethandle($WIN_Config_filter_LIN,"",$LVC_FilterDetails_ConfigFilter)		                ; Get handle of filter details selection list view
		Sleep(200)
		$noOfFiltersAfterPressingOK=_GUICtrlListView_GetItemCount($hWndFilterDetails)
		ConsoleWrite("$noOfFiltersAfterPressingOK = "& $noOfFiltersAfterPressingOK &@CRLF)
		Sleep(1000)
	    ControlClick($WIN_Config_filter_LIN,"",$BTN_OK_ConfigFilter)									                    ; Click on OK button
	EndIf

EndIf


   if ($noOfFiltersAfterDeleting=0 and $noOfFiltersAfterPressingCancel=3 and $noOfFiltersAfterPressingOK=0 )then											; Validate the result
		_WriteResult("Pass","TS_LIN_Filter_15")
        ConsoleWrite("Pass"&@CRLF)
   Else
		_WriteResult("Fail","TS_LIN_Filter_15")
		ConsoleWrite("Fail"&@CRLF)
   EndIf



$isAppNotRes=_CloseApp()																		; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_LIN_Filter_15")
EndIf

ConsoleWrite("****End : TS_LIN_Filter_15.au3****"&@CRLF)
ConsoleWrite(@CRLF)


