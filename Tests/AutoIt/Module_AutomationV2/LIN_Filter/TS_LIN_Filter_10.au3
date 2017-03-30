#cs
;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_LIN_Filter_10
; TestCases			:		Invoking Filter Dialog
; Test Data			:
; Test Setup		:		1. Continue from the previous test case.
							2. Select 'ID' combo box from filter attribute
							3. Select a DB message name from the Combo box.
							4. Select Add button

Expected Results  : 		1. The combo box will list all database message IDs. And will also allow the user to enter ID manually
							2. After step 4, a filter will be added to the list with the selected message name

#ce
;==========================================================================Test Procedure =========================================================================
ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start :TS_LIN_Filter_10.au3****"&@CRLF)


_launchApp()
WinActivate($WIN_BUSMASTER)

$filterCount=0
$filterDetailsCount=11
$DbMessagesExistsinCombibox=0

if winexists($WIN_BUSMASTER) then
	_createConfig("cfxLINFilter_10")
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
		ControlCommand($WIN_ClustrConfigLIN, "",$chckBoxEnableMaster, "Check", "") 					                             ;Select Check box for enable Master mode.
		Sleep(1000)
		ControlClick($WIN_ClusterConfiguration,"",$BTN_OK_ClusterConfig)
	EndIf

	_AppFilterMenu_LIN()					                                                                                     ;Select Configure->Log
	if winexists($WIN_Config_filter_LIN) then
		ControlClick($WIN_Config_filter,"",$BTN_AddFilter_ConfigFilter)							                                 ;Click on 'Add' filter button
		$hWnd=controlgethandle($WIN_Config_filter_LIN,"",$LVC_FilterList_ConfigFilter)		                                     ;Get handle of filter list view
		$filterCount=_GUICtrlListView_GetItemCount($hWnd)																		 ;Get the number of filter added
		consolewrite("$filterCount:"&$filterCount&@CRLF)

		$hWndIDCombobox=controlgethandle($WIN_Config_filter_LIN,"",$COMB_MsgID_ConfigFilter)		                             ;Get handle of id combobox
		$messageList=_GUICtrlComboBox_GetListArray($hWndIDCombobox)																 ;Get all the list present under the message id combo box
		consolewrite("$messageList[0]:"&$messageList[0]&@CRLF)                                                                   ;Validating each item
		if($messageList[0]=4) then
			consolewrite("$messageList[1]:"&$messageList[1]&@CRLF)
			consolewrite("$messageList[2]:"&$messageList[2]&@CRLF)
			consolewrite("$messageList[3]:"&$messageList[3]&@CRLF)
			consolewrite("$messageList[4]:"&$messageList[4]&@CRLF)
			if($messageList[1]="[0x1]Frame1" And $messageList[2]="[0x2]Frame2" And $messageList[3]="[0x3c]MasterReq" And $messageList[4]="[0x3d]SlaveResp") then
				$DbMessagesExistsinCombibox=1
			EndIf
	    EndIf
		ControlSetText($WIN_Config_filter,"",$Edit_ID_ConfigFilter,"44")					                                     ;Enter the  ID
		send("{ENTER}")
		sleep(1000)
		$hWndID=controlgethandle($WIN_Config_filter_LIN,"",$Edit_ID_ConfigFilter)		                                         ;Get handle of ID edit box
		$IdObtained=_GUICtrlEdit_GetText($hWndID)                                                                                ;Getting the value inside the ID esit box to check whether it allows to enter ID manually
        consolewrite("$IdObtained : "&$IdObtained&@CRLF)


		ControlCommand($WIN_Config_filter,"",$COMB_MsgID_ConfigFilter,"SelectString","[0x1]Frame1")	                             ;Select the MSG from ID combo box
        ControlClick($WIN_Config_filter,"",$BTN_Add_ConfigFilter)									                             ;Click on Add button
		$hWnd1=controlgethandle($WIN_Config_filter_LIN,"",$LVC_FilterDetails_ConfigFilter)		                                 ;Get handle of filter details selection list view
		$filterDetailsCount=_GUICtrlListView_GetItemCount($hWnd1)																 ;Get the total number of filter details added
		consolewrite("$filterDetailsCount:"&$filterDetailsCount&@CRLF)
		$addedFilterDetails=_GUICtrlListView_GetItemTextString($hWnd1,0)                                                         ;Get the details of the added filter details in the first row
		consolewrite("$addedFilterDetails:"&$addedFilterDetails&@CRLF)


		ControlCommand($WIN_Config_filter,"",$RBTN_Range_FilterSelect,"Check")													 ;Select the Range radio button
		sleep(200)
		ControlSetText($WIN_Config_filter,"",$EDIT_From_FilterSelect,"5")													     ;Enter the From ID
		sleep(200)
		ControlSetText($WIN_Config_filter,"",$EDIT_To_FilterSelect,"8")														    ; Enter the To ID
		ControlClick($WIN_Config_filter,"",$BTN_Add_ConfigFilter)									                            ; Click on Add button
		$hWnd1=controlgethandle($WIN_Config_filter_LIN,"",$LVC_FilterDetails_ConfigFilter)		                                ; Get handle of filter details selection list view
		$filterDetailsCountAfterAddingRange=_GUICtrlListView_GetItemCount($hWnd1)
		consolewrite("$filterDetailsCountAfterAddingRange:"&$filterDetailsCountAfterAddingRange&@CRLF)
		$addedFilterDetailsForRange=_GUICtrlListView_GetItemTextString($hWnd1,1)
		consolewrite("$addedFilterDetailsForRange:"&$addedFilterDetailsForRange&@CRLF)

		ControlClick($WIN_Config_filter_LIN,"",$BTN_OK_ConfigFilter)									                        ; Click on OK button
	EndIf

EndIf

consolewrite("$filterCount:"&$filterCount&@CRLF)
consolewrite("$DbMessagesExistsinCombibox:"&$DbMessagesExistsinCombibox&@CRLF)
if $filterCount=1 and $DbMessagesExistsinCombibox=1 and $filterDetailsCount=1 and $addedFilterDetails="1 [Frame1]||||Rx|All|"  and $addedFilterDetailsForRange="5|8|||Rx|All|" and $IdObtained="44" and $filterDetailsCountAfterAddingRange=2 Then												; Validate the result
	_WriteResult("Pass","TS_LIN_Filter_10")
	ConsoleWrite("Pass"&@CRLF)
Else
	_WriteResult("Fail","TS_LIN_Filter_10")
	ConsoleWrite("Fail"&@CRLF)
EndIf



$isAppNotRes=_CloseApp()																		; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_LIN_Filter_10")
EndIf

ConsoleWrite("****End : TS_LIN_Filter_10****"&@CRLF)
ConsoleWrite(@CRLF)


