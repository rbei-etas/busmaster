;Configure Signal Graph
#Include <GuiTreeView.au3>
#include<globalFunc.au3>
#Include <GuiToolBar.au3>

if winexists("BUSMASTER") Then
	sleep(1000)
	WinMenuSelectItem("BUSMASTER","","&Configure","&Hardware Interface","&Simulation")
	_loadConfig("sig_Graph")					;load cfx file
	_loadDatabase("GraphTest")				;load dbf file
	send("!fdc")
	_associateDB("&File","GraphTest.dbf")	;associate dbf
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
	if winexists("BUSMASTER") Then
		send("{ENTER}")
	EndIf
	sleep(2000)
	send("!cg")						; select Configure Signal Graph
	sleep(1000)
	ControlClick("Configure Signal Graph WIndow","",1014) ;configure display
	sleep(500)
	if WinExists("Graph Window Configuration") Then
		ControlClick("Graph Window Configuration","",2)		;cancel
	EndIf
	ControlClick("Configure Signal Graph WIndow","",1014)	;configure display
	sleep(2000)
	if WinExists("Graph Window Configuration") Then
		ControlSetText("Graph Window Configuration","",1181,6000)	;Buffer Size
		ControlSetText("Graph Window Configuration","",1183,2000)	;Display update rate
		ControlCommand("Graph Window Configuration","",215,"SelectString","Flat") ;select frame type
		ControlSetText("Graph Window Configuration","",1182,10)					;X-Axis Grid
		ControlSetText("Graph Window Configuration","",1184,10)					;Y-Axis Grid
		ControlClick("Graph Window Configuration","",207)						;frame color
		sleep(2000)
		if WinExists("Color") Then
			ControlClick("Color","",719)										;Define Custom Colors
			ControlClick("Color","",2)											;Cancel
		EndIf
		ControlClick("Graph Window Configuration","",207)						;frame color
		sleep(2000)
		if WinExists("Color") Then
			ControlClick("Color","",719)										;Define Custom Colors
			ControlSetText("Color","",703,110)									;set 'Hue'
			ControlSetText("Color","",706,6)									;set 'Red'
			ControlSetText("Color","",704,250)									;set 'Sat'
			ControlSetText("Color","",707,6)									;set 'Green'
			ControlSetText("Color","",705,6)									;set 'Lum'
			ControlSetText("Color","",708,5)									;set 'Blue'
			ControlClick("Color","",712)										;Add to Custom Colors
			ControlClick("Color","",1)											;Ok
		EndIf
		ControlClick("Graph Window Configuration","",1)							;ok
	EndIf
	ControlClick("Configure Signal Graph WIndow","",1031)						;configure display
	sleep(1000)
	ControlClick("Select Element","",2)											; Select 'Cancel'
	ControlClick("Configure Signal Graph WIndow","",1031)						;configure display
	sleep(2000)
	if WinExists("Select Element") Then
		$sltEleHWD=ControlGetHandle("Select Element","",1009)			;Get tree view handle in 'Select Element' window
		_GUICtrlTreeView_Expand($sltEleHWD)								;expand the tree view
		_GUICtrlTreeView_EnsureVisible($sltEleHWD,0)					;Scroll up to the parent item
		send("{DOWN}")
		send("{DOWN}")
		send("{DOWN}")
		ControlClick("Select Element","",1228)							;click add
		$parentHWD=_GUICtrlTreeView_GetParentHandle($sltEleHWD,0)
		_GUICtrlTreeView_ClickItem($sltEleHWD,$parentHWD,"left")
		send("{DOWN}")
		ControlClick("Select Element","",1228)							;click add
		ControlListView("Select Element","",1010,"Select",0)
		ControlClick("Select Element","",1002)							;delete
		ControlClick("Select Element","",1003)							;delete all
		_GUICtrlTreeView_Expand($sltEleHWD)								;expand the tree view
		_GUICtrlTreeView_EnsureVisible($sltEleHWD,0)					;Scroll up to the parent item
		$item1HWD=_GUICtrlTreeView_GetFirstItem($sltEleHWD)
		_GUICtrlTreeView_ClickItem($sltEleHWD,$item1HWD)				;click first item
		send("{DOWN}")
		send("{DOWN}")
		send("{DOWN}")
		ControlClick("Select Element","",1228)							;click add
		_GUICtrlTreeView_ClickItem($sltEleHWD,$item1HWD)				;click first item
		send("{DOWN}")
		send("{DOWN}")
		send("{DOWN}")
		ControlClick("Select Element","",1228)							;click add
		ControlClick("Select Element","",1)								;click OK
	EndIf
	ControlClick("Configure Signal Graph WIndow","",1)
	sleep(1000)
	$cntToolhWd=ControlGetHandle("BUSMASTER","",128)
	_GUICtrlToolbar_ClickIndex($cntToolhWd,4)							;connect
	WinMenuSelectItem("BUSMASTER","","F&unctions","&Transmit","&Normal Blocks")	;transmit
	sleep(2000)
	send("!vg")																	; view signal graph
	sleep(1000)
	ControlClick("BUSMASTER","",1022)
	ControlClick("BUSMASTER","",1022)
	ControlClick("BUSMASTER","",1022)
	send("^{F4}")
	send("{ESC}")
EndIf