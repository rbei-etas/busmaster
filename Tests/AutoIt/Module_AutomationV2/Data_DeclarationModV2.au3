Global $mWin, $hWare, $selectBTN, $okBTN, $ldCfx, $loadBTN, $nwCfx,$oDbf, $saveAs, $dbEditCAN, $msgDtails, $sigDetails, $ok1BTN, $nSigBTN
Global $crashRes, $lConfig, $cConfig,$channelCount,$time,$oExcel

;==============================================
; Bus Type declaration
;==============================================
$CAN="CAN"
$J1939="J1939"
$LIN="LIN"

$AppProcess="BUSMASTER.exe"
$LVC_CID_CANMsgWin=200	                                              ; Message window control ID
$LVC_CID_J1939MsgWin=200                                              ; Message window control ID for J1939
;==============================================
; BUSMASTER Registry Cleanup Setup: Completed
;==============================================

$WIN_RegClean="BUSMASTER Registry Cleanup Setup: Completed"
$BTN_Close_RegClean=1												; ControlID for 'Close' button
;==============================================
; Save Configuration File
;==============================================

;$TXT_FilePath_SaveConfigFile=1152									; Edit control instance of File path in "Save Configuration File" for default config
$TXT_FilePath_SaveConfigFile1="[CLASS:Edit; INSTANCE:1]"
$TXT_FilePath_SaveConfigFile=1001									; Edit control instance of File path in "Save Configuration File" for default config
$DefConfigFilePath="[CLASS:Edit; INSTANCE:1]"						; Edit control instance of File path in "Save Configuration File" for default config
$DefConfigFileSaveBTNinst2="[CLASS:Button; INSTANCE:2]"				; Save button instance in "Save Configuration File" for default config
$BTN_Save_SaveConfigFile="[CLASS:Button; INSTANCE:2]"				; Save button instance in "Save Configuration File" for default config
$BTN_Yes_SaveConfig=6												; controlID for 'Yes' button in save config warning pop up message
$BTN_No_SaveConfig=7												; controlID for 'No' button in save config warning pop up message
$saveCfxyesBTNctrlID=6
$BTN_Yes_Busmaster=6												; controlID for 'Yes' button in save config warning pop up message
$saveConfigtxt="Configuration File is modified"						; Save configuration dialog text
$SaveChangesConfigTXT="You have made changes to the configuration"
;==============================================
; New Configuration Filename
;==============================================
$MENU_Configuration="Confi&guration"
$WIN_NewCfx="New Configuration Filename..."
$dlgnwCfx="New Configuration Filename..."
;$TXT_FileName_NewCfx=1152											    ; controlID for the 'File Name' Edit control
$TXT_FileName_NewCfx=1001											    ; controlID for the 'File Name' Edit control
;$newCfxfNamectrlID=1152											        ; controlID for the 'File Name' Edit control
$newCfxfNamectrlID=1001											        ; controlID for the 'File Name' Edit control
$BTN_Save_NewCfx=1														; controlID for the 'Save' button
$newCfxSavectrlID=1														; controlID for the 'Save' button
;$BTN_SaveInst_NewCfx="[CLASS:Button; INSTANCE:2]"						; Save button instance in file->configuration->new window
$BTN_SaveInst_NewCfx="[CLASS:Button; INSTANCE:1]"
$savebtncfxInst2="[CLASS:Button; INSTANCE:2]"						; Save button instance in file->configuration->new window
;==============================================
; Load Configuration Filename
;==============================================

$WIN_LoadCfx="Load Configuration Filename..."
$dlgldCfx="Load Configuration Filename..."
;$TXT_FileName_LoadCfx=1152											    ; controlID for the 'File Name' Edit control
$TXT_FileName_LoadCfx="[CLASS:Edit; INSTANCE:1]"											    ; controlID for the 'File Name' Edit control
;$loadCfxfNamectrlID=1152												; controlID for the 'File Name' Edit control
$loadCfxfNamectrlID=1001												; controlID for the 'File Name' Edit control
$BTN_Load_LoadCfx=1														; controlID for the 'Load' button
$loadCfxloadctrlID=1													; controlID for the 'Load' button
$BTN_Cancel_LoadCfx=2													; controlID for the 'Cancel' button
$loadCfxcancelctrlID=2													; controlID for the 'Cancel' button
;$BTN_LoadInst_LoadCfx="[CLASS:Button; INSTANCE:2]"						; Load button instance in Load Configuration window
$BTN_LoadInst_LoadCfx="[CLASS:Button; INSTANCE:1]"						; Load button instance in Load Configuration window
$loadCfxloadInst2="[CLASS:Button; INSTANCE:2]"						    ; Load button instance in Load Configuration window

;==============================================
; Save As Configuration
;==============================================
$WIN_SaveAsCfx="SaveAs Configuration Filename..."
;$TXT_FileName_SaveAsCfx=1152                                           ; controlID for the 'File Name' Edit control
$TXT_FileName_SaveAsCfx=1001                                           ; controlID for the 'File Name' Edit control
;$BTN_Save_SaveAsCfx="[CLASS:Button; INSTANCE:2]"                        ; controlID for the 'Save' button
$BTN_Save_SaveAsCfx="[CLASS:Button; INSTANCE:1]"                        ; controlID for the 'Save' button
$BTN_Yes_ConfirmSaveAs="[CLASS:Button; INSTANCE:1]"
$WIN_ConfirmSaveAs="Confirm Save As"
;==============================================
; New Database Filename
;==============================================
$WIN_NewDBFile="New Database file name..."
;$TXT_FileName_NewDB=1152												; controlID for the 'File Name' Edit control
$TXT_FileName_NewDB=1001												; controlID for the 'File Name' Edit control
$BTN_Save_NewDB=1														; controlID for 'Save As' button
$BTN_Cancel_NewDB=2														; controlID for 'Cancel' button
;$BTN_SaveInst_NewDB="[CLASS:Button; INSTANCE:2]"						; Save button instance in file->database->new window
$BTN_SaveInst_NewDB="[CLASS:Button; INSTANCE:1]"						; Save button instance in file->database->new window
$savebtnDBInst2="[CLASS:Button; INSTANCE:2]"						; Save button instance in file->database->new window

;==============================================
; Select BUSMASTER Database Filename
;==============================================
$WIN_SelCAN_DBFile="Select BUSMASTER Database Filename..."
$dlgoDbf="Select BUSMASTER Database Filename..."
;$TXT_FileName_OpenDB=1148												; controlID for the 'File Name' Edit control in 'Select BUSMASTER Database Filename' dialog
$TXT_FileName_OpenDB="[CLASS:Edit; INSTANCE:1]"												; controlID for the 'File Name' Edit control in 'Select BUSMASTER Database Filename' dialog
;$selDbffNameCtrlID=1152												; controlID for the 'File Name' Edit control in 'Select BUSMASTER Database Filename' dialog
$selDbffNameCtrlID=1001												; controlID for the 'File Name' Edit control in 'Select BUSMASTER Database Filename' dialog
;$selDbffNameCtrlID=1148												; controlID for the 'File Name' Edit control in 'Select BUSMASTER Database Filename' dialog
$BTN_Open_OpenDB=1													; controlID for the 'Open' button in 'Select BUSMASTER Database Filename' dialog
$selDbfOpenctrlID=1													; controlID for the 'Open' button in 'Select BUSMASTER Database Filename' dialog
;$BTN_OpenInst_OpenDB="[CLASS:Button; INSTANCE:2]"						; Open button instance in file->database->open window
$BTN_OpenInst_OpenDB="[CLASS:Button; INSTANCE:1]"						; Open button instance in file->database->open window
$opendbfBTNInst2="[CLASS:Button; INSTANCE:2]"						; Open button instance in file->database->open window
;==============================================
; Select Active Database Filename
;==============================================
$WIN_Associate_CANDB="Select Active Database Filename..."
$dlgasoDB="Select Active Database Filename..."
;$TXT_FileName_AssocDB=1152											; controlID for the 'File Name' Edit control in 'Select Active Database Filename...' dialog
$TXT_FileName_AssocDB="[CLASS:Edit; INSTANCE:1]"											; controlID for the 'File Name' Edit control in 'Select Active Database Filename...' dialog
;$AssocfNamectrlID=1152											    ; controlID for the 'File Name' Edit control in 'Select Active Database Filename...' dialog
$AssocfNamectrlID=1001											    ; controlID for the 'File Name' Edit control in 'Select Active Database Filename...' dialog
;$BTN_Open_AssocDB="[CLASS:Button; INSTANCE:2]"						; Open button instance in file->database->Associate window
$BTN_Open_AssocDB="[CLASS:Button; INSTANCE:1]"						; Open button instance in file->database->Associate window
$assocdbfBTNinst2="[CLASS:Button; INSTANCE:2]"						; Open button instance in file->database->Associate window
;$BTN_Open_AssocDBJ1939="[CLASS:Button; INSTANCE:2]"
$BTN_Open_AssocDBJ1939="[CLASS:Button; INSTANCE:1]"

