#Include <GuiStatusBar.au3>
#Include <Excel.au3>
#Include <GuiComboBoxEx.au3>
#Include <GuiListView.au3>
;#Include "D:\Projects\BusMaster\AutoIt\TestRun_Usecase's\IsAppCrash.au3"
Global $oExcel,$app,$rVal,$lDB,$cDB,$dbMenu,$aDB,$flag,$txMode,$blckCount
Global $msgCount,$msg1SigCount,$msg2SigCount,$sigCount,$funcRes

;===Function _writeCrashRes===
;This function updates the results sheet with status=fail if an app crash is found.
;The respective testcase column and row is passed as parameters from the calling function
;=============================

Func _writeCrashRes($col,$row)
	_ExcelWriteCell($oExcel, "fail", $col, $row)
	_ExcelWriteCell($oExcel, "Application Crashed", $col, $row-1)
	;$oExcel.Activesheet.Range("J6").Interior.ColorIndex = 3
EndFunc


;Func _writeRes($resState,$col,$row)
;	switch $resState
;		Case 0
;			_ExcelWriteCell($oExcel, "fail", $col, $row)
;		Case 1
;			_ExcelWriteCell($oExcel, "Pass", $col, $row)
;	EndSwitch
;EndFunc

;===Function _delBlockMsg===
;Checks if a block exists in "Message Block" List View of configure Tx msgs window. If no block is found then adds it to the list view.
;If found then validates the popup window for deleting a msg block and returns the result to the calling function.
;===========================

Func _delBlockMsg()
WinWaitActive("BUSMASTER","",5)
$listViewCount=ControlListView("BUSMASTER", "", "SysListView323", "GetItemCount")
if $listViewCount>0 then
ControlClick("BUSMASTER","Delete",1227)
	if WinWaitActive("BUSMASTER","You are about to delete a message block",5) Then
	$res=1
else
	$res=0
	EndIf
Else
	ControlClick("BUSMASTER","Add",1226)
	ControlClick("BUSMASTER","Delete",1227)
	if WinWaitActive("BUSMASTER","You are about to delete a message block",5) Then
	$res=1
else
	$res=0
	EndIf
EndIf
Return $res
EndFunc

;===Function _launchApp===
;This function checks for the existence of the application. If not found then launches it.
;=========================

Func _launchApp()
$app=winexists("BUSMASTER","")
if $app=0 Then
	$rVal=Run("C:\Program Files\BUSMASTER\BUSMASTER.exe")
	consolewrite($rVal)
	sleep(2000)
	if WinWaitActive("MCNet","",5) Then
		ControlClick("MCNet","OK","[CLASS:Button; INSTANCE:1]")
	EndIf
	Return $app
	Return $rVal
Else
	Return
EndIf
if winexists("Hardware Selection") Then
	ControlClick("Hardware Selection","&Select","[CLASS:Button; INSTANCE:2]")
	ControlClick("Hardware Selection","&Select","[CLASS:Button; INSTANCE:2]")
	;$channelCount=ControlListView("Hardware Selection","","SysListView322","GetItemCount")
	ControlClick("Hardware Selection","&OK","[CLASS:Button; INSTANCE:4]")
EndIf
EndFunc

;==========================================================
;Function Name : _loadConfig
;Functionality : loads or creates a configuration file
;Input         : file Name to be loaded/created
;Output        : returns true if created/loaded else false
;==========================================================

Func _loadConfig($fName)
if winexists("BUSMASTER") Then
$sMenu=WinMenuSelectItem("BUSMASTER","","&File","Confi&guration","&Load")						;select menu : file->load
sleep(1000)
	if $sMenu=0 Then
		msgbox(0,"","Test execution Failed...'Load' Menu not found!!!can't proceed further")
		Exit
	EndIf
	if WinWait("BUSMASTER","Configuration File is modified",2) Then
		ControlClick("BUSMASTER","","[CLASS:Button; INSTANCE:2]","left")
		sleep(1000)
	EndIf
