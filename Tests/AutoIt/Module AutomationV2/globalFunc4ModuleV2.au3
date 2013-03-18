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
	$app = WinExists($WIN_BUSMASTER) 														;Check if the BUSMASTER window exists
	ConsoleWrite("app exists res: " & $app & @CRLF)
	If $app = 0 Then 																		;if not found then run the app
		$rVal = Run("C:\Program Files\BUSMASTER\BUSMASTER.exe")
		ConsoleWrite("App search result: " & $rVal & @CRLF)
		Sleep(2000)
		If WinWaitActive($WIN_BUSMASTER, "", 1) Then
			ControlClick($WIN_BUSMASTER, "OK", "[CLASS:Button; INSTANCE:1]")
			Sleep(2000)
		EndIf
		Return $app
		Return $rVal
	EndIf
	If WinExists($DLG_Hardware) Then 														; if a Hardware Selection dialog appears then click 'OK'
		ControlClick($DLG_Hardware, $BTN_Select, $BTN_Select_Hardware)
		Sleep(1000)
		ControlClick($DLG_Hardware, $BTN_Select, $BTN_Select_Hardware)
		Sleep(1000)
		ControlClick($DLG_Hardware, "",$BTN_OK_Hardware)
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
	$sMenu=WinMenuSelectItem($WIN_BUSMASTER,"",$fileMenu,$newMenu)					; Select File->Configuration->New
	sleep(1000)
	if WinWait($WIN_BUSMASTER,$saveConfigtxt,2) Then								; wait for save configuration dialog
		ControlClick($WIN_BUSMASTER,"",$BTN_Yes_SaveConfig)							; Click on Yes button
	EndIf
	sleep(1000)
	$DirPath = _SetOneFolderUp()													; Set the DirPath to save the dbf file
	if WinExists($WIN_DefaultCfxSave,"") Then
		ControlSend($WIN_DefaultCfxSave,"",$TXT_FilePath_SaveConfigFile,$DirPath&"\defaultFile")
		ControlClick($WIN_DefaultCfxSave,"",$BTN_Save_SaveConfigFile)
	EndIf
	sleep(1000)
	if winexists($WIN_NewCfx) Then
		ControlSend($WIN_NewCfx,"",$TXT_FileName_NewCfx,$DirPath&"\"&$cfxFName)		; Set the filename in 'New configuration filename' dialog
		sleep(500)
		$cConfig=ControlClick($WIN_NewCfx,"",$BTN_SaveInst_NewCfx)					; Click on Save button
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
	$sMenu=WinMenuSelectItem($WIN_BUSMASTER,"",$fileMenu,$loadMenu)					; Select File->Configuration->Load
	sleep(1000)
	if WinWait($WIN_BUSMASTER,$saveConfigtxt,2) Then								; wait for save configuration dialog
		ControlClick($WIN_BUSMASTER,"",$BTN_Yes_SaveConfig)							; Click on Yes button
	EndIf
	sleep(1000)
	$DirPath = _SetOneFolderUp()													; Set the DirPath to save the dbf file
	if winexists($WIN_LoadCfx) Then
		ControlSend($WIN_LoadCfx,"",$TXT_FileName_LoadCfx,$DirPath&"\"&$cfxFName)	; Set the filename in 'Load Configuration Filename...' dialog
		sleep(500)
		$cConfig=ControlClick($WIN_LoadCfx,"",$BTN_LoadInst_LoadCfx)				; Click on Load button
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
	$dbMenu=WinMenuSelectItem($WIN_BUSMASTER,"",$CANMenu,$databaseMenu,$newMenu)
	sleep(1000)
	$DBFolderPath = _SetOneFolderUp()													; Set the DirPath to save the dbf file
	if winexists($WIN_NewDBFile) Then
		ControlSend($WIN_NewDBFile,"",$TXT_FileName_NewDB,$DBFolderPath&"\"&$dbFName)   ; Set Filename
		sleep(1000)
		$cDB=ControlClick($WIN_NewDBFile,"",$BTN_SaveInst_NewDB,"left") 				; Click on Save button
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
	$dbMenu=WinMenuSelectItem($WIN_BUSMASTER,"",$CANMenu,$databaseMenu,$openMenu)
	sleep(1000)
	$DBFolderPath = _SetOneFolderUp()															; Set the DirPath to save the dbf file
	if winexists($WIN_SelCAN_DBFile) Then
		ControlSend($WIN_SelCAN_DBFile,"",$TXT_FileName_OpenDB,$DBFolderPath&"\"&$dbFName)  	; Set Filename
		sleep(1000)
		$cDB=ControlClick($WIN_SelCAN_DBFile,"",$BTN_OpenInst_OpenDB,"left") 					; Click on Save button
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
	$DBTreeVwHWD = ControlGetHandle($WIN_DBEditor_CAN, "", $TVC_DBFile_DBEditor) 	; Fetch tree view control handle
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
	ControlSetText($WIN_Msg_Details, "", $TXT_MsgName_MsgDetails, $msgName) 					; Enter the msg name
	ControlSetText($WIN_Msg_Details, "", $TXT_MsgID_MsgDetails, $msgID) 						; Enter the msg ID
	ControlSetText($WIN_Msg_Details, "", $TXT_MsgLen_MsgDetails, $msgLen) 						; Enter the msg len
	ControlClick($WIN_Msg_Details, "", $BTN_OK_MsgDetails) 										; Click 'OK' button
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
	ControlCommand($WIN_Signal_Details, "", $TXT_SigType_SigDetails, "SelectString", $sigType) 	; Select signal type from the combobox
	sleep(500)
	ControlSetText($WIN_Signal_Details, "", $TXT_SigName_SigDetails, $sigName) 					; Add signal name
	sleep(500)
	ControlSetText($WIN_Signal_Details, "", $TXT_ByteIndex_SigDetails, $sigByteInd) 			; Byte Index
	sleep(500)
	ControlSetText($WIN_Signal_Details, "", $TXT_StartBit_SigDetails, $sigStartBit) 			; Start Bit
	sleep(500)
	ControlSetText($WIN_Signal_Details, "", $TXT_SigLen_SigDetails, $sigLen) 					; enter signal length
	sleep(500)
;~ 	controlfocus($WIN_Signal_Details,"",$sigMinValctrlID)										; Set focus to 'Min Val' edit box
	send("{TAB}")
	send("{TAB}")
	send("{TAB}")
	send("{TAB}")
	sleep(1000)
	ControlClick($WIN_Signal_Details, "", $BTN_OK_SigDetails) 									; Click 'OK' button
EndFunc   ;==>_addSigDetails

;==========================================================================================================
;Function Name 		: _addSigDesc
;Functionality 		: This function adds or updates the Signal description in the 'Enter Value Descriptor and Value' dialog
;Input 		   		: $descName : Description Name,$value : Description Value
;Output		   		: -
;==========================================================================================================

Func _addSigDesc($descName, $value)
	Sleep(500)
	ControlSetText($WIN_Signal_Desc, "", $TXT_SigDescName_SigDesc, $descName) 					; Add signal desc name
	ControlSetText($WIN_Signal_Desc, "", $TXT_SigDescVal_SigDesc, $value) 						; Add signal desc value
	ControlClick($WIN_Signal_Desc, "", $BTN_OK_SigDesc) 										; Click 'OK'
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
	controlclick($DLG_Busmaster,"",$BTN_Yes_CHGSigrange)
EndFunc

;==========================================================================================================
;Function Name 		: _SelectSigDesc
;Functionality 		: This function clicks on the specified index of Signal details list view
;Input 		   		:
;Output		   		:
;==========================================================================================================

Func _SelectSigDesc($index)
	$sigDetlvhwd=controlgethandle($WIN_BUSMASTER,"",$LVC_SigDet_DBeditor)						; Get handle of signal details list view
	_GUICtrlListView_ClickItem($sigDetlvhwd,$index)												; Click on the first item in the Signal details list view
	$sigDesclvhwd=controlgethandle($WIN_BUSMASTER,"",$LVC_SigDesc_SigDesc)						; Get handle of signal details list view
	_GUICtrlListView_ClickItem($sigDesclvhwd,$index)											; Click on the first item in the Signal details list view
	sleep(250)
EndFunc

;==========================================================================================================
;Function Name 		: _saveCloseDB
;Functionality 		: This function saves the Database and Closes it
;Input 		   		:
;Output		   		:
;==========================================================================================================

Func _saveCloseCANDB()
	WinMenuSelectItem($WIN_BUSMASTER,"",$CANMenu,$databaseMenu,$saveMenu)						; Select File->Database->Save
	sleep(2000)
	if winexists($WIN_BUSMASTER,$SaveDBImporttxt) Then
		consolewrite("Dialog Exists"&@CRLF)
		ControlClick($WIN_BUSMASTER,"",$BTN_Yes_BM)
	EndIf
	WinMenuSelectItem($WIN_BUSMASTER,"",$CANMenu,$databaseMenu,$closeMenu)						; Select File->Database->Close
EndFunc

;==========================================================================================================
;Function Name 		: _AssociateDB
;Functionality 		: This function Associates the database
;Input 		   		: Database filename
;Output		   		:
;==========================================================================================================

Func _AssociateCANDB($dbFName)
	WinMenuSelectItem($WIN_BUSMASTER,"",$CANMenu,$databaseMenu,$AssocCANDB)						; Select File->Database->Associate
	sleep(1000)
	$DBFolderPath = _SetOneFolderUp()															; Set the DirPath to save the dbf file
	if winexists($WIN_Associate_CANDB) Then
		ControlSend($WIN_Associate_CANDB,"",$TXT_FileName_AssocDB,$DBFolderPath&"\"&$dbFName)   ; Set Filename
		sleep(1000)
		$cDB=ControlClick($WIN_Associate_CANDB,"",$BTN_Open_AssocDB,"left") 					; Click on open button
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
	WinMenuSelectItem($WIN_BUSMASTER,"",$CANMenu,$databaseMenu,$DissocCANDB)					; Select File->Database->Dissociate
	sleep(1000)
	$DBFolderPath = _SetOneFolderUp()															; Set the DirPath to save the dbf file
	if winexists($WIN_Dissociate_CANDB) Then
		$disDBlbHWD=ControlGetHandle($WIN_Dissociate_CANDB,"",$LSTB_DBFile_Dis_CANDB)			; Fetch the handle of dissociate Db list box
		_GUICtrlListBox_ClickItem($disDBlbHWD,$index)											; Click on the specified index
		controlclick($WIN_Dissociate_CANDB,"",$BTN_Dissoc_Dis_CANDB)							; Click on Dissociate button
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
	$sigDesclvhwd=controlgethandle($WIN_BUSMASTER,"",$LVC_SigDesc_SigDesc)							; Get handle of signal details list view
	for $i=0 to _GUICtrlListView_GetItemCount($sigDesclvhwd)-1
		_GUICtrlListView_ClickItem($sigDesclvhwd,0)													; Click on the first item in the Signal details list view
		sleep(800)
		controlclick($WIN_BUSMASTER,"",$BTN_DelSigDesc_DBEditor)									; Delete Signal Desc
		sleep(1500)
		$DelSignalDescPopUp=0
		if winexists("",$delSigDesctxt) Then
			$DelSignalDescPopUp=1
			controlclick($WIN_BUSMASTER,"",$BTN_Yes_BM)												; Click Yes button
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
	if winexists($WIN_BUSMASTER) Then
		sleep(500)
		WinMenuSelectItem($WIN_BUSMASTER,"",$HardwareMenu,$HWIMenu,$kvaserHWMenu)
		sleep(1000)
		if winexists($DLG_Hardware) Then
			ControlClick($DLG_Hardware, $BTN_Select, $BTN_Select_Hardware)
			Sleep(1000)
			ControlClick($DLG_Hardware, $BTN_Select, $BTN_Select_Hardware)
			Sleep(1000)
			ControlClick($DLG_Hardware, "",$BTN_OK_Hardware)
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
	WinMenuSelectItem($WIN_BUSMASTER,"",$CANMenu,$TxMsgMenu,$configTxmenu)							; Select Configure->Tx
EndFunc


;===============================================================================
;Function Name : _AddMsgBlock
;Functionality : Adds a message block
;Input 		   :
;Output 	   :
;===============================================================================

Func _AddMsgBlock()
	ControlClick($WIN_BUSMASTER,"",$BTN_AddBlock_ConfigTX)											; Click on 'Add' block message
	sleep(500)
EndFunc

;===============================================================================
;Function Name : _AddMsg2TxList
;Functionality : Adds a message to the TX list
;Input 		   :
;Output 	   :
;===============================================================================

Func _AddMsg2TxList()
	ControlClick($WIN_BUSMASTER,"",$BTN_AddMsg_ConfigTX)										; Click on 'Add' block message
	sleep(500)
EndFunc

;===============================================================================
;Function Name : _CloseTxWindow
;Functionality : Closes the TX window
;Input 		   :
;Output 	   :
;===============================================================================

Func _CloseTxWindow()
	ControlClick($WIN_BUSMASTER,"",$BTN_Close_ConfigTX)											; Click on Close button
	sleep(500)
EndFunc


;===============================================================================
;Function Name : _AutoUpdate
;Functionality : Checks Auto Update button if unchecked
;Input 		   :
;Output 	   :
;===============================================================================

Func _EnableAutoUpdate()
	if (ControlCommand($WIN_BUSMASTER,"",$BTN_AutoUpdate_ConfigTX,"IsChecked")=0) Then
		ControlCommand($WIN_BUSMASTER,"",$BTN_AutoUpdate_ConfigTX,"Check")							; Check 'Autoupdate' check box is enabled
	EndIf
EndFunc

;===============================================================================
;Function Name : _AppFilterMenu
;Functionality : Selects App filter from the menu
;Input 		   :
;Output 	   :
;===============================================================================

Func _AppFilterMenu()
	WinMenuSelectItem($WIN_BUSMASTER,"",$CANMenu,$AppFilterMenu)								; Select Configure->App Filters
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
		ControlClick($WIN_Config_filter,"",$BTN_AddFilter_ConfigFilter)							; Click on 'Add' filter button
		sleep(750)
	Else
		ControlClick($WIN_Config_filter,"",$BTN_AddFilter_ConfigFilter)							; Click on 'Add' filter button
		sleep(750)
		$hWnd = ControlGetHandle($WIN_Config_filter,"",$LVC_FilterList_ConfigFilter)
		$FilTypePos=_GUICtrlListView_GetItemPosition($hWnd, 0)									; Get position of Signal details list view control
		ControlClick($WIN_Config_filter,"",1000,"Left",2,$FilTypePos[0]+100,$FilTypePos[1])		; Double click on the 'Filter Type'
		sleep(500)
		send("{DOWN}")
		sleep(500)
		send("{ENTER}")																			; Select 'Pass' filter
	EndIf
	ControlCommand($WIN_Config_filter,"",$COMB_MsgID_ConfigFilter,"SelectString", $msgID)		; Select the MSG from ID combo box
	sleep(200)
	ControlCommand($WIN_Config_filter,"",$COMB_IDType_ConfigFilter,"SelectString", $IDType)		; Select the ID type
	sleep(200)
	ControlCommand($WIN_Config_filter,"",$COMB_MsgType_ConfigFilter,"SelectString", $Frame)		; Select the Frame
	sleep(200)
	ControlCommand($WIN_Config_filter,"",$COMB_Direction_ConfigFilter,"SelectString", $Dir)		; Select the Direction
	sleep(200)
	ControlCommand($WIN_Config_filter,"",$COMB_Channel_ConfigFilter,"SelectString", $Channel)	; Select the Channel
	sleep(200)
	ControlClick($WIN_Config_filter,"",$BTN_Add_ConfigFilter)									; Click on Add button
	sleep(750)
	ControlClick($WIN_Config_filter,"",$BTN_OK_ConfigFilter)									; Click on OK button
	sleep(500)
EndFunc


;===============================================================================
;Function Name : _MsgDisplayMenu
;Functionality : Selects message display from the menu
;Input 		   :
;Output 	   :
;===============================================================================

Func _MsgDisplayMenu()
	WinMenuSelectItem($WIN_BUSMASTER,"",$CANMenu,$CANMsgWinMenu,$configureMenu)									; Select CAN->Msg Window->Configure
EndFunc


;===============================================================================
;Function Name : _AddFiltertoMsgDisp
;Functionality : Adds the filter for message display for CAN
;Input 		   :
;Output 	   :
;===============================================================================

Func _AddFiltertoMsgDisp()
	sleep(100)
	ControlClick($WIN_MsgDisplay,"",$BTN_Configure_MsgDisplay)									; Click Configure button in msg display
	sleep(750)
	if winexists($WIN_FilterSelect) Then
		$fltSelhWd=controlgethandle($WIN_FilterSelect,"",$LSTC_ConfigFilter_FilterSelect)		; Get handle of filter selection list view
		_GUICtrlListView_ClickItem($fltSelhWd,0)												; Click on the first filter
		ControlClick($WIN_FilterSelect,"",$BTN_Add_FilterSelect)								; Click on Add button
		sleep(750)
		ControlClick($WIN_FilterSelect,"",$BTN_OK_FilterSelect)									; Click on OK button
		sleep(750)
	EndIf
	ControlClick($WIN_MsgDisplay,"",$BTN_OK_MsgDisplay)											; Click on OK button
EndFunc

;===============================================================================
;Function Name : _EnableFilterDispMenu
;Functionality : Enable filters for message display from menu
;Input 		   :
;Output 	   :
;===============================================================================

Func _Enable_DisableFilterDispMenu()
	WinMenuSelectItem($WIN_BUSMASTER,"",$CANMenu,$CANMsgWinMenu,$EnableFilterDisp)			; Select CAN->Msg Window->Enable Filter
EndFunc


;===============================================================================
;Function Name : _DisableFilterDispMenu
;Functionality : Disable filters for message display from menu
;Input 		   :
;Output 	   :
;===============================================================================

Func _DisableFilterDispMenu()
	$cntToolhWd=ControlGetHandle($WIN_BUSMASTER,"",$ToolBar_Con_Disconnect)						; Get handle of the 'Disable Msg Disp Filter' toolbar
	_GUICtrlToolbar_ClickIndex($cntToolhWd,9)													; Click on 'Disable Msg Disp Filter' icon
	sleep(750)
EndFunc

;===============================================================================
;Function Name : _Connect()
;Functionality : Clicks on the connect button in the toolbar
;Input 		   :
;Output 	   :
;===============================================================================

Func _ConnectDisconnect()
	$cntToolhWd=ControlGetHandle($WIN_BUSMASTER,"",$ToolBar_Con_Disconnect)						; Get handle of the 'Connect/Disconnect' toolbar
	_GUICtrlToolbar_ClickIndex($cntToolhWd,4)													; Click on 'Connect/Disconnect' icon
	sleep(750)
EndFunc

;===============================================================================
;Function Name : _TransmitMsgsMenu()
;Functionality : Select Transmit messages from the menu
;Input 		   :
;Output 	   :
;===============================================================================

Func _TransmitMsgsMenu()
	WinMenuSelectItem($WIN_BUSMASTER,"",$CANMenu,$transmitMenu,$EnableTransmitMenu)			; Click on 'Transmit normal blocks' menu
EndFunc


;===============================================================================
;Function Name : _CANLogMenu()
;Functionality : Open CAN Log Dialog
;Input 		   :
;Output 	   :
;===============================================================================

Func _CANLogMenu()
	WinMenuSelectItem($WIN_BUSMASTER,"",$CANMenu,$CANLogMenu,$configureMenu)								; Select Configure->Log
	Sleep(1000)
EndFunc


;===============================================================================
;Function Name : _ConfigureCANLog()
;Functionality : Configure Logging for CAN
;Input 		   : Log File Name, Time Mode, Channel #, Numeric Format, File Mode, Start Rec MsgID, Stop Rec MsgID, Filters={True,False}
;Output 	   :
;===============================================================================

Func _ConfigureCANLog($logFileName,$TimeMode,$Channel,$NumFormat,$FileMode,$Start_Rec,$Stop_Rec,$Filters)
	WinMenuSelectItem($WIN_BUSMASTER,"",$CANMenu,$CANLogMenu,$configureMenu)								; Select Configure->Log
	Sleep(1000)
	ControlClick($WIN_CANLog,"",$BTN_Add_ConfigLOG)												; Click on Add button
	sleep(250)
	ControlCommand ($WIN_CANLog,"",$BTN_EnDisable_ConfigLOG,"Check")							; Check Enable/Disable Logging during connect/disconnect
	$LogFilePath=_SetOneFolderUp()
	ControlClick($WIN_CANLog,"",$BTN_logFPath_ConfigLOG)										; Click on Log File Path button
	sleep(1000)
	if winexists($WIN_Select_LogFile) Then
		controlsend($WIN_Select_LogFile,"",$TXT_FileName_SelLogFile,$LogFilePath & "\" & $logFileName)	; Enter the Log file Name
		controlclick($WIN_Select_LogFile,"",$BTN_SaveInst_SelLogFile)									; Click on 'Save' button
	EndIf
	controlcommand($WIN_CANLog,"",$COMB_TimeMode_ConfigLOG,"SelectString", $TimeMode)			; Set the time mode
	controlcommand($WIN_CANLog,"",$COMB_Channel_ConfigLOG,"SelectString", $Channel)				; Set the Channel
	if $NumFormat="Hex" Then
		Controlcommand($WIN_CANLog,"",$RBTN_NumModeHex_ConfigLOG,"Check")						; Set the Numeric mode to Hex
	Else
		Controlcommand($WIN_CANLog,"",$RBTN_NumModeDec_ConfigLOG,"Check")						; Set the Numeric mode to Decimal
	EndIf
	sleep(1000)
	if $FileMode="Overwrite" Then
		Controlcommand($WIN_CANLog,"",$RBTN_FileModeOverWrite_ConfigLOG,"Check")				; Set the File mode
	Else
		Controlcommand($WIN_CANLog,"",$RBTN_FileModeApp_ConfigLOG,"Check")						; Set the File mode
	EndIf
	if $Start_Rec<>"" and $Stop_Rec<>"" Then
		Controlcommand($WIN_CANLog,"",$CHKB_StartTrig_ConfigLOG,"Check")						; enable the start trigger
		ControlSend($WIN_CANLog,"",$TXT_StartMsgID_ConfigLOG,$Start_Rec)						; Set the Start trigger Msg ID
		Controlcommand($WIN_CANLog,"",$CHKB_StopTrig_ConfigLOG,"Check")							; enable the stop trigger
		ControlSend($WIN_CANLog,"",$TXT_StopMsgID_ConfigLOG,$Stop_Rec)							; Set the Stop trigger Msg ID
	EndIf
	if $Filters="True" Then
		controlclick($WIN_CANLog,"",$BTN_Filter_ConfigLOG)										; Click on 'Filter' button
		sleep(1000)
		if winexists($WIN_FilterSelect) Then
			$fltSelhWd=controlgethandle($WIN_FilterSelect,"",$LSTC_ConfigFilter_FilterSelect)	; Get handle of filter selection list view
			_GUICtrlListView_ClickItem($fltSelhWd,0)											; Click on the first filter
			ControlClick($WIN_FilterSelect,"",$BTN_Add_FilterSelect)								; Click on Add button
			sleep(800)
			ControlClick($WIN_FilterSelect,"",$BTN_OK_FilterSelect)								; Click on OK button
			sleep(800)
		EndIf
	EndIf
	controlclick($WIN_CANLog,"",$BTN_OK_ConfigLOG)												; Click on 'OK' button
	sleep(1000)
EndFunc


;===============================================================================
;Function Name : _EnableFilterLog
;Functionality : Enable filters for Logging
;Input 		   :
;Output 	   :
;===============================================================================

Func _EnableFilterLog()
	WinMenuSelectItem($WIN_BUSMASTER,"",$CANMenu,$CANLogMenu,$EnableFilterLogMenu)			; Select CAN->Logging->Enable Filter log
EndFunc


;===============================================================================
;Function Name : _DisableJ1939Win
;Functionality : Checks whether J1939 message window is active. If J1939 message window is active then disable it.
;Input 		   :
;Output 	   :
;===============================================================================

Func _DisableJ1939Win()
	send("!JMA")																					; Select J1939->View from the menu
	sleep(1000)
	send("{ESC}")
	sleep(500)
	send("{ESC}")
	sleep(500)
	if winexists($WIN_J1939MsgWind) Then
		$winhWnd = WinGetHandle($WIN_BUSMASTER)												; Fetch the window handle
		$hMain = _GUICtrlMenu_GetMenu($winhWnd)												; Fetch the handle of the menu
		ConsoleWrite("menu handle : "&$hMain& @CRLF)
		$hFile = _GUICtrlMenu_GetItemSubMenu($hMain, 4)										; Fetch the handle of J1939 menu
		ConsoleWrite("Submenu handle : "&$hFile& @CRLF)
		$hSubmenu=_GUICtrlMenu_GetItemSubMenu($hFile, 4)									; Fetch the handle of J1939->Msg Window menu
		ConsoleWrite("$hSubmenu handle : "&$hSubmenu& @CRLF)
		$res=_GUICtrlMenu_GetItemChecked($hSubmenu,0,True)									; Check whether J1939->Message Window->Activate is checked or not
		$val=_GUICtrlMenu_GetItemText($hSubmenu,0,True)										; Fetch the text of first item in J1939->Message Window
		ConsoleWrite("$res : "&$res& @CRLF)
		ConsoleWrite("$val : "&$val& @CRLF)
		if $res="True" then																	; If J1939->View->Message Window is enabled then uncheck it
			WinMenuSelectItem($WIN_BUSMASTER,"",$J1939Menu,$J1939MsgWinMenu,$J1939MsgWinAct)
		EndIf
	Else
		$winhWnd = WinGetHandle($WIN_BUSMASTER)												; Fetch the window handle
		$hMain = _GUICtrlMenu_GetMenu($winhWnd)
		ConsoleWrite("menu handle : "&$hMain& @CRLF)
		$hFile = _GUICtrlMenu_GetItemSubMenu($hMain, 3)
		ConsoleWrite("Submenu handle : "&$hFile& @CRLF)
		$hSubmenu=_GUICtrlMenu_GetItemSubMenu($hFile, 4)
		ConsoleWrite("$hSubmenu handle : "&$hSubmenu& @CRLF)
		$res=_GUICtrlMenu_GetItemChecked($hSubmenu,0,True)
		$val=_GUICtrlMenu_GetItemText($hSubmenu,0,True)
		ConsoleWrite("$res : "&$res& @CRLF)
		ConsoleWrite("$val : "&$val& @CRLF)
		if $res="True" then																	; If J1939->View->Message Window is enabled then uncheck it
			WinMenuSelectItem($WIN_BUSMASTER,"",$J1939Menu,$J1939MsgWinMenu,$J1939MsgWinAct)
		EndIf
	EndIf
	send("!W1")																				; Select Window->1 Message Window CAN
	sleep(750)
EndFunc


;===============================================================================
;Function Name : _DisableOverwriteMode
;Functionality : Diables the Msg Window OVerwrite Mode
;Input 		   :
;Output 	   :
;===============================================================================

Func _DisableOverwriteMode()
	$OvrWriteToolhWd=ControlGetHandle($WIN_BUSMASTER,"",$ToolBar_OverWrite)					; Get handle of the 'Connect/Disconnect' toolbar
	_GUICtrlToolbar_ClickIndex($OvrWriteToolhWd,4)											; Click on 'Connect' icon
	sleep(1000)
EndFunc

;===============================================================================
;Function Name : _EnableOverwriteMode
;Functionality : Enables the Msg Window OVerwrite Mode
;Input 		   :
;Output 	   :
;===============================================================================

Func _EnableOverwriteMode()
	$OvrWriteToolhWd=ControlGetHandle($WIN_BUSMASTER,"",$ToolBar_OverWrite)					; Get handle of the 'Connect/Disconnect' toolbar
	_GUICtrlToolbar_ClickIndex($OvrWriteToolhWd,4)											; Click on 'Connect' icon
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

