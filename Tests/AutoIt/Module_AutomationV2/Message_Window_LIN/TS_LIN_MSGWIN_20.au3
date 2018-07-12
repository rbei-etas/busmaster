#cs
;==========================================================================Test Procedure =========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_LIN_MSGWIN_20
; TestCases			:		Message interpretation
; Test Data			:
; Test Setup		:		1. Create a filter
							2. Open configure message display window
							3. Click on configure button
							4. Select a filter
							5. Click on OK button
							6. Close and relaunch the application


; Expected Results  : 		After step 5, the selected filter will be shown in the filter list section. The filter will be automatically enabled(in checked state)
							After step 6, the configured settings should be retained in the configuration
;==========================================================================Test Procedure =========================================================================
#ce

;#include <IncludeFiles.au3>
ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_LIN_MSGWIN_20.au3****"&@CRLF)

_launchApp()

WinActivate($WIN_BUSMASTER)

if winexists($WIN_BUSMASTER) then
$FilterSel = 0
$FilterRetained  = 0
$FilterItemLINChecked2 = "False"

	;------------

	_createConfig("cfxLMW_20") 																		; Create New Configuration
	Sleep(1000)

	if winexists($WIN_ConfirmSaveAs) Then

		ControlClick($WIN_ConfirmSaveAs,"",$BTN_Yes_SaveAs)

	EndIf

	_linDriverSelection()
	ConsoleWrite("Driver selection is done" & @CRLF)

	_openLINClusterConfig()																			;open cluster config menu for LIN
	ConsoleWrite("Opened cluster configuration window" & @CRLF)

	_importLDF("LINALTECM_01-00")																	;import LDF file
	ConsoleWrite("LDF file imported for LIN" & @CRLF)


	_selectECU("ALL")																				;Select ECU

	ControlCommand($WIN_ClusterConfiguration, "", $LinEnableMasterMode , "Check")					;select check box for "Enable master mode"

	ControlClick($WIN_ClusterConfiguration,"",$BTN_OK_ClusterConfig)								;click on OK button
	;---------------


	;_loadConfig("cfxLINMsgWin_03")															 		; Load Configuration


	 _AddFilter_LIN("Stop","[0x15]ALT_LINALT_R_02","","","Tx","All")
	Sleep(2000)

	;open menu LIN->Message Window -> configure
	WinMenuSelectItem($WIN_BUSMASTER,"",$linMenu,$LINMsgWinMenu,$configMsgWindowLIN)
	sleep(2000)

	_AddFiltertoMsgDisp_LIN()																		; Adds the filter for message display for LIN

	sleep(1000)
	if WinExists($WIN_MsgDisplay_LIN) Then
		;---After step 5, the selected filter will be shown in the filter list section.
		$fltSelhWd1=controlgethandle($WIN_MsgDisplay_LIN,"",$LinconfgMsgDisp) 						; Get handle of filter selection list view
		$FilterItemLINChecked = _GUICtrlListView_GetItemChecked($fltSelhWd1,0)						; Get state of check box of  first filter

		$ItemCount=_GUICtrlListView_GetItemCount($fltSelhWd1)
		ConsoleWrite("$ItemCount= " & $ItemCount & @CRLF)											;Get the items count
		ConsoleWrite("$ItemCount=" &$ItemCount&  @CRLF)
		;$FilterItem = 1
	EndIf

	if ($FilterItemLINChecked = "True") Then														; check the first filter is automatically ebnabled
		ConsoleWrite("$FilterItemLINChecked=" & $FilterItemLINChecked &@CRLF)

		$FilterSel = 1
		ConsoleWrite("$FilterSel=" & $FilterSel &@CRLF)
		ConsoleWrite("The selected filter is shown in the filter list section. The filter is automatically enabled...." & $FilterSel &@CRLF)
	EndIf
	;------------------------
	;After step 6, the configured settings should be retained in the configuration
	ConsoleWrite("$FilterSel=" & $FilterSel &@CRLF)
	sleep(2000)
	ControlClick($WIN_MsgDisplay_LIN,"",$BTN_OK_MsgDisplay)											; Click on OK button
	Sleep(1000)
	_saveConfig()																					;save config

EndIf

$isAppNotRes=_CloseApp()																			; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_LIN_MSGWIN_20")
EndIf

_launchApp()																					;Relaunch Application
Sleep(2000)

if winexists($WIN_BUSMASTER) Then

	_AppFilterMenu_LIN()

	;WinActivate($WIN_Config_filter_LIN)
	WinWaitActive($WIN_Config_filter_LIN,"",2)
	sleep(2000)
	if WinExists($WIN_Config_filter_LIN) Then


		$fltSelhWd2=controlgethandle($WIN_Config_filter_LIN,"",$FilterListLIN) 					; Get handle of filter selection list view---1258
		$ItemCount=_GUICtrlListView_GetItemCount($fltSelhWd2)

		Sleep(1000)
		ConsoleWrite("$ItemCount= " & $ItemCount & @CRLF)										;Get the items count


		if $ItemCount = 1   Then
			ConsoleWrite("Retained the prev config for configure filter List" &@CRLF)
			$FilterRetained = 1
		sleep(750)
		EndIf

		ControlClick($WIN_Config_filter,"",$BTN_OK_ConfigFilter)								; Click on OK button
		sleep(500)
		_OpenMsgWindConfig_LIN()																;open Msg Window COnfig-LIN

		if WinExists($WIN_MsgDisplay_LIN) Then
			;To validate prev config in Window-Config -LIN
			$fltSelhWd3=controlgethandle($WIN_MsgDisplay_LIN,"",$LinconfgMsgDisp) 				; Get handle of filter selection list view
			$FilterItemLINChecked2 = _GUICtrlListView_GetItemChecked($fltSelhWd3,0)				; Get state of check box of  first filter

			$ItemCount1=_GUICtrlListView_GetItemCount($fltSelhWd1)
			ConsoleWrite("$ItemCount1= " & $ItemCount1 & @CRLF)									;Get the items count

		EndIf


	EndIf
EndIf


if $FilterSel = 1 and $FilterRetained = 1 and $FilterItemLINChecked2 = "True" Then
	_WriteResult("Pass","TS_LIN_MSGWIN_20")
	ConsoleWrite("PASS" & @CRLF)
Else
	_WriteResult("Fail","TS_LIN_MSGWIN_20")
	ConsoleWrite("FAIL" & @CRLF)

endif



$isAppNotRes=_CloseApp()																		; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_LIN_MSGWIN_20")
EndIf

ConsoleWrite("****End : TS_LIN_MSGWIN_20.au3****"&@CRLF)
ConsoleWrite(@CRLF)




