;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_Log_004
; TestCases			:		Configuring Logging for "after each file size exceeding"
; Test Data			:		-
; Test Setup		:		1. Add a log file with file name as "SampleLog".
;							2. Select create New Log File "after every hours/mins". Enter 0 hrs 3 mins.
;							3. Enter no. of files to be created as 3 and insert comments
;							4. Enable logging during tool connect/disconnect
;							5. Add messages to Tx window.
;							6. Connect, transmit CAN frames for 10 mins
;							7. Check log file list and log file details.

; Expected Results  : 		1. After step 2, a new log file with filename as "SampleLog_T0.log" will be added in to the list. New log file created is appended with ‘_T’ and counter, for every specified time with incremented log file name. Three log files will be created.
;								"SampleLog_T0.log"
;								"SampleLog_T1.log"
;								"SampleLog_T2.log"

;							2. Once the number of log files created exceeds the value specified by the user, incremental log file creation will be stopped and starts overwriting from the first file irrespective of append or overwrite.

;							3. All the log files will have start of comment and end of comment entry.
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("***********Advanced logging for CAN Script Execution Started************"&@CRLF)
ConsoleWrite("****Start : TS_Log_004.au3****"&@CRLF)

Local $validationObject1=0                                                               					    	  				   ;To validate whether the log file is adding to the listview
Local $validationObject2=0																				 							   ;To validate whether new file is getting created after exceeding the size given
Local $validationObject3=0                                                                                                             ;To validate whether new file is getting created after exceeding the size given
Local $validationObject4=0																											   ;To validate whether it is overwriting the file
;Local $validationObject5=0																											   ;To validate the  start of comment and end of comment entry.
local $logfile1CreatedTime1=0
local $logfile1CreatedTime2=0


_launchApp()																					                         		 	   ;Check if the App is open else launch it

ProcessWait("BUSMASTER.exe")																	                        			   ;Pauses script execution until a given process exists.
sleep(4000)

WinActivate($WIN_BUSMASTER,3)

if winexists($WIN_BUSMASTER) then
	_loadConfig("AdvancedLog_1")																										   ;Loading the already existing config
	sleep(1000)
	_ConfigureCANLogWithLogpath("SampleLog")																						   ;Configuring Can log with only file name

	if winexists($WIN_CANLog) then
		ControlCommand($WIN_CANLog,"",$CHKB_LogOnTime_ConfigLOG,"Check")														       ;Check the checkbox to create log in certain time limit
		sleep(1000)
		$logfilehwd = ControlGetHandle($WIN_CANLog, "", $LSTC_LogFile_ConfigLOG)
		$logfilePath = _GUICtrlListView_GetItemTextString($logfilehwd, 0)
		 ConsoleWrite("$logfilePath - :" & $logfilePath & @CRLF)

		If $logfilePath = _OutputDataPath() & "\SampleLog_T0.log" Then
			$validationObject1 = 1
		EndIf
        ConsoleWrite("$validationObject1 - For adding logfilepath to listview :" & $validationObject1 & @CRLF)
		$LogMinutesHWD=controlgethandle($WIN_CANLog,"",$EDIT_Minutes_ConfigLOG)
		_GUICtrlEdit_SetText($LogMinutesHWD,"1")                                                                                        ;Enter the value '1' for the minutes
		$NumberOfLogfilesHWD=controlgethandle($WIN_CANLog,"",$EDIT_NumberOfLogfiles_ConfigLOG)										    ;Fetch NumberOfLogfiles Edit control handle
		_GUICtrlEdit_SetText($NumberOfLogfilesHWD,"3")

		ControlSend ( $WIN_CANLog, "", $EDIT_InsertComments_ConfigLOG, "Inserting Comments..." )                                        ;Inserting comments

		ControlCommand($WIN_CANLog,"",$BTN_EnDisable_ConfigLOG,"Check")                                                                 ;Check "&Enable/Disable Logging during tool Connect/Disconnect"
		controlclick($WIN_CANLog,"",$BTN_OK_ConfigLOG)																					;Click on 'OK' button
	EndIf

	_ConnectDisconnect()
	 sleep(70000)
     $logFile1=LogFileExist("SampleLog_T0")
	 if $logFile1=1 Then
		$logfile1Content1 = _GetlogFileData("SampleLog_T0.log", "8")
		$logfile1CreatedTime1 = $logfile1Content1[6]
		ConsoleWrite("$logfile1CreatedTime1  :" & $logfile1CreatedTime1 & @CRLF)
	EndIf
    sleep(30000)
	sleep(30000)
	sleep(30000)
	sleep(30000)

	_ConnectDisconnect()

 	$logFile2=LogFileExist("SampleLog_T1")
	$logFile3=LogFileExist("SampleLog_T2")

	if $logFile2=1 And $logFile3=1 Then
		$validationObject2=1
	EndIf

	ConsoleWrite("$logFile1  :" & $logFile1 & @CRLF)
	ConsoleWrite("$logFile2  :" & $logFile2 & @CRLF)
	ConsoleWrite("$logFile3  :" & $logFile3 & @CRLF)

    ConsoleWrite("$validationObject2 - for creating logfiles after 3 min :" &$validationObject2 & @crlf)

	$logfile1Content2 = _GetlogFileData("SampleLog_T0.log", "8")
	$logfile1CreatedTime2 = $logfile1Content2[6]
	ConsoleWrite("$logfile1CreatedTime2  :" & $logfile1CreatedTime2 & @CRLF)

	$number = _CheckLogfileTimeIsEqual($logfile1CreatedTime1, $logfile1CreatedTime2)
	ConsoleWrite("$number - To check logfile created times are equal :" & $number & @CRLF)
	If $number = 0 Then
		$validationObject3 = 1
	EndIf
	ConsoleWrite("$validationObject3 - To check whether it is overwriting the file :" & $validationObject3 & @CRLF)

	$Ret1 = _CheckStartAndEndOfLogfile("SampleLog_T0.log")
	$Ret2 = _CheckStartAndEndOfLogfile("SampleLog_T1.log")
	$Ret3 = _CheckStartAndEndOfLogfile("SampleLog_T2.log")

	ConsoleWrite("$Ret1-" & $Ret1 & @CRLF)
	ConsoleWrite("$Ret2-" & $Ret2 & @CRLF)
	ConsoleWrite("$Ret3-" & $Ret3 & @CRLF)

	If $Ret1 = 1 And $Ret2 = 1 And $Ret3 = 1 Then
		$validationObject4 = 1
	EndIf

	ConsoleWrite("$validationObject1 - For adding logfilepath to listview :" & $validationObject1 & @CRLF)
	ConsoleWrite("$validationObject2 - for creating logfiles after 3 min :" &$validationObject2 & @crlf)
	ConsoleWrite("$validationObject3 - To check whether it is overwriting the file :" & $validationObject3 & @CRLF)
	ConsoleWrite("$validationObject4 - To check the comment entry in the logfile :" & $validationObject4 & @CRLF)

	If $validationObject1 = 1 And $validationObject2 = 1 And $validationObject3 = 1 And $validationObject4 = 1 Then
		_WriteResult("Pass", "TS_Log_004")
	Else
		_WriteResult("Fail", "TS_Log_004")
	EndIf

	_CloseApp()


EndIf

FileDelete ( _OutputDataPath() )
ConsoleWrite("****End : TS_LOG_04.au3****"&@CRLF)
ConsoleWrite(@CRLF)