;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_FormatConv_021
; TestCases			:		Other Converters
; Test Data			:		cfxFormatConv_01.cfx
; Test Setup		:	    1.Select File->Utilities->Format converter
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
ConsoleWrite("****Start : TS_FormatConv_021****"&@CRLF)

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
		$hWndOthrConverters=controlgethandle($WIN_FormatConv,"",$TAB_OtherConvts)                       ;Get handle of the Format converter window
	    _GUICtrlTab_ClickTab($hWndOthrConverters, 2)                                                     ; Select Other Converters tab
	    $handle=ControlGetHandle($WIN_FormatConv,"",$COMBO_SelectConvType)
	    $list=_GUICtrlComboBox_GetList($handle)
	    ConsoleWrite("list" & $list & @CRLF)
		If($list="ASC TO LOG Conversion|BLF TO LOG Conversion|DBC TO DBF Conversion|DBF TO DBC Conversion|J1939 DBC TO DBF Conversion|LOG TO ASC Conversion") Then
			_WriteResult("Pass","STS_FORMAT_CONV_021")
		Else
			_WriteResult("Fail","STS_FORMAT_CONV_021")
		EndIf
	EndIf
	WinClose($WIN_FormatConv)

EndIf
ConsoleWrite("****End : TS_FormatConv_021****"&@CRLF)



