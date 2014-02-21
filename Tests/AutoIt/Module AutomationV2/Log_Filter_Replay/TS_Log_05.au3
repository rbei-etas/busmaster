;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_Log_05
; TestCases			:		Header in log file
; Test Data			:
; Test Setup		:		1. Connect the tool by pressing connect tool bar button
;~ 							2. Select a log file by choosing "Configure ->Log"
;~ 							3.Make sure the "Hexadecimal" check box is checked.
;~ 							4. Start Message Transmission from other node
;~ 							5. Start Logging by selecting "Functions -> Start Logging" or tool bar button for logging
;~ 							6. Stop logging after few seconds

; Expected Results  : 		"All messages in message window will be logged with a header in log file having the date and time of start of logging. The message details will be in hexadecimal format
;~ 							The header format should be "***BUSMASTER Ver 3.00.01.C.D10***
;~ 							***NOTE: PLEASE DO NOT EDIT THIS DOCUMENT***
;~ 							***[START LOGGING SESSION]***
;~ 							***START DATE AND TIME DD:MM:YYYY HH:MM:SS:MSEC***"
;~ 							*** HEX***
;~ 							***TIME MODE***"
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_Log_05.au3****"&@CRLF)

_launchApp()
Local $Ver=""
if winexists($WIN_BUSMASTER) then
	_loadConfig("cfxLog_05")																; Load Configuration
	$ToolVer=_GetToolVersion()																; Fetch the Busmaster Version
	ConsoleWrite("$ToolVer :" &$ToolVer&@CRLF)
	ControlClick($DLG_About,"",$BTN_OK_About)
	$GetLogFile_Path=_OutputDataPath()														; Fetch the Log file path
	ConsoleWrite("$GetLogFile_Path:"&$GetLogFile_Path&@CRLF)

	if _FileCountLines($GetLogFile_Path & "\Log_04_Sample.log")>=7 Then
		$VersionFromLog=FileReadLine ($GetLogFile_Path & "\Log_04_Sample.log",1)				; Read the 1st line from the Log file
		consolewrite("$VersionFromLog :"&$VersionFromLog&@CRLF)
		$LogVer=StringReplace ($VersionFromLog,"***","")
		consolewrite("$LogVer:"&$LogVer&@CRLF)
		$Protocol=FileReadLine ($GetLogFile_Path & "\Log_04_Sample.log",2)						; Read the 2nd line from the Log file
		$NoEdit=FileReadLine ($GetLogFile_Path & "\Log_04_Sample.log",3)						; Read the 3rd line from the Log file
		$StartLog=FileReadLine ($GetLogFile_Path & "\Log_04_Sample.log",4)						; Read the 4rd line from the Log file
		$StartDateTime=FileReadLine ($GetLogFile_Path & "\Log_04_Sample.log",5)					; Read the 5rd line from the Log file
		$DateTime=StringSplit($StartDateTime," ")
		$PCDate=_NowDate()
		if $DateTime[1]="***START" and $DateTime[2]="DATE" and $DateTime[3]="AND" and $DateTime[4]="TIME" then
			$StartDateTimeInLog="Match"
		Else
			$StartDateTimeInLog="No Match"
		EndIf
		$NumericMode=FileReadLine ($GetLogFile_Path & "\Log_04_Sample.log",6)					; Read the 6rd line from the Log file
		$TimeMode=FileReadLine ($GetLogFile_Path & "\Log_04_Sample.log",7)						; Read the 7rd line from the Log file
	EndIf
	ControlFocus($DLG_About,"",$BTN_OK_About)
	ControlClick($DLG_About,"",$BTN_OK_About)
	send("{ENTER}")
	send("{ESC}")
EndIf
;~ $SetVer="***BUSMASTER Ver "&$ToolVer[2]&"***"
;~ consolewrite("$SetVer:"&$SetVer&@CRLF)
consolewrite("$VersionFromLog :"&$VersionFromLog&@CRLF)
consolewrite("$Protocol:"&$Protocol&@CRLF)
consolewrite("$NoEdit:"&$NoEdit&@CRLF)
consolewrite("$StartLog:"&$StartLog&@CRLF)
consolewrite("$PCDate:"&$PCDate&@CRLF)
consolewrite("$StartDateTime:"&$StartDateTime&@CRLF)
consolewrite("$StartDateTimeInLog:"&$StartDateTimeInLog&@CRLF)
consolewrite("$NumericMode:"&$NumericMode&@CRLF)
consolewrite("$TimeMode:"&$TimeMode&@CRLF)

if  $ToolVer=$LogVer and $Protocol="***PROTOCOL CAN***" and $NoEdit="***NOTE: PLEASE DO NOT EDIT THIS DOCUMENT***" and $StartLog="***[START LOGGING SESSION]***" and $StartDateTimeInLog="Match" and $NumericMode="***HEX***" and $TimeMode="***SYSTEM MODE***" Then
	_WriteResult("Pass","TS_Log_05")
Else
	_WriteResult("Fail","TS_Log_05")
EndIf

$isAppNotRes=_CloseApp()															; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_Log_05")
EndIf


ConsoleWrite("****End : TS_Log_05.au3****"&@CRLF)
ConsoleWrite(@CRLF)