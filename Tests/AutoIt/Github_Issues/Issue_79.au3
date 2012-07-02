;===========Test Procedure==============
; Test Cases		: Github Defects Test
; Issue ID			: #79
; Test Strategy		: Black Box
; Critical 			: Y
;===========Test Procedure==============

_launchApp()												; invoke the application and run the Process
_chkCrash(10,3)
sleep(1000)
if winexists($mWin) Then
	_loadConfig("79Github")
	_chkCrash(10,3)
	_loadCANDatabase("Test79",1,8)
	_chkCrash(10,3)
	sleep(1000)
	WinMenuSelectItem($mWin,"","&File","&Database","&Close")
	_asoDB("&File","Test79.dbf")
	$dbMenu=WinMenuSelectItem($mWin,"","&File","&Database","&Disassociate")
	sleep(1000)
	if WinExists($disDbDialog) Then
		$disDBHWD=ControlGetHandle($disDbDialog,"","[CLASS:ListBox; INSTANCE:1]")
		$fCount=_GUICtrlListBox_GetCount($disDBHWD)
		ControlClick($disDbDialog,"","[CLASS:Button; INSTANCE:2]")
	EndIf
	WinMenuSelectItem($mWin,"","&File","&Database","&Save")
	FileDelete(@DesktopDir&"\Test79")
	WinClose($mWin)
	if WinExists("","You have made changes to the configuration.") Then
		ControlClick("","You have made changes to the configuration.","[CLASS:Button; INSTANCE:1]")
	EndIf
EndIf
sleep(1000)
_launchApp()
sleep(1000)
WinMenuSelectItem($mWin,"","&File","&Database","&Disassociate")
if WinExists($disDbDialog) Then
	$fCount1=_GUICtrlListBox_GetCount($disDBHWD)
	ControlClick($disDbDialog,"","[CLASS:Button; INSTANCE:2]")
EndIf
if $fCount1=$fCount-1 then
	_ExcelWriteCell($oExcel, "Pass", 10, 3)
Else
	_ExcelWriteCell($oExcel, "Fail", 10, 3)
EndIf





