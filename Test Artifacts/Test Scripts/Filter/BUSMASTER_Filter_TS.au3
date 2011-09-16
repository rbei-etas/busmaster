; === Test Cases/Test Data ===
; Test Case Number:	TS_20_001
; Feature:			Filter
; Critical (C):		Y
; Test Cases:		Invoking Filter Dialog
; Test Strategy:	Black Box
; Test Data:		-

; === Test Procedure ===

#include <GUIConstantsEx.au3>
#include <EditConstants.au3>
#include <WindowsConstants.au3>
#include <Array.au3>

$FileName   = "TS_20"
$Number = "TS_20_001"
$Result = "Test did not finish"
$file = FileOpen($FileName & ".txt", 2)

$SuccessCount = 0
GUICreate("BUSMASTER App Browse", 420, 100)
GUICtrlCreateLabel("Please Browse the BUSMASTER Application Executable.", 30, 10)
$h_fileIn = GUICtrlCreateEdit("", 30, 30, 300, 20, BitOR($ES_WANTRETURN, $WS_HSCROLL, $ES_AUTOHSCROLL))
$okbutton1 = GUICtrlCreateButton("Browse", 330, 30, 60,20)
$okbutton2 = GUICtrlCreateButton("Go------>", 140, 60, 60)
;GUISetState(@SW_SHOW)

$initialDir = "C:\"

