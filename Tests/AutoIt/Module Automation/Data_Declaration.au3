Global $mWin, $hWare, $selectBTN, $okBTN, $ldCfx, $loadBTN, $nwCfx,$oDbf, $saveAs, $dbEditCAN, $msgDtails, $sigDetails, $ok1BTN, $nSigBTN
Global $crashRes, $lConfig, $cConfig,$channelCount,$time,$oExcel

;==============================================
;Data declaration for Window/Dialog Title
;==============================================

$mWin="BUSMASTER"
$dlghWare="Hardware Selection"
$dlgldCfx="Load Configuration Filename..."
$dlgnwCfx="New Configuration Filename..."
$dlgoDbf="Select BUSMASTER Database Filename..."
$dlgsaveAs="Save As"
$windbEditCAN="BUSMASTER - [DatabaseEditor - CAN]"
$windbEditJ1939="BUSMASTER - [DatabaseEditor - J1939]"
$dlgmsgDtails="Message Details"
$dlgsigDetails="Signal Details"
$dlgasoDB="Select Active Database Filename..."
$dlgcfgLog="Configure Logging for CAN"
$dlgfConvert="BUSMASTER Format Conversions"
$dlgsigDesc="Enter Value Descriptor and Value"
$dlgselJ1939Fl="Select J1939 Database Filename..."
$disDbDialog="Dissociate Database"
$dlgfrmtConvWin="BUSMASTER Format Conversions"
$dlgfCOpenWin="Open"
$dlgkvaserCANWin="CAN_Kvaser_CAN"
$dlgBusmaster="BUSMASTER"
$dlgNewDBFile="New Database file name..."
$dlgConfigfilter="Configure Filter List"
$dlgMsgDisp="Configure Message Display- CAN"
$dlgFilSel="Filter Selection Dialog"
$J1939MsgWind="BUSMASTER - [Message Window - J1939]"
$dlgSelLogFile="Select a Log file"
$DlgDefaultConfigSave="Save Configuration File"
$DlgAboutBM="About"
;==============================================
;Data declaration for Menu Text
;==============================================

$fileMenu="&File"
$configMenu="Confi&guration"
$loadMenu="&Load"
$newMenu="&New"
$databaseMenu="&Database"
$AssocCANDB="&Associate"
$DissocCANDB="&Disassociate"
$openMenu="&Open"
$saveMenu="&Save"
$closeMenu="&Close"
$saveImportMenu="Save and Im&port"
$configureMenu="&Configure"
$configHWMenu="&Configure"
$HWIMenu="&Hardware Interface"
$kvaserHWMenu="&Kvaser CAN"
$configTxmenu="&Configure"
$TxMsgMenu="&Tx Messages"
$AppFilterMenu="&App Filters"
$MsgDisplay="&Message Display"
$FunctionsMenu="F&unctions"
$enableFilterMenu="Enable &Filter"
$displayMenu="&Display"
$transmitMenu="&Transmit"
$normalBlckMenu="&Normal Blocks"
$J1939Menu="&J1939"
$J1939ViewMenu="&View"
$J1939MsgWinMenu="&Message Window"
$LogMenu="L&og"
$LogFilterMenu="&Log"
$HelpMenu="&Help"
$AboutBusmaster="&About BUSMASTER"
;==============================================
;Data declaration for Button Text
;==============================================

$selectBTN="&Select"
$okBTN="&OK"
$loadBTN="Load"
$ok1BTN="OK"
$newSigBTN="New Signal..."
$cancelBTN="Cancel"
$closeBTN="&Close"

;===============================================
;Data declaration for Control ID's
;===============================================

