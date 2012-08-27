;===========Test Procedure==============
; Test Cases		: Github Defects Test
; Issue ID			: #92
; Test Strategy		: Black Box
; Critical 			: Y
;===========Test Procedure==============


_launchApp()
_chkCrash(8,3)
sleep(1000)
$winExists=0
$BTN=0
if Winexists($mWin) Then
	sleep(2000)
	WinClose ($mWin)
EndIf
sleep(500)
if winexists($mWin) and winexists("","You have made changes to the configuration.") Then
	$winExists=1
	$yBTN=ControlCommand($mWin,"","[CLASS:Button; INSTANCE:1]","IsEnabled")
	$nBTN=ControlCommand($mWin,"","[CLASS:Button; INSTANCE:2]","IsEnabled")
	$cclBTN=ControlCommand($mWin,"","[CLASS:Button; INSTANCE:3]","IsEnabled")
	controlclick($mWin,"","[CLASS:Button; INSTANCE:3]")
	sleep(1000)
EndIf
if (($yBTN and $nBTN and $cclBTN)=1) Then
	$BTN=1
EndIf

if $winExists=1 and $BTN=1 Then
	_ExcelWriteCell($oExcel, "Pass", 8, 3)
Else
	_ExcelWriteCell($oExcel, "Fail", 8, 3)
EndIf


