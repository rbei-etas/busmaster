; === Test Cases/Test Data ===
; Test Case Number:	TS_XX_037
; Feature:			Menu Item functionalities
; Critical (C):		Y
; Test Cases:		Functions -> Test Automation -> Editor
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
#include<globalFunc.au3>
#Include <GuiTreeView.au3>
#Include <GuiToolBar.au3>

$Number = "TSEditor"
$Result = "Test did not finish"

; --- Test SetUp ---
WinActivate("BUSMASTER",3)
WinActivate("BUSMASTER","")

;load dbf file
;~ _loadDatabase("TestAutoCov")

;~ ; Save the DB
;~ send("!fds")

;~ ; Close the DB
;~ Send("!fdc")

; associate dbf
;~ _associateDB("&File", "TestAutoCov.dbf")
;~ Sleep(2000)

; Enable Filter for Logging
Send("!uae")	; Functions -> Test Automation -> Editor

; Should be in shown state
;~ if MsgBox(1, "Verify Test Automation Editor - Editing Module", "If Test Automation Editor window appears, close it and press 'OK' else press 'Cancel'.") = 1 Then
;~ 	$Result = "ok"
;~ Else	; Cancel selected
;~ 	$Result = "Failed"
;~ EndIf

Send("!fo");
Sleep(1000) ; 0.5 sec


if WinWaitActive("Select A TestSetup File", "", 2) Then ; wait till its open
	;$Result = "ok"
	Send(@ScriptDir & "\TestAutomationEditing.xml")
	;Send("c:\AutoIT_Input\TestAutoit.xml")
	Sleep(250)
	ControlClick("Select A TestSetup File", "","Button2")
Else
	$Result = "Failed"
EndIf

if WinWaitActive("Error","", 2) Then
	ControlClick("Error", "","Button1")
	Exit
EndIf

Sleep(500) ; 0.5 sec

Send("!fv");

sleep(1000)

if WinWaitActive("Validation Success", "", 2) Then ; wait till its open
	;$Result = "ok"
	ControlClick("Validation Success", "","Button1")
Elseif winexists("Validation Failed") Then
	ControlClick("Validation Failed", "",2)
EndIf

Send("!fa")


if WinWaitActive("Save As", "", 2) Then ; wait till its open
	;$Result = "ok"
	Send(@ScriptDir & "\TestAutomationEditing_Copy.xml")
	;Send("c:\AutoIT_Input\TestAutoit_Copy.xml")
	ControlClick("Save As", "","Button2")
Else
	$Result = "Failed"
EndIf

if WinWaitActive("Save As", "", 2) Then ; wait till its open
	;$Result = "ok"
	ControlClick("Save As", "","Button1")
EndIf

; Setting Dialog
Send("!dt")
if WinWaitActive("Settings", "", 2) Then ; wait till its open
	;$Result = "ok"
	ControlClick("Settings", "","Button11")
Else
	$Result = "Failed"
EndIf

	ControlClick("Settings", "","Button9")


if WinWaitActive("Color", "", 2) Then ; wait till its open
	ControlClick("Color", "","Button2")
Else
	$Result = "Failed"
EndIf

	ControlClick("Settings", "","Button10")


if WinWaitActive("Color", "", 2) Then ; wait till its open
	ControlClick("Color", "","Button2")
Else
	$Result = "Failed"
EndIf

	ControlClick("Settings", "","Button3")


if WinWaitActive("Color", "", 2) Then ; wait till its open
	ControlClick("Color", "","Button2")
Else
	$Result = "Failed"
EndIf

	ControlClick("Settings", "","Button4")


if WinWaitActive("Color", "", 2) Then ; wait till its open
	ControlClick("Color", "","Button2")
Else
	$Result = "Failed"
EndIf

	;Ok
	ControlClick("Settings", "", "Button7");

 ;Setting Dialog
Send("!dt")
if WinWaitActive("Settings", "", 2) Then ; wait till its open
	;$Result = "ok"
	ControlClick("Settings", "","Button11")
	ControlClick("Settings", "", "Button6");
Else
	$Result = "Failed"
EndIf

	;Ok
	ControlClick("Settings", "", "Button7");
sleep(2000)

