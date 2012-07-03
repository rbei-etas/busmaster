; Test Case Number:	TS_17_029
; Feature:			Frame Transmission
; Critical (C):		Y
; Test Cases:		Adding database message in TX message List
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===

#Include <GuiListView.au3>
WinWaitActive("BUSMASTER","",5)
$listViewCount1=ControlListView("BUSMASTER", "", "SysListView322", "GetItemCount")  
$hWnd = ControlGetHandle("BUSMASTER","",1232)
_GUICtrlListView_ClickItem($hWnd,0,"Left","",1)
ControlClick("BUSMASTER","Add &Message",1122)
$listViewCount2=ControlListView("BUSMASTER", "", "SysListView322", "GetItemCount")