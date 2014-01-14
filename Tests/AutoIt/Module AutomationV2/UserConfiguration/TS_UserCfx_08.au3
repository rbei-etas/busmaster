
;==================================================Test Cases/Test Data =============================================
; Critical (C)		:		Y
; TestCase No.		:		TS_UserCfx_07
; TestCases			:		Utility menu details Validation
; Test Data			:		cfx_UserCfx_08.cfx
; Test Setup		:		1. Open the Busmaster.
;                           2. Check the menu items of  File->Format Converters.
; Expected Results  : 		1. The utility menu will contain the following items.
;                           A. CAPL to CPP converter
;                           B. Log To Excel File converter.
;                           D. Other Converters
;==================================================Test Procedure =======================================================
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_UserCfx_08.au3****"&@CRLF)
    _launchApp()
	WinActivate($WIN_BUSMASTER)
	if winexists($WIN_BUSMASTER) Then
		_loadConfig("cfx_UserCfx_08.cfx")
		WinMenuSelectItem($WIN_BUSMASTER,"",$fileMenu,$FomatConverterMenu)                           ;open format converters from menu File->Format Converters
	$hWndCAPL2CPP=controlgethandle($WIN_FormatConv,"",$Tab_CAPL2CPP)                                 ;Get handle of the tab control
		_GUICtrlTab_ClickTab($hWndCAPL2CPP, 0)                                                       ;Click on CAPL2CPP tab
		Sleep(1000)
		$Tabtext1=_GUICtrlTab_GetItemText($hWndCAPL2CPP,0)                                            ;Get text of the tab1
		ConsoleWrite("text1" & $Tabtext1 & @CRLF)
		sleep(1000)
		_GUICtrlTab_ClickTab($hWndCAPL2CPP, 1)
		$Tabtext2=_GUICtrlTab_GetItemText($hWndCAPL2CPP,1)                                           ;Get text of the tab1
	    ConsoleWrite("text2" & $Tabtext2 & @CRLF)
		sleep(1000)
		_GUICtrlTab_ClickTab($hWndCAPL2CPP, 2)
         $Tabtext3=_GUICtrlTab_GetItemText($hWndCAPL2CPP,2)                                         ;Get text of the tab1
		  ConsoleWrite("text3" & $Tabtext3 & @CRLF)

		If($Tabtext1="CAPL_2_CPP Converter" And $Tabtext2="Log to Excel File Converter" And $Tabtext3="Other Converters ...") Then
			_WriteResult("Pass","TS_UserCfx_08")
		Else
			_WriteResult("Fail","TS_UserCfx_08")
		EndIf
		WinClose($WIN_FormatConv)
    EndIf


	$isAppNotRes=_CloseApp()																			; Close the app

	if $isAppNotRes=1 Then
         _WriteResult("Warning","TS_UserCfx_08")

	   EndIf