sleep(150)
	if winexists("Load Configuration Filename...") Then
		ControlSend("Load Configuration Filename...","","[CLASS:Edit; INSTANCE:1]",$fName)		;load cfx file
		sleep(1000)
		ControlClick("Load Configuration Filename...","Load","[CLASS:Button; INSTANCE:2]","")
		;send("{ENTER}")
		ConsoleWrite("load res:" &$lConfig)
		sleep(1000)
		sleep(1000)
			if winexists("Load Configuration Filename...",$fName) Then									; if the cfx file doesn't exists then create a new one
				ControlClick("Load Configuration Filename...","","[CLASS:Button; INSTANCE:1]","left")
				sleep(1000)
				ControlClick("Load Configuration Filename...","","[CLASS:Button; INSTANCE:3]","left")
				sleep(1000)
				$sMenu=WinMenuSelectItem("BUSMASTER","","&File","Confi&guration","&New")				;select menu : file->new
				sleep(1000)
				if $sMenu=0 Then
					msgbox(0,"","Test execution Failed...'New' Menu not found!!!can't proceed further")
					;Exit
				Endif
				if WinWait("","Configuration File is modified",2) Then
					ControlClick("BUSMASTER","","[CLASS:Button; INSTANCE:2]","left")
					sleep(1000)
				EndIf
				sleep(500)
				if winexists("New Configuration Filename...") Then
					ControlSend("New Configuration Filename...","","[CLASS:Edit; INSTANCE:1]",$fName)
					sleep(1000)
					$cConfig=ControlClick("New Configuration Filename...","","[CLASS:Button; INSTANCE:2]","left")
					sleep(2000)
					if winexists("Hardware Selection") Then
						ControlClick("Hardware Selection","&Select","[CLASS:Button; INSTANCE:2]")
						ControlClick("Hardware Selection","&Select","[CLASS:Button; INSTANCE:2]")
						ControlClick("Hardware Selection","&OK","[CLASS:Button; INSTANCE:4]")
					EndIf
				EndIf
			EndIf
	EndIf
sleep(1000)
$hwnd=ControlGetHandle("BUSMASTER","","[CLASS:msctls_statusbar32; INSTANCE:1]")
$statusBar=_GUICtrlStatusBar_GetText($hwnd,1)
ConsoleWrite(" statusbar="&$statusBar)
$findText=StringInStr($statusBar,$fname)
ConsoleWrite($findText)
if Not $findText=0 then
	$funcRes=1
Else
	$funcRes=0
EndIf
Return $funcRes
EndIf
EndFunc

;==========================================================
;Function Name : _loadDatabase
;Functionality : loads or creates a database file
;Input 		   : file Name to be loaded/created
;Output 	   : returns true if created/loaded else false
;==========================================================

Func _loadDatabase($dbFName)
	$lDB=0
	$cDB=0
	$msgCount=0
	$sigCount=0
if winexists("BUSMASTER") Then
$dbMenu=WinMenuSelectItem("BUSMASTER","","&File","&Database","&Open")						;select menu : file->load
sleep(1000)
	if $dbMenu=0 Then
		msgbox(0,"","Test execution Failed...'Database' Menu not found!!!can't proceed further")
		Exit
	EndIf
