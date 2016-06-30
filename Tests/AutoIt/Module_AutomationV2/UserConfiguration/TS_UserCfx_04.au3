;==================================================Test Cases/Test Data =============================================
; Critical (C)		:		Y
; TestCase No.		:		TS_UserCfx_04
; TestCases			:		Auto loading of configuration file
; Test Data			:		cfx_UserCfx_04.cfx
; Test Setup		:		1. Close application if opened
;                           2. Invoke the application"


; Expected Results  : 		The configuration file cfx_UserCfx_03.cfx should be loaded successfully.
;                           It means that configuration file should be read successfully."
;
;==================================================Test Procedure =======================================================
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_UserCfx_04.au3****"&@CRLF)


_launchApp()

local $text=""
WinActivate($WIN_BUSMASTER)
if winexists($WIN_BUSMASTER) Then

	_loadConfig("cfx_UserCfx_04.cfx")

	_CloseApp()

	_launchApp()
	sleep(2000)

	$path=_TestDataPath()

	$handle=ControlGetHandle($WIN_BUSMASTER,"",$statusBar)
	ConsoleWrite("handle" & $handle & @CRLF)
	$text=_GUICtrlStatusBar_GetText($handle,1)
	ConsoleWrite("text" & $text & @CRLF)

EndIf

If($text="Config File: " &$path&"\cfx_UserCfx_04.cfx") Then
	_WriteResult("Pass","TS_UserCfx_04")
Else
	_WriteResult("Fail","TS_UserCfx_04")
EndIf


$isAppNotRes=_CloseApp()																				; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_UserCfx_04")
EndIf



