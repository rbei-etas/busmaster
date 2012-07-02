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

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
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


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
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


;testcase 2
; TEST CASE FOR DBC-TO-DBF
;select the conversion dbc to dbf 

;get the focus back to the combo box list
Send("{TAB}{TAB}{TAB}")

; Select the Conversion Type
;Send("{DOWN}") ; DBC TO DBF Conversion

; Input File
Send("{TAB}{TAB}{ENTER}")
WinWaitActive("", "CANoe Database File(s) (*.dbc)", 3)
Send(@ScriptDir & "\dbc2dbf_signals.dbc{ENTER}")
WinWaitClose("", "CANoe Database File(s) (*.dbc)", 3)

; Output File
Send("{TAB}{TAB}{ENTER}")
WinWaitActive("", "BUSMASTER Database File(s) (*.dbf)", 3)
Send(@ScriptDir & "\out\outputsample1_signals_dbf.dbf{ENTER}")
WinWaitClose("", "BUSMASTER Database File(s) (*.dbf)", 3)

; Convert
Send("{TAB}{ENTER}")

; Result
Send("{TAB}")
Send("{CTRLDOWN}c{CTRLUP}") ; copy
$ResultStr = ClipGet();
ConsoleWrite($ResultStr & @CRLF)



;testcase 3
; TEST CASE FOR DBC-TO-DBF
;select the conversion dbc to dbf 

;get the focus back to the combo box list
Send("{TAB}{TAB}{TAB}")

; Select the Conversion Type
;Send("{DOWN}") ; DBC TO DBF Conversion

; Input File
Send("{TAB}{TAB}{ENTER}")
WinWaitActive("", "CANoe Database File(s) (*.dbc)", 3)
Send(@ScriptDir & "\testcocoon_testdata\all_in_one.dbc{ENTER}")
WinWaitClose("", "CANoe Database File(s) (*.dbc)", 3)

; Output File
Send("{TAB}{TAB}{ENTER}")
WinWaitActive("", "BUSMASTER Database File(s) (*.dbf)", 3)
Send(@ScriptDir & "\out\outputsample1_signals_dbf.dbf{ENTER}")
WinWaitClose("", "BUSMASTER Database File(s) (*.dbf)", 3)

; Convert
Send("{TAB}{ENTER}")

; Result
Send("{TAB}")
Send("{CTRLDOWN}c{CTRLUP}") ; copy
$ResultStr = ClipGet();
ConsoleWrite($ResultStr & @CRLF)




;testcase 4
; TEST CASE FOR DBC-TO-DBF
;select the conversion dbc to dbf ;;;huge file

;get the focus back to the combo box list
Send("{TAB}{TAB}{TAB}")

; Select the Conversion Type
;Send("{DOWN}") ; DBC TO DBF Conversion

; Input File
Send("{TAB}{TAB}{ENTER}")
WinWaitActive("", "CANoe Database File(s) (*.dbc)", 3)
Send(@ScriptDir & "\testcocoon_testdata\can_db_v6x.dbc{ENTER}")
WinWaitClose("", "CANoe Database File(s) (*.dbc)", 3)

; Output File
Send("{TAB}{TAB}{ENTER}")
WinWaitActive("", "BUSMASTER Database File(s) (*.dbf)", 4)
Send(@ScriptDir & "\out\BIGdbf.dbf{ENTER}")
WinWaitClose("", "BUSMASTER Database File(s) (*.dbf)", 3)

; Convert
Send("{TAB}{ENTER}")
Sleep(2000)

; Result
Send("{TAB}")
Send("{CTRLDOWN}c{CTRLUP}") ; copy
$ResultStr = ClipGet();
ConsoleWrite($ResultStr & @CRLF)


;testcase 5
; TEST CASE FOR DBC-TO-DBF
;select the conversion dbc to dbf ;;;huge file

;get the focus back to the combo box list
Send("{TAB}{TAB}{TAB}")

; Select the Conversion Type
;Send("{DOWN}") ; DBC TO DBF Conversion

