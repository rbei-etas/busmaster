;===========Test Procedure==============
; Test Cases		: Github Defects Test
; Issue ID			: #149
; Test Strategy		: Black Box
; Critical 			: Y
;===========Test Procedure==============

_launchApp()
_chkCrash(16,3)
sleep(2000)
if Winexists($mWin) Then
	sleep(1000)
	_loadConfig("Git149")
	if winexists($mWin) Then
		sleep(500)
		WinMenuSelectItem($mWin,"","&Configure","&Hardware Interface","&Kvaser CAN")    ; Select Kvaser Hardware
		sleep(1000)
		if winexists($hWare) Then
			$avlCANhWd=ControlGetHandle($hWare,"", "SysListView321")
			$avlCAN1=_GUICtrlListView_GetItemCount($avlCANhWd)
			ControlClick($hWare,$selectBTN,"[CLASS:Button; INSTANCE:2]")
			sleep(1000)
			ControlClick($hWare,$selectBTN,"[CLASS:Button; INSTANCE:2]")
			sleep(1000)
			$cfgCANhWd=ControlGetHandle($hWare,"", "SysListView322")
			$cfgCANChnls=_GUICtrlListView_GetItemCount($cfgCANhWd)
			for $i=0 to $cfgCANChnls-1
				_GUICtrlListView_ClickItem($cfgCANhWd,0)
				ControlClick($hWare,"","[CLASS:Button; INSTANCE:3]")
				sleep(1000)
			Next
			$avlCAN2=_GUICtrlListView_GetItemCount($avlCANhWd)
			ControlClick($hWare,"","[CLASS:Button; INSTANCE:5]")
			sleep(1000)
			if WinExists($kvaserCANWin) Then
				ControlClick($kvaserCANWin,"","[CLASS:Button; INSTANCE:1]")
				sleep(1000)
			EndIf
		EndIf
	EndIf
	WinMenuSelectItem($mWin,"","&Configure","&Hardware Interface","&Simulation")
	WinMenuSelectItem($mWin,"","&File","Confi&guration","&Save")
EndIf
if (($avlCAN1 and $cfgCANChnls and $avlCAN2)=2) Then
	_ExcelWriteCell($oExcel, "Pass", 16, 3)
Else
	_ExcelWriteCell($oExcel, "Fail", 16, 3)
EndIf


