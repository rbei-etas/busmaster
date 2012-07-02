;===========Test Procedure==============
; Test Cases		: Github Defects Test
; Issue ID			: #22
; Test Strategy		: Black Box
; Critical 			: Y
;===========Test Procedure==============

_launchApp()
_chkCrash(17,3)
sleep(2000)
if Winexists($mWin) Then
	sleep(1000)
	_loadConfig("Git22")
	if winexists($mWin) Then
		sleep(500)
		_hdWareSelect("&Kvaser CAN")
		$chnlbar=StatusbarGetText($mWin,"",5)
		$hwrName=StringInStr($chnlbar,"Kvaser CAN")
	EndIf
	WinMenuSelectItem($mWin,"","&Configure","&Hardware Interface","&Simulation")
	WinMenuSelectItem($mWin,"","&File","Confi&guration","&Save")
EndIf
if $kvsrHdw=1 and $hwrName>0 Then
	_ExcelWriteCell($oExcel, "Pass", 17, 3)
Else
	_ExcelWriteCell($oExcel, "Fail", 17, 3)
EndIf
