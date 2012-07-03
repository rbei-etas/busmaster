; === Description ===
; This test cases checks the all modules of format converters
; and generate the test coverage for autoit testcases for FOrmatconveter modules.
#Include <GuiTab.au3>

$TestName=StringTrimRight(@ScriptName, 4)

; Execute Format ConverterS\Tests\AutoIt\Format Converter\testcocoon_testdata\BUSMASTERLogFile00.
;Run(@ProgramFilesDir & "\BUSMASTER\FormatConverter.exe")
Run("D:\BUSMASTER OSS\Sources\Format Converter\bin\Release\FormatConverter.exe")
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

; TEST CASE FOR ASC-TO-LOG
;select the conversion ASC to Log ( default one )

; Input File
Send("{TAB}{TAB}{ENTER}")
WinWaitActive("", "CANoe Log File(s) (*.asc)", 3)
Send(@ScriptDir & "\testcocoon_testdata\BUSMASTERLogFileInput.asc{ENTER}")
WinWaitClose("", "CANoe Log File(s) (*.asc)", 3)

; Output File
Send("{TAB}{TAB}{ENTER}")
WinWaitActive("Open", "BUSMASTER Log File(s) (*.log)", 3)
Send(@ScriptDir & "\out\BUSMASTERLogFileOutput.log{ENTER}")
WinWaitClose("", "BUSMASTER Log File(s) (*.log)", 3)

; Convert
Send("{TAB}{ENTER}")

; Result
Send("{TAB}")
Send("{CTRLDOWN}c{CTRLUP}") ; copy
$ResultStr = ClipGet();
ConsoleWrite($ResultStr & @CRLF)



; TEST CASE FOR DBC-TO-DBF
;select the conversion dbc to dbf 

;get the focus back to the combo box list
Send("{TAB}{TAB}{TAB}")

; Select the Conversion Type
Send("{DOWN}") ; DBC TO DBF Conversion

; Input File
Send("{TAB}{TAB}{ENTER}")
WinWaitActive("", "CANoe Database File(s) (*.dbc)", 3)
Send(@ScriptDir & "\dbc2dbf_messages.dbc{ENTER}")
WinWaitClose("", "CANoe Database File(s) (*.dbc)", 3)

; Output File
Send("{TAB}{TAB}{ENTER}")
WinWaitActive("", "BUSMASTER Database File(s) (*.dbf)", 3)
Send(@ScriptDir & "\out\outputsample1_dbf.dbf{ENTER}")
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
If @error Then
	ConsoleWriteError("Format Conversions didn't close" & @CRLF)
	Exit
EndIf

