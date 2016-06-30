;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_Log_LIN_005
; TestCases			:		Configuring Logging for "every bus connect for transmission" and "after each file size exceeding"
; Test Data			:		-
; Test Setup		:		1. Add a log file with file name as "SampleLog".
;							2. Select create New Log File "for every bus connect for transmission" and  "after each file size exceeding". Enter file size as 5 MB.
;							3. Enter no. of files to be created as 3 and insert comments
;							4. Enable logging during tool connect/disconnect
;							5. Add messages to Tx window.
;							6. Connect, transmit CAN frames and disconnect. Repeat this 4 times.
;							7. Check log file list and log file details.

; Expected Results  : 		1. After step 2, a new log file with filename as "SampleLog_M0_S0.log" will be added in to the list.

;							2. After step 7, log File created is appended _M for measurement and _S for size .
 ;								 a. On every connect new log file will get created with incremented log file name _Mn if we disconnect and connect before the mentioned file size exceeds. Three log files will be created.
;									"SampleLog_M0_S0.log"
;									"SampleLog_M1_S0.log"
;									"SampleLog_M2_S0.log"
;  								b. if the current file size exceeds the specified file size with incremented log file name. Three log files will be created.
;									"SampleLog_M0_S0.log"
;									"SampleLog_M0_S1.log"
;									"SampleLog_M0_S2.log"

;							3. Once the number of log files created exceeds the value specified by the user, incremental log file creation will be stopped and starts overwriting from the first file irrespective of append or overwrite

;							4. All the log files will have start of comment and end of comment entry.
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("***********Advanced logging for LIN Script Execution Started************"&@CRLF)
ConsoleWrite("****Start : TS_Log_LIN_005.au3****"&@CRLF)

Local $validationObject1=0                                                               					    	  				    ;To validate whether the log file is adding to the listview
Local $validationObject2=0																				 							    ;to validate whether new file is getting created  for each connect and disconnect
Local $validationObject3=0                                                                                                              ;To validate whether new file is getting created after exceeding the size given
Local $validationObject4=0																											    ;To validate whether it is overwriting the file for "for every bus connect for transmission" config
Local $validationObject5=0																											    ;To validate whether new file is getting created after exceeding the size given for the second time
Local $validationObject6=0																											    ;To validate whether it is overwriting the file for "after each file size exceeding" config
Local $validationObject7=0																											    ;To validate the  start of comment and end of comment entry.
local $logfile1CreatedTime1=0
local $logfile1CreatedTime2=0
local $logfile2CreatedTime1=0
local $logfile2CreatedTime2=0



_launchApp()																					                         		 	    ;Check if the App is open else launch it

ProcessWait("BUSMASTER.exe")																	                        			    ;Pauses script execution until a given process exists.
sleep(4000)

WinActivate($WIN_BUSMASTER,3)

