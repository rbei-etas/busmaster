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
	if $crashRes=1 then												; If application crash is found then update the result sheet by calling _writeCrashRes function.
	_writeCrashRes(10,10)
	EndIf
Endif
if WinWaitActive("BUSMASTER","",5) then
$Text=WinGetText("BUSMASTER","")          ;fetch the visible text info
$col1 = StringInStr($Text, "Name/ ID")    ;StringInStr Checks if a string contains a given substring and returns the postion of the substring.
$col2 = StringInStr($Text, "DLC")
$col3 = StringInStr($Text, "Data Bytes")
$col4 = StringInStr($Text, "Type")
$col5 = StringInStr($Text, "Channel")
EndIf

if $col1>0 and $col2 >0 and $col3>0 and $col4>0 and $col5>0 then   ;if postion of the string is greater than 0 then found
_ExcelWriteCell($oExcel, "OK", 10, 10)
else
_ExcelWriteCell($oExcel, "Error", 10, 10)
EndIf