;==============================================
; Dissociate Database
;==============================================

$WIN_Dissociate_CANDB="Dissociate Database"
$BTN_Dissoc_Dis_CANDB=1285											; controlID for the dissociate button in Dissociate database window
$dissocBTNctlID=1285												; controlID for the dissociate button in Dissociate database window
;$BTN_Cancel_Dis_CANDB=2												; controlID for 'Cancel' button in Dissociate DB window
$BTN_Cancel_Dis_CANDB="OK"												; controlID for 'Cancel' button in Dissociate DB window
$cancelDisBTNctrlID=2												; controlID for 'Cancel' button in Dissociate DB window
$LSTB_DBFile_Dis_CANDB="[CLASS:ListBox; INSTANCE:1]"				; List Box instance in File->database->dissociate window
$dissocdbflbinst1="[CLASS:ListBox; INSTANCE:1]"						; List Box instance in File->database->dissociate window
$disDBtxt="Dissociation will clear Signal Watch List."				; Dissociate DB pop up message text
$disDBtxt1="Are you sure you want to dissociate the file(s)?"		; Dissociate DB pop up message text
$BTN_Yes_disDBtxt=6                                                 ; Control id for Yes button for Dissociate DB text popup message
;==============================================
; Database Editor - CAN
;==============================================
$WIN_DBEditor_CAN="BUSMASTER - [DatabaseEditor - CAN]"
$WIN_DBEditor_CAN1 = "DatabaseEditor - CAN"
$WIN_DBEditor_J1939="BUSMASTER - [DatabaseEditor - J1939]"
$WIN_DBEditor_J19391="DatabaseEditor - J1939"
$windbEditCAN="BUSMASTER - [DatabaseEditor - CAN]"
$WIN_Msg_Details="Message Details"
$dlgmsgDtails="Message Details"
$WIN_Signal_Details="Signal Details"
$dlgsigDetails="Signal Details"
$WIN_Signal_Desc="Enter Value Descriptor and Value"
$dlgsigDesc="Enter Value Descriptor and Value"
$WIN_DBSaveAs="Save As..."
$WIN_J1939SaveAs="Save As"
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
$EDIT_Factor_SigDetails=1003										; Signal scale factor edit controlID
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
$BTN_Yes_SaveDB=6													; ControlID for 'Yes' button
$BTN_No_SaveDB=7													; ControlID for 'No' button
$RBTN_Motorola_SigDetails=1307										; Motorola radio button control ID
;$Edit_FName_SaveAsDB=1152											; Filename edit control ID in Save As window
$Edit_FName_SaveAsDB=1001											; Filename edit control ID in Save As window
;$BTN_SaveInst_SaveAsDB="[CLASS:Button; INSTANCE:2]"					; Save button control ID in Save As window
$BTN_SaveInst_SaveAsDB="[CLASS:Button; INSTANCE:1]"					; Save button control ID in Save As window
$LVC_SigDet_DBeditor="[CLASS:SysListView32; INSTANCE:1]"					; List View instance for Signal Details in database editor window
$sigDetlvInst1="[CLASS:SysListView32; INSTANCE:1]"					; List View instance for Signal Details in database editor window
$LVC_SigDesc_SigDesc="[CLASS:SysListView32; INSTANCE:2]"					; List View instance for Signal Description in database editor window
$sigDesclvInst1="[CLASS:SysListView32; INSTANCE:2]"					; List View instance for Signal Description in database editor window


$sigDescDelmsgtxt="Are you sure you want to delete?"					; delete Signal description pop up message text
$sigDelmsgtxt="You are about to delete a signal."						; delete Message Signal pop up message text
$msgDeltxt="You are about to Delete a Message."							; delete message pop up dialog text
$saveDBtxt="Do you want to save changes to the database file?"			; Save DB changes warning message
$TXT_DupMsgName_BM="Duplicate message name found!"						; Duplicate Msg name text
$BTN_OKDupMsgName_BM=2													; OK button control ID in duplicate Msg name warning dialog
$TXT_DupMsgID_BM="Duplicate message code found!"						; Duplicate Msg ID text
$BTN_OKDupMsgID_BM=2													; OK button control ID in duplicate Msg ID warning dialog
$TXT_MsgIDValidation_BM="Please enter integer between 0 and 8"			; Msg ID validation for ID greater 8
$TXT_J1939MsgIDValidation_BM="Please enter integer between 0 and 1785"	; Msg ID validation for ID greater 1785
$BTN_MsgIDValidation_BM=2												; OK button control ID in Msg ID validation warning dialog
$TXT_DupSigName_BM="Signal name already exists!"						; Duplicate signal name text
$BTN_OKDupSigName_BM=2													; OK button control ID in Duplicate signal name warning dialog
$TXT_DupStartBit_BM="Duplicate signal start bit value"					; Duplicate signal start bit text
$BTN_DupStartBit_BM=2													; OK button control ID in Duplicate signal start bit warning dialog
;$TXT_InvStartBit="Please enter a valid number between 0 and 7."			; Invalid Start Bit text
$TXT_InvStartBit="Enter an integer between 0 and 7."			; Invalid Start Bit text
$BTN_OKInvStartBit_BM=2													; OK button control ID in  Invalid Start Bit warning dialog
;$TXT_InvSignalLen_BM="Please enter a valid number between 1 and 64."	; Invalid signal length text
$TXT_InvSignalLen_BM="Enter an integer between 1 and 64."	; Invalid signal length text
$BTN_InvSignalLen_BM=2													; OK button control ID in Invalid signal length warning dialog
$TXT_InvEndBit_BM="End bit of signal is out of message length"			; Invalid end bit text
$BTN_InvEndBit_BM=2														; OK button control ID in Invalid end bit warning dialog
$TXT_EmptySigDesc_BM="Signal Description cannot be emtpy!"				; Empty signal description text
$BTN_OKEmptySigDesc_BM=2												; OK button control ID in Empty signal description warning dialog
$TXT_DupSigDesc_BM="Signal Descriptor already exists!"					; Duplicate signal descriptor text
$BTN_OKDupSigDesc_BM=2													; Ok button control ID in Duplicate signal descriptor warning dialog
$TXT_InvSigValRange_BM="Signal value is out of range!"					; Signal value out of range text
$BTN_OKInvSigValRange_BM=2												; Ok button control ID in Signal value out of range warning dialog
$TXT_ReduceMsgLen_BM="You have signals defined for this message for previous message length."	; Reducing message length warning dialog
$BTN_YesReduceMsgLen_BM=6												; 'Yes' button control ID in reducing message length warning dialog
$BTN_NoReduceMsgLen_BM=7												; 'No' button control ID in reducing message length warning dialog
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
$WIN_MsgDisplay="Configure Message Display - CAN"
$dlgMsgDisp="Configure Message Display - CAN"
$BTN_Configure_MsgDisplay=1031										; controlID for 'Configure' button  in Configure msg display window
$confMsgDispBTNctrlID=1031											; controlID for 'Configure' button  in Configure msg display window
$BTN_OK_MsgDisplay=1												; controlID for 'OK' button  in Configure msg display window
$okConMsgDispBTNctrlID=1											; controlID for 'OK' button  in Configure msg display window
$TabConfigMsgDisp=12320                                             ;Control id for message display configure tab
$LSTDBMsgs=1235                                                     ;List view of DB messages
$CANMsgDispConfigMenu="C"
$LSTNonDBMsgs=1235
;==============================================
; Filter Selection Dialog
;==============================================
$WIN_FilterSelect="Filter Selection Dialog"
$dlgFilSel="Filter Selection Dialog"
$BTN_Add_FilterSelect=32950															; controlID for 'Add' button
$AddFilMsgDispBTNctrlID=32950														; controlID for 'Add' button
$BTN_OK_FilterSelect=1																; controlID for 'OK' button
$okFilSelDlgBTNctrlID=1																; controlID for 'OK' button
$RBTN_Range_FilterSelect=1254														; controlID for 'Range' button
$EDIT_From_FilterSelect=1041														; controlID for 'From' edit control
$EDIT_To_FilterSelect=1042															; controlID for 'To' edit control
$LSTC_ConfigFilter_FilterSelect="[CLASS:SysListView32; INSTANCE:1]"					; List View instance in Filter List Message display dialog window
$LSTC_SelectedFilter_FilterSelect="[CLASS:SysListView32; INSTANCE:2]"				; Selected Filter List View instance
$configFilMsgDisp="[CLASS:SysListView32; INSTANCE:1]"								; List View instance in Filter List Message display dialog window



