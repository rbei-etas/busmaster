#include <GuiStatusBar.au3>
#include <Excel.au3>
#include <GuiComboBoxEx.au3>
#include <GuiListView.au3>
#include <GuiMenu.au3>

Global $app, $rVal


;==========================================================================================================
;Function Name : _launchApp
;Functionality : This function checks for the existence of the application. If not found then launches it.
;Input 		   : -
;Output		   : If the app is launched then it returns the $rVal=1 to calling function
;==========================================================================================================

Func _launchApp()
	$rVal = 1
	ConsoleWrite("launchApp function" & @CRLF)
	Sleep(1000)
	$app = WinExists($mWin) 														;Check if the BUSMASTER window exists
	ConsoleWrite("app exists res: " & $app & @CRLF)
	If $app = 0 Then 																;if not found then run the app
		$rVal = Run(@ProgramFilesDir&"\BUSMASTER\BUSMASTER.exe")
		ConsoleWrite("App search result: " & $rVal & @CRLF)
		Sleep(2000)
		If WinWaitActive($mWin, "", 1) Then
			ControlClick($mWin, "OK", "[CLASS:Button; INSTANCE:1]")
			Sleep(2000)
		EndIf
		Return $app
		Return $rVal
	EndIf
	If WinExists($dlghWare) Then 													; if a Hardware Selection dialog appears then click 'OK'
		ControlClick($dlghWare, $selectBTN, $hwselBTNctrlID)
		Sleep(1000)
		ControlClick($dlghWare, $selectBTN, $hwselBTNctrlID)
		Sleep(1000)
		ControlClick($dlghWare, "",$hwselOKBTNctrlID)
		Sleep(1000)
	EndIf
EndFunc   ;==>_launchApp


;==========================================================================================================
;Function Name 		: _createConfig
;Functionality 		: This function creates cfx file
;Input 		   		: Config File Name
;Output		   		: -
;==========================================================================================================

