#cs
;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_LIN_Filter_18
; TestCases			:		Provision to enter message ID for filtering
; Test Data			:
; Test Setup		:		1. Invoke dialog box for filter configuration via LIN->Filters
							2. Update the filter list by entering a few message IDs
							3. Confirm by selecting OK button
							4. Save the current configuration
							5. Exit Busmaster
							6. Start Busmaster again
							7. Load the saved configuration
							8. Invoke dialog box for filter configuration via LIN->Filters and check the filter list."
Expected Results  : 		"Filter list should be the same"

#ce
;==========================================================================Test Procedure =========================================================================
ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start :TS_LIN_Filter_18.au3****"&@CRLF)
$addedFilterDetails=""

_launchApp()
WinActivate($WIN_BUSMASTER)

$addedFilterDetails1=""
$addedFilterDetails2=""
$addedFilterDetails3=""
$addedFilterDetails1AfterSaving=""
$addedFilterDetails2AfterSaving=""
$addedFilterDetails3AfterSaving=""
$fromIdExists=0
$toIDExists=0
$toIdObtained=""
$fromIdObtained=""
$retainedConfig=0
$idRangeAcceptOnlyHex=0

if winexists($WIN_BUSMASTER) then
	Sleep(1000)
	_createConfig("cfxLINFilter_18")															  ; Creating a new configuration
	_SetViewToolBarLIN()
	Sleep(3000)
    Send("{ENTER}")
	;_AppFilterMenu_LIN()					                                                      ; Select Configure->Log
	Send("{ALT}")
	Send("LFT")
	Sleep(1000)
	if winexists($WIN_Config_filter_LIN) then
		Sleep(1000)
		ControlClick($WIN_Config_filter,"",$BTN_AddFilter_ConfigFilter)							  ; Click on 'Add' filter button
		Sleep(1000)
	    $hWnd=controlgethandle($WIN_Config_filter_LIN,"",$LVC_FilterDetails_ConfigFilter)		  ; Get handle of filter details selection list view
		$hWndFilterList=controlgethandle($WIN_Config_filter_LIN,"",$LVC_FilterList_ConfigFilter)  ; Get handle of filter list selection list view

        _GUICtrlListView_ClickItem($hWndFilterList, 0, "left", False, 1)
		Send("{ENTER}")
		$colWidth=_GUICtrlListView_GetColumnWidth($hWndFilterList,0)
		$pos=_GUICtrlListView_GetItemPosition($hWndFilterList,0)
		consolewrite("$colWidth:"&$colWidth&@CRLF)
		consolewrite("$pos:"&$pos&@CRLF)
		ControlClick($WIN_Config_filter_LIN,"",$hWndFilterList,"Left",2,$colWidth-10,$pos[1])
		Send("NewName")
		ControlClick($WIN_Config_filter_LIN,"",$hWndFilterList,"Left",2,$colWidth+10,$pos[1])
		Sleep(1000)
		Send("{DOWN}")
		Send("{ENTER}")
		$filterDetails=_GUICtrlListView_GetItemTextString($hWndFilterList,0)
		consolewrite("$filterDetails:"&$filterDetails&@CRLF)

		ControlSetText($WIN_Config_filter,"",$Edit_ID_ConfigFilter,"11")					      ; Enter the  ID
		controlClick($WIN_Config_filter,"",$BTN_Add_ConfigFilter)								  ; Click on Add button
		ControlSetText($WIN_Config_filter,"",$Edit_ID_ConfigFilter,"12")					      ; Enter the  ID
		controlClick($WIN_Config_filter,"",$BTN_Add_ConfigFilter)								  ; Click on Add button
		ControlSetText($WIN_Config_filter,"",$Edit_ID_ConfigFilter,"13")					      ; Enter the  ID
		controlClick($WIN_Config_filter,"",$BTN_Add_ConfigFilter)								  ; Click on Add button
		Sleep(5000)
		$addedFilterDetails1=_GUICtrlListView_GetItemTextString($hWnd,0)                          ; Get the content from the first row in filter detail list
		consolewrite("$addedFilterDetails1:"&$addedFilterDetails1&@CRLF)
		$addedFilterDetails2=_GUICtrlListView_GetItemTextString($hWnd,1)                          ; Get the content from the second row in filter detail list
		consolewrite("$addedFilterDetails2:"&$addedFilterDetails2&@CRLF)
		$addedFilterDetails3=_GUICtrlListView_GetItemTextString($hWnd,2)                          ; Get the content from the third row in filter detail list
		consolewrite("$addedFilterDetails3:"&$addedFilterDetails3&@CRLF)
		ControlClick($WIN_Config_filter_LIN,"",$BTN_OK_ConfigFilter)							  ; Click on OK button
	EndIf
	_saveConfig()
EndIf
_CloseApp()

Sleep(2000)