;==============================================
; Configure Logging for CAN
;==============================================
$WIN_CANLog="Configure Logging for CAN"
$WIN_Select_LogFile="Select a Log file"
;$BTN_Add_ConfigLOG=1303													; controlID for Add button in Configure Logging Dialog
$BTN_Add_ConfigLOG=2000														; controlID for Add button in Configure Logging Dialog
;$BTN_RemoveLog_ctrlID=1304
$BTN_RemoveLog_ctrlID=2018
;$BTN_EnDisable_ConfigLOG=1092												; controlID for ""&Enable/Disable Logging during tool Connect/Disconnect" in Configure Logging Dialog
$BTN_EnDisable_ConfigLOG=2017												; controlID for ""&Enable/Disable Logging during tool Connect/Disconnect" in Configure Logging Dialog
;$BTN_logFPath_ConfigLOG=1122												; controlID for 'LOGFILEPATH" button in Configure Logging Dialog
;$BTN_logFPath_ConfigLOG=1122												; controlID for 'LOGFILEPATH" button in Configure Logging Dialog
$BTN_logFPath_ConfigLOG=2003												; controlID for 'LOGFILEPATH" button in Configure Logging Dialog
;$TXT_FileName_SelLogFile=1152												; controlID for File name edit control in Select a Log File dialog
$TXT_FileName_SelLogFile=1001												; controlID for File name edit control in Select a Log File dialog
;$TXT_FileName_SelLogFile=2033												; controlID for File name edit control in Select a Log File dialog
;$COMB_TimeMode_ConfigLOG=1101												; controlID for Time Mode combobox in Configure Logging Dialog
$COMB_TimeMode_ConfigLOG=2005												; controlID for Time Mode combobox in Configure Logging Dialog
;$COMB_Channel_ConfigLOG=1102												; controlID for channel combobox in Configure Logging Dialog
$COMB_Channel_ConfigLOG=2006												; controlID for channel combobox in Configure Logging Dialog
;$RBTN_NumModeDec_ConfigLOG=1081												; controlID for Decimal numeric mode in Configure Logging Dialog
$RBTN_NumModeDec_ConfigLOG=2008											; controlID for Decimal numeric mode in Configure Logging Dialog
;$RBTN_NumModeHex_ConfigLOG=1082												; controlID for Hex numeric mode in Configure Logging Dialog
$RBTN_NumModeHex_ConfigLOG=2009												; controlID for Hex numeric mode in Configure Logging Dialog
;$RBTN_FileModeApp_ConfigLOG=1350											; controlID for Append File mode in Configure Logging Dialog
$RBTN_FileModeApp_ConfigLOG=2011											; controlID for Append File mode in Configure Logging Dialog
;$RBTN_FileModeOverWrite_ConfigLOG=1351										; controlID for Overwrite File mode in Configure Logging Dialog
$RBTN_FileModeOverWrite_ConfigLOG=2012										; controlID for Overwrite File mode in Configure Logging Dialog
;$CHKB_StartTrig_ConfigLOG=1116												; controlID for start trigger in Configure Logging Dialog
$CHKB_StartTrig_ConfigLOG=2015												; controlID for start trigger in Configure Logging Dialog
;$CHKB_StopTrig_ConfigLOG=1117												; controlID for stop trigger in Configure Logging Dialog
$CHKB_StopTrig_ConfigLOG=2016											    ; controlID for stop trigger in Configure Logging Dialog
;$TXT_StartMsgID_ConfigLOG=1095												; controlID for start trigger MSGID Edit box in Configure Logging Dialog
$TXT_StartMsgID_ConfigLOG=2021												; controlID for start trigger MSGID Edit box in Configure Logging Dialog
;$TXT_StopMsgID_ConfigLOG=1096												; controlID for stop trigger MSGID Edit box in Configure Logging Dialog
$TXT_StopMsgID_ConfigLOG=2022												; controlID for stop trigger MSGID Edit box in Configure Logging Dialog
;$BTN_Filter_ConfigLOG=1121													; controlID for filter button in Configure Logging Dialog
$BTN_Filter_ConfigLOG=2025													; controlID for filter button in Configure Logging Dialog
$BTN_OK_ConfigLOG=1															; controlID for OK button in Configure Logging Dialog
;$BTN_RemoveLog_ConfigLog=1304												; Control ID for Remove button
$BTN_RemoveLog_ConfigLog=2018												; Control ID for Remove button
;$BTN_SaveInst_SelLogFile="[CLASS:Button; INSTANCE:2]"						; Save button instance in file->configuration->Log->Select a Log file
$BTN_SaveInst_SelLogFile="[CLASS:Button; INSTANCE:1]"						; Save button instance in file->configuration->Log->Select a Log file
$LSTC_LogFile_ConfigLOG="[CLASS:SysListView32; INSTANCE:1]"					; List View instance in Logging dialog for Log file
;$LSTC_LogFile_ConfigLOG=2019				; List View instance in Logging dialog for Log file

;==============================================
; Configure Logging for J1939
;==============================================
$WIN_J1939Log="Configure Logging for J1939"

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
$RBTN_Cyclic_ConfigTx=1007
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
$COMB_ChannelID_ConfigTX=1011											; 1010 is the Control ID for channel
$ChannelcomboCtrlID=1010											; 1010 is the Control ID for channel
$BTN_Update_ConfigTX=1014													;1014 is contol id for update button
$updatecrtlid=1014													;1014 is contol id for update button
$CHKB_MsgDelay_ConfigTX=1236														;1236 is control id for enabling time delay between msgs
$timdeltrg=1236														;1236 is control id for time delay between trigger
$CHKB_RTR_ConfigTX=1216												;1216 is control id for 'RTR' checkbox
$TXT_BlockDelay_ConfigTX=17002										;17002 is the control id for setting message delay btwn blocks
$timedelayBtmedit=17002												;17002 is the control id for setting message delay btwn blocks
$BTN_DeleteBlock_ConfigTX=1227										;deleting msg block
$delbutmsg="[CLASS:Button; INSTANCE:15]"							;deleting msg block
$BTN_Yes_Bumsaster=6												;clicking on yes botton to delete
$yesbtndel="[CLASS:Button; INSTANCE:1]"								;clicking on yes botton to delete
$LVC_MsgBlock_ConfigTX="[CLASS:SysListView32; INSTANCE:3]"			;List View instance for messages in message block
$msglistInst="[CLASS:SysListView32; INSTANCE:3]"					;List View instance for messages in message block
$LVC_TxMsgLST_ConfigTX="[CLASS:SysListView32; INSTANCE:2]"			;List View instance for Tx message list
$LSTC_MsgBlk_NameID=1225                                             ;Control Id for Message Blocks list in Configure transmission messages window
$LSTC_MsgDetailsID=1099                                              ;Control Id for Message Details List in Configure transmission messages window
$BTN_Send=1229                                                       ;Control Id for send button in Configure transmission messages window
$LSTC_Sig_DetailsID=1232                                                ;Control Id for Signal Details List view in TX window
$statusBar="[CLASS:msctls_statusbar32; INSTANCE:1]"

