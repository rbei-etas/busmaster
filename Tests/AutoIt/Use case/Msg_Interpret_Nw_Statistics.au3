#include<globalFunc.au3>
#Include <GuiTreeView.au3>
#Include <GuiToolBar.au3>
#Include <GuiListView.au3>

if WinExists("BUSMASTER") then
	WinMenuSelectItem("BUSMASTER","","&Configure","&Hardware Interface","&Simulation")
	_loadConfig("interpret")					;load cfx file
	_loadDatabase("nwstatistics")				;load dbf file
	send("!fdc")
	_associateDB("&File","nwstatistics.dbf")	;associate dbf
	WinMenuSelectItem("BUSMASTER","","&Configure","&Hardware Interface","&Kvaser CAN") ;select device
	if winexists("Hardware Selection") Then
		ControlClick("Hardware Selection","&Select","[CLASS:Button; INSTANCE:2]")
		sleep(1000)
		ControlClick("Hardware Selection","&Select","[CLASS:Button; INSTANCE:2]")
		ControlClick("Hardware Selection","&OK","[CLASS:Button; INSTANCE:4]")
		sleep(1000)
	EndIf
	sleep(2000)
	$Count=2
	WinMenuSelectItem("BUSMASTER","","&Configure","&Tx Messages")
	sleep(1000)
	_txMSG("Cyclic",$Count)
	ControlClick("BUSMASTER","","[CLASS:Button; INSTANCE:26]","left")
	ControlClick("BUSMASTER","","[CLASS:Button; INSTANCE:25]","left")
	sleep(1000)
	$cntToolhWd=ControlGetHandle("BUSMASTER","",128)
	_GUICtrlToolbar_ClickIndex($cntToolhWd,4)
	WinMenuSelectItem("BUSMASTER","","F&unctions","&Transmit","&Normal Blocks")
	sleep(2000)
	send("!vn")
	sleep(2000)
	WinClose("Network Statistics")
	send("!umi")
	sleep(2000)
	$msgWinHWD=ControlGetHandle("BUSMASTER","","[CLASS:SysListView32; INSTANCE:1]")
	_GUICtrlListView_ClickItem($msgWinHWD,1)						;select the 1st item in signal list view
	send("{ENTER}")
EndIf