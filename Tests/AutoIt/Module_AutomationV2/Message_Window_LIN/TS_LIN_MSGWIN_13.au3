#cs
;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_LIN_MSGWIN_13
; TestCases			:		Archieving message window attributes.
; Test Data			:
; Test Setup		:		1.Configure few frames for transmission in TX window
							2. Choose 'Relative Time' in message window 'Absolute/Relative Time display' option.
							3. Save configuration and close the application.
							4. Start application and make sure the previous configuration is loaded.
							5. Check status of 'Absolute/Relative Time display' option

; Expected Results  : 		1. The 'Absolute/Relative Time display' option should be set to 'Relative Time'.
;==========================================================================Test Procedure =========================================================================
#ce
;#include <IncludeFiles.au3>
ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start :TS_LIN_MSGWIN_13.au3****"&@CRLF)

_launchApp() 																	; Check if the App is open else launch it
ConsoleWrite("started bus master application")


ProcessWait("BUSMASTER.exe") 													; Pauses script execution until a given process exists.
sleep(1000)
$FirstMsg=0

WinActivate($WIN_BUSMASTER, 3)

if winexists($WIN_BUSMASTER) then


	;--1.Configure few frames for transmission in TX window-------
	_loadConfig("cfxLINMsgWin_06")												; Create New Configuration
	Sleep(1000)


	;_linDriverSelection()														; DriverSelection
	_linDriverSelection()
	ConsoleWrite("Driver selection is done" & @CRLF)

	WinMenuSelectItem($WIN_BUSMASTER,"","&LIN","&Message Window","Time &Mode","&Relative Time")
	Sleep(500)
	 WinMenuSelectItem($WIN_BUSMASTER,"",$fileMenu,$Exit)								; Select File->Exit from menu
	sleep(1000)															;
	if WinWaitActive($WIN_BUSMASTER,$SaveChangesConfigTXT,2) Then						; wait for save configuration dialog
		ControlClick($WIN_BUSMASTER,$SaveChangesConfigTXT,$BTN_Yes_SaveConfig)			; Click on Yes button

		ConsoleWrite("Application Closed through menu"&@CRLF)
	EndIf
	Sleep(1000)

	_launchApp()
	Sleep(1000)

	ProcessWait("BUSMASTER.exe") 														; Pauses script execution until a given process exists.
	sleep(1000)

	$winhWnd = WinGetHandle($WIN_BUSMASTER)                                              ; Fetch the window handle
	$hMain = _GUICtrlMenu_GetMenu($winhWnd)                                              ; Fetch the handle of the menu
	ConsoleWrite("menu handle : "&$hMain& @CRLF)
	If winexists($WIN_LINMsgWind) Then
		$hFile = _GUICtrlMenu_GetItemSubMenu($hMain, 4)								      ; Fetch the handle of LIN menu
		ConsoleWrite("inside win exit : "&$hFile& @CRLF)
	Else
		$hFile = _GUICtrlMenu_GetItemSubMenu($hMain, 3)
	EndIf
	$hSubmenu=_GUICtrlMenu_GetItemSubMenu($hFile, 8)                                     ; Fetch the handle of LIN->Msg Window menu
	$hsubmenu_time=_GUICtrlMenu_GetItemSubMenu($hSubmenu,5)                              ; Fetch the handle of LIN->Msg Window menu->Time mode
	$text=_GUICtrlMenu_GetItemText($hSubmenu,5)
    $resTimeMode=_GUICtrlMenu_GetItemChecked($hsubmenu_time,2)                           ; Check whether Relative time mode is checked
	ConsoleWrite("$text = " & $text & @CRLF)
	ConsoleWrite("$resTimeMode = " &  $resTimeMode & @CRLF)

	EndIf

if $resTimeMode=True Then
		_WriteResult("Pass","TS_LIN_MSGWIN_13")
		ConsoleWrite("Pass"& @CRLF)
	Else
		_WriteResult("Fail","TS_LIN_MSGWIN_13")
		ConsoleWrite("Fail"& @CRLF)
EndIf


Sleep(1000)
$isAppNotRes=_CloseApp()																	; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_LIN_MSGWIN_13")
	ConsoleWrite("WARNING")

EndIf



ConsoleWrite("****End : TS_LIN_MSGWIN_13.au3****"&@CRLF)
ConsoleWrite(@CRLF)