;==============================================
;Configure Filter List
;==============================================
$WIN_Config_filter="Configure Filter List"
$BTN_AddFilter_ConfigFilter=1228														; controlID for Add filter button
$Edit_ID_ConfigFilter=1001																; controlID for ID edit control
$COMB_MsgID_ConfigFilter=1004															; controlID for 'ID' combo box in Configure Filter window
$COMB_IDType_ConfigFilter=1009															; controlID for 'Id Type' combo box in Configure Filter window
$COMB_MsgType_ConfigFilter=1010															; controlID for 'Frame' combo box in Configure Filter window
$COMB_Direction_ConfigFilter=1011														; controlID for 'Direction' combo box in Configure Filter window
$COMB_Channel_ConfigFilter=1012															; controlID for 'Channel' combo box in Configure Filter window
$BTN_Add_ConfigFilter=3																	; controlID for 'Add' to filter list button  in Configure Filter window
$BTN_OK_ConfigFilter=1256																; controlID for 'OK' button  in Configure Filter window
$BTN_DEL_Filter=1002																	; controlID for 'Delete' button  in Configure Filter window
$BTN_DelFil_ConfigFilter=1250															; Control ID for 'Delete Filter ID' button
$BTN_DELAll_Filter=1251																	; Control ID for 'Delete All' button
$LVC_FilterList_ConfigFilter="[CLASS:SysListView32; INSTANCE:1]"						; List View instance in Configure Filter List window
$LVC_FilterDetails_ConfigFilter="[CLASS:SysListView32; INSTANCE:2]"						; Filter details List View instance in Configure Filter List window
$TXT_DelAllFilter_BM="Do you want to delete all filters from the list?"					; Delete All filter window warning message text
$BTN_DelAllFilterNO_BM=7																; Control ID for 'No' button
$BTN_DelAllFilterYes_BM=6																; Control ID for 'Yes' button

$RBTN_Event_FilterSelect=32951														     ; controlID for 'Event' radio button
$RBTN_ID_FilterSelect=1253														   		 ; controlID for 'id' radio button
$TXT_ErrorTxt=1233
$BTN_Cancel_ConfigFilter=2															     ; controlID for 'Cancel' button  in Configure Filter window
$TXT__To_FilterSelect=1255

;==============================================
;Data declaration for Replay window
;==============================================
$CANReplayMenu="RP"
$CANReplayConfigMenu="C"
$CANReplayGoMenu="G"
$CANReplayStepMenu="S"
$CANReplaySkipMenu="K"
$CANReplayStopMenu="T"
$CANEnableFilterReplayMenu="F"
$WIN_CANReplayConfig="Replay Configuration"
$BTN_DELFile_CANReplayConfig=4												; Delete button control ID
$BTN_ADDFile_CANReplayConfig=1262											; Add button control ID
$BTN_OK_CANReplayConfig=1													; OK button control ID
$BTN_Cancel_CANReplayConfig=2												; Cancel button control ID
$LVC_ReplayFile_CANReplayConfig="[CLASS:SysListView32; INSTANCE:1]"			; List View instance
$LVC_ReplayWin="[CLASS:SysListView32; INSTANCE:1]"							; List View instance
$DLG_SelReplayFile="Select Replay File Name"
$BTN_Open_SelReplayFile=1													; open button control ID
;$BTN_OpenInstSelReplayFile="[CLASS:Button; INSTANCE:2]"						; open button instance
$BTN_OpenInstSelReplayFile="[CLASS:Button; INSTANCE:1]"						; open button instance
;$TXT_FileName_SelReplayFile=1152											; file name edit filed control ID
$TXT_FileName_SelReplayFile="[CLASS:Edit; INSTANCE:1]"											; file name edit filed control ID
$TXT_DELReplayFile="Do you want to remove selected replay file?"
$CHKB_Interactive_CANReplayConfig=1014										; interactive replay control ID
$LVC_CID_ReplayWin=400														; Replay window control ID
$RBTN_Cyclic_CANReplayConfig=1013											; Cyclic radio button control ID
$CHKB_RecTimeDelay_CANReplayConfig=1009										; Retain recorded time delay control ID
$TXT_MsgDelay_CANReplayConfig=1010											; Delay btw replay messages control ID
$TXT_CyclicDelay_CANReplayConfig=1011										; Cyclic delay control ID
$BTN_Filters_CANReplayConfig=1002											; Filter button control ID
$BTN_DellReplayFileYes_BM=6													; Control ID for 'Yes' button in Delete replay file window
$BTN_DellReplayFileNo_BM=7													; Control ID for 'No' button in Delete replay file window
;==============================================
;Data declaration for About BUSMASTER window
;==============================================

$DLG_About="About"
$DlgAboutBM="About"
$BTN_OK_About=1

;==============================================
;Network Statistics
;==============================================
$CANNWStatsMenu="NT"
$WIN_NW_Stats="Network Statistics"
$LVC_Details_NW_Stats="[CLASS:SysListView32; INSTANCE:1]"						; List View instance in Network Statistics window

;==============================================
;CAN Node Simulation
;==============================================
$CANNodeSimMenu="N"												; CAN->Node Simulation
$CANNodeSimConfigMenu=""												; CAN->Node Simulation->Configure
$TVC_NodeSim_ConfigCANNodeSim="[CLASS:SysTreeView32; INSTANCE:1]"				; Tree View instance in Configure Simulated Systems for CAN window
$TVC_Handlers_CPP="[CLASS:SysTreeView32; INSTANCE:1]"							; Tree View instance in Cpp file
$TVC_Handlers_CPPJ1939="[CLASS:SysTreeView32; INSTANCE:1]"                       ;Tree View instance in Cpp file in J1939
$WIN_AddCPPFile="Select BUSMASTER Source Filename..."							; Window title for add a new cpp file
$WIN_CPP_BUSMASTER="BUSMASTER - KeyHandlerNS_01.cpp"							; CPP file window
$WIN_KeyValue="Enter Key Value"													; Key handler window title
$BTN_OK_KeyValue=1																; Control ID for "OK" button in "Enter Key Value" window
;$TXT_FileName_AddCPPFile=1152													; file name edit feild control ID
$TXT_FileName_AddCPPFile="[CLASS:Edit; INSTANCE:1]"													; file name edit feild control ID
;$BTN_Open_AddCPPFile="[CLASS:Button; INSTANCE:2]"								; Instance "Open" button
$BTN_Open_AddCPPFile="[CLASS:Button; INSTANCE:1]"								; Instance "Open" button
$TXT_KeyHandler_CPPFile=59664													; Control ID for Key handler Edit box
$TXT_MsgHandler_CPPFile=59664													; Control ID for Msg handler Edit box
$BTN_AddCppFile_ConfigCANNodeSim=1001											; Control ID for "Add" button
$BTN_EditCppFile_ConfigCANNodeSim=1002											; Control ID for "Edit" button
$BTN_BuildLoad_ConfigNodeSim=1004											; Control ID for "Build and Load" button

