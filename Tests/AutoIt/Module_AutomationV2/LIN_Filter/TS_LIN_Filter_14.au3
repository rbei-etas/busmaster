#cs
;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_LIN_Filter_14
; TestCases			:		Deleting filter from filter details list
; Test Data			:
; Test Setup		:		1. Select a filter entry from the filter details list.
							2. Select Delete button.
							3. Check filter details list.
							4. Select Delete all button.
							5. Check for delete confirmation message and select No.
							6. Check filter details list.
							7. Repeat steps 4 and 5 but select Yes to delete conformation message.
							8. Check the list again.
Expected Results  : 		1. After step 3, the selected filter entry will be removed from the list.
							2. After step 5, delete conformations message will be posted to the user.
							3. After step 6, the filter details list will remain unchanged as delete all was canceled.
							4. After step 8, the filter details list will be empty.

#ce
;==========================================================================Test Procedure =========================================================================
ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start :TS_LIN_Filter_14.au3****"&@CRLF)


_launchApp()
WinActivate($WIN_BUSMASTER)

$initialFilterCount=0
$FilterCountAfterDeletingOne=0
$popupExists=0
$noOfFiltersAfterPressingNo=0
$noOfFiltersAfterPressingYes=2


if winexists($WIN_BUSMASTER) then
	_loadConfig("cfxLINFilter_12")
	_SetViewToolBarLIN()
	Sleep(1000)

	_AppFilterMenu_LIN()					                                                                                     ; Select Configure->Log
	if winexists($WIN_Config_filter_LIN) then
		Sleep(1000)
		$hWnd=controlgethandle($WIN_Config_filter_LIN,"",$LVC_FilterList_ConfigFilter)		                                ; Get handle of filter details selection list view
        _GUICtrlListView_ClickItem($hWnd, 0, "left", False, 1)
		Send("{ENTER}")
		$hWndFilterDetails=controlgethandle($WIN_Config_filter_LIN,"",$LVC_FilterDetails_ConfigFilter)		                                ; Get handle of filter details selection list view
		Sleep(200)
        _GUICtrlListView_ClickItem($hWndFilterDetails, 2, "left", False, 1)
		$initialFilterCount=_GUICtrlListView_GetItemCount($hWndFilterDetails)
		consolewrite("$initialFilterCount:"&$initialFilterCount&@CRLF)
        ControlClick($WIN_Config_filter_LIN,"",$BTN_DelFil_ConfigFilter)
		$FilterCountAfterDeletingOne=_GUICtrlListView_GetItemCount($hWndFilterDetails)
		consolewrite("$FilterCountAfterDeletingOne:"&$FilterCountAfterDeletingOne&@CRLF)
		ControlClick($WIN_Config_filter_LIN,"",$BTN_DELAll_Filter)
		sleep(200)
		$popupExists=ControlCommand($WIN_BUSMASTER,"",$TXT_DelAllFilter_BM,"IsVisible")
		ConsoleWrite("$popupExists = "& $popupExists &@CRLF)
		controlclick($WIN_BUSMASTER,"",$BTN_DelAllFilterNO_BM)
		$noOfFiltersAfterPressingNo=_GUICtrlListView_GetItemCount($hWndFilterDetails)
		ConsoleWrite("$noOfFiltersAfterPressingNo = "& $noOfFiltersAfterPressingNo &@CRLF)
		controlclick($WIN_Config_filter_LIN,"",$BTN_DELAll_Filter)
		controlclick($WIN_BUSMASTER,"",$BTN_DelAllFilterYes_BM)
		$noOfFiltersAfterPressingYes=_GUICtrlListView_GetItemCount($hWndFilterDetails)
		ConsoleWrite("$noOfFiltersAfterPressingYes = "& $noOfFiltersAfterPressingYes &@CRLF)


	    ControlClick($WIN_Config_filter_LIN,"",$BTN_OK_ConfigFilter)									                        ; Click on OK button
	EndIf

EndIf


   if ($initialFilterCount=3 and $FilterCountAfterDeletingOne=2 and $popupExists=1 and $noOfFiltersAfterPressingNo=2 and $noOfFiltersAfterPressingYes=0 )then											; Validate the result
  	   _WriteResult("Pass","TS_LIN_Filter_14")
       ConsoleWrite("Pass"&@CRLF)
   Else
   	   _WriteResult("Fail","TS_LIN_Filter_14")
   	   ConsoleWrite("Fail"&@CRLF)
   EndIf



$isAppNotRes=_CloseApp()																		; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_LIN_Filter_14")
EndIf

ConsoleWrite("****End : TS_LIN_Filter_14.au3****"&@CRLF)
ConsoleWrite(@CRLF)