; Create new xml
send("!fn")
sleep(1000)
if winexists("BUSMASTER TestSetup Editor") Then
	controlclick("BUSMASTER TestSetup Editor","",6)
EndIf
sleep(2000)
if winexists("New TestSetup File") Then
	sleep(1000)

	; enter the file name
	send(@ScriptDir&"\NewTestEditorFile")

	; Click on 'Save' button
	send("{ENTER}")
EndIf


; open settings window
send("!dt")
sleep(2000)

; check if 'Save Changes Automatically' is enabled. If not enable it
if (ControlCommand("BUSMASTER","",3011,"IsChecked")=0) Then
	ControlClick("BUSMASTER","",3011)
EndIf

; Close the "Settings" window
ControlClick("Settings","",1)

; Get handle of tree view to add a message
$TAEditHWD = ControlGetHandle("BUSMASTER","","[CLASS:SysTreeView32; INSTANCE:1]")

; Fetch Parent node handle
$TAEdittreeVwparent = _GUICtrlTreeView_GetFirstItem($TAEditHWD)

; Click on the parent node
_GUICtrlTreeView_ClickItem($TAEditHWD, $TAEdittreeVwparent,"","",2)
sleep(2000)
;~ send("Sampletitle")

; Get handle of the 'Details' listview
$hWnd = ControlGetHandle("BUSMASTER","","[CLASS:SysListView32; INSTANCE:1]")

; enter 'title'
_GUICtrlListView_SetItemText($hWnd,0,"Sampletitle",1)


; enter 'description'
_GUICtrlListView_SetItemText($hWnd,1,"SampleDesc",1)


; enter 'module name'
_GUICtrlListView_SetItemText($hWnd,4,"TestMod1",1)


; enter 'Engineer Name'
_GUICtrlListView_SetItemText($hWnd,5,"ABC",1)


; enter 'Engineer Role'
_GUICtrlListView_SetItemText($hWnd,6,"SSE",1)


_GUICtrlListView_ClickItem($hWnd,0)

; enter 'DB Path'
$hWnd = ControlGetHandle("BUSMASTER","","[CLASS:SysListView32; INSTANCE:1]")
$TAEmsgDetPos=_GUICtrlListView_GetItemPosition($hWnd, 0)
ControlClick("BUSMASTER","","","Left",2,$TAEmsgDetPos[0]+300,$TAEmsgDetPos[1]+120)
controlclick("BUSMASTER","","[CLASS:Button; INSTANCE:1]")
sleep(2000)
if winexists("Open") then
	send(@ScriptDir&"\AutoitTest.dbf")
	send("{ENTER}")
EndIf
;~ send("D:\Code Coverage\1.6.6\TestAutomaion_Editor_Lib\AutoitTest.dbf")
send("{ENTER}")
sleep(1500)
if winexists("Database Path") Then
	controlclick("Database Path","",6)
EndIf

;~ send("{ENTER}")

; enter 'Report File Path'
_GUICtrlListView_SetItemText($hWnd,8,"D:\Code Coverage\1.6.6\TestAutomaion_Editor_Lib\report.txt",1)
sleep(1000)

;click on 'Confirm' Button
controlclick("BUSMASTER","",3004)
sleep(1500)
; save the xml
send("!fs")
sleep(1000)

; Get handle of tree view to add a message
$TAEditHWD = ControlGetHandle("BUSMASTER","","[CLASS:SysTreeView32; INSTANCE:1]")

; Fetch Parent node handle
$TAEdittreeVwparent = _GUICtrlTreeView_GetFirstItem($TAEditHWD)

; Click on the parent node
_GUICtrlTreeView_ClickItem($TAEditHWD, $TAEdittreeVwparent,"Right")
sleep(1500)

; select 'new testcase' from right click menu
send("n")

; expand the tree view
_GUICtrlTreeView_Expand($TAEditHWD,0)

send("{DOWN}")

; enter 'TestCase ID'
_GUICtrlListView_SetItemText($hWnd,0,001,1)
sleep(500)

_GUICtrlListView_SetItemText($hWnd,1,"TestCase 1",1)
sleep(500)

_GUICtrlListView_SetItemText($hWnd,2,"EXIT",1)
sleep(500)