$BTN_Unload_ConfigCANNodeSim=1005												; Control ID for "Unload" button
$BTN_EnableHandler_ConfigCANNodeSim=1014										; Control ID for "Enable handler" button
$LVC_HandlerDetails_ConfigCANNodeSim="[CLASS:SysListView32; INSTANCE:1]"		; Handler details list view instance
$BTN_OK_BuildLoadError=2														; Control ID for 'OK' button in "The file is either not properly built or Loaded" dialog
$Msg_BuildLoadError="The file is either not properly built or Loaded" 			; Error message text in "The file is either not properly built or Loaded" dialog
$MsgHandler_Index=0																; Message handler index in handler details list view
$TimeHandler_Index=1															; Time handler index in handler details list view
$KeyHandler_Index=2																; Key handler index in handler details list view
$ErrorHandler_Index=3															; Error handler index in handler details list view
$ToolBar_NodeSim=238															; Control ID for Node Simulation tool bar
$Icon_BuildLoadAll_Index=3														; Build and Load All index in Node Simulation tool bar
$Icon_UnloadAll_Index=2															; Unload All index in Node Simulation tool bar
$Icon_AllKeyHandlers_Index=8													; Enable/Disable All key Handlers index in Node Simulation tool bar
$Icon_TimeHandlers_Index=7														; Enable/Disable Time Handlers index in Node Simulation tool bar
$Icon_MsgHandlers_Index=6														; Enable/Disable Msg Handlers index in Node Simulation tool bar
$TXT_DelIncF="You are about to delete the #include."							; Text in the delete include file pop up message
$BTN_Yes_DelIncF=6																; Control ID for 'Yes' button in the delete include file pop up message
$WIN_EnterHeaderF="Enter Header File"											; Window title for Enter Header File
$TXT_DBFPath_EnterHeaderF=1049													; Control ID for File name edit field
$BTN_OK_EnterHeaderF=1042														; Control ID for OK button
$WIN_AddMsgHandler="Add Message Handler"										; window title for message handler
$TXT_MsgID_AddMsgHandler=1155													; Control ID for MsgID edit field
$BTN_OK_AddMsgHandler=1167														; Control ID for OK button
$TXT_MsgIDFrom_AddMsgHandler=1160												; Control ID for "MsgID From" edit field
$TXT_MsgIDTO_AddMsgHandler=1161													; Control ID for "MsgID To" edit field
$RBTN_MsgRange_AddMsgHandler=1172												; Control ID for "Msg range" radio button
$RBTN_MsgAll_AddMsgHandler=1173													; Control ID for "Msg All" radio button
$RBTN_MsgName_AddMsgHandler=1170												; Control ID for "Msg Name" radio button
$LBC_MsgList_AddMsgHandler=1169													; Control ID for "Database Message List" list box
$TXT_SaveSimFile_BUSMASTER="Do you want to save changes to the simulated system file(s)?"	; Text for Save simulated system window
$BTN_No_SaveSimFile=7															; Control ID for No button in Save simulated system window
$WIN_NewSimSys="New Simulated system Configuration Filename..."					; Window title for Enter Header File "New Simulated system Configuration Filename"
$TXT_SimFile_NewSimSys=1152														; Control ID for "File name" edit field in "New Simulated system Configuration Filename"
$BTN_Save_NewSimSys="[CLASS:Button; INSTANCE:2]"								; Control ID for Save button in "New Simulated system Configuration Filename"
$WIN_NodeDetails="Node Details"													; Window title for Node Details
$TXT_NodeName_NodeDetails=1278													; Control ID for "Node name" edit field in Node Details window
$BTN_OK_NodeDetails=1															; Control ID for "OK" button in Node Details windo
$WIN_TimeHandler="Define New Timer Handler"										; Window title for Time handler
$TXT_THname_TimeHandler=1044													; Control ID for "Time Handler Name" edit field in Time handler window
$TXT_TValue_TimeHandler=1045													; Control ID for "Timer Value" edit field in Time handler window
$BTN_OK_TimeHandler=1															; Control ID for OK button in Time handler window
$BTN_LoadDLLHandler=1164                                                        ; Control ID for Load Checkbox in DLL Handler
$BTN_UnLoadDLLHandler=1165                                                      ; Control ID for UnLoad Checkbox in DLL Handler
$WIN_BusEventhandler = "Add Bus Events Handler(s)"								; window title for Busevent handler
$BTN_Preconnect = "[CLASS:Button; INSTANCE:5]" 									; Radio button for Preconnect
$BTN_connect = "[CLASS:Button; INSTANCE:3]" 									; Radio button for Connect
$BTN_Disconnect = "[CLASS:Button; INSTANCE:4]" 									; Radio button for DisConnect
$BTN_OK_BusEvntHandler = "[CLASS:Button; INSTANCE:1]"
$WIN_LINNodesim = "Configure Simulated Systems - LIN Bus"						; Window title for LIN Node Simulation
$BTN_OK_DLLHandler = "[CLASS:Button; INSTANCE:1]"								; OK button in DLL handler window
$BTN_OK_UtilFun = "[CLASS:Button; INSTANCE:1]"									; OK button in Utils Funr window
;==============================================
;Signal Watch Window
;==============================================
$CANSigWatchMenu="S"												; CAN "Signal Watch" menu
$J1939SigWatchMenu="S"												; J1939 "Signal Watch" menu
$SigWatchConfigMenu="S"												; "Configure" menu
$SigWatchActivateMenu="&Activate"												; "Activate" menu
$WIN_SigWatchConfig="Signal Watch List"											; Window title for Signal Watch List
$WIN_SigWatch="Signal Watch - "													; Window title for Signal Watch
$COMBO_MsgList_SigWatch=1231													; Control ID for Message List combobox
$BTN_AddAll_SigWatch=32946														; Control ID for Add All button
$BTN_DelAll_SigWatch=32948														; Control ID for Delete All button
$BTN_Add_SigWatch=32950															; Control ID for Add button
$LVC_SigWatchList_SigWatch=1070													; Control ID for Signal Watch List view
$LVC_SigList_SigWatch=1071														; Control ID for Signal List view
$BTN_OK_SigWatch=1																; Control ID for 'OK' button
$BTN_Cancel_SigWatch=2															; Control ID for 'Cancel' button
$LVC_Value_SigWatch=1056														; Control ID for list view
$WIN_SignalWatch="Signal Watch - CAN"
$BTN_OK_ClearAllSignals=6                                                        ;Control id for 'yes' button in "Dissociation will clear Signal Watch List." warning message
;==============================================
;Output Window
;==============================================
$LSTB_Details_TraceWin="[CLASS:ListBox; INSTANCE:1]"							; List View instance of trace window

;==============================================
;Trace Window
;==============================================
$LSTB_Details_TraceWinInfo="[CLASS:ListBox; INSTANCE:2]"
;==============================================
;Test Automation Window
;==============================================

$WIN_TestAutomationEditorTitle="Test Automation Editor"
$TestAutomationMenu="TA"
$TestAutomationEditMenu="E"
$TestAutomationExeMenu="TE"
$TestAutomationFileMenu="&File"
$TestAutomationNewMenu="&New"
$TestAutomationVal="&Validate"
$DLG_ValSuccess="Validation Success"
$DLG_ValFail="Validation Failed"
$BTN_OK_ValSuccess=2																; Control ID for 'OK' button in Validation success dialog
$TVC_TestAuto_TestEditor="[CLASS:SysTreeView32; INSTANCE:1]"						; Tree viewe instance in test automation editor
$LVC_TestAuto_TestEditor="[CLASS:SysListView32; INSTANCE:1]"						; List View instance of test editor details
$TVC_TestSuite_TestExecutor="[CLASS:SysTreeView32; INSTANCE:1]"						; tree view instance in test suite executor window
$TestExe_AddMenu="a"																; Add menu in test executor window
$TestExe_ExecuteMenu="e"
$LVC_TestCaseResult_TestExe="[CLASS:SysListView32; INSTANCE:1]"						; List View instance of text executor window
$TEDBPathIndex=7																	; DB path index in the details list view
$TEReportPathIndex=7																; Report file path index in the details list view
$BTN_Confirm_TEditor=3004															; Confirm button Control ID
$WIN_TestExeOpen="Open"
;$TXT_XmlFPath_TestExeOpen=1152														; File path control ID in open window
$TXT_XmlFPath_TestExeOpen="[CLASS:Edit; INSTANCE:1]"														; File path control ID in open window
;$BTN_Open_TestExeOpen="[CLASS:Button; INSTANCE:2]"									; Open button instance
$BTN_Open_TestExeOpen="[CLASS:Button; INSTANCE:1]"									; Open button instance
$Win_TestSetup_TestEditor="New TestSetup File"
$Win_TestSetup_open="Select A TestSetup File"
;$Edit_FName_NewTestSetupFile=1152
$Edit_FName_NewTestSetupFile="[CLASS:Edit; INSTANCE:1]"
;$BTN_Save_NewTestSetupFile="[CLASS:Button; INSTANCE:2]"
$BTN_Save_NewTestSetupFile="[CLASS:Button; INSTANCE:1]"
$BTN_Save_loadTestSetupFile = "[CLASS:Button; INSTANCE:1]"
;==============================================
;Message Interpretation Window
;==============================================
$WIN_MsgInterpret="Message Interpretation"											; Message interpretation window title
$LVC_Signal_MsgIntWin=1229															; Control ID for Signal list view control
$TXT_MsgName_MsgIntWin=1019															; Control ID for "Message" edit list view control
$TXT_MsgID_MsgIntWin=1152															; Control ID for "ID" edit list view control
;==============================================
;Error
;==============================================
$DLG_Error="Error"
$BTN_OK_Error=2

