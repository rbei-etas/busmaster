; === Description ===
; steps to modify this script
; input proper path for instrumented Execute
;clear files from "OUT" folder
;keep input files in "testcocoon_testdata" folder.
;run the script
; This test cases checks the all modules of format converters
; and generate the test coverage for autoit testcases for FOrmatconveter modules.
#Include <GuiTab.au3>
#Include <GuiListBox.au3>
#Include <GuiListView.au3>

$TestName=StringTrimRight(@ScriptName, 4)

; Execute Format ConverterS\Tests\AutoIt\Format Converter\testcocoon_testdata\BUSMASTERLogFile00.
;Run(@ProgramFilesDir & "\BUSMASTER\FormatConverter.exe")
Run("C:\Program Files\BUSMASTER\FormatConverter.exe")
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

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; TEST CASE FOR ASC-TO-LOG
;select the conversion ASC to Log ( default one )

; Input File
WinActivate("BUSMASTER Format Conversions")
$hWnd=controlgethandle("BUSMASTER Format Conversions","","[CLASS:SysTabControl32; INSTANCE:1]")
_GUICtrlTab_ClickTab($hWnd, 2)																		; Click on the "Other Converters" tab
ControlClick("BUSMASTER Format Conversions","",1006)   ; Click on "Input" button
WinWaitActive("", "CANoe Log File(s) (*.asc)", 3)
Send(@ScriptDir & "\testcocoon_testdata\BUSMASTERLogFileInput.asc{ENTER}")
ControlClick("BUSMASTER Format Conversions","",1007)   ; Click on "Output" button
WinWaitActive("Open", "BUSMASTER Log File(s) (*.log)", 3)
Send(@ScriptDir & "\out\BUSMASTERLogFileOutput.log{ENTER}")
ControlClick("BUSMASTER Format Conversions","",1008)   ; Click on "Convert" button

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; TEST CASE FOR DBC-TO-DBF
;select the conversion dbc to dbf

ControlCommand("BUSMASTER Format Conversions","",1003,"SelectString","DBC to DBF Conversion")		; Select 'DBC to DBF Conversion' from combo box
; Input File
ControlClick("BUSMASTER Format Conversions","",1006)   ; Click on "Input" button
WinWaitActive("", "CANoe Database File(s) (*.dbc)", 3)
Send(@ScriptDir & "\dbc2dbf_messages.dbc{ENTER}")
ControlClick("BUSMASTER Format Conversions","",1007)   ; Click on "Output" button
WinWaitActive("", "BUSMASTER Database File(s) (*.dbf)", 3)
Send(@ScriptDir & "\out\outputsample1_dbf.dbf{ENTER}")
ControlClick("BUSMASTER Format Conversions","",1008)   ; Click on "Convert" button


;testcase 2
; TEST CASE FOR DBC-TO-DBF
;select the conversion dbc to dbf

;get the focus back to the combo box list
Send("{TAB}{TAB}{TAB}")

; Select the Conversion Type
;Send("{DOWN}") ; DBC TO DBF Conversion

; Input File
ControlClick("BUSMASTER Format Conversions","",1006)   ; Click on "Input" button
WinWaitActive("", "CANoe Database File(s) (*.dbc)", 3)
Send(@ScriptDir & "\dbc2dbf_signals.dbc{ENTER}")
ControlClick("BUSMASTER Format Conversions","",1007)   ; Click on "Output" button
WinWaitActive("", "BUSMASTER Database File(s) (*.dbf)", 3)
Send(@ScriptDir & "\out\outputsample1_signals_dbf.dbf{ENTER}")
ControlClick("BUSMASTER Format Conversions","",1008)   ; Click on "Convert" button


;testcase 3
; TEST CASE FOR DBC-TO-DBF
;select the conversion dbc to dbf

ControlClick("BUSMASTER Format Conversions","",1006)   ; Click on "Input" button
WinWaitActive("", "CANoe Database File(s) (*.dbc)", 3)
Send(@ScriptDir & "\testcocoon_testdata\all_in_one.dbc{ENTER}")
ControlClick("BUSMASTER Format Conversions","",1007)   ; Click on "Output" button
WinWaitActive("", "BUSMASTER Database File(s) (*.dbf)", 3)
Send(@ScriptDir & "\out\outputsample1_signals_dbf.dbf{ENTER}")
ControlClick("BUSMASTER Format Conversions","",1008)   ; Click on "Convert" button


;testcase 4
; TEST CASE FOR DBC-TO-DBF
;select the conversion dbc to dbf ;;;huge file

