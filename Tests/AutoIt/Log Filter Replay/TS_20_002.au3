; === Test Cases/Test Data ===
; Test Case Number:	TS_20_002
; Feature:			Filter
; Critical (C):		Y
; Test Cases:		Filter Configuration dialog
; Test Strategy:	Black Box
; Test Data:		-

; === Test Procedure ===
$Number = "TS_20_002"
$Result = "Test did not finish"
; --- Test SetUp ---
;1. In filter configuration dialog, check following fields
;       Filter List
;       Filter details and
;       Filter attributes
$Text1 = ControlGetText("Configure Filter List", "", "Button10")	;ClassnameNN for Filter Attributes
$Text2 = ControlGetText("Configure Filter List", "", "Button11")	;ClassnameNN for Filter List
$Text3 = ControlGetText("Configure Filter List", "", "Button12")  	;ClassnameNN for Filter Details

;compare the group names values
if $Text1 = "Filter Attributes" And $Text2 = "Filter List" And $Text3 = "Filter Details" Then
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
