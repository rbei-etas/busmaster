#Include <GuiStatusBar.au3>
#Include <Excel.au3>
#Include <GuiComboBoxEx.au3>
#Include <GuiListView.au3>
#Include <GuiMenu.au3>
#Include <Data_KIT.au3>
#Include <Date.au3>

Global $blckCount, $oExcel, $crashRes


Func _chkCrash($col,$row)
if $crashRes=1 Then
	_ExcelWriteCell($oExcel, "Fail", $col, $row)
Endif
EndFunc

;==========================================================================================================
;Function Name : _launchApp
;Functionality : This function checks for the existence of the application. If not found then launches it.
;Input 		   : -
;Output		   : -
;==========================================================================================================

Func _launchApp()
$appPath="C:\Program Files\BUSMASTER\BUSMASTER.exe"
$rVal=1
consolewrite("launchApp function"&@CRLF)
sleep(1000)
$app=winexists($mWin,"")													;Check if the BUSMASTER window exists
consolewrite("app exists res: "&$app&@CRLF)
if $app=0 Then																	;if not found then run the app
	$rVal=Run($appPath)
	consolewrite("App search result: "&$rVal&@CRLF)
	Return $app
	Return $rVal
EndIf
sleep(1000)
if WinExists($mWin) Then
	sleep(1000)
	if WinExists("MCNet") Then
		send("{ENTER}")
	EndIf
sleep(1000)
	if winexists($hWare) Then																		; if a Hardware Selection dialog appears then click 'OK'
		ControlClick($hWare,$selectBTN,"[CLASS:Button; INSTANCE:2]")
		sleep(1000)
		ControlClick($hWare,$selectBTN,"[CLASS:Button; INSTANCE:2]")
		sleep(1000)
		ControlClick($hWare,$okBTN,"[CLASS:Button; INSTANCE:4]")
		sleep(1000)
	EndIf
EndIf
EndFunc

;==========================================================
;Function Name : _loadConfig
;Functionality : loads or creates a configuration file
;Input         : file Name to be loaded/created
;Output        : -
;==========================================================

Func _loadConfig($fName)
if winexists($mWin) Then
$sMenu=WinMenuSelectItem($mWin,"","&File","Confi&guration","&Load")								;select menu : file->load
sleep(1000)
	if WinWait($mWin,"Configuration File is modified",2) Then
		ControlClick($mWin,"","[CLASS:Button; INSTANCE:2]","left")
		sleep(1000)
	EndIf
