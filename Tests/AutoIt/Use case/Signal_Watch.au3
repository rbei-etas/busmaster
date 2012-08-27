; Configure Signal Watch


#Include <GuiListView.au3>
#include<globalFunc.au3>
#Include <GuiToolBar.au3>

WinActivate("BUSMASTER")
if winexists("BUSMASTER") Then
	sleep(1000)
	WinMenuSelectItem("BUSMASTER","","&Configure","&Hardware Interface","&Simulation")
	_loadConfig("config_sig_watch")					;load cfx file
	_loadDatabase("sig_watchTest")					;load dbf file
	send("!fdc")
	_associateDB("&File","sig_watchTest.dbf")		;associate dbf
	if winexists("Hardware Selection") Then			; select channels
		ControlClick("Hardware Selection","&Select","[CLASS:Button; INSTANCE:2]")
		sleep(1000)
		ControlClick("Hardware Selection","&Select","[CLASS:Button; INSTANCE:2]")
		ControlClick("Hardware Selection","&OK","[CLASS:Button; INSTANCE:4]")
		sleep(1000)
	EndIf
	sleep(2000)
	$Count=2
	WinMenuSelectItem("BUSMASTER","","&Configure","&Tx Messages")		; configure Tx messages
	sleep(1000)
	if (ControlCommand("BUSMASTER","",17000,"IsChecked")=0) Then
		ControlCommand("BUSMASTER","",17000,"Check")					; check 'Autoupdate' check box
	EndIf
	_txMSG("Cyclic",$Count)
	ControlClick("BUSMASTER","",1015)									; Close Tx Window
	send("!cn")															; configure signal watch
	sleep(2000)
	if winexists("Signal Watch List") Then
		$sigListHWD=ControlGetHandle("Signal Watch List","",1071)		; Get signal list view handle
		_GUICtrlListView_ClickItem($sigListHWD,0)							; select 1st item
		ControlClick("Signal Watch List","",32950)						; click Add
		sleep(1000)
		$sigWatchListHWD=ControlGetHandle("Signal Watch List","",1070)		; Get signal watch list view handle
		_GUICtrlListView_ClickItem($sigWatchListHWD,0)							; select 1st item
		ControlClick("Signal Watch List","",32947)						; click delete
		sleep(1000)
		ControlClick("Signal Watch List","",32946)						; click Add All
		sleep(1000)
		ControlClick("Signal Watch List","",32948)						; click Delete All
		Sleep(1000)
		ControlClick("Signal Watch List","",32946)						; click Add All
		sleep(1000)
		ControlCommand("Signal Watch List","",1231,"SetCurrentSelection",1)	; Send 2nd msg from msg list
		Sleep(1000)
		ControlClick("Signal Watch List","",32946)						; click Add All
		sleep(1000)
		ControlClick("Signal Watch List","",1)						; click OK
	EndIf
	$cntToolhWd=ControlGetHandle("BUSMASTER","",128)
	_GUICtrlToolbar_ClickIndex($cntToolhWd,4)						; connect
	WinMenuSelectItem("BUSMASTER","","F&unctions","&Transmit","&Normal Blocks")			;transmit
	send("!vw")
	sleep(1000)
	send("!uh")															; select hex
	sleep(500)
	send("!uh")															; unselect hex
	$sigWatchWinHWD=ControlGetHandle("Signal Watch - CAN","",1056)
	_GUICtrlListView_ClickItem($sigWatchWinHWD,0,"Right")					; Right click on Signal Watch  and 'Clear'
	send("c")
	sleep(2000)
	_GUICtrlToolbar_ClickIndex($cntToolhWd,4)								; Disconnect
EndIf





