;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_Log_001
; TestCases			:		Configuring Log File with advanced log file settings
; Test Data			:		-
; Test Setup		:		1. Select a new configuration file.
;							2. Select Configure -> Log
;							3. Check for log file configuration dialog.
;							4. Check log file list and log file details.

; Expected Results  : 		1. After step 3, Log file  configuration dialog will be displayed with empty log file list. Other than Add button all controls will be disabled. and also create new log file group box will enabled with three options :
;								a.For every bus connect for transmission
;								b.After each file size exceeding 1MB
;								c.After every _ hours _ minutes

;							2. There is also option with check box "Number of files to be allowed"
;							   And initially it will be disabled. It is enabled only when one of the above three new log file creation options is selected
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("***********Advanced logging for CAN Script Execution Started************"&@CRLF)
ConsoleWrite("****Start : TS_Log_001.au3****"&@CRLF)

Local $validationObjectTS1=0                                                               					    	   ;To validate the first expected results
Local $validationObjectTS2=0																						   ;To validate the second expected results

Local $validationObjectForAdvancedLogging=0
Local $validationObjectForLogfileDetails=0
Local $validationObjectForLogTrigger = 0
Local $validationObjectForButtons =0
Local $validationObjectForAdvLogAfterAddingLog =0


_launchApp()																					                       ;Check if the App is open else launch it

ProcessWait("BUSMASTER.exe")																	                       ;Pauses script execution until a given process exists.
sleep(4000)

WinActivate($WIN_BUSMASTER,3)