sleep(150)
	if winexists($ldCfx) Then
		ControlSend($ldCfx,"","[CLASS:Edit; INSTANCE:1]",@DesktopDir&"\"&$fName)					;load cfx file
		ControlClick($ldCfx,$loadBTN,"[CLASS:Button; INSTANCE:2]","")
		sleep(1000)
		if winexists($ldCfx,$fName) Then														;if the cfx file doesn't exists then create a new one
			ControlClick($ldCfx,"","[CLASS:Button; INSTANCE:1]","left")
			sleep(1000)
			ControlClick($ldCfx,"","[CLASS:Button; INSTANCE:3]","left")
			sleep(1000)
			$sMenu=WinMenuSelectItem($mWin,"","&File","Confi&guration","&New")					;select menu : file->new
			sleep(1000)
			if WinWait("","Configuration File is modified",2) Then
				ControlClick($mWin,"","[CLASS:Button; INSTANCE:2]","left")
				sleep(1000)
			EndIf
			sleep(2000)
			if winexists($nwCfx) Then
				ControlSend($nwCfx,"","[CLASS:Edit; INSTANCE:1]",@DesktopDir&"\"&$fName)
				send("{ENTER}")
				sleep(1000)
				ControlClick($nwCfx,"","[CLASS:Button; INSTANCE:2]","left")
				sleep(1000)
				if winexists($hWare) Then
					ControlClick($hWare,$selectBTN,"[CLASS:Button; INSTANCE:2]")
					sleep(1000)
					ControlClick($hWare,$selectBTN,"[CLASS:Button; INSTANCE:2]")
					sleep(1000)
					ControlClick($hWare,$okBTN,"[CLASS:Button; INSTANCE:4]")
					sleep(1000)
				EndIf
			EndIf
		EndIf
	EndIf
EndIf
EndFunc

;==========================================================
;Function Name : _loadDatabase
;Functionality : loads or creates a CAN database file
;Input 		   : file Name to be loaded/created
;Output 	   : returns true if created/loaded else false
;==========================================================

Func _loadCANDatabase($dbFName,$noOfMsg,$msgLen)
	$lDB=0
	$cDB=0
	$msgCount=0
	$sigCount=0
if winexists($mWin) Then
$dbMenu=WinMenuSelectItem($mWin,"","&File","&Database","&Open")									;select menu : file->load
sleep(1000)
	if $dbMenu=0 Then
		msgbox(0,"","Test execution Failed...'Database' Menu not found!!!can't proceed further")
		Exit
	EndIf
sleep(150)
if winexists($oDbf) Then
	ControlSend($oDbf,"","[CLASS:Edit; INSTANCE:1]",@DesktopDir&"\"&$dbFName)			;load dbf file
	$lDb=ControlClick($oDbf,"","[CLASS:Button; INSTANCE:2]","left")
	sleep(1000)
		if winexists($mWin,"Specified database file is not found.") Then							; if the database file doesn't exists then create a new one
			ControlClick($mWin,"OK","[CLASS:Button; INSTANCE:1]","left")
			sleep(1000)
			$dbMenu=WinMenuSelectItem($mWin,"","&File","&Database","&New")
			sleep(1000)
			if $dbMenu=0 Then
				msgbox(0,"","Test execution Failed...'New' Menu not found!!!can't proceed further")
				Exit
			EndIf
			if winexists($saveAs) Then
				ControlSend($saveAs,"","[CLASS:Edit; INSTANCE:1]",@DesktopDir&"\"&$dbFName)
				sleep(1000)
				ControlClick($saveAs,"","[CLASS:Button; INSTANCE:2]","left")
				sleep(1000)
			EndIf
			sleep(2000)
			if Winexists($dbEditCAN) Then
				for $i=0 to $noOfMsg-1
				$pos=ControlGetPos ($dbEditCAN, "", "[CLASS:SysTreeView32; INSTANCE:1]" )
				MouseClick("right",$pos[0]+30,$pos[1]+55)															; Add new message
				send("n")
				sleep(1000)
					if WinExists($msgDtails) Then
						send("msg"&$i+1)
						send("{TAB}")
						send($i)
						send("{TAB}")
						send($msgLen)
						ControlClick($msgDtails,"",$ok1BTN)
						sleep(1000)
						ControlClick($dbEditCAN, $nSigBTN, 1075) 											; Add Signal
						sleep(1000)
						if WinExists($sigDetails) then
							send("SIG"&$i+1)
							send("{TAB}")
							send("i")
							send("{TAB}")
							send(0)
							send("{TAB}")
							send("{TAB}")
							send($msgLen*8)
							ControlClick($sigDetails,"",$ok1BTN)
							sleep(1000)
						EndIf
					EndIf
				Next
			EndIf
		EndIf
EndIf
WinMenuSelectItem($mWin,"","&File","&Database","&Save")
sleep(1000)
EndIf
EndFunc

;============================================
;Function Name : _associateDB
;Functionality : associates a database file
;Input 		   : file Name to be associated
;Output 	   : -
;============================================

Func _associateDB($menu,$afName)
	ConsoleWrite("_associateDB Function"&@CRLF)
	$sel=WinMenuSelectItem($dbEditCAN,"","&File","&Database","&Associate...")
	sleep(1000)
	ConsoleWrite("$sel : "&$sel&@CRLF)
	if winexists($asoDB) Then
		ControlSend($asoDB,"","[CLASS:Edit; INSTANCE:1]",@DesktopDir&"\"&$afName)
		ControlClick($asoDB,"","[CLASS:Button; INSTANCE:2]","left")
		sleep(1000)
	EndIf
EndFunc

;===============================================================================
;Function Name : _hdWareSelect
;Functionality : Selects Kvaser CAN Hardware
;Input 		   : -
;Output 	   :
;===============================================================================

Func _hdWareSelect($HWdriver)
	if winexists($mWin) Then
		sleep(500)
		WinMenuSelectItem($mWin,"","&Configure","&Hardware Interface",$HWdriver)   ;"&Kvaser CAN"
		sleep(1000)
		if winexists($hWare) Then
			ControlClick($hWare,$selectBTN,"[CLASS:Button; INSTANCE:2]")
			sleep(1000)
			ControlClick($hWare,$selectBTN,"[CLASS:Button; INSTANCE:2]")
			sleep(1000)
			ControlClick($hWare,$okBTN,"[CLASS:Button; INSTANCE:4]")
			sleep(1000)
		Endif
	EndIf
EndFunc

;===============================================================================
;Function Name : _txMSG
;Functionality : configures transmission messages
;Input 		   : txMode(Cyclic or Monoshot),$blckCount:no. of blocks to be added
;Output 	   : -
;===============================================================================

Func _txMSG($txMode,$blkCount)
if winexists($mWin) Then
	sleep(1000)
	controlCommand($mWin,"",17000,"Check")
	for $i=0 to $blckCount-1
		ControlClick($mWin,"","[CLASS:Button; INSTANCE:14]","left")
		sleep(1000)
		ControlSetText($mWin,"",1228,"BLOCK"&$i)
		if $txMode="Monoshot" Then
			ControlCommand($mWin,"",1008,"Check")
			sleep(1000)
		EndIf
		ControlSetText($mWin,"",1134,1)
		ControlFocus ( $mWin, "", 1221)
		$hWnd = ControlGetHandle($mWin,"",1221)
		_GUICtrlComboBoxEx_ShowDropDown($hWnd)
		if $i=1 then
			_GUICtrlComboBoxEx_SetCurSel($hWnd,$i+1)
			ControlCommand($mWin,"",1213,"Check")
			Sleep(1000)
			controlCommand($mWin,"",1215,"Check")
			sleep(1000)
			ControlClick($mWin,"","[CLASS:Button; INSTANCE:4]","left")
			sleep(1000)
			_GUICtrlComboBoxEx_SetCurSel($hWnd,$i+2)
			ControlCommand($mWin,"",1213,"Check")
			Sleep(1000)
			controlCommand($mWin,"",1215,"Check")
			sleep(1000)
			ControlClick($mWin,"","[CLASS:Button; INSTANCE:4]","left")
			sleep(1000)
		Else
			_GUICtrlComboBoxEx_SetCurSel($hWnd,$i)
			ControlCommand($mWin,"",1213,"Check")
			Sleep(1000)
			controlCommand($mWin,"",1215,"Check")
			sleep(1000)
			ControlClick($mWin,"","[CLASS:Button; INSTANCE:4]","left")
			sleep(1000)
			_GUICtrlComboBoxEx_SetCurSel($hWnd,$i+1)
			ControlCommand($mWin,"",1213,"Check")
			Sleep(1000)
			controlCommand($mWin,"",1215,"Check")
			sleep(1000)
			ControlClick($mWin,"","[CLASS:Button; INSTANCE:4]","left")
			sleep(1000)
		EndIf
	next
	ControlClick($mWin,"","[CLASS:Button; INSTANCE:23]","left")
	sleep(1000)
EndIf
EndFunc

;===============================================================================
;Function Name : _enableLog
;Functionality : Add a log file
;Input 		   : -
;Output 	   : -
;===============================================================================

Func _enableLog()
if winexists($mWin) Then
	sleep(2000)
	$logMenu=WinMenuSelectItem($mWin,"","&Configure","L&og")
	if winexists($cfgLog) Then
		$addBtn=ControlClick($cfgLog,"&Add","")
		sleep(1000)
		$enableLog=ControlCommand($cfgLog,"",1092,"Check")
		sleep(1000)
		ControlClick($cfgLog,"",1122)
		sleep(2000)
		if WinExists("Select a Log file") Then
			$name=StringReplace(_Now(),"/","-")
			$lfname=StringReplace($name,":","-")
			ControlSetText("Select a Log file","","[CLASS:Edit; INSTANCE:1]",$lfname&".log")
			ControlClick("Select a Log file","","[CLASS:Button; INSTANCE:2]")
			$oK=ControlClick($cfgLog,"&OK","[CLASS:Button; INSTANCE:14]")
		EndIf
	EndIf
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
if winexists($mWin) Then
$dbMenu=WinMenuSelectItem($mWin,"","&J1939","&Database","&Open")									;select menu : file->load
sleep(1000)
	if $dbMenu=0 Then
		msgbox(0,"","Test execution Failed...'Database' Menu not found!!!can't proceed further")
		Exit
	EndIf
sleep(150)
	if winexists($selJ1939Fl) Then
		ControlSend($selJ1939Fl,"","[CLASS:Edit; INSTANCE:1]",@DesktopDir&"\"&$dbFName)			;load dbf file
		$lDb=ControlClick($selJ1939Fl,"","[CLASS:Button; INSTANCE:2]","left")
		sleep(1000)
		if winexists($mWin,"Specified database file is not found.") Then							; if the database file doesn't exists then create a new one
			ControlClick($mWin,"OK","[CLASS:Button; INSTANCE:1]","left")
			sleep(1000)
			$dbMenu=WinMenuSelectItem($mWin,"","&J1939","&Database","&New")
			sleep(1000)
			if $dbMenu=0 Then
				msgbox(0,"","Test execution Failed...'New' Menu not found!!!can't proceed further")
				Exit
			EndIf
			if winexists($saveAs) Then
				ControlSend($saveAs,"","[CLASS:Edit; INSTANCE:1]",@DesktopDir&"\"&$dbFName)
				sleep(1000)
				$cDB=ControlClick($saveAs,"","[CLASS:Button; INSTANCE:2]","left")
				sleep(1000)
			EndIf
			if Winexists($dbEditJ1939) Then
				$pos=ControlGetPos ($dbEditJ1939, "", "[CLASS:SysTreeView32; INSTANCE:1]" )
				MouseClick("right",$pos[0]+30,$pos[1]+55)															; Add new message
				send("n")
				sleep(1000)
				if WinExists($msgDtails) Then
					send("msg1")
					send("{TAB}")
					send($PGN)
					send("{TAB}")
					send($msgLen)
					ControlClick($msgDtails,"","OK")
					sleep(1000)
					$noOfSigs=($msgLen*8)/$sgLen
					ConsoleWrite("$noOfSigs : "&$noOfSigs&@CRLF)
					for $i=0 to $noOfSigs-1
						ControlClick($dbEditJ1939, "New Signal...", 1075) 						; Add Signal
						sleep(1000)
						if WinExists($sigDetails) then
							send("SIG"&$i+1)
							send("{TAB}")
							send("i")
							send("{TAB}")
							send($i)
							send("{TAB}")
							send("{TAB}")
							send($sgLen)
							ControlClick($sigDetails,"","OK")
							sleep(1000)
						EndIf
					Next
				EndIf
				WinMenuSelectItem($mWin,"","&J1939","&Database","&Save")
			EndIf
		EndIf
	EndIf
EndIf
EndFunc

;============================================
;Function Name : _associateDB
;Functionality : associates a database file
;Input 		   : file Name to be associated
;Output 	   : -
;============================================

Func _asoDB($menu,$afName)
	$flag=0
	$dbMenu=WinMenuSelectItem($mWin,"",$menu,"&Database","&Associate")
	if winexists($asoDB) Then
		$flag=1
		ControlSend($asoDB,"","[CLASS:Edit; INSTANCE:1]",@DesktopDir&"\"&$afName)
		$aDB=ControlClick($asoDB,"","[CLASS:Button; INSTANCE:2]","left")
		sleep(1000)
	EndIf
EndFunc

