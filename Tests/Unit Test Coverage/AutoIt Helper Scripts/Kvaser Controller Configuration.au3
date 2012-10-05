#cs ----------------------------------------------------------------------------

 AutoIt Version: 3.3.6.1
 Author:         Pramod Krishna

 Script Function:
	Kvaser CAN Controller Settings Configuration

#ce ----------------------------------------------------------------------------

; Script Start - Add your code below here

#Include <GuiListView.au3>

Sleep(2000)

WinActivate("Configure Kvaser CAN Controller")
if WinExists("Configure Kvaser CAN Controller") Then
	; Set Baud Rate
	ControlSetText("Configure Kvaser CAN Controller","",1000,250)
	sleep(500)
	send("{ENTER}")
	sleep(500)

	; Set BTR0
	ControlSetText("Configure Kvaser CAN Controller","",1001,"C0")
	sleep(500)
	send("{ENTER}")
	sleep(500)

	; Set BTR1
	ControlSetText("Configure Kvaser CAN Controller","",1002,"3A")
	sleep(500)
	send("{ENTER}")
	sleep(500)

	; Click on Filters to activate filters dialog
	ControlClick("Configure Kvaser CAN Controller","&Filter","[CLASS:Button; INSTANCE:9]")
	sleep(1000)

	; Check 'Extended radio' button
	controlcommand("Acceptance Filter","",1209,"Check")
	sleep(1000)

	; Check 'Standard radio' button
	controlcommand("Acceptance Filter","",1208,"Check")
	sleep(1000)

	; Check 'Reject All' radio button
	controlcommand("Acceptance Filter","",1214,"Check")
	sleep(1000)

	; Check 'Accept All' radio button
	controlcommand("Acceptance Filter","",1213,"Check")
	sleep(1000)

	; Check 'Use Mask and Code' radio button
	controlcommand("Acceptance Filter","",1215,"Check")
	sleep(1000)

	; set Code Byte 0
	controlsettext("Acceptance Filter","",1206,"FE")
	sleep(300)
	controlsend("Acceptance Filter","",1206,"F")
	; set Code Byte 1
	controlsettext("Acceptance Filter","",1181,"FE")
	sleep(300)
	controlsend("Acceptance Filter","",1181,"F")
	; set Code Byte 2
	controlsettext("Acceptance Filter","",1202,"FE")
	sleep(300)
	controlsend("Acceptance Filter","",1202,"F")
	; set Code Byte 3
	controlsettext("Acceptance Filter","",1207,"FE")
	sleep(300)
	controlsend("Acceptance Filter","",1207,"F")
	; set Mask Byte 0
	controlsettext("Acceptance Filter","",1204,"FE")
	sleep(300)
	controlsend("Acceptance Filter","",1204,"F")
	; set Mask Byte 1
	controlsettext("Acceptance Filter","",1201,"FE")
	sleep(300)
	controlsend("Acceptance Filter","",1201,"F")
	; set Mask Byte 2
	controlsettext("Acceptance Filter","",1203,"FE")
	sleep(300)
	controlsend("Acceptance Filter","",1203,"F")
	; set Mask Byte 3
	controlsettext("Acceptance Filter","",1205,"FE")
	sleep(300)
	controlsend("Acceptance Filter","",1205,"F")
	
	; Check 'Accept All' radio button
	controlcommand("Acceptance Filter","",1213,"Check")
	sleep(1000)
	
	; Click on 'OK' button
	ControlClick("Acceptance Filter","O&K","[CLASS:Button; INSTANCE:4]")
	sleep(500)

	; Click on Filters to activate filters dialog
	ControlClick("Configure Kvaser CAN Controller","&Filter","[CLASS:Button; INSTANCE:9]")
	sleep(1000)

	; Click on 'Cancel' button
	ControlClick("Acceptance Filter","",2)
	sleep(1000)

	; Click on Blink option
	ControlClick("Configure Kvaser CAN Controller","&Blink","[CLASS:Button; INSTANCE:10]")
	sleep(500)

	sleep(1000)

	; Click on a item in the list view
	$KvaserListVwHWD=controlgethandle("Configure Kvaser CAN Controller","",1012)
	_GUICtrlListView_ClickItem($KvaserListVwHWD,3)
	sleep(1000)

	; Close controller dialog
	ControlClick("Configure Kvaser CAN Controller","OK","[CLASS:Button; INSTANCE:1]")
endif
