;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_FormatConv_003
; TestCases			:		Log To Excel conversion
; Test Data			:		cfxFormatConv_01.cfx
; Test Setup		:		1. Select File->Utilities->Format converter
;                           2.click on Log to Excel File converter tab.
;                           3. select Bus Type as CAN.
;                           4.Select Log file for input by clicking browse button. And select any log file for ex D:\logs\BUSMASTERLogFile1.7.0.log
;                           5. In the CSV File the output file (.xls)gets created with the same name as that of log file and in the same location.
;                            To change click on Browse button and create a new file in required location.
;                           6. In the Fields group box, from available options, select Time and click '>>'. and Click Export.
;
; Expected Results  : 		1. After step3, the columns(available fields) that are present for CAN are displayed. Ie Time,Direction,channel,CAN ID,type,DLC,Data Bytes.
;                           2. After step6 a pop up message should get displayed as "Successfully converted text file to Excel file". Open the excel file from respective location, the file should contain all the columns
;                           3.After step8,a pop up message should get displayed as "Successfully converted text file to Excel file".
;                           4.Open the excel file from respective location, the file should contain all the columns for selected options
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_FormatConv_003****"&@CRLF)

_launchApp()
WinActivate($WIN_BUSMASTER)
$OutputFolderPath=_OutputDataPath()
$InputFolderPath=_TestDataPath()

if winexists($WIN_BUSMASTER) then
	_loadConfig("cfxFormatConv_01.cfx")
    $Result=_LogToExcelConversions("CAN","TestLogToExcel.log","TestLogToExcel.xls")
    $OutputCppFilePath=$OutputFolderPath &"\"& "TestLogToExcel.xls"
    If FileExists($OutputCppFilePath) Then
		 $OutputFileExist=1
	 Else
		 $OutputFileExist=0
	 EndIf
	 If($Result="Successfully converted text file to Excel file!") Then
		 $ConvSuccess=1
	 Else
		 $ConvSuccess=0
	 EndIf
	 ConsoleWrite("$OutputFileExist" & $OutputFileExist & @CRLF)
	 ConsoleWrite("$Conv" & $ConvSuccess & @CRLF)
	  If($ConvSuccess=1 And $OutputFileExist=1) Then
		_WriteResult("Pass","STS_FORMAT_CONV_003")
	Else
		_WriteResult("Fail","STS_FORMAT_CONV_003")
	EndIf
	WinClose($WIN_FormatConv)
 EndIf



