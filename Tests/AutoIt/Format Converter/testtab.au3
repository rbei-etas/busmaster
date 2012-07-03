; === Description ===
; This test cases checks the all modules of format converters
; and generate the test coverage for autoit testcases for FOrmatconveter modules.
#Include <GuiTab.au3>
#Include <GuiListBox.au3>

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


$hWnd=controlgethandle("BUSMASTER Format Conversions","","[CLASS:SysTabControl32; INSTANCE:1]")

;; now test the last tab
_GUICtrlTab_ClickTab($hWnd, 2)

controlclick("BUSMASTER Format Conversions","",1010)
sleep(1000)
Send(@ScriptDir & "\testcocoon_testdata\BUSMASTERLogFileInput.log{ENTER}")
sleep(1000)
;ControlSetText ("Select Log File","","[CLASS:Edit; INSTANCE:1]",@ScriptDir & "\testcocoon_testdata\BUSMASTERLogFileInput.log{ENTER}")
controlclick("BUSMASTER Format Conversions","",1011)
Send(@ScriptDir & "\testcocoon_testdata\BUSMASTER_csvfileoutput.xls{ENTER}")
Send("{TAB}")
Send("{DOWN}")
sleep(1000)
controlclick("BUSMASTER Format Conversions","",1006)
sleep(1000)
controlclick("BUSMASTER Format Conversions","",1012)
;Send("{ENTER}")
sleep(1000)
controlclick("BUSMASTER","","[CLASS:Button; INSTANCE:1]")
controlclick("BUSMASTER Format Conversions","",2)

