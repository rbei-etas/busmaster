;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_FormatConv_001
; TestCases			:		CAPL_2_CPP converter
; Test Data			:		cfxFormatConv_01.cfx
; Test Setup		:		1. Select File->Utilities->Format converter
;                           2.click on CAPL_2_CPP converter tab
;                           3.click on Browse of .can file and select any .can file and add.
;                           4.Dont click Browse of .cpp file and do not add a new .cpp file.
; Expected Results  : 		Converter will create new .cpp file with same name as that of .can file and in the same location or directory.
;                          It will use the same path as that of .can file
;==========================================================================Test Procedure =========================================================================
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_FormatConv_002****"&@CRLF)

_launchApp()
WinActivate($WIN_BUSMASTER)
$OutputFolderPath=_TestDataPath()

if winexists($WIN_BUSMASTER) then
	_loadConfig("cfxFormatConv_01.cfx")
	$OutputCppFilePath=Capl2CppConversion1("CAPL2CPP.can","CAPL2CPP.cpp","temp_FormatConv.dbc","temp_FormatConv.dbf")
	ConsoleWrite("$OutputCppFilePath[0]" &$OutputCppFilePath[0] & @CRLF)
	ConsoleWrite("$OutputCppFilePath[1]" &$OutputCppFilePath[1] & @CRLF)
	ConsoleWrite("$OutputCppFilePath[2]" &$OutputCppFilePath[2] & @CRLF)
	$OutputCppFilePath1=$OutputFolderPath &"\"& "CAPL2CPP.cpp"
	If($OutputCppFilePath[0]="Conversion Completed Successfully") Then
		$ConvSuccess=1
	Else
		$ConvSuccess=0
	EndIf
	 If($OutputCppFilePath[2]="Output File : D:\BUSMASTER_SVN\branches\BusMaster - v2.0.0\Tests\AutoIt\Module AutomationV2\TestData\CAPL2CPP.cpp") Then
		 $OutputFilePath=1
	 Else
		 $OutputFilePath=0
	 EndIf
	 If FileExists($OutputCppFilePath1) Then
		 $OutputFileExist=1
	 Else
		 $OutputFileExist=0
	 EndIf
	 ConsoleWrite("$ConvSuccess " &$ConvSuccess & @CRLF)
	 ConsoleWrite("$OutputFileExist " & $OutputFileExist & @CRLF)
	  ConsoleWrite("$OutputFilePath " & $OutputFilePath & @CRLF)
	  If($ConvSuccess=1 And $OutputFilePath=1 And $OutputFileExist=1) Then
		 _WriteResult("Pass","STS_FORMAT_CONV_002")
	 Else
		 _WriteResult("Fail","STS_FORMAT_CONV_002")
	 EndIf
	 WinClose($WIN_FormatConv)
EndIf

ConsoleWrite("****End : TS_FormatConv_002****"&@CRLF)