; Input FileD:\BUSMASTER OSS\Tests\AutoIt\Format Converter\dbc2dbf_messages.dbc
		
		
Send("{TAB}{TAB}{ENTER}")
WinWaitActive("", "CANoe Database File(s) (*.dbc)", 3)
Send(@ScriptDir & "\testcocoon_testdata\vw360e_acan.dbc{ENTER}")
WinWaitClose("", "CANoe Database File(s) (*.dbc)", 3)

; Output File
Send("{TAB}{TAB}{ENTER}")
WinWaitActive("", "BUSMASTER Database File(s) (*.dbf)", 3)
Send(@ScriptDir & "\out\BIGdbf1.dbf{ENTER}")
WinWaitClose("", "BUSMASTER Database File(s) (*.dbf)", 3)

; Convert
Send("{TAB}{ENTER}")

Sleep(2000)
; Result
Send("{TAB}")
Send("{CTRLDOWN}c{CTRLUP}") ; copy
$ResultStr = ClipGet();
ConsoleWrite($ResultStr & @CRLF)


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; TEST CASE FOR DBF-TO-DBc
;select the conversion dbf to dbc 
;testcase 1
;get the focus back to the combo box list
Send("{TAB}{TAB}{TAB}")

; Select the Conversion Type
Send("{DOWN}") ; dbf TO dbc Conversion

; Input File
Send("{TAB}{TAB}{ENTER}")
WinWaitActive("Open", "", 3)
Send(@ScriptDir & "\dbc2dbf_messages.dbf{ENTER}")
WinWaitClose("Open", "", 3)



; Output File
Send("{TAB}{TAB}{ENTER}")
WinWaitActive("Open", "CANoe Database File(s) (*.dbc)", 3)
Send(@ScriptDir & "\out\output_dbc_sample1.dbc{ENTER}")
WinWaitClose("Open", "CANoe Database File(s) (*.dbc)", 3)

; Convert
Send("{TAB}{ENTER}")

; Result
Send("{TAB}")
Send("{CTRLDOWN}c{CTRLUP}") ; copy
$ResultStr = ClipGet();
ConsoleWrite($ResultStr & @CRLF)



;testcase 2

;get the focus back to the combo box list
Send("{TAB}{TAB}{TAB}")

; Select the Conversion Type
; for testcase 2 down is not required
;Send("{DOWN}") ; dbf TO dbc Conversion

; Input File
Send("{TAB}{TAB}{ENTER}")
WinWaitActive("Open", "", 3)
Send(@ScriptDir & "\dbf2dbc_signals.dbf{ENTER}")
WinWaitClose("Open", "", 3)



; Output File
Send("{TAB}{TAB}{ENTER}")
WinWaitActive("Open", "CANoe Database File(s) (*.dbc)", 3)
Send(@ScriptDir & "\out\output_dbc_11sample1.dbc{ENTER}")
WinWaitClose("Open", "CANoe Database File(s) (*.dbc)", 3)

; Convert
Send("{TAB}{ENTER}")

; Result
Send("{TAB}")
Send("{CTRLDOWN}c{CTRLUP}") ; copy
$ResultStr = ClipGet();
ConsoleWrite($ResultStr & @CRLF)



;testcase 3

;get the focus back to the combo box list
Send("{TAB}{TAB}{TAB}")

; Select the Conversion Type
; for testcase 2 down is not required
;Send("{DOWN}") ; dbf TO dbc Conversion

; Input File
Send("{TAB}{TAB}{ENTER}")
WinWaitActive("Open", "", 3)
Send(@ScriptDir & "\dbc2dbf_signals.dbf{ENTER}")
WinWaitClose("Open", "", 3)



; Output File
Send("{TAB}{TAB}{ENTER}")
WinWaitActive("Open", "CANoe Database File(s) (*.dbc)", 3)
Send(@ScriptDir & "\out\output_dbcf_11sample1.dbc{ENTER}")
WinWaitClose("Open", "CANoe Database File(s) (*.dbc)", 3)

; Convert
Send("{TAB}{ENTER}")

; Result
Send("{TAB}")
Send("{CTRLDOWN}c{CTRLUP}") ; copy
$ResultStr = ClipGet();
ConsoleWrite($ResultStr & @CRLF)



