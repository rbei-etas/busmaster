;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_LIN_Log_01
; TestCases			:		Configuring Log File
; Test Data			:
; Test Setup		:		1. Open Configure Log
;~ 							2. Add Some Log files
;~ 							3. Select OK to conform changes.
;~ 							4. Select log again and verify the log details

; Expected Results  : 		1. Log File details will show the list of log files added during step 2.
;==========================================================================Test Procedure =========================================================================

;#include <IncludeFiles.au3>

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_LIN_Log_01.au3****"&@CRLF)


_launchApp()

local $FirstLogFileDetails="",$SecondLogFileDetails=""

if winexists($mWin) then

	_createConfig("cfxLogging_01")																			; Create New Configuration

	_ConfigureLINLog("Logtest01_1","System","ALL","Hex","Overwrite","","","False")							; Configure Logging details

	_ConfigureLINLog("Logtest01_2","Absolute","ALL","Decimal","Append","","","False")						; Configure Logging details

	_LINLogMenu()																							; Select Configure->Log from menu

	; Fetch the details of configured first log file

	$LogFilehWd=controlgethandle($WIN_LINLog,"",$LSTC_LogFile_ConfigLOG)									; Fetch handle of log file list view
	_GUICtrlListView_ClickItem($LogFilehWd,0)
	sleep(500)
	$firstLogFilePath=_GUICtrlListView_GetItemText($LogFilehWd,0)											; Fetch the path of the first log file
	$firstFileTimeModeTxt=ControlGetText($WIN_LINLog, "", $COMB_TimeMode_ConfigLOG )						; Fetch the time mode
	$firstFileNumModeHex=Controlcommand($WIN_LINLog,"",$RBTN_NumModeHex_ConfigLOG,"IsChecked")				; Check whether Hex Numeric mode is set
	$firstFileModeOverWrite=Controlcommand($WIN_LINLog,"",$RBTN_FileModeOverWrite_ConfigLOG,"IsChecked")	; Check whether overwrite file mode is set
	ConsoleWrite("$firstLogFilePath : "&$firstLogFilePath&@CRLF)                                            ; Print the first log file path
	ConsoleWrite("$firstFileTimeModeTxt : "&$firstFileTimeModeTxt&@CRLF)                                    ; Print the first file time mode
	ConsoleWrite("$firstFileNumModeHex : "&$firstFileNumModeHex&@CRLF)                                      ; Print the first file number mode
	ConsoleWrite("$firstFileModeOverWrite : "&$firstFileModeOverWrite&@CRLF)                                ; Print the first file mode overwrite
	$DirPath=_OutputDataPath()
	if  $firstLogFilePath=$DirPath&"\Logtest01_1.log" and $firstFileTimeModeTxt="System" and $firstFileNumModeHex=1 and $firstFileModeOverWrite=1 Then
		$FirstLogFileDetails="Match"
	Else
		$FirstLogFileDetails="No Match"
	EndIf

	; Fetch the details of configured Second log file

	_GUICtrlListView_ClickItem($LogFilehWd,1)
	sleep(500)
	$SecondLogFilePath=_GUICtrlListView_GetItemText($LogFilehWd,1)											; Fetch the path of the Second log file
	$SecondFileTimeModeTxt=ControlGetText($WIN_LINLog, "", $COMB_TimeMode_ConfigLOG )						; Fetch the time mode
	$SecondFileNumModeHex=Controlcommand($WIN_LINLog,"",$RBTN_NumModeDec_ConfigLOG,"IsChecked")				; Check whether Dec Numeric mode is set
	$SecondFileModeOverWrite=Controlcommand($WIN_LINLog,"",$RBTN_FileModeApp_ConfigLOG,"IsChecked")			; Check whether Append file mode is set
	ConsoleWrite("$SecondLogFilePath : "&$SecondLogFilePath&@CRLF)                                          ; Print the second log file path
	ConsoleWrite("$SecondFileTimeModeTxt : "&$SecondFileTimeModeTxt&@CRLF)                                  ; Print the second file time mode
	ConsoleWrite("$SecondFileNumModeHex : "&$SecondFileNumModeHex&@CRLF)                                    ; Print the second file number mode
	ConsoleWrite("$SecondFileModeOverWrite : "&$SecondFileModeOverWrite&@CRLF)                              ; Print the second file mode overwrite
	if  $SecondLogFilePath=$DirPath&"\Logtest01_2.log" and $SecondFileTimeModeTxt="Absolute" and $SecondFileNumModeHex=1 and $SecondFileModeOverWrite=1 Then
		$SecondLogFileDetails="Match"
	Else
		$SecondLogFileDetails="No Match"
	EndIf
	Controlclick($WIN_LINLog,"",$BTN_OK_ConfigLOG)															; Click on 'OK' button

EndIf
if $FirstLogFileDetails="Match" and $SecondLogFileDetails="Match" then
	_WriteResult("Pass","TS_LIN_Log_01")
Else                                                                                                        ; Print the pass or fail
	_WriteResult("Fail","TS_LIN_Log_01")
EndIf

$isAppNotRes=_CloseApp()															                        ; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_LIN_Log_01")
EndIf



ConsoleWrite("****End : TS_LIN_Log_01.au3****"&@CRLF)
ConsoleWrite(@CRLF)
