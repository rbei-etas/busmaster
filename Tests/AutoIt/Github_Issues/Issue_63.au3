;===========Test Procedure==============
; Test Cases		: Github Defects Test
; Issue ID			: #63
; Test Strategy		: Black Box
; Critical 			: Y
;===========Test Procedure==============

_launchApp()
_chkCrash(13,3)
sleep(2000)
if winexists($mWin) Then
	_loadConfig("HWTest")
	_chkCrash(11,3)
	if winexists($hWare) Then																		; if a Hardware Selection dialog appears then click 'OK'
		ControlClick($hWare,$selectBTN,"[CLASS:Button; INSTANCE:2]")
		sleep(1000)
		ControlClick($hWare,$selectBTN,"[CLASS:Button; INSTANCE:2]")
		sleep(1000)
		ControlClick($hWare,$okBTN,"[CLASS:Button; INSTANCE:4]")
		sleep(1000)
	EndIf
	$ctrl=StatusbarGetText($mWin,"",5)
	ConsoleWrite($ctrl&@CRLF)
	$fndctrl=StringInStr($ctrl,"Vector XL")
	ConsoleWrite($fndctrl&@CRLF)
EndIf
if $fndctrl>0 Then
	_ExcelWriteCell($oExcel, "Pass", 13, 3)
Else
	_ExcelWriteCell($oExcel, "Fail", 13, 3)
EndIf