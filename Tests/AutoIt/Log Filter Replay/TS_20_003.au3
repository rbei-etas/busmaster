; === Test Cases/Test Data ===
; Test Case Number:	TS_20_003
; Feature:			Filter
; Critical (C):		Y
; Test Cases:		Filter Configuration dialog
; Test Strategy:	Black Box
; Test Data:		-

; === Test Procedure ===
$Number = "TS_20_003"
$Result = "Test did not finish"

WinWaitActive("Configure Filter List","") 
; --- Test SetUp ---
;1. Check the content of the filter list, Filter details list and filter attributes.
$CntFilterList = ControlListView("Configure Filter List", "", 1000, "GetItemCount")	;1000 is the ID for List view in Filter List groupbox.
$CntFilterDetails = ControlListView("Configure Filter List", "", 1003, "GetItemCount")	;1003 is the ID for List view in Filter Details groupbox.

$EnbMsgID      = ControlCommand("Configure Filter List", "", 1004, "IsEnabled")		;1004 is the ID for Combobox in Filter Attributes groupbox.
$EnbMsgIDFrom  = ControlCommand("Configure Filter List", "", 1041, "IsEnabled")		;1041 is the ID for "From" Edit control in Filter Attributes groupbox.
$EnbMsgIDTo    = ControlCommand("Configure Filter List", "", 1042, "IsEnabled")		;1042 is the ID for "To" Edit Control in Filter Attributes groupbox.
$EnbMsgIDType  = ControlCommand("Configure Filter List", "", 1009, "IsEnabled")		;1009 is the ID for "Type" Combobox in Filter Attributes groupbox.
$EnbMsgFrame   = ControlCommand("Configure Filter List", "", 1010, "IsEnabled")		;1010 is the ID for "Frame" Combobox in Filter Attributes groupbox.
$EnbMsgDir     = ControlCommand("Configure Filter List", "", 1011, "IsEnabled")		;1011 is the ID for "Direction" Combobox in Filter Attributes groupbox.
$EnbMsgChn     = ControlCommand("Configure Filter List", "", 1012, "IsEnabled")		;1012 is the ID for "Channel" Combobox in Filter Attributes groupbox.
$EnbRadioID    = ControlCommand("Configure Filter List", "", 1253, "IsEnabled")		;1253 is the ID for "ID" Radio button in Filter Attributes groupbox.
$EnbRadioRange = ControlCommand("Configure Filter List", "", 1254, "IsEnabled")		;1254 is the ID for "Range" Radio button in Filter Attributes groupbox.
$EnbAdd        = ControlCommand("Configure Filter List", "",    3, "IsEnabled")		;   3 is the ID for "Add" button in Filter Attributes groupbox.

If $CntFilterList > 0 Or $CntFilterDetails > 0 Or $EnbMsgID = 1 Or $EnbMsgIDFrom = 1 Or $EnbMsgIDTo = 1 Or $EnbMsgIDType = 1 Or $EnbMsgFrame = 1 Or $EnbMsgDir = 1 Or $EnbMsgChn = 1 Or $EnbRadioID = 1 Or $EnbRadioRange = 1 Or $EnbAdd = 1 Then	
	$Result = "failed"		
Else
	$Result = "ok"
EndIf

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else	
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf
