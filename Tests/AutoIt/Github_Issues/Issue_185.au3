;===========Test Procedure==============
; Test Cases		: Github Defects Test
; Issue ID			: #185
; Test Strategy		: Black Box
; Critical 			: Y
;===========Test Procedure==============
#Include <GuiListView.au3>
#Include <GuiTreeView.au3>

_launchApp()												; invoke the application and run the Process
_chkCrash(5,3)
sleep(1000)
if winexists($mWin) Then
	_loadConfig("Github")
	_chkCrash(5,3)
	_loadCANDatabase("TestGithub185",1,8)
	_chkCrash(5,3)
	sleep(1000)
	if Winexists($dbEditCAN) Then
		$hWd=ControlGetHandle($mWin,"", "SysListView321")
		_GUICtrlListView_ClickItem($hWd,0)
		sleep(1000)
		ControlClick($dbEditCAN,"","[CLASS:Button; INSTANCE:7]")
		sleep(1000)
		if Winexists($sigDesc) Then
			ControlSetText($sigDesc,"","[CLASS:Edit; INSTANCE:1]","test")
			ControlSetText($sigDesc,"","[CLASS:Edit; INSTANCE:2]",10)
			ControlClick($sigDesc,"","[CLASS:Button; INSTANCE:1]")
			sleep(1000)
		EndIf
		$sigDescCount=ControlListView($mWin, "", "SysListView322", "GetItemCount")
		$hWd=ControlGetHandle($mWin,"", "SysListView322")
		_GUICtrlListView_ClickItem($hWd,0)
		send("{DELETE}")
		sleep(1000)
		if WinExists($mWin,"You are about to delete a signal description and its value") Then
			ControlClick($mWin,"","[CLASS:Button; INSTANCE:1]")
			sleep(1000)
		EndIf
		$sigDescCount1=ControlListView($mWin, "", "SysListView322", "GetItemCount")
		$sigDetCount=ControlListView($mWin, "", "SysListView321", "GetItemCount")
		$hWd1=ControlGetHandle($mWin,"", "SysListView321")
		_GUICtrlListView_ClickItem($hWd1,0)
		sleep(500)
		send("{DELETE}")
		sleep(1000)
		if WinExists($mWin,"You are about to delete a signal") Then
			ControlClick($mWin,"","[CLASS:Button; INSTANCE:1]")
			sleep(1000)
		EndIf
		$sigDetCount1=ControlListView($mWin, "", "SysListView321", "GetItemCount")
		$a=ControlTreeView($mWin,"","[CLASS:SysTreeView32; INSTANCE:1]","GetText")
		$msgCount=ControlTreeView($mWin,"","[CLASS:SysTreeView32; INSTANCE:1]","GetItemCount",$a)
		;ControlTreeView("BUSMASTER","","[CLASS:SysTreeView32; INSTANCE:1]","Select","#0|#0")
		ControlFocus($dbEditCAN,"","[CLASS:SysTreeView32; INSTANCE:1]")
		$hWd2=ControlGetHandle($mWin,"", "SysTreeView321")
		_GUICtrlTreeView_ClickItem($hWd2,0)
		send("{DELETE}")
		sleep(1000)
		if WinExists($mWin,"") Then
			ControlClick($mWin,"","[CLASS:Button; INSTANCE:1]")
			sleep(1000)
		EndIf
		$msgCount1=ControlTreeView($mWin,"","[CLASS:SysTreeView32; INSTANCE:1]","GetItemCount",$a)
	EndIf
EndIf
if $sigDescCount1=$sigDescCount-1 and $sigDetCount1=$sigDetCount-1 and $msgCount1=$msgCount-1 Then
	_ExcelWriteCell($oExcel, "Pass", 5, 3)
Else
	_ExcelWriteCell($oExcel, "Fail", 5, 3)
EndIf
WinMenuSelectItem($mWin,"","&File","&Database","&Close")
if WinExists($mWin,"") Then
	ControlClick($mWin,"","[CLASS:Button; INSTANCE:2]")
	sleep(1000)
EndIf