; === Test Cases/Test Data ===
; Test Case Number:	TS_20_004
; Feature:			Filter
; Critical (C):		Y
; Test Cases:		Adding Filter
; Test Strategy:	Black Box
; Test Data:		-

; === Test Procedure ===
$Number = "TS_20_004"
$Result = "Test did not finish"

WinWaitActive("Configure Filter List","") 
; --- Test SetUp ---
;1. Select Add Button from left side.
ControlClick("Configure Filter List", "Add", 1228)		;1228 is the ID for Add button in Filter List groupbox.

;2. Check Filter list and filter attributes.

; A new entry will be added in to the filter list.
$CntFilterList = ControlListView("Configure Filter List", "", 1000, "GetItemCount")	;1000 is the ID for List view in Filter List groupbox.

; Filter attributes will be enabled
$EnbMsgID      = ControlCommand("Configure Filter List", "", 1001, "IsEnabled")		;1001 is the ID for Combobox in Filter Attributes groupbox.
$EnbMsgIDType  = ControlCommand("Configure Filter List", "", 1009, "IsEnabled")		;1009 is the ID for "Type" Combobox in Filter Attributes groupbox.
$EnbMsgFrame   = ControlCommand("Configure Filter List", "", 1010, "IsEnabled")		;1010 is the ID for "Frame" Combobox in Filter Attributes groupbox.
$EnbMsgDir     = ControlCommand("Configure Filter List", "", 1011, "IsEnabled")		;1011 is the ID for "Direction" Combobox in Filter Attributes groupbox.
$EnbMsgChn     = ControlCommand("Configure Filter List", "", 1012, "IsEnabled")		;1012 is the ID for "Channel" Combobox in Filter Attributes groupbox.
$EnbRadioID    = ControlCommand("Configure Filter List", "", 1253, "IsEnabled")		;1253 is the ID for "ID" Radio button in Filter Attributes groupbox.
$EnbRadioRange = ControlCommand("Configure Filter List", "", 1254, "IsEnabled")		;1254 is the ID for "Range" Radio button in Filter Attributes groupbox.
$EnbAdd        = ControlCommand("Configure Filter List", "",    3, "IsEnabled")		;   3 is the ID for "Add" button in Filter Attributes groupbox.

If $CntFilterList = 1 And $EnbMsgID = 1 And $EnbMsgIDType = 1 And $EnbMsgFrame = 1 And $EnbMsgDir = 1 And $EnbMsgChn = 1 And $EnbRadioID = 1 And $EnbRadioRange = 1 And $EnbAdd = 1 Then	
	$Result = "ok"
Else	
	$Result = "failed"
EndIf

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else		
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf
