;==================================================Test Cases/Test Data =============================================
; Critical (C)		:		Y
; TestCase No.		:		TS_UserCfx_011
; TestCases			:		Selection of configuration file from MRU list
; Test Setup		:		 Select "File -> Recent Configuration -> NewConfig.cfx" menu option
;                             or Load existing configuration

; Expected Results  : 		 The configuration file "NewConfig.cfx" should be loaded successfully. It means that configuration file should be read successfully.
;                            2. This configuration file should be listed in the top of the MRU list"
;
;==================================================Test Procedure =======================================================
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_UserCfx_11.au3****"&@CRLF)
Local $path=_TestDatapath()
    _launchApp()
	WinActivate($WIN_BUSMASTER)
	if winexists($WIN_BUSMASTER) Then
		_loadConfig("cfx_UserCfx_11.cfx")
	    Run("regedit")                                                                                                               ;Go to Registry editor
        Sleep(500)
		$handle=ControlGetHandle($WIN_RegEdit,"",$TVC_RegEdit)                                                                      ;Get handle for Tree view in Registry
        ConsoleWrite("handle" & $handle & @CRLF)
		_GUICtrlTreeView_Expand($handle,"",False)                                                                                   ;Expand tree view till MRU lsit in Registry editor
        Sleep(1000)
		_GUICtrlTreeView_Expand($handle,"",True)
        Sleep(1000)
        Send("{DOWN 2}")
        Sleep(1000)
        Send("{DOWN 10}")
        Sleep(1000)
        Send("{DOWN 31}")
        Sleep(1000)
        Send("{DOWN}")
        Sleep(1000)
        Send("{DOWN 3}")
        Sleep(1000)
        $handle1=ControlGetHandle($WIN_RegEdit,"",$LST_CfxFiles_RegEdit)                                                         ;Get handle of cfx files list in MRU list
        Sleep(500)
        $count=_GUICtrlListView_GetItemCount($handle1)                                                                            ;Get total count of item list in MRU list
        ConsoleWrite("Count" & $count & @CRLF)
		$text=_GUICtrlListView_GetItemTextString($handle1,1)                                                                      ;Get text of the second item in the MRU list
        ConsoleWrite("text" & $text & @CRLF)
        $splitText=StringSplit($text,"|")                                                                                         ;Split the string by delimiter "|"
        ConsoleWrite("cfx: " & $splitText[3] & @CRLF)
        If($count=6 And $splitText[3]=$path&"\cfx_UserCfx_11.cfx") Then
			_WriteResult("Pass","TS_UserCfx_11")
        Else
			_WriteResult("Fail","TS_UserCfx_11")
		EndIf
		WinClose($WIN_RegEdit)
	EndIf

$isAppNotRes=_CloseApp()																				; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_UserCfx_11")
EndIf

  ConsoleWrite("****End : TS_UserCfx_11.au3****"&@CRLF)
  ConsoleWrite(@CRLF)