$hwselBTNctrlID=5													; controlID for 'Select' button in hardware Selection dialog
$hwselOKBTNctrlID=1													; controlID for 'OK' button in hardware Selection dialog
$yesBTNctrlID=6														; controlID for 'Yes' button in a warning pop up message
$saveCfxyesBTNctrlID=6												; controlID for 'Yes' button in save config warning pop up message
$loadCfxfNamectrlID=1152											; controlID for the 'File Name' Edit control in 'Load Configuration filename' dialog
$loadCfxloadctrlID=1												; controlID for the 'Load' button in 'Load Configuration filename' dialog
$loadCfxcancelctrlID=2												; controlID for the 'Cancel' button in 'Load Configuration filename' dialog
$newCfxfNamectrlID=1152											    ; controlID for the 'File Name' Edit control in 'New Configuration filename' dialog
$newCfxSavectrlID=1													; controlID for the 'Save' button in 'New Configuration filename' dialog
$selDbffNameCtrlID=1152												; controlID for the 'File Name' Edit control in 'Select BUSMASTER Database Filename' dialog
$selDbfOpenctrlID=1													; controlID for the 'Open' button in 'Select BUSMASTER Database Filename' dialog
$DbfNotFoundOKctrlID=2												; controlID for the 'OK' button in 'DBF file not found' dialog
$saveAsfName=1152													; controlID for the 'File Name' Edit control in 'Save As' dialog for new database file
$saveAsSave=1														; controlID for 'Save As' button in Save As window
$msgNamectrlID=1019													; controlID for the 'message name' Edit control in 'Message Details' dialog
$msgIDctrlID=1035													; controlID for the 'message ID' Edit control in 'Message Details' dialog
$msglenctrlID=1013													; controlID for the 'message Length' Edit control in 'Message Details' dialog
$msgdetOKctrlID=1													; controlID for the 'OK' button in 'Message Details' dialog
$newSigctrlID=1075													; controlID for the 'New Signal' button in 'Signal Details' dialog
$sigNamectrlID=1000													; controlID for the 'Signal Name' edit box in 'Signal Details' dialog
$sigTypectrlID=1008													; controlID for the 'Signal Type' combo box in 'Signal Details' dialog
$sigLenctrlID=1024													; controlID for the 'Signal Length' edit box in 'Signal Details' dialog
$sigByteIndctrlID=1023												; controlID for the 'Signal Byte Index' edit box in 'Signal Details' dialog
$sigStartBitctrlID=1025												; controlID for the 'Signal Start Bit' edit box in 'Signal Details' dialog
$sigDetOKctrlID=1													; controlID for 'OK' button in the 'Signal Details' dialog in 'Database Editor' window
$sigDesctrlID=1089													; controlID for 'New Desc...' button in the'Database Editor' window
$sigDescNamectrlID=1010												; controlID for 'Desc Name' Edit box in the 'Enter Value Descriptor and Value' dialog
$sigDescValctrlID=1011												; controlID for 'Desc Value' Edit box in the 'Enter Value Descriptor and Value' dialog
$sigDescOKctrlID=1042												; controlID for 'OK' button in the 'Enter Value Descriptor and Value' dialog
$sigEditctrlID=1076													; controlID for 'Edit Signal' button in the Database Editor' window
$editSigDescctrlID=1090												; controlID for'Edit Desc' button in the Database Editor' window
$delSigDescctrlID=1091												; controlID for'Delete Desc' button in the Database Editor' window
$sigDelctrlID=1077													; controlID for'Delete Signal' button in the Database Editor' window
$sigMinValctrlID=1006												; controlID for'Min Val' Edit box in the Signal details window
$autoUpdatectrlID=17000												; controlID for 'Auto Update' Checkbox in TX window
$addMsgBlockctrlID=1226												; controlID for 'Add' msg block in TX window
$timedelayMsgctrlID=1134											; controlID for 'Time delay btw msgs' edit box in TX window
$msgIDNameComboctrlID=1221											; controlID for name/ID combo box in TX window
$exdMsgTypectrlID=1213												; controlID for 'Exd' msg type radio button in TX window
$stdMsgTypectrlID=1215												; controlID for 'Std' msg type radio button in TX window
$addTxMsgBTNctrlID=1122												; controlID for 'Add Message' button in Tx window
$msgIDNameEditctrlID=1001											; controlID for name/ID edit box in TX window
$sigRangeYesctrlID=6												; controlID for 'Yes' button in "changing signal range" warning pop up
$AssocfNamectrlID=1152											    ; controlID for the 'File Name' Edit control in 'Select Active Database Filename...' dialog
$dissocBTNctlID=1285												; controlID for the dissociate button in Dissociate database window
$cancelDisBTNctrlID=2												; controlID for 'Cancel' button in Dissociate DB window
$addFilterBTNctrlID=1228											; controlID for 'Add' button in Configure Filter window
$msgIDFilterctrlID=1004												; controlID for 'ID' combo box in Configure Filter window
$msgIDtypectrlID=1009												; controlID for 'Id Type' combo box in Configure Filter window
$msgtypectrlID=1010													; controlID for 'Frame' combo box in Configure Filter window
$msgDirectionctrlID=1011											; controlID for 'Direction' combo box in Configure Filter window
$msgChannelctrlID=1012												; controlID for 'Channel' combo box in Configure Filter window
$addFiltertoListctrlID=3											; controlID for 'Add' to filter list button  in Configure Filter window
$OKFilterBTNctrlID=1256												; controlID for 'OK' button  in Configure Filter window
$confMsgDispBTNctrlID=1031											; controlID for 'Configure' button  in Configure msg display window
$AddFilMsgDispBTNctrlID=32950										; controlID for 'Add' button  in filter Selection dialog
$okFilSelDlgBTNctrlID=1												; controlID for 'OK' button  in filter Selection dialog
$okConMsgDispBTNctrlID=1											; controlID for 'OK' button  in Configure msg display window
$closeTxWinBTNctrlID=1015											; controlID for 'Close' button  in Configure Tx window
$connectToolbarctrlID=128											; controlID for Connect toolbar
$addLogBTNctrlID=1303												; controlID for Add button in Configure Logging Dialog
$enableDisLogctrlID=1092											; controlID for ""&Enable/Disable Logging during tool Connect/Disconnect" in Configure Logging Dialog
$logFilePathctrlID=1122												; controlID for 'LOGFILEPATH" button in Configure Logging Dialog
$SelLogFnamectrlID=1152												; controlID for File name edit control in Select a Log File dialog
$LogTimeModectrlID=1101												; controlID for Time Mode combobox in Configure Logging Dialog
$LogChannelctrlID=1102												; controlID for channel combobox in Configure Logging Dialog
$LogNumModeDec=1081													; controlID for Decimal numeric mode in Configure Logging Dialog
$LogNumModeHex=1082													; controlID for Hex numeric mode in Configure Logging Dialog
$LogfileModeApp=1350												; controlID for Append File mode in Configure Logging Dialog
$LogfileModeOverWrite=1351											; controlID for Overwrite File mode in Configure Logging Dialog
$LogStartTrigger=1116												; controlID for start trigger in Configure Logging Dialog
$LogStopTrigger=1117												; controlID for stop trigger in Configure Logging Dialog
$LogStartMsgID=1095													; controlID for start trigger MSGID Edit box in Configure Logging Dialog
$LogStopMsgID=1096													; controlID for stop trigger MSGID Edit box in Configure Logging Dialog
$LogFilterBTNctrlID=1121											; controlID for filter button in Configure Logging Dialog
$LogOKBTNctrlID=1													; controlID for OK button in Configure Logging Dialog
$OverwriteToolbarctrlID=314											; controlID for Overwrite toolbar
$BMVersionStaticctrlID=1153											; controlID for Busmaster version in About dialog


