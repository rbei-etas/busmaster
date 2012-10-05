
If winexists("BUSMASTER") Then
	WinActivate("BUSMASTER", 3)
	sleep(1000)
	; switch to simulation mode
	WinMenuSelectItem("BUSMASTER","","&Configure","&Hardware Interface","&Simulation")

	; create or load config
	_loadConfig("dbmod")

	; Select database->new from the menu
	WinMenuSelectItem("BUSMASTER","","&File","&Database","&New")
	$DBFolderPath = _SetOneFolderUp()

	if winexists("Save As") Then

		; Enter the file name
		ControlSetText("Save As","",1152,$DBFolderPath&"\"&"StdExtDB")
		sleep(1000)

		; Click on Save button
		send("!s")
		sleep(3000)
	EndIf

	; -- Message : Standard--

	; Get handle of tree view to add a message
	$DBTreeVwHWD = ControlGetHandle("BUSMASTER","","[CLASS:SysTreeView32; INSTANCE:1]")

	; Fetch Parent node handle
	$DbtreeVwparent = _GUICtrlTreeView_GetFirstItem($DBTreeVwHWD)

	; Get handle of first child item
	$childHWD = _GUICtrlTreeView_GetFirstChild($DBTreeVwHWD, $DbtreeVwparent)

	; Right Click on the parent node
	_GUICtrlTreeView_ClickItem($DBTreeVwHWD, $DbtreeVwparent, "Right")

	; select 'new message' from the menu
	send("n")
	sleep(1000)

	if winexists("Message Details") Then

;~ 		; Select 'bool' sig type
;~ 		Controlcommand("Signal Details","",1008,"SelectString","bool")

		; Enter the msg name
		ControlSetText("Message Details","",1019,"MsgStdLil")

		; Enter msg ID
		ControlSetText("Message Details","",1035,101)

		; Enter msg length
		ControlSetText("Message Details","",1013,8)

		; Click on 'OK' button
		Controlclick("Message Details","",1)
		sleep(500)
	EndIf

	; Click on "New Signal" button
	ControlClick("BUSMASTER - [DatabaseEditor - CAN]", "New Signal...", 1075)
	sleep(2000)

	if WinExists("Signal Details") then

		; Select 'unsigned int' sig type
		Controlcommand("Signal Details","",1008,"SelectString","unsigned int")

		; Enter the signal name
		ControlSetText("Signal Details","",1000,"Sigstd1")

		; Enter the sig length
		ControlSetText("Signal Details","",1024,64)

		; Click on 'OK' button
		Controlclick("Signal Details","",1)
	EndIf

	; -- Message : Std big indian --

	; Get handle of tree view to add a message
	$DBTreeVwHWD = ControlGetHandle("BUSMASTER","","[CLASS:SysTreeView32; INSTANCE:1]")

	; Fetch Parent node handle
	$DbtreeVwparent = _GUICtrlTreeView_GetFirstItem($DBTreeVwHWD)

	; Get handle of first child item
	$childHWD = _GUICtrlTreeView_GetFirstChild($DBTreeVwHWD, $DbtreeVwparent)

	; Right Click on the parent node
	_GUICtrlTreeView_ClickItem($DBTreeVwHWD, $DbtreeVwparent, "Right")

	; select 'new message' from the menu
	send("n")
	sleep(1000)

	if winexists("Message Details") Then
		; Enter the msg name
		ControlSetText("Message Details","",1019,"MsgStdBig")

		; Enter msg ID
		ControlSetText("Message Details","",1035,102)

		; Enter msg length
		ControlSetText("Message Details","",1013,8)

		; Click on 'OK' button
		Controlclick("Message Details","",1)
		sleep(500)
	EndIf

	; Click on "New Signal" button
	ControlClick("BUSMASTER - [DatabaseEditor - CAN]", "New Signal...", 1075)
	sleep(2000)

	if WinExists("Signal Details") then

		; Select 'int' sig type
		Controlcommand("Signal Details","",1008,"SelectString","int")

		; Select Big Endian radio button
		ControlCommand("Signal Details","",1307,"Check")

		; Enter the signal name
		ControlSetText("Signal Details","",1000,"Sigstd2")

		; Enter the byte index
		ControlSetText("Signal Details","",1023,7)

		; Enter the sig length
		ControlSetText("Signal Details","",1024,64)

		; Enter offset value
		ControlSetText("Signal Details","",1032,3.1)

		; Click on 'OK' button
		Controlclick("Signal Details","",1)
	EndIf

	; -- Message : Ext Lil Endian --

	; Get handle of tree view to add a message
	$DBTreeVwHWD = ControlGetHandle("BUSMASTER","","[CLASS:SysTreeView32; INSTANCE:1]")

	; Fetch Parent node handle
	$DbtreeVwparent = _GUICtrlTreeView_GetFirstItem($DBTreeVwHWD)

	; Get handle of first child item
	$childHWD = _GUICtrlTreeView_GetFirstChild($DBTreeVwHWD, $DbtreeVwparent)

	; Right Click on the parent node
	_GUICtrlTreeView_ClickItem($DBTreeVwHWD, $DbtreeVwparent, "Right")

	; select 'new message' from the menu
	send("n")
	sleep(1000)

	if winexists("Message Details") Then

		; Select Extended radio button
		ControlCommand("Message Details","",1016,"Check")

		; Enter the msg name
		ControlSetText("Message Details","",1019,"MsgExtLil")

		; Enter msg ID
		ControlSetText("Message Details","",1035,103)

		; Enter msg length
		ControlSetText("Message Details","",1013,8)

		; Click on 'OK' button
		Controlclick("Message Details","",1)
		sleep(500)
	EndIf

	; Click on "New Signal" button
	ControlClick("BUSMASTER - [DatabaseEditor - CAN]", "New Signal...", 1075)
	sleep(2000)

	if WinExists("Signal Details") then

		; Select 'int' sig type
		Controlcommand("Signal Details","",1008,"SelectString","bool")

		; Enter the signal name
		ControlSetText("Signal Details","",1000,"SigExt1")

		; Enter the byte index
		ControlSetText("Signal Details","",1023,0)

		; Enter the sig length
		ControlSetText("Signal Details","",1024,1)

		; Click on 'OK' button
		Controlclick("Signal Details","",1)
	EndIf

	; -- Message : Ext Big Endian --

	; Get handle of tree view to add a message
	$DBTreeVwHWD = ControlGetHandle("BUSMASTER","","[CLASS:SysTreeView32; INSTANCE:1]")

	; Fetch Parent node handle
	$DbtreeVwparent = _GUICtrlTreeView_GetFirstItem($DBTreeVwHWD)

	; Get handle of first child item
	$childHWD = _GUICtrlTreeView_GetFirstChild($DBTreeVwHWD, $DbtreeVwparent)

	; Right Click on the parent node
	_GUICtrlTreeView_ClickItem($DBTreeVwHWD, $DbtreeVwparent, "Right")

	; select 'new message' from the menu
	send("n")
	sleep(1000)

	if winexists("Message Details") Then

		; Select Extended radio button
		ControlCommand("Message Details","",1016,"Check")

		; Enter the msg name
		ControlSetText("Message Details","",1019,"MsgExtBig")

		; Enter msg ID
		ControlSetText("Message Details","",1035,104)

		; Enter msg length
		ControlSetText("Message Details","",1013,8)

		; Click on 'OK' button
		Controlclick("Message Details","",1)
		sleep(500)
	EndIf

	; Click on "New Signal" button
	ControlClick("BUSMASTER - [DatabaseEditor - CAN]", "New Signal...", 1075)
	sleep(2000)

	if WinExists("Signal Details") then

		; Select 'int' sig type
		Controlcommand("Signal Details","",1008,"SelectString","int")

		; Select Big Endian radio button
		ControlCommand("Signal Details","",1307,"Check")

		; Enter the signal name
		ControlSetText("Signal Details","",1000,"SigExt2")

		; Enter the byte index
		ControlSetText("Signal Details","",1023,7)

		; Enter offset value
		ControlSetText("Signal Details","",1032,1.8)

		; Enter the sig length
		ControlSetText("Signal Details","",1024,64)

		; Click on 'OK' button
		Controlclick("Signal Details","",1)
	EndIf

	; Save the database
	send("!FDS")

	; Close the database
	send("!FDC")

	; Associate DB
	_associateDB("&File","StdExtDB.dbf")

	; Open Tx Window
	WinMenuSelectItem("BUSMASTER","","&Configure","&Tx Messages")

	; check if 'Autoupdate' is enabled. If not enable it
	if (ControlCommand("BUSMASTER","",17000,"IsChecked")=0) Then
		ControlCommand("BUSMASTER","",17000,"Check")
	EndIf

	;		 First Block
	; Click on 'Add' block message
	ControlClick("BUSMASTER","",1226)

	; check if 'Autoupdate' is enabled. If not enable it
	if (ControlCommand("BUSMASTER","",1236,"IsChecked")=0) Then
		ControlCommand("BUSMASTER","",1236,"Check")
	EndIf

	; Enter 'Time Delay btw msgs'
	ControlSetText("BUSMASTER","",1134,2000)

	; Get handle of 'Name/ID' control
	$hWnd = ControlGetHandle("BUSMASTER","",1221)

	; Select first message
	_GUICtrlComboBoxEx_SetCurSel($hWnd,0)

	; Click on Add message button
	ControlClick("BUSMASTER","",1122)
	sleep(1000)

	; Select second message
	_GUICtrlComboBoxEx_SetCurSel($hWnd,1)

	; Click on Add message button
	ControlClick("BUSMASTER","",1122)

	;		 Second Block
	; Click on 'Add' block message
	ControlClick("BUSMASTER","",1226)

	; Enter 'Time Delay btw msgs'
	ControlSetText("BUSMASTER","",1134,2000)

	; Get handle of 'Name/ID' control
	$hWnd = ControlGetHandle("BUSMASTER","",1221)

	; Select third message
	_GUICtrlComboBoxEx_SetCurSel($hWnd,2)

	; Click on Add message button
	ControlClick("BUSMASTER","",1122)
	sleep(1000)

	; Select fourth message
	_GUICtrlComboBoxEx_SetCurSel($hWnd,3)

	; Select RTR
	Controlcommand("BUSMASTER","",1216,"Check")

	; Click on Add message button
	ControlClick("BUSMASTER","",1122)

	; Get handle of tollbar
	$cntToolhWd=ControlGetHandle("BUSMASTER","",128)

	; Click on 'Connect' icon
	_GUICtrlToolbar_ClickIndex($cntToolhWd,4)
	sleep(500)

	; Transmit messages
	send("!utn")

	sleep(5000)


	; Click on 'Disconnect' icon
	_GUICtrlToolbar_ClickIndex($cntToolhWd,4)
	sleep(500)

	; Select Key Trigger option
	ControlCommand("BUSMASTER","",1237,"Check")

	; Enter key trigger value
	ControlSetText("BUSMASTER","",1137,"a")

	; Click on 'Connect' icon
	_GUICtrlToolbar_ClickIndex($cntToolhWd,4)
	sleep(500)

	; Transmit messages
	send("!utn")

	sleep(4000)

	; Invoke key trigger
	Send("a")

	sleep(3000)

	; Invoke key trigger
	Send("a")

	; Click on 'Disconnect' icon
	_GUICtrlToolbar_ClickIndex($cntToolhWd,4)
	sleep(500)

	; Select msg type = "ALL"
	ControlCommand("BUSMASTER","","ComboBox3","SelectString","All")

	; Click on 'Connect' icon
	_GUICtrlToolbar_ClickIndex($cntToolhWd,4)
	sleep(500)

	; Transmit messages
	send("!utn")

	sleep(4000)

	; Invoke key trigger
	Send("a")

	sleep(3000)

	; Click on 'Disconnect' icon
	_GUICtrlToolbar_ClickIndex($cntToolhWd,4)
	sleep(500)

	; Change Tx mode to 'Monoshot'
	ControlCommand("BUSMASTER","",1008,"Check")
	sleep(500)

	; Click on 'Connect' icon
	_GUICtrlToolbar_ClickIndex($cntToolhWd,4)
	sleep(500)

	; Transmit messages
	send("!utn")

	sleep(4000)

	; Invoke key trigger
	Send("a")

	sleep(3000)

	; Click on 'Disconnect' icon
	_GUICtrlToolbar_ClickIndex($cntToolhWd,4)
	sleep(500)

	; Select msg type = "Single"
	ControlCommand("BUSMASTER","","ComboBox3","SelectString","Single")

	; Click on 'Connect' icon
	_GUICtrlToolbar_ClickIndex($cntToolhWd,4)
	sleep(500)

	; Transmit messages
	send("!utn")

	sleep(4000)

	; Invoke key trigger
	Send("a")

	sleep(3000)

	; Invoke key trigger
	Send("a")

	sleep(2000)

	; Click on 'Disconnect' icon
	_GUICtrlToolbar_ClickIndex($cntToolhWd,4)
	sleep(500)

	; Uncheck time delay between msgs option
	ControlCommand("BUSMASTER","",1236,"UnCheck")

	; Click on 'Connect' icon
	_GUICtrlToolbar_ClickIndex($cntToolhWd,4)
	sleep(500)

	; Transmit messages
	send("!utn")

	sleep(2000)

	; Invoke key trigger
	Send("a")

	sleep(2000)

	; Click on 'Disconnect' icon
	_GUICtrlToolbar_ClickIndex($cntToolhWd,4)
	sleep(500)

	; Switch to 'Cyclic' mode
	ControlCommand("BUSMASTER","",1007,"Check")

	; Click on 'Connect' icon
	_GUICtrlToolbar_ClickIndex($cntToolhWd,4)
	sleep(500)

	; Transmit messages
	send("!utn")

	sleep(2000)

	; Invoke key trigger
	Send("a")

	sleep(4000)
	; Click on 'Disconnect' icon
	_GUICtrlToolbar_ClickIndex($cntToolhWd,4)
	sleep(500)

EndIf
