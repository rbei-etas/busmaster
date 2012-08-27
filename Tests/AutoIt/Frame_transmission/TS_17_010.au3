;Test Case Number:	TS_17_010
;Feature:			Frame Transmission
; Critical (C):		Y
; Test Cases:		Splitter and Resize
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===

_launchApp()														; check whether the app exists or else launch it.
if $app=0 Then
	Send("!ct")
	sleep(2000)
Endif
WinWaitActive("BUSMASTER","",2)
$pos=ControlGetPos("BUSMASTER", "", 59664) 					 		;fetch the position

$x=$pos[0]
$y=$pos[1]
$drag=MouseClickDrag( "left", $x+30, $y+45,$x+30,$y-100,20) 		;perform drag operation
$pos1=ControlGetPos("BUSMASTER", "", 59649)
$x1=$pos[0]
$y1=$pos[1]
$drag1=MouseClickDrag( "left", $x1-5, $y1+50,$x1+30,$y1+50,20)


