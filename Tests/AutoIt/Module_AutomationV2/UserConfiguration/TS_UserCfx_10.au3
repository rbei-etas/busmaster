;==================================================Test Cases/Test Data =============================================
; Critical (C)		:		Y
; TestCase No.		:		TS_UserCfx_010
; TestCases			:
;                           1. Select "File -> Configuration -> Save" menu option."
;                           2.Select "File -> Configuration -> Save As..." menu option
;                           3. Enter the file name specified in the Save As dialog box

; Expected Results  : 		1.Should succesfully save into the configuration file.
;                           2. This configuration file should be listed in the top of the MRU list"
;                           After step2,Should succesfully save into the selected configuration file.
;                           This should be the loaded configuration file
;                           This configuration file should be listed in the top of the MRU list"
;
;==================================================Test Procedure =======================================================
#RequireAdmin
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_UserCfx_10.au3****"&@CRLF)
Local $path=_TestDataPath()
_launchApp()
WinActivate($WIN_BUSMASTER)
$DirPath=_TestDataPath()
if winexists($WIN_BUSMASTER) Then
		_loadConfig("cfx_UserCfx_10.cfx")
		WinMenuSelectItem($WIN_BUSMASTER,"","&File","Save &As")
		Sleep(1000)
		if winexists($WIN_SaveAsCfx) Then
		ControlSend($WIN_SaveAsCfx,"",$TXT_FileName_SaveAsCfx,$DirPath&"\"&"cfx_UserCfx_10_SaveAs.cfx")	                       ; Set the filename in 'Load Configuration Filename...' dialog
		sleep(1000)
		ControlClick($WIN_SaveAsCfx,"",$BTN_Save_SaveAsCfx)			                                                           ; Click on save button
		sleep(2500)
		If WinWait($WIN_ConfirmSaveAs,"",5) Then
			ControlClick($WIN_ConfirmSaveAs,"",$BTN_Yes_ConfirmSaveAs)
		EndIf
		Sleep(1000)

		$isAppNotRes=_CloseApp()
		Sleep(1000)


		Run("regedit")                                                                                                        ;Go to Registry editor
        Sleep(500)
		Send("{HOME}")
		sleep(100)
		$handle=ControlGetHandle($WIN_RegEdit,"",$TVC_RegEdit)                                                                  ;Get handle for Tree view in Registry
        ConsoleWrite("handle" & $handle & @CRLF)
		;_GUICtrlTreeView_Expand($handle,"",False)                                                                               ;Expand tree view till MRU lsit in Registry editor
        Sleep(1000)
		;_GUICtrlTreeView_Expand($handle,"",True)
		;$a=_GUICtrlTreeView_FindItem($handle, "HKEY_CLASSES_ROOT")
		; ConsoleWrite("$a=" & $a & @CRLF)

		Sleep(1000)
		Send("{DOWN 2}")
		;Send("{RIGHT}")
		Send("{RIGHT}")
		Sleep(1000)

        Sleep(1000)
        ;Send("{DOWN 2}")
		Send("{DOWN 11}")
		Send("{RIGHT}")
        Sleep(1000)
		;Send("{DOWN 10}")
        Send("{DOWN 30}")
		Send("{RIGHT}")
        Sleep(1000)
		 Send("{DOWN 5}")
		Send("{RIGHT}")
        ;Send("{DOWN 31}")
        Sleep(1000)
        ;Send("{DOWN}")
        Sleep(1000)
        Send("{DOWN 3}")
        Sleep(1000)



        $handle1=ControlGetHandle($WIN_RegEdit,"",$LST_CfxFiles_RegEdit)                                                         ;Get handle of cfx files list in MRU list
        Sleep(500)
        ;$count=_GUICtrlListView_GetItemCount($handle1)                                                                            ;Get total count of item list in MRU list
       ; ConsoleWrite("Count" & $count & @CRLF)
		$text=_GUICtrlListView_GetItemTextString($handle1,1)                                                                      ;Get text of the Third item in the MRU list
        ConsoleWrite("text" & $text & @CRLF)
        $splitText=StringSplit($text,"|")                                                                                         ;Split the string by delimiter "|"
        ConsoleWrite("cfx: " & $splitText[3] & @CRLF)
        If($splitText[3]=$path&"\cfx_UserCfx_10_SaveAs.cfx") Then
			_WriteResult("Pass","TS_UserCfx_10")
        Else
			_WriteResult("Fail","TS_UserCfx_10")
		EndIf


		;-----------------------------closing regeditor tree list view---------
		Sleep(1000)
        ;Send("{DOWN 2}")
		;Send("{DOWN 11}")
		Send("{LEFT}")
		Send("{LEFT}")

		Send("{LEFT}")
		Send("{LEFT}")

		Send("{LEFT}")
		Send("{LEFT}")

		Send("{LEFT}")
		Send("{LEFT}")

		Send("{LEFT}")
		Send("{LEFT}")

		;-----------------------------closing regeditor tree list view---------

		WinClose($WIN_RegEdit)
EndIf

$isAppNotRes=_CloseApp()																				; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_UserCfx_10")
EndIf
	EndIf




