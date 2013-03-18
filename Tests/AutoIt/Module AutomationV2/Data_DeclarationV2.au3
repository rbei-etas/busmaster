Global $mWin, $hWare, $selectBTN, $okBTN, $ldCfx, $loadBTN, $nwCfx,$oDbf, $saveAs, $dbEditCAN, $msgDtails, $sigDetails, $ok1BTN, $nSigBTN
Global $crashRes, $lConfig, $cConfig,$channelCount,$time,$oExcel

;==============================================
; Save Configuration File
;==============================================

$TXT_FilePath_SaveConfigFile="[CLASS:Edit; INSTANCE:1]"						; Edit control instance of File path in "Save Configuration File" for default config
$DefConfigFilePath="[CLASS:Edit; INSTANCE:1]"						; Edit control instance of File path in "Save Configuration File" for default config
$DefConfigFileSaveBTNinst2="[CLASS:Button; INSTANCE:2]"				; Save button instance in "Save Configuration File" for default config
$BTN_Save_SaveConfigFile="[CLASS:Button; INSTANCE:2]"				; Save button instance in "Save Configuration File" for default config
$BTN_Yes_SaveConfig=6												; controlID for 'Yes' button in save config warning pop up message
$saveCfxyesBTNctrlID=6
$BTN_Yes_Busmaster=6											; controlID for 'Yes' button in save config warning pop up message
$saveConfigtxt="Configuration File is modified"										; Save configuration dialog text
;==============================================
; New Configuration Filename
;==============================================
$MENU_Configuration="Confi&guration"
$WIN_NewCfx="New Configuration Filename..."
$dlgnwCfx="New Configuration Filename..."
$TXT_FileName_NewCfx=1152											    ; controlID for the 'File Name' Edit control
$newCfxfNamectrlID=1152											        ; controlID for the 'File Name' Edit control
$BTN_Save_NewCfx=1														; controlID for the 'Save' button
$newCfxSavectrlID=1														; controlID for the 'Save' button
$BTN_SaveInst_NewCfx="[CLASS:Button; INSTANCE:2]"						; Save button instance in file->configuration->new window
$savebtncfxInst2="[CLASS:Button; INSTANCE:2]"						; Save button instance in file->configuration->new window
;==============================================
; Load Configuration Filename
;==============================================

$WIN_LoadCfx="Load Configuration Filename..."
$dlgldCfx="Load Configuration Filename..."
$TXT_FileName_LoadCfx=1152											    ; controlID for the 'File Name' Edit control
$loadCfxfNamectrlID=1152												; controlID for the 'File Name' Edit control
$BTN_Load_LoadCfx=1														; controlID for the 'Load' button
$loadCfxloadctrlID=1													; controlID for the 'Load' button
$BTN_Cancel_LoadCfx=2													; controlID for the 'Cancel' button
$loadCfxcancelctrlID=2													; controlID for the 'Cancel' button
$BTN_LoadInst_LoadCfx="[CLASS:Button; INSTANCE:2]"						; Load button instance in Load Configuration window
$loadCfxloadInst2="[CLASS:Button; INSTANCE:2]"						; Load button instance in Load Configuration window
;==============================================
; New Database Filename
;==============================================
$WIN_NewDBFile="New Database file name..."
$dlgNewDBFile="New Database file name..."
$TXT_FileName_NewDB=1152												; controlID for the 'File Name' Edit control
$saveAsfName=1152													; controlID for the 'File Name' Edit control in 'Save As' dialog for new database file
$BTN_Save_NewDB=1														; controlID for 'Save As' button in Save As window
$saveAsSave=1														; controlID for 'Save As' button in Save As window
$BTN_SaveInst_NewDB="[CLASS:Button; INSTANCE:2]"						; Save button instance in file->database->new window
$savebtnDBInst2="[CLASS:Button; INSTANCE:2]"						; Save button instance in file->database->new window

