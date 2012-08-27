;===========Test Procedure==============
; Test Cases		: Github Defects Test
; Issue ID			: #181
; Test Strategy		: Black Box
; Critical 			: Y
;===========Test Procedure==============


_launchApp()
_chkCrash(9,3)
if WinExists("MCNet") Then
	send("{ENTER}")
EndIf
sleep(1000)
_loadJ1939Database("181Github.DBF",12,8,64)
$errMsg=0
sleep(1000)
if Winexists($dbEditJ1939) Then
	$hWd=ControlGetHandle($mWin,"", "SysListView321")
	ConsoleWrite("$hWd:"&$hWd&@CRLF)
	_GUICtrlListView_ClickItem($hWd,0)
	sleep(1000)
	send("{F2}")
	sleep(1000)
	$editHWD=ControlGetHandle($sigDetails,"","[CLASS:Edit; INSTANCE:1]")
	$txtVal=_GUICtrlEdit_GetText($editHWD)
	if $txtVal="test" then
		ControlSetText($sigDetails,"","[CLASS:Edit; INSTANCE:1]","test1")
	Else
		ControlSetText($sigDetails,"","[CLASS:Edit; INSTANCE:1]","test")
	EndIf
	if WinExists("","Duplicate signal start bit value") Then
		$errMsg=1
	EndIf
	ControlClick($sigDetails,"","[CLASS:Button; INSTANCE:1]")
	if WinExists("","Duplicate signal start bit value") Then
		$errMsg=1
	EndIf
	WinMenuSelectItem($mWin,"","&J1939","&Database","&Close")
	if WinExists($mWin,"Do you want to save changes to the database file?") Then
		ControlClick($mWin,"","[CLASS:Button; INSTANCE:1]")
	EndIf
EndIf
if $errMsg=0 Then
	_ExcelWriteCell($oExcel, "Pass", 9, 3)
Else
	_ExcelWriteCell($oExcel, "Fail", 9, 3)
EndIf





