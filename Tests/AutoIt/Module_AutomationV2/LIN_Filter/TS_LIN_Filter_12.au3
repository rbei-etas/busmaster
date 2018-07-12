#cs
;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_LIN_Filter_12
; TestCases			:		Invoking Filter Dialog
; Test Data			:
; Test Setup		:		1. Continue from the previous test case.
							2. Select a filter entry from the filter details list.
							3. Change the following attributes.
								ID/Name, Range, ID type, Frame, Direction and channel
Expected Results  : 		1. Selected filter will be updated if the given values are correct.
							2. If given values are wrong then appropriate error message will be displayed in the status bar and the selected message will not be updated

#ce
;==========================================================================Test Procedure =========================================================================
ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start :TS_LIN_Filter_12.au3****"&@CRLF)


_launchApp()
WinActivate($WIN_BUSMASTER)

$filterCount=0
$filterDetailsCount=11
$DbMessagesExistsinCombibox=0
$error1=0
$error2=0
$error3=0
$addedFilterDetails1=""
$addedFilterDetails2=""
$addedFilterDetails1AfterChangingValue=""
$addedFilterDetails2AfterChangingValue=""
$addedFilterDetails3AfterChangingValue=""
$errorMessageDisplayed=0
$filterDetailsChanged=0
$filterNotChangedForWrongValue=0

