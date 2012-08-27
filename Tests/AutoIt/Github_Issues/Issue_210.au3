;===========Test Procedure==============
; Test Cases		: Github Defects Test
; Issue ID			: #210
; Test Strategy		: Black Box
; Critical 			: Y
;===========Test Procedure==============

_launchApp()													; invoke the application and run the Process
_chkCrash(3,3)
sleep(1000)
if WinExists($mWin) then
	sleep(1000)
	if WinExists("MCNet") Then
		send("{ENTER}")
	EndIf
	sleep(1000)
	WinMenuSelectItem($mWin,"","&File","&Utilities","&Format Converters")
	sleep(1000)
	if WinExists($fConvert) Then
		_ExcelWriteCell($oExcel, "Pass", 3, 3)
	Else
		_ExcelWriteCell($oExcel, "Fail", 3, 3)
	EndIf
	ControlClick($fConvert,"","[CLASS:Button; INSTANCE:8]","left")
EndIf