sleep(150)
if winexists("Select BUSMASTER Database Filename...") Then
	ControlSend("Select BUSMASTER Database Filename...","","[CLASS:Edit; INSTANCE:1]",$dbFName)		;load dbf file
	$lDb=ControlClick("Select BUSMASTER Database Filename...","","[CLASS:Button; INSTANCE:2]","left")
	sleep(1000)
		if winexists("BUSMASTER","Specified database file is not found.") Then						; if the database file doesn't exists then create a new one
			;$lDb=0
			;msgbox(0,"","hi")
			ControlClick("BUSMASTER","OK","[CLASS:Button; INSTANCE:1]","left")
			sleep(1000)
			$dbMenu=WinMenuSelectItem("BUSMASTER","","&File","&Database","&New")
			sleep(1000)
			if $dbMenu=0 Then
				msgbox(0,"","Test execution Failed...'New' Menu not found!!!can't proceed further")
				Exit
			EndIf
			if winexists("Save As") Then
				ControlSend("Save As","","[CLASS:Edit; INSTANCE:1]",$dbFName)
				sleep(1000)
				$cDB=ControlClick("Save As","","[CLASS:Button; INSTANCE:2]","left")
				sleep(1000)
			EndIf
			if Winexists("BUSMASTER - [DatabaseEditor - CAN]") Then
				$pos=ControlGetPos ("BUSMASTER - [DatabaseEditor - CAN]", "", "[CLASS:SysTreeView32; INSTANCE:1]" )
				MouseClick("right",$pos[0]+30,$pos[1]+55)
				send("n")
				sleep(1000)
				if WinExists("Message Details") Then
					send("msg1")
					send("{TAB}")
					send("51")
					send("{TAB}")
					send("5")
					ControlClick("Message Details","","OK")
					sleep(1000)
					ControlClick("BUSMASTER - [DatabaseEditor - CAN]", "New Signal...", 1075) 			;1075 is the ID for New Signal button in message and signal information
					sleep(1000)
					if WinExists("Signal Details") then
						send("SIG1")
						send("{TAB}")
						send("i")
						send("{TAB}")
						send("{TAB}")
						send("{TAB}")
						send("5")
						ControlClick("Signal Details","","OK")
						sleep(1000)
					EndIf
					ControlClick("BUSMASTER - [DatabaseEditor - CAN]", "New Signal...", 1075)
					if WinExists("Signal Details") then
						send("SIG2")
						send("{TAB}")
						send("i")
						send("{TAB}")
						send(1)
						send("{TAB}")
						send("{TAB}")
						send("5")
						ControlClick("Signal Details","","OK")
						sleep(1000)
					EndIf
				EndIf
				MouseClick("right",$pos[0]+30,$pos[1]+55)
				send("n")
				sleep(1000)
				if WinExists("Message Details") Then
					send("msg2")
					send("{TAB}")
					send("55")
					send("{TAB}")
					send("5")
					ControlClick("Message Details","","OK")
					sleep(1000)
					ControlClick("BUSMASTER - [DatabaseEditor - CAN]", "New Signal...", 1075) 			;1075 is the ID for New Signal button in message and signal information
					sleep(1000)
					if WinExists("Signal Details") then
						send("SIG3")
						send("{TAB}")
						send("i")
						send("{TAB}")
						send("{TAB}")
						send("{TAB}")
						send("5")
						ControlClick("Signal Details","","OK")
						sleep(1000)
					EndIf
					ControlClick("BUSMASTER - [DatabaseEditor - CAN]", "New Signal...", 1075)
					if WinExists("Signal Details") then
						send("SIG4")
						send("{TAB}")
						send("i")
						send("{TAB}")
						send(1)
						send("{TAB}")
						send("{TAB}")
						send("5")
						ControlClick("Signal Details","","OK")
						sleep(1000)
					EndIf
				EndIf
			EndIf
		EndIf
	$a=ControlTreeView("BUSMASTER","","[CLASS:SysTreeView32; INSTANCE:1]","GetText")
	$msgCount=ControlTreeView("BUSMASTER","","[CLASS:SysTreeView32; INSTANCE:1]","GetItemCount",$a)
	if $msgCount=2 Then
	ControlTreeView("BUSMASTER","","[CLASS:SysTreeView32; INSTANCE:1]","Select","#0|#0")
	$msg1SigCount=ControlListView("BUSMASTER","","SysListView321","GetItemCount")
	ControlTreeView("BUSMASTER","","[CLASS:SysTreeView32; INSTANCE:1]","Select","#0|#1")
	$msg2SigCount=ControlListView("BUSMASTER","","SysListView321","GetItemCount")
	$sigCount=$msg1SigCount+$msg2SigCount
	EndIf
EndIf
WinMenuSelectItem("BUSMASTER","","&File","&Database","&Save")
Return $lDb
Return $msgCount
Return $sigCount
EndIf
EndFunc

;==========================================================
;Function Name : _associateDB
;Functionality : associates a database file
;Input 		   : file Name to be associated
;Output 	   : -
;==========================================================

Func _associateDB($afName)
	$flag=0
	$dbMenu=WinMenuSelectItem("BUSMASTER","","&File","&Database","&Associate")
	if winexists("Select Active Database Filename...") Then
		$flag=1
		ControlSend("Select Active Database Filename...","","[CLASS:Edit; INSTANCE:1]",$afName)
		$aDB=ControlClick("Select Active Database Filename...","","[CLASS:Button; INSTANCE:2]","left")
		sleep(1000)
	EndIf
EndFunc

;==========================================================
;Function Name : _txMSG
;Functionality : configures transmission messages
;Input 		   : txMode(Cyclic or Monoshot),$blckCount:no. of blocks to be added
;Output 	   : -
;==========================================================

func _txMSG($txMode,$blckCount)
if winexists("BUSMASTER") Then
	sleep(1000)
	for $i=1 to $blckCount
		ControlClick("BUSMASTER","","[CLASS:Button; INSTANCE:14]","left")
		sleep(1000)
		ControlSetText("BUSMASTER","",1228,"BLOCK"&$i)
		if $txMode="Monoshot" Then
			ControlCommand("BUSMASTER","",1008,"Check")
		EndIf
		ControlSetText("BUSMASTER","",1134,1000)
		ControlFocus ( "BUSMASTER", "", 1221)
		$hWnd = ControlGetHandle("BUSMASTER","",1221)
		_GUICtrlComboBoxEx_ShowDropDown($hWnd)
		_GUICtrlComboBoxEx_SetCurSel($hWnd,$i-1)
		ControlCommand("BUSMASTER","",1213,"Check")
		Sleep(100)
		controlCommand("BUSMASTER","",1215,"Check")
		ControlClick("BUSMASTER","","[CLASS:Button; INSTANCE:4]","left")
		sleep(1000)
		ControlClick("BUSMASTER","","[CLASS:Button; INSTANCE:22]","left")
		sleep(1000)
	next
EndIf
EndFunc