if winexists($WIN_BUSMASTER) then
	_loadConfig("cfxLINFilter_12")
	_SetViewToolBarLIN()
	Sleep(1000)

	_AppFilterMenu_LIN()					                                                                                     ; Select Configure->Log
	if winexists($WIN_Config_filter_LIN) then
		Sleep(1000)
	    $hWnd=controlgethandle($WIN_Config_filter_LIN,"",$LVC_FilterDetails_ConfigFilter)		                                ; Get handle of filter details selection list view
        _GUICtrlListView_ClickItem($hWnd, 0, "left", False, 1)                                                                  ; Click on the first item
		Send("{ENTER}")
		ControlCommand($WIN_Config_filter_LIN,"",$COMB_MsgID_ConfigFilter,"SelectString","[0x2]Frame2")	                        ; Select the MSG from ID combo box
		ControlCommand($WIN_Config_filter_LIN,"",$COMB_Direction_ConfigFilter,"SelectString", "Tx")		                        ; Select the Direction
		sleep(200)
		ControlCommand($WIN_Config_filter_LIN,"",$COMB_Channel_ConfigFilter,"SelectString", "1")	                            ; Select the Channel
		sleep(200)
	     _GUICtrlListView_ClickItem($hWnd, 1, "left", False, 1)                                                                 ; Select the second item from filter detail list
		ControlSetText($WIN_Config_filter_LIN,"",$EDIT_From_FilterSelect,"2")					                                ; Enter the From ID
		sleep(200)
		ControlSetText($WIN_Config_filter_LIN,"",$EDIT_To_FilterSelect,"4")						                                ; Enter the To ID
		$hWnd1=controlgethandle($WIN_Config_filter_LIN,"",$LVC_FilterDetails_ConfigFilter)		                                ; Get handle of filter details selection list view
		$addedFilterDetails1=_GUICtrlListView_GetItemTextString($hWnd1,0)                                                       ; Get the content from the first row in filter detail
		$hWnd1=controlgethandle($WIN_Config_filter_LIN,"",$LVC_FilterDetails_ConfigFilter)		                                ; Get handle of filter details selection list view
		$addedFilterDetails2=_GUICtrlListView_GetItemTextString($hWnd1,1)                                                       ; Get the content from the second row in filter detail
		consolewrite("$addedFilterDetails1:"&$addedFilterDetails1&@CRLF)
		consolewrite("$addedFilterDetails2:"&$addedFilterDetails2&@CRLF)
		if($addedFilterDetails1="2 [Frame2]||||Tx|1|" and $addedFilterDetails2="2|4|||Rx|All|" )Then
			$filterDetailsChanged=1
		EndIf


		_GUICtrlListView_ClickItem($hWnd, 0, "left", False, 1)                                                                   ; Select the first item from filter detail list
		ControlClick ( $WIN_Config_filter_LIN, "", $Edit_ID_ConfigFilter ,"left",1)												 ; Click on the msg id edit box
		Send("666")                                                                                                              ; Enter the invalid value
		sleep(1000)
        $error1=_CheckErrorMessageLINFilter("ValidIDRange")                                                                      ; Checking whether the error msg show is proper and the add button is disabled
        consolewrite("$error1:"&$error1&@CRLF)
		$addedFilterDetails1AfterChangingValue=_GUICtrlListView_GetItemTextString($hWnd1,0)                                      ; Get the details of the added filter details in the first row

		_GUICtrlListView_ClickItem($hWnd, 0, "left", False, 1)                                                                   ; Select the first item from filter detail list
		ControlClick ( $WIN_Config_filter_LIN, "", $Edit_ID_ConfigFilter ,"left",1)
		Sleep(1000)
		Send( "{BACKSPACE}" ) 																									 ; Remove the content of msg id edit box
		Send( "{BACKSPACE}" )
		Send( "{BACKSPACE}" )
		Send("-71")                                                                                                              ; Send the invalid value
		sleep(1000)
        $error2=_CheckErrorMessageLINFilter("InvalidID")                                                                         ; Checking whether the error msg show is proper and the add button is disabled
        consolewrite("$error2:"&$error2&@CRLF)
		$addedFilterDetails2AfterChangingValue=_GUICtrlListView_GetItemTextString($hWnd1,0)                                      ; Get the details of the added filter details in the first row

        $hWnd=controlgethandle($WIN_Config_filter_LIN,"",$LVC_FilterDetails_ConfigFilter)		                                 ; Get handle of filter details selection list view
		_GUICtrlListView_ClickItem($hWnd, 1, "left", False, 1)                                                                   ; Select the second item from filter detail list
		Sleep(3000)
		ControlSetText($WIN_Config_filter_LIN,"",$EDIT_From_FilterSelect,"8")  					                                 ; Enter the From ID
        $error3=_CheckErrorMessageLINFilter("MinGreaterThanMax")                                                                 ; Checking whether the error msg show is proper and the add button is disabled
        consolewrite("$error3:"&$error3&@CRLF)
        $addedFilterDetails3AfterChangingValue=_GUICtrlListView_GetItemTextString($hWnd1,1)                                      ; Get the details of the added filter details in the second row
		consolewrite("$addedFilterDetails1AfterChangingValue:"&$addedFilterDetails1AfterChangingValue&@CRLF)
		consolewrite("$addedFilterDetails2AfterChangingValue:"&$addedFilterDetails2AfterChangingValue&@CRLF)
		consolewrite("$addedFilterDetails3AfterChangingValue:"&$addedFilterDetails3AfterChangingValue&@CRLF)
	    ControlClick($WIN_Config_filter_LIN,"",$BTN_OK_ConfigFilter)									                         ; Click on OK button

		if($error1=1 and $error2=1 and $error3=1)Then
			$errorMessageDisplayed=1
		EndIf

		if($addedFilterDetails1AfterChangingValue="6||||Tx|1|" and $addedFilterDetails2AfterChangingValue="6||||Tx|1|" and $addedFilterDetails3AfterChangingValue="2|4|||Rx|All|") Then
			$filterNotChangedForWrongValue=1
		EndIf



	EndIf

EndIf

consolewrite("$filterDetailsChanged:"&$filterDetailsChanged&@CRLF)
consolewrite("$errorMessageDisplayed:"&$errorMessageDisplayed&@CRLF)
consolewrite("$filterNotChangedForWrongValue:"&$filterNotChangedForWrongValue&@CRLF)

 if ($filterDetailsChanged=1 and $errorMessageDisplayed=1 and $filterNotChangedForWrongValue=1 )then											; Validate the result
 	_WriteResult("Pass","TS_LIN_Filter_12")
 	ConsoleWrite("Pass"&@CRLF)
 Else
 	_WriteResult("Fail","TS_LIN_Filter_12")
 	ConsoleWrite("Fail"&@CRLF)
 EndIf



$isAppNotRes=_CloseApp()																		; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_LIN_Filter_12")
EndIf

ConsoleWrite("****End : TS_LIN_Filter_12.au3****"&@CRLF)
ConsoleWrite(@CRLF)