;click on 'Confirm' Button
controlclick("BUSMASTER","",3004)
sleep(1500)

; save the xml
send("!fs")
sleep(1000)

; Get handle of tree view to add a message
$TAEditHWD = ControlGetHandle("BUSMASTER", "", "[CLASS:SysTreeView32; INSTANCE:1]")

; Fetch First node handle
$TAEdittreeVwparent = _GUICtrlTreeView_GetFirstItem($TAEditHWD)

; Fetch handle of first node's child item
$tc1HWD = _GUICtrlTreeView_GetFirstChild($TAEditHWD, $TAEdittreeVwparent)

; Right click on first node's child item
_GUICtrlTreeView_ClickItem($TAEditHWD, $tc1HWD, "Right")

; select 'New'
Send("n") ; Add a new handler
Sleep(500)

; select 'Send'
Send("s")
sleep(500)

; expand the tree view
_GUICtrlTreeView_Expand($TAEditHWD,0)

send("{DOWN}")

_GUICtrlListView_ClickItem($hWnd,0)

; Get position of Signal details list view control
$TAEmsgDetPos=_GUICtrlListView_GetItemPosition($hWnd, 0)

; Double click to select msgs from dropdown list
ControlClick("BUSMASTER","","","Left",2,$TAEmsgDetPos[0]+300,$TAEmsgDetPos[1]+20)
sleep(500)
send("{DOWN}")
sleep(500)
send("{DOWN}")
sleep(500)
send("{ENTER}")

; Double click to select msgs from dropdown list
ControlClick("BUSMASTER","","","Left",2,$TAEmsgDetPos[0]+300,$TAEmsgDetPos[1]+40)
sleep(500)
send("{DOWN}")
sleep(500)
send("{DOWN}")
sleep(500)
send("{DOWN}")
sleep(500)
send("{ENTER}")

;click on 'Confirm' Button
controlclick("BUSMASTER","",3004)
sleep(1500)

; save the xml
send("!fs")
sleep(1000)

; expand the tree view
_GUICtrlTreeView_Expand($TAEditHWD,0)
sleep(1000)

; Get handle of tree view to add a message
$TAEditHWD = ControlGetHandle("BUSMASTER", "", "[CLASS:SysTreeView32; INSTANCE:1]")

; Fetch First node handle
$TAEdittreeVwparent = _GUICtrlTreeView_GetFirstItem($TAEditHWD)

; Fetch handle of first node's child item
$tc1HWD = _GUICtrlTreeView_GetFirstChild($TAEditHWD, $TAEdittreeVwparent)

; Right click on first node's child item
_GUICtrlTreeView_ClickItem($TAEditHWD, $tc1HWD)

send("{DOWN 2}")

_GUICtrlListView_ClickItem($hWnd,0)

$TAEmsgDetPos=_GUICtrlListView_GetItemPosition($hWnd, 0)										; Get position of Signal details list view control
ControlClick("BUSMASTER","","","Left",2,$TAEmsgDetPos[0]+300,$TAEmsgDetPos[1]+20)
sleep(500)
send("5")
send("{ENTER}")

ControlClick("BUSMASTER","","","Left",2,$TAEmsgDetPos[0]+300,$TAEmsgDetPos[1]+40)
sleep(500)
send("6")
send("{ENTER}")

; Get handle of tree view to add a message
$TAEditHWD = ControlGetHandle("BUSMASTER","","[CLASS:SysTreeView32; INSTANCE:1]")

; Fetch Parent node handle
$TAEdittreeVwparent = _GUICtrlTreeView_GetFirstItem($TAEditHWD)

; Click on the parent node
_GUICtrlTreeView_ClickItem($TAEditHWD, $TAEdittreeVwparent)
sleep(2000)

; press down key four times to select the second msg
send("{DOWN 4}")

_GUICtrlListView_ClickItem($hWnd,0)

ControlClick("BUSMASTER","","","Left",2,$TAEmsgDetPos[0]+300,$TAEmsgDetPos[1]+10)
sleep(500)

; select RAW
send("{UP}")
send("{ENTER}")

