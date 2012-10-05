; Feature:			Frame Transmission
#Include <GuiListView.au3>
#Include <GuiToolBar.au3>
#include <globalFunc.au3>

WinActivate("BUSMASTER",3)
if WinExists("BUSMASTER") then
	sleep(2000)
	WinMenuSelectItem("BUSMASTER","","&Configure","&Hardware Interface","&Simulation")
	sleep(1000)
	WinMenuSelectItem("BUSMASTER","","&Configure","&Hardware Interface","&Kvaser CAN") 			;select device
	sleep(2000)
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
	$Count=2
	WinMenuSelectItem("BUSMASTER","","&Configure","&Tx Messages")								; Open 'configure Tx messages' window
	sleep(1000)
	if (ControlCommand("BUSMASTER","",17000,"IsChecked")=0) Then
		ControlCommand("BUSMASTER","",17000,"Check")											; check 'Autoupdate' check box is enabled
	EndIf
	for $i=1 to 4
		ControlClick("BUSMASTER","",1226)															; Click on 'Add' block message
		sleep(500)
		ControlSetText("BUSMASTER","",1134,1)														; Enter time delay of 1 msec
		ControlSetText("BUSMASTER","",1001,$i&1)														; Enter '11' in Name/ID combo box
		sleep(100)
		ControlClick("BUSMASTER","",1122)																; Click on Add message button
		sleep(100)
		ControlSetText("BUSMASTER","",1001,$i&2)														; Enter '12' in Name/ID combo box
		sleep(100)
		ControlClick("BUSMASTER","",1122)																; Click on Add message button
		sleep(100)
		ControlSetText("BUSMASTER","",1001,$i&3)														; Enter '13' in Name/ID combo box
		sleep(100)
		ControlClick("BUSMASTER","",1122)																; Click on Add message button
		sleep(100)
		ControlSetText("BUSMASTER","",1001,$i&4)														; Enter '14' in Name/ID combo box
		sleep(100)
		ControlClick("BUSMASTER","",1122)																; Click on Add message button
		sleep(100)
	Next
	$cntToolhWd=ControlGetHandle("BUSMASTER","",128)												; Get handle of tollbar
	_GUICtrlToolbar_ClickIndex($cntToolhWd,4)														; Click on 'Connect' icon
	sleep(1000)
	WinMenuSelectItem("BUSMASTER","","F&unctions","&Transmit","&Normal Blocks")						; Click on 'Transmit normal blocks' icon
	sleep(1000)
	$txMsgListHWD=ControlGetHandle("BUSMASTER","","[CLASS:SysListView32; INSTANCE:2]")
	for $j=0 to 10
		_GUICtrlListView_SetItemChecked($txMsgListHWD,1,False)
		sleep(100)
		_GUICtrlListView_SetItemChecked($txMsgListHWD,1)
		sleep(100)
		_GUICtrlListView_SetItemChecked($txMsgListHWD,2,False)
		sleep(100)
		_GUICtrlListView_SetItemChecked($txMsgListHWD,2)
		sleep(100)
		_GUICtrlListView_SetItemChecked($txMsgListHWD,3,False)
		sleep(100)
		_GUICtrlListView_SetItemChecked($txMsgListHWD,3)
		sleep(100)
		_GUICtrlListView_SetItemChecked($txMsgListHWD,0,False)
		sleep(100)
		_GUICtrlListView_SetItemChecked($txMsgListHWD,0)
		sleep(100)
		_GUICtrlListView_SetItemChecked($txMsgListHWD,-1,False)										; uncheck all items in 'Tx msg List'
		_GUICtrlListView_SetItemChecked($txMsgListHWD,-1,True)										; check all items in 'Tx msg List'
	next
	_GUICtrlToolbar_ClickIndex($cntToolhWd,4)														; Click on 'Disconnect' icon
	ControlClick("BUSMASTER","",1227)																; Click 'Delete' block button
	sleep(1000)
	if winexists("BUSMASTER") Then
		ControlClick("BUSMASTER","",6)																; Click 'yes'
	EndIf
	_GUICtrlToolbar_ClickIndex($cntToolhWd,4)														; Click on 'Connect' icon
	sleep(1000)
	WinMenuSelectItem("BUSMASTER","","F&unctions","&Transmit","&Normal Blocks")						; Click on 'Transmit normal blocks' icon
	sleep(1000)
	for $j=0 to 10
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
		_GUICtrlListView_SetItemChecked($txMsgListHWD,-1,False)										; uncheck all items in 'Tx msg List'
		_GUICtrlListView_SetItemChecked($txMsgListHWD,-1,True)										; check all items in 'Tx msg List'
	next
	_GUICtrlToolbar_ClickIndex($cntToolhWd,4)														; Click on 'Disconnect' icon
EndIf