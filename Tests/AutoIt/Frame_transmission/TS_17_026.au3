; Test Case Number:	TS_17_026
; Feature:			Frame Transmission
; Critical (C):		Y
; Test Cases:		Value assignement for the database messages
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===


#Include <GuiListView.au3>
WinWaitActive("BUSMASTER","",5)
$hWnd = ControlGetHandle("BUSMASTER","",1232)
$a=_GUICtrlListView_GetItemPosition($hWnd, 0)
ControlClick("BUSMASTER","",1232,"Left",2,$a[0]+100,$a[1])
$spinCtrlRaw=ControlCommand("BUSMASTER","",74565,"IsVisible")
ControlClick("BUSMASTER","",1232,"Left",2,$a[0]+200,$a[1])
$spinCtrlPhy=ControlCommand("BUSMASTER","",74565,"IsVisible")
