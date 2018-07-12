;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_Log_008
; TestCases			:		"Number of files to be allowed" validation
; Test Data			:		-
; Test Setup		:		1. Add a log file with file name as "SampleLog".
;							2. Select any of the create new log file option.
;							3. Enter number of files to be allowed as 0 and 1100

; Expected Results  : 		1. After step2, Default number of log files will be limited to 10.

;							2. After step3, a message should pop up saying "Please enter a valid number between 1 and 1000."
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("***********Advanced logging for CAN Script Execution Started************"&@CRLF)
ConsoleWrite("****Start : TS_Log_008.au3****"&@CRLF)

Local $validationObject1=0                                                               					    	  				   ;To validate whether the Default number of log files will be limited to 10.
Local $validationObject2=0																				 							   ;To check the pop up saying "Please enter a valid number between 1 and 1000." when we give 0 as the number
Local $validationObject3=0                                                                                                             ;To check the pop up saying "Please enter a valid number between 1 and 1000." when we give 1100 as the number

_launchApp()																					                         		 	   ;Check if the App is open else launch it

ProcessWait("BUSMASTER.exe")																	                        			   ;Pauses script execution until a given process exists.
sleep(4000)

WinActivate($WIN_BUSMASTER,3)

if winexists($WIN_BUSMASTER) then
	_loadConfig("AdvancedLog_1")																									    ;Loading the already existing config
	sleep(1000)
	_ConfigureCANLogWithLogpath("SampleLog")																							;Add a log file with file name as "SampleLog".
	if winexists($WIN_CANLog) then
		ControlCommand($WIN_CANLog, "", $CHKB_OnTransmission_ConfigLOG, "Check")														;Check the checkbox to create log for every bus connect
		Sleep(3)

		$NumberOfLogfilesHWD=controlgethandle($WIN_CANLog,"",$EDIT_NumberOfLogfiles_ConfigLOG)										    ;Fetch NumberOfLogfiles Edit control handle
		$DefaultValue=_GUICtrlEdit_GetText($NumberOfLogfilesHWD)																		;Get the default value of the edit box for the number of files to be created
		ConsoleWrite("$DefaultValue - :" & $DefaultValue & @CRLF)
		if $DefaultValue=10 Then																										;Check whether the default value is 10
			$validationObject1=1
		EndIf
		ConsoleWrite("$validationObject1 - To check the default value :" & $validationObject1 & @CRLF)
		sleep(2000)

		_GUICtrlEdit_SetSel($NumberOfLogfilesHWD, 0, -1)																				;Select the text in the edit box for the number of files to be created
		ControlSend ( $WIN_CANLog, "", $EDIT_NumberOfLogfiles_ConfigLOG, "0" )															;Set the value to 0

		WinWait("BUSMASTER","",2)																									;Wait till the pop up comes
            if winexists("BUSMASTER","") Then
				$txt=ControlGetText ( "BUSMASTER", "","[CLASS:Static; INSTANCE:2]" )												;Get the text in the pop up window
				ConsoleWrite("$txt - "&$txt & @CRLF)
				if $txt="Enter an integer between 1 and 1000." Then																		;Validate the text
					$validationObject2=1
				EndIf
				send("{ENTER}")
            EndIf
		ConsoleWrite("$validationObject2 - To check the popup for the value 0 :" & $validationObject2 & @CRLF)

		_GUICtrlEdit_SetSel($NumberOfLogfilesHWD, 0, -1)
		ControlSend ( $WIN_CANLog, "", $EDIT_NumberOfLogfiles_ConfigLOG, "1100")														;Set the value to 1100

		WinWait("BUSMASTER","",2)																									;Wait till the pop up comes
            if winexists("BUSMASTER","") Then
				$txt=ControlGetText ( "BUSMASTER", "","[CLASS:Static; INSTANCE:2]" )												;Get the text in the pop up window
				ConsoleWrite("$txt - "&$txt & @CRLF)
				if $txt="Enter an integer between 1 and 1000." Then																		;Validate the text
					$validationObject3=1
				EndIf
				send("{ENTER}")
            EndIf
		ConsoleWrite("$validationObject3 - To check the popup for the value 1100 :" & $validationObject3 & @CRLF)
        controlclick($WIN_CANLog,"",$BTN_OK_ConfigLOG)
	EndIf

	If $validationObject1 = 1 And $validationObject2 = 1 And $validationObject3 = 1 Then
		_WriteResult("Pass", "TS_Log_008")
	Else
		_WriteResult("Fail", "TS_Log_008")
	EndIf
	_CloseApp()

EndIf

FileDelete ( _OutputDataPath() )
ConsoleWrite("****End : TS_LOG_08.au3****"&@CRLF)
ConsoleWrite(@CRLF)