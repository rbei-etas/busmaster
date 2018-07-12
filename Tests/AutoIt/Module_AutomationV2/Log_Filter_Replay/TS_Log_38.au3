;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_Log_38
; TestCases			:		Archieving Logging configuration.
; Test Data			:
; Test Setup		:		1. Continue from previous test case.
;~ 							2. Check the 'Enable/disable logging during tool connect/disconnect' check box at the bottom of the dialog.
;~ 							3. Click 'Ok' to close the dialog.
;~ 							4. Reopen the 'Configure logging for CAN' dialog.
;~ 							5. Click 'Ok' to close the dialog.
;~ 							6. Save the configuration.
;~ 							7. Close Busmaster application and reopen the application.
;~ 							8. Make sure the previous configuration is loaded.
;~ 							9. Open the 'Configure logging for CAN' dialog.

; Expected Results  : 		1. After step 10, the previously added 'log file' should be present in  the 'Configure logging for CAN' dialog.

; Critical (C)		:		Y
; TestCase No.		:		TS_Log_39
; TestCases			:		Archieving Logging configuration.
; Test Data			:
; Test Setup		:		1. Continue from previous test case.
;~ 							2. Check the 'Enable/disable logging during tool connect/disconnect' check box at the bottom of the dialog.
;~ 							3. Click 'Ok' to close the dialog.
;~ 							4. Reopen the 'Configure logging for CAN' dialog.
;~ 							5. Click 'Ok' to close the dialog.
;~ 							6. Save the configuration.
;~ 							7. Close Busmaster application and reopen the application.
;~ 							8. Make sure the previous configuration is loaded.
;~ 							9. Open the 'Configure logging for CAN' dialog.

; Expected Results  : 		1. After step4, the check box 'Enable/disable logging during tool connect/disconnect' should be in checked state.
;~ 								After step9, the check box 'Enable/disable logging during tool connect/disconnect' should be in checked state.

; Critical (C)		:		Y
; TestCase No.		:		TS_Log_40
; TestCases			:		Archieving Logging configuration.
; Test Data			:
; Test Setup		:		1. Continue from previous test case.
;~ 							2. Change the 'Time mode' option to 'Relative' and 'channel' to 1 from the respective comboboxes.
;~ 							3. Click 'Ok' to close the dialog.
;~ 							4. Reopen the 'Configure logging for CAN' dialog.
;~ 							5. Save the configuration.
;~ 							6. Close Busmaster application and reopen the application.
;~ 							7. Make sure the previous configuration is loaded.
;~ 							8. Open the 'Configure logging for CAN' dialog.

; Expected Results  : 		1. After step4 and step8, The 'Time mode' should be 'Relative' and channel should be 1 in the 'Configure logging for CAN' dialog.

; Critical (C)		:		Y
; TestCase No.		:		TS_Log_41
; TestCases			:		Archieving Logging configuration.
; Test Data			:
; Test Setup		:		1. Continue from previous test case.
;~ 							2. Choose the 'Numeric format' as 'Hex' and 'File mode' to 'Overwrite' by choosing respective radioboxes.
;~ 							3. Click 'Ok' to close the dialog.
;~ 							4. Reopen the 'Configure logging for CAN' dialog.
;~ 							5.Save the configuration.
;~ 							6. Close Busmaster application and reopen the application.
;~ 							7. Make sure the previous configuration is loaded.
;~ 							8. Open the 'Configure logging for CAN' dialog.

; Expected Results  : 		1. After step4 and step8, the 'Numeric format' should be 'Hex' and 'File mode' should be 'Overwrite' in the 'Configure logging for CAN' dialog.

; Critical (C)		:		Y
; TestCase No.		:		TS_Log_42
; TestCases			:		Archieving Logging configuration.
; Test Data			:
; Test Setup		:		1. Continue from previous test case.
;~ 							2. Check the options 'Start on Reception of ID 0x' and 'Stop on Reception of ID 0x' and assign values 0x1 and 0xA respectively.
;~ 							3. Click 'Ok' to close the dialog.
;~ 							4. Reopen the 'Configure logging for CAN' dialog.
;~ 							5.Click ok and close
;~ 							6.Save the configuration.
;~ 							7. Close Busmaster application and reopen the application.
;~ 							8. Make sure the previous configuration is loaded.
;~ 							9. Open the 'Configure logging for CAN' dialog.

