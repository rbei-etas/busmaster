;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_LIN_Log_03
; TestCases			:		Functionality test - Log Filters
; Test Data			:
; Test Setup		:		1. Configure a log file with a filter that will filter all messages.
;~ 							2. Uncheck the Log Filter enable button if it is already enabled.
;~ 							3. Connect the tool and start message transmission.
;~ 							4. Enable Logging.
;~ 							5. Now enable log filter button from toolbar.
;~ 							6. Stop logging and message transmission.
;~ 							7. Open the log file and check the log file content

; Expected Results  : 		1. After step 7, the log file will contain only the entries that are received before enabling logging filter.
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_LIN_Log_03.au3****"&@CRLF)


_launchApp()

Dim $a[10]=["","","","","","","","","",""],$text[10]=["","","","","","","","","",""],$LastMsgTime[10]=["","","","","","","","","",""]
Local $text1 = "",$text2="",$lastmsg=""

if winexists($WIN_BUSMASTER) then
	_loadConfig("cfxLogging_03_LIN")														    ; Load Configuration
	;WinRibbonSelectItem($WIN_BUSMASTER,"",$CANMenu,$CandriverSelectMenu,$ActiveMenu)
	sleep(200)
	_SetViewToolBarLIN()																	 	; Select LIN Tool Bar to view.
	sleep(1000)
	_linDriverSelection()                                                                       ; Select the hardware
	sleep(1000)
    ;WinRibbonSelectItem($WIN_BUSMASTER,"",$CANMenu,$CandriverSelectMenu,$ActiveMenu)
	;sleep(2000)

	_ConfigureLINLog("Log_03_Sample","System","ALL","Decimal","Overwrite","","","True")		    ; Configure Logging details

	;If (WinActivate($winLinMsgWin) = 0) Then
;~ 	If (_ActivatechildWindowState($winLinMsgWin) = 0) Then
;~ 		ConsoleWrite("window not found")
;~ 		WinRibbonSelectItem($WIN_BUSMASTER,"",$linMenu,$LINMsgWinMenu,$activateLinMsgWin)
;~ 	EndIf

	_ConnectDisconnect_LIN()																	; Connect the tool


	;WinRibbonSelectItem($WIN_BUSMASTER,"",$LINMenu,$transmitMenu,$EnableTransmitMenu)			; Click on 'Transmit normal blocks' menu

	;sleep(3000)

	_EnableFilterLogLIN()																	    ; Enable Filter during logging
	;sleep(5)

	_ConnectDisconnect_LIN()																	; Disconnect the tool
	Sleep(500)

	;_DisableJ1939Win()																		    ; If J1939 Msg Window is active then disable it

	;_DisableOverwriteMode()																    ; Disable Overwrite Mode


	$GetLogFile_Path=_OutputDataPath()													    	; Fetch the Log file path
	$txt=_FileCountLines ($GetLogFile_Path & "\Log_03_Sample.log")
	ConsoleWrite("$txt : " &$txt & @CRLF)

	if $txt>2 Then
		$LastMsg=FileReadLine($GetLogFile_Path & "\Log_03_Sample.log",$txt-2)
		$a=stringsplit($LastMsg," ")
		ConsoleWrite("$a[1] : " &$a[1] & @CRLF)
		$totalMsgCount=$txt-2-14														    	; No. of lines in the log file-2(Last two lines in log file)-15(First 15 lines in log file) 341
		ConsoleWrite("$totalMsgCount : " & $totalMsgCount & @CRLF)
		$MsgWinItemCount=_GetLINMsgWinItemCount()											    ; Fetch the item count in the msg window
		ConsoleWrite("$MsgWinItemCount: " &$MsgWinItemCount & @CRLF)
		if $MsgWinItemCount>$totalMsgCount Then
			$hWd=ControlGetHandle($WIN_BUSMASTER,"",$LSTC_LINMsgWin)                            ; Get handle of the message window
			$fromMsgWin=_GUICtrlListView_GetItemTextString($hWd,$totalMsgCount-1)
			ConsoleWrite("$a[1] : " & $a[1] & @CRLF)
			ConsoleWrite("$fromMsgWin : " & $fromMsgWin & @CRLF)
			$text=stringsplit($fromMsgWin,"|")
			ConsoleWrite("$text[2]: " &$text[2] & @CRLF)
			$LastMsgWin=_GUICtrlListView_GetItemTextString($hWd,$totalMsgCount)
			ConsoleWrite("$LastMsgWin :" & $LastMsgWin & @CRLF)
			$LastMsgTime=stringsplit($LastMsgWin,"|")
			ConsoleWrite("$LastMsgTime[2] :" &$LastMsgTime[2] & @CRLF)

			$text1 = $a[1]
			$text2 = $text[2]
			$lastmsg = $LastMsgTime[2]

		EndIf
	EndIf

EndIf

	consolewrite(" $text1 :"& $text1&@CRLF)
	consolewrite("$text2 :"&$text2 &@CRLF)
	consolewrite("$lastmsg :"&$lastmsg &@CRLF)

;if $a[1]=$text[2] and $LastMsgTime[2]<>$a[1] then											     ; Validate the result
if $text1=$text2 and $lastmsg<>$text1 then											     ; Validate the result
	_WriteResult("Pass","TS_LIN_Log_03")
Else
	_WriteResult("Fail","TS_LIN_Log_03")
EndIf

$isAppNotRes=_CloseApp()																	     ; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_LIN_Log_03")
EndIf


ConsoleWrite("****End :TS_LIN_Log_03.au3****"&@CRLF)
ConsoleWrite(@CRLF)