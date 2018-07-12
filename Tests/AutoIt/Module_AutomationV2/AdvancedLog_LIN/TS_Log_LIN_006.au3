;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_Log_LIN_006
; TestCases			:		Configuring Logging for "every bus connect for transmission" and "after each file size exceeding"
; Test Data			:		-
; Test Setup		:		1. Add a log file with file name as "SampleLog".
;							2. Select create New Log File "for every bus connect for transmission" and "after every hours/mins". Enter 0 hrs 3 mins.
;							3. Enter no. of files to be created as 3 and insert comments
;							4. Enable logging during tool connect/disconnect
;							5. Add messages to Tx window.
;							6. Connect, transmit CAN frames and disconnect. Repeat this 4 times.
;							7. Check log file list and log file details.
; Expected Results  : 		1. After step 2, a new log file with filename as "SampleLog_M0_T0.log" will be added in to the list.

;							2. After step 7, log File created is appended _M for measurement and _T for time .
 ; 								a. On every connect new log file will get created with incremented log file name _Mn if we disconnect and connect before the specified time is reached. Three log files will be created.
;									"SampleLog_M0_T0.log"
;									"SampleLog_M1_T0.log"
;									"SampleLog_M2_T0.log"
 ; 								b. if the specified time is reached. Three log files will be created.
;									"SampleLog_M0_T0.log"
;									"SampleLog_M0_T1.log"
;									"SampleLog_M0_T2.log"

;							3. Once the number of log files created exceeds the value specified by the user, incremental log file creation will be stopped and starts overwriting from the first file irrespective of append or overwrite

;							4. All the log files will have start of comment and end of comment entry.
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("***********Advanced logging for LIN Script Execution Started************"&@CRLF)
ConsoleWrite("****Start : TS_Log_LIN_006.au3****"&@CRLF)

Local $validationObject1=0                                                               					    	  				    ;To validate whether the log file is adding to the listview
Local $validationObject2=0																				 							    ;To validate whether it is creating 3 log files for each connect and disconnect
Local $validationObject3=0                                                                                                              ;To validate whether new file is getting created after exceeding the time limit
Local $validationObject4=0																											    ;To validate whether it is overwriting the file for "every bus connect for transmission" config
Local $validationObject5=0																											    ;To validate whether it is overwriting the file for ""after every hours/mins"" config
Local $validationObject6=0																											    ;To validate the  start of comment and end of comment entry.
local $Time1=0
local $Time2=0
local $Time3=0

_launchApp()																					                         		 	    ;Check if the App is open else launch it

ProcessWait("BUSMASTER.exe")																	                        			    ;Pauses script execution until a given process exists.
sleep(4000)

WinActivate($WIN_BUSMASTER,3)

if winexists($WIN_BUSMASTER) then
	_loadConfig("AdvancedLogLIN")
	_SetViewToolBarLIN()
	Sleep(1000)
;Loading the already existing config
	Sleep(1000)
	_linDriverSelection()																							 				    ;DriverSelection
	_openLINClusterConfig()
	WinWaitActive($WIN_ClustrConfigLIN)
	if winexists($WIN_ClustrConfigLIN) then
		ControlCommand($WIN_ClustrConfigLIN, "",$chckBoxEnableMaster, "Check", "") 					                 				    ;Select Check box for enable Master mode.
		ControlClick($WIN_ClusterConfiguration,"",$BTN_OK_ClusterConfig)											 				    ;Click on ok button for lin cluster configuration
	EndIf
	_ConfigureLINLogWithLogpath("SampleLog") 																		 				    ;Configuring Can log with only file name

	if winexists($WIN_LINLog) then
		ControlCommand($WIN_LINLog, "", $CHKB_OnTransmission_ConfigLOG, "Check")														;Check the checkbox to create log for every bus connect
		Sleep(1000)
		ControlCommand($WIN_LINLog,"",$CHKB_LogOnTime_ConfigLOG,"Check")														        ;Check the checkbox to create log in certain time limit
		$LogMinutesHWD=controlgethandle($WIN_LINLog,"",$EDIT_Minutes_ConfigLOG)															;Geth the handler for the edit box to enter minutes
		_GUICtrlEdit_SetText($LogMinutesHWD,"1")                                                                                        ;Set the value '1' for the minutes
		ControlCommand($WIN_LINLog,"",$BTN_EnDisable_ConfigLOG,"Check")                                                                 ;Check "&Enable/Disable Logging during tool Connect/Disconnect"

		$logfilehwd = ControlGetHandle($WIN_LINLog, "", $LSTC_LogFile_ConfigLOG)									                    ;Get the handler for the list view to show the logfiles added
		$logfilePath = _GUICtrlListView_GetItemTextString($logfilehwd, 0)										                        ;Get the first item in the list view
		ConsoleWrite("$logfilePath - :" & $logfilePath & @CRLF)

		If $logfilePath = _OutputDataPath() & "\SampleLog_M0_T0.log" Then                                                               ;Validating the log file path
			$validationObject1 = 1
		EndIf
        ConsoleWrite("$validationObject1 - For adding logfilepath to listview :" & $validationObject1 & @CRLF)

		$NumberOfLogfilesHWD=controlgethandle($WIN_LINLog,"",$EDIT_NumberOfLogfiles_ConfigLOG)										    ;Fetch NumberOfLogfiles Edit control handle
		_GUICtrlEdit_SetText($NumberOfLogfilesHWD,"3")																				    ;Set the maximum number of files to be created as 3
        ControlSend ( $WIN_LINLog, "", $EDIT_InsertComments_ConfigLOG, "Inserting Comments..." )                                        ;Inserting comments

		controlclick($WIN_LINLog,"",$BTN_OK_ConfigLOG)																					;Click on 'OK' button
	EndIf

	_ConnectDisconnect_LIN() 																											;Connect
	_ConnectDisconnect_LIN() 																											;DisConnect
	$logFile1 = LogFileExist("SampleLog_M0_T0")
	$logfile1Content1 = _GetlogFileData("SampleLog_M0_T0.log", "8")                                                                     ;Get the 8th row of the logfile to get the created date and time info to veryfy that it is overwritting
	if($logfile1Content1[0]>=6) then
		$Time1 = $logfile1Content1[6]																				                        ;Get the created time of the log file 1 for the "after every hours/mins" config
	EndIf
	ConsoleWrite("$Time1  :" & $Time1 & @CRLF)


	_ConnectDisconnect_LIN() 																											;Connect
	_ConnectDisconnect_LIN() 																											;DisConnect
	$logFile2 = LogFileExist("SampleLog_M1_T0")

	_ConnectDisconnect_LIN() 																											;Connect
	_ConnectDisconnect_LIN() 																							  				;DisConnect
	$logFile3 = LogFileExist("SampleLog_M2_T0")

	ConsoleWrite("$logFile1  :" & $logFile1 & @CRLF)
	ConsoleWrite("$logFile2  :" & $logFile2 & @CRLF)
	ConsoleWrite("$logFile3  :" & $logFile3 & @CRLF)

	If $logFile1 = 1 And $logFile2 = 1 And $logFile3 = 1 Then
		$validationObject2 = 1
	EndIf

	ConsoleWrite("$validationObject2 - For creating 3 log files for each connect and disconnect :" & $validationObject2 & @CRLF)



	_ConnectDisconnect_LIN()
	 sleep(70000)
     $logFile1ForTime=LogFileExist("SampleLog_M0_T0")
	 if $logFile1ForTime=1 Then
		$logfile1Content1 = _GetlogFileData("SampleLog_M0_T0.log", "8")																	;Get the 8th row of the logfile to get the created date and time info to veryfy that it is overwritting
		if($logfile1Content1[0]>=6) then
			$Time2 = $logfile1Content1[6]																	                                ;Get the created time of the log file
		EndIf
		ConsoleWrite("$Time2  :" & $Time2 & @CRLF)
	EndIf
	sleep(30000)
	sleep(30000)
	sleep(30000)
	sleep(30000)

	_ConnectDisconnect_LIN()																											;Disconnect

 	$logFile2ForTime=LogFileExist("SampleLog_M0_T1")
	$logFile3ForTime=LogFileExist("SampleLog_M0_T2")

	ConsoleWrite("$logFile1ForTime  :" & $logFile1ForTime & @CRLF)
	ConsoleWrite("$logFile2ForTime  :" & $logFile2ForTime & @CRLF)
	ConsoleWrite("$logFile3ForTime  :" & $logFile3ForTime & @CRLF)

	if $logFile2ForTime=1 And $logFile3ForTime=1 Then
		$validationObject3=1
	EndIf

    ConsoleWrite("$validationObject3 - for creating logfiles after 3 min :" &$validationObject3 & @crlf)

	$logfile1Content2 = _GetlogFileData("SampleLog_M0_T0.log", "8")																	    ;Get the 8th row of the logfile to get the created date and time info to veryfy that it is overwritting
	if($logfile1Content2[0]>=6) then
		$Time3 = $logfile1Content2[6]																				    					;Get the created time of the log file
	EndIf
	ConsoleWrite("$Time3  :" & $Time3 & @CRLF)



	$number = _CheckLogfileTimeIsEqual($Time1, $Time2)
	ConsoleWrite("$number - To check logfile created times are equal :" & $number & @CRLF)
	If $number = 0 Then
		$validationObject4 = 1
	EndIf
	ConsoleWrite("$validationObject4 - To check whether it is overwriting the file for each connect  :" & $validationObject4 & @CRLF)


	$number1 = _CheckLogfileTimeIsEqual($Time2, $Time3)
	ConsoleWrite("$number1 - To check logfile created times are equal :" & $number1 & @CRLF)
	If $number1 = 0 Then
		$validationObject5 = 1
	EndIf
	ConsoleWrite("$validationObject5 - To check whether it is overwriting the file for ""after every hours/mins"" config :" & $validationObject5 & @CRLF)

    $Ret1 = _CheckStartAndEndOfLogfile("SampleLog_M0_T0.log")                                                            				;Check the start and end of comment entry
	$Ret2 = _CheckStartAndEndOfLogfile("SampleLog_M1_T0.log")                                                            				;Check the start and end of comment entry
	$Ret3 = _CheckStartAndEndOfLogfile("SampleLog_M2_T0.log")                                                            				;Check the start and end of comment entry
	$Ret4 = _CheckStartAndEndOfLogfile("SampleLog_M0_T1.log")                                                            				;Check the start and end of comment entry
	$Ret5 = _CheckStartAndEndOfLogfile("SampleLog_M0_T2.log")                                                            				;Check the start and end of comment entry

	ConsoleWrite("$Ret1-" & $Ret1 & @CRLF)
	ConsoleWrite("$Ret2-" & $Ret2 & @CRLF)
	ConsoleWrite("$Ret3-" & $Ret3 & @CRLF)
	ConsoleWrite("$Ret4-" & $Ret4 & @CRLF)
	ConsoleWrite("$Ret5-" & $Ret5 & @CRLF)

	If $Ret1 = 1 And $Ret2 = 1 And $Ret3 = 1 And $Ret4 = 1 And $Ret5 = 1 Then
		$validationObject6 = 1
	EndIf

	ConsoleWrite("$validationObject1 - For adding logfilepath to listview :" & $validationObject1 & @CRLF)
	ConsoleWrite("$validationObject2 - For creating 3 log files for each connect and disconnect :" & $validationObject2 & @CRLF)
	ConsoleWrite("$validationObject3 - for creating logfiles after 3 min :" &$validationObject3 & @crlf)
	ConsoleWrite("$validationObject4 - To check whether it is overwriting the file for each connect  :" & $validationObject4 & @CRLF)
	ConsoleWrite("$validationObject5 - To check whether it is overwriting the file for ""after every hours/mins"" config :" & $validationObject5 & @CRLF)
 	ConsoleWrite("$validationObject6 - To check the comment entry in the logfile :" & $validationObject6 & @CRLF)


	If $validationObject1 = 1 And $validationObject2 = 1 And $validationObject3 = 1 And $validationObject4 = 1 And $validationObject5 = 1 And $validationObject6 = 1 Then
		_WriteResult("Pass", "TS_Log_LIN_006")
	Else
		_WriteResult("Fail", "TS_Log_LIN_006")
	EndIf

	_CloseApp()


EndIf

FileDelete ( _OutputDataPath() )
ConsoleWrite("****End : TS_LOG_LIN_06.au3****"&@CRLF)
ConsoleWrite(@CRLF)