;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_Log_007
; TestCases			:		Configuring Logging for "every bus connect for transmission" and "after each file size exceeding"
; Test Data			:		-
; Test Setup		:		1. Add a log file with file name as "SampleLog".
;							2. Select create New Log File "after each file size exceeding" and "after every hours/mins". Enter file size of 5MB and time 0 hrs 3 mins.
;							3. Enter no. of files to be created as 3 and insert comments
;							4. Enable logging during tool connect/disconnect
;							5. Add messages to Tx window.
;							6. Connect, transmit CAN frames for 10 mins
;							7. Check log file list and log file details.
; Expected Results  : 		1. After step 2, a new log file with filename as "SampleLog_S0_T0.log" will be added in to the list.

;							2. After step 7, log File created is appended _S for Size and _T for time .
 ; 								a. if the current file size exceeds the specified file size with incremented log file name. Three log files will be created.
;									"SampleLog_S0_T0.log"
;									"SampleLog_S1_T0.log"
;									"SampleLog_S2_T0.log"
 ;							    b. if the specified time is reached. Three log files will be created.
;									"SampleLog_S0_T0.log"
;									"SampleLog_S0_T1.log"
;									"SampleLog_S0_T2.log"

;							3. Once the number of log files created exceeds the value specified by the user, incremental log file creation will be stopped and starts overwriting from the first file irrespective of append or overwrite

;							4. All the log files will have start of comment and end of comment entry.
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("***********Advanced logging for CAN Script Execution Started************"&@CRLF)
ConsoleWrite("****Start : TS_Log_007.au3****"&@CRLF)

Local $validationObject1=0                                                               					    	  				   ;To validate whether the log file is adding to the listview
Local $validationObject2=0																				 							   ;To validate whether new file is getting created after exceeding the size given
Local $validationObject3=0                                                                                                             ;To validate whether new file is getting created after exceeding the size given
Local $validationObject4=0																											   ;To validate whether it is overwriting the file for "after each file size exceeding" config
Local $validationObject5=0																											   ;To check whether it is creating logfiles after 3 min
Local $validationObject6=0																											   ;To validate whether it is overwriting the file for "after every hours/mins" configuration
Local $validationObject7=0																											   ;To validate the  start of comment and end of comment entry.
local $logfile1CreatedTime1=0
local $logfile1CreatedTime2=0
local $logfile1CreatedTime1ForTime=0
local $logfile1CreatedTime2ForTime=0


_launchApp()																					                         		 	   ;Check if the App is open else launch it

ProcessWait("BUSMASTER.exe")																	                        			   ;Pauses script execution until a given process exists.
sleep(4000)

WinActivate($WIN_BUSMASTER,3)

