;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_Log_LIN_002
; TestCases			:		Configuring Logging for "every bus connect for transmission"
; Test Data			:		-
; Test Setup		:		1. Add a log file with file name as "SampleLog".
;							2. Select create New Log File "for every bus connect for transmission".
;							3. Enter no. of files to be created as 3 and insert comments
;							4. Enable logging during tool connect/disconnect
;							5. Add messages to Tx window.
;							6. Connect, transmit CAN frames and disconnect. Repeat this 4 times.
;							7. Check log file list and log file details.

; Expected Results  : 		1. After step 2, a new log file with filename as "SampleLog_M0.log" will be added in to the list.
;
;							2. After step 7, log File created is appended _M for measurement. On every connect new log file will get created with incremented log file name _Mn. Three log files will be created.
;							"SampleLog_M0.log"
;							"SampleLog_M1.log"
;							"SampleLog_M2.log"
;							All the frames transmitted/received during the first connect will be logged in "SampleLog_M0.log"
;							All the frames transmitted/received during the second connect will be logged in "SampleLog_M1.log"
;							All the frames transmitted/received during the third connect will be logged in "SampleLog_M2.log"
;
;							3. Once the number of log files created exceeds the value specified by the user, incremental log file creation will be stopped and starts overwriting from the first file irrespective of append or overwrite.
;
;							4. All the log files will have start of comment and end of comment entry.
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("***********Advanced logging for LIN Script Execution Started************" & @CRLF)
ConsoleWrite("****Start : TS_Log_LIN_002.au3****" & @CRLF)

Local $validationObject1 = 0 																						 ;To validate the first expected results
Local $validationObject2 = 0 																						 ;To validate the second expected results
Local $validationObject3 = 0 																						 ;To validate the Third expected results
Local $validationObject4 = 0 																						 ;To validate the forth expected results
Local $validationObject5 = 0																						 ;To validate the logfile content

$logfile1CreatedTime1=0
$logfile1CreatedTime2=0


_launchApp() 																										 ;Check if the App is open else launch it

ProcessWait("BUSMASTER.exe") 																						 ;Pauses script execution until a given process exists.
Sleep(4000)

WinActivate($WIN_BUSMASTER, 3)

