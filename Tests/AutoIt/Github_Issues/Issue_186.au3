;===========Test Procedure==============
; Test Cases		: Github Defects Test
; Issue ID			: #186
; Test Strategy		: Black Box
; Critical 			: Y
;===========Test Procedure==============
#Include <GuiListView.au3>
#Include <GuiTreeView.au3>

_launchApp()												; invoke the application and run the Process
_chkCrash(6,3)
sleep(1000)
$testRes=0
if winexists($mWin) Then
	_loadConfig("Github")
	_chkCrash(6,3)
	_loadCANDatabase("186TestGithub",1,8)
	_chkCrash(6,3)
	sleep(1000)
	if Winexists($dbEditCAN) Then
		$hWd=ControlGetHandle($mWin,"", "SysListView321")
		_GUICtrlListView_ClickItem($hWd,0)
		sleep(500)
		send("{F2}")
	EndIf
EndIf
sleep(1000)
if Winexists($sigDetails) Then
	$testRes=1
	ControlClick($sigDetails,"","[CLASS:Button; INSTANCE:2]")
EndIf
if $testRes=1 Then
	_ExcelWriteCell($oExcel, "Pass", 6, 3)
Else
	_ExcelWriteCell($oExcel, "Fail", 6, 3)
EndIf
WinMenuSelectItem($mWin,"","&File","&Database","&Close")
if WinExists($mWin,"") Then
	ControlClick($mWin,"","[CLASS:Button; INSTANCE:2]")
	sleep(1000)
EndIf