While 0
  $msg = GUIGetMsg()
  Select
    Case $msg = $okbutton1
      $filepath = FileOpenDialog("Select BUSMASTER executable.", $initialDir, "Exe files (*.exe)", 1)
	  $initialDir = StringTrimRight($filepath, StringInStr($filepath, "\", "-1"))
	  GUICtrlSetData($h_fileIn, $filepath)	  
	  
    Case $msg = $okbutton2
		; 1. Start the application		
		Run($filepath)      			
		; Close the GUI
		GUIDelete(); 
		ExitLoop
		
    Case $msg = $GUI_EVENT_CLOSE
      MsgBox(0, "GUI Event", "You clicked CLOSE! Script is not executed.")
      Exit	  
  EndSelect
WEnd

MsgBox(0, "User Action-TS 1", "Run BUSMASTER Application and press OK.")
; --- Test SetUp ---
WinActivate("BUSMASTER","")

If (WinWaitActive("BUSMASTER","",5) ) Then
	
Else
	MsgBox(0, "GUI Event", "BUSMASTER not active")
	FileWriteLine($file,  "BUSMASTER application is not opened.")	
	Exit
EndIf
 
WinActivate("BUSMASTER","")
; 2. Select New Configuration
Send("!fgn") ; File->Configuration->New
Sleep(1000) ; 1 sec
if (WinActive("BUSMASTER", "Configuration File is modified")) Then
	Send("!y") ; Yes
EndIf
WinWaitActive("New Configuration Filename...", "", 2) ; wait till its open
Send("test.cfx") ; Filename
Send("!s") ; Save
if (WinWaitActive("New Configuration Filename...", "Do you want to replace it?", 2)) Then
	Send("!y") ; Yes
EndIf

WinActivate("BUSMASTER","")
; 3. Associate database
;Send("!fda") ; File -> Database -> Associate...

;WinWaitActive("Select Active Database Filename...", "BUSMASTER Datatbase File(*.dbf)", 2) ; wait till its open
;WinWaitActive("BUSMASTER","")

WinActivate("BUSMASTER","")
Send("!fda") ; File -> Database -> Associate...

WinWaitActive("Select Active Database Filename...", "BUSMASTER Datatbase File(*.dbf)", 2) ; wait till its open
Send("d:\BUSMASTER\ConfigFiles\New_Database1.DBF")
Sleep(1000)
ControlClick("Select Active Database Filename...", "","Button2")

WinActivate("BUSMASTER","")
; 4. Select Configuration -> App Filters.
Send("!ca") ; Configure->App Filters
; 5. Check for Filter Configuration dialog
WinWaitActive("Configure Filter List", "")
; --- Expected Results ---
; 1. A dialog with "Configure Filter list" will be shown
If WinActive("Configure Filter List", "") Then
	$Result = "ok"
Else
	$Result = "failed"
EndIf
; --- Finish ---
;WinClose("Configure Filter List")
;Send("!n") ; Save: No
; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else		
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf

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

; === Test Cases/Test Data ===
; Test Case Number:	TS_20_005
; Feature:			Filter
; Critical (C):		Y
; Test Cases:		Adding Filter
; Test Strategy:	Black Box
; Test Data:		-
$Number = "TS_20_005"
$Result = "Test did not finish"

GUICreate("BUSMASTER_Filter_Module_test_case_TS_20_005", 630, 80)
GUICtrlCreateLabel("Verify the test case :'The ID combo box will list all database message IDs. And will also allow the user to enter ID manually.'", 30, 10)
$okbutton1 = GUICtrlCreateButton("Success", 250, 40, 60,20)
$okbutton2 = GUICtrlCreateButton("Failure", 320, 40, 60,20)
GUISetState(@SW_SHOW)

While 1
  $msg = GUIGetMsg()

  Select
	Case $msg = $okbutton1
		$Result = "ok"
		ExitLoop
    Case $msg = $okbutton2
		$Result = "failed"		
		ExitLoop
		
    Case $msg = $GUI_EVENT_CLOSE
		$Result = "failed"				
		ExitLoop
  EndSelect
WEnd 
GUIDelete(); 

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else	
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf

; === Test Cases/Test Data ===
; Test Case Number:	TS_20_006
; Feature:			Filter
; Critical (C):		Y
; Test Cases:		Adding Filter
; Test Strategy:	Black Box
; Test Data:		-
$Number = "TS_20_006"
$Result = "Test did not finish"

; Select a DB message name from the Combo box.
ControlCommand("Configure Filter List", "", 1004, "SetCurrentSelection", 0)	

; Select Add button in 'Filter Attributes' groupbox.
ControlClick("Configure Filter List","Add", 3) ;   3 is the ID for "Add" button in Filter Attributes groupbox.

;a filter will be added to the list with the selected message name
$CntFilterDetails = ControlListView("Configure Filter List", "", 1003, "GetItemCount");1003 is the ID for List view in Filter Details groupbox.
If $CntFilterDetails > 0 Then
	$strAddedItem = ControlListView("Configure Filter List", "", 1003, "GetText", 0, 0)
	$strSelItem   = ControlCommand("Configure Filter List", "", 1004, "GetCurrentSelection")		
	Local $MsgName = StringSplit($strSelItem, "]")
	$blnResult = StringInStr($strAddedItem, $MsgName[2])
	if $blnResult > 0 Then
		$Result = "ok"
	EndIf
EndIf

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else	
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf

; === Test Cases/Test Data ===
; Test Case Number:	TS_20_007
; Feature:			Filter
; Critical (C):		Y
; Test Cases:		Adding Filter
; Test Strategy:	Black Box
; Test Data:		-
$Number = "TS_20_007"
$Result = "Test did not finish"

; Select Range radio button and give a valid range
ControlCommand("Configure Filter List", "", 1254, "Check")	;1254 is the ID for "Range" Radio button in Filter Attributes groupbox.

;Set Range as 0x10-0x15
$EnbMsgIDFrom  = ControlSetText("Configure Filter List", "", 1041, "10")	;1041 is the ID for "From" Edit control in Filter Attributes groupbox.
$EnbMsgIDTo    = ControlSetText("Configure Filter List", "", 1042, "15")	;1042 is the ID for "To" Edit Control in Filter Attributes groupbox.

; Select Add button in 'Filter Attributes' groupbox.
ControlClick("Configure Filter List","Add", 3) ;   3 is the ID for "Add" button in Filter Attributes groupbox.

;A filter will be added to the list with the selected range
$CntFilterDetails = ControlListView("Configure Filter List", "", 1003, "GetItemCount");1003 is the ID for List view in Filter Details groupbox.
If $CntFilterDetails > 0 Then
	$strRangeFrom = ControlListView("Configure Filter List", "", 1003, "GetText", 1, 0)	; Column 1 should hold the Range filter
	$strRangeTo   = ControlListView("Configure Filter List", "", 1003, "GetText", 1, 1)
	if $strRangeFrom = 10 And $strRangeTo = 15 Then
		$Result = "ok"
	EndIf
EndIf

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else	
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf

; === Test Cases/Test Data ===
; Test Case Number:	TS_20_008
; Feature:			Filter
; Critical (C):		Y
; Test Cases:		Filter Attributes
; Test Strategy:	Black Box
; Test Data:		-
$Number = "TS_20_008"
$Result = "Test did not finish"

GUICreate("BUSMASTER_Filter_Module_test_case_TS_20_008", 630, 80)
GUICtrlCreateLabel("Verify the test case :'Changing message attributes of existing filters should be allowed and warnings should be displayed.'", 30, 10)
$okbutton1 = GUICtrlCreateButton("Success", 250, 40, 60,20)
$okbutton2 = GUICtrlCreateButton("Failure", 320, 40, 60,20)
GUISetState(@SW_SHOW)

While 1
  $msg = GUIGetMsg()

  Select
	Case $msg = $okbutton1
		$Result = "ok"
		ExitLoop
    Case $msg = $okbutton2
		$Result = "failed"		
		ExitLoop
		
    Case $msg = $GUI_EVENT_CLOSE
		$Result = "failed"				
		ExitLoop
  EndSelect
WEnd 
GUIDelete(); 

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else	
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf

; === Test Cases/Test Data ===
; Test Case Number:	TS_20_009
; Feature:			Filter
; Critical (C):		Y
; Test Cases:		Filter Name and type
; Test Strategy:	Black Box
; Test Data:		-
$Number = "TS_20_009"
$Result = "Test did not finish"

GUICreate("BUSMASTER_Filter_Module_test_case_TS_20_009", 630, 80)
GUICtrlCreateLabel("Verify the test case :'Should be able to modify filter name and type by double click and editing in 'Filter list' groupbox.'", 30, 10)
$okbutton1 = GUICtrlCreateButton("Success", 250, 40, 60,20)
$okbutton2 = GUICtrlCreateButton("Failure", 320, 40, 60,20)
GUISetState(@SW_SHOW)

While 1
  $msg = GUIGetMsg()

  Select
	Case $msg = $okbutton1
		$Result = "ok"
		ExitLoop
    Case $msg = $okbutton2
		$Result = "failed"		
		ExitLoop
		
    Case $msg = $GUI_EVENT_CLOSE
		$Result = "failed"				
		ExitLoop
  EndSelect
WEnd 
GUIDelete(); 

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else	
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf

; === Test Cases/Test Data ===
; Test Case Number:	TS_20_010
; Feature:			Filter
; Critical (C):		Y
; Test Cases:		Deleting filter from filter details list
; Test Strategy:	Black Box
; Test Data:		-
$Number = "TS_20_010"
$Result = "Test did not finish"

; Select a filter in 'Filter Details' groupbox.
ControlListView("Configure Filter List", "", 1003, "Select", 0)

; Select 'Delete All' button in 'Filter Details' groupbox.
ControlClick("Configure Filter List","Delete", 1251) ;   1251 is the ID for "Delete All" button in Filter Details groupbox.

;Check for delete conformation message and select No.

If (WinWaitActive("BUSMASTER","Do you want to delete all filters from the list?")) Then	
	ControlClick("BUSMASTER","Do you want to delete all filters from the list?", 7) ; 7 is the ID for 'No' button
EndIf

; Select 'Delete All' button in 'Filter Details' groupbox.
ControlClick("Configure Filter List","Delete", 1251) ;   1251 is the ID for "Delete All" button in Filter Details groupbox.

;Check for delete conformation message and select No.

If (WinWaitActive("BUSMASTER","Do you want to delete all filters from the list?")) Then	
	ControlClick("BUSMASTER","Do you want to delete all filters from the list?", 6) ; 6 is the ID for 'Yes' button
EndIf

$CntFilterDetails = ControlListView("Configure Filter List", "", 1003, "GetItemCount");1003 is the ID for List view in Filter Details groupbox.

If $CntFilterDetails = 0 Then
	$Result = "ok"
Else
	$Result = "Failed"
EndIf

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else	
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf

; === Test Cases/Test Data ===
; Test Case Number:	TS_20_011
; Feature:			Filter
; Critical (C):		Y
; Test Cases:		Deleting filter
; Test Strategy:	Black Box
; Test Data:		-
$Number = "TS_20_011"
$Result = "Test did not finish"

; Select filter in 'Filter List' groupbox.
ControlListView("Configure Filter List", "", 1000, "Select", 0) ;1000 is the ID for List view in Filter List groupbox.

; Click 'Delete' button in 'Filter List' groupbox.
ControlClick("Configure Filter List","Delete", 1002) ;   1002 is the ID for "Delete" button in Filter List groupbox.

$CntFilterList = ControlListView("Configure Filter List", "", 1000, "GetItemCount") ;1000 is the ID for List view in Filter List groupbox.

If $CntFilterList = 0 Then
	$Result = "ok"
Else
	$Result = "Failed"
EndIf

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else	
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf

; === Test Cases/Test Data ===
; Test Case Number:	TS_20_012
; Feature:			Filter
; Critical (C):		Y
; Test Cases:		OK and Cancel functionality
; Test Strategy:	Black Box
; Test Data:		-
$Number = "TS_20_012"
$Result = "Test did not finish"

;Select Add Button from left side.
ControlClick("Configure Filter List", "Add", 1228)		;1228 is the ID for Add button in Filter List groupbox.

;Select Add Button from left side.
ControlClick("Configure Filter List", "Add", 1228)		;1228 is the ID for Add button in Filter List groupbox.

;Click 'OK' button to close the dialog.
ControlClick("Configure Filter List", "OK", 1256)		;1256 is the ID for OK button in Configure Filter List dialog.

WinWaitActive("BUSMASTER","") 
; 1. Select Configuration -> App Filters.
Send("!ca") ; Configure->App Filters.

; 2. Note down the filter list
$CntStoreFilterList = ControlListView("Configure Filter List", "", 1000, "GetItemCount") ;1000 is the ID for List view in Filter List groupbox.

; 3. Delete all filters in the filter list.

while ControlListView("Configure Filter List", "", 1000, "GetItemCount") >0
	ControlListView("Configure Filter List", "", 1000, "Select", 0)
	; Click 'Delete' button in 'Filter List' groupbox.
	ControlClick("Configure Filter List","Delete", 1002) ;   1002 is the ID for "Delete" button in Filter List groupbox.
WEnd 

; 4. Select Cancel button to close the dialog.
ControlClick("Configure Filter List", "Cancel", 2)		;2 is the ID for Cancel button in Configure Filter List dialog.

; 5. Invoke the dialog again.
WinWaitActive("BUSMASTER","") 
; 1. Select Configuration -> App Filters.
Send("!ca") ; Configure->App Filters.

; 6. Check the filter list.
if $CntStoreFilterList = ControlListView("Configure Filter List", "", 1000, "GetItemCount") Then	
	; 7. Delete all filters in the filter list.

	while ControlListView("Configure Filter List", "", 1000, "GetItemCount") >0
		ControlListView("Configure Filter List", "", 1000, "Select", 0)
		; Click 'Delete' button in 'Filter List' groupbox.
		ControlClick("Configure Filter List","Delete", 1002) ;   1002 is the ID for "Delete" button in Filter List groupbox.
	WEnd 

	; 8. Select OK.
	ControlClick("Configure Filter List", "OK", 1256)		;1256 is the ID for OK button in Configure Filter List dialog.
	
	; 9. Invoke the dialog again and check the filter list content.
	WinWaitActive("BUSMASTER","") 
	; Select Configuration -> App Filters.
	Send("!ca") ; Configure->App Filters.
	
	; the filter list should be empty.
	if ControlListView("Configure Filter List", "", 1000, "GetItemCount") = 0 Then
		$Result = "ok"		
	Else
		$Result = "Failed"
	EndIf	
Else
	$Result = "Failed"
EndIf

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else	
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf

; === Test Cases/Test Data ===
; Test Case Number:	TS_20_013
; Feature:			Filter
; Critical (C):		Y
; Test Cases:		Navigation through filter list
; Test Strategy:	Black Box
; Test Data:		-
$Number = "TS_20_013"
$Result = "Test did not finish"

;Add Filters and filter details
;Select Add Button from left side.
ControlClick("Configure Filter List", "Add", 1228)		;1228 is the ID for Add button in Filter List groupbox.

;Select Add Button from left side.
ControlClick("Configure Filter List", "Add", 1228)		;1228 is the ID for Add button in Filter List groupbox.

; Select filter1 in 'Filter List' groupbox.
ControlListView("Configure Filter List", "", 1000, "Select", 1) ;1000 is the ID for List view in Filter List groupbox

; Select a DB message name from the Combo box.
ControlCommand("Configure Filter List", "", 1004, "SetCurrentSelection", 0)	

; Select Add button in 'Filter Attributes' groupbox.
ControlClick("Configure Filter List","Add", 3) ;   3 is the ID for "Add" button in Filter Attributes groupbox.

GUICreate("BUSMASTER_Filter_Module_test_case_TS_20_013", 640, 80)
GUICtrlCreateLabel("Verify the test case :'Filter details for each entry in 'Filter List' groupbox should get updated upon selection of Filter from 'Filter List''.", 30, 10)
$okbutton1 = GUICtrlCreateButton("Success", 250, 40, 60,20)
$okbutton2 = GUICtrlCreateButton("Failure", 320, 40, 60,20)
GUISetState(@SW_SHOW)

While 1
  $msg = GUIGetMsg()

  Select
	Case $msg = $okbutton1
		$Result = "ok"
		ExitLoop
    Case $msg = $okbutton2
		$Result = "failed"		
		ExitLoop
		
    Case $msg = $GUI_EVENT_CLOSE
		$Result = "failed"				
		ExitLoop
  EndSelect
WEnd 
GUIDelete(); 

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else	
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf

; === Test Cases/Test Data ===
; Test Case Number:	TS_20_014
; Feature:			Filter
; Critical (C):		Y
; Test Cases:		display Filter
; Test Strategy:	Black Box
; Test Data:		-
$Number = "TS_20_014"
$Result = "Test did not finish"

; Select OK.
ControlClick("Configure Filter List", "OK", 1256)		;1256 is the ID for OK button in Configure Filter List dialog.
WinWaitActive("BUSMASTER","",5) 

; 1. Select a new configuration file.
Send("!fgn") ; File->Configuration->New
Sleep(1000) ; 1 sec
if (WinActive("BUSMASTER", "Configuration File is modified")) Then
	Send("!y") ; Yes
EndIf
WinWaitActive("New Configuration Filename...", "", 2) ; wait till its open
Send("test.cfx") ; Filename
Send("!s") ; Save
if (WinWaitActive("New Configuration Filename...", "Do you want to replace it?", 2)) Then
	Send("!y") ; Yestest.cfx
EndIf

; Add few filters
WinWaitActive("BUSMASTER","",5) 
Send("!ca") ; Configure->App Filters.

;Add Filters and filter details
;Select Add Button from left side.
ControlClick("Configure Filter List", "Add", 1228)		;1228 is the ID for Add button in Filter List groupbox.

;Select Add Button from left side.
ControlClick("Configure Filter List", "Add", 1228)		;1228 is the ID for Add button in Filter List groupbox.

; Select filter1 in 'Filter List' groupbox.
ControlListView("Configure Filter List", "", 1000, "Select", 1) ;1000 is the ID for List view in Filter List groupbox

; Select a DB message name from the Combo box.
ControlCommand("Configure Filter List", "", 1004, "SetCurrentSelection", 0)	

; Select Add button in 'Filter Attributes' groupbox.
ControlClick("Configure Filter List","Add", 3) ;   3 is the ID for "Add" button in Filter Attributes groupbox.

; Select OK.
ControlClick("Configure Filter List", "OK", 1256)		;1256 is the ID for OK button in Configure Filter List dialog.

WinWaitActive("BUSMASTER","",5) 
; 2. Select 'Configure -> Message Display'.
Send("!cm") 

; 3. Select Filter page.
; 4. Check the list of filters.
; Filter list should be empty as it is a new configuration.
;1226 is the ID for List control in Filter List groupbox of 'Configure Message Display- CAN' dialog.
If ControlListView("Configure Message Display- CAN", "", 1226, "GetItemCount") = 0 Then
	$Result = "ok"			
Else
	$Result = "Failed"
EndIf

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else	
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf

; === Test Cases/Test Data ===
; Test Case Number:	TS_20_015
; Feature:			Filter
; Critical (C):		Y
; Test Cases:		display Filter
; Test Strategy:	Black Box
; Test Data:		-
$Number = "TS_20_015"
$Result = "Test did not finish"

WinWaitActive("Configure Message Display- CAN","",5) 
; 2. Select Configure Button.
ControlClick("Configure Message Display- CAN", "", 1031)	;1031 is the ID for 'Configure' button in Filter tab of 'Configure Message Display- CAN' dialog.

; 3. Check for filter configuration dialog.
If WinWaitActive("Filter Selection Dialog","",5) Then
	
	; 4. Select few filters from the available list of filters and note down the selected filter list.
	ControlClick("Filter Selection Dialog", "", 32946) ;32946 is the ID for 'Add All' button in 'Filter Selection Dialog'.
	;1070 is the ID for 'Selected Filters' list in 'Filter Selection Dialog'.
	$cntFilterAdded = ControlListView("Filter Selection Dialog", "", 1070, "GetItemCount") 
	; 5. Select OK to save changes.
	ControlClick("Filter Selection Dialog", "", 1) ;1 is the ID for 'OK' button in 'Filter Selection Dialog'.
	WinWaitActive("Configure Message Display- CAN","",5) 
	; 6. Check the message filter list
	;  the modifications done in the filter configuration dialog will reflect.
	If ControlListView("Configure Message Display- CAN", "", 1226, "GetItemCount") = $cntFilterAdded Then	
		$Result = "ok"			
	Else
		$Result = "Failed"
	EndIf
Else
	$Result = "Failed"
EndIf
WinClose("Configure Message Display- CAN")

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else	
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf

; === Test Cases/Test Data ===
; Test Case Number:	TS_20_024
; Feature:			Filter
; Critical (C):		Y
; Test Cases:		Check for GUI - Message Filter
; Test Strategy:	Black Box
; Test Data:		-
$Number = "TS_20_024"
$Result = "Test did not finish"

; 1. Invoke dialog box for filter configuration
WinWaitActive("BUSMASTER","",5) 
Send("!ca") ; Configure->App Filters.


; Select Range radio button and give a valid range
ControlCommand("Configure Filter List", "", 1254, "Check")	;1254 is the ID for "Range" Radio button in Filter Attributes groupbox.
WinWaitActive("Configure Filter List","",5) 

; 2. Specify a range with hexadecimal values
;Set Range as 0xAB-0xCD
ControlSetText("Configure Filter List", "", 1041, "AB")	;1041 is the ID for "From" Edit control in Filter Attributes groupbox.
ControlSetText("Configure Filter List", "", 1042, "CD")	;1042 is the ID for "To" Edit Control in Filter Attributes groupbox.

; Hexadecimal values should be accepted.
$blnVisbFrom = ControlCommand("Configure Filter List", "", 1041, "IsVisible")	;1041 is the ID for "From" Edit control in Filter Attributes groupbox.
$blnVisbTo   = ControlCommand("Configure Filter List", "", 1042, "IsVisible")	;1042 is the ID for "To" Edit Control in Filter Attributes groupbox.


If $blnVisbFrom = 1 And $blnVisbTo = 1 Then
	$Result = "ok"						
Else
	$Result = "Failed"
EndIf
WinClose("Configure Filter List")

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else	
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf

; === Test Cases/Test Data ===
; Test Case Number:	TS_20_025
; Feature:			Filter
; Critical (C):		Y
; Test Cases:		Check for GUI - Message Filter
; Test Strategy:	Black Box
; Test Data:		-
$Number = "TS_20_025"
$Result = "Test did not finish"

; 1. Invoke dialog box for filter configuration
WinWaitActive("BUSMASTER","",5) 
Send("!ca") ; Configure->App Filters.


; Select Range radio button and give a valid range
ControlCommand("Configure Filter List", "", 1254, "Check")	;1254 is the ID for "Range" Radio button in Filter Attributes groupbox.
WinWaitActive("Configure Filter List","",5) 

; 2. Specify a range with hexadecimal values
;Set Range as 0xAB-0xCD
ControlSetText("Configure Filter List", "", 1041, "AB")	;1041 is the ID for "From" Edit control in Filter Attributes groupbox.
ControlSetText("Configure Filter List", "", 1042, "CD")	;1042 is the ID for "To" Edit Control in Filter Attributes groupbox.

; Hexadecimal values should be accepted.
$strFromRange = ControlGetText("Configure Filter List", "", 1041)	;1041 is the ID for "From" Edit control in Filter Attributes groupbox.
$strToRange   = ControlGetText("Configure Filter List", "", 1042)	;1042 is the ID for "To" Edit Control in Filter Attributes groupbox.


If $strFromRange = "AB" And $strToRange = "CD" Then
	$Result = "ok"					
Else
	$Result = "Failed"
EndIf
WinClose("Configure Filter List")

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else	
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf

; === Test Cases/Test Data ===
; Test Case Number:	TS_20_026
; Feature:			Filter
; Critical (C):		Y
; Test Cases:		Number in "From" text field should be lesser than that in "To" text field
; Test Strategy:	Black Box
; Test Data:		-
$Number = "TS_20_026"
$Result = "Test did not finish"

WinActivate("BUSMASTER","")
; 1. Invoke dialog box for filter configuration
WinWaitActive("BUSMASTER","",5) 
Send("!ca") ; Configure->App Filters.


; Select Range radio button and give a valid range
ControlCommand("Configure Filter List", "", 1254, "Check")	;1254 is the ID for "Range" Radio button in Filter Attributes groupbox.

; 2. Specify a range such that number keyed in "From" field is greater than that in "To" text field
;Set Range as 0x10-0x5
ControlSetText("Configure Filter List", "", 1041, "10")	;1041 is the ID for "From" Edit control in Filter Attributes groupbox.
ControlSetText("Configure Filter List", "", 1042, "5")	    ;1042 is the ID for "To" Edit Control in Filter Attributes groupbox.

$strWarning = ControlGetText("Configure Filter List", "", 1233)	;1233 is the ID for "Warning" Edit control in Filter Attributes groupbox.
$EnbAdd        = ControlCommand("Configure Filter List", "",    3, "IsEnabled")		;   3 is the ID for "Add" button in Filter Attributes groupbox.

; Check for the warning message display and 'Add' button enable status.
If $strWarning = "Start Range ID is greater then End Range" And $EnbAdd = "0" Then
	$Result = "ok"				
Else
	$Result = "Failed"
EndIf
WinClose("Configure Filter List")

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else	
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf

; === Test Cases/Test Data ===
; Test Case Number:	TS_20_029
; Feature:			Filter
; Critical (C):		Y
; Test Cases:		Check for Validation - Message Filter
; Test Strategy:	Black Box
; Test Data:		-
$Number = "TS_20_029"
$Result = "Test did not finish"

; 1. Invoke dialog box for filter configuration
WinWaitActive("BUSMASTER","",5) 
Send("!ca") ; Configure->App Filters.


GUICreate("BUSMASTER_Filter_Module_test_case_TS_20_029", 640, 80)
GUICtrlCreateLabel("Verify the test case :'Enter -1 in 'ID' combobox and check if the 'Invalid Message ID' waring is displayed in status text bar'.", 30, 10)
$okbutton1 = GUICtrlCreateButton("Success", 250, 40, 60,20)
$okbutton2 = GUICtrlCreateButton("Failure", 320, 40, 60,20)
GUISetState(@SW_SHOW)

While 1
  $msg = GUIGetMsg()

  Select
	Case $msg = $okbutton1
		$Result = "ok"
		ExitLoop
    Case $msg = $okbutton2
		$Result = "failed"		
		ExitLoop
		
    Case $msg = $GUI_EVENT_CLOSE
		$Result = "failed"				
		ExitLoop
  EndSelect
WEnd 
GUIDelete(); 
WinClose("Configure Filter List")

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else	
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf

; === Test Cases/Test Data ===
; Test Case Number:	TS_20_021
; Feature:			Filter
; Critical (C):		Y
; Test Cases:		Check for stop filter mode - Message filter
; Test Strategy:	Black Box
; Test Data:		-
$Number = "TS_20_021"
$Result = "Test did not finish"

;1. Invoke dialog box for filter configuration
WinActivate("BUSMASTER","") 
Send("!ca") ; Configure->App Filters.

; Delete all filters in the filter list.

while ControlListView("Configure Filter List", "", 1000, "GetItemCount") > 0
	ControlListView("Configure Filter List", "", 1000, "Select", 0)
	; Click 'Delete' button in 'Filter List' groupbox.
	ControlClick("Configure Filter List","Delete", 1002) ;   1002 is the ID for "Delete" button in Filter List groupbox.
WEnd 

;2. Specify a filter list
;3. Select stop filter mode
; Select Add Button from left side.
ControlClick("Configure Filter List", "Add", 1228)		;1228 is the ID for Add button in Filter List groupbox.

; 2. Enter  ox20 as ID of a message to update the filter list
ControlSetText("Configure Filter List", "", 1004, "20")

; Set Direction as 'All'
ControlCommand("Configure Filter List", "", 1011, "SetCurrentSelection", 2) ;1011 is the ID for "Direction" Combobox in Filter Attributes groupbox.
; Select Add button in 'Filter Attributes' groupbox.
ControlClick("Configure Filter List","Add", 3) ;   3 is the ID for "Add" button in Filter Attributes groupbox.

;4. Confirm by selecting OK button
;Click 'OK' button to close the dialog.
ControlClick("Configure Filter List", "OK", 1256)		;1256 is the ID for OK button in Configure Filter List dialog.

;5. Enable message display / logging
WinWaitActive("BUSMASTER","",5) 
; Select 'Configure -> Message Display'.
Send("!cm") 
WinWaitActive("Configure Message Display- CAN","",5) 
; Select Configure Button.
ControlClick("Configure Message Display- CAN", "", 1031)	;1031 is the ID for 'Configure' button in Filter tab of 'Configure Message Display- CAN' dialog.
WinWaitActive("Filter Selection Dialog","",5)
; Select few filters from the available list of filters and note down the selected filter list.
ControlClick("Filter Selection Dialog", "", 32946) ;32946 is the ID for 'Add All' button in 'Filter Selection Dialog'.
; Select OK to save changes.
ControlClick("Filter Selection Dialog", "", 1) ;1 is the ID for 'OK' button in 'Filter Selection Dialog'.
WinWaitActive("Configure Message Display- CAN","",5) 
ControlClick("Configure Message Display- CAN", "", 1) ;1 is the ID for 'OK' button in 'Configure Message Display- CAN' dialog.

; Add messages to transmission window.
WinWaitActive("BUSMASTER","",5) 
Send("!ct") ; Configure->Tx Messages.
WinWaitActive("BUSMASTER","Configure Transmission Messages",5) 
ControlClick("BUSMASTER","Configure Transmission Messages", 1226) ;1226 is the ID for 'Add' button in 'Configure Transmission Messages' Dialog.

; Add Hex messages ox20(32) and 0x13(19) to message list
ControlSetText("BUSMASTER","Configure Transmission Messages", 1001, "32")	;1001 is the ID for "Name / ID" ComboBox in 'Configure Transmission Messages' Dialog.
ControlClick("BUSMASTER","Configure Transmission Messages", 1122) ;1122 is the ID for 'Add Message' button in 'Configure Transmission Messages' Dialog.

; Click on Update button.
ControlClick("BUSMASTER","Configure Transmission Messages", 1014) ;1014 is the ID for 'Update' button in 'Configure Transmission Messages' Dialog.
ControlClick("BUSMASTER","Configure Transmission Messages", 1015) ;1015 is the ID for 'Close' button in 'Configure Transmission Messages' Dialog.

WinWaitActive("BUSMASTER","",5) 
; Enable display filter
Send("!ufd") ; Funtions -> Enable Filter -> Display

WinWaitActive("BUSMASTER","",5) 
; Connect 
Send("!uc")	; Functions -> Connect

WinWaitActive("BUSMASTER","") 
;6. Send some messages across CAN bus, which shall contain a few messages from the filter list"
Send("!utn")	; Functions -> Transmit -> Normal Blocks
Sleep(1000)	;Sleep for 1 second

; Stop message transmission
Send("!utn")	; Functions -> Transmit -> Normal Blocks
; Disconnect 
Send("!ui")	; Functions -> Disconnect
; Disable display filter
Send("!ufd") ; Funtions -> Enable Filter -> Display

; Message window should not contain any messages as the configured messages in Tx Window are filtered.
If ControlListView("BUSMASTER", "Message Window - CAN", 200, "GetItemCount") = 0 Then 
	$Result = "ok"			
Else
	$Result = "Failed"	
EndIf	

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else	
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf

; === Test Cases/Test Data ===
; Test Case Number:	TS_20_022
; Feature:			Filter
; Critical (C):		Y
; Test Cases:		Check for stop filter mode - Message filter
; Test Strategy:	Black Box
; Test Data:		-
$Number = "TS_20_022"
$Result = "Test did not finish"
WinActivate("BUSMASTER","") 
Send("!co") ; Configure -> Log

Sleep(100)
; Should be in shown state
if ControlCommand("Configure Logging for CAN", "","", "IsVisible") = 1 Then 	
	WinActivate("Configure Logging for CAN","")
	; Click on 'Add' log option.
	ControlClick("Configure Logging for CAN", "Add", 1303)
	; Click on 'Filter' option.
	ControlClick("Configure Logging for CAN", "Add", 1121)
	If WinWaitActive("Filter Selection Dialog","",5) Then
		; Select few filters from the available list of filters and note down the selected filter list.
		ControlClick("Filter Selection Dialog", "", 32946) ;32946 is the ID for 'Add All' button in 'Filter Selection Dialog'.
		; Store the log file path.
		$strLogFile = ControlGetText("Configure Logging for CAN", "", 1062) ;1062 is the ID for log file edit control in 'Configure logging for CAN' dialog.
		ControlClick("Filter Selection Dialog", "", 1) ;1 is the ID for 'OK' button in 'Filter Selection Dialog'.
	Else
		$Result = "Failed"
	EndIf
	ControlClick("Configure Logging for CAN", "OK",  1)	
Else
	$Result = "Failed"
EndIf

WinActivate("BUSMASTER","") 
; 4. Enable Start Logging
Send("!ul")	; Functions -> Start Logging
; Connect 
Send("!uc")	; Functions -> Connect
; Now enable display filter button in the toolbar.
; 5. Enable Log filter
Send("!ufl") ; Funtions -> Enable Filter -> Log
; 6. Send some messages across CAN bus, which shall contain a few messages from the filter list"
Send("!utn")	; Functions -> Transmit -> Normal Blocks
Sleep(1000)	;Sleep for 1 second
; Stop message transmission
Send("!utn")	; Functions -> Transmit -> Normal Blocks
; Disconnect 
Send("!ui")	; Functions -> Disconnect
; Disable Log filter
Send("!ufl") ; Funtions -> Enable Filter -> Log
; Disable Start Logging
Send("!ul")	; Functions -> Start Logging

; Open Log File
ShellExecute("notepad.exe", $strLogFile)

if MsgBox(1, "Verify Test Case: TS- 22", "Verify the log file which is opened for Msg 0x20. If it doesn't contain 0x20 press'OK' else 'Cancel'.") = 1 Then 
	$Result = "ok"	
Else	; Cancel selected
	$Result = "Failed"
EndIf

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else	
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf

; === Test Cases/Test Data ===
; Test Case Number:	TS_20_031
; Feature:			Filter
; Critical (C):		Y
; Test Cases:		Check for stop filter mode - Message filter
; Test Strategy:	Black Box
; Test Data:		-
$Number = "TS_20_031"
$Result = "Test did not finish"

;1. Invoke dialog box for filter configuration
WinWaitActive("BUSMASTER","",5) 
Send("!ca") ; Configure->App Filters.

; Delete all filters in the filter list.

while ControlListView("Configure Filter List", "", 1000, "GetItemCount") > 0
	ControlListView("Configure Filter List", "", 1000, "Select", 0)
	; Click 'Delete' button in 'Filter List' groupbox.
	ControlClick("Configure Filter List","Delete", 1002) ;   1002 is the ID for "Delete" button in Filter List groupbox.
WEnd 

;2. Specify a filter list
;3. Select stop filter mode
; Select Add Button from left side.
ControlClick("Configure Filter List", "Add", 1228)		;1228 is the ID for Add button in Filter List groupbox.

; Select Range radio button and give a valid range
ControlCommand("Configure Filter List", "", 1254, "Check")	;1254 is the ID for "Range" Radio button in Filter Attributes groupbox.
; Specify a range such that number keyed in "From" field is greater than that in "To" text field
; Set Range as 0x10-0x15
ControlSetText("Configure Filter List", "", 1041, "10")	;1041 is the ID for "From" Edit control in Filter Attributes groupbox.
ControlSetText("Configure Filter List", "", 1042, "15")	    ;1042 is the ID for "To" Edit Control in Filter Attributes groupbox.

; Set Direction as 'All'
ControlCommand("Configure Filter List", "", 1011, "SetCurrentSelection", 2) ;1011 is the ID for "Direction" Combobox in Filter Attributes groupbox.
; Select Add button in 'Filter Attributes' groupbox.
ControlClick("Configure Filter List","Add", 3) ;   3 is the ID for "Add" button in Filter Attributes groupbox.

;4. Confirm by selecting OK button
;Click 'OK' button to close the dialog.
ControlClick("Configure Filter List", "OK", 1256)		;1256 is the ID for OK button in Configure Filter List dialog.

;5. Enable message display / logging
WinWaitActive("BUSMASTER","",5) 
; Select 'Configure -> Message Display'.
Send("!cm") 
WinWaitActive("Configure Message Display- CAN","",5) 
; Select Configure Button.
ControlClick("Configure Message Display- CAN", "", 1031)	;1031 is the ID for 'Configure' button in Filter tab of 'Configure Message Display- CAN' dialog.
WinWaitActive("Filter Selection Dialog","",5)
; Select few filters from the available list of filters and note down the selected filter list.
ControlClick("Filter Selection Dialog", "", 32946) ;32946 is the ID for 'Add All' button in 'Filter Selection Dialog'.
; Select OK to save changes.
ControlClick("Filter Selection Dialog", "", 1) ;1 is the ID for 'OK' button in 'Filter Selection Dialog'.
WinWaitActive("Configure Message Display- CAN","",5) 
ControlClick("Configure Message Display- CAN", "", 1) ;1 is the ID for 'OK' button in 'Configure Message Display- CAN' dialog.

; Add messages to transmission window.
WinWaitActive("BUSMASTER","",5) 
Send("!ct") ; Configure->Tx Messages.
WinWaitActive("BUSMASTER","Configure Transmission Messages",5) 
;ControlClick("BUSMASTER","Configure Transmission Messages", 1226) ;1226 is the ID for 'Add' button in 'Configure Transmission Messages' Dialog.

; Delete existing messages if any 
ControlClick("BUSMASTER","Configure Transmission Messages", 1231) ;1015 is the ID for 'Close' button in 'Configure Transmission Messages' Dialog.
Sleep(200)
If WinActive("BUSMASTER", "Are you sure you want to delete?") Then
	Send("!y")
EndIf

; Add Hex messages ox12(18) and 0x13(19) to message list
ControlSetText("BUSMASTER","Configure Transmission Messages", 1001, "18")	;1001 is the ID for "Name / ID" ComboBox in 'Configure Transmission Messages' Dialog.
ControlClick("BUSMASTER","Configure Transmission Messages", 1122) ;1122 is the ID for 'Add Message' button in 'Configure Transmission Messages' Dialog.

ControlSetText("BUSMASTER","Configure Transmission Messages", 1001, "19")	;1001 is the ID for "Name / ID" ComboBox in 'Configure Transmission Messages' Dialog.
ControlClick("BUSMASTER","Configure Transmission Messages", 1122) ;1122 is the ID for 'Add Message' button in 'Configure Transmission Messages' Dialog.
; Click on Update button.
ControlClick("BUSMASTER","Configure Transmission Messages", 1014) ;1014 is the ID for 'Update' button in 'Configure Transmission Messages' Dialog.
ControlClick("BUSMASTER","Configure Transmission Messages", 1015) ;1015 is the ID for 'Close' button in 'Configure Transmission Messages' Dialog.

WinWaitActive("BUSMASTER","",5) 
; Enable display filter
Send("!ufd") ; Funtions -> Enable Filter -> Display

WinWaitActive("BUSMASTER","",5) 
; Connect 
Send("!uc")	; Functions -> Connect

WinWaitActive("BUSMASTER","") 
;6. Send some messages across CAN bus, which shall contain a few messages from the filter list"
Send("!utn")	; Functions -> Transmit -> Normal Blocks
Sleep(1000)	;Sleep for 1 second

; Stop message transmission
Send("!utn")	; Functions -> Transmit -> Normal Blocks
; Disconnect 
Send("!ui")	; Functions -> Disconnect
; Disable display filter
Send("!ufd") ; Funtions -> Enable Filter -> Display

; Message window should not contain any messages as the configured messages in Tx Window are filtered.
If ControlListView("BUSMASTER", "Message Window - CAN", 200, "GetItemCount") = 0 Then 
	$Result = "ok"		
Else
	$Result = "Failed"	
EndIf	

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else	
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf

; === Test Cases/Test Data ===
; Test Case Number:	TS_20_032
; Feature:			Filter
; Critical (C):		Y
; Test Cases:		Check for pass filter mode
; Test Strategy:	Black Box
; Test Data:		-
$Number = "TS_20_032"
$Result = "Test did not finish"

WinActivate("BUSMASTER","")
;1. Invoke dialog box for filter configuration
WinWaitActive("BUSMASTER","",5) 
Send("!ca") ; Configure->App Filters.

;3. Select pass filter mode
GUICreate("BUSMASTER_Filter_Module_test_case_TS_20_032", 660, 80)
GUICtrlCreateLabel("User action needed : Select 'Type' value as 'Pass' for 'Filter_1' in 'Filter List' groupbox and the close this message box by pressing 'OK'.", 30, 10)
$okbutton1 = GUICtrlCreateButton("OK", 280, 40, 60,20)
GUISetState(@SW_SHOW)

While 1
  $msg = GUIGetMsg()

  Select
	Case $msg = $okbutton1
		$Result = "ok"
		ExitLoop
		
    Case $msg = $GUI_EVENT_CLOSE
		$Result = "failed"				
		ExitLoop
  EndSelect
WEnd 
GUIDelete(); 


;4. Confirm by selecting OK button
;Click 'OK' button to close the dialog.
ControlClick("Configure Filter List", "OK", 1256)		;1256 is the ID for OK button in Configure Filter List dialog.

; Add messages to transmission window.
WinWaitActive("BUSMASTER","",5) 
Send("!ct") ; Configure->Tx Messages.
WinWaitActive("BUSMASTER","Configure Transmission Messages",5) 

; Add Hex message ox16(22) to message list
ControlSetText("BUSMASTER","Configure Transmission Messages", 1001, "22")	;1001 is the ID for "Name / ID" ComboBox in 'Configure Transmission Messages' Dialog.
ControlClick("BUSMASTER","Configure Transmission Messages", 1122) ;1122 is the ID for 'Add Message' button in 'Configure Transmission Messages' Dialog.
; Click on Update button.
ControlClick("BUSMASTER","Configure Transmission Messages", 1014) ;1014 is the ID for 'Update' button in 'Configure Transmission Messages' Dialog.
ControlClick("BUSMASTER","Configure Transmission Messages", 1015) ;1015 is the ID for 'Close' button in 'Configure Transmission Messages' Dialog.

;5. Enable message display / logging
WinWaitActive("BUSMASTER","",5) 
Send("!ufd") ; Funtions -> Enable Filter -> Display

WinWaitActive("BUSMASTER","",5) 
; Connect 
Send("!uc")	; Functions -> Connect

WinWaitActive("BUSMASTER","") 
;6. Send a some messages across CAN bus, which shall contain a few messages from the filter list"
Send("!utn")	; Functions -> Transmit -> Normal Blocks
Sleep(1000)	;Sleep for 1 second

; Stop message transmission
Send("!utn")	; Functions -> Transmit -> Normal Blocks
; Disconnect 
Send("!ui")	; Functions -> Disconnect
; Disable display filter
Send("!ufd") ; Funtions -> Enable Filter -> Display
; Message window should not contain any messages as the configured messages in Tx Window are filtered.
If ControlListView("BUSMASTER", "Message Window - CAN", 200, "GetItemCount") = 2 Then 
	$MsgID1 = ControlListView("BUSMASTER", "Message Window - CAN", 200, "GetText", 0, 5) ; 5 is the 'ID' column in message window.	
	$MsgID2 = ControlListView("BUSMASTER", "Message Window - CAN", 200, "GetText", 1, 5) ; 5 is the 'ID' column in message window.		
	If ( Number($MsgID1) > 16 And Number($MsgID1) < 21) And (Number($MsgID2) > 16 And Number($MsgID2) < 21) Then
		$Result = "ok"				
	Else
		$Result = "Failed"	
	EndIf	
Else
	$Result = "Failed"	
EndIf	

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else	
	FileWriteLine($file,  $Number & "," & $Result)
EndIf

; === Test Cases/Test Data ===
; Test Case Number:	TS_20_033
; Feature:			Filter
; Critical (C):		Y
; Test Cases:		Saving of filter mode in the configuration file
; Test Strategy:	Black Box
; Test Data:		-
$Number = "TS_20_033"
$Result = "Test did not finish"

WinActivate("BUSMASTER","")
WinClose("BUSMASTER")
ControlClick("BUSMASTER", "", 6)	;6 is the ID for 'Yes' button in Save configuration confirm message box.

MsgBox(0, "User Action-TS 33", "Restart BUSMASTER Application and press OK.")

WinActivate("BUSMASTER","") 
Send("!ca") ; Configure->App Filters.
$strFilterMode = ControlListView("Configure Filter List", "", 1000, "GetText", 0, 1) ;1000 is the ID for List view in Filter List groupbox.
WinClose("Configure Filter List")

; Check if the filter mode stored to configuration retains its filter state of 'Pass'.
If $strFilterMode = "Pass" Then
	$Result = "ok"				
Else
	$Result = "Failed"	
EndIf	

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else	
	FileWriteLine($file,  $Number & "," & $Result)
EndIf

; === Test Cases/Test Data ===
; Test Case Number:	TS_20_034
; Feature:			Filter
; Critical (C):		Y
; Test Cases:		Applying display Filter
; Test Strategy:	Black Box
; Test Data:		-
$Number = "TS_20_034"
$Result = "Test did not finish"

; 3. Disable display filtering if it is enabled.
; Already in disabled state.

WinActivate("BUSMASTER","") 
; Connect 
Send("!uc")	; Functions -> Connect

WinWaitActive("BUSMASTER","") 
;6. Send some messages across CAN bus, which shall contain a few messages from the filter list"
Send("!utn")	; Functions -> Transmit -> Normal Blocks
Sleep(1000)	;Sleep for 1 second

; Stop message transmission
Send("!utn")	; Functions -> Transmit -> Normal Blocks
; Disconnect 
Send("!ui")	; Functions -> Disconnect

; Message window should contain all 3 messages as configured messages in Tx Window.
If ControlListView("BUSMASTER", "Message Window - CAN", 200, "GetItemCount") = 3 Then 
	$Result = "ok"		
	; Getting column values in Msg wnd not working..Need to work on this logic.
	;$MsgID1 = ControlListView("BUSMASTER", "Message Window - CAN", 200, "GetText", 0, 5) ; 5 is the 'ID' column in message window.	
	;$MsgID2 = ControlListView("BUSMASTER", "Message Window - CAN", 200, "GetText", 1, 5) ; 5 is the 'ID' column in message window.	
	;$MsgID3 = ControlListView("BUSMASTER", "Message Window - CAN", 200, "GetText", 2, 5) ; 5 is the 'ID' column in message window.	
	;If (Number($MsgID1) <= 16 Or Number($MsgID1) >= 21) Or (Number($MsgID2) <= 16 Or Number($MsgID2) >= 21) Or (Number($MsgID1) <= 16 Or Number($MsgID1) >= 21) Then
	;	$Result = "ok"			
	;Else
	;	$Result = "Failed"	
	;EndIf	
Else
	$Result = "Failed"	
EndIf	

WinActivate("BUSMASTER","") 
; Connect 
Send("!uc")	; Functions -> Connect
; 6. Now enable display filter button in the toolbar.
; Enable display filter
Send("!ufd") ; Funtions -> Enable Filter -> Display
; Send some messages across CAN bus, which shall contain a few messages from the filter list"
Send("!utn")	; Functions -> Transmit -> Normal Blocks
Sleep(1000)	;Sleep for 1 second
; Stop message transmission
Send("!utn")	; Functions -> Transmit -> Normal Blocks
; Disconnect 
Send("!ui")	; Functions -> Disconnect
; Disable display filter
Send("!ufd") ; Funtions -> Enable Filter -> Display


; Message window should contain 2 messages as configured messages in Tx Window which are in 'Pass' range of 0x10 - 0x15.
If ControlListView("BUSMASTER", "Message Window - CAN", 200, "GetItemCount") = 2 Then 
	If $Result = "ok" Then
		$Result = "ok"			
	EndIf
Else
	$Result = "Failed"	
EndIf

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else	
	FileWriteLine($file,  $Number & "," & $Result)
EndIf

; --- Write Summary ---
FileWriteLine($file,  $SuccessCount& " out of " & "25 test cases successful.")	
FileClose($file)

; --- Show Summary ---
$strRunPath = "notepad.exe " & @ScriptDir & "\" & $FileName & ".txt"
Run($strRunPath)

; --- Closure ---
;WinClose("BUSMASTER")
ControlClick("BUSMASTER", "", 7)	;7 is the ID for 'No' button


;End of Script
