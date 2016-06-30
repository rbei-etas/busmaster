#cs
;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_LIN_MSGWIN_12
; TestCases			:		Archieving message window attributes.
; Test Data			:
; Test Setup		:		2. Make sure a Frames are configured in TX window in  current configuration.
							3. Enable overwrite mode
							4. Save configuration and close the application.
							5. Start application and make sure the previous configuration is loaded.
							6. Check status of overwrite
							7. Disable overwrite mode
							8. Save configuration and close the application.
							9. Start application and make sure the previous configuration is loaded.
							10.Again Check status of overwrite option

; Expected Results  : 		1. After step6, The 'overwrite' mode and  should be in enabled state.
							2.After step10, The 'overwrite' mode  should be in disabled state.
;==========================================================================Test Procedure =========================================================================
#ce
;#include <IncludeFiles.au3>
ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start :TS_LIN_MSGWIN_12.au3****"&@CRLF)

_launchApp() 																	; Check if the App is open else launch it
ConsoleWrite("started bus master application")


ProcessWait("BUSMASTER.exe") 													; Pauses script execution until a given process exists.
sleep(1000)
$FirstMsg=0

WinActivate($WIN_BUSMASTER, 3)

if winexists($WIN_BUSMASTER) then

	_loadConfig("cfxLMW_12.cfx")												; Create New Configuration
	Sleep(1000)

	;_linDriverSelection()														; DriverSelection
	_linDriverSelection()
	ConsoleWrite("Driver selection is done" & @CRLF)

	_EnableOverwriteMode()
	ConsoleWrite("Enable overwrite mode" & @CRLF)

	_saveConfig()
	ConsoleWrite("Saved config..." & @CRLF)

	Sleep(1000)
	$isAppNotRes=_CloseApp()													; Close the app
	ConsoleWrite("closed App..." & @CRLF)

	_launchApp() 																;Lunch App again
	;sleep(12000)

	ProcessWait("BUSMASTER.exe") 												; Pauses script execution until a given process exists.
	sleep(1000)

	$OvrWriteToolhWd=ControlGetHandle($WIN_BUSMASTER,"",$ToolBar_OverWrite)		; Get handle of the 'Connect/Disconnect' toolbar
	;$res = _GUICtrlToolbar_GetButtonInfo($OvrWriteToolhWd,4)
	$resOvrWrite = _GUICtrlToolbar_IsButtonPressed($OvrWriteToolhWd,4)


	ConsoleWrite("$resOvrWrite = " &$resOvrWrite &@CRLF)


	_DisableOverwriteMode()														;Disable overwrite mode

	_saveConfig()
	ConsoleWrite("Saved config after disabled overwrite mode..." & @CRLF)

	$isAppNotRes=_CloseApp()													; Close the app
	ConsoleWrite("closed App..." & @CRLF)

	_launchApp()

	ProcessWait("BUSMASTER.exe") 												; Pauses script execution until a given process exists.
	sleep(1000)

	$OvrWriteToolhWd1=ControlGetHandle($WIN_BUSMASTER,"",$ToolBar_OverWrite)	; Get handle of the 'Connect/Disconnect' toolbar
	;$res = _GUICtrlToolbar_GetButtonInfo($OvrWriteToolhWd,4)
	$resOvrWrite1 = _GUICtrlToolbar_IsButtonPressed($OvrWriteToolhWd,4)			; Get the status of overwriteTool Button

	ConsoleWrite("$resOvrWrite1 = " &$resOvrWrite1 &@CRLF)


EndIf


if $resOvrWrite=True and $resOvrWrite1 =False Then
		_WriteResult("Pass","TS_LIN_MSGWIN_12")
		ConsoleWrite("Pass"& @CRLF)
	Else
		_WriteResult("Fail","TS_LIN_MSGWIN_12")
		ConsoleWrite("Fail"& @CRLF)
EndIf



Sleep(1000)
$isAppNotRes=_CloseApp()																							; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_LIN_MSGWIN_12")
	ConsoleWrite("WARNING")

EndIf



ConsoleWrite("****End : TS_LIN_MSGWIN_12.au3****"&@CRLF)
ConsoleWrite(@CRLF)




