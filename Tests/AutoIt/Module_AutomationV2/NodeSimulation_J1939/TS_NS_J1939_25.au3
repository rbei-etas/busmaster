;==================================================================Test Cases/Test Data ===========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_NS_J1939_24
; TestCases			:		Executing UnLoad DLL Handler
; Test Data			:		cfxNSJ1939_24.cfx
; Test Setup		:		Edit the DLL handler I.e. OnDLL_UnLoad() and save it and select menu item User Program-->Unload

; Expected Results  : 		The DLL Unload handlers will be executed displaying the output in notification window
;=================================================================Test Procedure ====================================================================================
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_J1939Node_25****"&@CRLF)


_launchApp()

WinActivate($WIN_BUSMASTER)

if winexists($WIN_BUSMASTER) then

	Local $text=""

	_loadConfig("cfxNSJ1939_25.cfx")

    _ActivateJ1939()                                                                  ;Activate J1939 Message window

    _ClickNode(0)

	_J1939NodeSimConfigMenu()

	$TVHWD1=_GetJ1939CppTVHWD()	                                                           ; Fetch the handle of tree view
   _GUICtrlTreeView_Expand($TVHWD1,"",True)													; Expand the tree view items
    sleep(1000)

    _ClickNode(0)

    _AddJ1939CppFile("GlobalVariableJ1939.cpp")

	$TVHWD=_GetJ1939CppTVHWD()															; Fetch the handle of tree view
   	$DLLHWD=_GetJ1939HandlerHWD(2,8)														; Fetch the handle of the DLL handler item
   	_GUICtrlTreeView_ClickItem($TVHWD, $DLLHWD, "Right") 								; Right click on DLL Handler item in the tree view
   	sleep(500)
   	send("a")
	Sleep(1000)

	ControlSend($WIN_BUSMASTER, "", $TXT_TimeHandler_CPPFile, "int 5;")
	Sleep(1000)
    $text= ControlGetText("BUSMASTER - GlobalVariableJ1939.cpp","",59664)
	ConsoleWrite("$text" & $text & @CRLF)
	Sleep(1000)
	Send("^{F4}")
	If WinExists($WIN_BUSMASTER,"Save changes to GlobalVariableJ1939.cpp?") Then
		ControlClick($WIN_BUSMASTER,"",7)
		Sleep(1000)
	EndIf

	Send("^{F4}")

	_DeactJ1939()																			; Deactivate J1939

EndIf

If($text="int 5;") Then
	_WriteResult("Pass","TS_J1939Node_25")
Else
	_WriteResult("Fail","TS_J1939Node_25")
EndIf

$isAppNotRes=_CloseApp()																				; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_J1939Node_25")
EndIf

  ConsoleWrite("****End : TS_J1939Node_25****"&@CRLF)
  ConsoleWrite(@CRLF)

