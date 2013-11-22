;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_Log_03
; TestCases			:		Functionality test - Log Filters
; Test Data			:
; Test Setup		:		1. Configure a log file with a filter that will filter all messages.
;~ 							2. Uncheck the Log Filer enable button if it is already enabled.
;~ 							3. Connect the tool and start message transmission.
;~ 							4. Enable Logging.
;~ 							5. Now enable log filter button from toolbar.
;~ 							6. Stop logging and message transmission.
;~ 							7. Open the log file and check the log file content

; Expected Results  : 		1. After step 7, the log file will contain only the entries that are received before enabling logging filter.
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_Log_03.au3****"&@CRLF)


_launchApp()

Dim $a[10]=["","","","","","","","","",""],$text[10]=["","","","","","","","","",""],$LastMsgTime[10]=["","","","","","","","","",""]
if winexists($WIN_BUSMASTER) then
	_loadConfig("cfxLog_03")																; Load Configuration

	_ConfigureCANLog("Log_05_Sample","System","ALL","Decimal","Overwrite","","","True")		; Configure Logging details

	_ConnectDisconnect()																	; Connect the tool

	_TransmitMsgsMenu()																		; Transmit normal blocks
	sleep(3000)

	_EnableFilterLog()																		; Enable Filter during logging
	sleep(1500)

	_ConnectDisconnect()																	; Disconnect the tool

	Sleep(2000)
	_DisableJ1939Win()																		; If J1939 Msg Window is active then disable it

	_DisableOverwriteMode()																	; Disable Overwrite Mode
	Sleep(2000)

	$GetLogFile_Path=_OutputDataPath()														; Fetch the Log file path
	$txt=_FileCountLines ($GetLogFile_Path & "\Log_05_Sample.log")
	if $txt>2 Then
		$LastMsg=FileReadLine($GetLogFile_Path & "\Log_05_Sample.log",$txt-2)
		$a=stringsplit($LastMsg," ")
		ConsoleWrite("$a[1] : " &$a[1] & @CRLF)
		$totalMsgCount=$txt-2-15															; No. of lines in the log file-2(Last two lines in log file)-15(First 15 lines in log file)
		ConsoleWrite("$totalMsgCount : " & $totalMsgCount & @CRLF)
		$MsgWinItemCount=_GetCANMsgWinItemCount()											; Fetch the item count in the msg window
		ConsoleWrite("$MsgWinItemCount: " &$MsgWinItemCount & @CRLF)
		if $MsgWinItemCount>$totalMsgCount Then
			$hWd=ControlGetHandle($WIN_BUSMASTER,"",$LSTC_CANMsgWin)
			$fromMsgWin=_GUICtrlListView_GetItemTextString($hWd,$totalMsgCount-1)
			ConsoleWrite("$a[1] : " & $a[1] & @CRLF)
			ConsoleWrite("$fromMsgWin : " & $fromMsgWin & @CRLF)
			$text=stringsplit($fromMsgWin,"|")
			ConsoleWrite("$text[2]: " &$text[2] & @CRLF)
			$LastMsgWin=_GUICtrlListView_GetItemTextString($hWd,$totalMsgCount)
			ConsoleWrite("$LastMsgWin :" & $LastMsgWin & @CRLF)
			$LastMsgTime=stringsplit($LastMsgWin,"|")
			ConsoleWrite("$LastMsgTime[2] :" &$LastMsgTime[2] & @CRLF)
		EndIf
	EndIf
EndIf

if $a[1]=$text[2] and $LastMsgTime[2]<>$a[1] then											; Validate the result
	_WriteResult("Pass","TS_Log_03")
Else
	_WriteResult("Fail","TS_Log_03")
EndIf

$isAppNotRes=_CloseApp()																	; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_Log_03")
EndIf


ConsoleWrite("****End : TS_Log_03.au3****"&@CRLF)
ConsoleWrite(@CRLF)