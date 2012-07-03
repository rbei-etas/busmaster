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




Send("!uax");

; Should be in shown state



;ControlClick("BUSMASTER", "", "[CLASS:SysTreeView32; INSTANCE:1]" , "click")
$pos=ControlGetPos ("BUSMASTER", "", "[CLASS:SysTreeView32; INSTANCE:1]" )
MouseClick("right",$pos[0]+30,$pos[1]+55,1)
Send("{DOWN}")
send("{ENTER}")
if WinWaitActive("Open", "", 2) Then ; wait till its open
	;$Result = "ok"
	Send(@ScriptDir & "\TestAutomationEditing.xml")
	Send("{ENTER}")
	Sleep(100);
Else
	$Result = "Failed"
EndIf

;MouseClick("left",$pos[0]+30,$pos[1]+55,2,
;MouseClick("right",$pos[0]+30,$pos[1]+55,1)
Send("{SPACE}")
Send("{DOWN}")
Send("{SPACE}")
Send("{RIGHT}")
Send("{DOWN}")
Send("{SPACE}")
Send("{DOWN}")
Send("{SPACE}")
Send("!uc")
MouseClick("right",$pos[0]+30,$pos[1]+55,1)
Send("{DOWN}")
Send("{DOWN}")
Send("{ENTER}")