Func _createConfig($cfxFName)
	$sMenu=WinMenuSelectItem($mWin,"",$fileMenu,$configMenu,$newMenu)		; Select File->Configuration->New
	sleep(1000)
	if WinWait($mWin,$saveConfigtxt,2) Then									; wait for save configuration dialog
		ControlClick($mWin,"",$saveCfxyesBTNctrlID)							; Click on Yes button
	EndIf
	sleep(1000)
	$DirPath = _SetOneFolderUp()											; Set the DirPath to save the dbf file
	if WinExists($DlgDefaultConfigSave,"") Then
		ControlSend($DlgDefaultConfigSave,"",$DefConfigFilePath,$DirPath&"\defaultFile")
		ControlClick($DlgDefaultConfigSave,"",$DefConfigFileSaveBTNinst2)
	EndIf
	sleep(1000)
	if winexists($dlgnwCfx) Then
		ControlSend($dlgnwCfx,"",$newCfxfNamectrlID,$DirPath&"\"&$cfxFName)	; Set the filename in 'New configuration filename' dialog
		sleep(500)
		$cConfig=ControlClick($dlgnwCfx,"",$savebtncfxInst2)				; Click on Save button
		sleep(2500)
	EndIf
EndFunc


;==========================================================================================================
;Function Name 		: _loadConfig
;Functionality 		: loads cfx file
;Input 		   		: Config File Name
;Output		   		: -
;==========================================================================================================

Func _loadConfig($cfxFName)
	$sMenu=WinMenuSelectItem($mWin,"",$fileMenu,$configMenu,$loadMenu)		; Select File->Configuration->Load
	sleep(1000)
	if WinWait($mWin,$saveConfigtxt,2) Then									; wait for save configuration dialog
		ControlClick($mWin,"",$saveCfxyesBTNctrlID)							; Click on Yes button
	EndIf
	sleep(1000)
	$DirPath = _SetOneFolderUp()											; Set the DirPath to save the dbf file
	if winexists($dlgldCfx) Then
		ControlSend($dlgldCfx,"",$loadCfxfNamectrlID,$DirPath&"\"&$cfxFName)	; Set the filename in 'Load Configuration Filename...' dialog
		sleep(500)
		$cConfig=ControlClick($dlgldCfx,"",$loadCfxloadInst2)				; Click on Load button
		sleep(2500)
	EndIf
EndFunc



;==========================================================================================================
;Function Name 		: _createCANDB
;Functionality 		: This function creates a database file
;Input 		   		: DBF File Name
;Output		   		: -
;==========================================================================================================

Func _createCANDB($dbFName)
	$dbMenu=WinMenuSelectItem($mWin,"",$fileMenu,$databaseMenu,$newMenu)
	sleep(1000)
	$DBFolderPath = _SetOneFolderUp()										; Set the DirPath to save the dbf file
	if winexists($dlgNewDBFile) Then
		ControlSend($dlgNewDBFile,"",$saveAsfName,$DBFolderPath&"\"&$dbFName)  ; Set Filename
		sleep(1000)
		$cDB=ControlClick($dlgNewDBFile,"",$savebtnDBInst2,"left") 				; Click on Save button
		sleep(2500)
	EndIf
EndFunc

;==========================================================================================================
;Function Name 		: _openCANDB
;Functionality 		: This function opens a CAN database file
;Input 		   		: DBF File Name
;Output		   		: -
;==========================================================================================================

Func _openCANDB($dbFName)
	$dbMenu=WinMenuSelectItem($mWin,"",$fileMenu,$databaseMenu,$openMenu)
	sleep(1000)
	$DBFolderPath = _SetOneFolderUp()											; Set the DirPath to save the dbf file
	if winexists($dlgoDbf) Then
		ControlSend($dlgoDbf,"",$selDbffNameCtrlID,$DBFolderPath&"\"&$dbFName)  ; Set Filename
		sleep(1000)
		$cDB=ControlClick($dlgoDbf,"",$opendbfBTNInst2,"left") 					; Click on Save button
		sleep(2500)
	EndIf
EndFunc

;==========================================================================================================
;Function Name 		: _DBmessage
;Functionality 		: This function selects either 'Edit message' or 'Delete message' from the right click menu of the msg
;Input 		   		: $menu("e" for 'Edit Message' or "d" for 'Delete Message'
;Output		   		: -
;==========================================================================================================

Func _DBmessage($menu)
	$DBTreeVwHWD = ControlGetHandle($windbEditCAN, "", $tvInst1) 					; Fetch tree view control handle
	$DbtreeVwparent = _GUICtrlTreeView_GetFirstItem($DBTreeVwHWD) 					; Fetch Parent node handle
	$childHWD = _GUICtrlTreeView_GetFirstItem($DBTreeVwHWD)
	ConsoleWrite("$DbtreeVwparent :"&$DbtreeVwparent & @CRLF)
	ConsoleWrite("$childHWD :"&$childHWD & @CRLF)
	_GUICtrlTreeView_ClickItem($DBTreeVwHWD, $childHWD, "Right") 					; Right Click on the parent node
	Send($menu) 																	; Select New message menu
	Sleep(1000)
EndFunc   ;==>_DBmessage

;==========================================================================================================
;Function Name 		: _addMsgDetails
;Functionality 		: This function adds or updates the message details in the 'Message Details' dialog
;Input 		   		: $msgName : Message Name,$msgID : Message ID, $msgLen : Message Length
;Output		   		: -
;==========================================================================================================

Func _addMsgDetails($msgName, $msgID, $msgLen)
	ControlSetText($dlgmsgDtails, "", $msgNamectrlID, $msgName) 					; Enter the msg name
	ControlSetText($dlgmsgDtails, "", $msgIDctrlID, $msgID) 						; Enter the msg ID
	ControlSetText($dlgmsgDtails, "", $msglenctrlID, $msgLen) 						; Enter the msg len
	ControlClick($dlgmsgDtails, "", $msgdetOKctrlID) 								; Click 'OK' button
	sleep(1500)
EndFunc   ;==>_addMsgDetails

;==========================================================================================================
;Function Name 		: _addSigDetails
;Functionality 		: This function adds or updates the Signal details in the 'Signal Details' dialog
;Input 		   		: $msgName : Message Name,$msgID : Message ID, $msgLen : Message Length
;Output		   		: -
;==========================================================================================================

Func _addSigDetails($sigType, $sigName, $sigLen, $sigByteInd, $sigStartBit)
	Sleep(1000)
	ControlCommand($dlgsigDetails, "", $sigTypectrlID, "SelectString", $sigType) 	; Select signal type from the combobox
	sleep(500)
	ControlSetText($dlgsigDetails, "", $sigNamectrlID, $sigName) 					; Add signal name
	sleep(500)
	ControlSetText($dlgsigDetails, "", $sigByteIndctrlID, $sigByteInd) 				; Byte Index
	sleep(500)
	ControlSetText($dlgsigDetails, "", $sigStartBitctrlID, $sigStartBit) 			; Start Bit
	sleep(500)
	ControlSetText($dlgsigDetails, "", $sigLenctrlID, $sigLen) 						; enter signal length
	sleep(500)
;~ 	controlfocus($dlgsigDetails,"",$sigMinValctrlID)								; Set focus to 'Min Val' edit box
	send("{TAB}")
	send("{TAB}")
	send("{TAB}")
	send("{TAB}")
	sleep(1000)
	ControlClick($dlgsigDetails, "", $sigDetOKctrlID) 								; Click 'OK' button
EndFunc   ;==>_addSigDetails

;==========================================================================================================
;Function Name 		: _addSigDesc
;Functionality 		: This function adds or updates the Signal description in the 'Enter Value Descriptor and Value' dialog
;Input 		   		: $descName : Description Name,$value : Description Value
;Output		   		: -
;==========================================================================================================

Func _addSigDesc($descName, $value)
	Sleep(500)
	ControlSetText($dlgsigDesc, "", $sigDescNamectrlID, $descName) 					; Add signal desc name
	ControlSetText($dlgsigDesc, "", $sigDescValctrlID, $value) 						; Add signal desc value
	ControlClick($dlgsigDesc, "", $sigDescOKctrlID) 								; Click 'OK'
	Sleep(500)
EndFunc   ;==>_addSigDesc

;==========================================================================================================
;Function Name 		: _invalidSigDescMsg()
;Functionality 		: This function clicks on 'yes' button of "Changing Signal Range will delete invalid signal description(s)" warning pop up message
;Input 		   		:
;Output		   		:
;==========================================================================================================

Func _invalidSigDescMsg()
	sleep(800)
	controlclick($dlgBusmaster,"",$sigRangeYesctrlID)
EndFunc

;==========================================================================================================
;Function Name 		: _SelectSigDesc
;Functionality 		: This function clicks on the specified index of Signal details list view
;Input 		   		:
;Output		   		:
;==========================================================================================================

Func _SelectSigDesc($index)
	$sigDetlvhwd=controlgethandle($mWin,"",$sigDetlvInst1)								; Get handle of signal details list view
	_GUICtrlListView_ClickItem($sigDetlvhwd,$index)										; Click on the first item in the Signal details list view
	$sigDesclvhwd=controlgethandle($mWin,"",$sigDesclvInst1)							; Get handle of signal details list view
	_GUICtrlListView_ClickItem($sigDesclvhwd,$index)									; Click on the first item in the Signal details list view
	sleep(250)
EndFunc

;==========================================================================================================
;Function Name 		: _saveCloseDB
;Functionality 		: This function saves the Database and Closes it
;Input 		   		:
;Output		   		:
;==========================================================================================================

Func _saveCloseCANDB()
	WinMenuSelectItem($mWin,"",$fileMenu,$databaseMenu,$saveMenu)				; Select File->Database->Save
	sleep(2000)
	if winexists($mWin,$SaveDBImporttxt) Then
		consolewrite("Dialog Exists"&@CRLF)
		ControlClick($mWin,"",$yesBTNctrlID)
	EndIf
	WinMenuSelectItem($mWin,"",$fileMenu,$databaseMenu,$closeMenu)				; Select File->Database->Close
EndFunc

;==========================================================================================================
;Function Name 		: _AssociateDB
;Functionality 		: This function Associates the database
;Input 		   		: Database filename
;Output		   		:
;==========================================================================================================

Func _AssociateCANDB($dbFName)
	WinMenuSelectItem($mWin,"",$fileMenu,$databaseMenu,$AssocCANDB)				; Select File->Database->Associate
	sleep(1000)
	$DBFolderPath = _SetOneFolderUp()											; Set the DirPath to save the dbf file
	if winexists($dlgasoDB) Then
		ControlSend($dlgasoDB,"",$AssocfNamectrlID,$DBFolderPath&"\"&$dbFName)  ; Set Filename
		sleep(1000)
		$cDB=ControlClick($dlgasoDB,"",$assocdbfBTNinst2,"left") 				; Click on open button
		sleep(1000)
	EndIf
EndFunc

;==========================================================================================================
;Function Name 		: _DissociateCANDB
;Functionality 		: This function Dissociates the database
;Input 		   		: Database filename
;Output		   		:
;==========================================================================================================

Func _DissociateCANDB($index)
	WinMenuSelectItem($mWin,"",$fileMenu,$databaseMenu,$DissocCANDB)			; Select File->Database->Dissociate
	sleep(1000)
	$DBFolderPath = _SetOneFolderUp()											; Set the DirPath to save the dbf file
	if winexists($disDbDialog) Then
		$disDBlbHWD=ControlGetHandle($disDbDialog,"",$dissocdbflbinst1)			; Fetch the handle of dissociate Db list box
		_GUICtrlListBox_ClickItem($disDBlbHWD,$index)							; Click on the specified index
		controlclick($disDbDialog,"",$dissocBTNctlID)							; Click on Dissociate button
	EndIf
	sleep(1000)
EndFunc

;==========================================================================================================
;Function Name 		: _DelSignalDesc()
;Functionality 		: This function Deletes the signal description
;Input 		   		:
;Output		   		:
;==========================================================================================================


Func _DelSignalDesc()
	$sigDesclvhwd=controlgethandle($mWin,"",$sigDesclvInst1)							; Get handle of signal details list view
	for $i=0 to _GUICtrlListView_GetItemCount($sigDesclvhwd)-1
		_GUICtrlListView_ClickItem($sigDesclvhwd,0)										; Click on the first item in the Signal details list view
		sleep(800)
		controlclick($mWin,"",$delSigDescctrlID)										; Delete Signal Desc
		sleep(1500)
		$DelSignalDescPopUp=0
		if winexists("",$delSigDesctxt) Then
			$DelSignalDescPopUp=1
			controlclick($mWin,"",$yesBTNctrlID)										; Click Yes button
		EndIf
	Next
	return $DelSignalDescPopUp
EndFunc

;===============================================================================
;Function Name : _hdWareSelect
;Functionality : Selects Kvaser CAN Hardware
;Input 		   : -
;Output 	   : -
;===============================================================================

Func _hdWareSelectKvaser()
	if winexists($mWin) Then
		sleep(500)
		WinMenuSelectItem($mWin,"",$configHWMenu,$HWIMenu,$kvaserHWMenu)
		sleep(1000)
		if winexists($dlghWare) Then
			ControlClick($dlghWare, $selectBTN, $hwselBTNctrlID)
			Sleep(1000)
			ControlClick($dlghWare, $selectBTN, $hwselBTNctrlID)
			Sleep(1000)
			ControlClick($dlghWare, "",$hwselOKBTNctrlID)
			Sleep(1000)
		Endif
	EndIf
	Return $channelCount
EndFunc

;===============================================================================
;Function Name : _TxMsgMenu
;Functionality : Selects Tx Messages from the menu
;Input 		   :
;Output 	   :
;===============================================================================

Func _TxMsgMenu()
	WinMenuSelectItem($mWin,"",$configTxmenu,$TxMsgMenu)									; Select Configure->App Filters
EndFunc


;===============================================================================
;Function Name : _AddMsgBlock
;Functionality : Adds a message block
;Input 		   :
;Output 	   :
;===============================================================================

Func _AddMsgBlock()
	ControlClick($mWin,"",$addMsgBlockctrlID)											; Click on 'Add' block message
	sleep(500)
EndFunc

;===============================================================================
;Function Name : _AddMsg2TxList
;Functionality : Adds a message to the TX list
;Input 		   :
;Output 	   :
;===============================================================================

Func _AddMsg2TxList()
	ControlClick($mWin,"",$addTxMsgBTNctrlID)										; Click on 'Add' block message
	sleep(500)
EndFunc

;===============================================================================
;Function Name : _CloseTxWindow
;Functionality : Closes the TX window
;Input 		   :
;Output 	   :
;===============================================================================

Func _CloseTxWindow()
	ControlClick($mWin,"",$closeTxWinBTNctrlID)										; Click on Close button
	sleep(500)
EndFunc


;===============================================================================
;Function Name : _AutoUpdate
;Functionality : Checks Auto Update button if unchecked
;Input 		   :
;Output 	   :
;===============================================================================

Func _EnableAutoUpdate()
	if (ControlCommand($mWin,"",$autoUpdatectrlID,"IsChecked")=0) Then
		ControlCommand($mWin,"",$autoUpdatectrlID,"Check")								; Check 'Autoupdate' check box is enabled
	EndIf
EndFunc

;===============================================================================
;Function Name : _AppFilterMenu
;Functionality : Selects App filter from the menu
;Input 		   :
;Output 	   :
;===============================================================================

Func _AppFilterMenu()
	WinMenuSelectItem($mWin,"",$configureMenu,$AppFilterMenu)								; Select Configure->App Filters
	sleep(1000)
EndFunc

;===============================================================================
;Function Name : _AddFilter
;Functionality : Configures Filter details for CAN
;Input 		   :
;Output 	   :
;===============================================================================
Func _AddFilter($filterType,$msgID,$IDType,$Frame,$Dir,$Channel)
	sleep(750)
	if $filterType="Stop" Then
		ControlClick($dlgConfigfilter,"",$addFilterBTNctrlID)							; Click on 'Add' filter button
		sleep(750)
	Else
		ControlClick($dlgConfigfilter,"",$addFilterBTNctrlID)							; Click on 'Add' filter button
		sleep(750)
		$hWnd = ControlGetHandle($dlgConfigfilter,"",$filterList)
		$FilTypePos=_GUICtrlListView_GetItemPosition($hWnd, 0)							; Get position of Signal details list view control
		ControlClick($dlgConfigfilter,"",1000,"Left",2,$FilTypePos[0]+100,$FilTypePos[1])	; Double click on the 'Filter Type'
		sleep(500)
		send("{DOWN}")
		sleep(500)
		send("{ENTER}")																	; Select 'Pass' filter
	EndIf
	ControlCommand($dlgConfigfilter,"",$msgIDFilterctrlID,"SelectString", $msgID)		; Select the MSG from ID combo box
	sleep(200)
	ControlCommand($dlgConfigfilter,"",$msgIDtypectrlID,"SelectString", $IDType)		; Select the ID type
	sleep(200)
	ControlCommand($dlgConfigfilter,"",$msgtypectrlID,"SelectString", $Frame)			; Select the Frame
	sleep(200)
	ControlCommand($dlgConfigfilter,"",$msgDirectionctrlID,"SelectString", $Dir)		; Select the Direction
	sleep(200)
	ControlCommand($dlgConfigfilter,"",$msgChannelctrlID,"SelectString", $Channel)		; Select the Channel
	sleep(200)
	ControlClick($dlgConfigfilter,"",$addFiltertoListctrlID)							; Click on Add button
	sleep(750)
	ControlClick($dlgConfigfilter,"",$OKFilterBTNctrlID)								; Click on OK button
	sleep(500)
EndFunc


;===============================================================================
;Function Name : _MsgDisplayMenu
;Functionality : Selects message display from the menu
;Input 		   :
;Output 	   :
;===============================================================================

Func _MsgDisplayMenu()
	WinMenuSelectItem($mWin,"",$configureMenu,$MsgDisplay)									; Select Configure->msg display
EndFunc


;===============================================================================
;Function Name : _AddFiltertoMsgDisp
;Functionality : Adds the filter for message display for CAN
;Input 		   :
;Output 	   :
;===============================================================================

Func _AddFiltertoMsgDisp()
	sleep(100)
	ControlClick($dlgMsgDisp,"",$confMsgDispBTNctrlID)									; Click Configure button in msg display
	sleep(750)
	if winexists($dlgFilSel) Then
		$fltSelhWd=controlgethandle($dlgFilSel,"",$configFilMsgDisp)					; Get handle of filter selection list view
		_GUICtrlListView_ClickItem($fltSelhWd,0)										; Click on the first filter
		ControlClick($dlgFilSel,"",$AddFilMsgDispBTNctrlID)								; Click on Add button
		sleep(750)
		ControlClick($dlgFilSel,"",$okFilSelDlgBTNctrlID)								; Click on OK button
		sleep(750)
	EndIf
	ControlClick($dlgMsgDisp,"",$okConMsgDispBTNctrlID)									; Click on OK button
EndFunc

;===============================================================================
;Function Name : _EnableFilterDispMenu
;Functionality : Enable filters for message display from menu
;Input 		   :
;Output 	   :
;===============================================================================

Func _Enable_DisableFilterDispMenu()
	WinMenuSelectItem($mWin,"",$FunctionsMenu,$enableFilterMenu,$displayMenu)			; Select Function->Enable Filter->Display
EndFunc


;===============================================================================
;Function Name : _DisableFilterDispMenu
;Functionality : Disable filters for message display from menu
;Input 		   :
;Output 	   :
;===============================================================================

Func _DisableFilterDispMenu()
	$cntToolhWd=ControlGetHandle($mWin,"",$connectToolbarctrlID)						; Get handle of the 'Disable Msg Disp Filter' toolbar
	_GUICtrlToolbar_ClickIndex($cntToolhWd,9)											; Click on 'Disable Msg Disp Filter' icon
	sleep(750)
EndFunc

;===============================================================================
;Function Name : _Connect()
;Functionality : Clicks on the connect button in the toolbar
;Input 		   :
;Output 	   :
;===============================================================================

Func _ConnectDisconnect()
	$cntToolhWd=ControlGetHandle($mWin,"",$connectToolbarctrlID)						; Get handle of the 'Connect/Disconnect' toolbar
	_GUICtrlToolbar_ClickIndex($cntToolhWd,4)											; Click on 'Connect' icon
	sleep(750)
EndFunc

;===============================================================================
;Function Name : _TransmitMsgsMenu()
;Functionality : Select Transmit messages from the menu
;Input 		   :
;Output 	   :
;===============================================================================

Func _TransmitMsgsMenu()
	WinMenuSelectItem($mWin,"",$FunctionsMenu,$transmitMenu,$normalBlckMenu)			; Click on 'Transmit normal blocks' menu
EndFunc


;===============================================================================
;Function Name : _CANLogMenu()
;Functionality : Open CAN Log Dialog
;Input 		   :
;Output 	   :
;===============================================================================

Func _CANLogMenu()
	WinMenuSelectItem($mWin,"",$configureMenu,$LogMenu)								; Select Configure->Log
	Sleep(1000)
EndFunc


;===============================================================================
;Function Name : _ConfigureCANLog()
;Functionality : Configure Logging for CAN
;Input 		   : Log File Name, Time Mode, Channel #, Numeric Format, File Mode, Start Rec MsgID, Stop Rec MsgID, Filters={True,False}
;Output 	   :
;===============================================================================

Func _ConfigureCANLog($logFileName,$TimeMode,$Channel,$NumFormat,$FileMode,$Start_Rec,$Stop_Rec,$Filters)
	WinMenuSelectItem($mWin,"",$configureMenu,$LogMenu)								; Select Configure->Log
	Sleep(1000)
	ControlClick($dlgcfgLog,"",$addLogBTNctrlID)									; Click on Add button
	sleep(250)
	ControlCommand ($dlgcfgLog,"",$enableDisLogctrlID,"Check")						; Check Enable/Disable Logging during connect/disconnect
	$LogFilePath=_SetOneFolderUp()
	ControlClick($dlgcfgLog,"",$logFilePathctrlID)									; Click on Log File Path button
	sleep(1000)
	if winexists($dlgSelLogFile) Then
		controlsend($dlgSelLogFile,"",$SelLogFnamectrlID,$LogFilePath & "\" & $logFileName)	; Enter the Log file Name
		controlclick($dlgSelLogFile,"",$savebtnlogInst2)									; Click on 'Save' button
	EndIf
	controlcommand($dlgcfgLog,"",$LogTimeModectrlID,"SelectString", $TimeMode)		; Set the time mode
	controlcommand($dlgcfgLog,"",$LogChannelctrlID,"SelectString", $Channel)		; Set the Channel
	if $NumFormat="Hex" Then
		Controlcommand($dlgcfgLog,"",$LogNumModeHex,"Check")						; Set the Numeric mode to Hex
	Else
		Controlcommand($dlgcfgLog,"",$LogNumModeDec,"Check")						; Set the Numeric mode to Decimal
	EndIf
	sleep(1000)
	if $FileMode="Overwrite" Then
		Controlcommand($dlgcfgLog,"",$LogfileModeOverWrite,"Check")					; Set the File mode
	Else
		Controlcommand($dlgcfgLog,"",$LogfileModeApp,"Check")					; Set the File mode
	EndIf
	if $Start_Rec<>"" and $Stop_Rec<>"" Then
		Controlcommand($dlgcfgLog,"",$LogStartTrigger,"Check")						; enable the start trigger
		ControlSend($dlgcfgLog,"",$LogStartMsgID,$Start_Rec)						; Set the Start trigger Msg ID
		Controlcommand($dlgcfgLog,"",$LogStopTrigger,"Check")						; enable the stop trigger
		ControlSend($dlgcfgLog,"",$LogStopMsgID,$Stop_Rec)							; Set the Stop trigger Msg ID
	EndIf
	if $Filters="True" Then
		controlclick($dlgcfgLog,"",$LogFilterBTNctrlID)									; Click on 'Filter' button
		sleep(1000)
		if winexists($dlgFilSel) Then
			$fltSelhWd=controlgethandle($dlgFilSel,"",$configFilMsgDisp)					; Get handle of filter selection list view
			_GUICtrlListView_ClickItem($fltSelhWd,0)										; Click on the first filter
			ControlClick($dlgFilSel,"",$AddFilMsgDispBTNctrlID)								; Click on Add button
			sleep(800)
			ControlClick($dlgFilSel,"",$okFilSelDlgBTNctrlID)								; Click on OK button
			sleep(800)
		EndIf
	EndIf
	controlclick($dlgcfgLog,"",$LogOKBTNctrlID)										; Click on 'OK' button
	sleep(1000)
EndFunc


;===============================================================================
;Function Name : _EnableFilterLog
;Functionality : Enable filters for Logging
;Input 		   :
;Output 	   :
;===============================================================================

Func _EnableFilterLog()
	WinMenuSelectItem($mWin,"",$FunctionsMenu,$enableFilterMenu,$LogFilterMenu)			; Select Function->Enable Filter->Log
EndFunc


;===============================================================================
;Function Name : _DisableJ1939Win
;Functionality : Checks whether J1939 message window is active. If J1939 message window is active then disable it.
;Input 		   :
;Output 	   :
;===============================================================================

Func _DisableJ1939Win()
	send("!JV")																		; Select J1939->View from the menu
	sleep(1000)
	send("{ESC}")
	sleep(500)
	send("{ESC}")
	sleep(500)
	if winexists($J1939MsgWind) Then
		$winhWnd = WinGetHandle($mWin)												; Fetch the window handle
		$hMain = _GUICtrlMenu_GetMenu($winhWnd)										; Fetch the handle of the menu
		ConsoleWrite("menu handle : "&$hMain& @CRLF)
		$hFile = _GUICtrlMenu_GetItemSubMenu($hMain, 6)								; Fetch the handle of J1939 menu
		ConsoleWrite("Submenu handle : "&$hFile& @CRLF)
		$hSubmenu=_GUICtrlMenu_GetItemSubMenu($hFile, 2)							; Fetch the handle of J1939->View menu
		ConsoleWrite("$hSubmenu handle : "&$hSubmenu& @CRLF)
		$res=_GUICtrlMenu_GetItemChecked($hSubmenu,0,True)							; Check whether J1939->View->Message Window is checked or not
		$val=_GUICtrlMenu_GetItemText($hSubmenu,0,True)								; Fetch the text of first item in J1939->View submenu
		ConsoleWrite("$res : "&$res& @CRLF)
		ConsoleWrite("$val : "&$val& @CRLF)
		if $res="True" then															; If J1939->View->Message Window is enabled then uncheck it
			WinMenuSelectItem($mWin,"",$J1939Menu,$J1939ViewMenu,$J1939MsgWinMenu)
		EndIf
	Else
		$winhWnd = WinGetHandle($mWin)												; Fetch the window handle
		$hMain = _GUICtrlMenu_GetMenu($winhWnd)
		ConsoleWrite("menu handle : "&$hMain& @CRLF)
		$hFile = _GUICtrlMenu_GetItemSubMenu($hMain, 5)
		ConsoleWrite("Submenu handle : "&$hFile& @CRLF)
		$hSubmenu=_GUICtrlMenu_GetItemSubMenu($hFile, 2)
		ConsoleWrite("$hSubmenu handle : "&$hSubmenu& @CRLF)
		$res=_GUICtrlMenu_GetItemChecked($hSubmenu,0,True)
		$val=_GUICtrlMenu_GetItemText($hSubmenu,0,True)
		ConsoleWrite("$res : "&$res& @CRLF)
		ConsoleWrite("$val : "&$val& @CRLF)
		if $res="True" then															; If J1939->View->Message Window is enabled then uncheck it
			WinMenuSelectItem($mWin,"",$J1939Menu,$J1939ViewMenu,$J1939MsgWinMenu)
		EndIf
	EndIf
	send("!W1")																		; Select Window->1 Message Window CAN
	sleep(750)
EndFunc


;===============================================================================
;Function Name : _DisableOverwriteMode
;Functionality : Diables the Msg Window OVerwrite Mode
;Input 		   :
;Output 	   :
;===============================================================================

Func _DisableOverwriteMode()
	$OvrWriteToolhWd=ControlGetHandle($mWin,"",$OverwriteToolbarctrlID)				; Get handle of the 'Connect/Disconnect' toolbar
	_GUICtrlToolbar_ClickIndex($OvrWriteToolhWd,4)									; Click on 'Connect' icon
	sleep(1000)
EndFunc

;===============================================================================
;Function Name : _EnableOverwriteMode
;Functionality : Enables the Msg Window OVerwrite Mode
;Input 		   :
;Output 	   :
;===============================================================================

Func _EnableOverwriteMode()
	$OvrWriteToolhWd=ControlGetHandle($mWin,"",$OverwriteToolbarctrlID)				; Get handle of the 'Connect/Disconnect' toolbar
	_GUICtrlToolbar_ClickIndex($OvrWriteToolhWd,4)									; Click on 'Connect' icon
	sleep(1000)
EndFunc



;===============================================================================
;Function Name : _SetOneFolderUp
;Functionality : Sets path to parent directory of script dir and creates a folder with name "BUSMASTERTestData_Autoit" to save test files created by autoit scripts.
;Input 		   : -
;Output 	   : Returns the path of the folder
;===============================================================================

Func _SetOneFolderUp()
	$CurrentDirPath = @ScriptDir
	ConsoleWrite($CurrentDirPath&@CRLF)
	; Split $CurrentDirPath value into substrings based on the delimiter '\'
	$arrStrings = StringSplit($CurrentDirPath, "\")

	ConsoleWrite("Size  = "&$arrStrings[0]&@CRLF)
	ConsoleWrite("Value = "&$arrStrings[$arrStrings[0]]&@CRLF)

	$CntRemove = stringlen($arrStrings[$arrStrings[0]]) + 1
	$TrimmedPath = StringTrimRight($CurrentDirPath, $CntRemove)

	ConsoleWrite("Trimmed  = "&$TrimmedPath&@CRLF)
	if fileexists($TrimmedPath & "\" & "BUSMASTERTestData_Autoit")=1 Then
		ConsoleWrite("$isFolderExists=" & fileexists($TrimmedPath & "\" & "BUSMASTERTestData_Autoit") & @CRLF)
		$folderPath=$TrimmedPath & "\" & "BUSMASTERTestData_Autoit"
		return $folderPath
	Else
		DirCreate($TrimmedPath & "\" & "BUSMASTERTestData_Autoit")
		$folderPath=$TrimmedPath & "\" & "BUSMASTERTestData_Autoit"
		return $folderPath
	EndIf
EndFunc