if winexists($WIN_BUSMASTER) then
	_loadConfig("AdvancedLog")																									       ;Loading the already existing config
	sleep(1000)
	_ConfigureCANLogWithLogpath("SampleLog")																						   ;Configuring Can log with only file name

	if winexists($WIN_CANLog) then
		ControlCommand($WIN_CANLog,"",$CHKB_LogfileSize_ConfigLOG,"Check")															   ;Check the checkbox to create log after each file size exceeding the limit
		$LogfileSizeHWD=controlgethandle($WIN_CANLog,"",$EDIT_Filesize_ConfigLOG)													   ;Get the handler for the edit box to enter the logfilesize
		_GUICtrlEdit_SetText($LogfileSizeHWD,"5")																					   ;Set the value '5' for the size of the log file
		ControlCommand($WIN_CANLog,"",$CHKB_LogOnTime_ConfigLOG,"Check")														       ;Check the checkbox to create log in certain time limit
		$LogMinutesHWD=controlgethandle($WIN_CANLog,"",$EDIT_Minutes_ConfigLOG)														   ;Geth the handler for the edit box to enter minutes
		_GUICtrlEdit_SetText($LogMinutesHWD,"1")                                                                                       ;Set the value '1' for the minutes
		sleep(1000)
		$logfilehwd = ControlGetHandle($WIN_CANLog, "", $LSTC_LogFile_ConfigLOG)									                   ;Get the handler for the list view to show the logfiles added
		$logfilePath = _GUICtrlListView_GetItemTextString($logfilehwd, 0)										                       ;Get the first item in the list view
		 ConsoleWrite("$logfilePath - :" & $logfilePath & @CRLF)

		If $logfilePath = _OutputDataPath() & "\SampleLog_S0_T0.log" Then                                                              ;Validating the log file path
			$validationObject1 = 1
		EndIf
        ConsoleWrite("$validationObject1 - For adding logfilepath to listview :" & $validationObject1 & @CRLF)

		$NumberOfLogfilesHWD=controlgethandle($WIN_CANLog,"",$EDIT_NumberOfLogfiles_ConfigLOG)										   ;Fetch NumberOfLogfiles Edit control handle
		_GUICtrlEdit_SetText($NumberOfLogfilesHWD,"3")																				   ;Set the maximum number of files to be created as 3
        ControlSend ( $WIN_CANLog, "", $EDIT_InsertComments_ConfigLOG, "Inserting Comments..." )                                       ;Inserting comments
        ControlCommand($WIN_CANLog,"",$BTN_EnDisable_ConfigLOG,"Check")                                                                ;Check "&Enable/Disable Logging during tool Connect/Disconnect"
		controlclick($WIN_CANLog,"",$BTN_OK_ConfigLOG)																				   ;Click on 'OK' button
	EndIf


	_ConnectDisconnect() 																										   	   ;Connect
	 sleep(6000)                                                                                                                      ;transmit for 10 sec
	_ConnectDisconnect()


  	$logFile1=LogFileExist("SampleLog_S0_T0")          																				   ;Check whether the logfiles exists
  	$logFile2=LogFileExist("SampleLog_S1_T0")  																						   ;Check whether the logfiles exists

	ConsoleWrite("$logFile1  :" & $logFile1 & @CRLF)
	ConsoleWrite("$logFile2  :" & $logFile2 & @CRLF)

	if $logFile1=1 And $logFile2=1 Then
		$logfile1Content1 = _GetlogFileData("SampleLog_S0_T0.log", "8")											               		   ;Get the 8th row of the logfile to get the created date and time info to veryfy that it is overwritting
		$logfile1CreatedTime1 = $logfile1Content1[6]																				   ;Get the created time of the log file
		ConsoleWrite("$logfile1CreatedTime1  :" & $logfile1CreatedTime1 & @CRLF)
 		$sizeinBytes = FileGetSize ( _OutputDataPath()&"\SampleLog_S0_T0.log" )
		$size=$sizeinBytes/1048576
		if $size >5 Then
			$validationObject2=1
		EndIf
	EndIf
    ConsoleWrite("$validationObject2 - for exceeding size :" &$validationObject2 & @crlf)


	_ConnectDisconnect() 																												;Connect
	 sleep(6000)                                                                                                                       ;transmit for 10 sec so that the first file size exceeds 5 mb size
	_ConnectDisconnect()

	$logFile3=LogFileExist("SampleLog_S2_T0")
	ConsoleWrite("$logFile3  :" & $logFile3 & @CRLF)


	if $logFile3=1 Then
		$sizeinBytes = FileGetSize ( _OutputDataPath()&"\SampleLog_S1_T0.log" )														    ;Get the 2nd file size
		$size=$sizeinBytes/1048576
		if $size >5 Then																		                                     	;Check whether the size is greater than 5 MB
			$validationObject3=1
		EndIf
	EndIf

   ConsoleWrite("$validationObject3 - for exceeding size :" &$validationObject3 & @crlf)

	_ConnectDisconnect() 																												;Connect
	 sleep(6000)																														;transmit for 10 sec so that it creates 3rd file after the 2nd one exceeding 5 MB
	_ConnectDisconnect()

	$logfile1Content2 = _GetlogFileData("SampleLog_S0_T0.log", "8")																	    ;Get the 8th row of the logfile to get the created date and time info to veryfy that it is overwritting
	$logfile1CreatedTime2 = $logfile1Content2[6]																	                    ;Get the created time of the log file
	ConsoleWrite("$logfile1CreatedTime2  :" & $logfile1CreatedTime2 & @CRLF)

	$number = _CheckLogfileTimeIsEqual($logfile1CreatedTime1, $logfile1CreatedTime2)													;Check whether the log files created times are equal ,if not then it is overwritten
	ConsoleWrite("$number - To check logfile created times are equal :" & $number & @CRLF)
	If $number = 0 Then
		$validationObject4 = 1
	EndIf

	ConsoleWrite("$validationObject4 - To check the whether it is overwriting :" & $validationObject4 & @CRLF)

	_TxMsgMenu()																														;Open transmit window
	_DelMsgTxWindow(0)                            																						;Deleting messages from transmit window so that it will not exceed 5 MB if connected for 1 min
	_DelMsgTxWindow(0)
	_DelMsgTxWindow(0)
	_DelMsgTxWindow(0)

	_ConfigTXWinDetails(0,"ndb","100",1,"","","No","1000","")																			;$Rownum,$HSCaseNo(1-dbmessage,2-NonDbmsg),$messageid/Index(For dbMessage),chnl,datalength,Messageype,RTR,Repetition,Key
    _CloseTxWindowArg("CAN")																     										;Close Tx window

	_ConnectDisconnect()
	 sleep(62000)
     $logFile1ForTime=LogFileExist("SampleLog_S0_T0")
	 if $logFile1ForTime=1 Then
		$logfile1Content1 = _GetlogFileData("SampleLog_S0_T0.log", "8")																	;Get the 8th row of the logfile to get the created date and time info to veryfy that it is overwritting
		$logfile1CreatedTime1ForTime = $logfile1Content1[6]																	            ;Get the created time of the log file
		ConsoleWrite("$logfile1CreatedTime1ForTime  :" & $logfile1CreatedTime1ForTime & @CRLF)
	EndIf
	sleep(30000)
	sleep(30000)
	sleep(30000)
	sleep(30000)

	_ConnectDisconnect()																												;Disconnect

 	$logFile2ForTime=LogFileExist("SampleLog_S0_T1")
	$logFile3ForTime=LogFileExist("SampleLog_S0_T2")

	if $logFile2ForTime=1 And $logFile3ForTime=1 Then
		$validationObject5=1
	EndIf

	ConsoleWrite("$logFile1ForTime  :" & $logFile1ForTime & @CRLF)
	ConsoleWrite("$logFile2ForTime  :" & $logFile2ForTime & @CRLF)
	ConsoleWrite("$logFile3ForTime  :" & $logFile3ForTime & @CRLF)

    ConsoleWrite("$validationObject5 - for creating logfiles after 3 min :" &$validationObject5 & @crlf)

	$logfile1Content2 = _GetlogFileData("SampleLog_S0_T0.log", "8")																	    ;Get the 8th row of the logfile to get the created date and time info to veryfy that it is overwritting
	$logfile1CreatedTime2ForTime = $logfile1Content2[6]																				    ;Get the created time of the log file
	ConsoleWrite("$logfile1CreatedTime2ForTime  :" & $logfile1CreatedTime2ForTime & @CRLF)

	$number = _CheckLogfileTimeIsEqual($logfile1CreatedTime1ForTime, $logfile1CreatedTime2ForTime)
	ConsoleWrite("$number - To check logfile created times are equal :" & $number & @CRLF)
	If $number = 0 Then
		$validationObject6 = 1
	EndIf
	ConsoleWrite("$validationObject6 - To check whether it is overwriting the file :" & $validationObject6 & @CRLF)

    $Ret1 = _CheckStartAndEndOfLogfile("SampleLog_S0_T0.log")                                                            				;Check the start and end of comment entry
	$Ret2 = _CheckStartAndEndOfLogfile("SampleLog_S1_T0.log")                                                            				;Check the start and end of comment entry
	$Ret3 = _CheckStartAndEndOfLogfile("SampleLog_S2_T0.log")                                                            				;Check the start and end of comment entry
	$Ret4 = _CheckStartAndEndOfLogfile("SampleLog_S0_T1.log")                                                            				;Check the start and end of comment entry
	$Ret5 = _CheckStartAndEndOfLogfile("SampleLog_S0_T2.log")                                                            				;Check the start and end of comment entry

	ConsoleWrite("$Ret1-" & $Ret1 & @CRLF)
	ConsoleWrite("$Ret2-" & $Ret2 & @CRLF)
	ConsoleWrite("$Ret3-" & $Ret3 & @CRLF)
	ConsoleWrite("$Ret4-" & $Ret4 & @CRLF)
	ConsoleWrite("$Ret5-" & $Ret5 & @CRLF)

	If $Ret1 = 1 And $Ret2 = 1 And $Ret3 = 1 And $Ret4 = 1 And $Ret5 = 1 Then
		$validationObject7 = 1
	EndIf

	ConsoleWrite("$validationObject1 - For adding logfilepath to listview :" & $validationObject1 & @CRLF)
	ConsoleWrite("$validationObject2 - for exceeding size :" &$validationObject2 & @crlf)
	ConsoleWrite("$validationObject3 - for exceeding size :" &$validationObject3 & @crlf)
	ConsoleWrite("$validationObject4 - To check the whether it is overwriting :" & $validationObject4 & @CRLF)
	ConsoleWrite("$validationObject5 - for creating logfiles after 3 min :" &$validationObject5 & @crlf)
	ConsoleWrite("$validationObject6 - To check whether it is overwriting the file :" & $validationObject6 & @CRLF)
	ConsoleWrite("$validationObject7 - To check the comment entry in the logfile :" & $validationObject7 & @CRLF)


	If $validationObject1 = 1 And $validationObject2 = 1 And $validationObject3 = 1 And $validationObject4 = 1 And $validationObject5 = 1 And $validationObject6 = 1 And $validationObject7 = 1 Then
		_WriteResult("Pass", "TS_Log_007")
	Else
		_WriteResult("Fail", "TS_Log_007")
	EndIf

	_CloseApp()


EndIf

FileDelete ( _OutputDataPath() )

ConsoleWrite("****End : TS_LOG_07.au3****"&@CRLF)
ConsoleWrite(@CRLF)