;==============================================
; Select BUSMASTER Database Filename
;==============================================
$WIN_SelCAN_DBFile="Select BUSMASTER Database Filename..."
$dlgoDbf="Select BUSMASTER Database Filename..."
$TXT_FileName_OpenDB=1152												; controlID for the 'File Name' Edit control in 'Select BUSMASTER Database Filename' dialog
$selDbffNameCtrlID=1152												; controlID for the 'File Name' Edit control in 'Select BUSMASTER Database Filename' dialog
$BTN_Open_OpenDB=1													; controlID for the 'Open' button in 'Select BUSMASTER Database Filename' dialog
$selDbfOpenctrlID=1													; controlID for the 'Open' button in 'Select BUSMASTER Database Filename' dialog
$BTN_OpenInst_OpenDB="[CLASS:Button; INSTANCE:2]"						; Open button instance in file->database->open window
$opendbfBTNInst2="[CLASS:Button; INSTANCE:2]"						; Open button instance in file->database->open window
;==============================================
; Select Active Database Filename
;==============================================
$WIN_Associate_CANDB="Select Active Database Filename..."
$dlgasoDB="Select Active Database Filename..."
$TXT_FileName_AssocDB=1152											; controlID for the 'File Name' Edit control in 'Select Active Database Filename...' dialog
$AssocfNamectrlID=1152											    ; controlID for the 'File Name' Edit control in 'Select Active Database Filename...' dialog
$BTN_Open_AssocDB="[CLASS:Button; INSTANCE:2]"						; Open button instance in file->database->Associate window
$assocdbfBTNinst2="[CLASS:Button; INSTANCE:2]"						; Open button instance in file->database->Associate window

;==============================================
; Dissociate Database
;==============================================

$WIN_Dissociate_CANDB="Dissociate Database"
$BTN_Dissoc_Dis_CANDB=1285												; controlID for the dissociate button in Dissociate database window
$dissocBTNctlID=1285												; controlID for the dissociate button in Dissociate database window
$BTN_Cancel_Dis_CANDB=2												; controlID for 'Cancel' button in Dissociate DB window
$cancelDisBTNctrlID=2												; controlID for 'Cancel' button in Dissociate DB window
$LSTB_DBFile_Dis_CANDB="[CLASS:ListBox; INSTANCE:1]"						; List Box instance in File->database->dissociate window
$dissocdbflbinst1="[CLASS:ListBox; INSTANCE:1]"						; List Box instance in File->database->dissociate window
$disDBtxt="Dissociation will clear Signal Watch List."								; Dissociate DB pop up message text

;==============================================
; Database Editor - CAN
;==============================================
$WIN_DBEditor_CAN="BUSMASTER - [DatabaseEditor - CAN]"
$windbEditCAN="BUSMASTER - [DatabaseEditor - CAN]"
$WIN_Msg_Details="Message Details"
$dlgmsgDtails="Message Details"
$WIN_Signal_Details="Signal Details"
$dlgsigDetails="Signal Details"
$WIN_Signal_Desc="Enter Value Descriptor and Value"
$dlgsigDesc="Enter Value Descriptor and Value"
$TVC_DBFile_DBEditor="[CLASS:SysTreeView32; INSTANCE:1]"						; tree view instance in database editor window
$tvInst1="[CLASS:SysTreeView32; INSTANCE:1]"						; tree view instance in database editor window
$TXT_MsgName_MsgDetails=1019													; controlID for the 'message name' Edit control in 'Message Details' dialog
$msgNamectrlID=1019													; controlID for the 'message name' Edit control in 'Message Details' dialog
$TXT_MsgID_MsgDetails=1035													; controlID for the 'message ID' Edit control in 'Message Details' dialog
$msgIDctrlID=1035													; controlID for the 'message ID' Edit control in 'Message Details' dialog
$TXT_MsgLen_MsgDetails=1013													; controlID for the 'message Length' Edit control in 'Message Details' dialog
$msglenctrlID=1013													; controlID for the 'message Length' Edit control in 'Message Details' dialog
$BTN_OK_MsgDetails=1													; controlID for the 'OK' button in 'Message Details' dialog
$msgdetOKctrlID=1													; controlID for the 'OK' button in 'Message Details' dialog
$BTN_NewSignal_DBEditor=1075													; controlID for the 'New Signal' button in 'Signal Details' dialog
$newSigctrlID=1075													; controlID for the 'New Signal' button in 'Signal Details' dialog
$TXT_SigName_SigDetails=1000													; controlID for the 'Signal Name' edit box in 'Signal Details' dialog
$sigNamectrlID=1000													; controlID for the 'Signal Name' edit box in 'Signal Details' dialog
$TXT_SigType_SigDetails=1008													; controlID for the 'Signal Type' combo box in 'Signal Details' dialog
$sigTypectrlID=1008													; controlID for the 'Signal Type' combo box in 'Signal Details' dialog
$TXT_SigLen_SigDetails=1024													; controlID for the 'Signal Length' edit box in 'Signal Details' dialog
$sigLenctrlID=1024													; controlID for the 'Signal Length' edit box in 'Signal Details' dialog
$TXT_ByteIndex_SigDetails=1023												; controlID for the 'Signal Byte Index' edit box in 'Signal Details' dialog
$sigByteIndctrlID=1023												; controlID for the 'Signal Byte Index' edit box in 'Signal Details' dialog
$TXT_StartBit_SigDetails=1025												; controlID for the 'Signal Start Bit' edit box in 'Signal Details' dialog
$sigStartBitctrlID=1025												; controlID for the 'Signal Start Bit' edit box in 'Signal Details' dialog
$BTN_OK_SigDetails=1													; controlID for 'OK' button in the 'Signal Details' dialog in 'Database Editor' window
$sigDetOKctrlID=1													; controlID for 'OK' button in the 'Signal Details' dialog in 'Database Editor' window
$TXT_MinVal_SigDetails=1006												; controlID for'Min Val' Edit box in the Signal details window
$sigMinValctrlID=1006												; controlID for'Min Val' Edit box in the Signal details window
$BTN_SigDes_DBEditor=1089													; controlID for 'New Desc...' button in the'Database Editor' window
$sigDesctrlID=1089													; controlID for 'New Desc...' button in the'Database Editor' window
$TXT_SigDescName_SigDesc=1010												; controlID for 'Desc Name' Edit box in the 'Enter Value Descriptor and Value' dialog
$sigDescNamectrlID=1010												; controlID for 'Desc Name' Edit box in the 'Enter Value Descriptor and Value' dialog
$TXT_SigDescVal_SigDesc=1011												; controlID for 'Desc Value' Edit box in the 'Enter Value Descriptor and Value' dialog
$sigDescValctrlID=1011												; controlID for 'Desc Value' Edit box in the 'Enter Value Descriptor and Value' dialog
$BTN_OK_SigDesc=1042												; controlID for 'OK' button in the 'Enter Value Descriptor and Value' dialog
$sigDescOKctrlID=1042												; controlID for 'OK' button in the 'Enter Value Descriptor and Value' dialog
$BTN_EditSig_DBEditor=1076													; controlID for 'Edit Signal' button in the Database Editor' window
$sigEditctrlID=1076													; controlID for 'Edit Signal' button in the Database Editor' window
$BTN_EditSigDesc_DBEditor=1090												; controlID for'Edit Desc' button in the Database Editor' window
$editSigDescctrlID=1090												; controlID for'Edit Desc' button in the Database Editor' window
$BTN_DelSigDesc_DBEditor=1091												; controlID for'Delete Desc' button in the Database Editor' window
$delSigDescctrlID=1091												; controlID for'Delete Desc' button in the Database Editor' window
$BTN_DeleteSig_DBEditor=1077													; controlID for'Delete Signal' button in the Database Editor' window
$sigDelctrlID=1077													; controlID for'Delete Signal' button in the Database Editor' window
$BTN_Yes_CHGSigrange=6												; controlID for 'Yes' button in "changing signal range" warning pop up
$sigRangeYesctrlID=6												; controlID for 'Yes' button in "changing signal range" warning pop up
$LVC_SigDet_DBeditor="[CLASS:SysListView32; INSTANCE:1]"					; List View instance for Signal Details in database editor window
$sigDetlvInst1="[CLASS:SysListView32; INSTANCE:1]"					; List View instance for Signal Details in database editor window
$LVC_SigDesc_SigDesc="[CLASS:SysListView32; INSTANCE:2]"					; List View instance for Signal Description in database editor window
$sigDesclvInst1="[CLASS:SysListView32; INSTANCE:2]"					; List View instance for Signal Description in database editor window


$sigDescDelmsgtxt="Are you sure you want to delete?"								; delete Signal description pop up message text
$sigDelmsgtxt="You are about to delete a signal."									; delete Message Signal pop up message text
$msgDeltxt="You are about to Delete a Message."										; delete message pop up dialog text
;==============================================
; Hardware Selection
;==============================================
$DLG_Hardware_Select="Hardware Selection"
$BTN_Select_Hardware=5												; controlID for 'Select' button in hardware Selection dialog
$hwselBTNctrlID=5													; controlID for 'Select' button in hardware Selection dialog
$BTN_OK_Hardware=1													; controlID for 'OK' button in hardware Selection dialog
$hwselOKBTNctrlID=1													; controlID for 'OK' button in hardware Selection dialog

;==============================================
; Configure Message Display - CAN
;==============================================
$WIN_MsgDisplay="Configure Message Display- CAN"
$dlgMsgDisp="Configure Message Display- CAN"
$BTN_Configure_MsgDisplay=1031										; controlID for 'Configure' button  in Configure msg display window
$confMsgDispBTNctrlID=1031											; controlID for 'Configure' button  in Configure msg display window
$BTN_OK_MsgDisplay=1												; controlID for 'OK' button  in Configure msg display window
$okConMsgDispBTNctrlID=1											; controlID for 'OK' button  in Configure msg display window

;==============================================
; Filter Selection Dialog
;==============================================
$WIN_FilterSelect="Filter Selection Dialog"
$dlgFilSel="Filter Selection Dialog"
$BTN_Add_FilterSelect=32950											; controlID for 'Add' button  in filter Selection dialog
$AddFilMsgDispBTNctrlID=32950										; controlID for 'Add' button  in filter Selection dialog
$BTN_OK_FilterSelect=1												; controlID for 'OK' button  in filter Selection dialog
$okFilSelDlgBTNctrlID=1												; controlID for 'OK' button  in filter Selection dialog
$LSTC_ConfigFilter_FilterSelect="[CLASS:SysListView32; INSTANCE:1]"				; List View instance in Filter List Message display dialog window
$configFilMsgDisp="[CLASS:SysListView32; INSTANCE:1]"				; List View instance in Filter List Message display dialog window

;==============================================
; Configure Logging for CAN
;==============================================
$WIN_CANLog="Configure Logging for CAN"
$dlgcfgLog="Configure Logging for CAN"
$WIN_Select_LogFile="Select a Log file"
$dlgSelLogFile="Select a Log file"
$BTN_Add_ConfigLOG=1303												; controlID for Add button in Configure Logging Dialog
$addLogBTNctrlID=1303												; controlID for Add button in Configure Logging Dialog
$BTN_EnDisable_ConfigLOG=1092											; controlID for ""&Enable/Disable Logging during tool Connect/Disconnect" in Configure Logging Dialog
$enableDisLogctrlID=1092											; controlID for ""&Enable/Disable Logging during tool Connect/Disconnect" in Configure Logging Dialog
$BTN_logFPath_ConfigLOG=1122												; controlID for 'LOGFILEPATH" button in Configure Logging Dialog
$logFilePathctrlID=1122												; controlID for 'LOGFILEPATH" button in Configure Logging Dialog
$TXT_FileName_SelLogFile=1152												; controlID for File name edit control in Select a Log File dialog
$SelLogFnamectrlID=1152												; controlID for File name edit control in Select a Log File dialog
$COMB_TimeMode_ConfigLOG=1101												; controlID for Time Mode combobox in Configure Logging Dialog
$LogTimeModectrlID=1101												; controlID for Time Mode combobox in Configure Logging Dialog
$COMB_Channel_ConfigLOG=1102												; controlID for channel combobox in Configure Logging Dialog
$LogChannelctrlID=1102												; controlID for channel combobox in Configure Logging Dialog
$RBTN_NumModeDec_ConfigLOG=1081													; controlID for Decimal numeric mode in Configure Logging Dialog
$LogNumModeDec=1081													; controlID for Decimal numeric mode in Configure Logging Dialog
$RBTN_NumModeHex_ConfigLOG=1082													; controlID for Hex numeric mode in Configure Logging Dialog
$LogNumModeHex=1082													; controlID for Hex numeric mode in Configure Logging Dialog
$RBTN_FileModeApp_ConfigLOG=1350												; controlID for Append File mode in Configure Logging Dialog
$LogfileModeApp=1350												; controlID for Append File mode in Configure Logging Dialog
$RBTN_FileModeOverWrite_ConfigLOG=1351											; controlID for Overwrite File mode in Configure Logging Dialog
$LogfileModeOverWrite=1351											; controlID for Overwrite File mode in Configure Logging Dialog
$CHKB_StartTrig_ConfigLOG=1116												; controlID for start trigger in Configure Logging Dialog
$LogStartTrigger=1116												; controlID for start trigger in Configure Logging Dialog
$CHKB_StopTrig_ConfigLOG=1117												; controlID for stop trigger in Configure Logging Dialog
$LogStopTrigger=1117												; controlID for stop trigger in Configure Logging Dialog
$TXT_StartMsgID_ConfigLOG=1095													; controlID for start trigger MSGID Edit box in Configure Logging Dialog
$LogStartMsgID=1095													; controlID for start trigger MSGID Edit box in Configure Logging Dialog
$TXT_StopMsgID_ConfigLOG=1096													; controlID for stop trigger MSGID Edit box in Configure Logging Dialog
$LogStopMsgID=1096													; controlID for stop trigger MSGID Edit box in Configure Logging Dialog
$BTN_Filter_ConfigLOG=1121											; controlID for filter button in Configure Logging Dialog
$LogFilterBTNctrlID=1121											; controlID for filter button in Configure Logging Dialog
$BTN_OK_ConfigLOG=1													; controlID for OK button in Configure Logging Dialog
$LogOKBTNctrlID=1													; controlID for OK button in Configure Logging Dialog
$BTN_SaveInst_SelLogFile="[CLASS:Button; INSTANCE:2]"						; Save button instance in file->configuration->Log->Select a Log file
$savebtnlogInst2="[CLASS:Button; INSTANCE:2]"						; Save button instance in file->configuration->Log->Select a Log file
$LSTC_LogFile_ConfigLOG="[CLASS:SysListView32; INSTANCE:1]"						; List View instance in Logging dialog for Log file
$logFilelv="[CLASS:SysListView32; INSTANCE:1]"						; List View instance in Logging dialog for Log file

;==============================================
; Configure Transmission Messages
;==============================================
$BTN_AutoUpdate_ConfigTX=17000												; controlID for 'Auto Update' Checkbox in TX window
$autoUpdatectrlID=17000												; controlID for 'Auto Update' Checkbox in TX window
$BTN_AddBlock_ConfigTX=1226												; controlID for 'Add' msg block in TX window
$addMsgBlockctrlID=1226												; controlID for 'Add' msg block in TX window
$TXT_MsgDelay_ConfigTX=1134											; controlID for 'Time delay btw msgs' edit box in TX window
$timedelayMsgctrlID=1134											; controlID for 'Time delay btw msgs' edit box in TX window
$COMB_MsgID_ConfigTX=1221											; controlID for name/ID combo box in TX window
$msgIDNameComboctrlID=1221											; controlID for name/ID combo box in TX window
$RBTN_Exd_ConfigTX=1213												; controlID for 'Exd' msg type radio button in TX window
$exdMsgTypectrlID=1213												; controlID for 'Exd' msg type radio button in TX window
$RBTN_Std_ConfigTX=1215												; controlID for 'Std' msg type radio button in TX window
$stdMsgTypectrlID=1215												; controlID for 'Std' msg type radio button in TX window
$BTN_AddMsg_ConfigTX=1122												; controlID for 'Add Message' button in Tx window
$addTxMsgBTNctrlID=1122												; controlID for 'Add Message' button in Tx window
$TXT_MsgID_ConfigTX=1001											; controlID for name/ID edit box in TX window
$msgIDNameEditctrlID=1001											; controlID for name/ID edit box in TX window
$BTN_Close_ConfigTX=1015											; controlID for 'Close' button  in Configure Tx window
$closeTxWinBTNctrlID=1015											; controlID for 'Close' button  in Configure Tx window
$CHKB_KeyTrig_ConfigTX=1237														;1237 is the controlID for enabling Key trigger
$keyctrlID=1237														;1237 is the controlID for enabling Key trigger
$CHKB_BlockDelay_ConfigTX=17001										;17001 is the controlID for enabling time delay between blocks
$timedelayBtwblckCtrlID=17001										;17001 is the controlID for enabling time delay between blocks
$TXT_DLC_ConfigTX=1125												;1125 is the Control ID for DLC
$dLCctrlID=1125														;1125 is the Control ID for DLC
$RBTN_Monoshot_ConfigTX=1008											;1008 is the Control ID for Tx mode monoshot
$txmodemonoshotCtrlID=1008											;1008 is the Control ID for Tx mode monoshot
$TXT_KeyValue_ConfigTX=1137													;1137 is the Control ID for Key trigger Editbox
$keyeditCtrlID=1137													;1137 is the Control ID for Key trigger Editbox
$COMB_KeyType_ConfigTX=1010												;1010 is the Control Id for selecting key type in combo box
$keycomboCtrlId=1010												;1010 is the Control Id for selecting key type in combo box
$TXT_DB1_ConfigTX=1126												;1126 is the control ID for databyte of 1st block
$databyte1CtrlId=1126												;1126 is the control ID for databyte of 1st block
$TXT_DB2_ConfigTX=1127												;1127 is the control ID for databyte of 2nd block
$databyte2CtrlId=1127												;1127 is the control ID for databyte of 2nd block
$TXT_DB3_ConfigTX=1128												;1128 is the control ID for databyte of 3rd block
$databyte3CtrlId=1128												;1128 is the control ID for databyte of 3rd block
$TXT_DB4_ConfigTX=1129												;1129 is the control ID for databyte of 4th block
$databyte4CtrlId=1129												;1129 is the control ID for databyte of 4th block
$TXT_DB5_ConfigTX=1130												;1130 is the control ID for databyte of 5th block
$databyte5CtrlId=1130												;1130 is the control ID for databyte of 5th block
$TXT_DB6_ConfigTX=1131												;1131 is the control ID for databyte of 6th block
$databyte6CtrlId=1131												;1131 is the control ID for databyte of 6th block
$TXT_DB7_ConfigTX=1132												;1132 is the control ID for databyte of 7th block
$databyte7CtrlId=1132												;1132 is the control ID for databyte of 7th block
$TXT_DB8_ConfigTX=1133												;1133 is the control ID for databyte of 8th block
$databyte8CtrlId=1133												;1133 is the control ID for databyte of 8th block
$COMB_ChannelID_ConfigTX=1010											; 1010 is the Control ID for channel
$ChannelcomboCtrlID=1010											; 1010 is the Control ID for channel
$BTN_Update_ConfigTX=1014													;1014 is contol id for update button
$updatecrtlid=1014													;1014 is contol id for update button
$CHKB_MsgDelay_ConfigTX=1236														;1236 is control id for enabling time delay between msgs
$timdeltrg=1236														;1236 is control id for time delay between trigger
$TXT_BlockDelay_ConfigTX=17002												;17002 is the control id for setting message delay btwn blocks
$timedelayBtmedit=17002												;17002 is the control id for setting message delay btwn blocks
$BTN_DeleteBlock_ConfigTX=1227										;deleting msg block
$delbutmsg="[CLASS:Button; INSTANCE:15]"							;deleting msg block
$BTN_Yes_Bumsaster=6												;clicking on yes botton to delete
$yesbtndel="[CLASS:Button; INSTANCE:1]"								;clicking on yes botton to delete
$LVC_MsgBlock_ConfigTX="[CLASS:SysListView32; INSTANCE:3]"			;List View instance for messages in message block
$msglistInst="[CLASS:SysListView32; INSTANCE:3]"					;List View instance for messages in message block

;==============================================
;Configure Filter List
;==============================================
$WIN_Config_filter="Configure Filter List"
$dlgConfigfilter="Configure Filter List"
$BTN_AddFilter_ConfigFilter=1228											; controlID for 'Add' button in Configure Filter window
$addFilterBTNctrlID=1228											; controlID for 'Add' button in Configure Filter window
$COMB_MsgID_ConfigFilter=1004												; controlID for 'ID' combo box in Configure Filter window
$msgIDFilterctrlID=1004												; controlID for 'ID' combo box in Configure Filter window
$COMB_IDType_ConfigFilter=1009												; controlID for 'Id Type' combo box in Configure Filter window
$msgIDtypectrlID=1009												; controlID for 'Id Type' combo box in Configure Filter window
$COMB_MsgType_ConfigFilter=1010													; controlID for 'Frame' combo box in Configure Filter window
$msgtypectrlID=1010													; controlID for 'Frame' combo box in Configure Filter window
$COMB_Direction_ConfigFilter=1011											; controlID for 'Direction' combo box in Configure Filter window
$msgDirectionctrlID=1011											; controlID for 'Direction' combo box in Configure Filter window
$COMB_Channel_ConfigFilter=1012												; controlID for 'Channel' combo box in Configure Filter window
$msgChannelctrlID=1012												; controlID for 'Channel' combo box in Configure Filter window
$BTN_Add_ConfigFilter=3											; controlID for 'Add' to filter list button  in Configure Filter window
$addFiltertoListctrlID=3											; controlID for 'Add' to filter list button  in Configure Filter window
$BTN_OK_ConfigFilter=1256												; controlID for 'OK' button  in Configure Filter window
$OKFilterBTNctrlID=1256												; controlID for 'OK' button  in Configure Filter window
$LVC_FilterList_ConfigFilter="[CLASS:SysListView32; INSTANCE:1]"						; List View instance in Configure Filter List window
$filterList="[CLASS:SysListView32; INSTANCE:1]"						; List View instance in Configure Filter List window

;==============================================
;Data declaration for About BUSMASTER window
;==============================================

$DLG_About="About"
$DlgAboutBM="About"
$BTN_OK_About=1

;==============================================
;Data declaration for Window/Dialog Title
;==============================================
$WIN_BUSMASTER="BUSMASTER"
$mWin="BUSMASTER"
$DLG_Hardware="Hardware Selection"
$dlghWare="Hardware Selection"



$WIN_SelCAN_DBFile="Select BUSMASTER Database Filename..."
$dlgoDbf="Select BUSMASTER Database Filename..."
$WIN_SaveAs="Save As"
$dlgsaveAs="Save As"
$WIN_DBEditor_CAN="BUSMASTER - [DatabaseEditor - CAN]"
$windbEditCAN="BUSMASTER - [DatabaseEditor - CAN]"
$WIN_DBEditor_J1939="BUSMASTER - [DatabaseEditor - J1939]"
$windbEditJ1939="BUSMASTER - [DatabaseEditor - J1939]"
$WIN_Msg_Details="Message Details"
$dlgmsgDtails="Message Details"
$WIN_Signal_Details="Signal Details"
$dlgsigDetails="Signal Details"
$WIN_Associate_CANDB="Select Active Database Filename..."
$dlgasoDB="Select Active Database Filename..."
$WIN_CANLog="Configure Logging for CAN"
$dlgcfgLog="Configure Logging for CAN"
$WIN_Format_CNV="BUSMASTER Format Conversions"
$dlgfConvert="BUSMASTER Format Conversions"
$WIN_Signal_Desc="Enter Value Descriptor and Value"
$dlgsigDesc="Enter Value Descriptor and Value"
$WIN_SelJ1939_DBFile="Select J1939 Database Filename..."
$dlgselJ1939Fl="Select J1939 Database Filename..."
$WIN_Dissociate_CANDB="Dissociate Database"
$disDbDialog="Dissociate Database"
$dlgfrmtConvWin="BUSMASTER Format Conversions"
$WIN_fCOpen="Open"
$dlgfCOpenWin="Open"
$DLG_Kvaser="CAN_Kvaser_CAN"
$dlgkvaserCANWin="CAN_Kvaser_CAN"
$DLG_Busmaster="BUSMASTER"
$dlgBusmaster="BUSMASTER"
$WIN_NewDBFile="New Database file name..."
$dlgNewDBFile="New Database file name..."
$WIN_Config_filter="Configure Filter List"
$dlgConfigfilter="Configure Filter List"
$WIN_MsgDisplay="Configure Message Display- CAN"
$dlgMsgDisp="Configure Message Display- CAN"
$WIN_FilterSel="Filter Selection Dialog"
$dlgFilSel="Filter Selection Dialog"
$WIN_J1939MsgWind="BUSMASTER - [Message Window - J1939]"
$J1939MsgWind="BUSMASTER - [Message Window - J1939]"
$WIN_Select_LogFile="Select a Log file"
$dlgSelLogFile="Select a Log file"
$WIN_DefaultCfxSave="Save Configuration File"
$DlgDefaultConfigSave="Save Configuration File"

;==============================================
;Data declaration for Menu Text
;==============================================