ControlClick("BUSMASTER","","","Left",2,$TAEmsgDetPos[0]+300,$TAEmsgDetPos[1]+20)
sleep(500)
send("15")
send("{ENTER}")

ControlClick("BUSMASTER","","","Left",2,$TAEmsgDetPos[0]+300,$TAEmsgDetPos[1]+40)
sleep(500)
send("16")
send("{ENTER}")

;click on 'Confirm' Button
controlclick("BUSMASTER","",3004)
sleep(1500)

; save the xml
send("!fs")
sleep(1000)

; Get handle of tree view to add a message
$TAEditHWD = ControlGetHandle("BUSMASTER", "", "[CLASS:SysTreeView32; INSTANCE:1]")

; Fetch First node handle
$TAEdittreeVwparent = _GUICtrlTreeView_GetFirstItem($TAEditHWD)

; Fetch handle of first node's child item
$tc1HWD = _GUICtrlTreeView_GetFirstChild($TAEditHWD, $TAEdittreeVwparent)

; Right click on first node's child item
_GUICtrlTreeView_ClickItem($TAEditHWD, $tc1HWD, "Right")

; select 'New'
Send("n") ; Add a new handler
Sleep(500)

; select 'Verify'
Send("v")
sleep(500)

send("{ENTER}")

; press down key four times to select the Verify node
send("{DOWN 4}")

_GUICtrlListView_ClickItem($hWnd,0)

; Double click to select'failure classification
ControlClick("BUSMASTER","","","Left",2,$TAEmsgDetPos[0]+300,$TAEmsgDetPos[1]+20)
sleep(500)

send("{ESC}")

; Double click to select first msg from dropdown list
ControlClick("BUSMASTER","","","Left",2,$TAEmsgDetPos[0]+300,$TAEmsgDetPos[1]+40)
sleep(500)
send("{DOWN}")
sleep(500)
send("{DOWN}")
sleep(500)
send("{ENTER}")

; Double click to select second msg from dropdown list
ControlClick("BUSMASTER","","","Left",2,$TAEmsgDetPos[0]+300,$TAEmsgDetPos[1]+60)		; Double click on the 'Filter Type'
sleep(500)
send("{DOWN}")
sleep(500)
send("{DOWN}")
sleep(500)
send("{DOWN}")
sleep(500)
send("{ENTER}")

;click on 'Confirm' Button
controlclick("BUSMASTER","",3004)
sleep(1500)

; save the xml
send("!fs")
sleep(1000)

; expand the tree view
_GUICtrlTreeView_Expand($TAEditHWD,0)

ControlFocus("BUSMASTER","","[CLASS:SysTreeView32; INSTANCE:1]")
sleep(500)

; Click on first msg of Verify node
send("{DOWN}")

_GUICtrlListView_ClickItem($hWnd,0)

$TAEmsgDetPos=_GUICtrlListView_GetItemPosition($hWnd, 0)

; Double click to select'failure classification
ControlClick("BUSMASTER","","","Left",2,$TAEmsgDetPos[0]+300,$TAEmsgDetPos[1]+10)
sleep(500)

send("{DOWN}")
send("{ENTER}")

; Double click to select first msg from dropdown list
ControlClick("BUSMASTER","","","Left",2,$TAEmsgDetPos[0]+300,$TAEmsgDetPos[1]+20)
sleep(500)
send("5")
sleep(500)
send("{ENTER}")

; Double click to select second msg from dropdown list
ControlClick("BUSMASTER","","","Left",2,$TAEmsgDetPos[0]+300,$TAEmsgDetPos[1]+40)		; Double click on the 'Filter Type'
sleep(500)
send("6")
sleep(500)
send("{ENTER}")

ControlFocus("BUSMASTER","","[CLASS:SysTreeView32; INSTANCE:1]")
sleep(500)

send("{DOWN}")
sleep(500)

_GUICtrlListView_ClickItem($hWnd,0)

; Double click to select first msg from dropdown list
ControlClick("BUSMASTER","","","Left",2,$TAEmsgDetPos[0]+300,$TAEmsgDetPos[1]+20)
sleep(500)
send("15")
sleep(500)
send("{ENTER}")

