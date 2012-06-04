;Test Case Number:	TS_17_008
;Feature:			Frame Transmission
; Critical (C):		Y
; Test Cases:		Message Block view
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===

#include <GuiHeader.au3>
Local $arr1[3], $arr2[3]
_launchApp()														; check whether the app exists or else launch it.
if $app=0 Then
	Send("!ct")
	sleep(2000)
	if $crashRes=1 then												; If application crash is found then update the result sheet by calling _writeCrashRes function.
	_writeCrashRes(13,10)
	EndIf
Endif
$arr1[0] = "Name"
$arr1[1] = "Type"
$arr1[2] = "Value"
$equal=1
WinWaitActive("BUSMASTER","",2)
$handle=ControlGetHandle("BUSMASTER","","[CLASS:SysHeader32; INSTANCE:3]")
if $handle="" Then
	_ExcelWriteCell($oExcel, "Error", 13, 10)
	_ExcelWriteCell($oExcel, "No Such list control exists", 13, 9)
	else
for $i=0 to _GUICtrlHeader_GetItemCount($handle)-1  ; Retrieves a count of the items in the header
$a= _GUICtrlHeader_GetItemText($handle, $i)         ; Retrieves the item text
$arr2[$i]=""&$a
$equal=StringCompare($arr1[$i],$arr2[$i])			; compare the actual value with expected value
if not $equal=0 then								; if not equal exit the loop
	  $equal=1
	exitloop
EndIf
next

if $equal=0 Then
	_ExcelWriteCell($oExcel, "OK", 13, 10)
Else
	_ExcelWriteCell($oExcel, "Error", 13, 10)
EndIf
EndIf
