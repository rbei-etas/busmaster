;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_Log_011
; TestCases			:		Save/Load of advanced log file settings into configuration
; Test Data			:		-
; Test Setup		:		1. Add a log file
;							2. Select all of the create new log file options and enter values accordingly.
;							3. Insert comments
;							4. Close the logging window.
;							5. Save and load the configuration
;							6. Open configure logging window

; Expected Results  : 		1. After step 6, all the settings done will be retained in the logging window
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("***********Advanced logging for LIN Script Execution Started************"&@CRLF)
ConsoleWrite("****Start : TS_Log_LIN_011.au3****"&@CRLF)


Local $validationObject1=0                                                               					    	  				   ;To validate whether all the settings done will be retained in the logging window.

_launchApp()																					                         		 	   ;Check if the App is open else launch it

ProcessWait("BUSMASTER.exe")																	                        			   ;Pauses script execution until a given process exists.
sleep(4000)

WinActivate($WIN_BUSMASTER,3)


if winexists($WIN_BUSMASTER) then

 	_loadConfig("AdvancedLogLIN")																									    ;Loading the already existing config
	sleep(1000)
	_ConfigureLINLogWithLogpath("SampleLog")																							;Add a log file with file name as "SampleLog".


	if winexists($WIN_LINLog) then
		ControlCommand($WIN_LINLog, "", $CHKB_OnTransmission_ConfigLOG, "Check")														;Check the checkbox to create log for every bus connect
		ControlCommand($WIN_LINLog,"",$CHKB_LogfileSize_ConfigLOG,"Check")														        ;Check the checkbox to create log after each file size exceeding the limit
		ControlCommand($WIN_LINLog,"",$CHKB_LogOnTime_ConfigLOG,"Check")														        ;Check the checkbox to create log in certain time limit
		ControlSend ( $WIN_LINLog, "", $EDIT_InsertComments_ConfigLOG, "Inserting Comments..." )                                        ;Inserting comments
		controlclick($WIN_LINLog,"",$BTN_OK_ConfigLOG)
	EndIf

	_saveAsConfig("test")
	_CloseApp()

EndIf

_launchApp()																					                         		 	   ;Check if the App is open else launch it

ProcessWait("BUSMASTER.exe")																	                        			   ;Pauses script execution until a given process exists.
sleep(4000)

WinActivate($WIN_BUSMASTER,3)
if winexists($WIN_BUSMASTER) then
	sleep(2000)
	WinMenuSelectItem($WIN_BUSMASTER,"",$linMenu,$LINLogMenu,$configureMenu)
	WinActivate($WIN_LINLog,3)
	if winexists($WIN_LINLog) then
		$CHKB_OnTransmissionHWD=controlgethandle($WIN_LINLog,"",$CHKB_OnTransmission_ConfigLOG)											;Get the handler for the check box to create log files for each connect
		$CHKB_OnTransmission_State=_GUICtrlButton_GetCheck($CHKB_OnTransmissionHWD)																			;Checking whether the check box is checked
		$CHKB_LogfileSizeHWD=controlgethandle($WIN_LINLog,"",$CHKB_LogfileSize_ConfigLOG)											    ;Get the handler for the check box to create log files after exceeding the time limit
		$CHKB_LogfileSize_State=_GUICtrlButton_GetCheck($CHKB_LogfileSizeHWD)																			    ;Checking whether the check box is checked
		$CHKB_LogOnTimeHWD=controlgethandle($WIN_LINLog,"",$CHKB_LogOnTime_ConfigLOG)													;Get the handler for the check box to create log files in certain time limit
		$CHKB_LogOnTime_State= _GUICtrlButton_GetCheck($CHKB_LogOnTimeHWD)																			    ;Checking whether the check box is checked
		$EDIT_NumberOfLogfiles_State=ControlCommand($WIN_LINLog,"",$EDIT_NumberOfLogfiles_ConfigLOG,"IsEnabled")                                                 ;Status of the checkbox to enable the number of logfiles to be created
		ConsoleWrite("$CHKB_OnTransmission_State :" &$CHKB_OnTransmission_State & @crlf)
		ConsoleWrite("$CHKB_LogfileSize_State :" &$CHKB_LogfileSize_State & @crlf)
		ConsoleWrite("$CHKB_LogOnTime_State :" &$CHKB_LogOnTime_State & @crlf)
		ConsoleWrite("$EDIT_NumberOfLogfiles_State :" &$EDIT_NumberOfLogfiles_State & @crlf)
		if $CHKB_OnTransmission_State=$BST_CHECKED And $CHKB_LogfileSize_State=$BST_CHECKED And $CHKB_LogOnTime_State=$BST_CHECKED And $EDIT_NumberOfLogfiles_State=$BST_CHECKED Then
			  $validationObject1=1
		EndIf
		controlclick($WIN_LINLog,"",$BTN_OK_ConfigLOG)
	EndIf
	ConsoleWrite("$validationObject1 :" &$validationObject1 & @crlf)
	_CloseApp()
EndIf

	If $validationObject1 = 1  Then
		_WriteResult("Pass", "TS_Log_LIN_011")
	Else
		_WriteResult("Fail", "TS_Log_LIN_011")
	EndIf

;FileDelete ( _OutputDataPath() )
ConsoleWrite("****End : TS_LOG_LIN_11.au3****"&@CRLF)
ConsoleWrite(@CRLF)