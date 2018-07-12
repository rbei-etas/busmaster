;==================================================Test Cases/Test Data =============================================
; Critical (C)		:		Y
; TestCase No.		:		TS_UserCfx_06
; TestCases			:		Saving associated database files
; Test Data			:		cfx_UserCfx_06.cfx
; Test Setup		:		1. Associate one or more database file to the Busmaster..
;                           2. Save the configuration.
;                           3. Close and open the Busmaster..
;                           4. Check the associate database files

; Expected Results  : 		1. After step 4 all the files associated in step 2 will exists
;==================================================Test Procedure =======================================================
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_UserCfx_06.au3****"&@CRLF)
Local $path=_TestDataPath()
    _launchApp()
	WinActivate($WIN_BUSMASTER)
	if winexists($WIN_BUSMASTER) Then
		_loadConfig("cfx_UserCfx_06.cfx")
	     Sleep(1000)
		  _AssociateCANDB("testUserConfig_06.dbf")
		  sleep(1000)
		  WinMenuSelectItem($WIN_BUSMASTER,"",$fileMenu,$Exit)								; Select File->Exit from menu
	sleep(2000)
	if WinWaitActive("",$SaveChangesConfigTXT,2) Then									; wait for save configuration dialog
		ControlClick("",$SaveChangesConfigTXT,$BTN_Yes_SaveConfig)						; Click on Yes button
	EndIf
	_launchApp()
	sleep(1000)
	WinMenuSelectItem($WIN_BUSMASTER,"","&CAN","&Database","&Dissociate")
	$handle=ControlGetHandle("Dissociate Database","",1286)
	$DBName=_GUICtrlListBox_GetText($handle,0)
	ConsoleWrite("Db name" & $DBName & @CRLF)
	sleep(1000)
	If($DBName=$path&"\testUserConfig_06.dbf") Then
		  _WriteResult("Pass","TS_UserCfx_06")
		Else
			_WriteResult("Fail","TS_UserCfx_06")
		EndIf
    EndIf


	$isAppNotRes=_CloseApp()																			; Close the app

	if $isAppNotRes=1 Then
         _WriteResult("Warning","TS_UserCfx_06")

	EndIf

	     ConsoleWrite("****End : TS_UserCfx_06.au3****"&@CRLF)
	    ConsoleWrite(@CRLF)