if winexists($WIN_BUSMASTER) then
	_createConfig("cfxLOG_01")																	                        ;Create New Configuration

	;WinMenuSelectItem($WIN_BUSMASTER,"",$CANMenu,$CANLogMenu,$configureMenu)					                        ;Select Configure->Log
	WinRibbonSelectItem($WIN_BUSMASTER,"",$CANMenu,$CANLogMenu,$configureMenu)
	WinWaitActive($WIN_CANLog)

	if WinActive($WIN_CANLog) Then


		if WinWaitActive($WIN_CANLog,"",5) Then									                                        ;wait for the busmaster window to be active
		;ControlClick($WIN_CANLog,"",$BTN_Add_ConfigLOG)
		EndIf


		$canLogHWD=ControlGetHandle($WIN_CANLog,"",$LSTC_LogFile_ConfigLOG)
		$logfilesCount=_GUICtrlListView_GetItemCount($canLogHWD)                                                         ;Getting the number of logfiles created

		;$CHKB_OnTransmission_ConfigLOG=2027               															     ;controlID for the checkbox to create log for every bus connect
		;$CHKB_LogfileSize_ConfigLOG=2028																				 ;controlID for the checkbox to create log after each file size exceeding the limit
		;$CHKB_LogOnTime_ConfigLOG=2031																					 ;controlID for the checkbox to create log in certain time limit
		;$EDIT_NumberOfLogfiles_ConfigLOG=2037																			 ;controlID for the editbox for the number of logfiles to be created
		;$EDIT_Filesize_ConfigLOG=2029																					 ;controlID for the editbox for the size of the log file
		;$EDIT_Hours_ConfigLOG=2032																						 ;controlID for the editbox for hours
		;$EDIT_Minutes_ConfigLOG=2034																					 ;controlID for the editbox for minutes

		$addButtonStatus=ControlCommand($WIN_CANLog,"",$BTN_Add_ConfigLOG,"IsEnabled")               					 ;Status of the Add button in Configure Logging Dialog
		$EnDisablButtonStatus=ControlCommand($WIN_CANLog,"",$BTN_EnDisable_ConfigLOG,"IsEnabled")  						 ;Status of the ""&Enable/Disable Logging during tool Connect/Disconnect" in Configure Logging Dialog
		$logFPathButtonStatus=ControlCommand($WIN_CANLog,"",$BTN_logFPath_ConfigLOG,"IsEnabled") 						 ;Status of the'LOGFILEPATH" button in Configure Logging Dialog
		;$TXT_FileName_SelLogFile=1001
		$TimeModeButtonStatus=ControlCommand($WIN_CANLog,"",$COMB_TimeMode_ConfigLOG,"IsEnabled")						 ;Status of the Time Mode combobox in Configure Logging Dialog
		$ChannelButtonStatus=ControlCommand($WIN_CANLog,"",$COMB_Channel_ConfigLOG,"IsEnabled") 						 ;Status of the channel combobox in Configure Logging Dialog
		$NumModeDecButtonStatus=ControlCommand($WIN_CANLog,"",$RBTN_NumModeDec_ConfigLOG,"IsEnabled")					 ;Status of the Decimal numeric mode in Configure Logging Dialog
		$NumModeHexButtonStatus=ControlCommand($WIN_CANLog,"",$RBTN_NumModeHex_ConfigLOG,"IsEnabled") 					 ;Status of the Hex numeric mode in Configure Logging Dialog
		$FileModeAppButtonStatus=ControlCommand($WIN_CANLog,"",$RBTN_FileModeApp_ConfigLOG,"IsEnabled") 				 ;Status of the Append File mode in Configure Logging Dialog
		$FileModeOverWriteButtonStatus=ControlCommand($WIN_CANLog,"",$RBTN_FileModeOverWrite_ConfigLOG,"IsEnabled")		 ;Status of the Overwrite File mode in Configure Logging Dialog
		$StartTrigButtonStatus=ControlCommand($WIN_CANLog,"",$CHKB_StartTrig_ConfigLOG,"IsEnabled") 					 ;Status of the start trigger in Configure Logging Dialog
		$StopTrigButtonStatus=ControlCommand($WIN_CANLog,"",$CHKB_StopTrig_ConfigLOG,"IsEnabled")                        ;Status of the stop trigger in Configure Logging Dialog
		$StartMsgIDButtonStatus=ControlCommand($WIN_CANLog,"",$TXT_StartMsgID_ConfigLOG,"IsEnabled")
		$StopMsgIDButtonStatus=ControlCommand($WIN_CANLog,"",$TXT_StopMsgID_ConfigLOG,"IsEnabled")
		$FilterButtonStatus=ControlCommand($WIN_CANLog,"",$BTN_Filter_ConfigLOG,"IsEnabled") 						     ;Status of the filter button in Configure Logging Dialog
		$okButtonStatus=ControlCommand($WIN_CANLog,"",$BTN_OK_ConfigLOG,"IsEnabled")								     ;Status of the OK button in Configure Logging Dialog
		$removeButtonStatus=ControlCommand($WIN_CANLog,"",$BTN_RemoveLog_ctrlID,"IsEnabled")                             ;Status of the Remove button

		$OnTransmissionButtonStatus=ControlCommand($WIN_CANLog,"",$CHKB_OnTransmission_ConfigLOG,"IsEnabled")            ;Status of the checkbox to create log for every bus connect
		$LogfileSizeButtonStatus=ControlCommand($WIN_CANLog,"",$CHKB_LogfileSize_ConfigLOG,"IsEnabled")                  ;Status of the checkbox to create log after each file size exceeding the limit
		$LogOnTimeButtonStatus=ControlCommand($WIN_CANLog,"",$CHKB_LogOnTime_ConfigLOG,"IsEnabled")                      ;Status of the checkbox to create log in certain time limit
        $NumberOfLogfilesButtonStatus=ControlCommand($WIN_CANLog,"",$EDIT_NumberOfLogfiles_ConfigLOG,"IsEnabled")        ;Status of the checkbox to enable the number of logfiles to be created

		;=============================
		if $addButtonStatus=1 And $EnDisablButtonStatus=1 And $removeButtonStatus=0 And $okButtonStatus=1 And $FilterButtonStatus=0 Then
			$validationObjectForButtons=1
		EndIf

		if $OnTransmissionButtonStatus=0 And $LogfileSizeButtonStatus=0 And $LogOnTimeButtonStatus=0 And $NumberOfLogfilesButtonStatus=0 Then
			$validationObjectForAdvancedLogging=1
		EndIf

		if $logFPathButtonStatus=0 And $TimeModeButtonStatus=0 And $ChannelButtonStatus=0 And $NumModeDecButtonStatus=0 And $NumModeHexButtonStatus=0 And $FileModeAppButtonStatus=0 And $FileModeOverWriteButtonStatus=0 Then
			$validationObjectForLogfileDetails=1
		EndIf

		if $StartTrigButtonStatus=0 And $StopTrigButtonStatus=0 And $StartMsgIDButtonStatus=0 And $StopMsgIDButtonStatus=0 Then
			$validationObjectForLogTrigger=1
		EndIf

		;=============================

		ConsoleWrite("$validationObjectForButtons :" &$validationObjectForButtons & @crlf)
		ConsoleWrite("$validationObjectForAdvancedLogging :" &$validationObjectForAdvancedLogging & @crlf)
		ConsoleWrite("$validationObjectForLogfileDetails :" &$validationObjectForLogfileDetails & @crlf)
		ConsoleWrite("$validationObjectForLogTrigger :" &$validationObjectForLogTrigger & @crlf)
		ConsoleWrite("$logfilesCount :" &$logfilesCount & @crlf)
		if $validationObjectForButtons=1 And $validationObjectForAdvancedLogging=1 And $validationObjectForLogfileDetails=1 And $validationObjectForLogTrigger=1 And $logfilesCount=0 Then
			$validationObjectTS1=1
		EndIf


		ControlClick($WIN_CANLog,"",$BTN_Add_ConfigLOG)                                                                            ;Click on add button
		$OnTransmissionButtonStatus=ControlCommand($WIN_CANLog,"",$CHKB_OnTransmission_ConfigLOG,"IsEnabled")           		   ;Status of the checkbox to create log for every bus connect
		$LogfileSizeButtonStatus=ControlCommand($WIN_CANLog,"",$CHKB_LogfileSize_ConfigLOG,"IsEnabled")                  		   ;Status of the checkbox to create log after each file size exceeding the limit
		$LogOnTimeButtonStatus=ControlCommand($WIN_CANLog,"",$CHKB_LogOnTime_ConfigLOG,"IsEnabled")                     		   ;Status of the checkbox to create log in certain time limit

		if $OnTransmissionButtonStatus=1 And $LogfileSizeButtonStatus=1 And $LogOnTimeButtonStatus=1 Then                          ;Checking whether checkboxes in advanced logging are enabled
			$validationObjectForAdvLogAfterAddingLog=1
		EndIf

		ControlCommand($WIN_CANLog,"",$CHKB_OnTransmission_ConfigLOG,"Check")   													;check the checkbox to create log for every bus connect
		ControlCommand($WIN_CANLog,"",$CHKB_LogfileSize_ConfigLOG,"Check")  														;check the checkbox to create log after each file size exceeding the limit
		ControlCommand($WIN_CANLog,"",$CHKB_LogOnTime_ConfigLOG,"Check") 													        ;check the checkbox to create log in certain time limit
		$NumberOfLogfilesButtonStatus=ControlCommand($WIN_CANLog,"",$EDIT_NumberOfLogfiles_ConfigLOG,"IsEnabled")                   ;Status of the checkbox to enable the number of logfiles to be created

		ConsoleWrite("$validationObjectForAdvLogAfterAddingLog :" &$validationObjectForAdvLogAfterAddingLog & @crlf)

		$NumberOfLogfilesHWD=controlgethandle($WIN_CANLog,"",$EDIT_NumberOfLogfiles_ConfigLOG)										; Fetch NumberOfLogfiles Edit control handle
		$NumberOfLogfilestxt=_GUICtrlEdit_GetText($NumberOfLogfilesHWD)

		$FilesizeHWD=controlgethandle($WIN_CANLog,"",$EDIT_Filesize_ConfigLOG)														; Fetch Filesize Edit control handle
		$Filesizetxt=_GUICtrlEdit_GetText($FilesizeHWD)

		$HoursHWD=controlgethandle($WIN_CANLog,"",$EDIT_Hours_ConfigLOG)															; Fetch Hours Edit control handle
		$Hourstxt=_GUICtrlEdit_GetText($HoursHWD)

		$MinutesHWD=controlgethandle($WIN_CANLog,"",$EDIT_Minutes_ConfigLOG)														; Fetch Minutes Edit control handle
		$Minutestxt=_GUICtrlEdit_GetText($MinutesHWD)

		ConsoleWrite("$validationObjectForAdvLogAfterAddingLog :" &$validationObjectForAdvLogAfterAddingLog & @crlf)
		ConsoleWrite("$NumberOfLogfilesButtonStatus :" &$NumberOfLogfilesButtonStatus & @crlf)
		ConsoleWrite("$NumberOfLogfilestxt :" &$NumberOfLogfilestxt & @crlf)
		ConsoleWrite("$Filesizetxt :" &$Filesizetxt & @crlf)
		ConsoleWrite("$Hourstxt :" &$Hourstxt & @crlf)
		ConsoleWrite("$Minutestxt :" &$Minutestxt & @crlf)
		if $validationObjectForAdvLogAfterAddingLog=1 And $NumberOfLogfilesButtonStatus=1 And $NumberOfLogfilestxt=10 And $Filesizetxt="50" And $Hourstxt=0 And $Minutestxt=30  Then
			$validationObjectTS2=1
		EndIf

		ConsoleWrite("$validationObjectTS1 :" &$validationObjectTS1 & @crlf)
		ConsoleWrite("$validationObjectTS2 :" &$validationObjectTS2 & @crlf)

		if $validationObjectTS1=1 And $validationObjectTS2=1  Then
			_WriteResult("Pass","TS_Log_001")
		Else
			_WriteResult("Fail","TS_Log_001")
		endif

	EndIf

	_CloseApp()

EndIf

ConsoleWrite("****End : TS_LOG_01.au3****"&@CRLF)
ConsoleWrite(@CRLF)