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


; test case for capl to c conversion
; test case 1
if winexists("BUSMASTER Format Conversions") then
	sleep(2000)
$hWnd=controlgethandle("BUSMASTER Format Conversions","","[CLASS:SysTabControl32; INSTANCE:1]")


_GUICtrlTab_ClickTab($hWnd, 1)

; Input File
controlclick("BUSMASTER Format Conversions","",1011)
;Send("{TAB}{TAB}{ENTER}")
WinWaitActive("Select CAN File ", "", 2)
Send(@ScriptDir & "\testcocoon_testdata\can_db_mqb.can{ENTER}")
WinWaitClose("Select CAN File", "", 2)

; Output C  File
;Send("{TAB}{TAB}{ENTER}")
controlclick("BUSMASTER Format Conversions","",1012)
WinWaitActive("Select BUSMASTER File ", "", 2)
Send(@ScriptDir & "\out\busmaster1q.c{ENTER}")
WinWaitClose("Select BUSMASTER File", "", 2)
endif

controlclick("BUSMASTER Format Conversions","",1018)
controlclick("BUSMASTER Format Conversions","",1004)
WinWaitActive("Select CANoe Database File", "", 2)
Send(@ScriptDir & "\testcocoon_testdata\can_db_mqb.dbc{ENTER}")

controlclick("BUSMASTER Format Conversions","","[CLASS:Button; INSTANCE:9]")
controlclick("BUSMASTER Format Conversions","","[CLASS:Button; INSTANCE:8]")
sleep(1000)
Send(@ScriptDir & "\out\busmasteroutp.dbf{ENTER}")
;click on convert
controlclick("BUSMASTER Format Conversions","",1013)


$text = controlgetText("BUSMASTER Format Conversions","",1015)

;click on LOG
controlclick("BUSMASTER Format Conversions","",1008)
sleep(1000)

WinClose("busmaster1qlog.txt - Notepad")

; Direct convert without database
;Send("{TAB}{ENTER}")
;controlclick("Assign Database","",1004)
;WinWaitActive("Select CANoe Database File ", "", 2)
;Send(@DesktopDir & "\can_db_mqb.dbc{ENTER}")


; test case 2 for capl to c comversion

_GUICtrlTab_ClickTab($hWnd, 1)

; Input File
controlclick("BUSMASTER Format Conversions","",1011)
;Send("{TAB}{TAB}{ENTER}")
WinWaitActive("Select CAN File ", "", 2)
Send(@ScriptDir & "\testcocoon_testdata\cancapl_AEE2010_H91_v1p1_pf.CAN{ENTER}")
WinWaitClose("Select CAN File", "", 2)

; Output C  File
;Send("{TAB}{TAB}{ENTER}")
controlclick("BUSMASTER Format Conversions","",1012)
WinWaitActive("Select BUSMASTER File ", "", 2)
Send(@ScriptDir & "\out\busmaster_cfile.c{ENTER}")
WinWaitClose("Select BUSMASTER File", "", 2)


;controlclick("BUSMASTER Format Conversions","",1018)
controlclick("BUSMASTER Format Conversions","",1004)
WinWaitActive("Select CANoe Database File", "", 2)
Send(@ScriptDir & "\testcocoon_testdata\CanDB_AEE2010_H91_v1p1_pf.dbc{ENTER}")

controlclick("BUSMASTER Format Conversions","","[CLASS:Button; INSTANCE:8]")
sleep(1000)
Send(@ScriptDir & "\out\busmasteroutp22.dbf{ENTER}")
;click on convert
controlclick("BUSMASTER Format Conversions","",1013)

Send("{ESC}")
sleep(1000)
send("{ENTER}")
sleep(1000)
$text = controlgetText("BUSMASTER Format Conversions","",1015)

;end





; test case 3 for capl to c comversion

_GUICtrlTab_ClickTab($hWnd, 1)

; Input File
controlclick("BUSMASTER Format Conversions","",1011)
;Send("{TAB}{TAB}{ENTER}")
WinWaitActive("Select CAN File ", "", 2)
Send(@ScriptDir & "\testcocoon_testdata\cancapl_AEE2010_H91_v1p1_pf.CAN{ENTER}")
WinWaitClose("Select CAN File", "", 2)

