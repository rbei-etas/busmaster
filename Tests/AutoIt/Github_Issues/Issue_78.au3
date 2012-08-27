;===========Test Procedure==============
; Test Cases		: Github Defects Test
; Issue ID			: #18
; Test Strategy		: Black Box
; Critical 			: Y
;===========Test Procedure==============


_launchApp()
_chkCrash(14,3)
sleep(2000)
if Winexists($mWin) Then
	sleep(1000)
	_loadConfig("frmtconvrttest")
	_chkCrash(14,3)
	_loadCANDatabase("DBCTest",1,8)
	WinMenuSelectItem($mWin,"","&File","&Utilities","&Format Converters")
	sleep(1000)
	$frmtConvComboHWD=ControlGetHandle($frmtConvWin,"",1003)
	_GUICtrlComboBox_SelectString($frmtConvComboHWD,"DBF to DBC Conversion")
	ControlClick($frmtConvWin,"",1006)
	sleep(1000)
	if winexists($fCOpenWin) Then
		sleep(1000)
		ControlSend($fCOpenWin,"","[CLASS:Edit; INSTANCE:1]",@DesktopDir&"\DBCTest")
		ControlClick($fCOpenWin,"","[CLASS:Button; INSTANCE:2]")
	EndIf
	ControlClick($frmtConvWin,"",1008)
	$dbcFile=FileExists(@DesktopDir&"\DBCTest.dbc")
	FileDelete(@DesktopDir&"\DBCTest.dbc")
EndIf
if $dbcFile=1 Then
	_ExcelWriteCell($oExcel, "Pass", 14, 3)
Else
	_ExcelWriteCell($oExcel, "Fail", 14, 3)
EndIf