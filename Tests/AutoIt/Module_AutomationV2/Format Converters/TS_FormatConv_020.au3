#;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_FormatConv_020
; TestCases			:		Log to Excel File Converter
; Test Data			:		cfxFormatConv_01.cfx
; Test Setup		:		1. Select File->Utilities->Format converter
;                           2.click on Log to Excel File converter tab.
;                           3.Select Bus Type as CAN.
;                           4. Select Time from Fields. And Click '>'.
;                           5. Again click '<'.
;                           6.Select time and click '>>'.
;                           7.Again click '<<'

;
; Expected Results  : 		1.After step4,Time gets added to the selected list.
;                           2.After step5, Time gets removed from selected list.
;                           3.After step6 all the column options are added to the selected list.
;                           4.After step7, the added items get removed from the list and are added back to available list.
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_FormatConv_020****"&@CRLF)

_launchApp()
WinActivate($WIN_BUSMASTER)
If WinExists($WIN_BUSMASTER) Then
	_loadConfig("cfxFormatConv_01.cfx")
	Sleep(1000)
	local $ConversionRes[10]
	 _FormatConverterMenu()                                                                                      ;Invoke Format converters menu
     if @error Then
		ConsoleWriteError("Format Converter didn't run" & @CRLF)
	Exit
    EndIf
		WinWaitActive($WIN_FormatConv, "", 5)                                                                     ;Wait for Format converter window
    if @error Then
		ConsoleWriteError("Format Conversions didn't activate" & @CRLF)
    Exit
    EndIf
    If winexists($WIN_FormatConv) then
		sleep(2000)
		$hWndLogToExcel=controlgethandle($WIN_FormatConv,"",$Tab_CAPL2CPP)
		_GUICtrlTab_ClickTab($hWndLogToExcel, 1)
		$handle=ControlGetHandle($WIN_FormatConv,"",$COMBO_SelectConvTypeLogToExcel)
		_GUICtrlComboBox_SelectString($handle,"CAN")                                                              ;Select Bus type CAN
		$handle=ControlGetHandle($WIN_FormatConv,"",$LSTB_AvailableFields)
		_GUICtrlListBox_ClickItem($handle,0)
		sleep(500)
		ControlClick($WIN_FormatConv,"",$BTN_SelectOne)                                                                     ;Select one field
		Sleep(500)
		$handle1=ControlGetHandle($WIN_FormatConv,"",$LSTB_SelectedFields)
		$count_one_sel=_GUICtrlListBox_GetCount($handle1)                                                              ;Get count of selected fields
		ConsoleWrite("Count one" & $count_one_sel & @CRLF)
		Sleep(500)
		ControlClick($WIN_FormatConv,"",$BTN_DeleteOneField)                                                                    ; Click on button to remove selected field
		$count_one_rem=_GUICtrlListBox_GetCount($handle1)
		ConsoleWrite("Count one rem" & $count_one_rem & @CRLF)
		Sleep(1000)
		_GUICtrlListBox_ClickItem($handle,0)
		Sleep(500)
		ControlClick($WIN_FormatConv,"",$BTN_SelectAll)
    	$count_All=	_GUICtrlListBox_GetCount($handle1)
		ConsoleWrite("All count" & $count_All & @CRLF)
		Sleep(500)

		ControlClick($WIN_FormatConv,"",$BTN_DeleteAllField)
		$Count_All_rem=_GUICtrlListBox_GetCount($handle1)
		ConsoleWrite("All count after rem" & $Count_All_rem & @CRLF)

        If($count_one_sel=1 And $count_one_rem=0 And $count_All=7 And $Count_All_rem=0) Then
			_WriteResult("Pass","STS_FORMAT_CONV_020")
		Else
			_WriteResult("Fail","STS_FORMAT_CONV_020")
		EndIf
	EndIf
	WinClose($WIN_FormatConv)
EndIf
ConsoleWrite("****End : TS_FormatConv_020****"&@CRLF)