;===============================================
;Data Declaration for Instance's
;===============================================
$loadCfxloadInst2="[CLASS:Button; INSTANCE:2]"						; Load button instance in Load Configuration window
$tvInst1="[CLASS:SysTreeView32; INSTANCE:1]"						; tree view instance in database editor window
$savebtncfxInst2="[CLASS:Button; INSTANCE:2]"						; Save button instance in file->configuration->new window
$savebtnDBInst2="[CLASS:Button; INSTANCE:2]"						; Save button instance in file->database->new window
$sigDetlvInst1="[CLASS:SysListView32; INSTANCE:1]"					; List View instance for Signal Details in database editor window
$sigDesclvInst1="[CLASS:SysListView32; INSTANCE:2]"					; List View instance for Signal Description in database editor window
$opendbfBTNInst2="[CLASS:Button; INSTANCE:2]"						; Open button instance in file->database->open window
$assocdbfBTNinst2="[CLASS:Button; INSTANCE:2]"						; Open button instance in file->database->Associate window
$dissocdbflbinst1="[CLASS:ListBox; INSTANCE:1]"						; List Box instance in File->database->dissociate window
$filterList="[CLASS:SysListView32; INSTANCE:1]"						; List View instance in Configure Filter List window
$configFilMsgDisp="[CLASS:SysListView32; INSTANCE:1]"				; List View instance in Filter List Message display dialog window
$msgWinlv="[CLASS:SysListView32; INSTANCE:1]"						; List View instance in Message window
$savebtnlogInst2="[CLASS:Button; INSTANCE:2]"						; Save button instance in file->configuration->Log->Select a Log file
$logFilelv="[CLASS:SysListView32; INSTANCE:1]"						; List View instance in Logging dialog for Log file
$DefConfigFilePath="[CLASS:Edit; INSTANCE:1]"						; Edit control instance of File path in "Save Configuration File" for default config
$DefConfigFileSaveBTNinst2="[CLASS:Button; INSTANCE:2]"				; Save button instance in "Save Configuration File" for default config


;==================================================
;Data Declaration for warning pop up messsage text
;==================================================

$sigDescDelmsgtxt="Are you sure you want to delete?"								; delete Signal description pop up message text
$sigDelmsgtxt="You are about to delete a signal."									; delete Message Signal pop up message text
$msgDeltxt="You are about to Delete a Message."										; delete message pop up dialog text
$saveConfigtxt="Configuration File is modified"										; Save configuration dialog text
$dbfnotfound="Specified database file is not found."								; DBF file not found dialog
$appCrashtxt="Unhandled exception in BUSMASTER. Would you like to save a dump file?"; Crash dialog text
$appCrashtitle="BUSMASTER.exe"														; Crash dialog title
$disDBtxt="Dissociation will clear Signal Watch List."								; Dissociate DB pop up message text
$delSigDesctxt="You are about to delete a signal description"						; Delete signal description message text
$SaveDBImporttxt="File"																;