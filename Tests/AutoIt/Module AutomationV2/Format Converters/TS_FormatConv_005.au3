;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_FormatConv_005
; TestCases			:		Other Converters
; Test Data			:		cfxFormatConv_01.cfx
; Test Setup		:		1. Select File->Utilities->Format converter
;                           2.Click on other converters.
;                           3.select ASC TO LOG conversion.
;                           4.Select input .asc file. For ex D:\BUSMASTER DBFs\BUSMASTERLogFile0.asc.
;                           5. In the output File field the output file (.log)gets created with the same name as that of asc file and in the same location.
;                           To change click on Browse button and create a new file in required location.
;                           6.Click on convert
; Expected Results  : 		1.After step5, convert button is enabled and View log button is disabled initially.
;                           2.After step6, Conversion gets completed."Conversion completed successfully" message is displayed. And View log button should be disabled.
;                           It is enabled only when conversion is completed with errors or warnings
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_FormatConv_005****"&@CRLF)

_launchApp()
WinActivate($WIN_BUSMASTER)

$OutputFolderPath=_OutputDataPath()
$InputFolderPath=_TestDataPath()

if winexists($WIN_BUSMASTER) then
	_loadConfig("cfxFormatConv_01.cfx")
	$Result=_OthrConversionsFormatConv("ASC TO LOG Conversion","FormatConvASC2LOG.asc","FormatConvASC2LOG.log")
	$res=StringInStr($Result,"Conversion Completed Successfully")
	$OutputCppFilePath=$OutputFolderPath &"\"& "FormatConvASC2LOG.log"
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
		_WriteResult("Pass","STS_FORMAT_CONV_005")
	Else
		_WriteResult("Fail","STS_FORMAT_CONV_005")
	EndIf
	WinClose($WIN_FormatConv)
EndIf

ConsoleWrite("****End : TS_FormatConv_005****"&@CRLF)