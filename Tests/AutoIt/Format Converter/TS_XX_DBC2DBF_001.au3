; === Description ===
; This test checks DBC2DBF conversion.

; Execute Format Converter
Run(@ProgramFilesDir & "\BUSMASTER\FormatConverter.exe")
if @error Then
	ConsoleWriteError("Format Converter didn't run" & @CRLF)
	Exit
EndIf

; BUSMASTER Format Conversions
WinWaitActive("BUSMASTER Format Conversions", "", 15)
if @error Then
	ConsoleWriteError("Format Conversions didn't activate" & @CRLF)
	Exit
EndIf

; Select the Conversion Type
Send("{DOWN}") ; DBC TO DBF Conversion

; Input File
Send("{TAB}{TAB}{ENTER}")
WinWaitActive("", "CANoe Database File(s) (*.dbc)", 3)
Send(@ScriptDir & "\TS_XX_DBC2DBF_001.dbc{ENTER}")
WinWaitClose("", "CANoe Database File(s) (*.dbc)", 3)

; Output File
Send("{TAB}{TAB}{ENTER}")
WinWaitActive("", "BUSMASTER Database File(s) (*.dbf)", 3)
Send(@ScriptDir & "\out\TS_XX_DBC2DBF_001.dbf{ENTER}")
WinWaitClose("", "BUSMASTER Database File(s) (*.dbf)", 3)

; Convert
Send("{TAB}{ENTER}")

; Result
Send("{TAB}")
Send("{CTRLDOWN}c{CTRLUP}") ; copy
$ResultStr = ClipGet();
ConsoleWrite($ResultStr & @CRLF)

; Close Window
Send("{TAB}{ENTER}")
WinWaitClose("BUSMASTER Format Conversions", "", 3)
if @error Then
	ConsoleWriteError("Format Conversions didn't close" & @CRLF)
	Exit
EndIf

; There should be no crash window
Sleep(3000)
if (WinActive("", "debug")) Then
	ConsoleWriteError("Format Conversions crashed after close" & @CRLF)
	Send("{ENTER}")
EndIf

; Compare generated/expected log file
$Ret = RunWait(@ComSpec & " /c FC /B out\TS_XX_DBC2DBF_001.log TS_XX_DBC2DBF_001_expected.log", @ScriptDir, @SW_HIDE)
If $Ret Then
	ConsoleWriteError("Unexpected log file" & @CRLF)
EndIf

; Compare generated/expected dbf file
$Ret = RunWait(@ComSpec & " /c FC /B out\TS_XX_DBC2DBF_001.dbf TS_XX_DBC2DBF_001_expected.dbf", @ScriptDir, @SW_HIDE)
If $Ret Then
	ConsoleWriteError("Unexpected dbf file" & @CRLF)
EndIf
