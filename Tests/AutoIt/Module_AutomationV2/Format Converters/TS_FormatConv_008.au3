;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_FormatConv_008
; TestCases			:		Other Converters
; Test Data			:		cfxFormatConv_01.cfx
; Test Setup		:		1. Select File->Utilities->Format converter
;                           2.Click on other converters.
;                           3.select J1939 DBC TO DBF conversion.
;                           4.Add input .dbf file.For ex D:\BUSMASTER DBFs\FormatConvJ1939DBC2DBF.dbc
;                           5.In the output File field the output file (.dbc)gets created with the same name as that of dbc file and in the same location.
;                           To change click on Browse button and create a new file in required location.
;                           6.Click on convert
; Expected Results  : 		1.After step5, convert button is enabled and View log button is disabled initially.
;                           2.After step6, Conversion gets completed."Conversion completed successfully" message should be displayed. And View log button should be disabled.
;                           It is enabled only when conversion is completed with errors or warnings.And if you click on the View Log, the log file should get opened.
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_FormatConv_007****"&@CRLF)

_launchApp()
WinActivate($WIN_BUSMASTER)

$OutputFolderPath=_OutputDataPath()
$InputFolderPath=_TestDataPath()

if winexists($WIN_BUSMASTER) then
	_loadConfig("cfxFormatConv_01.cfx")
	Sleep(1000)
	$Result=_OthrConversionsFormatConv("J1939 DBC TO DBF Conversion","FormatConvJ1939DBC2DBF.dbc","FormatConvJ1939DBC2DBF.dbf")
	$res=StringInStr($Result,"Conversion completed Successfully")
	$OutputCppFilePath=$OutputFolderPath &"\"& "FormatConvJ1939DBC2DBF.dbf"
	If($res>0) Then
		$ConvSuccess=1
	Else
		$ConvSuccess=0
		$temp=ControlCommand($WIN_FormatConv,"",$BTN_ViewLog,"IsEnabled")
		ConsoleWrite("$temp" & $temp & @CRLF)
		If($temp=1) Then

			controlclick($WIN_FormatConv,"",$BTN_ViewLog)                                                               ; open log file to view warnings if any
 		    sleep(2000)

         EndIf

	EndIf
     If FileExists($OutputCppFilePath) Then
		 $OutputFileExist=1
	 Else
		 $OutputFileExist=0
	 EndIf
	 Winclose($WIN_FormatConv)
	 If($ConvSuccess=1 And $OutputFileExist=1) Then
		_WriteResult("Pass","STS_FORMAT_CONV_008")
	Else
		_WriteResult("Fail","STS_FORMAT_CONV_008")
	EndIf
    WinClose($WIN_FormatConv)
EndIf


