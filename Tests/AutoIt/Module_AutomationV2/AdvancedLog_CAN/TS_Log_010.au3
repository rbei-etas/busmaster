;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_Log_010
; TestCases			:		"if specified time is reached" validation
; Test Data			:		-
; Test Setup		:		1. Add a log file with file name as "SampleLog".
;							2. Select create New Log File for "After every hrs and mins"
;							3. Enter hrs as 170
;							4. Reset the hrs value to 0.
;							5. Enter values 0 and 60 for mins

; Expected Results  : 		1. After step2, 3. 3. Default allowed Time of log files will be limited to 0 Hrs and 30 Min.

;							2. After step3, a message should pop up saying "Please enter a valid number between 0 and 168."

;							3. After step5, a message should pop up saying "Please enter a valid number between 0 and 59"
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("***********Advanced logging for CAN Script Execution Started************"&@CRLF)
ConsoleWrite("****Start : TS_Log_010.au3****"&@CRLF)

Local $validationObject1=0                                                               					    	  				   ;To validate whether the Default allowed Time of log files will be limited to 0 Hrs and 30 Min.
Local $validationObject2=0                                                                                                             ;To check the pop up saying "Please enter a valid number between 0 and 168." when we give 170 for hours
Local $validationObject3=0                                                                                                             ;To check the pop up saying "Please enter a valid number between 0 and 59" when we give 60 for minutes

_launchApp()																					                         		 	   ;Check if the App is open else launch it

ProcessWait("BUSMASTER.exe")																	                        			   ;Pauses script execution until a given process exists.
sleep(4000)

WinActivate($WIN_BUSMASTER,3)

if winexists($WIN_BUSMASTER) then
	_loadConfig("AdvancedLog_1")																										;Loading the already existing config
	sleep(1000)
	_ConfigureCANLogWithLogpath("SampleLog")																							;Add a log file with file name as "SampleLog".
	if winexists($WIN_CANLog) then
		ControlCommand($WIN_CANLog,"",$CHKB_LogOnTime_ConfigLOG,"Check")														        ;Check the checkbox to create log in certain time limit
		sleep(3)
		$LogHoursHWD=controlgethandle($WIN_CANLog,"",$EDIT_Hours_ConfigLOG)
		$LogMinutesHWD=controlgethandle($WIN_CANLog,"",$EDIT_Minutes_ConfigLOG)
        $DefaultValueFotHours=_GUICtrlEdit_GetText($LogHoursHWD)																		;Get the default value of the edit box for hours
        $DefaultValueFotMinutes=_GUICtrlEdit_GetText($LogMinutesHWD)																    ;Get the default value of the edit box for minutes
		ConsoleWrite("$DefaultValueFotHours - :" & $DefaultValueFotHours & @CRLF)
		ConsoleWrite("$DefaultValueFotMinutes - :" & $DefaultValueFotMinutes & @CRLF)
		if $DefaultValueFotHours=0 And $DefaultValueFotMinutes=30 Then
			$validationObject1=1
		EndIf
		ConsoleWrite("$validationObject1 - To check the default values for hours and minutes :" & $validationObject1 & @CRLF)
		sleep(2000)

		_GUICtrlEdit_SetSel($LogHoursHWD, 0, -1)																				        ;Select the text in the edit box for hours
		ControlSend ( $WIN_CANLog, "", $EDIT_Hours_ConfigLOG, "170" )															        ;Set the value to 170
		WinWait("BUSMASTER","",2)																									;Wait till the pop up comes
            if winexists("BUSMASTER","") Then
				$txt=ControlGetText ( "BUSMASTER", "","[CLASS:Static; INSTANCE:2]" )												;Get the text in the pop up window
				ConsoleWrite("$txt - "&$txt & @CRLF)
				if $txt="Enter an integer between 0 and 168." Then																		;Validate the text
					$validationObject2=1
				EndIf
				send("{ENTER}")
            EndIf
		ConsoleWrite("$validationObject2 - To check the popup for the value 0 for hours :" & $validationObject2 & @CRLF)

		_GUICtrlEdit_SetSel($LogMinutesHWD, 0, -1)																				        ;Select the text in the edit box for minutes
		ControlSend ( $WIN_CANLog, "", $EDIT_Minutes_ConfigLOG, "60" )															        ;Set the value to 60
		WinWait("BUSMASTER","",2)																									;Wait till the pop up comes
            if winexists("BUSMASTER","") Then
				$txt=ControlGetText ( "BUSMASTER", "","[CLASS:Static; INSTANCE:2]" )												;Get the text in the pop up window
				ConsoleWrite("$txt - "&$txt & @CRLF)
				if $txt="Enter an integer between 0 and 59." Then																		;Validate the text
					$validationObject3=1
				EndIf
				send("{ENTER}")
            EndIf
		ConsoleWrite("$validationObject3 - To check the popup for the value 0 for hours :" & $validationObject3 & @CRLF)

        controlclick($WIN_CANLog,"",$BTN_OK_ConfigLOG)
	EndIf

	If $validationObject1 = 1 And $validationObject2 = 1 And $validationObject3 = 1 Then
		_WriteResult("Pass", "TS_Log_010")
	Else
		_WriteResult("Fail", "TS_Log_010")
	EndIf
	_CloseApp()

EndIf

FileDelete ( _OutputDataPath() )
ConsoleWrite("****End : TS_LOG_10.au3****"&@CRLF)
ConsoleWrite(@CRLF)