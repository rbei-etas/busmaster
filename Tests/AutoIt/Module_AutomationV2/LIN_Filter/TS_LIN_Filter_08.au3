#cs
;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_LIN_Filter_08
; TestCases			:		Invoking Filter Dialog
; Test Data			:
; Test Setup		:		1. Start the application
							2. Select New Configuration
							3. Associate Database.
							4. Select LIN->Filters
							5. Check for Filter Configuration dialog
							6. In filter configuration dialog, check following fields
								Filter List
								Filter details list and
								Filter attributes
							7. Check the content of the filter list, Filter details list and filter attributes.

Expected Results  : 		1. A dialog with "Configure Filter list" will be shown
							2.After step6,The listed components will be present in the dialog
							3.After step7, Filter list will be empty, filter details list will be empty and filter attributes components will be disabled

#ce
;==========================================================================Test Procedure =========================================================================
ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start :TS_LIN_Filter_08.au3****"&@CRLF)


_launchApp()
WinActivate($WIN_BUSMASTER)

$filterDialogueExists=0
$filterListExists=0
$filterDetailListExists=0
$filterAttributesExists=0
$filterCount=11
$filterDetailsCount=11
$filterAttributesComponentsDisabled=0

if winexists($WIN_BUSMASTER) then
	_createConfig("cfxLINFilter_08")
	_SetViewToolBarLIN()
	Sleep(1000)
	;_openLINClusterConfig()
	Send("{ALT}")
	Send("LCC")
	WinWaitActive($WIN_ClustrConfigLIN)
	if winexists($WIN_ClustrConfigLIN) then
		sleep(1000)
		_importLDF("TestLIN2_1.ldf")
		sleep(1000)
		ControlCommand($WIN_ClustrConfigLIN, "",$chckBoxEnableMaster, "Check", "") 					                              ; Select Check box for enable Master mode.
		Sleep(1000)
		ControlClick($WIN_ClusterConfiguration,"",$BTN_OK_ClusterConfig)
	EndIf

	_AppFilterMenu_LIN()					       ; Select Configure->Log
	if winexists($WIN_Config_filter_LIN) then
		$filterDialogueExists=1
		$filterListExists=ControlCommand($WIN_Config_filter_LIN,"",$LVC_FilterList_ConfigFilter,"IsVisible") 				     ;Checking whether filter list exists
		$filterDetailListExists=ControlCommand($WIN_Config_filter_LIN,"",$LVC_FilterDetails_ConfigFilter,"IsVisible") 			 ;Checking whether filter details list list exists
		$filterAttributesExists=ControlCommand($WIN_Config_filter_LIN,"",$FilterListLIN,"IsVisible") 						     ;Checking whether filter attributes list exists

		$idCheckbox=ControlCommand($WIN_Config_filter_LIN,"",$RBTN_ID_FilterSelect,"IsEnabled") 						         ;Status of the id radio button
		$rangeCheckbox =ControlCommand($WIN_Config_filter_LIN,"",$RBTN_Range_FilterSelect,"IsEnabled") 						     ;Status of the range radio button
		$eventCheckbox=ControlCommand($WIN_Config_filter_LIN,"",$RBTN_Event_FilterSelect,"IsEnabled") 						     ;Status of the event radio button
		$direction=ControlCommand($WIN_Config_filter_LIN,"",$COMB_Direction_ConfigFilter,"IsEnabled") 						     ;Status of the direction combobox
		$channel=ControlCommand($WIN_Config_filter_LIN,"",$COMB_Channel_ConfigFilter,"IsEnabled") 						         ;Status of the channel combobox
		$addButton=ControlCommand($WIN_Config_filter_LIN,"",$BTN_Add_ConfigFilter,"IsEnabled") 						             ;Status of the add to filter details button
        consolewrite("$idCheckbox:"&$idCheckbox&@CRLF)
		consolewrite("$rangeCheckbox:"&$rangeCheckbox&@CRLF)
		consolewrite("$eventCheckbox:"&$eventCheckbox&@CRLF)
		consolewrite("$direction:"&$direction&@CRLF)
		consolewrite("$channel:"&$channel&@CRLF)
		consolewrite("$addButton:"&$addButton&@CRLF)
		if($idCheckbox=0 and $rangeCheckbox=0 and $eventCheckbox=0 and $direction=0 and $channel=0 and $addButton=0 ) Then
			$filterAttributesComponentsDisabled=1
		EndIf

		$hWnd=controlgethandle($WIN_Config_filter_LIN,"",$LVC_FilterList_ConfigFilter)		                                    ; Get handle of filter selection list view
		$filterCount=_GUICtrlListView_GetItemCount($hWnd)
        $hWnd1=controlgethandle($WIN_Config_filter_LIN,"",$LVC_FilterDetails_ConfigFilter)		                                ; Get handle of filter details selection list view
		$filterDetailsCount=_GUICtrlListView_GetItemCount($hWnd1)
		consolewrite("$filterCount:"&$filterCount&@CRLF)
		consolewrite("$filterDetailsCount:"&$filterDetailsCount&@CRLF)
        ControlClick($WIN_Config_filter_LIN,"",$BTN_OK_ConfigFilter)									                        ; Click on OK button
	EndIf

EndIf

consolewrite("$filterDialogueExists:"&$filterDialogueExists&@CRLF)
consolewrite("$filterListExists:"&$filterListExists&@CRLF)
consolewrite("$filterDetailListExists:"&$filterDetailListExists&@CRLF)
consolewrite("$filterAttributesExists:"&$filterAttributesExists&@CRLF)
consolewrite("$filterAttributesComponentsDisabled:"&$filterAttributesComponentsDisabled&@CRLF)

if $filterDialogueExists=1 and $filterListExists=1 and $filterDetailListExists=1 and $filterAttributesComponentsDisabled=1 and $filterCount=0 and $filterDetailsCount=0 Then												; Validate the result
	_WriteResult("Pass","TS_LIN_Filter_08")
	ConsoleWrite("Pass"&@CRLF)
Else
	_WriteResult("Fail","TS_LIN_Filter_08")
	ConsoleWrite("Fail"&@CRLF)
EndIf



$isAppNotRes=_CloseApp()																		; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_LIN_Filter_08")
EndIf

ConsoleWrite("****End : TS_LIN_Filter_08.au3****"&@CRLF)
ConsoleWrite(@CRLF)