;==============================================
;Toolbar
;==============================================
$Icon_EnableLog_Index=6
;$Icon_ConDiscon_Index=4
$Icon_ConDiscon_Index=1
$Icon_RplyStep_Index=14
$Icon_RplySkip_Index=15
$Icon_RplyStart_Index=16
$Icon_RplyStop_Index=17
$ToolBar_Con_Disconnect=128											; controlID for Connect toolbar
$ToolBar_MsgWindow=314												; controlID for Message Window toolbar
$Icon_ClearMsgWin_Index=0
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
$WIN_MsgDisplay="Configure Message Display - CAN"
$dlgMsgDisp="Configure Message Display- CAN"
$WIN_FilterSel="Filter Selection Dialog"
$dlgFilSel="Filter Selection Dialog"
$WIN_J1939MsgWind="BUSMASTER - [Message Window - J1939]"
$J1939MsgWind="BUSMASTER - [Message Window - J1939]"
$WIN_CANMsgWind="BUSMASTER - [Message Window - CAN]"
$WIN_Select_LogFile="Select a Log file"
$dlgSelLogFile="Select a Log file"
$WIN_DefaultCfxSave="Save Configuration File"
$DlgDefaultConfigSave="Save Configuration File"
$WIN_AssocJ1939DB="Select Active Database Filename..."
$WIN_J1939Transmit="J1939 Transmit Message Window"
$InterpretMsgMenu="&Interpret"
;==============================================
;J1939 Node Simulation
;==============================================


$J1939NodeSimMenu="N"                                                    ;J1939->Node Simulation
$J1939NodeSimConfigMenu="N"	                                                   ;J1939->NodeSimulation->Configure
$BTN_BuildLoad_ConfigJ1939NodeSim=1004                                                    ;Control Id for Build And Load Button
$J1939TXT_PrefAddress=4000                                                                ;Control Id for Preffered Address edit box in Node details
$J1939TXT_ECUName=4001
$TXT_MsgHandler_CPPFile=59664                                                            ;Control Id for Message handler edit box in Node details
$TXT_TimeHandler_CPPFile=59664                                                           ;Control Id for Timer Handler edit box
$LVC_HandlerDetails_ConfigJ1939NodeSim="[CLASS:SysListView32; INSTANCE:1]"
$J1939TXT_ECUName=4001
$WIN_EventHandler="Add Event Handler"
$TXT_EventHandler_CPPFile=59664
;==============================================
;Data declaration for Menu Text
;==============================================

$fileMenu="!F"
$configMenu=""
$loadMenu="L"
$newMenu="N"
$CANMenu="!C"
$saveConfig="S"
$databaseMenu="DB"
$AssocCANDB="A"
$DissocCANDB="D"
$openMenu="O"
$saveMenu="S"
$DBSaveAsMenu="V"
$closeMenu="X"
$closeMenuDBEditor = "C"
$saveImportMenu="P"
$configureMenu="C"
$configHWMenu="C"
$HardwareMenu="&Hardware"
$HWIMenu="&Hardware Interface"
$configTxmenu=""
$TxMsgMenu="TW"
;$AppFilterMenu="&Filter Configuration"
$AppFilterMenu="FT"
$MsgDisplay="&Message Display"
$FunctionsMenu="F&unctions"
$enableFilterMenu="E"
$displayMenu="&Display"
$EnableFilterDisp="F"
$EnableFilterLogMenu="F"
$transmitMenu="TW"
$EnableTransmitMenu="&Enable"	;NA
$normalBlckMenu="&Normal Blocks"
$CANMsgWinMenu="MW"
$J1939Menu="!J"
$J1939LogMenu="LG"
$J1939DBMenu="DB"
$J1939AssocDB="A"
$J1939DissocDB="D"
$J1939ViewMenu="&View"
$J1939MsgWinMenu="MW"
$J1939MsgWinAct="AT"
$J1939MsgWinDeact="AT"
$CANLogMenu="LG"
$LogMenu="LG"
$LogFilterMenu="F"
$HelpMenu="&Help"
$AboutBusmaster="I"
$FomatConverterMenu="FC"
$GoOnlineJ1939 = "AT"
$GoOfflineJ1939="AT"
$J1939TransmitMenu="TW"
$Exit="X"
$DriverSelectMenu="V"
$Tool = "!T"
$CANDBMenuTool = "CE"
$J1939DBMenuTool = "JE"
$connectCAN = "CN"
$overwritej1939 = "O"
$clearMsgwin = "E"
$messageWindow = "MW"
$overwriteMode = "O"
;----driver Selection mneu---

$canDriversel = "DS"
$kvaserHWMenu="K"
$ixxatHWMenu="X"
$nsiHWMenu="N"

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
$ToolBar_Configure=276
$OverwriteToolbarctrlID=314											; controlID for Overwrite toolbar
$Static_Version_About=1153											; controlID for Busmaster version in About dialog
$BMVersionStaticctrlID=1153											; controlID for Busmaster version in About dialog
;$TXT_FileName_AssoJ1939DB=1152                                      ;control ID for J1939 Associate DB path
$TXT_FileName_AssoJ1939DB="[CLASS:Edit; INSTANCE:1]"                                     ;control ID for J1939 Associate DB path
$CHKB_J1939Transport=1013                                           ; Control ID for J1939 Transport Protocol Check box in transmit window
$J1939PGNctrlID=1346                                                ; Control ID for PGN combo box in J1939 transmit window
$J1939MsgtypeCtrlID=1058                                            ; Control ID for message type edit control in J1939 Transmit Window
$J1939PriorityCtrlID=1348                                           ; Control ID for Priority edit control in J1939 Transmit Window
$J1939DLC_CtrlID=1125                                               ; Control ID for DLC edit control in J1939 transmit window
$J1939DataCtrlID=1016                                               ; Control ID for Databytes edit control in J1939 transmit window
$J1939FromAddCtrlID=1333                                            ; Control ID for From Address edit control in J1939 transmit window
$J1939ToAddCtrlID=1181                                              ; Control ID for To Address edit control in J1939 transmit window
$J1939NMCtrlID=1335                                                 ; Control ID for Network Management Checkbox in J1939 transmit window
$J1939NMAddCtrlID=1336                                              ; Control ID for Network Management Address Edit Control in J1939 transmit window
$CHKB_J1939ReqAdd=1008                                              ; Control ID for Request Address Edit Control in J1939 transmit window
$CHKB_J1939CommAdd=1009                                             ; Control ID for Command Address Edit Control in J1939 transmit window
$BTN_J1939Transmit=1011                                             ; Control ID for Transmit button in J1939 transmit window
$BTN_Yes_DeleteBlk=6                                                ; controlID for 'Yes' button in a warning pop up message for delete message Block in Configure Transmission messages
$BTN_AddCppFile_ConfigJ1939NodeSim=1001	                            ; control Id for Add New File button in Node Simulation
$WIN_RegEdit="Registry Editor"                                      ; Registry editor Window
$TVC_RegEdit=1                                                      ; Control id for Tree view in Registry Editor
$LST_CfxFiles_RegEdit=2



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
$LSTC_J1939MsgWin="[CLASS:SysListView32; INSTANCE:1]"               ; List view instance for J1939 Messsage window

