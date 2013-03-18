;=== Test Cases/Test Data ===
; Module			:		Logging
; Critical (C)		:		Y
; TestCase No.		:		TS_Log_01
; TestCases			:		Configuring Log File
; Test Strategy		:		Black Box
; Test Data			:		-
; === Test Procedure ===

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_Log_01.au3****"&@CRLF)


_launchApp()

if winexists($mWin) then

	_createConfig("cfxLogging_01")													; Create New Configuration

	_ConfigureCANLog("Logtest01_1","System","ALL","Hex","Overwrite","","","False")		; Configure Logging details

	_ConfigureCANLog("Logtest01_2","Absolute","ALL","Decimal","Append","","","False")	; Configure Logging details

	_CANLogMenu()																	; Select Configure->Log from menu

	; Fetch the details of configured first log file

	$LogFilehWd=controlgethandle($WIN_CANLog,"",$LSTC_LogFile_ConfigLOG)							; Fetch handle of log file list view
	_GUICtrlListView_ClickItem($LogFilehWd,0)
	sleep(500)
	$firstLogFilePath=_GUICtrlListView_GetItemText($LogFilehWd,0)					; Fetch the path of the first log file
	$firstFileTimeModeTxt=ControlGetText($WIN_CANLog, "", $COMB_TimeMode_ConfigLOG )				; Fetch the time mode
	$firstFileNumModeHex=Controlcommand($WIN_CANLog,"",$RBTN_NumModeHex_ConfigLOG,"IsChecked")			; Check whether Hex Numeric mode is set
	$firstFileModeOverWrite=Controlcommand($WIN_CANLog,"",$RBTN_FileModeOverWrite_ConfigLOG,"IsChecked")	; Check whether overwrite file mode is set
	ConsoleWrite("$firstLogFilePath : "&$firstLogFilePath&@CRLF)
	ConsoleWrite("$firstFileTimeModeTxt : "&$firstFileTimeModeTxt&@CRLF)
	ConsoleWrite("$firstFileNumModeHex : "&$firstFileNumModeHex&@CRLF)
	ConsoleWrite("$firstFileModeOverWrite : "&$firstFileModeOverWrite&@CRLF)
	$DirPath=_SetOneFolderUp()
	if  $firstLogFilePath=$DirPath&"\Logtest01_1.log" and $firstFileTimeModeTxt="System" and $firstFileNumModeHex=1 and $firstFileModeOverWrite=1 Then
		$FirstLogFileDetails="Match"
	Else
		$FirstLogFileDetails="No Match"
	EndIf

	; Fetch the details of configured Second log file

	_GUICtrlListView_ClickItem($LogFilehWd,1)
	sleep(500)
	$SecondLogFilePath=_GUICtrlListView_GetItemText($LogFilehWd,1)					; Fetch the path of the Second log file
	$SecondFileTimeModeTxt=ControlGetText($WIN_CANLog, "", $COMB_TimeMode_ConfigLOG )				; Fetch the time mode
	$SecondFileNumModeHex=Controlcommand($WIN_CANLog,"",$RBTN_NumModeDec_ConfigLOG,"IsChecked")			; Check whether Dec Numeric mode is set
	$SecondFileModeOverWrite=Controlcommand($WIN_CANLog,"",$RBTN_FileModeApp_ConfigLOG,"IsChecked")		; Check whether Append file mode is set
	ConsoleWrite("$SecondLogFilePath : "&$SecondLogFilePath&@CRLF)
	ConsoleWrite("$SecondFileTimeModeTxt : "&$SecondFileTimeModeTxt&@CRLF)
	ConsoleWrite("$SecondFileNumModeHex : "&$SecondFileNumModeHex&@CRLF)
	ConsoleWrite("$SecondFileModeOverWrite : "&$SecondFileModeOverWrite&@CRLF)
	if  $SecondLogFilePath=$DirPath&"\Logtest01_2.log" and $SecondFileTimeModeTxt="Absolute" and $SecondFileNumModeHex=1 and $SecondFileModeOverWrite=1 Then
		$SecondLogFileDetails="Match"
	Else
		$SecondLogFileDetails="No Match"
	EndIf
	Controlclick($WIN_CANLog,"",$BTN_OK_ConfigLOG)										; Click on 'OK' button

EndIf
if $FirstLogFileDetails="Match" and $SecondLogFileDetails="Match" then
	_ExcelWriteCell($oExcel, "Pass", 13, 2)
Else
	_ExcelWriteCell($oExcel, "Fail", 13, 2)
EndIf

ConsoleWrite("****End : TS_Log_01.au3****"&@CRLF)
ConsoleWrite(@CRLF)
