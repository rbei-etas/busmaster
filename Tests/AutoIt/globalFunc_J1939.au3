#Include <GuiStatusBar.au3>
#Include <Excel.au3>
#Include <GuiComboBoxEx.au3>
#Include <GuiListView.au3>
#Include <GuiMenu.au3>

Global $oExcel,$app,$rVal,$lDB,$cDB,$dbMenu,$aDB,$flag,$txMode,$blckCount,$sgLen,$sel,$gLine,$recMsg2,$ACL
Global $msgCount,$msg1SigCount,$msg2SigCount,$sigCount,$funcRes,$starttime,$lConfig,$noOfSigs

;=========================================================================================================
;Function Name : _writeCrashRes
;Functionality : This function updates the results sheet with status=fail if an app crash is found.
;Input 		   : The respective testcase column and row is passed as parameters from the calling function.
;Output 	   : -
;==========================================================================================================

Func _writeCrashRes($col,$row)
	_ExcelWriteCell($oExcel, "fail", $col, $row)
	_ExcelWriteCell($oExcel, "Application Crashed", $col, $row-1)
EndFunc


;======================================================================================================================================================
;Function Name : _delBlockMsg
;Functionality : Checks if a block exists in "Message Block" List View of configure Tx msgs window. If no block is found then adds it to the list view.
;Input 		   : -
;Output 	   : If a block is found then the func validates the popup window for deleting a msg block and returns the result to the calling function.
;======================================================================================================================================================

Func _delBlockMsg()
WinWaitActive("BUSMASTER","",5)
$listViewCount=ControlListView("BUSMASTER", "", "SysListView323", "GetItemCount")	; count the no. of items in the 'Message Block' listview
if $listViewCount>0 then
ControlClick("BUSMASTER","Delete",1227)												; if an item is found, delete.
	if WinWaitActive("BUSMASTER","You are about to delete a message block",5) Then
	$res=1
else
	$res=0
	EndIf
Else
	ControlClick("BUSMASTER","Add",1226)											; if an item is not found then add and delete
	ControlClick("BUSMASTER","Delete",1227)
	if WinWaitActive("BUSMASTER","You are about to delete a message block",5) Then
	$res=1
else
	$res=0
	EndIf
EndIf
Return $res
EndFunc

;==========================================================================================================
;Function Name : _launchApp
;Functionality : This function checks for the existence of the application. If not found then launches it.
;Input 		   : -
;Output		   : If the app is launched then it returns the $rVal=1 to calling function
;==========================================================================================================

Func _launchApp()
$rVal=1
consolewrite("launchApp function"&@CRLF)
sleep(1000)
$app=winexists("BUSMASTER","")													;Check if the BUSMASTER window exists
consolewrite("app exists res: "&$app&@CRLF)
if $app=0 Then																	;if not found then run the app
	$rVal=Run("C:\Program Files\BUSMASTER\BUSMASTER.exe")
	consolewrite("App search result: "&$rVal&@CRLF)
	sleep(2000)
	if WinWaitActive("MCNet","",1) Then
		ControlClick("MCNet","OK","[CLASS:Button; INSTANCE:1]")
		sleep(2000)
	EndIf
;~ 	Return $app
;~ 	Return $rVal
EndIf
sleep(1000)
if winexists("Hardware Selection") Then											; if a Hardware Selection dialog appears then click 'OK'
	ControlClick("Hardware Selection","&Select","[CLASS:Button; INSTANCE:2]")	
	sleep(1000)
	ControlClick("Hardware Selection","&Select","[CLASS:Button; INSTANCE:2]")
	sleep(1000)
	ControlClick("Hardware Selection","&OK","[CLASS:Button; INSTANCE:4]")
	sleep(1000)
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
$sMenu=WinMenuSelectItem("BUSMASTER","","&File","Confi&guration","&Load")								;select menu : file->load
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
		ControlSend("Load Configuration Filename...","","[CLASS:Edit; INSTANCE:1]",@ScriptDir&"\"&$fName)				;load cfx file
		sleep(1000)
		ControlClick("Load Configuration Filename...","Load","[CLASS:Button; INSTANCE:2]","")
		sleep(1000)
			if winexists("Load Configuration Filename...",$fName) Then									;if the cfx file doesn't exists then create a new one
				ControlClick("Load Configuration Filename...","","[CLASS:Button; INSTANCE:1]","left")
				sleep(1000)
				ControlClick("Load Configuration Filename...","","[CLASS:Button; INSTANCE:3]","left")
				sleep(1000)
				$sMenu=WinMenuSelectItem("BUSMASTER","","&File","Confi&guration","&New")				;select menu : file->new
				sleep(1000)
				if $sMenu=0 Then
					msgbox(0,"","Test execution Failed...'New' Menu not found!!!can't proceed further")
					Exit
				Endif
				if WinWait("","Configuration File is modified",2) Then
					ControlClick("BUSMASTER","","[CLASS:Button; INSTANCE:2]","left")
					sleep(1000)
				EndIf
				sleep(500)
				if winexists("New Configuration Filename...") Then
					ControlSend("New Configuration Filename...","","[CLASS:Edit; INSTANCE:1]",@ScriptDir&"\"&$fName)
					sleep(1000)
					$cConfig=ControlClick("New Configuration Filename...","","[CLASS:Button; INSTANCE:2]","left")
					sleep(2000)
					if winexists("Hardware Selection") Then
						ControlClick("Hardware Selection","&Select","[CLASS:Button; INSTANCE:2]")
						sleep(1000)
						ControlClick("Hardware Selection","&Select","[CLASS:Button; INSTANCE:2]")
						sleep(1000)
						ControlClick("Hardware Selection","&OK","[CLASS:Button; INSTANCE:4]")
						sleep(1000)
					EndIf
				EndIf
			EndIf
	EndIf
sleep(1000)
$hwnd=ControlGetHandle("BUSMASTER","","[CLASS:msctls_statusbar32; INSTANCE:1]")
$statusBar=_GUICtrlStatusBar_GetText($hwnd,1)
ConsoleWrite("statusbar = "&$statusBar&@CRLF)
$findText=StringInStr($statusBar,$fName)
ConsoleWrite("CFX File : "&$findText&@CRLF)
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
;Functionality : loads or creates a CAN database file
;Input 		   : file Name to be loaded/created
;Output 	   : returns true if created/loaded else false
;==========================================================

Func _loadDatabase($dbFName)
	$lDB=0
	$cDB=0
	$msgCount=0
	$sigCount=0
if winexists("BUSMASTER") Then
$dbMenu=WinMenuSelectItem("BUSMASTER","","&File","&Database","&Open")									;select menu : file->load
sleep(1000)
	if $dbMenu=0 Then
		msgbox(0,"","Test execution Failed...'Database' Menu not found!!!can't proceed further")
		Exit
	EndIf
sleep(150)
if winexists("Select BUSMASTER Database Filename...") Then
	ControlSend("Select BUSMASTER Database Filename...","","[CLASS:Edit; INSTANCE:1]",@ScriptDir&"\"&$dbFName)			;load dbf file
	$lDb=ControlClick("Select BUSMASTER Database Filename...","","[CLASS:Button; INSTANCE:2]","left")
	sleep(1000)
		if winexists("BUSMASTER","Specified database file is not found.") Then							; if the database file doesn't exists then create a new one
			ControlClick("BUSMASTER","OK","[CLASS:Button; INSTANCE:1]","left")
			sleep(1000)
			$dbMenu=WinMenuSelectItem("BUSMASTER","","&File","&Database","&New")
			sleep(1000)
			if $dbMenu=0 Then
				msgbox(0,"","Test execution Failed...'New' Menu not found!!!can't proceed further")
				Exit
			EndIf
			if winexists("Save As") Then
				ControlSend("Save As","","[CLASS:Edit; INSTANCE:1]",@ScriptDir&"\"&$dbFName)
				sleep(1000)
				$cDB=ControlClick("Save As","","[CLASS:Button; INSTANCE:2]","left")
				sleep(1000)
			EndIf
			if Winexists("BUSMASTER - [DatabaseEditor - CAN]") Then
				$pos=ControlGetPos ("BUSMASTER - [DatabaseEditor - CAN]", "", "[CLASS:SysTreeView32; INSTANCE:1]" )
				MouseClick("right",$pos[0]+30,$pos[1]+55)															; Add new message
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
					ControlClick("BUSMASTER - [DatabaseEditor - CAN]", "New Signal...", 1075)						; Add Signal
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
					ControlClick("BUSMASTER - [DatabaseEditor - CAN]", "New Signal...", 1075)						; Add Signal
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
				MouseClick("right",$pos[0]+30,$pos[1]+55)															; Add new message
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
					ControlClick("BUSMASTER - [DatabaseEditor - CAN]", "New Signal...", 1075) 						; Add Signal
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
					ControlClick("BUSMASTER - [DatabaseEditor - CAN]", "New Signal...", 1075)						; Add Signal
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
;Function Name : _loadJ1939Database
;Functionality : loads or creates a J1939 database file
;Input 		   : file Name to be loaded/created
;Output 	   : returns true if created/loaded else false
;==========================================================


Func _loadJ1939Database($dbFName,$PGN,$msgLen,$sgLen)
	$lDB=0
	$cDB=0
	$msgCount=0
	$sigCount=0
if winexists("BUSMASTER") Then
$dbMenu=WinMenuSelectItem("BUSMASTER","","&J1939","&Database","&Open")									;select menu : file->load
sleep(1000)
	if $dbMenu=0 Then
		msgbox(0,"","Test execution Failed...'Database' Menu not found!!!can't proceed further")
		Exit
	EndIf
sleep(150)
	if winexists("Select J1939 Database Filename...") Then
		ControlSend("Select J1939 Database Filename...","","[CLASS:Edit; INSTANCE:1]",@ScriptDir&"\"&$dbFName)			;load dbf file
		$lDb=ControlClick("Select J1939 Database Filename...","","[CLASS:Button; INSTANCE:2]","left")
		sleep(1000)
		if winexists("BUSMASTER","Specified database file is not found.") Then							; if the database file doesn't exists then create a new one
			ControlClick("BUSMASTER","OK","[CLASS:Button; INSTANCE:1]","left")
			sleep(1000)
			$dbMenu=WinMenuSelectItem("BUSMASTER","","&J1939","&Database","&New")
			sleep(1000)
			if $dbMenu=0 Then
				msgbox(0,"","Test execution Failed...'New' Menu not found!!!can't proceed further")
				Exit
			EndIf
			if winexists("Save As") Then
				ControlSend("Save As","","[CLASS:Edit; INSTANCE:1]",@ScriptDir&"\"&$dbFName)
				sleep(1000)
				$cDB=ControlClick("Save As","","[CLASS:Button; INSTANCE:2]","left")
				sleep(1000)
			EndIf
			if Winexists("BUSMASTER - [DatabaseEditor - J1939]") Then
				$pos=ControlGetPos ("BUSMASTER - [DatabaseEditor - J1939]", "", "[CLASS:SysTreeView32; INSTANCE:1]" )
				MouseClick("right",$pos[0]+30,$pos[1]+55)															; Add new message
				send("n")
				sleep(1000)
				if WinExists("Message Details") Then
					send("msg1")
					send("{TAB}")
					send($PGN)
					send("{TAB}")
					send($msgLen)
					ControlClick("Message Details","","OK")
					sleep(1000)
					$noOfSigs=($msgLen*8)/$sgLen
					ConsoleWrite("$noOfSigs : "&$noOfSigs&@CRLF)
					for $i=0 to $noOfSigs-1
						ControlClick("BUSMASTER - [DatabaseEditor - J1939]", "New Signal...", 1075) 						; Add Signal
						sleep(1000)
						if WinExists("Signal Details") then
							send("SIG"&$i+1)
							send("{TAB}")
							send("i")
							send("{TAB}")
							send($i)
							send("{TAB}")
							send("{TAB}")
							send($sgLen)
							ControlClick("Signal Details","","OK")
							sleep(1000)
						EndIf
					Next
				EndIf
				WinMenuSelectItem("BUSMASTER","","&J1939","&Database","&Save")
			EndIf
		EndIf
		$a=ControlTreeView("BUSMASTER","","[CLASS:SysTreeView32; INSTANCE:1]","GetText")
		$msgCount=ControlTreeView("BUSMASTER","","[CLASS:SysTreeView32; INSTANCE:1]","GetItemCount",$a)
		if $msgCount=1 Then
			ControlTreeView("BUSMASTER","","[CLASS:SysTreeView32; INSTANCE:1]","Select","#0|#0")
			$msg1SigCount=ControlListView("BUSMASTER","","SysListView321","GetItemCount")
			ConsoleWrite("$msg1SigCount : "&$msg1SigCount&@CRLF)
		EndIf
	EndIf
	Return $msgCount
	Return $msg1SigCount
	Return $noOfSigs
EndIf
EndFunc



;============================================
;Function Name : _associateDB
;Functionality : associates a database file
;Input 		   : file Name to be associated
;Output 	   : -
;============================================

Func _associateDB($menu,$afName)
	$flag=0
	$dbMenu=WinMenuSelectItem("BUSMASTER","",$menu,"&Database","&Associate")
	if winexists("Select Active Database Filename...") Then
		$flag=1
		$AsscPath = _SetOneFolderUp()
		ControlSend("Select Active Database Filename...","","[CLASS:Edit; INSTANCE:1]",$AsscPath&"\"&$afName)
		$aDB=ControlClick("Select Active Database Filename...","","[CLASS:Button; INSTANCE:2]","left")
		sleep(1000)
	EndIf
EndFunc

;===============================================================================
;Function Name : _txMSG
;Functionality : configures transmission messages
;Input 		   : txMode(Cyclic or Monoshot),$blckCount:no. of blocks to be added
;Output 	   : -
;===============================================================================

Func _txMSG($txMode,$blckCount)
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

;===============================================================================
;Function Name : _hdWareSelect
;Functionality : Selects Kvaser CAN Hardware
;Input 		   : -
;Output 	   : Returns $channelCount to calling script
;===============================================================================

Func _hdWareSelect()
	if winexists("BUSMASTER") Then
		sleep(500)
		WinMenuSelectItem("BUSMASTER","","&Configure","&Hardware Interface","&Kvaser CAN")
		sleep(1000)
		if winexists("Hardware Selection") Then
			ControlClick("Hardware Selection","&Select","[CLASS:Button; INSTANCE:2]")
			sleep(1000)
			ControlClick("Hardware Selection","&Select","[CLASS:Button; INSTANCE:2]")
			sleep(1000)
			$channelCount=ControlListView("Hardware Selection","","SysListView322","GetItemCount")
			ControlClick("Hardware Selection","&OK","[CLASS:Button; INSTANCE:4]")
			sleep(1000)
		Endif
	EndIf
	Return $channelCount
EndFunc

;===============================================================================
;Function Name : act_dctJ1939
;Functionality : Activates or Deactivates J1939 transmission
;Input 		   : $menu('&Activate' or 'Deac&tivate') is passed from calling script
;Output 	   : returns return value of 'WinMenuSelectItem'
;===============================================================================

func _act_dctJ1939($menu)
$sel=0
	if winexists("BUSMASTER") Then
		sleep(500)
		$sel=WinMenuSelectItem("BUSMASTER","","&J1939",$menu)
		sleep(1000)
	EndIf
Return $sel
EndFunc

;===============================================================================
;Function Name : _goOnline
;Functionality : Selects 'Go Online' menu
;Input 		   : -
;Output 	   : returns return value of 'WinMenuSelectItem'
;===============================================================================

Func _goOnline()
$gLine=0
	if winexists("BUSMASTER") Then
		sleep(500)
		$gLine=WinMenuSelectItem("BUSMASTER","","&J1939","&Function","&Go Online")
		sleep(1000)
	EndIf
Return $gLine
EndFunc

;=====================================================================================================
;Function Name : _J1939tMsgWin
;Functionality : Configures 'Transport Protocol Function' for J1939 transmission.
;Input 		   : $msgType('R-Request PGN' or 'D-Data' or 'B-Broadcast') is passed from calling script.
;Output 	   : returns return value of 'WinMenuSelectItem'
;=====================================================================================================

Func _J1939tMsgWin($msgType)
	consolewrite("$msgType : "&$msgType&@CRLF)
	if winexists("BUSMASTER") Then
		sleep(1000)
		WinMenuSelectItem("BUSMASTER","","&J1939","&View","&Transmit Window")
		ControlCommand("J1939 Transmit Message Window","",1013,"Check")
		if $msgType="Request PGN" then
			ControlCommand("J1939 Transmit Message Window","",1058,"SelectString",$msgType)
			ControlSetText("J1939 Transmit Message Window","",1348,6)
		Elseif $msgType="Data" then
			ControlCommand("J1939 Transmit Message Window","",1346,"SelectString","[0x0]TSCee1")
			ControlCommand("J1939 Transmit Message Window","",1058,"SelectString",$msgType)
			ControlSetText("J1939 Transmit Message Window","",1125,8)
			ControlSetText("J1939 Transmit Message Window","",1016,"100000000000000000000000")
			ControlSetText("J1939 Transmit Message Window","",1348,7)
		Else			
			ControlCommand("J1939 Transmit Message Window","",1346,"SelectString","[0xfeff]WFI")
			ControlCommand("J1939 Transmit Message Window","",1058,"SelectString",$msgType)
			ControlSetText("J1939 Transmit Message Window","",1333,0)
			ControlSetText("J1939 Transmit Message Window","",1181,"FF")
			ControlSetText("J1939 Transmit Message Window","",1125,21)
			ControlSetText("J1939 Transmit Message Window","",1016,"100000000000000000000000100000000000000000000000100000000000000000000000")
			ControlSetText("J1939 Transmit Message Window","",1348,7)
		EndIf
		_overWriteMode()														; Call _overWriteMode function to disable overWriteMode
		;WinMenuSelectItem("BUSMASTER","","F&unctions","&Connect")
		sleep(1000)
		ControlClick("J1939 Transmit Message Window","",1011,"left")
		sleep(3000)
		send("{TAB}")
		send("{ENTER}")
		sleep(1000)
	EndIf
EndFunc


Func _overWriteMode()
	if winexists("BUSMASTER") Then
		sleep(2000)
		$hWnd = WinGetHandle("BUSMASTER")
		$hMain = _GUICtrlMenu_GetMenu($hWnd)
		ConsoleWrite("menu handle : "&$hMain& @CRLF)
		$hFile = _GUICtrlMenu_GetItemSubMenu($hMain, 4)
		ConsoleWrite("Submenu handle : "&$hFile& @CRLF)
		$text=_GUICtrlMenu_GetItemText($hFile,6,True)
		ConsoleWrite("menu text : "&$text& @CRLF)
		$hm = _GUICtrlMenu_GetItemSubMenu($hFile, 6)
		ConsoleWrite("Submenu handle : "&$hm& @CRLF)
		$text2 = _GUICtrlMenu_GetItemText($hm,1,True)
		ConsoleWrite("text2: "&$text2& @CRLF)
		$chk=_GUICtrlMenu_GetItemChecked($hm,1,True)
		ConsoleWrite("$chk: "&$chk& @CRLF)
		if $chk=True then
			WinMenuSelectItem("BUSMASTER","","F&unctions","&Message Window","&Overwrite")
			sleep(1000)
		EndIf
	EndIf
EndFunc


;=====================================================================================================
;Function Name : _J1939ConfigSignalWatch
;Functionality : Configures 'signal watch list' for J1939
;Input 		   : $Confirm('OK' or 'Cancel') is passed from calling script.
;Output 	   : void
;=====================================================================================================

Func _J1939ConfigSignalWatch($Confirm)
	; Open 'Configure Signal Watch' dialog
	Send("!jcw") ; J1939 -> configure -> Signal Watch
	Sleep(1000)
	$hndSignalList = ControlGetHandle("Signal Watch List","",1071)		; Get Signal list control handle

	;Select a signal in Signal List View
	_GUICtrlListView_ClickItem($hndSignalList, 0);
	Sleep(500)

	; Click Add button
	Send("!a");

	Sleep(500)

	$hndSignalWatchList = ControlGetHandle("Signal Watch List","",1070)		; Get Signal watch list control handle

	;Select signal in Signal Watch List View
	_GUICtrlListView_ClickItem($hndSignalWatchList, 0);

	Sleep(500)
	; Click Delete button
	Send("!d");

	Sleep(500)
	; Click Add All button
	Send("!l");

	Sleep(500)
	; Click Delete All button
	Send("!e");

	Sleep(500)
	; Click Add All button
	Send("!l")

	Sleep(500)
	; Select broadcast message - WFI
	ControlCommand("Signal Watch List","",1231,"SelectString","[0xfeff]WFI")

	Sleep(500)
	; Click Add All button
	Send("!l");
	Sleep(500)
	
	if $Confirm = "OK" Then
		ControlClick("Signal Watch List","OK",1,"left")		; Click 'OK' button
	Else
		ControlClick("Signal Watch List","Cancel",2,"left")		; Click 'Cancel' button
	EndIf	
EndFunc

Func _SetOneFolderUp()
	$CurrentDirPath = @ScriptDir
	ConsoleWrite($CurrentDirPath&@CRLF)
	$arrStrings = StringSplit($CurrentDirPath, "\")

	ConsoleWrite("Size  = "&$arrStrings[0]&@CRLF)
	ConsoleWrite("Value = "&$arrStrings[$arrStrings[0]]&@CRLF)

	$CntRemove = stringlen($arrStrings[$arrStrings[0]]) + 1
	$TrimmedPath = StringTrimRight($CurrentDirPath, $CntRemove)

	ConsoleWrite("Trimmed  = "&$TrimmedPath&@CRLF)
	return $TrimmedPath
EndFunc









