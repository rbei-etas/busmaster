;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_Log_16
; TestCases			:		Using the Filter Attributes
; Test Data			:
; Test Setup		:		1. Open 'Configure Filters' dialog by menu option 'Configure -> App Filters'.
;~ 							2. Make a filter with the following attributes:
;~ 							3. Choose 'ID' radio box in 'Filter Attributes' group box.
;~ 							4. Select an 'ID' from the combobox or manually enter it.
;~ 							5. Choose 'ID Type' as ''std'.
;~ 							6. Choose 'Frame' as 'All'.
;~ 							7. Choose 'Direction' as 'Tx'.
;~ 							8. Choose 'Channel' as 'All'.
;~ 							9. Add the filter by clicking on Add button and close the 'Configure Filters' dialog by clicking on 'OK'.
;~ 							10. Open 'ConfigureLogging for CAN' window by menu 'Configure->Log' and add a new log file.
;~ 							11. Choose the configured filter by opening 'Filter Selection dialog' and close the  'ConfigureLogging for CAN' window by 'Ok' click.
;~ 							12. Now click on connect and enable logging by menu 'Functions->Enable Filter->Log'.
;~ 							13. Transmit a message which matches the filter attributes set for configured filter. Observe the log file contents after transmission.

; Expected Results  : 		1. The log file should not contain any messages which matches the filter attributes configured for this log file.
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_Log_16.au3****"&@CRLF)


_launchApp()

Dim $HexMsgID21Tx1[10]=["","","","","","","","","",""],$HexMsgID21Tx2[10]=["","","","","","","","","",""],$HexMsgID21Tx3[10]=["","","","","","","","","",""],$HexMsgID21Tx4[10]=["","","","","","","","","",""]
if winexists($WIN_BUSMASTER) then
	_loadConfig("cfxLog_16")															; Load Configuration

	_ConfigureCANLog("LOG_16_Sample","System","ALL","Decimal","Overwrite","","","True")	; Configure Logging details

	_EnableFilterLog()
	sleep(1000)

	_ConnectDisconnect()																; Connect the tool

	_TransmitMsgsMenu()																	; Transmit normal blocks
	sleep(1500)

	_ConnectDisconnect()																; Disconnect the tool

	Sleep(2000)
	_DisableJ1939Win()																	; If J1939 Msg Window is active then disable it

EndIf

$GetLogFile_Path=_OutputDataPath()														; Fetch the Log file path
ConsoleWrite("$GetLogFile_Path:"&$GetLogFile_Path&@CRLF)

if _FileCountLines($GetLogFile_Path & "\Log_16_Sample.log")>=19 Then
	$FindTx1=FileReadLine ($GetLogFile_Path & "\LOG_16_Sample.log",16)						; Read the 17th line from the Log file
	consolewrite("$FindTx1 :"&$FindTx1&@CRLF)
	$HexMsgID21Tx1=StringSplit($FindTx1," ")
	consolewrite("$HexMsgID21Tx1_Dir :"&$HexMsgID21Tx1[2]&@CRLF)							; Fetch the Direction
	consolewrite("$HexMsgID21Tx1_CH :"&$HexMsgID21Tx1[3]&@CRLF)								; Fetch the channel
	consolewrite("$HexMsgID21Tx1_Type :"&$HexMsgID21Tx1[5]&@CRLF)							; Fetch type

	$FindTx2=FileReadLine ($GetLogFile_Path & "\LOG_16_Sample.log",17)						; Read the 19th line from the Log file
	consolewrite("$FindTx2 :"&$FindTx2&@CRLF)
	$HexMsgID21Tx2=StringSplit($FindTx2," ")
	consolewrite("$HexMsgID21Tx2_Dir :"&$HexMsgID21Tx2[2]&@CRLF)							; Fetch the Direction
	consolewrite("$HexMsgID21Tx2_CH :"&$HexMsgID21Tx2[3]&@CRLF)								; Fetch the channel
	consolewrite("$HexMsgID21Tx2_Type :"&$HexMsgID21Tx2[5]&@CRLF)							; Fetch type

	$FindTx3=FileReadLine ($GetLogFile_Path & "\LOG_16_Sample.log",18)						; Read the 21th line from the Log file
	consolewrite("$FindTx3 :"&$FindTx3&@CRLF)
	$HexMsgID21Tx3=StringSplit($FindTx3," ")
	consolewrite("$HexMsgID21Tx3_Dir :"&$HexMsgID21Tx3[2]&@CRLF)							; Fetch the Direction
	consolewrite("$HexMsgID21Tx3_CH :"&$HexMsgID21Tx3[3]&@CRLF)								; Fetch the channel
	consolewrite("$HexMsgID21Tx3_Type :"&$HexMsgID21Tx3[5]&@CRLF)							; Fetch type

	$FindTx4=FileReadLine ($GetLogFile_Path & "\LOG_16_Sample.log",19)						; Read the 21th line from the Log file
	consolewrite("$FindTx4 :"&$FindTx4&@CRLF)
	$HexMsgID21Tx4=StringSplit($FindTx4," ")
	consolewrite("$HexMsgID21Tx4_Dir :"&$HexMsgID21Tx4[2]&@CRLF)							; Fetch the Direction
	consolewrite("$HexMsgID21Tx4_CH :"&$HexMsgID21Tx4[3]&@CRLF)								; Fetch the channel
	consolewrite("$HexMsgID21Tx4_Type :"&$HexMsgID21Tx4[5]&@CRLF)							; Fetch type
EndIf

if $HexMsgID21Tx1[2]="Rx" and $HexMsgID21Tx2[2]="Rx" and $HexMsgID21Tx3[2]="Rx" and $HexMsgID21Tx4[2]="Rx" then
	$TxAllDir="Found"
Else
	$TxAllDir="Not Found"
EndIf

consolewrite("$TxAllDir :"&$TxAllDir&@CRLF)

if $HexMsgID21Tx1[3]=1 and $HexMsgID21Tx2[3]=1 and $HexMsgID21Tx3[3]=1 and $HexMsgID21Tx4[3]=1 then
	$TxAllCH="Found"
Else
	$TxAllCH="Not Found"
EndIf

consolewrite("$TxAllCH :"&$TxAllCH&@CRLF)

if $HexMsgID21Tx1[5]="sr" and $HexMsgID21Tx2[5]="sr"  and $HexMsgID21Tx3[5]="sr"  and $HexMsgID21Tx4[5]="sr" then
	$TxAllType="Found"
Else
	$TxAllType="Not Found"
EndIf

consolewrite("$TxAllType :"&$TxAllType&@CRLF)

if $TxAllDir="Found" and $TxAllCH="Found" and $TxAllType="Found" Then
	_WriteResult("Pass","TS_Log_16")
Else
	_WriteResult("Fail","TS_Log_16")
EndIf

$isAppNotRes=_CloseApp()															; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_Log_16")
EndIf

ConsoleWrite("****End : TS_Log_16.au3****"&@CRLF)
ConsoleWrite(@CRLF)