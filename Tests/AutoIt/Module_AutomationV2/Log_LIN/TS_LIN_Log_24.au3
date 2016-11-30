;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_LIN_Log_24                                      Source : (TS_LIN_Log_08)
; TestCases			:		Using the Filter Attributes - Pass Filter
; Test Data			:
; Test Setup		:		1. Open 'Configure Filters' dialog by menu option 'Configure -> App Filters'.
;                           2. Make a filter with the following attributes:
;                           3. Choose 'ID' radio box in 'Filter Attributes' group box.
;                           4. Select an 'ID' from the combobox or manually enter it.
;                           5. Choose 'Direction' as 'ALL'.
;                           6. Choose 'Channel' as 'ALL'.
;                           7. Add the filter by clicking on Add button and close the 'Configure Filters' dialog by clicking on 'OK'.
;                           8. Open 'ConfigureLogging for LIN' window by menu 'Configure->Log' and add a new log file.
;                           9. Choose the configured filter by opening 'Filter Selection dialog' and close the  'ConfigureLogging for LIN' window by 'Ok' click.
;                          10. Now click on connect and enable logging by menu 'Functions->Enable Filter->Log'.
;                          11. Transmit a frame which matches the filter attributes set for configured filter. Observe the log file contents after transmission

; Expected Results  : 		1. The log file should not contain any messages which matches the filter attributes configured for this log file.
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_LIN_Log_24.au3****"&@CRLF)


_launchApp()

Local $EndOfLog=""
local $fileName = "LOG_24_Sample.log",$finddatacountTxChnl=0,$cnt=0,$EndOfLog=""

if winexists($WIN_BUSMASTER) then

	_loadConfig("cfxLogging_24_LIN")															; Load Configuration

	_ConfigureLINLog("LOG_24_Sample","System","ALL","Decimal","Overwrite","","","True")		    ; Configure Logging details
    _SetViewToolBarLIN()																	 	; Select LIN Tool Bar to view.
	sleep(1000)
	_linDriverSelection()                                                                       ; Select the hardware
	sleep(1000)

	_EnableFilterLogLIN()                                                                       ; Enable filter while logging
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

$cnt = _FileCountLines($GetLogFile_Path &"\"& $filename)
ConsoleWrite("$cnt:"&$cnt&@CRLF)
if $cnt>=14 Then
	$EndOfLog=FileReadLine ($GetLogFile_Path &"\"& $filename,$cnt)						    ; Read the 12th line from the Log file
	consolewrite("$EndOfLog :"&$EndOfLog&@CRLF)
	For $i=15 To $cnt-2
		$Finddata=FileReadLine ($GetLogFile_Path &"\"& $filename,$i)
		$finddatalogFile =StringSplit($Finddata," ")
		ConsoleWrite("$finddatalogFile1[2] = "&$finddatalogFile[2]&@CRLF)
		ConsoleWrite("$finddatalogFile2[3] = "&$finddatalogFile[3]&@CRLF)
		ConsoleWrite("$finddatalogFile8[4] = "&$finddatalogFile[4]&@CRLF)
		if $finddatalogFile[2]="Tx" and $finddatalogFile[3]=1 and $finddatalogFile[4]="0001" Then
			$finddatacountTxChnl=$finddatacountTxChnl+1
		EndIf
	Next
EndIf

consolewrite("$finddatacountTxChnl :"&$finddatacountTxChnl&@CRLF)
consolewrite("$EndOfLog :"&$EndOfLog&@CRLF)

if $EndOfLog="***[STOP LOGGING SESSION]***" and $finddatacountTxChnl=18 Then                                                ; Check the last line of the log file
	_WriteResult("Pass","TS_LIN_Log_24")
Else
	_WriteResult("Fail","TS_LIN_Log_24")
EndIf

$isAppNotRes=_CloseApp()															            ; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_LIN_Log_24")
EndIf


ConsoleWrite("****End : TS_LIN_Log_24.au3****"&@CRLF)
ConsoleWrite(@CRLF)