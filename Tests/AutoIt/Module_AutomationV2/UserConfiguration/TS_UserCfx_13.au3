;==================================================Test Cases/Test Data =============================================
; Critical (C)		:  Y
; TestCase No.		:  TS_UserCfx_013
; TestCases			:  Deleted configuration file
;
; Test Setup		:  1. Note down the current configuration file of Busmaster.
;                      2. Close the  Busmaster.
;                      3. Delete the configuration file.
;                      4. Open the  Busmaster
;
; Expected Results  : 	After step 4 Busmaster will throw a error message saying the configuration file was not found.
;==================================================Test Procedure =======================================================
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_UserCfx_13.au3****"&@CRLF)
    _launchApp()
	WinActivate($WIN_BUSMASTER)
	$DirPath=_TestDataPath()
	$FilePath= @ScriptDir&"\TestData\cfx_UserCfx_13.cfx"
	if winexists($WIN_BUSMASTER) Then
		_createConfigTest("cfx_UserCfx_13.cfx")
		Sleep(1000)
		$isAppNotRes=_CloseApp()																		 ; Close the app
        if $isAppNotRes=1 Then
			ConsoleWrite("Application not responding!! Error")
	    EndIf
		$result=FileDelete($FilePath)                                                                    ;Delete cfx file from the specified location
		Sleep(1000)
		ConsoleWrite("$result: " & $result & @CRLF)
	    Run($BusMasterExeFPath & "\BUSMASTER.exe")
        ProcessWait($AppProcess)															             ; Pauses script execution until a given process exists.
		WinWaitActive($WIN_BUSMASTER,"",5)
        Sleep(1000)
		If WinWait($WIN_BUSMASTER, "", 5) Then
			Sleep(1000)
			ControlClick($WIN_BUSMASTER, "", 2)
			Sleep(1000)
			_WriteResult("Pass","TS_UserCfx_13")
        Else
			_WriteResult("Fail","TS_UserCfx_13")
		EndIf

	EndIf
     Sleep(3000)
$isAppNotRes=_CloseApp()																				; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_UserCfx_13")
EndIf

  ConsoleWrite("****End : TS_UserCfx_13.au3****"&@CRLF)
  ConsoleWrite(@CRLF)


