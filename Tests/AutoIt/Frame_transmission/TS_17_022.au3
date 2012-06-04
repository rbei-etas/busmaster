;Test Case Number:	TS_17_022
;Feature:			Frame Transmission
; Critical (C):		Y
; Test Cases:		Database messages in Name\ID combo box
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===

#Include <GuiToolBar.au3>
#include <GuiComboBox.au3>
#Include <GuiComboBoxEx.au3>
if (WinWaitActive("BUSMASTER","",5) ) Then
send("!fgn")         ;File->Configuration->New
Send("!n")
EndIf
if (WinWaitActive("New Configuration Filename...","",5)) Then
send("cfx_TS_17_FT!s")
if (WinWaitActive("New Configuration Filename...","Do you want to replace it?",5)) Then
Send("!y")
Else
	Send("!s")
EndIf
EndIf
if (WinWaitActive("BUSMASTER","",5) ) Then
Send("!fdo")         ;File->Database->Open
;WinWaitActive("Select BUSMASTER Database Filename...",5)
send("DBF_TS_17_FT")
sleep(100)
;ControlSend ("BUSMASTER","",1152,"DBF_TS_17_FT")
ControlClick("Select BUSMASTER Database Filename...","&Open","[CLASS:Button; INSTANCE:2]","left")
;msgbox(0,"",$a)
if (WinWaitActive("BUSMASTER","Specified database file is not found",5)) Then
send("{ENTER}")
Send("!fdn")         ;File->Database->New
Send("DBF_TS_17_FT!s")
sleep(100)
WinWaitActive("BUSMASTER - [DatabaseEditor - CAN]","",5)
$pos=ControlGetPos("BUSMASTER - [DatabaseEditor - CAN]", "", "[CLASS:SysTreeView32; INSTANCE:1]" )
$x=$pos[0]
$y=$pos[1]
MouseClick("right",$x+30,$y+55)
send("n")
send("TS_17")		;Message Name
send("{TAB}")
send("17")			;Message ID
send("{TAB}")
send("5")			;Message Length
ControlClick("Message Details","","OK")
ControlClick("BUSMASTER - [DatabaseEditor - CAN]", "New Signal...", 1075) ;1075 is the ID for New Signal button in message and signal information
WinWaitActive("Signal Details","",5)
send("SIG1")
send("{TAB}")
send("i")
send("{TAB}")
send("{TAB}")
send("{TAB}")
send("5") 
ControlClick("Signal Details","","OK")
EndIf
EndIf
send("!fds")
sleep(50)
send("!fda")
WinWaitActive("Select Active Database Filename...","",5)
sleep(100)
send("DBF_TS_17_FT.dbf")
sleep(100)
ControlClick("Select Active Database Filename...","&Open","[CLASS:Button; INSTANCE:2]")
$handle=ControlGetHandle("BUSMASTER", "","[CLASS:ToolbarWindow32; INSTANCE:3]")
_GUICtrlToolbar_clickbutton($handle,32891,"left") ;Clicks a specific button.32891 is the command ID of the specific button
sleep(100)
ControlClick("BUSMASTER","Add",1226)
if (WinWaitActive("BUSMASTER","",5) ) Then
$hWnd = ControlGetHandle("BUSMASTER","",1221)
_GUICtrlComboBox_ShowDropDown($hWnd, True)

$a=_GUICtrlComboBox_SelectString($hWnd, "TS_17[0x17]")
_GUICtrlComboBox_SelectString($hWnd, "",0)
ControlCommand("BUSMASTER","",1213,"Check")
sleep(100)
controlCommand("BUSMASTER","",1215,"Check")
_GUICtrlComboBox_ShowDropDown($hWnd, False)
$DLC=ControlGetText("BUSMASTER", "", 1125)
if $a=0 and $DLC=5 Then
   _ExcelWriteCell($oExcel, "OK", 27, 10)
else
   _ExcelWriteCell($oExcel, "Error", 27, 10)
EndIf
EndIf
