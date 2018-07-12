;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_Log_LIN_003
; TestCases			:		Configuring Logging for "after each file size exceeding"
; Test Data			:		-
; Test Setup		:		1. Add a log file with file name as "SampleLog".
;							2. Select create New Log File "after each file size exceeding". Enter file size as 5 MB.
;							3. Enter no. of files to be created as 3 and insert comments
;							4. Enable logging during tool connect/disconnect
;							5. Add messages to Tx window.
;							6. Connect, transmit CAN frames for 10 mins
;							7. Check log file list and log file details.

; Expected Results  : 		1. After step 2, a new log file with filename as "SampleLog_S0.log" will be added in to the list. New log file created is appended with ‘_S’ and counter, if the current file size exceeds the specified file size with incremented log file name. Three log files will be created.
;								"SampleLog_S0.log"
;								"SampleLog_S1.log"
;								"SampleLog_S2.log"

;							2. Once the number of log files created exceeds the value specified by the user, incremental log file creation will be stopped and starts overwriting from the first file irrespective of append or overwrite.

;							3. All the log files will have start of comment and end of comment entry.
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("***********Advanced logging for LIN Script Execution Started************"&@CRLF)
ConsoleWrite("****Start : TS_Log_LIN_003.au3****"&@CRLF)

Local $validationObject1=0                                                               					    	  				   ;To validate whether the log file is adding to the listview
Local $validationObject2=0																				 							   ;To validate whether new file is getting created after exceeding the size given
Local $validationObject3=0                                                                                                             ;To validate whether new file is getting created after exceeding the size given
Local $validationObject4=0																											   ;To validate whether it is overwriting the file
Local $validationObject5=0																											   ;To validate the  start of comment and end of comment entry.
$logfile1CreatedTime1=0
$logfile1CreatedTime2=0


_launchApp()																					                         		 	   ;Check if the App is open else launch it

ProcessWait("BUSMASTER.exe")																	                        			   ;Pauses script execution until a given process exists.
sleep(4000)

WinActivate($WIN_BUSMASTER,3)

