; === Test Cases/Test Data ===
; Test Case Number:	
; Feature:			J1939 Node Simulation
; Critical (C):		Y
; Test Cases:		Code coverage for J1939 simulation API list
; Test Strategy:	White Box
; Test Data:		"C:\BUSMASTER\NODESIM_FILES\" folder contents
; AutoIt Version: 3.3.6.1
; Author:         Arunkumar Karri


; === Test Procedure ===
; --- Test SetUp ---

#Include <GuiTreeView.au3>
#include "globalFunc.au3"


; Initialization - Copy the simulated systems folder to C:\BUSMASTER\ directory
DirCreate("C:\BUSMASTER\NODESIM_FILES")
FileCopy("NODESIM_FILES\*.*", "C:\BUSMASTER\NODESIM_FILES\")


;Launch BUSMASTER Application
_launchApp()

WinActivate("BUSMASTER","")

If (WinWaitActive("BUSMASTER","",5) ) Then
	
Else
	MsgBox(0, "GUI Event", "BUSMASTER not active")
	FileWriteLine($file,  "BUSMASTER application is not opened.")	
	Exit
EndIf	


; 1. Connect to CAN network	

Send("!uc") ; Functions -> Connect	
	
; 2. Activate J1939 
Send("!ja") ; J1939 -> Activate

; 3. Go Online on J1939 
Send("!jfg") ; J1939 -> Functions -> Go Online

; 4. Configure simulated Systems
Send("!jcs") ; J1939 -> Configure -> Simulated Systems

sleep(1000)
; 5. Add simulated system
if winexists("BUSMASTER") Then
	
	sleep(2000)
	$pos=ControlGetPos ("BUSMASTER", "", "[CLASS:SysTreeView32; INSTANCE:1]" )
	; Right click on 'J1939 BUS' tree root item
	MouseClick("right",$pos[0]+30,$pos[1]+55)
	; Click on 'Add Simulated System'
	send("a")
	sleep(1000)
	if winexists("Add Configuration Filename...") Then
			$fName = "C:\BUSMASTER\NODESIM_FILES\SIMSYS_J1939.sim"
			ControlSend("Add Configuration Filename...","","[CLASS:Edit; INSTANCE:1]",$fName)				;load simulation file
			Sleep(1000)
			ControlClick("Add Configuration Filename...","Add","[CLASS:Button; INSTANCE:2]","")			; Click 'Add' button
	EndIf
	
EndIf

; 6. Load all nodes in simulated system
Send("!jnl") ; J1939 -> Node Simulation -> Load All

sleep(1000)

; 7. Enable All Handlers
Send("!jna") ; J1939 -> Node Simulation -> All Handlers	

sleep(1000)	

; 8. Open J1939 Tx Window
Send("!jvt") ; J1939 -> View -> Transmit window
sleep(1000)

; 9. Send a J1939 Data message
$msgType = "Data"
ControlCommand("J1939 Transmit Message Window","",1013,"Check")
ControlCommand("J1939 Transmit Message Window","",1058,"SelectString",$msgType)
ControlSetText("J1939 Transmit Message Window","",1125,8)
ControlSetText("J1939 Transmit Message Window","",1016,"100000000000000000000000")
ControlSetText("J1939 Transmit Message Window","",1348,7)

sleep(1000)
ControlClick("J1939 Transmit Message Window","",1011,"left")
sleep(1000)
send("{TAB}")
send("{ENTER}")

; --- Closure ---
WinClose("BUSMASTER")
ControlClick("BUSMASTER", "", 7)	;7 is the ID for 'No' button

sleep(2000)
; Remove the simulated system Files
;DirRemove("C:\BUSMASTER\NODESIM_FILES", 1)
;DirRemove("C:\BUSMASTER", 1)