; Expected Results  : 		1. After step4 and step9, the check boxes  'Start on Reception of ID 0x' and 'Stop on Reception of ID 0x' should be in checked state and 0x1 and 0xA should be the respective values.

; Critical (C)		:		Y
; TestCase No.		:		TS_Log_43
; TestCases			:		Archieving Logging configuration.
; Test Data			:
; Test Setup		:		1. Continue from previous test case.
;~ 							2. Add the preconfigured filters for log file by using 'Filters' option.
;~ 							3. Click 'Ok' to close the dialog.
;~ 							4. Reopen the 'Configure logging for CAN' dialog.
;~ 							Click 'Ok' to close the dialog.
;~ 							3. Save the configuration.
;~ 							5. Close Busmaster application and reopen the application.
;~ 							6. Make sure the previous configuration is loaded.
;~ 							6. Open the 'Configure logging for CAN' dialog.

; Expected Results  : 		1. The filters configured in step 2 for log file should be present in the 'Selected Filters' list in 'Filter Selection dialog'.

; Critical (C)		:		Y
; TestCase No.		:		TS_Log_44
; TestCases			:		Archieving Logging configuration.
; Test Data			:
; Test Setup		:		1. Continue from previous test case.
;~ 							2. Remove the log file by clicking on 'Remove' option in 'Log blocks' groupbox.
;~ 							3. Click 'Ok' to close the dialog.
;~ 							4. Reopen the 'Configure logging for CAN' dialog.
;~ 							5.Click 'Ok' to close the dialog.
;~ 							6. Save the configuration.
;~ 							7. Close BUSMASTER application and reopen the application.
;~ 							8. Make sure the previous configuration is loaded.
;~ 							9. Open the 'Configure logging for CAN' dialog.

; Expected Results  : 		1. After step4 and step9, the log files list should be empty and all the options in 'Log File Details' groupbox should be in disabled state.
;==========================================================================Test Procedure =========================================================================
ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_Log_38.au3****"&@CRLF)

_launchApp()																					; Check if the App is open else launch it

WinActivate($WIN_BUSMASTER,3)

