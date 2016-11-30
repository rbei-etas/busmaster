;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_LIN_Log_34                                    Source : (TS_LIN_Log_18)
; TestCases			:		Using the Filter Attributes -Pass Filter
; Test Data			:
; Test Setup		:		1. Open 'Configure Filters' dialog by menu option 'Configure -> App Filters'.
;                           2. Make a filter with the following attributes:
;                           3. Choose 'Event' radio button
;                           4. Select 'Slave No Response'
;                           5. Choose 'Channel' as '1'.
;                           6. Add the filter by clicking on Add button and close the 'Configure Filters' dialog by clicking on 'OK'.
;                           7. Open 'ConfigureLogging for LIN' window by menu 'Configure->Log' and add a new log file.
;                           8. Choose the configured filter by opening 'Filter Selection dialog' and close the  'ConfigureLogging for LIN' window by 'Ok' click.
;                           9. Now click on connect and enable logging by menu 'Functions->Enable Filter->Log'.
;                          10. Transmit a frame which matches the filter attributes set for configured filter. Observe the log file contents after transmission.

;                                Follow the same procedure for different combinations of Direction and channel

; Expected Results  : 		1. The log file should not contain any frames which matches the filter attributes configured for this log file.
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_LIN_Log_34.au3****"&@CRLF)


_launchApp()

;Dim $HexMsgID21Tx1[10]=["","","","","","","","","",""],$HexMsgID21Tx2[10]=["","","","","","","","","",""],$HexMsgID21Tx3[10]=["","","","","","","","","",""]

Local $EndOfLog="",$finddataslaveNoRespnse=0,$cnt=0,$EndOfLog=""
Local $filename = "LOG_34_Sample.log"


if winexists($WIN_BUSMASTER) then

	_loadConfig("cfxLogging_34_LIN")															; Load Configuration

	_ConfigureLINLog("LOG_34_Sample","System","1","Decimal","Overwrite","","","True")		    ; Configure Logging details
    _SetViewToolBarLIN()																	 	; Select LIN Tool Bar to view.
	sleep(1000)
	_linDriverSelection()                                                                       ; Select the hardware
	sleep(1000)

	_EnableFilterLogLIN()                                                                       ; Enable Filter during logging
	sleep(1000)

	_ConnectDisconnect_LIN()																	; Connect the tool
	sleep(3000)

	;_TransmitMsgsMenu()																		; Transmit normal blocks
	;sleep(3000)

	_ConnectDisconnect_LIN()																	; Disconnect the tool
	Sleep(2000)
	;_DisableJ1939Win()																		    ; If J1939 Msg Window is active then disable it

EndIf

$GetLogFile_Path=_OutputDataPath()															    ; Fetch the Log file path
ConsoleWrite("$GetLogFile_Path:"&$GetLogFile_Path&@CRLF)

;WinRibbonSelectItem($WIN_BUSMASTER,"",$LINMenu,$LVC_CID_LINMsgWin,$EnableMsgWindowLIN)            ; Enable filters

$cnt = _FileCountLines($GetLogFile_Path &"\"& $filename)
ConsoleWrite("$cnt:"&$cnt&@CRLF)

if $cnt>=19 Then

	For $i=15 To $cnt-2
		$EndOfLog=FileReadLine ($GetLogFile_Path &"\"& $filename,$cnt)						    ; Read the 17th line from the Log file
		;consolewrite("$EndOfLog :"&$EndOfLog&@CRLF)
		$Finddata=FileReadLine ($GetLogFile_Path &"\"& $filename,$i)
		$finddatalogFile =StringSplit($Finddata," ")
		ConsoleWrite("$finddatalogFile1[2] = "&$finddatalogFile[2]&@CRLF)
		ConsoleWrite("$finddatalogFile1[3] = "&$finddatalogFile[3]&@CRLF)
		ConsoleWrite("$finddatalogFile1[3] = "&$finddatalogFile[4]&@CRLF)
		ConsoleWrite("$finddatalogFile1[5] = "&$finddatalogFile[5]&@CRLF)
		ConsoleWrite("$finddatalogFile1[6] = "&$finddatalogFile[6]&@CRLF)
		ConsoleWrite("$finddatalogFile1[7] = "&$finddatalogFile[7]&@CRLF)
		ConsoleWrite("$finddatalogFile1[7] = "&$finddatalogFile[8]&@CRLF)


		$slaveNoRespnse = $finddatalogFile[2] &" "& $finddatalogFile[3] &" "&  $finddatalogFile[4] &" "& $finddatalogFile[5] &" "& $finddatalogFile[6]
		ConsoleWrite("$slaveNoRespnse = "&$slaveNoRespnse&@CRLF)

		if $slaveNoRespnse="Error - Slave Not Responding" and $finddatalogFile[8]=1 Then
			$finddataslaveNoRespnse=$finddataslaveNoRespnse+1
		EndIf

	Next
	if $cnt=(14+$finddataslaveNoRespnse+2) Then
		$totalCountlogFile = 1
	EndIf

EndIf

consolewrite("$finddataslaveNoRespnse :"&$finddataslaveNoRespnse&@CRLF)
consolewrite("$EndOfLog :"&$EndOfLog&@CRLF)


if $EndOfLog="***[STOP LOGGING SESSION]***" and $finddataslaveNoRespnse=14 Then                                                ; Check the last line of the lof file
	_WriteResult("Pass","TS_LIN_Log_34")
Else
	_WriteResult("Fail","TS_LIN_Log_34")
EndIf

$isAppNotRes=_CloseApp()															            ; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_LIN_Log_34")
EndIf

ConsoleWrite("****End : TS_LIN_Log_34.au3****"&@CRLF)
ConsoleWrite(@CRLF)