;testcase 4

;get the focus back to the combo box list
Send("{TAB}{TAB}{TAB}")

; Select the Conversion Type
; for testcase 2 down is not required
;Send("{DOWN}") ; dbf TO dbc Conversion

; Input File
Send("{TAB}{TAB}{ENTER}")
WinWaitActive("Open", "", 3)
Sleep(1000)
Send(@ScriptDir & "\testcocoon_testdata\all_in_one.dbf{ENTER}")
sleep(500)
WinWaitClose("Open", "", 3)



; Output File
Send("{TAB}{TAB}{ENTER}")
WinWaitActive("Open", "CANoe Database File(s) (*.dbc)", 3)
Send(@ScriptDir & "\out\outpu8t_dbcf_11sample1.dbc{ENTER}")
WinWaitClose("Open", "CANoe Database File(s) (*.dbc)", 3)

; Convert
Send("{TAB}{ENTER}")

; Result
Send("{TAB}")
Send("{CTRLDOWN}c{CTRLUP}") ; copy
$ResultStr = ClipGet();
ConsoleWrite($ResultStr & @CRLF)



;testcase 5
;test case of invalid input file
;get the focus back to the combo box list
Send("{TAB}{TAB}{TAB}")

; Select the Conversion Type
; for testcase 2 down is not required
;Send("{DOWN}") ; dbf TO dbc Conversion

; Input File
Send("{TAB}{TAB}{ENTER}")
WinWaitActive("Open", "", 3)
Sleep(1000)
Send(@ScriptDir & "\testcocoon_testdata\Copy of CanDB_AEE2010_H91_v1p1_pf.dbf{ENTER}")
sleep(500)
WinWaitClose("Open", "", 3)



; Output File

Send("{TAB}{TAB}{ENTER}")
WinWaitActive("Open", "CANoe Database File(s) (*.dbc)", 3)
Send(@ScriptDir & "\out\outpu8t_dbcf_11sample1.dbc{ENTER}")
WinWaitClose("Open", "CANoe Database File(s) (*.dbc)", 3)

; Convert
Send("{TAB}{ENTER}")

; Result
Send("{TAB}")
Send("{CTRLDOWN}c{CTRLUP}") ; copy
$ResultStr = ClipGet();
ConsoleWrite($ResultStr & @CRLF)




;testcase 6
;test case of big dbf file with all constructs
;get the focus back to the combo box list
Send("{TAB}{TAB}{TAB}")

; Select the Conversion Type
; for testcase 2 down is not required
;Send("{DOWN}") ; dbf TO dbc Conversion

; Input File
Send("{TAB}{TAB}{ENTER}")
WinWaitActive("Open", "", 3)
Sleep(1000)
Send(@ScriptDir & "\testcocoon_testdata\BigDBF.dbf{ENTER}")
sleep(500)
WinWaitClose("Open", "", 3)



; Output File

Send("{TAB}{TAB}{ENTER}")
WinWaitActive("Open", "CANoe Database File(s) (*.dbc)", 3)
Send(@ScriptDir & "\out\bigdbcutpu8t_dbcf_11sample1.dbc{ENTER}")
WinWaitClose("Open", "CANoe Database File(s) (*.dbc)", 3)

; Convert
Send("{TAB}{ENTER}")
sleep(1500)
; Result
Send("{TAB}")
Send("{CTRLDOWN}c{CTRLUP}") ; copy
$ResultStr = ClipGet();
ConsoleWrite($ResultStr & @CRLF)



;testcase 7
;test case of big dbf file with all constructs
;get the focus back to the combo box list
Send("{TAB}{TAB}{TAB}")

; Select the Conversion Type
; for testcase 2 down is not required
;Send("{DOWN}") ; dbf TO dbc Conversion

; Input File
Send("{TAB}{TAB}{ENTER}")
WinWaitActive("Open", "", 3)
Sleep(1000)
Send(@ScriptDir & "\testcocoon_testdata\BIGdbf1.dbf{ENTER}")
sleep(500)
WinWaitClose("Open", "", 3)



; Output File

