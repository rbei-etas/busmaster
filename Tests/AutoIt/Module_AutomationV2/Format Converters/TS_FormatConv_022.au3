;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_FormatConv_022
; TestCases			:		Other converters for system time mode for log to Asc conversion
; Test Data			:		cfxFormatConv_01.cfx
; Test Setup		:		1. Select File->Utilities->Format converter
;                           2.Click on other converters.
;                           3.select LOG TO ASC conversion.
;                           4.Add input .log file which has System Time mode for ex D:\logs\log_asc.log.
;                           5.In the output File field the output file (.asc)gets created with the same name as that of log file and in the same location.
;                           6.Click Convert.

; Expected Results  : 		After step6, an error message is displayed as "Conversion May not be work with CANoe since the input file is logged in system mode"
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_FormatConv_022****"&@CRLF)

_launchApp()
WinActivate($WIN_BUSMASTER)
If WinExists($WIN_BUSMASTER) Then
	_loadConfig("cfxFormatConv_01.cfx")
	Sleep(1000)
	local $result[10]
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
	EndIf
		ControlCommand($WIN_FormatConv,"",$COMBO_SelectConvType,"SelectString","Log TO ASC Conversion")		       ; Select 'DBC to DBF Conversion' from combo box
        ControlClick($WIN_FormatConv,"",$BTN_Input)                                                        ; Click on "Input" button
		sleep(1000)
		WinWaitActive("", $Dlg_OpenOthrConv, 5)                                      						;wait for open dialog
        $InputFilePath=_TestDataPath()
		$InputFile="LogFileFormatConv.log"
		ControlSend($Dlg_OpenOthrConv,"",$TXT_Open_FilePath_OthrConv,$InputFilePath &"\"& $InputFile)      ;Send path of the input file
        ControlClick($Dlg_OpenOthrConv,"",$BTN_Open_FilePath_OthrConv)
		sleep(1000)
        ControlClick($WIN_FormatConv,"",$BTN_Output)                                                       ; Click on "Output" button
        sleep(1000)
        WinWaitActive("", $Dlg_OpenOthrConv, 5)                                     					   ; wait for open dialog
        $OutputFilePath= _OutputDataPath()
		$OutputFile="LogFileFormatConv.asc"
		ControlSend($Dlg_OpenOthrConv,"",$TXT_Open_FilePath_OthrConv,$OutputFilePath &"\"& $OutputFile)    ;Send path of the input file
        ControlClick($Dlg_OpenOthrConv,"",$BTN_Open_FilePath_LogToExcel)
        sleep(1000)
        ControlClick($WIN_FormatConv,"",$BTN_Convert_OtherConvts)                                          ; Click on "Convert" button
        sleep(4000)
        $OutputFileOthrConv=$OutputFilePath &"\"& $OutputFile
		$result=WinGetText($WIN_FormatConv)
		ConsoleWrite("Result of conversion" & $result & @CRLF)
        $res=StringInStr($result,"Conversion May not be work with CANoe since the input file is logged in system mode")
        If($res>0) Then
		    _WriteResult("Pass","STS_FORMAT_CONV_022")
		Else
			_WriteResult("Fail","STS_FORMAT_CONV_022")
		EndIf
		WinClose($WIN_FormatConv)

EndIf

ConsoleWrite("****End : TS_FormatConv_022****"&@CRLF)
