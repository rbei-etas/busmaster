;===========Test Procedure==============
; Test Cases		: Github Defects Test
; Issue ID			: #93
; Test Strategy		: Black Box
; Critical 			: Y
;===========Test Procedure==============

_launchApp()
_chkCrash(2,3)
sleep(1000)
if Winexists($mWin) Then
	if winexists("","Trace Window") Then
		WinMenuSelectItem($mWin,"","&View","T&race Window")
	EndIf
	sleep(1000)
	WinMenuSelectItem($mWin,"","&View","T&race Window")
	$trace=WinExists("","Trace Window")
	ConsoleWrite("$trace : "&$trace&@CRLF)

	if WinExists("","Trace Window") Then
		_ExcelWriteCell($oExcel, "Pass", 4, 3)
	Else
		_ExcelWriteCell($oExcel, "Fail", 4, 3)
	EndIf
	WinMenuSelectItem($mWin,"","&View","T&race Window")
EndIf