if winexists($WIN_BUSMASTER) then
	_loadConfig("AdvancedLogLIN")																					 					;Loading the already existing config
	_SetViewToolBarLIN()
	Sleep(1000)
	_linDriverSelection()																												;DriverSelection
	_openLINClusterConfig()
	WinWaitActive($WIN_ClustrConfigLIN)
	if winexists($WIN_ClustrConfigLIN) then
		ControlCommand($WIN_ClustrConfigLIN, "",$chckBoxEnableMaster, "Check", "") 					                  					;Select Check box for enable Master mode.
		ControlClick($WIN_ClusterConfiguration,"",$BTN_OK_ClusterConfig)											  					;Click on ok button for lin cluster configuration
	EndIf
	_ConfigureLINLogWithLogpath("SampleLog") 																		  					;Configuring Can log with only file name

	if winexists($WIN_LINLog) then
		ControlCommand($WIN_LINLog,"",$CHKB_LogfileSize_ConfigLOG,"Check")														       ;Check the checkbox to create log after each file size exceeding the limit
		sleep(1000)
		$logfilehwd = ControlGetHandle($WIN_LINLog, "", $LSTC_LogFile_ConfigLOG)									                   ;Get the handler for the list view to show the logfiles added
		$logfilePath = _GUICtrlListView_GetItemTextString($logfilehwd, 0)										                       ;Get the first item in the list view
		 ConsoleWrite("$logfilePath - -------:" & $logfilePath & @CRLF)

		If $logfilePath = _OutputDataPath() & "\SampleLog_S0.log" Then                                                                 ;Validating the log file path
			$validationObject1 = 1
		EndIf
        ConsoleWrite("$validationObject1 - For adding logfilepath to listview :" & $validationObject1 & @CRLF)
		$LogfileSizeHWD=controlgethandle($WIN_LINLog,"",$EDIT_Filesize_ConfigLOG)													   ;Get the handler for the edit box to enter the logfilesize
		_GUICtrlEdit_SetText($LogfileSizeHWD,"1")                                                                                      ;Set the value '1' for the size of the log file
		$NumberOfLogfilesHWD=controlgethandle($WIN_LINLog,"",$EDIT_NumberOfLogfiles_ConfigLOG)										   ;Fetch NumberOfLogfiles Edit control handle
		_GUICtrlEdit_SetText($NumberOfLogfilesHWD,"3")																				   ;Set the no of logfiles to be created as 3

		ControlSend ( $WIN_LINLog, "", $EDIT_InsertComments_ConfigLOG, "Inserting Comments..." )                                       ;Inserting comments

		ControlCommand($WIN_LINLog,"",$BTN_EnDisable_ConfigLOG,"Check")                                                                ;Check "&Enable/Disable Logging during tool Connect/Disconnect"
		controlclick($WIN_LINLog,"",$BTN_OK_ConfigLOG)																				   ;Click on 'OK' button
		sleep(500)
	EndIf

	_ConnectDisconnect_LIN() 																										   ;Connect
	 sleep(120000)                                                                                                                      ;transmit for 10 sec so that the first file size exceeds 5 mb size
	_ConnectDisconnect_LIN()																										   ;DisConnect

 	$logFile1=LogFileExist("SampleLog_S0")
 	$logFile2=LogFileExist("SampleLog_S1")

	if $logFile1=1 And $logFile2=1 Then																								   ;Check whether it created 2 log files
		$logfile1Content1 = _GetlogFileData("SampleLog_S0.log", "8")											               		   ;Get the 8th row of the logfile to get the created date and time info to veryfy that it is overwritting
		$logfile1CreatedTime1 = $logfile1Content1[6]																				   ;Get the created time of the log file
		ConsoleWrite("$logfile1CreatedTime1  :" & $logfile1CreatedTime1 & @CRLF)
		$sizeinBytes = FileGetSize ( _OutputDataPath()&"\SampleLog_S0.log" )														   ;Get the file size in bytes
		$size=$sizeinBytes/1048576																									   ;Converting the size in MB
		if $size >1 Then																										       ;Checking whether it is greater than 5 MB
			$validationObject2=1
		EndIf
	EndIf
    ConsoleWrite("$validationObject2 - for exceeding size :" &$validationObject2 & @crlf)


	_ConnectDisconnect_LIN() 																											;Connect
	 sleep(120000)                                                                                                                       ;transmit for 10 sec so that it creates 3rd file after the 2nd one exceeding 5 MB
	_ConnectDisconnect_LIN()																											;Disconnect

	$logFile3=LogFileExist("SampleLog_S2")


	if $logFile3=1 Then
		$sizeinBytes = FileGetSize ( _OutputDataPath()&"\SampleLog_S1.log" )															;Get the 2nd file size
		$size=$sizeinBytes/1048576																										;Check whether the size is greater than 5 MB
		if $size >1 Then
			$validationObject3=1
		EndIf
	EndIf

    ConsoleWrite("$validationObject3 - for exceeding size :" &$validationObject3 & @crlf)

	_ConnectDisconnect_LIN() 																											;Connect
	 sleep(120000)
	_ConnectDisconnect_LIN()																											;Disconnect

	$logfile1Content2 = _GetlogFileData("SampleLog_S0.log", "8")																	    ;Get the 8th row of the logfile to get the created date and time info
	$logfile1CreatedTime2 = $logfile1Content2[6]																	                    ;Get the created time of the log file
	ConsoleWrite("$logfile1CreatedTime2  :" & $logfile1CreatedTime2 & @CRLF)

	$number = _CheckLogfileTimeIsEqual($logfile1CreatedTime1, $logfile1CreatedTime2)													;Check whether the log files created times are equal ,if not then it is overwritten
	ConsoleWrite("$number - To check logfile created times are equal :" & $number & @CRLF)
	If $number = 0 Then
		$validationObject4 = 1
	EndIf

	ConsoleWrite("$validationObject4 - To check the whether it is overwriting :" & $validationObject4 & @CRLF)

	$Ret1 = _CheckStartAndEndOfLogfile("SampleLog_S0.log")                                                            					;Check the start and end of comment entry
	$Ret2 = _CheckStartAndEndOfLogfile("SampleLog_S1.log")                                                            					;Check the start and end of comment entry
	$Ret3 = _CheckStartAndEndOfLogfile("SampleLog_S2.log")																				;Check the start and end of comment entry

	ConsoleWrite("$Ret1-" & $Ret1 & @CRLF)
	ConsoleWrite("$Ret2-" & $Ret2 & @CRLF)
	ConsoleWrite("$Ret3-" & $Ret3 & @CRLF)

	If $Ret1 = 1 And $Ret2 = 1 And $Ret3 = 1 Then
		$validationObject5 = 1
	EndIf

	ConsoleWrite("$validationObject1 - For adding logfilepath to listview :" & $validationObject1 & @CRLF)
	ConsoleWrite("$validationObject2 - for exceeding size :" &$validationObject2 & @crlf)
	ConsoleWrite("$validationObject3 - for exceeding size :" &$validationObject3 & @crlf)
	ConsoleWrite("$validationObject4 - To check the whether it is overwriting :" & $validationObject4 & @CRLF)
	ConsoleWrite("$validationObject5 - To check the comment entry in the logfile :" & $validationObject5 & @CRLF)

	If $validationObject1 = 1 And $validationObject2 = 1 And $validationObject3 = 1 And $validationObject4 = 1 And $validationObject5 = 1 Then
		_WriteResult("Pass", "TS_Log_LIN_003")
	Else
		_WriteResult("Fail", "TS_Log_LIN_003")
	EndIf

	_CloseApp()

EndIf

FileDelete ( _OutputDataPath() )
ConsoleWrite("****End : TS_LOG_LIN_03.au3****"&@CRLF)
ConsoleWrite(@CRLF)