If WinExists($WIN_BUSMASTER) Then
	_loadConfig("AdvancedLogLIN")																					 ;Loading the already existing config
	_SetViewToolBarLIN()
	Sleep(1000)
	_linDriverSelection()																							 ;DriverSelection
	_openLINClusterConfig()
	WinWaitActive($WIN_ClustrConfigLIN)
	if winexists($WIN_ClustrConfigLIN) then
		ControlCommand($WIN_ClustrConfigLIN, "",$chckBoxEnableMaster, "Check", "") 					                 ;Select Check box for enable Master mode.
		ControlClick($WIN_ClusterConfiguration,"",$BTN_OK_ClusterConfig)											 ;Click on ok button for lin cluster configuration
	EndIf
	_ConfigureLINLogWithLogpath("SampleLog") 																		 ;Configuring Can log with only file name

	If WinExists($WIN_LINLog) Then
		ControlCommand($WIN_LINLog, "", $CHKB_OnTransmission_ConfigLOG, "Check")									 ;Check the checkbox to create log for every bus connect
		Sleep(1000)
		$logfilehwd = ControlGetHandle($WIN_LINLog, "", $LSTC_LogFile_ConfigLOG)									 ;Get the handler for the list view to show the logfiles added
		$logfilePath = _GUICtrlListView_GetItemTextString($logfilehwd, 0)										     ;Get the first item in the list view
		ConsoleWrite("$logfilePath : " & $logfilePath & @CRLF)

		If $logfilePath = _OutputDataPath() & "\SampleLog_M0.log" Then                                               ;Validating the log file path
			$validationObject1 = 1
		EndIf

		ConsoleWrite("$validationObject1 - For adding logfilepath to listview :" & $validationObject1 & @CRLF)
		$NumberOfLogfilesHWD = ControlGetHandle($WIN_LINLog, "", $EDIT_NumberOfLogfiles_ConfigLOG)					 ;Fetch NumberOfLogfiles Edit control handle
		_GUICtrlEdit_SetText($NumberOfLogfilesHWD, "3")																 ;Set the no of logfiles to be created as 3

		ControlSend ( $WIN_LINLog, "", $EDIT_InsertComments_ConfigLOG, "Inserting Comments..." )                     ;Inserting comments

		ControlCommand($WIN_LINLog, "", $BTN_EnDisable_ConfigLOG, "Check") 											 ;Check "&Enable/Disable Logging during tool Connect/Disconnect"
		ControlClick($WIN_LINLog, "", $BTN_OK_ConfigLOG)															 ;Click on 'OK' button
		Sleep(500)
	EndIf

	_ConnectDisconnect_LIN() 																						 ;Connect
	_ConnectDisconnect_LIN() 																						 ;DisConnect
	$logFile1 = LogFileExist("SampleLog_M0")

	if $logFile1=1 Then
		$logfile1Content1 = _GetlogFileData("SampleLog_M0.log", "8")													 ;Get the 8th row of the logfile to get the created date and time info
		$logfile1CreatedTime1 = $logfile1Content1[6]																	 ;Get the created time of the log file
		ConsoleWrite("$logfile1CreatedTime1  :" & $logfile1CreatedTime1 & @CRLF)
	EndIf


	_ConnectDisconnect_LIN() 																						 ;Connect
	_ConnectDisconnect_LIN() 																						 ;DisConnect
	$logFile2 = LogFileExist("SampleLog_M1")

	_ConnectDisconnect_LIN() 																						 ;Connect
	_ConnectDisconnect_LIN() 																						 ;DisConnect
	$logFile3 = LogFileExist("SampleLog_M2")

	_ConnectDisconnect_LIN() 																						 ;Connect
	_ConnectDisconnect_LIN() 																						 ;DisConnect

	ConsoleWrite("$logFile1  :" & $logFile1 & @CRLF)
	ConsoleWrite("$logFile2  :" & $logFile2 & @CRLF)
	ConsoleWrite("$logFile3  :" & $logFile3 & @CRLF)

	If $logFile1 = 1 And $logFile2 = 1 And $logFile3 = 1 Then														 ;Checking whether 3 log files are created
		$validationObject2 = 1
	EndIf
	ConsoleWrite("$validationObject2 - For creating 3 log files :" & $validationObject2 & @CRLF)


	$logFile1 = LogFileExist("SampleLog_M0")

	if $logFile1=1 Then
		$logfile1Content2 = _GetlogFileData("SampleLog_M0.log", "8")													 ;Get the 8th row of logfile after overwriting it
		$logfile1CreatedTime2 = $logfile1Content2[6]																	 ;Get the created time of the log file
		ConsoleWrite("$logfile1CreatedTime2  :" & $logfile1CreatedTime2 & @CRLF)
	EndIf

	$number = _CheckLogfileTimeIsEqual($logfile1CreatedTime1, $logfile1CreatedTime2)								 ;Check whether the logfile created times are equal
	ConsoleWrite("$number - To check logfile created times are equal :" & $number & @CRLF)
	If $number = 0 Then
		$validationObject3 = 1
	EndIf

	ConsoleWrite("$validationObject3 - To check whether it overwrites the logfile :" & $validationObject3 & @CRLF)

	$Ret1 = _CheckStartAndEndOfLogfile("SampleLog_M0.log")                                                            ;Check the start and end of comment entry
	$Ret2 = _CheckStartAndEndOfLogfile("SampleLog_M1.log")                                                            ;Check the start and end of comment entry
	$Ret3 = _CheckStartAndEndOfLogfile("SampleLog_M2.log")                                                            ;Check the start and end of comment entry
	ConsoleWrite("$Ret1-" & $Ret1 & @CRLF)
	ConsoleWrite("$Ret2-" & $Ret2 & @CRLF)
	ConsoleWrite("$Ret3-" & $Ret3 & @CRLF)

	If $Ret1 = 1 And $Ret2 = 1 And $Ret3 = 1 Then
		$validationObject4 = 1
	EndIf

	if $logFile1=1 Then
		$NoOfLines=_FileCountLines ( _OutputDataPath()&"\SampleLog_M0.log" )
		if $NoOfLines >=22 Then
			For $i = 19 To 22
			$logfileTransmitContent = _GetlogFileData("SampleLog_M0.log",$i)
			if $logfileTransmitContent[2]="Tx" And $logfileTransmitContent[3]="1" And $logfileTransmitContent[4]="0x010" And $logfileTransmitContent[5]="8" And $logfileTransmitContent[6]="FF" Then
				$validationObject5=1
			EndIf
			If $validationObject5 =0 Then ExitLoop
			Next
		EndIf
	EndIf

	ConsoleWrite("$validationObject1 - For adding logfilepath to listview :" & $validationObject1 & @CRLF)
	ConsoleWrite("$validationObject2 - For creating 3 log files :" & $validationObject2 & @CRLF)
	ConsoleWrite("$validationObject3 - To check whether it overwrites the logfile :" & $validationObject3 & @CRLF)
	ConsoleWrite("$validationObject4 - To check the comment entry in the logfile :" & $validationObject4 & @CRLF)
	ConsoleWrite("$validationObject5 - For logfile content " & $validationObject5 & @CRLF)


	If $validationObject1 = 1 And $validationObject2 = 1 And $validationObject3 = 1 And $validationObject4 = 1 And $validationObject5 = 1 Then
		_WriteResult("Pass", "TS_Log_LIN_002")
	Else
		_WriteResult("Fail", "TS_Log_LIN_002")
	EndIf

	_CloseApp()

EndIf

FileDelete ( _OutputDataPath() )
ConsoleWrite("****End : TS_LOG_LIN_02.au3****" & @CRLF)
ConsoleWrite(@CRLF)