if winexists($WIN_BUSMASTER) then
	_loadConfig("AdvancedLogLIN")																					 				    ;Loading the already existing config
	_SetViewToolBarLIN()
	Sleep(1000)
	_linDriverSelection()																							 				    ;DriverSelection
	_openLINClusterConfig()
	WinWaitActive($WIN_ClustrConfigLIN)
	if winexists($WIN_ClustrConfigLIN) then
		ControlCommand($WIN_ClustrConfigLIN, "",$chckBoxEnableMaster, "Check", "") 					                  				    ;Select Check box for enable Master mode.
		ControlClick($WIN_ClusterConfiguration,"",$BTN_OK_ClusterConfig)											  				    ;Click on ok button for lin cluster configuration
	EndIf
	_ConfigureLINLogWithLogpath("SampleLog") 																		  				    ;Configuring Can log with only file name

	if winexists($WIN_LINLog) then
		ControlCommand($WIN_LINLog, "", $CHKB_OnTransmission_ConfigLOG, "Check")														;Check the checkbox to create log for every bus connect
		Sleep(1000)
		ControlCommand($WIN_LINLog,"",$CHKB_LogfileSize_ConfigLOG,"Check")                                                              ;Check the checkbox to create log after each file size exceeding the limit
		$LogfileSizeHWD=controlgethandle($WIN_LINLog,"",$EDIT_Filesize_ConfigLOG)														;Get the handler for the edit box to enter the logfilesize
		_GUICtrlEdit_SetText($LogfileSizeHWD,"1")                                                                                       ;Set the value '1' for the size of the log file

		$logfilehwd = ControlGetHandle($WIN_LINLog, "", $LSTC_LogFile_ConfigLOG)									                    ;Get the handler for the list view to show the logfiles added
		$logfilePath = _GUICtrlListView_GetItemTextString($logfilehwd, 0)										                        ;Get the first item in the list view
		 ConsoleWrite("$logfilePath - :" & $logfilePath & @CRLF)

		If $logfilePath = _OutputDataPath() & "\SampleLog_M0_S0.log" Then                                                               ;Validating the log file path
			$validationObject1 = 1
		EndIf
        ConsoleWrite("$validationObject1 - For adding logfilepath to listview :" & $validationObject1 & @CRLF)
		$NumberOfLogfilesHWD=controlgethandle($WIN_LINLog,"",$EDIT_NumberOfLogfiles_ConfigLOG)										    ;Fetch NumberOfLogfiles Edit control handle
		_GUICtrlEdit_SetText($NumberOfLogfilesHWD,"3")																				    ;Set the maximum number of files to be created as 3
        ControlSend ( $WIN_LINLog, "", $EDIT_InsertComments_ConfigLOG, "Inserting Comments..." )                                        ;Inserting comments
		ControlCommand($WIN_LINLog,"",$BTN_EnDisable_ConfigLOG,"Check")                                                                 ;Check "&Enable/Disable Logging during tool Connect/Disconnect"
		controlclick($WIN_LINLog,"",$BTN_OK_ConfigLOG)																					;Click on 'OK' button
	EndIf

	_ConnectDisconnect_LIN() 																											;Connect
	_ConnectDisconnect_LIN() 																											;DisConnect
	$logFile1 = LogFileExist("SampleLog_M0_S0")
	$logfile1Content1 = _GetlogFileData("SampleLog_M0_S0.log", "8")                                                                     ;Get the 8th row of the logfile to get the created date and time info to veryfy that it is overwritting
	$logfile1CreatedTime1 = $logfile1Content1[6]																				        ;Get the created time of the log file 1 for the "for every bus connect for transmission" config
	ConsoleWrite("$logfile1CreatedTime1  :" & $logfile1CreatedTime1 & @CRLF)


	_ConnectDisconnect_LIN() 																							                ;Connect
	_ConnectDisconnect_LIN() 																							                ;DisConnect
	$logFile2 = LogFileExist("SampleLog_M1_S0")


	_ConnectDisconnect_LIN() 																											;Connect
	_ConnectDisconnect_LIN() 																							                ;DisConnect
	$logFile3 = LogFileExist("SampleLog_M2_S0")

	$logfile1Content1 = _GetlogFileData("SampleLog_M2_S0.log", "8")											               		        ;Get the 8th row of the logfile to get the created date and time info to veryfy that it is overwritting
	$logfile2CreatedTime1 = $logfile1Content1[6]																				        ;Get the created time of the log file
	ConsoleWrite("$logfile2CreatedTime1  :" & $logfile2CreatedTime1 & @CRLF)

	ConsoleWrite("$logFile1  :" & $logFile1 & @CRLF)
	ConsoleWrite("$logFile2  :" & $logFile2 & @CRLF)
	ConsoleWrite("$logFile3  :" & $logFile3 & @CRLF)

	If $logFile1 = 1 And $logFile2 = 1 And $logFile3 = 1 Then
		$validationObject2 = 1
	EndIf

	ConsoleWrite("$validationObject2 - For creating 3 log files for each connect and disconnect :" & $validationObject2 & @CRLF)


	_ConnectDisconnect_LIN() 																										     ;Connect
	 sleep(120000)                                                                                                                        ;transmit for 10 sec so that the file1 exceeds 5 MB and creates next file
	_ConnectDisconnect_LIN()																											 ;DisConnect

  	$logFile1ForSize=LogFileExist("SampleLog_M0_S0")
  	$logFile2ForSize=LogFileExist("SampleLog_M0_S1")

	if $logFile1ForSize=1 And $logFile2ForSize=1 Then
		$logfile1Content2 = _GetlogFileData("SampleLog_M0_S0.log", "8")											               		     ;Get the 8th row of the logfile to get the created date and time info to veryfy that it is overwritting
		$logfile1CreatedTime2 = $logfile1Content2[6]																				     ;Get the created time of the log file
		ConsoleWrite("$logfile1CreatedTime2  :" & $logfile1CreatedTime2 & @CRLF)
 		$sizeinBytes = FileGetSize ( _OutputDataPath()&"\SampleLog_M0_S0.log" )															 ;Get the file size in bytes
		$size=$sizeinBytes/1048576																										 ;Converting to MB
		if $size >1 Then
			$validationObject3=1
		EndIf
	EndIf
    ConsoleWrite("$validationObject3 - for exceeding size :" &$validationObject3 & @crlf)

	$number = _CheckLogfileTimeIsEqual($logfile1CreatedTime1, $logfile1CreatedTime2)													 ;Check whether the log files created times are equal ,if not then it is overwritten
	ConsoleWrite("$number - To check logfile created times are equal :" & $number & @CRLF)
	If $number = 0 Then
		$validationObject4 = 1
	EndIf

	ConsoleWrite("$validationObject4 - To check whether it overwrites the logfile :" & $validationObject4 & @CRLF)


	_ConnectDisconnect_LIN() 																											;Connect
	 sleep(120000)                                                                                                                       ;transmit for 10 sec
	_ConnectDisconnect_LIN() 																											;Disonnect

	$logFile3ForSize=LogFileExist("SampleLog_M1_S2")


	if $logFile3ForSize=1 Then
		$sizeinBytes = FileGetSize ( _OutputDataPath()&"\SampleLog_M1_S1.log" )              				                            ;Get the file size in bytes
		$size=$sizeinBytes/1048576																										;Converting to MB
		if $size >1 Then
			$validationObject5=1
		EndIf
	EndIf

    ConsoleWrite("$validationObject5 - for exceeding size :" &$validationObject5 & @crlf)

	_ConnectDisconnect_LIN() 																											;Connect
	 sleep(120000)                                                                                                                       ;transmit for 10 sec
	_ConnectDisconnect_LIN() 																											;Disonnect

	$logfile1Content1 = _GetlogFileData("SampleLog_M2_S0.log", "8")	;///										               		    ;Get the 8th row of the logfile to get the created date and time info to veryfy that it is overwritting
	$logfile2CreatedTime2 = $logfile1Content1[6]       																				    ;Get the created time of the log file
	ConsoleWrite("$logfile2CreatedTime2  :" & $logfile2CreatedTime2 & @CRLF)

	$number1 = _CheckLogfileTimeIsEqual($logfile2CreatedTime1, $logfile2CreatedTime2)													;Check whether the log files created times are equal ,if not then it is overwritten
	ConsoleWrite("$number1 - To check logfile2 created times are equal :" & $number1 & @CRLF)
	If $number1 = 0 Then
		$validationObject6 = 1
	EndIf

	ConsoleWrite("$validationObject6 - To check the whether it is overwriting :" & $validationObject6 & @CRLF)

	$Ret1 = _CheckStartAndEndOfLogfile("SampleLog_M0_S0.log")                                                            				;Check the start and end of comment entry
	$Ret2 = _CheckStartAndEndOfLogfile("SampleLog_M1_S0.log")                                                            				;Check the start and end of comment entry
	$Ret3 = _CheckStartAndEndOfLogfile("SampleLog_M2_S0.log")                                                            				;Check the start and end of comment entry
	$Ret4 = _CheckStartAndEndOfLogfile("SampleLog_M0_S1.log")                                                            				;Check the start and end of comment entry
	$Ret5 = _CheckStartAndEndOfLogfile("SampleLog_M1_S1.log")                                                           				;Check the start and end of comment entry
	$Ret6 = _CheckStartAndEndOfLogfile("SampleLog_M1_S2.log")                                                           				;Check the start and end of comment entry
	$Ret7 = _CheckStartAndEndOfLogfile("SampleLog_M2_S2.log")                                                           				;Check the start and end of comment entry

	ConsoleWrite("$Ret1-" & $Ret1 & @CRLF)
	ConsoleWrite("$Ret2-" & $Ret2 & @CRLF)
	ConsoleWrite("$Ret3-" & $Ret3 & @CRLF)
	ConsoleWrite("$Ret4-" & $Ret4 & @CRLF)
	ConsoleWrite("$Ret5-" & $Ret5 & @CRLF)
	ConsoleWrite("$Ret6-" & $Ret6 & @CRLF)
	ConsoleWrite("$Ret7-" & $Ret7 & @CRLF)

	If $Ret1 = 1 And $Ret2 = 1 And $Ret3 = 1 And $Ret4 = 1 And $Ret5 = 1 And $Ret6=1 And $Ret7=1 Then
		$validationObject7 = 1
	EndIf

	ConsoleWrite("$validationObject1 - For adding logfilepath to listview :" & $validationObject1 & @CRLF)
	ConsoleWrite("$validationObject2 - For creating 3 log files for each connect and disconnect :" & $validationObject2 & @CRLF)
    ConsoleWrite("$validationObject3 - for exceeding size :" &$validationObject3 & @crlf)
	ConsoleWrite("$validationObject4 - To check whether it overwrites the logfile :" & $validationObject4 & @CRLF)
	ConsoleWrite("$validationObject5 - for exceeding size :" &$validationObject5 & @crlf)
	ConsoleWrite("$validationObject6 - To check the whether it is overwriting :" & $validationObject6 & @CRLF)
	ConsoleWrite("$validationObject7 - To check the comment entry in the logfile :" & $validationObject7 & @CRLF)


	If $validationObject1 = 1 And $validationObject2 = 1 And $validationObject3 = 1 And $validationObject4 = 1 And $validationObject5 = 1 And $validationObject6 = 1 And $validationObject7 = 1 Then
		_WriteResult("Pass", "TS_Log_LIN_005")
	Else
		_WriteResult("Fail", "TS_Log_LIN_005")
	EndIf

	_CloseApp()


EndIf

FileDelete ( _OutputDataPath() )
ConsoleWrite("****End : TS_LOG_LIN_05.au3****"&@CRLF)
ConsoleWrite(@CRLF)