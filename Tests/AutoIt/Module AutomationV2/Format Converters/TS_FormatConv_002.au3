;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_FormatConv_002
; TestCases			:		CAPL_2_CPP converter
; Test Data			:		cfxFormatConv_01.cfx
; Test Setup		:		1. Select File->Utilities->Format converter
;                           2.click on CAPL_2_CPP converter tab
;                           3.click on Browse of .can file and select any .can file and add.
;                           4.click on Browse of .cpp file to add a new cpp file which will hold contents converted .can file to cpp file.
;                           5.Click convert
;                           6. After conversion is done, click Report button.
; Expected Results  : 		After step6, a log file with .txt gets opened which shows the results for conversion.
;                           It also specifies the input file name and path and output file name and path. Errors and warnings if any.
;==========================================================================Test Procedure =========================================================================
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_FormatConv_001****"&@CRLF)

_launchApp()
WinActivate($WIN_BUSMASTER)
$OutputFolderPath=_OutputDataPath()
$InputFolderPath=_TestDataPath()
if winexists($WIN_BUSMASTER) then
	_loadConfig("cfxFormatConv_01.cfx")
	$Res=Capl2CppConversion("CAPL2CPP.can","CAPL2CPP.cpp","temp_FormatConv.dbc","temp_FormatConv.dbf")
	$OutputCppFilePath=$OutputFolderPath &"\"& "CAPL2CPP.cpp"
	If($Res[0]="Conversion Completed Successfully") Then
		$ConvSuccess=1
	Else
		$ConvSuccess=0
	EndIf
	 If($Res[2]="Output File : D:\BUSMASTER_SVN\branches\BusMaster - v2.0.0\Tests\AutoIt\Module AutomationV2\Output\CAPL2CPP.cpp") Then
		 $OutputFilePath1=1
	 Else
		 $OutputFilePath1=0
	 EndIf
	 If FileExists($OutputCppFilePath) Then
		 $OutputFileExist=1
	 Else
		 $OutputFileExist=0
	 EndIf

	 ConsoleWrite("$ConvSuccess " &$ConvSuccess & @CRLF)
	 ConsoleWrite("$OutputFilePath1 " & $OutputFilePath1 & @CRLF)
	 ConsoleWrite("$OutputFileExist " & $OutputFileExist & @CRLF)
	 If($ConvSuccess=1 And $OutputFilePath1=1 And $OutputFileExist=1) Then
		 _WriteResult("Pass","STS_FORMAT_CONV_001")
	 Else
		 _WriteResult("Fail","STS_FORMAT_CONV_001")
	 EndIf
	 WinClose($WIN_FormatConv)
EndIf
ConsoleWrite("****End : TS_FormatConv_001****"&@CRLF)