Send("{TAB}{TAB}{ENTER}")
WinWaitActive("Open", "CANoe Database File(s) (*.dbc)", 3)
Send(@ScriptDir & "\out\bigdbcu23tpu8t_dbcf_11sample1.dbc{ENTER}")
WinWaitClose("Open", "CANoe Database File(s) (*.dbc)", 3)

; Convert
Send("{TAB}{ENTER}")
sleep(1500)
; Result
Send("{TAB}")
Send("{CTRLDOWN}c{CTRLUP}") ; copy
$ResultStr = ClipGet();
ConsoleWrite($ResultStr & @CRLF)


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; TEST CASE FOR LOG-TO-ASC
;select the conversion log to asc  

;testcase 1
;get the focus back to the combo box list
Send("{TAB}{TAB}{TAB}")

; Select the Conversion Type
Send("{DOWN}") ; LOG TO asc  Conversion

; Input File
Send("{TAB}{TAB}{ENTER}")
WinWaitActive("", "BUSMASTER Log File(s) (*log)", 3)
Send(@ScriptDir & "\testcocoon_testdata\BUSMASTERLogFileInput.log{ENTER}")
WinWaitClose("", "BUSMASTER Log File(s) (*.log)", 3)

; Output File
Send("{TAB}{TAB}{ENTER}")
WinWaitActive("", "CANoe Log File(s) (*.asc)", 3)
Send(@ScriptDir & "\out\BUSMASTERoutput.asc{ENTER}")
WinWaitClose("", "CANoe Log File(s) (*.asc)", 3)

; Convert
Send("{TAB}{ENTER}")

; Result
Send("{TAB}")
Send("{CTRLDOWN}c{CTRLUP}") ; copy
$ResultStr = ClipGet();
ConsoleWrite($ResultStr & @CRLF)


;testcase 2
; log with relative + hex format

;get the focus back to the combo box list
Send("{TAB}{TAB}{TAB}")

; Select the Conversion Type
; now not required
;Send("{DOWN}") ; LOG TO asc  Conversion

; Input File
Send("{TAB}{TAB}{ENTER}")
WinWaitActive("", "BUSMASTER Log File(s) (*log)", 3)
Send(@ScriptDir & "\testcocoon_testdata\relative-hex-BUSMASTERLogFile0.log{ENTER}")
WinWaitClose("", "BUSMASTER Log File(s) (*.log)", 3)

; Output File
Send("{TAB}{TAB}{ENTER}")
WinWaitActive("", "CANoe Log File(s) (*.asc)", 3)
Send(@ScriptDir & "\out\BUSMASTERoutputrelhex.asc{ENTER}")
WinWaitClose("", "CANoe Log File(s) (*.asc)", 3)

; Convert
Send("{TAB}{ENTER}")

; Result
Send("{TAB}")
Send("{CTRLDOWN}c{CTRLUP}") ; copy
$ResultStr = ClipGet();
ConsoleWrite($ResultStr & @CRLF)


;testcase 3
; log with absolute with deci format

;get the focus back to the combo box list
Send("{TAB}{TAB}{TAB}")



; Select the Conversion Type
; now not required
;Send("{DOWN}") ; LOG TO asc  Conversion

; Input File
Send("{TAB}{TAB}{ENTER}")
WinWaitActive("", "BUSMASTER Log File(s) (*log)", 3)
Send(@ScriptDir & "\testcocoon_testdata\absolute-BUSMASTERLogFile0.log{ENTER}")
WinWaitClose("", "BUSMASTER Log File(s) (*.log)", 3)

; Output File
Send("{TAB}{TAB}{ENTER}")
WinWaitActive("", "CANoe Log File(s) (*.asc)", 3)
Send(@ScriptDir & "\out\BUSMASTERoutputdecabslox.asc{ENTER}")
WinWaitClose("", "CANoe Log File(s) (*.asc)", 3)

; Convert
Send("{TAB}{ENTER}")

; Result
Send("{TAB}")
Send("{CTRLDOWN}c{CTRLUP}") ; copy
$ResultStr = ClipGet();
ConsoleWrite($ResultStr & @CRLF)



;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; test case for capl to c conversion



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

