; Module :	Frame Transmission
; Feature:	Time Delay btw Blocks with key trigger, monoshot usecase's

#Include <GuiListView.au3>
#Include <GuiToolBar.au3>
#include <globalFunc.au3>

WinActivate("BUSMASTER", 3)

If WinExists("BUSMASTER") Then
	Sleep(2000)
	;load cfx file
	_loadConfig("TxTDBB")

	;load dbf file
	_loadDatabase("Txwin")

	;close the dbf
	Send("!fdc")

	;associate dbf
	_associateDB("&File", "Txwin.dbf")

	;select kvaser device
	WinMenuSelectItem("BUSMASTER","","&Configure","&Hardware Interface","&Kvaser CAN")
	sleep(1500)
	if winexists("Hardware Selection") Then
		sleep(1000)
		ControlClick("Hardware Selection","&Select","[CLASS:Button; INSTANCE:2]")
		sleep(1000)
		ControlClick("Hardware Selection","&Select","[CLASS:Button; INSTANCE:2]")
		sleep(1000)
		ControlClick("Hardware Selection","&OK","[CLASS:Button; INSTANCE:4]")
		sleep(1000)
	EndIf
	sleep(2000)

	; Open 'configure Tx messages' window
	WinMenuSelectItem("BUSMASTER","","&Configure","&Tx Messages")
	sleep(1000)

	; check if 'Autoupdate' check box is enabled
	if (ControlCommand("BUSMASTER","",17000,"IsChecked")=0) Then
		ControlCommand("BUSMASTER","",17000,"Check")
	EndIf


	; ---first block---

	; Click on 'Add' block message
	ControlClick("BUSMASTER","",1226)
	sleep(500)

	; Check 'Time delay between blocks'
	ControlClick("BUSMASTER","",17001)

	; Enter the delay as 1000
	ControlSetText("BUSMASTER","",17002,1000)

	; Add the DB msgs to the Tx message list
	ControlCommand("BUSMASTER","",1221,"SetCurrentSelection",0)

	; Click on Add message button
	ControlClick("BUSMASTER","",1122)

	; Enter Non Db msg
	ControlSetText("BUSMASTER","",1001,1)
	sleep(100)

	; Click on Add message button
	ControlClick("BUSMASTER","",1122)

	; ---EO first block---


	; ---Second block with key trigger 'Single'---

	; Click on 'Add' block message
	ControlClick("BUSMASTER","",1226)
	sleep(500)

	; Check Key Trigger for Single msg type
	ControlCommand("BUSMASTER","",1237,"Check")
	sleep(500)

	; Enter the Key trigger value
	ControlSetText("BUSMASTER","",1137,"a")

	; Add the DB msgs to the Tx message list
	ControlCommand("BUSMASTER","",1221,"SetCurrentSelection",1)

	; Click on Add message button
	ControlClick("BUSMASTER","",1122)

	; Enter Non Db msg
	ControlSetText("BUSMASTER","",1001,2)
	sleep(100)

	; Click on Add message button
	ControlClick("BUSMASTER","",1122)



	; ---EO second block---

	; ---Third block with key trigger 'All'---

	; Click on 'Add' block message
	ControlClick("BUSMASTER","",1226)
	sleep(500)

	; Check 'Monoshot' radio button
	Controlcommand("BUSMASTER","",1008,"Check")

	; Check Key Trigger for All msg type
	ControlCommand("BUSMASTER","",1237,"Check")
	sleep(500)

	; Select msg type = "ALL"
	ControlCommand("BUSMASTER","","ComboBox3","SelectString","All")

	; Enter the Key trigger value
	ControlSetText("BUSMASTER","",1137,"b")

	; Enter Non Db msg
	ControlSetText("BUSMASTER","",1001,3)
	sleep(100)

	; Click on Add message button
	ControlClick("BUSMASTER","",1122)

	; ---EO third block---


	; Get handle of the 'Connect/Disconnect' toolbar
	$cntToolhWd=ControlGetHandle("BUSMASTER","",128)

	; Click on 'Connect' icon
	_GUICtrlToolbar_ClickIndex($cntToolhWd,4)
	sleep(1000)

	; Click on 'Transmit normal blocks' menu
	WinMenuSelectItem("BUSMASTER","","F&unctions","&Transmit","&Normal Blocks")
	sleep(1000)

	; Get handle og Tx msg list control
	$txMsgListHWD=ControlGetHandle("BUSMASTER","","[CLASS:SysListView32; INSTANCE:2]")

	; Check and uncheck the msgs in Tx msg list during transmission
	for $j=0 to 2
		_GUICtrlListView_SetItemChecked($txMsgListHWD,3,False)
		sleep(100)
		_GUICtrlListView_SetItemChecked($txMsgListHWD,3)
		sleep(100)
		_GUICtrlListView_SetItemChecked($txMsgListHWD,2,False)
		sleep(100)
		_GUICtrlListView_SetItemChecked($txMsgListHWD,2)
		sleep(100)
		_GUICtrlListView_SetItemChecked($txMsgListHWD,0,False)
		sleep(100)
		_GUICtrlListView_SetItemChecked($txMsgListHWD,0)
		sleep(100)
		_GUICtrlListView_SetItemChecked($txMsgListHWD,1,False)
		sleep(100)
		_GUICtrlListView_SetItemChecked($txMsgListHWD,1)
		sleep(100)

		; uncheck all items in 'Tx msg List'
		_GUICtrlListView_SetItemChecked($txMsgListHWD,-1,False)

		; check all items in 'Tx msg List'
		_GUICtrlListView_SetItemChecked($txMsgListHWD,-1,True)
	next

	; Send "a" key stroke to invoke key trigger for 'single' msg
	send("a")
	sleep(3000)
	; Send "a" key stroke to invoke key trigger for 'single' msg
	send("a")
	sleep(3000)

	; Send "b" key stroke to invoke key trigger for 'All' msg
	send("b")
	sleep(3000)

	;Click on 'Disconnect' icon
	_GUICtrlToolbar_ClickIndex($cntToolhWd,4)

	; Close the Tx Window
	ControlClick("BUSMASTER","",1015)
EndIf




