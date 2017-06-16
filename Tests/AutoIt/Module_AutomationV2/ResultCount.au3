	#include <Array.au3>
	#include <WinAPI.au3>
	#include <GUIMenu.au3>
	#include <WindowsConstants.au3>
	#include <File.au3>
	#include <Excel.au3>
Global $CountPass = 0
Global $CountFail = 0
Global $TotalTestScrpt = 287
Global $TotalRow = 1402



$ResultFilepath = @ScriptDir&"\Result\TestRunResults.xlsx"
if FileExists($ResultFilepath) Then
	;$FileOpen=_ExcelSheetActivate($oExcel, $vSheet)
	;ConsoleWrite("$FileOpen="&$FileOpen)
	$oExcel = _ExcelBookOpen($ResultFilepath)
	;for $i = 4 to $TotalTestScrpt
	for $i = 3 to $TotalRow
		$dataCell = _ExcelReadCell($oExcel, $i, 3)
		;$TotalTestScrpt1 = _ExcelReadCell($oExcel, $i, 2)
		ConsoleWrite("$dataCell = "&$dataCell&@CRLF)
		if $dataCell = "Pass" Then
			$CountPass = $CountPass + 1
		ElseIf $dataCell = "FAIL" Then
			$CountFail = $CountFail + 1
		EndIf

	Next
EndIf
Consolewrite("$CountPass = "&$CountPass)
Consolewrite("$CountFail = "&$CountFail)
$CountNotExectd = $TotalTestScrpt-($CountPass+$CountFail)

$ResultCountFile = @ScriptDir&"\ResultDetails.ini"
$RetVal = FileExists($ResultCountFile)
if $RetVal<>1 Then
	_FileCreate($ResultCountFile)
EndIf
	_FileWriteToLine($ResultCountFile, 1, "Total_TestScript = "&$TotalTestScrpt, 1)
	_FileWriteToLine($ResultCountFile, 2, "Total_TS_PASS = "&$CountPass, 1)
	_FileWriteToLine($ResultCountFile, 3, "Total_TS_FAIL = "&$CountFail+$CountNotExectd, 1)

_ExcelBookClose($oExcel)