; Output C  File
;Send("{TAB}{TAB}{ENTER}")
controlclick("BUSMASTER Format Conversions","",1012)
WinWaitActive("Select BUSMASTER File ", "", 2)
Send(@ScriptDir & "\out\busmaster_cfile.c{ENTER}")
WinWaitClose("Select BUSMASTER File", "", 2)

;click on convert without any dbc file
controlclick("BUSMASTER Format Conversions","",1018)
controlclick("BUSMASTER Format Conversions","",1018)

controlclick("BUSMASTER Format Conversions","",1013)

sleep(500)

WinClose("Warning");
sleep(500)
;end



; test case 4for capl to c comversion

_GUICtrlTab_ClickTab($hWnd, 1)

; Input File
controlclick("BUSMASTER Format Conversions","",1011)
;Send("{TAB}{TAB}{ENTER}")
WinWaitActive("Select CAN File ", "", 2)
Send(@ScriptDir & "\testcocoon_testdata\cancapl_AEE2010_H91_v1p1_pf.CAN{ENTER}")
WinWaitClose("Select CAN File", "", 2)

; Output C  File
;Send("{TAB}{TAB}{ENTER}")
controlclick("BUSMASTER Format Conversions","",1012)
WinWaitActive("Select BUSMASTER File ", "", 2)
Send(@ScriptDir & "\out\busmastter_ecfile.c{ENTER}")
WinWaitClose("Select BUSMASTER File", "", 2)

;click on convert without any dbc file
controlclick("BUSMASTER Format Conversions","",1018)

sleep(6000)
controlclick("BUSMASTER Format Conversions","",1013)

WinWaitActive("Key Mapping", "", 2)
controlclick("Assign Database","",1004)

WinWaitActive("Assign Database", "", 2)
controlclick("Assign Database","",1004)
Send(@ScriptDir & "\testcocoon_testdata\CanDB_AEE2010_H91_v1p1_pf.dbc{ENTER}")

controlclick("BUSMASTER Format Conversions","","[CLASS:Button; INSTANCE:1]")
sleep(1000)

sleep(500)

WinClose("Warning");
sleep(500)
;end




;; now test the last tab LOG - to - CVS conversion
_GUICtrlTab_ClickTab($hWnd, 2)

; test case 1 for can log to 
controlclick("BUSMASTER Format Conversions","",1010)
sleep(1000)
Send(@ScriptDir & "\testcocoon_testdata\BUSMASTERLogFileInput.log{ENTER}")
sleep(1000)
;ControlSetText ("Select Log File","","[CLASS:Edit; INSTANCE:1]",@ScriptDir & "\testcocoon_testdata\BUSMASTERLogFileInput.log{ENTER}")
controlclick("BUSMASTER Format Conversions","",1011)
Send(@ScriptDir & "\out\Bt.xls{ENTER}")
Send("{TAB}")
Send("{DOWN}")
sleep(1000)
controlclick("BUSMASTER Format Conversions","",1006)
sleep(1000)
controlclick("BUSMASTER Format Conversions","",1012)
;Send("{ENTER}")
sleep(1000)
controlclick("BUSMASTER","","[CLASS:Button; INSTANCE:1]")


;test case2 for J1939 log to CVS conversion


;Send("{DOWN}")
ControlCommand ("BUSMASTER Format Conversions","","[CLASS:ComboBox; INSTANCE:1]","SelectString","J1939")
sleep(500)
controlclick("BUSMASTER Format Conversions","",1010)
sleep(1000)
Send(@ScriptDir & "\testcocoon_testdata\BUSMASTERLogFileInput.log{ENTER}")
sleep(1000)
;ControlSetText ("Select Log File","","[CLASS:Edit; INSTANCE:1]",@ScriptDir & "\testcocoon_testdata\BUSMASTERLogFileInput.log{ENTER}")
controlclick("BUSMASTER Format Conversions","",1011)
Send(@ScriptDir & "\out\BUSMASTER_csvJ1939fileoutput.xls{ENTER}")
Send("{TAB}")
Send("{DOWN}")
sleep(1000)
controlclick("BUSMASTER Format Conversions","",1006)
sleep(1000)
controlclick("BUSMASTER Format Conversions","",1012)
;Send("{ENTER}")
sleep(1000)
controlclick("BUSMASTER","","[CLASS:Button; INSTANCE:1]")


;close the dialog
controlclick("BUSMASTER Format Conversions","",2)

