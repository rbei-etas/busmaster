;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_J1939Log_03
; TestCases			:		Functionality test - Stopping logging
; Test Data			:
; Test Setup		:		1.Repreat the previous test case but instead of closing the application change the configuration file.

; Expected Results  : 		1. As mentioned in the previous test case.
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_J1939Log_03.au3****"&@CRLF)


_launchApp()

WinActivate($WIN_BUSMASTER,3)
if winexists($WIN_BUSMASTER) then

	_loadConfig("cfxJ1939Log_03")																		; Load Configuration

	_ActivateJ1939()																					; Activate J1939

	_ConnectDisconnect()																				; Connect
	sleep(1500)

	_J1939tMsgWin("Request Address","","","")																	; Transmit "Request Address" msg

	_ConnectDisconnect()																				; Disconnect
	sleep(1500)

	WinMenuSelectItem($WIN_BUSMASTER,"",$HelpMenu,$AboutBusmaster)
	$Ver=controlgettext($DLG_About,"",$Static_Version_About)											; Fetch version
	$ToolVer="BUSMASTER " & StringTrimLeft($Ver,4)
	ConsoleWrite("$ToolVer :" &$ToolVer&@CRLF)
	ControlClick($DLG_About,"",$BTN_OK_About)
	$GetLogFile_Path=_TestDataPath()																	; Fetch the Log file path
	ConsoleWrite("$GetLogFile_Path:"&$GetLogFile_Path&@CRLF)
	$VersionFromLog=FileReadLine ($GetLogFile_Path & "\LogtestJ1939_03.log",1)							; Read the 1st line from the Log file
	consolewrite("$VersionFromLog :"&$VersionFromLog&@CRLF)
	$LogVer=StringReplace ($VersionFromLog,"***","")
	consolewrite("$LogVer:"&$LogVer&@CRLF)
	$Protocol=FileReadLine ($GetLogFile_Path & "\LogtestJ1939_03.log",2)								; Read the 2nd line from the Log file
	$NoEdit=FileReadLine ($GetLogFile_Path & "\LogtestJ1939_03.log",3)									; Read the 3rd line from the Log file
	$StartLog=FileReadLine ($GetLogFile_Path & "\LogtestJ1939_03.log",4)								; Read the 4rd line from the Log file
	$StartDateTime=FileReadLine ($GetLogFile_Path & "\LogtestJ1939_03.log",5)							; Read the 5rd line from the Log file
	$DateTime=StringSplit($StartDateTime," ")
	$PCDate=_NowDate()
	if $DateTime[1]="***START" and $DateTime[2]="DATE" and $DateTime[3]="AND" and $DateTime[4]="TIME" then
		$StartDateTimeInLog="Match"
	Else
		$StartDateTimeInLog="No Match"
	EndIf
	$NumericMode=FileReadLine ($GetLogFile_Path & "\LogtestJ1939_03.log",6)								; Read the 6rd line from the Log file
	$TimeMode=FileReadLine ($GetLogFile_Path & "\LogtestJ1939_03.log",7)								; Read the 7rd line from the Log file
	ControlFocus($DLG_About,"",$BTN_OK_About)
	ControlClick($DLG_About,"",$BTN_OK_About)
	send("{ENTER}")
	send("{ESC}")

	consolewrite("$VersionFromLog :"&$VersionFromLog&@CRLF)
	consolewrite("$Protocol:"&$Protocol&@CRLF)
	consolewrite("$NoEdit:"&$NoEdit&@CRLF)
	consolewrite("$StartLog:"&$StartLog&@CRLF)
	consolewrite("$PCDate:"&$PCDate&@CRLF)
	consolewrite("$StartDateTime:"&$StartDateTime&@CRLF)
	consolewrite("$StartDateTimeInLog:"&$StartDateTimeInLog&@CRLF)
	consolewrite("$NumericMode:"&$NumericMode&@CRLF)
	consolewrite("$TimeMode:"&$TimeMode&@CRLF)

	_DeactJ1939()																						; Deactivate J1939

EndIf


if  $ToolVer=$LogVer and $Protocol="***PROTOCOL J1939***" and $NoEdit="***NOTE: PLEASE DO NOT EDIT THIS DOCUMENT***" and $StartLog="***[START LOGGING SESSION]***" and $StartDateTimeInLog="Match" and $NumericMode="***HEX***" and $TimeMode="***SYSTEM MODE***" Then
	_WriteResult("Pass","TS_J1939Log_03")
Else
	_WriteResult("Fail","TS_J1939Log_03")
EndIf

$isAppNotRes=_CloseApp()																				; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_J1939Log_03")
EndIf



ConsoleWrite("****End : TS_J1939Log_03.au3****"&@CRLF)
ConsoleWrite(@CRLF)
