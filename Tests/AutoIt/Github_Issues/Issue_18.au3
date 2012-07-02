;===========Test Procedure==============
; Test Cases		: Github Defects Test
; Issue ID			: #18
; Test Strategy		: Black Box
; Critical 			: Y
;===========Test Procedure==============

_launchApp()
_chkCrash(15,3)
sleep(2000)
if Winexists($mWin) Then
	sleep(1000)
	_loadConfig("saveChge")
	winclose($mWin)
	if WinExists("","You have made changes to the configuration.") Then
		ControlClick("","You have made changes to the configuration.","[CLASS:Button; INSTANCE:1]")
	EndIf
EndIf
_launchApp()
sleep(1000)
$res=0
$cfxSbar=StatusbarGetText($mWin,"",2)
$cfxFName=StringInStr($cfxSbar,"saveChge.cfx")
winclose($mWin)
if WinExists("","You have made changes to the configuration.") Then
	ControlClick("","You have made changes to the configuration.","[CLASS:Button; INSTANCE:1]")
	$res=-1
EndIf
FileDelete(@DesktopDir&"\saveChge.cfx")
ConsoleWrite("$cfxSbar : "&$cfxSbar&@CRLF)
ConsoleWrite("$res : "&$res&@CRLF)
if $cfxFName>0 and $res<>-1 Then
	_ExcelWriteCell($oExcel, "Pass", 15, 3)
Else
	_ExcelWriteCell($oExcel, "Fail", 15, 3)
EndIf