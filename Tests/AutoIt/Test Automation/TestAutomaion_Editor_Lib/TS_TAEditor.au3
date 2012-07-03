; === Test Cases/Test Data ===
; Test Case Number:	TS_XX_037
; Feature:			Menu Item functionalities
; Critical (C):		Y
; Test Cases:		Functions -> Test Automation -> Editor
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
#Include <GuiTreeView.au3>
$Number = "TSEditor"
$Result = "Test did not finish"

; --- Test SetUp ---
WinActivate("BUSMASTER","")

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

if WinWaitActive("Validation Success", "", 2) Then ; wait till its open
	;$Result = "ok"		
	ControlClick("Validation Success", "","Button1")	
Else
	$Result = "Failed"
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



$pos=ControlGetPos ("BUSMASTER", "", "[CLASS:SysTreeView32; INSTANCE:1]" )
$hWd=controlgethandle("BUSMASTER","","[CLASS:SysTreeView32; INSTANCE:1]")
ConsoleWrite($hWd&@CRLF)
$res=_GUICtrlTreeView_Expand($hWd,0)
for $i=0 to 28
	send("{DOWN}")
Next

Send("!dr")
;Expand
MouseClick("left",$pos[0]+30,$pos[1]+55,2)	
$res=_GUICtrlTreeView_Expand($hWd,0)
send("{DOWN}")	;TestCase1
send("{DOWN}")	;send

;send copy
;--------------------------------------
Send("!ec") ;copy

send("{UP}") ;TestCase1

send("!ep");
;--------------------------------------

;wait copy
send("{DOWN}")	;send
send("{DOWN}")	;wait

Send("!ec") ;copy

send("{UP}") ;
send("{UP}") ;

send("!ep");


;verify copy
send("{DOWN}")	;send
send("{DOWN}")	;wait
send("{DOWN}")	;verify

Send("!ec") ;copy

send("{UP}") ;
send("{UP}") ;
send("{UP}") ;

send("!ep");


;verfiy Response copy
send("{DOWN}")	;send
send("{DOWN}")	;wait
send("{DOWN}")	;verify
send("{DOWN}")	;verify response

Send("!ec") ;copy

send("{UP}") ;
send("{UP}") ;
send("{UP}") ;
send("{UP}") ;

send("!ep");

;Now Deleting
;Delete second send
send("{DOWN}")	;send
send("{DOWN}")	;send
send("{DOWN}")	;wait
send("{DOWN}")	;verify
send("{DOWN}")	;second send

send("{Delete}") ;second send
send("{Delete}") ;second wait
send("{Delete}") ;second verify
send("{Delete}") ;second verify response

;Now Cut Paste
;cut send
send("{DOWN}")	;send
send("!et")
send("{UP}")	;TestCase
send("!ep")

;cut send
send("{DOWN}")	;wait
send("!et")
send("{UP}")	;TestCase
send("!ep")


;cut send
send("{DOWN}")	;verify
send("!et")
send("{UP}")	;TestCase
send("!ep")

;cut send
send("{DOWN}")	;verify response
send("!et")
send("{UP}")	;TestCase
send("!ep")

;help
send("!hi")
if WinWaitActive("BUSMASTER", "", 2) Then ; wait till its open
	;$Result = "ok"		
	ControlClick("BUSMASTER", "","Button1")
Else
	$Result = "Failed"
EndIf




;-------------------------------------------------
;Testcase Change
$poslist=ControlGetPos ("BUSMASTER", "", "[CLASS:SysListView32; INSTANCE:1]")
MouseClick("left", $poslist[0]+160, $poslist[1]+110,2)
Send("{DOWN}")
sEND("{ENTER}")

;Again Main Tree
MouseClick("left",$pos[0]+30,$pos[1]+55,2)	
Send("{DOWM}")
$res=_GUICtrlTreeView_Expand($hWd,0)
Send("{DOWN}"); ;Testcase
Send("{DOWN}"); ;Verify reponse

MouseClick("left", $poslist[0]+226, $poslist[1]+180,2)
Send("{DOWN}")
Send("{DOWN}")
Send("{ENTER}")


MouseClick("left", $poslist[0]+226, $poslist[1]+196,2)
Send("{DOWN}")
Send("{DOWN}")
Send("{ENTER}")

MouseClick("left", $poslist[0]+226, $poslist[1]+196,2)
Send("{UP}")
Send("{UP}")
Send("{ENTER}")


;Again Main Tree
MouseClick("left",$pos[0]+30,$pos[1]+55,2)	
Send("{RIGHT}")

send("{DOWN}")
send("{DOWN}")
send("{DOWN}") 
send("{DOWN}") 
send("{DOWN}") 
send("{DOWN}") 
send("{DOWN}") 

;VERIFY--------------
MouseClick("left", $poslist[0]+226, $poslist[1]+180,2)
Send("{DOWN}")
Send("{DOWN}")
Send("{ENTER}")


MouseClick("left", $poslist[0]+226, $poslist[1]+196,2)
Send("{DOWN}")
Send("{DOWN}")
Send("{ENTER}")

MouseClick("left", $poslist[0]+226, $poslist[1]+196,2)
Send("{UP}")
Send("{UP}")
Send("{ENTER}")
;----------------------

;Again Main Tree
MouseClick("left",$pos[0]+30,$pos[1]+55,2)	
Send("{RIGHT}")

for $i=0 to 13
	send("{DOWN}")
Next

;Ssend--------------
MouseClick("left", $poslist[0]+226, $poslist[1]+180,2)
Send("{DOWN}")
Send("{DOWN}")
Send("{ENTER}")


MouseClick("left", $poslist[0]+226, $poslist[1]+196,2)
Send("{DOWN}")
Send("{DOWN}")
Send("{ENTER}")

MouseClick("left", $poslist[0]+226, $poslist[1]+196,2)
Send("{UP}")
Send("{UP}")
Send("{ENTER}")
;----------------------

;Again Main Tree
MouseClick("left",$pos[0]+30,$pos[1]+55,2)	
Send("{RIGHT}")

;Now Exit
send("!fx")
if WinWaitActive("BUSMASTER TestSetup Editor", "", 2) Then ; wait till its open
	;$Result = "ok"		
	ControlClick("BUSMASTER TestSetup Editor", "","Button1")
Else
	$Result = "Failed"
EndIf