ControlClick("BUSMASTER Format Conversions","",1006)   ; Click on "Input" button
WinWaitActive("", "CANoe Database File(s) (*.dbc)", 3)
Send(@ScriptDir & "\testcocoon_testdata\can_db_v6x.dbc{ENTER}")
ControlClick("BUSMASTER Format Conversions","",1007)   ; Click on "Output" button
WinWaitActive("", "BUSMASTER Database File(s) (*.dbf)", 4)
Send(@ScriptDir & "\out\BIGdbf.dbf{ENTER}")
ControlClick("BUSMASTER Format Conversions","",1008)   ; Click on "Convert" button


;testcase 5
; TEST CASE FOR DBC-TO-DBF
;select the conversion dbc to dbf ;;;huge file

ControlClick("BUSMASTER Format Conversions","",1006)   ; Click on "Input" button
WinWaitActive("", "CANoe Database File(s) (*.dbc)", 3)
Send(@ScriptDir & "\testcocoon_testdata\vw360e_acan.dbc{ENTER}")
ControlClick("BUSMASTER Format Conversions","",1007)   ; Click on "Output" button
WinWaitActive("", "BUSMASTER Database File(s) (*.dbf)", 3)
Send(@ScriptDir & "\out\BIGdbf1.dbf{ENTER}")
ControlClick("BUSMASTER Format Conversions","",1008)   ; Click on "Convert" button


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; TEST CASE FOR DBF-TO-DBc
;select the conversion dbf to dbc
;testcase 1
ControlCommand("BUSMASTER Format Conversions","",1003,"SelectString","DBF to DBC Conversion")		; Select 'DBF to DBC Conversion' from combo box

ControlClick("BUSMASTER Format Conversions","",1006)   ; Click on "Input" button
WinWaitActive("Open", "", 3)
Send(@ScriptDir & "\dbc2dbf_messages.dbf{ENTER}")
ControlClick("BUSMASTER Format Conversions","",1007)   ; Click on "Output" button
WinWaitActive("Open", "CANoe Database File(s) (*.dbc)", 3)
Send(@ScriptDir & "\out\output_dbc_sample1.dbc{ENTER}")
ControlClick("BUSMASTER Format Conversions","",1008)   ; Click on "Convert" button



;testcase 2

ControlClick("BUSMASTER Format Conversions","",1006)   ; Click on "Input" button
WinWaitActive("Open", "", 3)
Send(@ScriptDir & "\dbf2dbc_signals.dbf{ENTER}")
ControlClick("BUSMASTER Format Conversions","",1007)   ; Click on "Output" button
WinWaitActive("Open", "CANoe Database File(s) (*.dbc)", 3)
Send(@ScriptDir & "\out\output_dbc_11sample1.dbc{ENTER}")
ControlClick("BUSMASTER Format Conversions","",1008)   ; Click on "Convert" button



;testcase 3

ControlClick("BUSMASTER Format Conversions","",1006)   ; Click on "Input" button
WinWaitActive("Open", "", 3)
Send(@ScriptDir & "\dbc2dbf_signals.dbf{ENTER}")
ControlClick("BUSMASTER Format Conversions","",1007)   ; Click on "Output" button
WinWaitActive("Open", "CANoe Database File(s) (*.dbc)", 3)
Send(@ScriptDir & "\out\output_dbcf_11sample1.dbc{ENTER}")
ControlClick("BUSMASTER Format Conversions","",1008)   ; Click on "Convert" button



;testcase 4

ControlClick("BUSMASTER Format Conversions","",1006)   ; Click on "Input" button
WinWaitActive("Open", "", 3)
Sleep(1000)
Send(@ScriptDir & "\testcocoon_testdata\all_in_one.dbf{ENTER}")
sleep(500)
ControlClick("BUSMASTER Format Conversions","",1007)   ; Click on "Output" button
WinWaitActive("Open", "CANoe Database File(s) (*.dbc)", 3)
Send(@ScriptDir & "\out\outpu8t_dbcf_11sample1.dbc{ENTER}")
ControlClick("BUSMASTER Format Conversions","",1008)   ; Click on "Convert" button



;testcase 5
;test case of invalid input file
ControlClick("BUSMASTER Format Conversions","",1006)   ; Click on "Input" button
WinWaitActive("Open", "", 3)
Sleep(1000)
Send(@ScriptDir & "\testcocoon_testdata\Copy of CanDB_AEE2010_H91_v1p1_pf.dbf{ENTER}")
sleep(500)
ControlClick("BUSMASTER Format Conversions","",1007)   ; Click on "Output" button
WinWaitActive("Open", "CANoe Database File(s) (*.dbc)", 3)
Send(@ScriptDir & "\out\outpu8t_dbcf_11sample1.dbc{ENTER}")
ControlClick("BUSMASTER Format Conversions","",1008)   ; Click on "Convert" button