$COMB_KeyType_ConfigTX_INST="[CLASS:ComboBox; INSTANCE:3]"          ; Key type instance for key trigger in Configure CAN messages
$BTN_YesConfirmSaveAsConfig="[CLASS:Button; INSTANCE:1]"            ;Instance for Yes Button to confirm save As dialog that pops up

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



;===============================================================
;Data Declaration for Format Converters
;===============================================================

;Data Declaration for CAPL to CPP conversion

$WIN_FormatConv="BUSMASTER Format Conversions"
$Tab_CAPL2CPP="[CLASS:SysTabControl32; INSTANCE:1]"
$BTN_CaplInput=1011
$WIN_SelectCAN_CAPL2CPP="Select CAPL (.can) File"
$WIN_SelectCAPL_Wait=2
$WIN_SelectCPP_Wait=2
$WIN_SelectCpp_CAPL2CPP="Select BUSMASTER File"
$BTN_CPPOut_CAPL2CPP=1012
;$SelectDBC_CAPL2CPP="Select CANoe Database File"
$SelectDBC_CAPL2CPP="Select"
$WIN_SelctDBCWait_CAPL2CPP=2
$SelectDBCFileCtrlId=2
$Capl2CppLogFile="CAPL2CPPlog.txt - Notepad"
$BTN_Add_DBC_CAPL2CPP=1004
;$BTN_Add_DBC_CAPL2CPP=1009
$BTN_Change_DBF_Path=1005
$BTN_Convert_CAPL2CPP=1013
$TXT_RESULT=1015
$BTN_Report=1008
$LST_DBCFiles=1046
$CppFilePath="\Output\can.cpp"
$CHKB_SaveDB_CAPL2CPP=1018
$ChangeDBCCAPL2CPP="Select BUSMASTER Database File"
;$TXT_Open_File_CAPL2CPP="[CLASS:Edit; INSTANCE:1]"
$TXT_Open_File_CAPL2CPP="[CLASS:Edit; INSTANCE:1]"
;$BTN_Open_File_CAPL2CPP="[CLASS:Button; INSTANCE:2]"
$BTN_Open_File_CAPL2CPP="[CLASS:Button; INSTANCE:1]"
$TXT_ConversionRes=1016
$BTN_InputLogFile=1010
$WIN_SelectLogFile="Select log file"
$WIN_SelectCSVFile="Select export file"
$TXT_Open_File_Log2Excel="[CLASS:Edit; INSTANCE:1]"
;$BTN_Open_FilePath_LogToExcel="[CLASS:Button; INSTANCE:2]"
$BTN_Open_FilePath_LogToExcel="[CLASS:Button; INSTANCE:1]"
$BTN_OutputCSVFile=1011
;$BTN_SaveCSVFile="[CLASS:Button; INSTANCE:2]"
$BTN_SaveCSVFile="[CLASS:Button; INSTANCE:1]"
$LSTB_AvailableFields=1004
$BTN_SelectAll=1006
$BTN_Export="[CLASS:Button; INSTANCE:3]"
$BTN_OK_LogToExcel=2
$COMBO_SelectConvTypeLogToExcel=1015
$COMBO_SelectConvTypeLogToExcel1="[CLASS:ComboBox; INSTANCE:1]"
$WIN_Warning="Warning"
$BTN_NO_Continue=7
$WIN_ConfirmSaveAs="Confirm Save As"
$BTN_Yes_SaveAs="[CLASS:Button; INSTANCE:1]"
$BTN_Delete_DBC_CAPL2CPP=1009

$COMBO_SelectConvType=1003
$TXT_SelectConvType="DBC to DBF Conversion"
$BTN_Input=1006
$Dlg_OthrConv_Open_Wait=3
$BTN_Output=1007
$BTN_Convert_OtherConvts=1008
$BTN_ViewLog=1013
$TAB_OtherConvts="[CLASS:SysTabControl32; INSTANCE:1]"
$Dlg_OpenOthrConv="Open"
$TXT_Open_FilePath_OthrConv="[CLASS:Edit; INSTANCE:1]"
;$BTN_Open_FilePath_OthrConv="[CLASS:Button; INSTANCE:2]"
$BTN_Open_FilePath_OthrConv="[CLASS:Button; INSTANCE:1]"
$BTN_OK_EnterLogFileDetails=2
$CHBK_SaveConvDB=1045
$CHBK_SaveConvDB=1045
$CHBK_SaveConvDB=1045
$CHBK_SaveConvDB=1045
$BTN_Yes_Continue=6
$Win_Keymapping="Key Mapping"
$BTN_AddKey=1004
$BTN_Close_KeyMap=1005
$BTN_OK_Warning=2

$BTN_SelectOne=1007                                                                      ;Select one field
$LSTB_SelectedFields=1005                                                                ;Control id for selected fields listbox
$BTN_DeleteOneField=1008                                                                 ;Control id for deleting one field
$BTN_DeleteAllField=1009                                                                 ;Control id for deleting all the field
$BTNExcSaveOutput="[CLASS:Edit; INSTANCE:1]"




;;------------------Added By Srinibas Das -----------------------------------------------------------------

;=================================================================================================
 ;Data Declaration for LIN
;==================================================================================================
$linMenu = "!L"  																		;LIN menu item
$WIN_SelectLDFFile ="Select A LDF File"													;Window for selecting LDF file
$Lindriver = "V"																		;menu item for Vector XL
$LindriverSelectMenu ="DS"																;Menu For Driverselection
$LinHwSelect = "[CLASS:SysListView32; INSTANCE:1]"										;Selected Hardware List View instance
$WinHwSelection= "Hardware Selection"													;Window for Hardware Selection
$BTN_Add_HwSelect = "[CLASS:Button; INSTANCE:1]"										;Instance for "Select" button in hardware selection
$BTN_OK_HwSelect = "[CLASS:Button; INSTANCE:3]"											;Instance for "OK" button in hardware selection
$LinEcuListView = "[CLASS:SysListView32; INSTANCE:1]" 									;ECU List View instance
$LinEnableMasterMode = 1015																;control id for check box of enable master mode
$LVC_CID_LINMsgWin=200																	;control id for LIN message window
$TxMsgMenuLin = "TW"																	;tx menu for LIN
$configTxmenuLin = "DS"																	;config menu for Lin
$enableMaster_ConfigTXLIN = 1213														;controlID for 'master' radio button in LIN TX window
$enableSlave_ConfigTXLIN = 1214															;controlID for 'salve' radio button in LIN TX window
$BTN_AddMsg_ConfigTXLIN = 1250															;controlID for 'Add Message' button in LIN Tx window
$ToolBar_Con_Disconnect_LIN=334															;controlID for Connect toolbar for LIN
$LinTxFrameList = "[CLASS:Button; INSTANCE:1]"
$LSTC_LINMsgWin= "[CLASS:SysListView32; INSTANCE:1]"									;List View instance in Message window
$WIN_LINMsgWind="BUSMASTER - [Message Window - LIN]"
$WIN_MsgDisplay_LIN = "Configure Message Display - LIN"
$LINMsgWinMenu = "MW"
$configMsgWindowLIN = "C"
$LinconfgMsgDisp = "[CLASS:SysListView32; INSTANCE:1]"									;Instance for list view of config message Display.
$WIN_Config_filter_LIN = "Configure Filter List - LIN"
;$BTN_Close_ConfigFilter =																; controlID for 'Close' button  in Configure filter window -LIN
$FilterListLIN = 1000																	; controlID for Filter List in Configure filter window -LIN
$LINSigWatchMenu = "SW"																	; LIN "Signal Watch" menu
$WinSigwatchConfigLIN = "SW"															; Window for Signal Watch Configure - LIN
$LSTC_LINSigWatConfigWin = "[CLASS:SysTreeView32; INSTANCE:1]"							; List View instance in signal watch configure -Window for LIN
$saveAsMenu = "A"
$WIN_SaveAsCfx="SaveAs Configuration Filename..."										; SaveAs configuration dialog text
$BTN_SaveAsInst_Cfx = "[CLASS:Button; INSTANCE:1]"
$LSTC_LINSigWatchgWin = "[CLASS:SysListView32; INSTANCE:1]"
$WIN_SigWatchLIN = "Signal Watch - LIN"
$EnableMsgWindowLIN = "F"
$WIN_LINLog="Configure Logging for LIN"
$LINLogMenu="LG"
$WIN_LINMsgWind="BUSMASTER - [Message Window - LIN]"
$LINDriverSelMenu="V"																	; Driver selection menu
$View = "&View"
$Toolbar = "T"
$ClusterConfigMenu="CC"                                           						; Cluster Configuration menu item
$WIN_ClusterConfiguration="Cluster Configuration"
$BTN_OK_ClusterConfig=1                                                               	; Control id for OK Button
$view = "V"																				; View
$dec = "DH"
$hex = "DH"
$connectLIN = "CN"																		; Connect LIN
$activateLinMsgWin = "A"																; LIN - Message window Activate Ribbon Bar
$winLinMsgWin = "Message Window - LIN"
;=================================================================================================
 ;Node Simulation