Local $LogConfigSettings=0, $LogConfigDisabled=0, $LogFileName="",$En_DisLogCHKBState=0,$TimeModeName="",$ChannelNo=0,$NumFormatState=0,$FileModeState=0,$StartRecState=0,$StartRecID=0,$StopRecState=0,$StopRecID=0,$FCount=0
Local $HexBTNState=0, $OverwriteBTNState=0
if winexists($WIN_BUSMASTER) then

	_loadConfig("cfxLog_38.cfx")																; Load Configuration

	_ConfigureCANLog("LOG_38_Sample","Relative",1,"Hex","Overwrite",10,100,"True")				; Configure Logging details

	_saveConfig()																				; Save the config

	_CloseApp()																					; Close the app

	_launchApp()																				; Launch the app

	_CANLogMenu()																				; Open configure logging window

	$hWd=controlgethandle($WIN_CANLog,"",$LSTC_LogFile_ConfigLOG)								; Get handle of log file instance
	$Count=_GUICtrlListView_GetItemCount($hWd)

	if $Count=1 Then

		$LogFileName=_GUICtrlListView_GetItemText($hWd,0,0)

		$En_DisLogCHKBState=ControlCommand ($WIN_CANLog,"",$BTN_EnDisable_ConfigLOG,"IsChecked")			; Fetch the "Enable/Disable Logging during tool Connect/Disconnect" checkbox status(Check/Uncheck)

		$TimeModeName=Controlcommand($WIN_CANLog,"",$COMB_TimeMode_ConfigLOG,"GetCurrentSelection")			; Fetch the current selection of "Timer Mode"

		$ChannelNo=Controlcommand($WIN_CANLog,"",$COMB_Channel_ConfigLOG,"GetCurrentSelection")				; Fetch the current selection of "Channel"

		$HexBTNState=Controlcommand($WIN_CANLog,"",$RBTN_NumModeHex_ConfigLOG,"IsChecked")					; Fetch the Numeric Format BTN status

		$OverwriteBTNState=Controlcommand($WIN_CANLog,"",$RBTN_FileModeOverWrite_ConfigLOG,"IsChecked")		; Fetch the File Mode BTN status

		$StartRecState=Controlcommand($WIN_CANLog,"",$CHKB_StartTrig_ConfigLOG,"IsChecked")					; Fetch start on reception BTN status

		$StartRecID=ControlGetText ($WIN_CANLog,"",$TXT_StartMsgID_ConfigLOG)								; Fetch start on reception MsgID

		$StopRecState=Controlcommand($WIN_CANLog,"",$CHKB_StopTrig_ConfigLOG,"IsChecked")					; Fetch stop on reception BTN status

		$StopRecID=ControlGetText ($WIN_CANLog,"",$TXT_StopMsgID_ConfigLOG)									; Fetch stop on reception MsgID

		Controlclick($WIN_CANLog,"",$BTN_Filter_ConfigLOG)													; Click on 'Filter' button
		WinWaitActive($WIN_FilterSelect,"",3)

		if winexists($WIN_FilterSelect) Then
			$SelFilterhWd=controlgethandle($WIN_FilterSelect,"",$LSTC_SelectedFilter_FilterSelect)			; Get handle of selected filter list view
			$FCount=_GUICtrlListView_GetItemCount($SelFilterhWd)											; Fetch the item count
			ControlClick($WIN_FilterSelect,"",$BTN_OK_FilterSelect)											; Click on OK button
		EndIf

		Controlclick($WIN_CANLog,"",$BTN_RemoveLog_ConfigLog)												; Click on 'Remove' button
		sleep(500)
		Controlclick($WIN_CANLog,"",$BTN_OK_ConfigLOG)												        ; Click on 'OK' button
	EndIf

	_saveConfig()																							; Save the config

	_CloseApp()																								; Close the app

	_launchApp()																							; Launch the app

	_CANLogMenu()																							; Open configure logging window

	if WinExists($WIN_CANLog) Then

		$IsLogfPathBTNDisabled=ControlCommand ($WIN_CANLog,"",$BTN_logFPath_ConfigLOG,"IsEnabled")			; Fetch the Log File Path button state(enabled/disabled)

		$IsTimeModeDisabled=Controlcommand($WIN_CANLog,"",$COMB_TimeMode_ConfigLOG,"IsEnabled")				; Fetch the "Time Mode" button state(enabled/disabled)

		$IsChlNoDisabled=Controlcommand($WIN_CANLog,"",$COMB_Channel_ConfigLOG,"IsEnabled")					; Fetch the "Channel No" button state(enabled/disabled)

		$IsDecBTNDisabled=Controlcommand($WIN_CANLog,"",$RBTN_NumModeDec_ConfigLOG,"IsEnabled")				; Fetch the "Dec" Rbutton state(enabled/disabled)

		$IsHexBTNDisabled=Controlcommand($WIN_CANLog,"",$RBTN_NumModeHex_ConfigLOG,"IsEnabled")				; Fetch the "Hex" Rbutton state(enabled/disabled)

		$IsAppBTNDisabled=Controlcommand($WIN_CANLog,"",$RBTN_FileModeApp_ConfigLOG,"IsEnabled")			; Fetch the "Append" Rbutton state(enabled/disabled)

		$IsOvrwriteBTNDisabled=Controlcommand($WIN_CANLog,"",$RBTN_FileModeOverWrite_ConfigLOG,"IsEnabled")	; Fetch the "Overwrite" Rbutton state(enabled/disabled)

		$IsStartRecBTNDisabled=Controlcommand($WIN_CANLog,"",$CHKB_StartTrig_ConfigLOG,"IsEnabled")			; Fetch start on reception Check box state(enabled/disabled)

		$IsStopRecBTNDisabled=Controlcommand($WIN_CANLog,"",$CHKB_StopTrig_ConfigLOG,"IsEnabled")			; Fetch stop on reception Check box state(enabled/disabled)

		$IsFilterBTNDisabled=Controlcommand($WIN_CANLog,"",$BTN_Filter_ConfigLOG,"IsEnabled")				; Fetch Filter button state(enabled/disabled)

		Controlclick($WIN_CANLog,"",$BTN_OK_ConfigLOG)														; Click on 'OK' button

	EndIf

	ConsoleWrite("$Count:"&$Count&@CRLF)
	ConsoleWrite("$LogFileName:"&$LogFileName&@CRLF)
	ConsoleWrite("$En_DisLogCHKBState:"&$En_DisLogCHKBState&@CRLF)
	ConsoleWrite("$TimeModeName:"&$TimeModeName&@CRLF)
	ConsoleWrite("$ChannelNo:"&$ChannelNo&@CRLF)
	ConsoleWrite("$HexBTNState:"&$HexBTNState&@CRLF)
	ConsoleWrite("$OverwriteBTNState:"&$OverwriteBTNState&@CRLF)
	ConsoleWrite("$StartRecState:"&$StartRecState&@CRLF)
	ConsoleWrite("$StartRecID:"&$StartRecID&@CRLF)
	ConsoleWrite("$StopRecState:"&$StopRecState&@CRLF)
	ConsoleWrite("$StopRecID:"&$StopRecID&@CRLF)
	ConsoleWrite("$FCount:"&$FCount&@CRLF)

	ConsoleWrite("$IsLogfPathBTNDisabled:"&$IsLogfPathBTNDisabled&@CRLF)
	ConsoleWrite("$IsTimeModeDisabled:"&$IsTimeModeDisabled&@CRLF)
	ConsoleWrite("$IsChlNoDisabled:"&$IsChlNoDisabled&@CRLF)
	ConsoleWrite("$IsDecBTNDisabled:"&$IsDecBTNDisabled&@CRLF)
	ConsoleWrite("$IsHexBTNDisabled:"&$IsHexBTNDisabled&@CRLF)
	ConsoleWrite("$IsAppBTNDisabled:"&$IsAppBTNDisabled&@CRLF)
	ConsoleWrite("$IsOvrwriteBTNDisabled:"&$IsOvrwriteBTNDisabled&@CRLF)
	ConsoleWrite("$IsStartRecBTNDisabled:"&$IsStartRecBTNDisabled&@CRLF)
	ConsoleWrite("$IsStopRecBTNDisabled:"&$IsStopRecBTNDisabled&@CRLF)
	ConsoleWrite("$IsFilterBTNDisabled:"&$IsFilterBTNDisabled&@CRLF)

	$LogFPath=_OutputDataPath()																				; Fetch the OutputData path
