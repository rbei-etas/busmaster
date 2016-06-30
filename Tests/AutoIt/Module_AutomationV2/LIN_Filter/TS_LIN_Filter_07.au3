#cs
;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_LIN_Filter_07
; TestCases			:		Check for pass filter mode
; Test Data			:
; Test Setup		:		"1. Invoke dialog box for filter configuration
							2. Specify a filter list
							3. Select stop filter mode
							4. Confirm by selecting OK button
							5. Enable message display / logging
							6. Send a some messages across LIN bus, which shall contain a few messages from the filter list"

Expected Results  : 		"Only those messages occuring in the filter list should be found in the message display window / log file"

#ce
;==========================================================================Test Procedure =========================================================================


ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start :TS_LIN_Filter_07.au3****"&@CRLF)
Local $Msg = ObjCreate("System.Collections.ArrayList") ; create
Dim $FirstMsg[13]=[1,2,3,4,5,6,7,8,9,10,11,12,13],$SecMsg[13]=[1,2,3,4,5,6,7,8,9,10,11,12,13]
Local $FirstMsg = 0,$msgidCheck=0,$txmsg=0,$endOfFIle=0
Local $FirstMsgBeforeEnbleFilter=0,$SecMsgBeforeEnbleFilter=0,$FirstMsgAfterEnbleFilter=0
Local $LoGFiltername = "LIN_Log_Filter_06",$rCount1=0,$rCount=0

_launchApp()

WinActivate($WIN_BUSMASTER)

if winexists($WIN_BUSMASTER) then

	_loadConfig("cfxLINFilter_07")																	; Load Configuration
	_SetViewToolBarLIN()																			; Select LIN Tool Bar to view.
	_linDriverSelection()

	Sleep(1000)

	_ConnectDisconnect_LIN()
	sleep(1000)
	_ConnectDisconnect_LIN()


	Sleep(1000)

	$rCount=_GetLINMsgWinItemCount()																;get the row count
	if $rCount = 2 Then


		$Count1=_GetLINMsgWinColCount()

		for $j=0 to $rCount-1
			$Msg=_GetMsgWinInfo($LVC_CID_LINMsgWin,$j,10)												; Fetch the first row data in the msg window

			if $Msg.Item(0) <>" " and $Msg.Item(1)="Frame1" and $Msg.Item(2)="LIN Message" and $Msg.Item(3)="Tx" and $Msg.Item(4)=1 and $Msg.Item(5)=4 and $Msg.Item(6)="0x001" and $Msg.Item(7)="12 00 00 00" and $Msg.Item(8)="0x2C (""Enhanced"")" Then			; Compare the Direction, Channel and Msg ID of the first row
				$FirstMsgBeforeEnbleFilter=1
			EndIf
			if $Msg.Item(0) <>" " and $Msg.Item(1)="Frame2" and $Msg.Item(2)="LIN Message" and $Msg.Item(3)="Tx" and $Msg.Item(4)=1 and $Msg.Item(5)=4 and $Msg.Item(6)="0x002" and $Msg.Item(7)="01 00 00 00" and $Msg.Item(8)="0xBC (""Enhanced"")" Then			; Compare the Direction, Channel and Msg ID of the first row
				$SecMsgBeforeEnbleFilter=1
			EndIf

		Next
	EndIf
		_EnableFilterDispMenuLIN()														  			; Enable filters for message display
		_ConnectDisconnect_LIN()																	; Connect LIN
		sleep(1000)
		_ConnectDisconnect_LIN()																	; DisConnect LIN

	$rCount1=_GetLINMsgWinItemCount()																; Get the row count
	if $rCount1 = 1 Then


		$Count1=_GetLINMsgWinColCount()
		for $i=0 to $Count1-2
			$Value =$Msg.Item($i)
			ConsoleWrite("$Msg1[" & $i &"] : "& $Value &@CRLF)
		next

		for $j=0 to $rCount1-1
			$Msg=_GetMsgWinInfo($LVC_CID_LINMsgWin,$j,10)												; Fetch the first row data in the msg window

			if $Msg.Item(0) <>" " and $Msg.Item(1)="Frame1" and $Msg.Item(2)="LIN Message" and $Msg.Item(3)="Tx" and $Msg.Item(4)=1 and $Msg.Item(5)=4 and $Msg.Item(6)=0x001 and $Msg.Item(7)="12 00 00 00" and $Msg.Item(8)="0x2C (""Enhanced"")" Then			; Compare the Direction, Channel and Msg ID of the first row
				$FirstMsgAfterEnbleFilter=1
			EndIf

		Next
	EndIf


EndIf

consolewrite("$FirstMsgBeforeEnbleFilter:"&$FirstMsgBeforeEnbleFilter&@CRLF)
consolewrite("$SecMsgBeforeEnbleFilter:"&$SecMsgBeforeEnbleFilter&@CRLF)
consolewrite("$FirstMsgAfterEnbleFilter:"&$FirstMsgAfterEnbleFilter&@CRLF)

if $FirstMsgBeforeEnbleFilter=1 and $SecMsgBeforeEnbleFilter=1 and $FirstMsgAfterEnbleFilter=1 Then							; Validate the result
	_WriteResult("Pass","TS_LIN_Filter_07")
	ConsoleWrite("Pass"&@CRLF)
Else
	_WriteResult("Fail","TS_LIN_Filter_07")
	ConsoleWrite("Fail"&@CRLF)
EndIf



$isAppNotRes=_CloseApp()																									; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_LIN_Filter_07")
EndIf

ConsoleWrite("****End : TS_LIN_Filter_07.au3****"&@CRLF)
ConsoleWrite(@CRLF)


