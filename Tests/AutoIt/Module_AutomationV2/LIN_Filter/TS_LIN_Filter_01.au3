#cs
;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_LIN_Filter_01
; TestCases			:		Validity of filter list for message window
; Test Data			:
; Test Setup		:		"1. Invoke dialog box for filter configuration
							2. Enter ID of a message to update the filter list
							3. Confirm by pressing OK button
							4. Enable message filter
							5. Send the same message across LIN bus"

Expected Results  : 		"The message ID shouldn't appear in the message window"

#ce
;==========================================================================Test Procedure =========================================================================
;#include <IncludeFiles.au3>
ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start :TS_LIN_Filter_01.au3****"&@CRLF)
Local $Msg = ObjCreate("System.Collections.ArrayList") ; create
$FirstMsg = 0
$StopMsgId = 0
;#include <IncludeFiles.au3>


_launchApp()

WinActivate($WIN_BUSMASTER)

if winexists($WIN_BUSMASTER) then

	_loadConfig("cfxLINFilter_01")																	; Load Configuration

	;_SetViewToolBarLIN()																			; Select LIN Tool Bar to view.
	sleep(1000)
	_linDriverSelection()																 		; Select LIN driver selection from xml configuration File.

	;_openLINClusterConfig()

	;if winexists($WIN_ClustrConfigLIN) then
		;ControlCommand($WIN_ClustrConfigLIN, "",$chckBoxEnableMaster, "Check", "")
		;sleep(1000)
		;ControlClick($WIN_ClustrConfigLIN,"",1)
	;EndIf



	ConsoleWrite("Driver selection is done" & @CRLF)

;-----------------------------------
	_AddFilter_LIN("Stop","[0x1]Frame1","","","Tx","All")							; Configure Filter details
	sleep(1000)

	_MsgDisplayMenuLIN()															; Open Message Display dialog -LIN


	_AddFiltertoMsgDisp_LIN()													   ; Adds the filter for message display for LIN
	sleep(1000)

	;_En_Dis_FilterDisplay()														  ; Enable filters for message display

	;ControlClick($WIN_MsgDisplay_LIN,"",$BTN_OK_MsgDisplay)						  ; Click on OK button

	sleep(2000)

	WinRibbonSelectItem($WIN_BUSMASTER,"",$linMenu,$LINMsgWinMenu,$EnableMsgWindowLIN)
	sleep(1000)

	sleep(1000)
	_ConnectDisconnect_LIN()													;Click connect/Disconnect in tool bar-LIN
	sleep(1000)
	_ConnectDisconnect_LIN()													;Click connect/Disconnect in tool bar-LIN
	Sleep(1000)

	_Maximize_childWindow("Message Window - LIN")
	Sleep(1000)
	;WinActivate("Message Window - LIN")

	$rCount=_GetLINMsgWinItemCount()																;get the row count
	ConsoleWrite("$rCount="&$rCount)

	if $rCount = 1 Then

		$Msg=_GetMsgWinInfo($LVC_CID_LINMsgWin,0,10)												; Fetch the first row data in the msg window
		$Count1=_GetLINMsgWinColCount()
		for $i=0 to $Count1-2
			$Value =$Msg.Item($i)
			ConsoleWrite("$Msg[" & $i &"] : "& $Value &@CRLF)
		next

		if $Msg.Item(0) <>" " and $Msg.Item(1)="Frame2" and $Msg.Item(2)="LIN Message" and $Msg.Item(3)="Tx" and $Msg.Item(4)=1 and $Msg.Item(5)=4 and $Msg.Item(6)="0x002" and $Msg.Item(7)="01 00 00 00" and $Msg.Item(8)="0xBC (""Enhanced"")" Then			; Compare the Direction, Channel and Msg ID of the first row
			$FirstMsg=1
		EndIf


	EndIf


EndIf
ConsoleWrite("$FirstMsg="&$FirstMsg& @CRLF)

if $FirstMsg=1   Then
	_WriteResult("Pass","TS_LIN_Filter_01")
	ConsoleWrite("PASS" & @CRLF)
Else
	_WriteResult("Fail","TS_LIN_Filter_01")
	ConsoleWrite("FAIL" & @CRLF)

endif



$isAppNotRes=_CloseApp()																		; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_LIN_Filter_01")
EndIf

ConsoleWrite("****End : TS_LIN_Filter_01.au3****"&@CRLF)
ConsoleWrite(@CRLF)


