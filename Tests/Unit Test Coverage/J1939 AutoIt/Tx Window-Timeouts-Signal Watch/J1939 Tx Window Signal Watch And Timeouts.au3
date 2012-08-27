; === Test Cases/Test Data ===
; Test Case Number:	
; Feature:			J1939 Tx Window, Timeouts Window and Signal watch
; Critical (C):		Y
; Test Cases:		Code coverage for J1939 TX window, Timeouts WIndow and Signal watch 
; Test Strategy:	White Box
; Test Data:		-
; AutoIt Version: 3.3.6.1
; Author:         Arunkumar Karri


; === Test Procedure ===
; --- Test SetUp ---
#Include <GuiListView.au3>
#include "globalFunc_J1939.au3"

;Launch BUSMASTER Application
_launchApp()

WinActivate("BUSMASTER","")

If (WinWaitActive("BUSMASTER","",5) ) Then
	
Else
	MsgBox(0, "GUI Event", "BUSMASTER not active")	
	Exit
EndIf	


; 1. Connect to CAN network	

Send("!uc") ; Functions -> Connect

; 2. Activate J1939 
Send("!ja") ; J1939 -> Activate

; 3. Go Online on J1939 
Send("!jfg") ; J1939 -> Functions -> Go Online

;~ ; Associate database_
;_associateDB("&J1939","J1939_SAE.dbf")

; ------------------ J1939 Transmission Window ------------------------------

; 4. Request a PGN
_J1939tMsgWin("Request PGN")

Sleep(100)

; Disconnect from CAN network

Send("!ui") ; Functions -> Disconnect

Sleep(1000)

; 1. Connect to CAN network	

Send("!uc") ; Functions -> Connect

; 5. Transmit DATA message
_J1939tMsgWin("Data")


Sleep(100)

; 6. Transmit Broadcast message
_J1939tMsgWin("Broadcast")

; Send broadcast messages in cyclic mode
WinMenuSelectItem("BUSMASTER","","&J1939","&View","&Transmit Window")	
WinActivate("BUSMASTER","")
ControlCommand("J1939 Transmit Message Window","",1015,"Check")				; Check 'Cyclic' option
Sleep(500)
ControlClick("J1939 Transmit Message Window","Transmit",1011,"left")		; Click on Transmit button
Sleep(3000)
ControlClick("J1939 Transmit Message Window","Stop",1011,"left")			; Click on Stop button
Sleep(500)
ControlCommand("J1939 Transmit Message Window","",1015,"UnCheck")				; UnCheck 'Cyclic' option

; Close J1939 Tx window
Send("{TAB}")
Send("{ENTER}")

Sleep(1000)

; Network Management message transmission

if winexists("BUSMASTER") Then
	; Open J1939 Tx window
	WinMenuSelectItem("BUSMASTER","","&J1939","&View","&Transmit Window")	
	
	; 7. Claim Address
	ControlCommand("J1939 Transmit Message Window","",1335,"Check")				; Check 'Network Management' Radio Button
	sleep(500)
	ControlCommand("J1939 Transmit Message Window","",1012,"Check")				; Check 'Claim Address' Radio Button	
	sleep(1000)
	ControlClick("J1939 Transmit Message Window","",1011,"left")				; Click on Transmit button
	sleep(500)

	; 8. Request Address
	ControlCommand("J1939 Transmit Message Window","",1008,"Check")				; Check 'Request Address' Radio Button	
	sleep(1000)
	ControlClick("J1939 Transmit Message Window","",1011,"left")				; Click on Transmit button
	sleep(500)	
	
	; 9. Command Address
	ControlCommand("J1939 Transmit Message Window","",1014,"SelectString","2")	; Select channel 2
	ControlCommand("J1939 Transmit Message Window","",1009,"Check")				; Check 'Command Address' Radio Button	
	sleep(1000)
	ControlClick("J1939 Transmit Message Window","",1011,"left")				; Click on Transmit button
	
	; Close J1939 Tx window
	Send("{TAB}")
	Send("{ENTER}")
EndIf
sleep(1000)

; ------------------ Configure Timeouts Window ------------------------------
if winexists("BUSMASTER") Then
	Sleep(1000)
	Send("!jct") ; J1939 -> Configure -> Timeouts
	
	Sleep(500)	
	ControlCommand("Configure Timeouts","",1009,"Check")	; Check 'Broadcast (Tb)' option
	ControlCommand("Configure Timeouts","",1014,"Check")	; Check 'Response (Tr)'  option
	ControlCommand("Configure Timeouts","",1015,"Check")	; Check 'Holding (Th)'   option
	ControlCommand("Configure Timeouts","",1264,"Check")	; Check 'T1' option
	ControlCommand("Configure Timeouts","",1265,"Check")	; Check 'T2' option
	ControlCommand("Configure Timeouts","",1340,"Check")	; Check 'T3' option
	ControlCommand("Configure Timeouts","",1341,"Check")	; Check 'T4' option
	
	ControlClick("Configure Timeouts","Apply",1,"left")		; Click 'Apply' button
	
	Sleep(1000)
	; Try to give 0 value for Broadcast timeout
	ControlCommand("Configure Timeouts","",1009,"UnCheck")	; Uncheck 'Broadcast (Tb)' option
	Sleep(500)
	ControlCommand("Configure Timeouts","",1009,"Check")	; Check   'Broadcast (Tb)' option
	Sleep(100)
	ControlSetText("Configure Timeouts", "", 1181, "0" ) 	;set a value of 0
	
	ControlClick("Configure Timeouts","Apply",1,"left")		; Click 'Apply' button

	Sleep(500)
	ControlClick("Configure Timeouts","Cancel",2,"left")		; Click 'Cancel' button	
EndIf
Sleep(500)

; ------------------ J1939 Signal Watch Window ------------------------------

; Add signals to signal watch list
_J1939ConfigSignalWatch("Cancel")
_J1939ConfigSignalWatch("OK")

Sleep(500)
;View Signal Watch window
Send("!jvs")	; J1939 -> View -> Signal Watch Window

; Transmit J1939 messages

; 10. Transmit DATA message
_J1939tMsgWin("Data")

Sleep(100)

; 11. Transmit Broadcast message
_J1939tMsgWin("Broadcast")


WinActivate("BUSMASTER","")
; Open 'Configure Signal Watch' dialog
Send("!jcw") ; J1939 -> configure -> Signal Watch
Sleep(500)
	
; Click Delete All button
Send("!e");

Sleep(200)
; Select broadcast message - WFI
ControlCommand("Signal Watch List","",1231,"SelectString","[0xfeff]WFI")

; Click Add All button
Send("!l");
Sleep(200)
	
ControlClick("Signal Watch List","OK",1,"left")		; Click 'OK' button	

; 12. Transmit Broadcast message
_J1939tMsgWin("Broadcast")

; 13. Close Signal Watch window 
WinClose("Signal Watch - J1939")

; --- Closure ---
;WinClose("BUSMASTER")
;ControlClick("BUSMASTER", "", 7)	;7 is the ID for 'No' button