; Double click to select second msg from dropdown list
ControlClick("BUSMASTER","","","Left",2,$TAEmsgDetPos[0]+300,$TAEmsgDetPos[1]+40)		; Double click on the 'Filter Type'
sleep(500)
send("16")
sleep(500)
send("{ENTER}")

;click on 'Confirm' Button
controlclick("BUSMASTER","",3004)
sleep(1500)

; save the xml
send("!fs")
sleep(1000)

; Validate the xml
Send("!fv");

sleep(1000)

if WinWaitActive("Validation Success", "", 2) Then ; wait till its open
	;$Result = "ok"
	ControlClick("Validation Success", "","Button1")
Elseif winexists("Validation Failed") Then
	ControlClick("Validation Failed", "",2)
EndIf


; Close the test automation editor window
send("^{F4}")

; select Functions->test automation->Executor
send("!uax")

; Get handle of tree view to add a message
$TAExeHWD = ControlGetHandle("BUSMASTER", "", "[CLASS:SysTreeView32; INSTANCE:1]")

; Get handle of parent node
$TAExetreeVwparent=_GUICtrlTreeView_GetFirstItem($TAExeHWD)

; Fetch Child node handle
$TAExetreeChild = _GUICtrlTreeView_GetNext($TAExeHWD,"")

; Right click on first node's child item
_GUICtrlTreeView_ClickItem($TAExeHWD, $TAExetreeVwparent, "Right")

; select 'Add' from right click menu
send("a")
sleep(2000)

if winexists("Open") Then
	sleep(1000)

	; enter the file name
	send(@ScriptDir&"\NewTestEditorFile.xml")

	; Click on 'Save' button
	send("{ENTER}")
EndIf
sleep(2000)

; Get handle of tree view
$TAExeHWD = ControlGetHandle("BUSMASTER", "", "[CLASS:SysTreeView32; INSTANCE:1]")

; expand the tree view
_GUICtrlTreeView_Expand($TAExeHWD,0)

;~ sleep(2000)
;~ ; Get handle of tree view to add a message
;~ $TAExeHWD = ControlGetHandle("BUSMASTER", "", "[CLASS:SysTreeView32; INSTANCE:1]")

;~ ; Get handle of parent node
;~ $TAExetreeVwparent=_GUICtrlTreeView_GetFirstItem($TAExeHWD)

;~ ; Check the parent node
;~ _GUICtrlTreeView_SetChecked($TAExeHWD,$TAExetreeVwparent)

;~ ; Fetch Child node handle
;~ $TAExetreeChild = _GUICtrlTreeView_GetNext($TAExeHWD,"")

;~ ; Check the Child node
;~ _GUICtrlTreeView_SetChecked($TAExeHWD,$TAExetreeChild)

;~ ; Get handle of the item in the child node
;~ $TAExesib=_GUICtrlTreeView_GetNextVisible($TAExeHWD,$TAExetreeChild)

;~ ; Check the item in the child node
;~ _GUICtrlTreeView_SetChecked($TAExeHWD,$TAExesib)

; Get handle of tollbar
$cntToolhWd=ControlGetHandle("BUSMASTER","",128)

$RetVal=msgbox(0,"","Please Check the three checkbox in the Test Suite Executor and click 'OK'")

if $RetVal=1 then

	; Click on 'Connect' icon
	_GUICtrlToolbar_ClickIndex($cntToolhWd,4)
	sleep(1000)

	; Get handle of tree view to add a message
	$TAExeHWD = ControlGetHandle("BUSMASTER", "", "[CLASS:SysTreeView32; INSTANCE:1]")

	; Get handle of parent node
	$TAExetreeVwparent=_GUICtrlTreeView_GetFirstItem($TAExeHWD)

	; Fetch Child node handle
	$TAExetreeChild = _GUICtrlTreeView_GetNext($TAExeHWD,"")

	; Right click on first node's child item
	_GUICtrlTreeView_ClickItem($TAExeHWD, $TAExetreeVwparent, "Right")

	; select 'Execute' from right click menu
	send("e")
	sleep(3000)

	; Click on 'Disconnect' icon
	_GUICtrlToolbar_ClickIndex($cntToolhWd,4)
	sleep(1000)

send("!fgs")
EndIf