;testcase 6
;test case of big dbf file with all constructs
ControlClick("BUSMASTER Format Conversions","",1006)   ; Click on "Input" button
WinWaitActive("Open", "", 3)
Sleep(1000)
Send(@ScriptDir & "\testcocoon_testdata\BigDBF.dbf{ENTER}")
sleep(500)
ControlClick("BUSMASTER Format Conversions","",1007)   ; Click on "Output" button
WinWaitActive("Open", "CANoe Database File(s) (*.dbc)", 3)
Send(@ScriptDir & "\out\bigdbcutpu8t_dbcf_11sample1.dbc{ENTER}")
ControlClick("BUSMASTER Format Conversions","",1008)   ; Click on "Convert" button



;testcase 7
;test case of big dbf file with all constructs
ControlClick("BUSMASTER Format Conversions","",1006)   ; Click on "Input" button
WinWaitActive("Open", "", 3)
Sleep(1000)
Send(@ScriptDir & "\testcocoon_testdata\BIGdbf1.dbf{ENTER}")
sleep(500)
ControlClick("BUSMASTER Format Conversions","",1007)   ; Click on "Output" button
WinWaitActive("Open", "CANoe Database File(s) (*.dbc)", 3)
Send(@ScriptDir & "\out\bigdbcu23tpu8t_dbcf_11sample1.dbc{ENTER}")
ControlClick("BUSMASTER Format Conversions","",1008)   ; Click on "Convert" button


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; TEST CASE FOR LOG-TO-ASC
;select the conversion log to asc

;testcase 1
ControlCommand("BUSMASTER Format Conversions","",1003,"SelectString","LOG to ASC Conversion")		; Select 'LOG to ASC Conversion' from combo box

ControlClick("BUSMASTER Format Conversions","",1006)   ; Click on "Input" button
WinWaitActive("", "BUSMASTER Log File(s) (*log)", 3)
Send(@ScriptDir & "\testcocoon_testdata\BUSMASTERLogFileInput.log{ENTER}")
ControlClick("BUSMASTER Format Conversions","",1007)   ; Click on "Output" button
WinWaitActive("", "CANoe Log File(s) (*.asc)", 3)
Send(@ScriptDir & "\out\BUSMASTERoutput.asc{ENTER}")
ControlClick("BUSMASTER Format Conversions","",1008)   ; Click on "Convert" button

;testcase 2
; log with relative + hex format

ControlClick("BUSMASTER Format Conversions","",1006)   ; Click on "Input" button
WinWaitActive("", "BUSMASTER Log File(s) (*log)", 3)
Send(@ScriptDir & "\testcocoon_testdata\relative-hex-BUSMASTERLogFile0.log{ENTER}")
ControlClick("BUSMASTER Format Conversions","",1007)   ; Click on "Output" button
WinWaitActive("", "CANoe Log File(s) (*.asc)", 3)
Send(@ScriptDir & "\out\BUSMASTERoutputrelhex.asc{ENTER}")
ControlClick("BUSMASTER Format Conversions","",1008)   ; Click on "Convert" button


;testcase 3
; log with absolute with deci format

ControlClick("BUSMASTER Format Conversions","",1006)   ; Click on "Input" button
WinWaitActive("", "BUSMASTER Log File(s) (*log)", 3)
Send(@ScriptDir & "\testcocoon_testdata\absolute-BUSMASTERLogFile0.log{ENTER}")
ControlClick("BUSMASTER Format Conversions","",1007)   ; Click on "Output" button
WinWaitActive("", "CANoe Log File(s) (*.asc)", 3)
Send(@ScriptDir & "\out\BUSMASTERoutputdecabslox.asc{ENTER}")
ControlClick("BUSMASTER Format Conversions","",1008)   ; Click on "Convert" button

;testcase 3
; J1939 dbc to dbf
ControlCommand("BUSMASTER Format Conversions","",1003,"SelectString","J1939 DBC to DBF Conversion")		; Select 'LOG to ASC Conversion' from combo box
ControlClick("BUSMASTER Format Conversions","",1006)   ; Click on "Input" button
WinWaitActive("", "CANoe Database File(s) (*.dbc)", 3)
Send(@ScriptDir & "\testcocoon_testdata\BB_2010_Version24.dbc{ENTER}")
ControlClick("BUSMASTER Format Conversions","",1007)   ; Click on "Output" button
WinWaitActive("", "BUSMASTER Database File(s) (*.dbf)", 3)
Send(@ScriptDir & "\out\BB_2010_Version24out.dbf{ENTER}")
ControlClick("BUSMASTER Format Conversions","",1008)   ; Click on "Convert" button
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; test case for capl to c conversion



; test case for capl to c conversion
; test case 1
if winexists("BUSMASTER Format Conversions") then
	sleep(2000)
