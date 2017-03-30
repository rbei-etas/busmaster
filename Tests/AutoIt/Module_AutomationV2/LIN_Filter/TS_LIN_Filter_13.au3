#cs
;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_LIN_Filter_13
; TestCases			:		Filter Name and type
; Test Data			:
; Test Setup		:		1. Select a filter from the filter list.
							2. Double click the Name column.
							3. Edit the name and select enter.
							4. Check the name of the filter.
							5. Double click the Type column.
							6. Select an entry from the drop down list.
							7. Check the filter type.
Expected Results  : 		1. After step 4, the modified name will be displayed in the list.
							2. The filter type will be modified to the user selected value.

#ce
;==========================================================================Test Procedure =========================================================================
ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start :TS_LIN_Filter_13.au3****"&@CRLF)


_launchApp()
WinActivate($WIN_BUSMASTER)
$filterDetailsBeforeChanging=""
$filterDetails=""


if winexists($WIN_BUSMASTER) then
	_loadConfig("cfxLINFilter_12")
	_SetViewToolBarLIN()
	Sleep(1000)

	_AppFilterMenu_LIN()					                                                                                     ; Select Configure->Log
	if winexists($WIN_Config_filter_LIN) then
		Sleep(1000)
		$hWnd=controlgethandle($WIN_Config_filter_LIN,"",$LVC_FilterList_ConfigFilter)		                                ; Get handle of filter details selection list view
		$filterDetailsBeforeChanging=_GUICtrlListView_GetItemTextString($hWnd,1)
		consolewrite("$filterDetailsBeforeChanging:"&$filterDetailsBeforeChanging&@CRLF)
        _GUICtrlListView_ClickItem($hWnd, 1, "left", False, 1)
		Send("{ENTER}")
		$colWidth=_GUICtrlListView_GetColumnWidth($hWnd,0)
		$pos=_GUICtrlListView_GetItemPosition($hWnd,1)
		ControlClick($WIN_Config_filter_LIN,"",$hWnd,"Left",2,$colWidth-10,$pos[1])
		Send("NewName")
		ControlClick($WIN_Config_filter_LIN,"",$hWnd,"Left",2,$colWidth+10,$pos[1])
		Sleep(1000)
		Send("{DOWN}")
		Send("{ENTER}")
		$filterDetails=_GUICtrlListView_GetItemTextString($hWnd,1)
		consolewrite("$filterDetails:"&$filterDetails&@CRLF)
	    ControlClick($WIN_Config_filter_LIN,"",$BTN_OK_ConfigFilter)									                        ; Click on OK button
	EndIf

EndIf



  if ($filterDetailsBeforeChanging="Filter_2|Stop" and $filterDetails="NewName|Pass")then											; Validate the result
 	 _WriteResult("Pass","TS_LIN_Filter_13")
      ConsoleWrite("Pass"&@CRLF)
  Else
  	_WriteResult("Fail","TS_LIN_Filter_13")
  	ConsoleWrite("Fail"&@CRLF)
  EndIf



$isAppNotRes=_CloseApp()																		; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_LIN_Filter_13")
EndIf

ConsoleWrite("****End : TS_LIN_Filter_13.au3****"&@CRLF)
ConsoleWrite(@CRLF)


