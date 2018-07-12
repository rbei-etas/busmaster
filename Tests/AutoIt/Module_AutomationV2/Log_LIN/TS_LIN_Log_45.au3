;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_LIN_Log_45
; TestCases			:		Functionality test - Enable log
; Test Data			:
; Test Setup		:		1. Select Log from Configuration menu.
;							2. disable all log files by unchecking all check boxes associated with the log file.
;							3. Select OK to save changes.
;							4. Connect the tool and start frame transmission. Start logging.
;							5. Disconnect the tool and stop logging.
;							6. Check the log files status.

; Expected Results  : 		1. As log files are not enabled, log files will remain unchanged.
;==========================================================================Test Procedure =========================================================================#include <IncludeFiles.au3>

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_LIN_Log_45.au3****" & @CRLF)

local $data1
local $data2

_launchApp() 																										 ;Check if the App is open else launch it

ProcessWait("BUSMASTER.exe") 																						 ;Pauses script execution until a given process exists.
Sleep(4000)

WinActivate($WIN_BUSMASTER, 3)

If WinExists($WIN_BUSMASTER) Then
	Sleep(1000)
	_loadConfig("cfxLogging_49_LIN")																		 ;Loading the already existing config
	Sleep(1000)
	_linDriverSelection()																							 ;DriverSelection
	Sleep(1000)
	_ConfigureLINLogWithLogpath("SampleLog_45") 																	 ;Configuring Can log with only file name
	ControlCommand ($WIN_LINLog,"",$BTN_EnDisable_ConfigLOG,"Check")							; Check Enable/Disable Logging during connect/disconnect
	ControlClick($WIN_LINLog,"",$BTN_OK_ConfigLOG)                                                                   ;Click on the ok button
EndIf

_ConnectDisconnect_LIN() 																						 ;Connect
_ConnectDisconnect_LIN() 																						 ;Connect

$read1=_FileReadToArray(_OutputDataPath() & "\SampleLog_45.log",$data1)


_LINLogMenu()																								  ;Open LIN->Logging->Configure
If WinExists($WIN_LINLog) Then
	$logfilehwd = ControlGetHandle($WIN_LINLog, "", $LSTC_LogFile_ConfigLOG)									  ;Get the handler for the list view to show the logfiles added
	_GUICtrlListView_SetItemChecked($logfilehwd,0,False)														  ;Uncheck the first item
	ControlClick($WIN_LINLog,"",$BTN_OK_ConfigLOG)                                                                   ;Click on the ok button
EndIf

_ConnectDisconnect_LIN() 																						 ;Connect
_ConnectDisconnect_LIN() 																						 ;Connect


$read2=_FileReadToArray(_OutputDataPath() & "\SampleLog_45.log",$data2)
if($read1=True and $read2=True) Then
	consolewrite("$data1[0] : "&$data1[0]&@CRLF)
	consolewrite("$data2[0] : "&$data2[0]&@CRLF)
EndIf

if  $data1[0]=$data2[0] then
	_WriteResult("Pass","TS_LIN_Log_45")
	ConsoleWrite("Pass" & @CRLF)
Else
	_WriteResult("Fail","TS_LIN_Log_45")
	ConsoleWrite("Fail" & @CRLF)
EndIf

$isAppNotRes= _CloseApp()

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_LIN_Log_45")
EndIf


ConsoleWrite("****End : TS_LIN_Log_45.au3****"&@CRLF)
ConsoleWrite(@CRLF)