;==================================================================================================
;$BTN_Brows_NodeDetails = 1001
$BTN_Brows_NodeDetails = "[CLASS:Button; INSTANCE:1]"
$WinselectFile= "Select the File"
$WIN_TestCPP_BUSMASTER="BUSMASTER - TestNode_cppFile.cpp"								; CPP file window
$NodeSimMenu = "NS"																		; Node Simulation
$NodeSimConfigMenu = ""																	; Config menu for node sim
$COMB_SlotID = 4028																		; Combo Box ID of Slot ID
$COMB_BaseCycle= 4016																	; Combo Box ID of Base Cycle
$COMB_Repetition = 4026																	; Combo Box ID of Repetition

$RBTN_Frame_AddMsgHandler = 1171														; Radio Button ID for Frame
$RBTN_Framename_AddMsgHandler =	1170													; Radio Button ID for Frame Name
$RBTN_AllRxMsg_AddMsgHandler = 1173														; Radio Button ID for AllRx Frame(s)
$LVC_CID_FrameNameMsgHW = 1169															;
$BTN_Cancel_NodeDetails = 2																; controlid for cancel buttin in node details window
$WIN_DLLHandler = "Add DLL Handler(s)"													; Win Title of DLL handler
$WIN_UtilityFun = "Add Utility Function"												; Win Title for Utility Fun
$TXT_UtilityFun = "[CLASS:Edit; INSTANCE:2]"
$COMB_UtilFun_RetValue = 1029																; Control ID for return value combo box in Utility fun dialog box
$WIN_ERRHandler = "Add Error Handler(s)" 												; Window Title of Error handler in Node Simulation
$BTN_ErrChksmERRHandler = 4015															; Control ID for Error CheckSum check box in dialog box.
$BTN_ErrRcvfrmERRHandler = 4016															; Control ID for Error Rcv Frame check box in dialog box.
$BTN_ErrSlvNoRspnsERRHandler = 4017														; Control ID for Error Slave No Response check box in dialog box.
$BTN_ErrErrSynchERRHandler = 4018														; Control ID for Error Synch check box in dialog box.
$BTN_OK_ERRHandler = "[CLASS:Button; INSTANCE:1]"										; Button Instance for "OK" button in error handler dialog box.
$WIN_ClustrConfigLIN = "Cluster Configuration"											; Window Title of Cluster Config in LIN
$LDFFilePath = "[CLASS:Edit; INSTANCE:1]"												; Instance of LDF file path in LIN cluster Configuration
$ChannelSelInCCLIN="[CLASS:ComboBox; INSTANCE:1]"										; Instance of Channel Selection in LIN cluster Configuration
$chckBoxEnableMaster = "[CLASS:Button; INSTANCE:8]"										; Check Box instance for enable master in Lin cluster configuration
$toolBarLin="[CLASS:ToolbarWindow32; INSTANCE:3]"										; Tool Bar instance for LIN
$EnableFilterLin = "F"
;=================================================================================================
 ;CAN Tx Windown
;==================================================================================================
$WinCANTxWindow = "Configure Transmission Messages - CAN"								; Window Title for CAN -Tx Window
$WindTxWindow = "Configure Transmission Messages - "									; Window Title for CAN -Tx Window
$WinCANTxWindowMax = "BUSMASTER - [Configure Transmission Messages - CAN]"				; Window Title for CAN -Tx Window When it maximized
$LVC_TxWin = "[CLASS:SysListView32; INSTANCE:1]"										; List View of Can-Tx Window -Tx Frame List
$CandriverSelectMenu ="Dri&ver Selection"												; Menu For Driverselection
$BTN_SendTxWin= "[CLASS:Button; INSTANCE:2]"											; Send Button Instance In TxWindow
$BTN_Del_TxWindow="[CLASS:Button; INSTANCE:5]"											; Delete BTN Instance in Tx Window
$BTN_DelALL_TxWindow="[CLASS:Button; INSTANCE:4]"										; Delete ALL BTN Instance in Tx Window
$TXT_DelAllMsgsTxWin="Are you sure you want to delete all messages?"					; Delete All message text
$BTN_Yes_DelALlDialog=6																	; Control ID for Yes Button Delete ALL dialog Box
$BTN_No_DelALlDialog=7																	; Control ID for NO Button Delete ALL dialog Box
$TXT_DeleALL_DialogBox="[CLASS:Static; INSTANCE:2]"										; Instance if TExt in Dialog box for Delete ALL
$LVC_TxWin_SigDtls = "[CLASS:SysListView32; INSTANCE:2]"								; List View of Can-Tx Window -Signall View
$LVC_TxWin_dtaView = "[CLASS:SysListView32; INSTANCE:3]"								; List View of Can-Tx Window -Signall View
;=================================================================================================
 ;Advanced log File-CAN--- Added by Shilpa
;==================================================================================================
$CHKB_OnTransmission_ConfigLOG=2027               																;controlID for the checkbox to create log for every bus connect
$CHKB_LogfileSize_ConfigLOG=2028																				;controlID for the checkbox to create log after each file size exceeding the limit
$CHKB_LogOnTime_ConfigLOG=2031																					;controlID for the checkbox to create log in certain time limit
$EDIT_NumberOfLogfiles_ConfigLOG=2037																			;controlID for the editbox for the number of logfiles to be created
$EDIT_Filesize_ConfigLOG=2029																					;controlID for the editbox for the size of the log file
$EDIT_Hours_ConfigLOG=2032																						;controlID for the editbox for hours
$EDIT_Minutes_ConfigLOG=2034																					;controlID for the editbox for minutes
$EDIT_InsertComments_ConfigLOG=2038																			    ;controlID for the editbox for InsertComments
;=================================================================================================
 ;Busmaster - Menu index
;==================================================================================================
$index_Trace = 5																		; Index of Trace menu
$index_View_TraceWin = 0																; Index of View->Trace menu
$index_View = 5																			; Index of View menu
$index_View_Toolbar = 3																	; Index of View -> Tollbar menu
$index_View_Toolbar_Lin = 5																; Index of View -> Tollbar->Lin  menu
$index_View_Hex = 1																		; Index of View->Hex menu
$index_Lin = 4																			; Index of Lin
$index_Lin_MsgWin = 8																	; Index of Lin->msgWin
$index_Lin_MsgWin_TimeMode = 5															; Index of Lin->msgWin->TimeMode
$index_Lin_MsgWin_TimeMode_Rel = 2														; Index of Lin->msgWin->TimeMode->Relative
;=================================================================================================
 ;Advanced log File-CAN--- Added by Meghna
;==================================================================================================
$PEAKUSBMenu= "P"
$ISOLAREVE= "L"
$MessageWindowMenu= "M"
$ActiveMenu= "A"
$WindowMenu= "&Window"
$1MessageWindowCANMenu= "1 Message Window-CAN"
$OverwriteMenu= "O"
$InterpretMenu= "I"
$TimeModeMenu= "M"
$SystemTimeMenu= "S"
$ETASBOAMenu= "B"
$ToolBar_Con_DisconnectLIN=334