$hWnd=controlgethandle("BUSMASTER Format Conversions","","[CLASS:SysTabControl32; INSTANCE:1]")
_GUICtrlTab_ClickTab($hWnd, 0)

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
Send(@ScriptDir & "\out\busmaster1q.cpp{ENTER}")
WinWaitClose("Select BUSMASTER File", "", 2)
endif

controlclick("BUSMASTER Format Conversions","",1018)
controlclick("BUSMASTER Format Conversions","",1004)
WinWaitActive("Select CANoe Database File", "", 2)
Send(@ScriptDir & "\testcocoon_testdata\can_db_mqb.dbc{ENTER}")

;~ controlclick("BUSMASTER Format Conversions","","[CLASS:Button; INSTANCE:9]")
;~ controlclick("BUSMASTER Format Conversions","","[CLASS:Button; INSTANCE:8]")
$hWd=ControlGetHandle("BUSMASTER Format Conversions","",1046)
_GUICtrlListView_ClickItem($hWd,0)
ControlClick("BUSMASTER Format Conversions","",1005)			; click on change dbf button
sleep(1000)
Send(@ScriptDir & "\out\busmasteroutp.dbf{ENTER}")
;click on convert
controlclick("BUSMASTER Format Conversions","",1013)
sleep(4000)

$text = controlgetText("BUSMASTER Format Conversions","",1015)

;click on LOG
controlclick("BUSMASTER Format Conversions","",1008)
sleep(3000)

WinClose("busmaster1qlog.txt - Notepad")

; Direct convert without database
;Send("{TAB}{ENTER}")
;controlclick("Assign Database","",1004)
;WinWaitActive("Select CANoe Database File ", "", 2)
;Send(@DesktopDir & "\can_db_mqb.dbc{ENTER}")


; test case 2 for capl to c comversion

_GUICtrlTab_ClickTab($hWnd, 0)

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
Send(@ScriptDir & "\out\busmaster_cfile.cpp{ENTER}")
WinWaitClose("Select BUSMASTER File", "", 2)


controlclick("BUSMASTER Format Conversions","",1018)
controlclick("BUSMASTER Format Conversions","",1004)
WinWaitActive("Select CANoe Database File", "", 2)
Send(@ScriptDir & "\testcocoon_testdata\CanDB_AEE2010_H91_v1p1_pf.dbc{ENTER}")

;~ ;controlclick("BUSMASTER Format Conversions","","[CLASS:Button; INSTANCE:8]")
$hWd=ControlGetHandle("BUSMASTER Format Conversions","",1046)
_GUICtrlListView_ClickItem($hWd,0)
ControlClick("BUSMASTER Format Conversions","",1005)			; click on change dbf button
sleep(1000)
Send(@ScriptDir & "\out\busmasteroutp22.dbf{ENTER}")
;click on convert
controlclick("BUSMASTER Format Conversions","",1013)
sleep(3000)
Send("{ESC}")
sleep(1000)
send("{ENTER}")
sleep(1000)
$text = controlgetText("BUSMASTER Format Conversions","",1015)

;end





; test case 3 for capl to c comversion

_GUICtrlTab_ClickTab($hWnd, 0)

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
Send(@ScriptDir & "\out\busmaster_cfile.cpp{ENTER}")
WinWaitClose("Select BUSMASTER File", "", 2)

;click on convert without any dbc file
controlclick("BUSMASTER Format Conversions","",1018)
;~ controlclick("BUSMASTER Format Conversions","",1018)

controlclick("BUSMASTER Format Conversions","",1013)

sleep(500)

WinClose("Warning");
sleep(500)
;end
ControlClick("Key Mapping","",1005)  ; click on close button
sleep(1000)
if winexists("Warning") Then
	controlclick("Warning","",2)
endif

sleep(3000)


;~ ;; now test the last tab LOG - to - CVS conversion
_GUICtrlTab_ClickTab($hWnd, 1)

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
sleep(1500)
controlclick("BUSMASTER","",2)


 ;test case2 for J1939 log to CVS conversion


;~ ;Send("{DOWN}")
ControlCommand ("BUSMASTER Format Conversions","","[CLASS:ComboBox; INSTANCE:1]","SelectString","J1939")
sleep(500)
controlclick("BUSMASTER Format Conversions","",1010)
sleep(1000)
Send(@ScriptDir & "\testcocoon_testdata\J1939_Log.log{ENTER}")
sleep(1000)
;ControlSetText ("Select Log File","","[CLASS:Edit; INSTANCE:1]",@ScriptDir & "\testcocoon_testdata\BUSMASTERLogFileInput.log{ENTER}")
controlclick("BUSMASTER Format Conversions","",1011)
Send(@ScriptDir & "\out\J1939fileoutput.xls{ENTER}")
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