EndIf

if $LogFileName=$LogFPath & "\" & "LOG_38_Sample.log" and $En_DisLogCHKBState=1 and $TimeModeName="Relative" And $ChannelNo=1 and $HexBTNState=1 and $OverwriteBTNState=1 and $StartRecState=1 and $StartRecID=10 and $StopRecState=1 and $StopRecID=100 and $FCount=1 Then
	$LogConfigSettings=1
EndIf

if $IsLogfPathBTNDisabled=0 and $IsTimeModeDisabled=0 and $IsChlNoDisabled=0 And $IsDecBTNDisabled=0 and $IsHexBTNDisabled=0 and $IsAppBTNDisabled=0 and $IsOvrwriteBTNDisabled=0 and $IsStartRecBTNDisabled=0 and $IsStopRecBTNDisabled=0 and $IsFilterBTNDisabled=0 Then
	$LogConfigDisabled=1
EndIf

ConsoleWrite("$LogConfigSettings:"&$LogConfigSettings&@CRLF)
ConsoleWrite("$LogConfigDisabled:"&$LogConfigDisabled&@CRLF)

If $LogConfigSettings=1 and $LogConfigDisabled=1 then																				; Validate the result
	_WriteResult("Pass","TS_Log_38")
Else
	_WriteResult("Fail","TS_Log_38")
EndIf

$isAppNotRes=_CloseApp()																		; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_Log_38")
EndIf




ConsoleWrite("****End : TS_Log_38.au3****"&@CRLF)
ConsoleWrite(@CRLF)