_launchApp()																					   ; Launching application again
WinActivate($WIN_BUSMASTER)
if winexists($WIN_BUSMASTER) then
_AppFilterMenu_LIN()					                                                           ; Select Configure->Log
	if winexists($WIN_Config_filter_LIN) then
		$hWnd=controlgethandle($WIN_Config_filter_LIN,"",$LVC_FilterDetails_ConfigFilter)		   ; Get handle of filter details selection list view
		$addedFilterDetails1AfterSaving=_GUICtrlListView_GetItemTextString($hWnd,0)                ; Get the content from the first row in filter detail list
		consolewrite("$addedFilterDetails1AfterSaving:"&$addedFilterDetails1AfterSaving&@CRLF)
		$addedFilterDetails2AfterSaving=_GUICtrlListView_GetItemTextString($hWnd,1)                ; Get the content from the second row in filter detail list
		consolewrite("$addedFilterDetails2AfterSaving:"&$addedFilterDetails2AfterSaving&@CRLF)
		$addedFilterDetails3AfterSaving=_GUICtrlListView_GetItemTextString($hWnd,2)                ; Get the content from the third row in filter detail list
		consolewrite("$addedFilterDetails3AfterSaving:"&$addedFilterDetails3AfterSaving&@CRLF)
		_GUICtrlListView_ClickItem($hWnd, 0, "left", False, 2)                                     ; Select the first item from the filter detail list
		$fromIdExists=ControlCommand($WIN_Config_filter_LIN,"",$TXT__To_FilterSelect,"IsVisible")
		$toIDExists=ControlCommand($WIN_Config_filter_LIN,"",$RBTN_Range_FilterSelect,"IsVisible")
		consolewrite("$fromIdExists:"&$fromIdExists&@CRLF)
		consolewrite("$toIDExists:"&$toIDExists&@CRLF)
		ControlCommand($WIN_Config_filter,"",$RBTN_Range_FilterSelect,"Check")				    	; Select the Range radio button
		sleep(200)
		ControlClick($WIN_Config_filter_LIN,"",$EDIT_From_FilterSelect,"Left",1)                    ; Click on the msd id edit box
		Send( "{BACKSPACE}" ) 																		; Remove the content of from id edit box
		Send( "{BACKSPACE}" )
		Send("ALW")                                                                                 ; Enter the msd id
		send("{ENTER}")
		ControlClick($WIN_Config_filter_LIN,"",$EDIT_To_FilterSelect,"Left",1)                      ; Click on the msd id edit box																								 ; Remove the content of msg id edit box
		Send( "{BACKSPACE}" ) 																		; Remove the content of to id edit box
		Send( "{BACKSPACE}" )
		Send("E!Y")                                                                                 ; Enter the msd id
		send("{ENTER}")
        $hWndFromID=controlgethandle($WIN_Config_filter_LIN,"",$EDIT_From_FilterSelect)		        ; Get handle of from ID edit box
		$fromIdObtained=_GUICtrlEdit_GetText($hWndFromID)                                           ; Get the value present in from ID edit box
		$hWndToID=controlgethandle($WIN_Config_filter_LIN,"",$EDIT_To_FilterSelect)		            ; Get handle of to ID edit box
		$toIdObtained=_GUICtrlEdit_GetText($hWndToID)                                               ; Get the value present in to ID edit box
		consolewrite("$fromIdObtained:"&$fromIdObtained&@CRLF)
		consolewrite("$toIdObtained:"&$toIdObtained&@CRLF)
		Sleep(1000)

		$hWndFilterList=controlgethandle($WIN_Config_filter_LIN,"",$LVC_FilterList_ConfigFilter)    ; Get handle of filter list selection list view
		$filterDetailsAfterSaving=_GUICtrlListView_GetItemTextString($hWndFilterList,0)
		consolewrite("$filterDetailsAfterSaving:"&$filterDetailsAfterSaving&@CRLF)

		ControlClick($WIN_Config_filter_LIN,"",$BTN_OK_ConfigFilter)							   ; Click on OK button
	EndIf
EndIf

 if ($addedFilterDetails1AfterSaving=$addedFilterDetails1 and $addedFilterDetails2AfterSaving=$addedFilterDetails2 and $addedFilterDetails3AfterSaving=$addedFilterDetails3)then
	 $retainedConfig=1
 EndIf
if ($fromIdObtained="A" and $toIdObtained="E" and $fromIdExists=1 and $toIDExists=1 )then
	 $idRangeAcceptOnlyHex=1
 EndIf

	consolewrite("$retainedConfig:"&$retainedConfig&@CRLF)
	consolewrite("$idRangeAcceptOnlyHex:"&$idRangeAcceptOnlyHex&@CRLF)

   if ($retainedConfig=1 and $idRangeAcceptOnlyHex=1 and $filterDetails=$filterDetailsAfterSaving)then		; Validate the result
		_WriteResult("Pass","TS_LIN_Filter_18")
        ConsoleWrite("Pass"&@CRLF)
   Else
		_WriteResult("Fail","TS_LIN_Filter_18")
		ConsoleWrite("Fail"&@CRLF)
   EndIf

$isAppNotRes=_CloseApp()																		   ; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_LIN_Filter_18")
EndIf

ConsoleWrite("****End : TS_LIN_Filter_18.au3****"&@CRLF)
ConsoleWrite(@CRLF)
















