;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_FormatConv_023
; TestCases			:		Invoking Busmaster from Installation location
; Test Data			:		cfxFormatConv_01.cfx
; Test Setup		:		1. Goto  Busmaster is installation directory. C:\Program Files\BUSMASTER.
;                           2.Double Click on FormatConverter.exe.

; Expected Results  : 		Busmaster Format conversion dialog gets opened.
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_FormatConv_023****"&@CRLF)
    Run($BusMasterExeFPath & "\FormatConverter.exe")                                                     ;Run FormatConverter.exe app from Busmaster installation location
     if @error Then
		ConsoleWriteError("Format Converter didn't run" & @CRLF)
	Exit
    EndIf
	WinWaitActive($WIN_FormatConv, "", 5)                                                                     ;Wait for Format converter window
    if @error Then
		ConsoleWriteError("Format Conversions didn't activate" & @CRLF)
        Exit
	EndIf
	If WinExists($WIN_FormatConv) Then
	     _WriteResult("Pass","STS_FORMAT_CONV_023")
	Else
		_WriteResult("Fail","STS_FORMAT_CONV_023")
	EndIf
    WinClose($WIN_FormatConv)

ConsoleWrite("****End : TS_FormatConv_023****"&@CRLF)
