;Test Case Number:	TS_17_006
;Feature:			Frame Transmission
; Critical (C):		Y
; Test Cases:		Tx window Message details GUI check
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===

_launchApp()														; check whether the app exists or else launch it.
if $app=0 Then
	Send("!ct")
	sleep(2000)
Endif
WinWaitActive("BUSMASTER","",5)
$mName=ControlCommand ("BUSMASTER", "Name/ ID (in Dec)",1221, "IsVisible") ;checks whether the conttrol is visible or not. if visible returns 1 else 0.
$mDLC=ControlCommand ("BUSMASTER", "",1125, "IsVisible")
$mDB1=ControlCommand ("BUSMASTER", "",1126, "IsVisible")
$mDB2=ControlCommand ("BUSMASTER", "",1127, "IsVisible")
$mDB3=ControlCommand ("BUSMASTER", "",1128, "IsVisible")
$mDB4=ControlCommand ("BUSMASTER", "",1129, "IsVisible")
$mDB5=ControlCommand ("BUSMASTER", "",1130, "IsVisible")
$mDB6=ControlCommand ("BUSMASTER", "",1131, "IsVisible")
$mDB7=ControlCommand ("BUSMASTER", "",1132, "IsVisible")
$mDB8=ControlCommand ("BUSMASTER", "",1133, "IsVisible")
$mChid=ControlCommand ("BUSMASTER", "",1010, "IsVisible")
$mRadstd=ControlCommand ("BUSMASTER", "",1215, "IsVisible")
$mRadextd=ControlCommand ("BUSMASTER", "",1213, "IsVisible")
$mRTR=ControlCommand ("BUSMASTER", "",1216, "IsVisible")
$mSdetails=ControlCommand ("BUSMASTER", "Signal Details","", "IsVisible")
$mData=ControlCommand ("BUSMASTER", "","[CLASS:Static; INSTANCE:22]", "IsVisible")
$mAddmsg=ControlCommand ("BUSMASTER", "Add &Message",1122, "IsVisible")
if ($mName and $mDLC and $mDB1 and $mDB2 and $mDB3 and $mDB4 and $mDB5 and $mDB6 and $mDB7 and $mDB8)=1 then
		$abc=1
	else
		$abc=0
EndIf
if ($mChid and $mRadstd and $mRadextd and $mRTR and $mSdetails and $mData and $mAddmsg)=1 then
		$defg=1
	Else
		$defg=0
EndIf
