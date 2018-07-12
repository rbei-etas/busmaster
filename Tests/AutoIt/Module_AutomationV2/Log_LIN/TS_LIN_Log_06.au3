;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_LIN_Log_06                                           SOURCE :(TS_Log_012)
; TestCases			:		Using the Filter Attributes
; Test Data			:
; Test Setup		:		1. Open 'Configure Filters' dialog by menu option 'Configure -> App Filters'.
;                           2. Make a filter with the following attributes:
;                           3. Choose 'ID' radio box in 'Filter Attributes' group box.
;                           4. Select an 'ID' from the combobox or manually enter it.
;                           5. Choose 'Direction' as 'Tx'.
;                           6. Choose 'Channel' as 'ALL'.
;                           7. Add the filter by clicking on Add button and close the 'Configure Filters' dialog by clicking on 'OK'.
;                           8. Open 'ConfigureLogging for LIN' window by menu 'Configure->Log' and add a new log file.
;                           9. Choose the configured filter by opening 'Filter Selection dialog' and close the  'ConfigureLogging for LIN' window by 'Ok' click.
;                           10. Now click on connect and enable logging by menu 'Functions->Enable Filter->Log'.
;                           11. Transmit a frame which matches the filter attributes set for configured filter. Observe the log file contents after transmission

; Expected Results  : 		1. The log file should not contain any frames which matches the filter attributes configured for this log file.
;==========================================================================Test Procedure =========================================================================


ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_LIN_Log_06.au3****"&@CRLF)


_launchApp()

Dim $HexMsgID21Tx1[10]=["","","","","","","","","",""],$HexMsgID21Tx2[10]=["","","","","","","","","",""],$HexMsgID21Tx3[10]=["","","","","","","","","",""]
Local $TxAllDir="",$TxAllCH="",$TxAllID=""



if winexists($WIN_BUSMASTER) then
	_loadConfig("cfxLogging_06_LIN")														    ; Load Configuration

	_ConfigureLINLog("LOG_06_Sample","System","ALL","Decimal","Overwrite","","","True")		    ; Configure Logging details
	_SetViewToolBarLIN()																	    ; Select LIN Tool Bar to view.
	sleep(1000)
	_linDriverSelection()                                                                       ; Select the hardware
	sleep(1000)


	_EnableFilterLogLIN()																		; Enable Filter during logging
	sleep(1000)

	_ConnectDisconnect_LIN()																	; Connect the tool

	;_TransmitMsgsMenu()																		; Transmit normal blocks
	;sleep(3000)

	_ConnectDisconnect_LIN()																	; Disconnect the tool
    Sleep(2000)
	;_DisableJ1939Win()																		    ; If J1939 Msg Window is active then disable it

EndIf

$GetLogFile_Path=_OutputDataPath()															    ; Fetch the Log file path
ConsoleWrite("$GetLogFile_Path:"&$GetLogFile_Path&@CRLF)

if _FileCountLines($GetLogFile_Path & "\Log_06_Sample.log")>=21 Then
	$FindTx1=FileReadLine ($GetLogFile_Path & "\LOG_06_Sample.log",17)							; Read the 17th line from the Log file
	consolewrite("$FindTx1 :"&$FindTx1&@CRLF)
	$HexMsgID21Tx1=StringSplit($FindTx1," ")
	consolewrite("$HexMsgID21Tx1_Dir :"&$HexMsgID21Tx1[2]&@CRLF)								; Fetch the Direction
	consolewrite("$HexMsgID21Tx1_CH :"&$HexMsgID21Tx1[3]&@CRLF)									; Fetch the Channel
	consolewrite("$HexMsgID21Tx1_ID :"&$HexMsgID21Tx1[4]&@CRLF)                                 ; Fetch the Hex Msg ID

	$FindTx2=FileReadLine ($GetLogFile_Path & "\LOG_06_Sample.log",19)							; Read the 19th line from the Log file
	consolewrite("$FindTx2 :"&$FindTx2&@CRLF)
	$HexMsgID21Tx2=StringSplit($FindTx2," ")
	consolewrite("$HexMsgID21Tx2_Dir :"&$HexMsgID21Tx2[2]&@CRLF)								; Fetch the Direction
	consolewrite("$HexMsgID21Tx2_CH :"&$HexMsgID21Tx2[3]&@CRLF)									; Fetch the Channel
	consolewrite("$HexMsgID21Tx2_ID :"&$HexMsgID21Tx2[4]&@CRLF)                                 ; Fetch the Hex Msg ID

	$FindTx3=FileReadLine ($GetLogFile_Path & "\LOG_06_Sample.log",21)							; Read the 21th line from the Log file
	consolewrite("$FindTx3 :"&$FindTx3&@CRLF)
	$HexMsgID21Tx3=StringSplit($FindTx3," ")
	consolewrite("$HexMsgID21Tx3_Dir :"&$HexMsgID21Tx3[2]&@CRLF)								; Fetch the Direction
	consolewrite("$HexMsgID21Tx3_CH :"&$HexMsgID21Tx3[3]&@CRLF)									; Fetch the Channel
	consolewrite("$HexMsgID21Tx3_ID :"&$HexMsgID21Tx3[4]&@CRLF)                                 ; Fetch the Hex Msg ID


	if $HexMsgID21Tx1[2]="Tx" and $HexMsgID21Tx2[2]="Tx" and $HexMsgID21Tx3[2]="Tx" then            ; Check Direction
	$TxAllDir="Found"
Else
	$TxAllDir="Not Found"
EndIf

consolewrite("$TxAllDir :"&$TxAllDir&@CRLF)

if $HexMsgID21Tx1[3]=1 and $HexMsgID21Tx2[3]=1 and $HexMsgID21Tx3[3]=1 then                     ; Chech Channel
	$TxAllCH="Found"
Else
	$TxAllCH="Not Found"
EndIf

consolewrite("$TxAllCH :"&$TxAllCH&@CRLF)

if $HexMsgID21Tx1[4]=0002 and $HexMsgID21Tx2[4]=0002 and $HexMsgID21Tx3[4]=0002 then            ; Check ID
	$TxAllID="Found"
Else
	$TxAllID="Not Found"
EndIf

EndIf


consolewrite("$TxAllID :"&$TxAllID&@CRLF)

if $TxAllDir="Found" and $TxAllCH="Found" and $TxAllID="Found" Then
	_WriteResult("Pass","TS_LIN_Log_06")
Else
	_WriteResult("Fail","TS_LIN_Log_06")
EndIf

$isAppNotRes=_CloseApp()															           ; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_LIN_Log_06")
EndIf


ConsoleWrite("****End : TS_LIN_Log_06.au3****"&@CRLF)
ConsoleWrite(@CRLF)