;Test Case Number:	TS_17_005
;Feature:			Frame Transmission
; Critical (C):		Y
; Test Cases:		Tx Message List control GUI check
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===

_launchApp()														; check whether the app exists or else launch it.
if $app=0 Then
	Send("!ct")
	sleep(2000)
Endif
if WinWaitActive("BUSMASTER","",5) then
$Text=WinGetText("BUSMASTER","")          ;fetch the visible text info
$col1 = StringInStr($Text, "Name/ ID")    ;StringInStr Checks if a string contains a given substring and returns the postion of the substring.
$col2 = StringInStr($Text, "DLC")
$col3 = StringInStr($Text, "Data Bytes")
$col4 = StringInStr($Text, "Type")
$col5 = StringInStr($Text, "Channel")
EndIf