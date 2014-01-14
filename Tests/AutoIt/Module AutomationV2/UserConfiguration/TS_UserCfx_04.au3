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
Local $path=_TestDataPath()
    _launchApp()
	WinActivate($WIN_BUSMASTER)
	if winexists($WIN_BUSMASTER) Then
		_loadConfig("cfx_UserCfx_04.cfx")
	     Sleep(1000)
		  _CloseApp()
		  sleep(1000)
		  _launchApp()
	 $handle=ControlGetHandle($WIN_BUSMASTER,"","[CLASS:msctls_statusbar32; INSTANCE:2]")
		  ConsoleWrite("handle" & $handle & @CRLF)
		  $text=_GUICtrlStatusBar_GetText($handle,1)
		  ConsoleWrite("text" & $text & @CRLF)
		  If($text="Config File: " &$path&"\cfx_UserCfx_04.cfx") Then
			_WriteResult("Pass","TS_UserCfx_04")
         Else
		   _WriteResult("Fail","TS_UserCfx_04")
	   EndIf
   EndIf


$isAppNotRes=_CloseApp()																				; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_UserCfx_04")
EndIf



