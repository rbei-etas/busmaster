;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_Log_009
; TestCases			:		"After each file size exceeding" validation
; Test Data			:		-
; Test Setup		:		1. Add a log file with file name as "SampleLog".
;							2. Select create New Log File "after each file size exceeding"
;							3. Enter file size of 0 and 5000

; Expected Results  : 		1. After step2, 3. Default allowed size of log files will be limited to 50MB.

;							2. After step3, a message should pop up saying "Please enter a valid number between 1 and 4096."
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("***********Advanced logging for CAN Script Execution Started************"&@CRLF)
ConsoleWrite("****Start : TS_Log_009.au3****"&@CRLF)

Local $validationObject1=0                                                               					    	  				   ;To validate whether the Default number for the size limit is 50.
Local $validationObject2=0																				 							   ;To check the pop up saying "Please enter a valid number between 1 and 4096."when we give 0 as the number
Local $validationObject3=0                                                                                                             ;To check the pop up saying "Please enter a valid number between 1 and 4096." when we give 5000 as the number

_launchApp()																					                         		 	   ;Check if the App is open else launch it

ProcessWait("BUSMASTER.exe")																	                        			   ;Pauses script execution until a given process exists.
sleep(4000)

WinActivate($WIN_BUSMASTER,3)

if winexists($WIN_BUSMASTER) then
	_loadConfig("AdvancedLog_1")																										;Loading the already existing config
	sleep(1000)
	_ConfigureCANLogWithLogpath("SampleLog")																							;Add a log file with file name as "SampleLog".
	if winexists($WIN_CANLog) then
		ControlCommand($WIN_CANLog,"",$CHKB_LogfileSize_ConfigLOG,"Check")														        ;Check the checkbox to create log after each file size exceeding the limit
		Sleep(3)
		$LogfileSizeHWD=controlgethandle($WIN_CANLog,"",$EDIT_Filesize_ConfigLOG)
		$DefaultValue=_GUICtrlEdit_GetText($LogfileSizeHWD)																		        ;Get the default value of the edit box for the size limit
		ConsoleWrite("$DefaultValue - :" & $DefaultValue & @CRLF)
		if $DefaultValue=50 Then																										;Check whether the default value is 50
			$validationObject1=1
		EndIf
		ConsoleWrite("$validationObject1 - To check the default value :" & $validationObject1 & @CRLF)
		sleep(2000)

		_GUICtrlEdit_SetSel($LogfileSizeHWD, 0, -1)																				        ;Select the text in the edit box for the size limit
		ControlSend ( $WIN_CANLog, "", $EDIT_Filesize_ConfigLOG, "0" )															        ;Set the value to 0
		WinWait("BUSMASTER","",2)																									;Wait till the pop up comes
            if winexists("BUSMASTER","") Then
				$txt=ControlGetText ( "BUSMASTER", "","[CLASS:Static; INSTANCE:2]" )												;Get the text in the pop up window
				ConsoleWrite("$txt - "&$txt & @CRLF)
				if $txt="Enter an integer between 1 and 4096." Then																		;Validate the text
					$validationObject2=1
				EndIf
				send("{ENTER}")
            EndIf
		ConsoleWrite("$validationObject2 - To check the popup for the value 0 :" & $validationObject2 & @CRLF)


		_GUICtrlEdit_SetSel($LogfileSizeHWD, 0, -1)																				        ;Select the text in the edit box for the size limit
		ControlSend ( $WIN_CANLog, "", $EDIT_Filesize_ConfigLOG, "5000" )															    ;Set the value to 5000
		WinWait("BUSMASTER","",2)																									;Wait till the pop up comes
            if winexists("BUSMASTER","") Then
				$txt=ControlGetText ( "BUSMASTER", "","[CLASS:Static; INSTANCE:2]" )												;Get the text in the pop up window
				ConsoleWrite("$txt - "&$txt & @CRLF)
				if $txt="Enter an integer between 1 and 4096." Then																		;Validate the text
					$validationObject3=1
				EndIf
				send("{ENTER}")
            EndIf
		ConsoleWrite("$validationObject3 - To check the popup for the value 5000 :" & $validationObject3 & @CRLF)
        controlclick($WIN_CANLog,"",$BTN_OK_ConfigLOG)
	EndIf


	If $validationObject1 = 1 And $validationObject2 = 1 And $validationObject3 = 1 Then
		_WriteResult("Pass", "TS_Log_009")
	Else
		_WriteResult("Fail", "TS_Log_009")
	EndIf

	_CloseApp()

EndIf

FileDelete ( _OutputDataPath() )
ConsoleWrite("****End : TS_LOG_09.au3****"&@CRLF)
ConsoleWrite(@CRLF)