$fileMenu="&File"
$configMenu="Confi&guration"
$loadMenu="&Load"
$newMenu="&New"
$CANMenu="&CAN"
$databaseMenu="&Database"
$AssocCANDB="&Associate"
$DissocCANDB="&Disassociate"
$openMenu="&Open"
$saveMenu="&Save"
$closeMenu="&Close"
$saveImportMenu="Save and Im&port"
$configureMenu="&Configure"
$configHWMenu="&Configure"
$HardwareMenu="&Hardware"
$HWIMenu="&Hardware Interface"
$kvaserHWMenu="&Kvaser CAN"
$configTxmenu="&Configure"
$TxMsgMenu="&Transmit"
$AppFilterMenu="&Filter Configuration"
$MsgDisplay="&Message Display"
$FunctionsMenu="F&unctions"
$enableFilterMenu="Enable &Filter"
$displayMenu="&Display"
$EnableFilterDisp="&Enable Filters"
$EnableFilterLogMenu="&Enable Filters"
$transmitMenu="&Transmit"
$EnableTransmitMenu="&Enable"
$normalBlckMenu="&Normal Blocks"
$CANMsgWinMenu="&Message Window"
$J1939Menu="&J1939"
$J1939ViewMenu="&View"
$J1939MsgWinMenu="&Message Window"
$J1939MsgWinAct="&Activate"
$CANLogMenu="&Logging"
$LogMenu="L&og"
$LogFilterMenu="&Log"
$HelpMenu="&Help"
$AboutBusmaster="&About BUSMASTER"
;==============================================
;Data declaration for Button Text
;==============================================
$BTN_Select="&Select"
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
$BTN_Yes_BM=6														; controlID for 'Yes' button in a warning pop up message
$yesBTNctrlID=6														; controlID for 'Yes' button in a warning pop up message
$saveCfxyesBTNctrlID=6												; controlID for 'Yes' button in save config warning pop up message
;~ $loadCfxfNamectrlID=1152											; controlID for the 'File Name' Edit control in 'Load Configuration filename' dialog
;~ $loadCfxloadctrlID=1												; controlID for the 'Load' button in 'Load Configuration filename' dialog
$loadCfxcancelctrlID=2												; controlID for the 'Cancel' button in 'Load Configuration filename' dialog
;~ $selDbffNameCtrlID=1152											; controlID for the 'File Name' Edit control in 'Select BUSMASTER Database Filename' dialog
;~ $selDbfOpenctrlID=1												; controlID for the 'Open' button in 'Select BUSMASTER Database Filename' dialog
$DbfNotFoundOKctrlID=2												; controlID for the 'OK' button in 'DBF file not found' dialog
;~ $saveAsfName=1152												; controlID for the 'File Name' Edit control in 'Save As' dialog for new database file
;~ $saveAsSave=1													; controlID for 'Save As' button in Save As window
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
$ToolBar_Con_Disconnect=128											; controlID for Connect toolbar
$connectToolbarctrlID=128											; controlID for Connect toolbar
$addLogBTNctrlID=1303												; controlID for Add button in Configure Logging Dialog
$enableDisLogctrlID=1092											; controlID for "&Enable/Disable Logging during tool Connect/Disconnect" in Configure Logging Dialog
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
$ToolBar_OverWrite=314												; controlID for Overwrite toolbar
$OverwriteToolbarctrlID=314											; controlID for Overwrite toolbar
$Static_Version_About=1153											; controlID for Busmaster version in About dialog
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
$delbutmsg="[CLASS:Button; INSTANCE:15]"							; Deleting msg block
$configFilMsgDisp="[CLASS:SysListView32; INSTANCE:1]"				; List View instance in Filter List Message display dialog window
$LSTC_CANMsgWin="[CLASS:SysListView32; INSTANCE:1]"					; List View instance in Message window
$msgWinlv="[CLASS:SysListView32; INSTANCE:1]"						; List View instance in Message window
$LSTC_CANMsgWinInst4="[CLASS:SysListView32; INSTANCE:4]"							; message window list view
$savebtnlogInst2="[CLASS:Button; INSTANCE:2]"						; Save button instance in file->configuration->Log->Select a Log file
$logFilelv="[CLASS:SysListView32; INSTANCE:1]"						; List View instance in Logging dialog for Log file
$yesbtndel="[CLASS:Button; INSTANCE:1]"								; Clicking on yes button to delete
$DefConfigFilePath="[CLASS:Edit; INSTANCE:1]"						; Edit control instance of File path in "Save Configuration File" for default config
$DefConfigFileSaveBTNinst2="[CLASS:Button; INSTANCE:2]"				; Save button instance in "Save Configuration File" for default config
$msglistInst="[CLASS:SysListView32; INSTANCE:3]"					; List View instance for messages in message block

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