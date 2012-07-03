;Test Case Number:	TS_17_011
;Feature:			Frame Transmission
; Critical (C):		Y
; Test Cases:		Adding/Modifying Message Block
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===

_launchApp()														; check whether the app exists or else launch it.
if $app=0 Then
	Send("!ct")
	sleep(2000)
Endif
WinWaitActive("BUSMASTER","",5)
if (ControlClick("BUSMASTER","Add",1226))=1 then
	local $msgName = ControlListView("BUSMASTER", "", "SysListView323", "GetText", "0", "");

	$mBlock=ControlCommand ("BUSMASTER", "",1225, "IsEnabled")

	$bName=ControlCommand ("BUSMASTER", "",1228, "IsEnabled")
	$bCyc=ControlCommand ("BUSMASTER", "Cyclic",1007, "IsEnabled")
	$bMono=ControlCommand ("BUSMASTER", "Monoshot",1008, "IsEnabled")
	$bTd=ControlCommand ("BUSMASTER", "",1236, "IsEnabled")
	$bTdval=ControlCommand ("BUSMASTER", "",1134, "IsEnabled")
	$bKey=ControlCommand ("BUSMASTER", "",1237, "IsEnabled")
	$badd=ControlCommand ("BUSMASTER", "Add",1226, "IsEnabled")
	$bdel=ControlCommand ("BUSMASTER", "Delete",1227, "IsEnabled")

	if ($msgName="OnTime") and ($mBlock and $bName and $bCyc and $bMono and $bTd and $bTdval and $bKey and $badd and $bdel)=1 Then
		$BlockDetails=1
		Else
		$BlockDetails=0
	EndIf
	$txList=ControlCommand ("BUSMASTER", "",1099, "IsEnabled")

	$mName=ControlCommand ("BUSMASTER", "Name/ ID (in Dec)",1221, "IsEnabled") ;checks whether the conttrol is visible or not. if visible returns 1 else 0.
	$mDLC=ControlCommand ("BUSMASTER", "",1125, "IsEnabled")
	$mDB1=ControlCommand ("BUSMASTER", "",1126, "IsEnabled")
	$mDB2=ControlCommand ("BUSMASTER", "",1127, "IsEnabled")
	$mDB3=ControlCommand ("BUSMASTER", "",1128, "IsEnabled")
	$mDB4=ControlCommand ("BUSMASTER", "",1129, "IsEnabled")
	$mDB5=ControlCommand ("BUSMASTER", "",1130, "IsEnabled")
	$mDB6=ControlCommand ("BUSMASTER", "",1131, "IsEnabled")
	$mDB7=ControlCommand ("BUSMASTER", "",1132, "IsEnabled")
	$mDB8=ControlCommand ("BUSMASTER", "",1133, "IsEnabled")
	$mChlid=ControlCommand ("BUSMASTER", "",1010, "IsEnabled")
	$mRadstd=ControlCommand ("BUSMASTER", "",1215, "IsEnabled")
	$mRadextd=ControlCommand ("BUSMASTER", "",1213, "IsEnabled")
	$mRTR=ControlCommand ("BUSMASTER", "",1216, "IsEnabled")


	$mSdetails=ControlCommand ("BUSMASTER", "Signal Details","1232", "IsEnabled")
	$mAddmsg=ControlCommand ("BUSMASTER", "Add &Message",1122, "IsEnabled")
EndIf	