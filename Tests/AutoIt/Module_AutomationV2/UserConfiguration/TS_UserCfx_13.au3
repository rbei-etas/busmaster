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

;$CopycfxFile=_OutputDataPath()																		; Fetch the Log file path
;$DirPath=_TestDataPath()
;ConsoleWrite("$CopycfxFile"&$CopycfxFile&@CRLF)
FileCopy(_TestDataPath()&"\cfx_UserCfx_13.cfx", _OutputDataPath()&"\")

    _launchApp()

	$res=0
	$fileNotFoundPopUp =0

	WinActivate($WIN_BUSMASTER)
	$DirPath=_TestDataPath()
	ConsoleWrite("$DirPath:"&$DirPath & @CRLF)
	;$FilePath= @ScriptDir&"\TestData\cfx_UserCfx_13.cfx"
	$FilePathcfx= $DirPath&"\cfx_UserCfx_13.cfx"
	ConsoleWrite("$FilePath:"&$FilePath & @CRLF)
	if winexists($WIN_BUSMASTER) Then
		_createConfigTest("cfx_UserCfx_13.cfx")
		Sleep(1000)
		$isAppNotRes=_CloseApp()																		 ; Close the app
        if $isAppNotRes=1 Then
			ConsoleWrite("Application not responding!! Error")
	    EndIf
		;$result=FileDelete($FilePath)                                                                    ;Delete cfx file from the specified location
		$result=FileDelete($FilePathcfx)                                                                    ;Delete cfx file from the specified location
		Sleep(1000)
		ConsoleWrite("$result: " & $result & @CRLF)
	    Run($BusMasterExeFPath & "\BUSMASTER.exe")
        ProcessWait($AppProcess)															             ; Pauses script execution until a given process exists.
		WinWaitActive($WIN_BUSMASTER,"",5)
        Sleep(1000)
		If WinWait($WIN_BUSMASTER, "", 5) Then
			Sleep(1000)

			autoitsetoption("WinTextMatchMode",2)
			;$Text=WinGetText($WIN_BUSMASTER,$FilePathcfx&" was not found.")								; get  the text from window
			$Text=WinGetText($WIN_BUSMASTER,"The Configuration file is not Valid")								; get  the text from window

			ConsoleWrite("$Text: " & $Text & @CRLF)
			;$res=StringInStr($Text,$FilePathcfx&" was not found.")										; Check error message poped up saying the file was not found
			$res=StringInStr($Text,"The Configuration file is not Valid")										; Check error message poped up saying the file was not found
			ConsoleWrite("$res: " & $res & @CRLF)
			if $res>0 Then
				ControlClick($WIN_BUSMASTER, "", 2)
				Sleep(1000)
				$fileNotFoundPopUp = 1
				;_WriteResult("Pass","TS_UserCfx_13")
			endif

			;ControlClick($WIN_BUSMASTER, "", 2)

        Else
			$fileNotFoundPopUp =0
			;_WriteResult("Fail","TS_UserCfx_13")
		EndIf

	EndIf
     Sleep(3000)

	If($result=1 And $fileNotFoundPopUp=1) Then
		_WriteResult("Pass","TS_UserCfx_13")
	Else
		_WriteResult("Fail","TS_UserCfx_13")
	EndIf

$isAppNotRes=_CloseApp()																				; Close the app


if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_UserCfx_13")
EndIf

FileCopy(_OutputDataPath()&"\cfx_UserCfx_13.cfx", _TestDataPath()&"\")

  ConsoleWrite("****End : TS_UserCfx_13.au3****"&@CRLF)
  ConsoleWrite(@CRLF)


