
;===============Test Cases/Test Data ========================
; Critical (C)		:		Y
; TestCase No.		:		TS_NS_J1939_01
; TestCases			:		Node all handlers enabling/disabling
; Test Data			:		cfxNSJ1939_01.cfx
; Test Setup		:		1.right click on the node name whose DLL is loaded.
;                           2.Enable any handler
;                           3.Try to execute the handler

;~
; Expected Results  : 		The handler should be executed
;================Test Procedure ==============================
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_NS_J1939_01.au3****"&@CRLF)


_launchApp()

WinActivate($WIN_BUSMASTER)
if winexists($WIN_BUSMASTER) then
    _loadConfig("cfxNSJ1939_01.cfx")                                                                ; Load Configuration
	sleep(1000)
	_ActivateJ1939()                                                                               ;Activate J1939 Message window
	sleep(1000)
	local $KeyHandler_A=0,$KeyHandler_Trace=0
	local $outtxt[200]
	_J1939NodeSimConfigMenu()																; Open CAN node Simulation window

	$TVHWD=_GetJ1939CppTVHWD()																; Fetch the handle of tree view
	_GUICtrlTreeView_Expand($TVHWD,"",True)													; Expand the tree view items
	sleep(1000)

	ControlFocus ( $WIN_BUSMASTER, "", $TVHWD )

	send("{DOWN 2}")																		; Send the down key twice to select the node
	sleep(1000)

 	_AddJ1939CppFile("KeyHandlerNS_J1939_01")														; Add a new Cpp file

 	_AddJ1939KeyHandler("a","BUSMASTER - KeyHandlerNS_J1939_01.cpp")																	; Add key handler for key "a"

	$KeyHWD=_GetJ1939HandlerHWD(2,4)
	_GUICtrlTreeView_ClickItem($TVHWD, $KeyHWD)					 							; Right click on Key Handler item in the tree view

	send("{DOWN}")																			; Send the down key to select the key handler function

	send("{ENTER}")

	ControlSend($WIN_BUSMASTER, "", $TXT_KeyHandler_CPPFile, "Trace" & "(" & Chr(34) & "You Pressed Key 'a' " & Chr(34) & ");")

	Send("^s")																				; Save the cpp file

	Send("^{F4}") 																			; Close the C file window

	_Click_BuildLoad()
	WinWaitActive("",$Msg_BuildLoadError,5)
	if winexists("",$Msg_BuildLoadError) Then

		_BuildLoadError()
																			; Close the pop up msg
		$OutputwinHWD=ControlGetHandle($WIN_BUSMASTER,"",$LSTB_Details_TraceWin)			; get the handle of output window
		$itemCount=_GUICtrlListBox_GetCount($OutputwinHWD)									; Get the output window item count
		for $i=0 to $itemCount-1
			$outtxt[$i]=_GUICtrlListBox_GetText($OutputwinHWD, $i)							; Fetch the text in the output window
			ConsoleWrite("$outtxt["&$i&"]:"&$outtxt[$i]&@CRLF)
		Next
		send("^{F4}")																		; Close the output window
		send("^{F4}")																		; Close Simulated system window

	Else

		_ConnectDisconnect()																; Connect the tool
		Sleep(700)

		_SelectHandler($KeyHandler_Index)													; select and Enable Key Handler

		send("a")																			; Invoke key handler
		sleep(2000)

		_ConnectDisconnect()																; Disconnect the tool
		Sleep(1000)

		_Click_Unload()																		; Click on unload button


		sleep(1000)

			$OutputwinHWD=ControlGetHandle($WIN_BUSMASTER,"",$LSTB_Details_TraceWinInfo)			; get the handle of output window
		$itemCount=_GUICtrlListBox_GetCount($OutputwinHWD)									; Get the output window item count
		for $i=0 to $itemCount-1
			$outtxt[$i]=_GUICtrlListBox_GetText($OutputwinHWD, $i)							; Fetch the text in the output window
			ConsoleWrite("$outtxt["&$i&"]:"&$outtxt[$i]&@CRLF)
		Next
		 $OutputTxt=_GUICtrlListBox_GetText($OutputwinHWD, 7)
		 ConsoleWrite("Text " & $OutputTxt & @CRLF)
		 _Click_Unload()																		; Click on unload button

		send("^{F4}")																		; Close output window
		sleep(1000)

		send("^{F4}")																		; Close the simulated system window
		sleep(1000)


;~ 		_DisableOverwriteMode()																; Disable overwrite mode

;~ 		_EnableHex()																		; Enable Hex display mode

	EndIf
	if $OutputTxt="You Pressed Key 'a' " Then									; Validate the result
	_WriteResult("Pass","TS_J1939Node_01")
Else
	_WriteResult("Fail","TS_J1939Node_01")
EndIf

$isAppNotRes=_CloseApp()															; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_J1939Node_01")
EndIf


ConsoleWrite("****End : TS_J1939Node_01.au3****"&@CRLF)
ConsoleWrite(@CRLF)


EndIf
