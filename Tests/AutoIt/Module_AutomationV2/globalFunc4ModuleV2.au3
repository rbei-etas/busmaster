#include <GuiStatusBar.au3>
#include <Excel.au3>
#include <GuiComboBoxEx.au3>
#include <GuiListView.au3>
#include <GuiMenu.au3>
#include<GuiImageList.au3>


Global $app, $rVal,$LSTC_FilterList_ConfigFilter,$BTN_DEL_Filter,$oIE,$FilePath,$FilterItem
local $sHTML
Local $TCIDIndex="B", $TCResIndex=3
Global $driverSelXml[20],$channelNo[10]
;$BusMasterExeFPath=@ProgramFilesDir&"\BUSMASTER_v2.5.0"

;$versionBusMaster  = To get the busmaster software version no from text file
;=============================================//Added By Srinibas Das =============================================================
;Function Name : _GetSoftVersion()
;Functionality : This function returns the software version no
;Input 		   : -
;Output		   : software version
;==========================================================================================================

Func _GetSoftVersion()

ConsoleWrite("_GetSoftVersion")
$CurrentDirPath = _SetOneFolderUp()
ConsoleWrite("$CurrentDirPath"&$CurrentDirPath&@CRLF)
Local $hFileOpen = FileOpen($CurrentDirPath&"\BusmasterVersion.txt", 0)
Local $sFileRead = FileRead($hFileOpen)
ConsoleWrite("$sFileRead="&$sFileRead&@CRLF)
Local $versionBusMaster = StringTrimLeft($sFileRead, 18)
ConsoleWrite("$versionBusMaster="&$versionBusMaster&@CRLF)
FileClose($hFileOpen)

Return $versionBusMaster

EndFunc

$BusMasterExeFPathtemp=@ProgramFilesDir&"\"& _GetSoftVersion()
Local $BusMasterExeFPath = StringStripWS($BusMasterExeFPathtemp, 2)

;$BusMasterExeFPath=@ProgramFilesDir&"\"& _GetSoftVersion()
ConsoleWrite("$BusMasterExeFPath="&$BusMasterExeFPath&@CRLF)

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
	$app = WinExists($WIN_BUSMASTER) 														; Check if the BUSMASTER window exists
	ConsoleWrite("app exists res: " & $app & @CRLF)

	If $app = 0 Then 																		; If not found then run the app
;~ 		$rVal = Run ("C:\ProgramFiles(x86)\BUSMASTER_v2.0.0\BUSMASTER.exe")
		$rVal = Run ($BusMasterExeFPath & "\BUSMASTER.exe")
		ConsoleWrite("App search result: " & $rVal & @CRLF)
;~ 		ProcessWait($AppProcess)															; Pauses script execution until a given process exists.
		WinWaitActive($WIN_BUSMASTER,"",5)
		If WinWaitActive($WIN_BUSMASTER, "", 1) Then
			ControlClick($WIN_BUSMASTER, "OK", "[CLASS:Button; INSTANCE:1]")
			Sleep(2000)
		EndIf
		WinSetState($WIN_BUSMASTER, "", @SW_MAXIMIZE)
	EndIf
	WinActivate($WIN_BUSMASTER)
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
;Function Name : _CloseApp
;Functionality : This function will closes the application.
;Input 		   : -
;Output		   : If the process is killed then it returns the $isAppKilled=1 to calling function
;==========================================================================================================
Func _CloseApp()
	$isAppKilled=0
	WinMenuSelectItem($WIN_BUSMASTER,"",$fileMenu,$Exit)								; Select File->Exit from menu
	sleep(2000)
	if WinWaitActive("",$SaveChangesConfigTXT,2) Then									; wait for save configuration dialog
		ControlClick("",$SaveChangesConfigTXT,$BTN_No_SaveConfig)						; Click on No button
	EndIf
	_HandleSaveSimSysWin()																; Check for Save Simulated System window
	if WinWaitActive($WIN_BUSMASTER,$saveDBtxt,2) Then
		ControlClick($WIN_BUSMASTER,"",$BTN_No_SaveDB)
	EndIf
	if WinWaitActive($WIN_BUSMASTER,$SaveChangesConfigTXT,2) Then						; wait for save configuration dialog
		ControlClick($WIN_BUSMASTER,$SaveChangesConfigTXT,$BTN_No_SaveConfig)			; Click on No button
;~ 		$OutPath = _OutputDataPath()
;~ 		WinWaitActive($WIN_DefaultCfxSave,"",2)
;~ 		if WinExists($WIN_DefaultCfxSave,"") Then
;~ 			ControlSend($WIN_DefaultCfxSave,"",$TXT_FilePath_SaveConfigFile,$OutPath&"\defaultFile")
;~ 			ControlClick($WIN_DefaultCfxSave,"",$BTN_Save_SaveConfigFile)
;~ 		EndIf
		ConsoleWrite("Application Closed through menu"&@CRLF)
;~ 		sleep(2000)
	EndIf
	if ProcessExists ($AppProcess) Then													; if the application still exists then kill the process
		ProcessClose($AppProcess)
		ConsoleWrite("Application Killed"&@CRLF)
		$isAppKilled=1																	; if app is forcefully killed then set $isAppKilled value to 1
	EndIf
	Return $isAppKilled
EndFunc

;==========================================================================================================
;Function Name : _WriteExHWDRes
;Functionality : This function will write thhe application terminated result.
;Input 		   : Column and row no.
;Output		   : -
;==========================================================================================================

Func _WriteExHWDRes($Column,$Row)
	_ExcelWriteCell($oExcel, "App Terminated",$Column,$Row+1)
	_ExcelWriteCell($oExcel, "Fail",$Column,$Row)
EndFunc

;==========================================================================================================
;Function Name : _CreateHTMLTabHeader
;Functionality : This function will create the HTML table header to save the result.
;Input 		   : Module Name
;Output		   : -
;==========================================================================================================
Func _StartoHTMLTable($ModName)
	$sHTML &= "<head><left>"&$ModName&"</left></head>" & @CR & @CR
	$sHTML &= "<HTML>" & @CR
	$sHTML &= "<table border=1 Align=centre>" & @CR
	$sHTML &= "<tr>" & @CR
	$sHTML &= "<th>TestCase No.</th>" & @CR
	$sHTML &= "<th width=30>Result</th>" & @CR
	$sHTML &= " </tr>" & @CR
EndFunc

;==========================================================================================================
;Function Name : _CreateHTMLTabFooter
;Functionality : This function will add the end tag of HTML table.
;Input 		   : -
;Output		   : -
;==========================================================================================================
Func _EndoHTMLTable()
	$sHTML &= "</table>" & @CR
	$sHTML &= "</HTML>" & @CR
	$sHTML &="<br>"
EndFunc


;==========================================================================================================
;Function Name : _WriteResult
;Functionality : This function will write the result to HTML and Excel(If exists).
;Input 		   : TestCase Status, TestCase No.
;Output		   : -
;==========================================================================================================

Func _WriteResult($TCStatus,$TCNo)
	; Assign the color code passed on the testcase status
	if $TCStatus="Pass" Then
		$ColorCode="Green"
	Elseif $TCStatus="Fail" Then
		$ColorCode="Red"
	Else
		$TCStatus="Warning"
		$ColorCode="Orange"
	EndIf
	; Write the test result to HTML table
	$sHTML &= " <tr>" & @CR
	$sHTML &= "   <td>"&$TCNo&"</td>" & @CR
	$sHTML &= "   <td BGCOLOR="&$ColorCode&">"&$TCStatus&"</td>" & @CR
	$sHTML &= " </tr>" & @CR
	_IEAction($oIE, "refresh")

	_IEAction($oIE, "visible")									; Sets an object state to visible
	_IEDocWriteHTML($oIE, $sHTML)								; Replaces the HTML for the entire document.

	; Write the test result to Excel Sheet
	if FileExists($FilePath) Then
		$oExcel.Application.ActiveSheet.Columns($TCIDIndex).Select						; Select the Testcase ID column
		$TCAddress=$oExcel.Application.Selection.Find($TCNo).Address					; Fetch the address of the Testcase ID
		ConsoleWrite("$TCAddress:"&$TCAddress&@CRLF)									; Addess will in $Column$Row format (Ex : $B$14)
		$TCRow=StringTrimLeft($TCAddress,3)												; Remove the first 3 characters from left hand side (Ex : $B$)
		ConsoleWrite("$TCRow="&$TCRow)
		_ExcelWriteCell($oExcel, $TCStatus, $TCRow, $TCResIndex)						; Write the result
	EndIf
	_ExcelBookSave($oExcel)
EndFunc

;==========================================================================================================
;Function Name : _SaveHTML
;Functionality : This function will save the HTML test report
;Input 		   : -
;Output		   : -
;==========================================================================================================

Func _SaveHTMLReport()
	$HTMLFName=_GetDateTime()
	$htmlFilePath=@ScriptDir &"\Result"&$HTMLFName
	WinActivate("Blank Page - Windows Internet Explorer")
	$hIE = _IEPropertyGet($oIE, "hwnd")
	WinActivate($hIE)
	WinWaitActive($hIE,"",5)
	ControlSend($hIE, "", "", "!f") 								; File
	ControlSend($hIE, "", "", "a") 									; SaveAs
	WinWait("Save Web Page", "", 5)
	$hSave = WinGetHandle("Save Web Page", "")
	WinActivate($hSave)
	ControlSend($hSave,"",1001,$htmlFilePath)
	ControlCommand($hSave,"","ComboBox2","SetCurrentSelection",2)
	ControlClick("Confirm Save As","","[CLASS:Button; INSTANCE:1]")
	ControlClick($hSave,"",1)
EndFunc

;==========================================================================================================
;Function Name 		: _GetDateTime()
;Functionality 		: This function fetches the date,time and Replaces strings "/" and ":" with "_"
;Input 		   		: -
;Output		   		: Returns the system date and time in DD_MM_YYYY_HH_MM_SS format
;==========================================================================================================
Func _GetDateTime()
	$Now=_Now()													; Fetch the date and time
	$RemAMPM=StringTrimRight ($Now,3)							; Remove the last three characters(AM,PM," ")
	$RepBSlash=StringReplace($RemAMPM,"/","_")					; Replace "/" with "_"
	$RepColon=StringReplace($RepBSlash,":","_")					; Replace ":" with "_"
	$DateTime=StringReplace($RepColon," ","_")					; Replace " " with "_"
	ConsoleWrite("$DateTime:"&$DateTime&@CRLF)
	Return $DateTime											; Return the data to calling function
EndFunc
;==========================================================================================================
;Function Name 		: _saveConfigMenu
;Functionality 		: This function saves the configuration
;Input 		   		: -
;Output		   		: -
;==========================================================================================================
Func _saveConfig()
	WinMenuSelectItem($WIN_BUSMASTER,"",$fileMenu,$saveMenu)					; Select File->Save
EndFunc

;==========================================================================================================
;Function Name : _RegistryCleanup
;Functionality : This function selects BUSMASTER Registry Cleanup.exe to clean up the registry
;Input 		   : -
;Output		   : -
;==========================================================================================================
Func _RegistryCleanup()
	ConsoleWrite("$BusMasterExeFPath="&$BusMasterExeFPath&@CRLF)
	Run($BusMasterExeFPath&"\BUSMASTER_Cleanup_Registry.exe")
	WinWaitActive($WIN_RegClean,"",5)
	if winexists($WIN_RegClean) Then
		ControlClick($WIN_RegClean,"",$BTN_Close_RegClean)
		WinClose($WIN_RegClean)
	EndIf
EndFunc

;==========================================================================================================
;Function Name 		: _createConfig
;Functionality 		: This function creates cfx file
;Input 		   		: Config File Name
;Output		   		: -
;==========================================================================================================

Func _createConfig($cfxFName)
	$sMenu=WinMenuSelectItem($WIN_BUSMASTER,"",$fileMenu,$newMenu)					; Select File->Configuration->New
	WinWaitActive($WIN_BUSMASTER,$saveConfigtxt,3)
	if WinExists($WIN_BUSMASTER,$saveConfigtxt) Then								; wait for save configuration dialog
		ControlClick($WIN_BUSMASTER,"",$BTN_No_SaveConfig)							; Click on No button
	EndIf
	sleep(1000)
	$DirPath =_OutputDataPath()													; Set the DirPath to save the dbf file
	WinWaitActive($WIN_DefaultCfxSave,"",3)
	if WinExists($WIN_DefaultCfxSave,"") Then
		ControlSend($WIN_DefaultCfxSave,"",$TXT_FilePath_SaveConfigFile,$DirPath&"\defaultFile")
;~ 		If WinExists($WIN_ConfirmSaveAs) Then
;~ 			ControlClick($WIN_ConfirmSaveAs,"",$BTN_YesConfirmSaveAsConfig)
;~ 		EndIf
		ControlClick($WIN_DefaultCfxSave,"",$BTN_Save_SaveConfigFile)
	EndIf
	_HandleSaveSimSysWin()															; Check for Save Simulated System window
	WinWaitActive($WIN_NewCfx,"",5)
	ConsoleWrite("$DirPath----------"&$DirPath&@CRLF)
	if winexists($WIN_NewCfx) Then
		ControlSend($WIN_NewCfx,"",$TXT_FileName_NewCfx,$DirPath&"\"&$cfxFName)		; Set the filename in 'New configuration filename' dialog
		sleep(500)
		$cConfig=ControlClick($WIN_NewCfx,"",$BTN_SaveInst_NewCfx)					; Click on Save button
		sleep(2500)
	EndIf
	_HandleSaveSimSysWin()															; Check for Save Simulated System window
EndFunc

;==========================================================================================================
;Function Name 		: _createConfig
;Functionality 		: This function creates cfx file in Test Data folder
;Input 		   		: Config File Name
;Output		   		: -
;==========================================================================================================

Func _createConfigTest($cfxFName)
	$sMenu=WinMenuSelectItem($WIN_BUSMASTER,"",$fileMenu,$newMenu)					; Select File->Configuration->New
	WinWaitActive($WIN_BUSMASTER,$saveConfigtxt,3)
	if WinExists($WIN_BUSMASTER,$saveConfigtxt) Then								; wait for save configuration dialog
		ControlClick($WIN_BUSMASTER,"",$BTN_No_SaveConfig)							; Click on No button
	EndIf
	sleep(1000)
	$DirPath =_TestDataPath()												; Set the DirPath to save the dbf file
	WinWaitActive($WIN_DefaultCfxSave,"",3)
	if WinExists($WIN_DefaultCfxSave,"") Then
		ControlSend($WIN_DefaultCfxSave,"",$TXT_FilePath_SaveConfigFile,$DirPath&"\defaultFile")
		ControlClick($WIN_DefaultCfxSave,"",$BTN_Save_SaveConfigFile)
	EndIf

	_HandleSaveSimSysWin()															; Check for Save Simulated System window
	WinWaitActive($WIN_NewCfx,"",5)
	if winexists($WIN_NewCfx) Then
		ControlSend($WIN_NewCfx,"",$TXT_FileName_NewCfx,$DirPath&"\"&$cfxFName)		; Set the filename in 'New configuration filename' dialog
		sleep(500)
		$cConfig=ControlClick($WIN_NewCfx,"",$BTN_SaveInst_NewCfx)					; Click on Save button
		Sleep(500)
		If WinExists($WIN_ConfirmSaveAs) Then
			ControlClick($WIN_ConfirmSaveAs,"",$BTN_YesConfirmSaveAsConfig)
			sleep(1000)
		EndIf
		sleep(2500)
	EndIf
	_HandleSaveSimSysWin()															; Check for Save Simulated System window
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
		ControlClick($WIN_BUSMASTER,"",$BTN_No_SaveConfig)							; Click on No button
	EndIf
	sleep(1000)
	$DirPath = _TestDataPath()														; Set the DirPath to save the dbf file
	WinWaitActive($WIN_LoadCfx,"",5)
	ConsoleWrite("$cfxFName " &$cfxFName & @CRLF)
	if winexists($WIN_LoadCfx) Then
		$FileExt=StringInStr($cfxFName,".cfx")
		if $FileExt=0 Then
			$cfxFName=$cfxFName&".cfx"
		EndIf
		ControlSetText($WIN_LoadCfx,"",$TXT_FileName_LoadCfx,$DirPath&"\"&$cfxFName)	; Set the filename in 'Load Configuration Filename...' dialog
		sleep(500)
		$cConfig=ControlClick($WIN_LoadCfx,"",$BTN_LoadInst_LoadCfx)				; Click on Load button
		sleep(2500)
	EndIf
	_HandleSaveSimSysWin()															; Check for Save Simulated System window
EndFunc

;==========================================================================================================
;Function Name 		: _createCANDB
;Functionality 		: This function creates a database file
;Input 		   		: DBF File Name
;Output		   		: -
;==========================================================================================================

Func _createCANDB($dbFName)
	$dbMenu=WinMenuSelectItem($WIN_BUSMASTER,"",$CANMenu,$databaseMenu,$newMenu)
	WinWaitActive($WIN_NewDBFile,"",3)
	$DBFolderPath = _OutputDataPath()													; Set the DirPath to save the dbf file

	if winexists($WIN_NewDBFile) Then
		ControlSend($WIN_NewDBFile,"",$TXT_FileName_NewDB,$DBFolderPath&"\"&$dbFName)   ; Set Filename
		sleep(1000)
		$cDB=ControlClick($WIN_NewDBFile,"",$BTN_SaveInst_NewDB,"left") 				; Click on Save button
		sleep(1000)
	EndIf

EndFunc

;==========================================================================================================
;Function Name 		: _createJ1939DB
;Functionality 		: This function creates a J1939 database file
;Input 		   		: DBF File Name
;Output		   		: -
;==========================================================================================================

Func _createJ1939DB($dbFName)
	$dbMenu=WinMenuSelectItem($WIN_BUSMASTER,"",$J1939Menu,$databaseMenu,$newMenu)
	sleep(1000)
	$DBFolderPath = _OutputDataPath()													; Set the DirPath to save the dbf file
	if winexists($WIN_SaveAs) Then
		ControlSend($WIN_SaveAs,"",$TXT_FileName_NewDB,$DBFolderPath&"\"&$dbFName)   ; Set Filename
		sleep(1000)
		$cDB=ControlClick($WIN_SaveAs,"",$BTN_SaveInst_NewDB,"left") 				; Click on Save button
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
	if winexists($WIN_BUSMASTER) Then
		send("{ENTER}")																			; if warning message appears then select yes
	EndIf
	if WinWaitActive($WIN_BUSMASTER,$saveDBtxt,2) Then
		ControlClick($WIN_BUSMASTER,"",$BTN_No_SaveDB)
	EndIf
	$DBFolderPath = _TestDataPath()																; Set the DirPath to save the dbf file
	if WinWaitActive($WIN_SelCAN_DBFile,"",5) Then
		ControlSend($WIN_SelCAN_DBFile,"",$TXT_FileName_OpenDB,$DBFolderPath&"\"&$dbFName)  	; Set Filename
		sleep(1000)
		$cDB=ControlClick($WIN_SelCAN_DBFile,"",$BTN_OpenInst_OpenDB) 							; Click on Save button
		sleep(2500)
	EndIf
EndFunc

;==========================================================================================================
;Function Name 		: _openJ1939DB
;Functionality 		: This function opens a J1939 database file
;Input 		   		: DBF File Name
;Output		   		: -
;==========================================================================================================

Func _openJ1939DB($dbFName)
	$dbMenu=WinMenuSelectItem($WIN_BUSMASTER,"",$J1939Menu,$databaseMenu,$openMenu)
	sleep(1000)
	if winexists($WIN_BUSMASTER) Then
		send("{ENTER}")																			; if warning message appears then select yes
	EndIf
	if WinWaitActive($WIN_BUSMASTER,$saveDBtxt,2) Then
		ControlClick($WIN_BUSMASTER,"",$BTN_No_SaveDB)
	EndIf
	$DBFolderPath = _TestDataPath()																; Set the DirPath to save the dbf file
	if WinWaitActive($WIN_SelJ1939_DBFile,"",5) Then
		ControlSend($WIN_SelJ1939_DBFile,"",$TXT_FileName_OpenDB,$DBFolderPath&"\"&$dbFName)  	; Set Filename
		sleep(1000)
		$cDB=ControlClick($WIN_SelJ1939_DBFile,"",$BTN_OpenInst_OpenDB) 							; Click on Save button
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
;Function Name 		: _J1939DBmessage
;Functionality 		: This function selects either 'Edit message' or 'Delete message' from the right click menu of the msg
;Input 		   		: $menu("e" for 'Edit Message' or "d" for 'Delete Message'
;Output		   		: -
;==========================================================================================================

Func _J1939DBmessage($menu)
	$DBTreeVwHWD = ControlGetHandle($WIN_DBEditor_J1939, "", $TVC_DBFile_DBEditor) 	; Fetch tree view control handle
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
	sleep(500)
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
	WinMenuSelectItem($WIN_BUSMASTER,"",$CANMenu,$databaseMenu,$saveMenu)						; Select CAN->Database->Save
	if WinWaitActive($WIN_BUSMASTER,$SaveDBImporttxt,3) Then
		consolewrite("Dialog Exists"&@CRLF)
		ControlClick($WIN_BUSMASTER,"",$BTN_Yes_BM)
	EndIf
	WinMenuSelectItem($WIN_BUSMASTER,"",$CANMenu,$databaseMenu,$closeMenu)						; Select File->Database->Close
EndFunc

;==========================================================================================================
;Function Name 		: _AssociateCANDB
;Functionality 		: This function Associates the database
;Input 		   		: Database filename
;Output		   		:
;==========================================================================================================

Func _AssociateCANDB($dbFName)
	WinMenuSelectItem($WIN_BUSMASTER,"",$CANMenu,$databaseMenu,$AssocCANDB)						; Select File->Database->Associate
	sleep(1000)
	$DBFolderPath =  _TestDataPath()															; Set the DirPath to Associate the dbf file
	if winexists($WIN_Associate_CANDB) Then
		ControlSend($WIN_Associate_CANDB,"",$TXT_FileName_AssocDB,$DBFolderPath&"\"&$dbFName)   ; Set Filename
		sleep(1000)
		$cDB=ControlClick($WIN_Associate_CANDB,"",$BTN_Open_AssocDB,"left") 					; Click on open button
		sleep(1000)
	EndIf
EndFunc

;==========================================================================================================
;Function Name 		: _AssociateJ1939DB
;Functionality 		: This function Associates the database
;Input 		   		: Database filename
;Output		   		:
;==========================================================================================================

Func _AssociateJ1939DB($dbFName)
	WinMenuSelectItem($WIN_BUSMASTER,"",$J1939Menu,$databaseMenu,$AssocCANDB)					; Select J1939->Database->Associate
	sleep(1000)
	$DBFolderPath =  _TestDataPath()															; Set the DirPath to Associate the dbf file
	if winexists($WIN_Associate_CANDB) Then
		ControlSend($WIN_Associate_CANDB,"",$TXT_FileName_AssocDB,$DBFolderPath&"\"&$dbFName)   ; Set Filename
		sleep(1000)
		$cDB=ControlClick($WIN_Associate_CANDB,"",$BTN_Open_AssocDB,"left") 					; Click on open button
		sleep(1000)
	EndIf
EndFunc

;==========================================================================================================
;Function Name 		: _DissociateCANDBMenu
;Functionality 		: This function selects CAN->Database->Dissociate menu
;Input 		   		:
;Output		   		:
;==========================================================================================================

Func _DissociateCANDBMenu()
	send("!CDD")																				; Select CAN->Database->Dissociate
EndFunc

;==========================================================================================================
;Function Name 		: _DissociateJ1939DBMenu
;Functionality 		: This function selects j1939->Database->Dissociate menu
;Input 		   		:
;Output		   		:
;==========================================================================================================

Func _DissociateJ1939DBMenu()
	send("!JDD")																				; Select J1939->Database->Dissociate
EndFunc

;==========================================================================================================
;Function Name 		: _DissociateCANDB
;Functionality 		: This function Dissociates the database
;Input 		   		: Database filename
;Output		   		:
;==========================================================================================================

Func _DissociateCANDB($index)
	send("!CDD")																				; Select CAN->Database->Dissociate
	sleep(1000)
	$DBFolderPath = _OutputDataPath()															; Set the DirPath to save the dbf file
	if WinExists($WIN_Dissociate_CANDB) Then
		ConsoleWrite("Dissociate CAN DB window exists"&@CRLF)
		$disDBlbHWD=ControlGetHandle($WIN_Dissociate_CANDB,"",$LSTB_DBFile_Dis_CANDB)			; Fetch the handle of dissociate Db list box
		_GUICtrlListBox_ClickItem($disDBlbHWD,$index)											; Click on the specified index
		controlclick($WIN_Dissociate_CANDB,"",$BTN_Dissoc_Dis_CANDB)							; Click on Dissociate button\
		Sleep(1000)
		if WinWaitActive($WIN_BUSMASTER,$disDBtxt1,5) Then
			ControlClick($WIN_BUSMASTER,$disDBtxt1,1)
		EndIf
		send("{ESC}")
		return 1;
	EndIf
EndFunc

;==========================================================================================================
;Function Name 		: _DissociateJ1939DB
;Functionality 		: This function Dissociates J1939 database
;Input 		   		: Database filename
;Output		   		:
;==========================================================================================================

Func _DissociateJ1939DB($index)
	send("!JDD")																				; Select J1939->Database->Dissociate
	sleep(1000)
	$DBFolderPath = _OutputDataPath()															; Set the DirPath to save the dbf file
	if winexists($WIN_Dissociate_CANDB) Then
		ConsoleWrite("Dissociate CAN DB window exists"&@CRLF)
		$disDBlbHWD=ControlGetHandle($WIN_Dissociate_CANDB,"",$LSTB_DBFile_Dis_CANDB)			; Fetch the handle of dissociate Db list box
		_GUICtrlListBox_ClickItem($disDBlbHWD,$index)											; Click on the specified index
		controlclick($WIN_Dissociate_CANDB,"",$BTN_Dissoc_Dis_CANDB)							; Click on Dissociate button
	EndIf
EndFunc

;==========================================================================================================
;Function Name 		: _DelSignalDesc()
;Functionality 		: This function Deletes the signal description
;Input 		   		:
;Output		   		:
;==========================================================================================================

Func _DelSignalDesc()
	$DelSignalDescPopUp=0
	$sigDesclvhwd=controlgethandle($WIN_BUSMASTER,"",$LVC_SigDesc_SigDesc)							; Get handle of signal details list view
	for $i=0 to _GUICtrlListView_GetItemCount($sigDesclvhwd)-1
		_GUICtrlListView_ClickItem($sigDesclvhwd,0)													; Click on the first item in the Signal details list view
		sleep(800)
		controlclick($WIN_BUSMASTER,"",$BTN_DelSigDesc_DBEditor)									; Delete Signal Desc
		sleep(1500)
		$DelSignalDescPopUp=0
		if winexists($WIN_BUSMASTER,$delSigDesctxt) Then
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
		WinMenuSelectItem($WIN_BUSMASTER,"",$CANMenu,$DriverSelectMenu,$kvaserHWMenu)
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
;Function Name : _ConfigCANTXBlockDetails
;Functionality : Configs CAN Tx block details
;Input 		   : Tx Mode,Time Delay btw msg='Yes'/'No', Msg Delay Value, Key Trigger='Yes'/'No', Key,Msgs='Single'/'All', Time Delay btw block='Yes'/'No', Block Delay Value)
;Output 	   :
;===============================================================================

Func _ConfigCANTXBlockDetails($TxMode,$TDBM,$TDBMVal,$KeyTrigger,$Key,$Msgs,$TDBB,$TDBBVal)
	ControlClick($WIN_BUSMASTER,"",$BTN_AddBlock_ConfigTX)											; Click on 'Add' block message
	sleep(500)
	if $TxMode="Monoshot" Then
		ControlCommand($WIN_BUSMASTER,"",$RBTN_Monoshot_ConfigTX,"Check")							; Enable 'Monoshot'
	EndIf
	If $TxMode="Cyclic" Then
		ControlCommand($WIN_BUSMASTER,"",$RBTN_Cyclic_ConfigTx,"Check")
	EndIf

		if $TDBB="Yes" Then
			ControlCommand($WIN_BUSMASTER,"",$CHKB_BlockDelay_ConfigTX,"Check")							; Enable 'Time Delay Between Block'
		    ControlSetText($WIN_BUSMASTER,"",$TXT_BlockDelay_ConfigTX,$TDBBVal)							; Set 'Time Delay Between Block' value
;~ 	Else
;~ 		ControlSetText($WIN_BUSMASTER,"",$TXT_MsgDelay_ConfigTX,$TDBMVal)							; Set 'Time Delay Between Block' value
	    EndIf
		If $TDBM="Yes" Then
			ControlCommand($WIN_BUSMASTER,"",$CHKB_BlockDelay_ConfigTX,"UnCheck")
			sleep(700)
		    ControlCommand($WIN_BUSMASTER,"",$CHKB_MsgDelay_ConfigTX,"Check")
		    ControlSetText($WIN_BUSMASTER,"",$TXT_MsgDelay_ConfigTX,$TDBMVal)
;~ 	Else
;~ 		ControlCommand($WIN_BUSMASTER,"",$CHKB_MsgDelay_ConfigTX,"UnCheck")
	EndIf
	If $TDBM="No" Then
		ControlCommand($WIN_BUSMASTER,"",$CHKB_MsgDelay_ConfigTX,"UnCheck")
	EndIf

	if $KeyTrigger="Yes" Then
		ControlCommand($WIN_BUSMASTER,"",$CHKB_KeyTrig_ConfigTX,"Check")							; Enable 'Key Trigger'
		ControlSetText($WIN_BUSMASTER,"",$TXT_KeyValue_ConfigTX,$Key)								; Set 'Time Delay Between Block' value
		ControlCommand($WIN_BUSMASTER,"",$COMB_KeyType_ConfigTX_INST,"SelectString",$Msgs)				; Select Msgs for Key trigger
	EndIf
EndFunc



;===============================================================================
;Function Name : _AddMsg2TxList
;Functionality : Adds a message to the TX list
;Input 		   :
;Output 	   :
;===============================================================================
Func _AddMsg2TxList($Index)
	ControlCommand($WIN_BUSMASTER,"",$COMB_MsgID_ConfigTX,"SetCurrentSelection",$Index)				; Add the DB msgs to the Tx message list
	ControlClick($WIN_BUSMASTER,"",$BTN_AddMsg_ConfigTX)											; Click on 'Add' block message
	sleep(500)
EndFunc

 Func _AddNonDBMsg2TxList($Id)
	 ControlSetText($WIN_BUSMASTER,"",$TXT_MsgID_ConfigTX,$Id)
	ControlClick($WIN_BUSMASTER,"",$BTN_AddMsg_ConfigTX)
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
;Function Name : _DisableAutoUpdate
;Functionality : UnChecks Auto Update button if checked
;Input 		   :
;Output 	   :
;===============================================================================
Func _DisableAutoUpdate()
	if (ControlCommand($WIN_BUSMASTER,"",$BTN_AutoUpdate_ConfigTX,"IsChecked")=1) Then
		ControlCommand($WIN_BUSMASTER,"",$BTN_AutoUpdate_ConfigTX,"UnCheck")							; Check 'Autoupdate' check box is enabled
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
	winwaitactive($WIN_Config_filter,"",3)
EndFunc

;===============================================================================
;Function Name : _AddFilter
;Functionality : Configures Filter details for CAN
;Input 		   :
;Output 	   :
;===============================================================================
Func _AddFilter($filterType,$msgID,$FromID,$ToID,$IDType,$Frame,$Dir,$Channel)
	_AppFilterMenu()																			; Select CAN->Filter menu
	WinWaitActive($WIN_Config_filter,"",2)														; Wait for the window to be active
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
	if $msgID<>"" Then
		ControlCommand($WIN_Config_filter,"",$COMB_MsgID_ConfigFilter,"SelectString", $msgID)	; Select the MSG from ID combo box
		sleep(200)
	Else
		ControlCommand($WIN_Config_filter,"",$RBTN_Range_FilterSelect,"Check")					; Select the Range radio button
		sleep(200)
		ControlSetText($WIN_Config_filter,"",$EDIT_From_FilterSelect,$FromID)					; Enter the From ID
		sleep(200)
		ControlSetText($WIN_Config_filter,"",$EDIT_To_FilterSelect,$ToID)						; Enter the To ID
	EndIf
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
	WinMenuSelectItem($WIN_BUSMASTER,"",$CANMenu,$CANMsgWinMenu,$configureMenu)					; Select CAN->Msg Window->Configure
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
;Function Name : _DeleteCANFilter()
;Function Name : _DeleteCANFilter()
;Functionality : Deleting filter from "configure filter list" dialog
;Input 		   : Index in the list view
;Output 	   :
;===============================================================================
Func _DeleteCANFilter($FilterIndex)
	_AppFilterMenu()
	winwaitactive($WIN_Config_filter,"",3)															; Wait for "Configure Logging for CAN" dialog
	$FilterListHWD=ControlGetHandle($WIN_Config_filter,"",$LSTC_FilterList_ConfigFilter)			; Fetch the handle of filter list view
	_GUICtrlListView_ClickItem($FilterListHWD,$FilterIndex)											; Click on the index specified
	sleep(500)
	ControlClick($WIN_Config_filter,"",1002)														; Click on Delete button
	ControlClick($WIN_Config_filter,"",$BTN_OK_ConfigFilter)										; Click on OK button
	sleep(500)
EndFunc

;===============================================================================
;Function Name : _DeleteCANFilterDet()
;Functionality : Deleting filter detail from "configure filter list" dialog
;Input 		   : Index in the list view
;Output 	   :
;===============================================================================
Func _DeleteCANFilterDet($FilterIDIndex)
	_AppFilterMenu()
	winwaitactive($WIN_Config_filter,"",3)															; Wait for "Configure Logging for CAN" dialog
	$FilterDetHWD=ControlGetHandle($WIN_Config_filter,"",$LVC_FilterDetails_ConfigFilter)			; Fetch the handle of filter list view
	_GUICtrlListView_ClickItem($FilterDetHWD,$FilterIDIndex)										; Click on the index specified
	sleep(500)
	ControlClick($WIN_Config_filter,"",$BTN_DelFil_ConfigFilter)									; Click on Delete button
	sleep(500)
	ControlClick($WIN_Config_filter,"",$BTN_OK_ConfigFilter)										; Click on OK button
	sleep(500)
EndFunc

;===============================================================================
;Function Name : _EnableFilterDispMenu
;Functionality : Enable filters for message display from menu
;Input 		   :
;Output 	   :
;===============================================================================
Func _En_Dis_FilterDisplay()
	$cntToolhWd=ControlGetHandle($WIN_BUSMASTER,"",$ToolBar_Con_Disconnect)						; Get handle of the 'Disable Msg Disp Filter' toolbar
	_GUICtrlToolbar_ClickIndex($cntToolhWd,9)													; Click on 'Disable Msg Disp Filter' icon
	sleep(750)
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
	_GUICtrlToolbar_ClickIndex($cntToolhWd,$Icon_ConDiscon_Index)								; Click on 'Connect/Disconnect' icon
	sleep(1500)
EndFunc

;===============================================================================
;Function Name : _ClearMsgWindow()
;Functionality : Clicks on the Clear Msg window icon in the toolbar
;Input 		   :
;Output 	   :
;===============================================================================
Func _ClearMsgWindow()
	$hWd=ControlGetHandle($WIN_BUSMASTER,"",$ToolBar_MsgWindow)									; Get handle of the 'Message Window' toolbar
	_GUICtrlToolbar_ClickIndex($hWd,$Icon_ClearMsgWin_Index)									; Click on 'Clear Msg Win' icon
	sleep(1000)
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
;Function Name : _J1939LogMenu()
;Functionality : Open J1939 Log Dialog
;Input 		   :
;Output 	   :
;===============================================================================
Func _J1939LogMenu()
	WinMenuSelectItem($WIN_BUSMASTER,"",$J1939Menu,$J1939LogMenu,$configureMenu)						; Select J1939->Logging->Configure
	WinWaitActive($WIN_J1939Log,"",3)
EndFunc

;===============================================================================
;Function Name : _ConfigureCANLog()
;Functionality : Configure Logging for CAN
;Input 		   : Log File Name, Time Mode, Channel #, Numeric Format, File Mode, Start Rec MsgID, Stop Rec MsgID, Filters={True,False}
;Output 	   :
;===============================================================================
Func _ConfigureCANLog($logFileName,$TimeMode,$Channel,$NumFormat,$FileMode,$Start_Rec,$Stop_Rec,$Filters)
	WinMenuSelectItem($WIN_BUSMASTER,"",$CANMenu,$CANLogMenu,$configureMenu)					; Select Configure->Log
	Sleep(1000)
	ControlClick($WIN_CANLog,"",$BTN_Add_ConfigLOG)												; Click on Add button
	sleep(250)
	ControlCommand ($WIN_CANLog,"",$BTN_EnDisable_ConfigLOG,"Check")							; Check Enable/Disable Logging during connect/disconnect
	$LogFilePath=_OutputDataPath()
	ControlClick($WIN_CANLog,"",$BTN_logFPath_ConfigLOG)										; Click on Log File Path button
	sleep(1000)
	if winexists($WIN_Select_LogFile) Then
		controlsend($WIN_Select_LogFile,"",$TXT_FileName_SelLogFile,$LogFilePath & "\" & $logFileName)	; Enter the Log file Name
		controlclick($WIN_Select_LogFile,"",$BTN_SaveInst_SelLogFile)									; Click on 'Save' button
	EndIf
	controlcommand($WIN_CANLog,"",$COMB_TimeMode_ConfigLOG,"SelectString", $TimeMode)			; Set the time mode
	controlcommand($WIN_CANLog,"",$COMB_Channel_ConfigLOG,"SelectString", $Channel)				; Set the Channel
	sleep(500)
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
	sleep(500)
	if $Filters="True" Then
		controlclick($WIN_CANLog,"",$BTN_Filter_ConfigLOG)										; Click on 'Filter' button
		sleep(1000)
		if WinWaitActive($WIN_FilterSelect,"",3) Then
			$fltSelhWd=controlgethandle($WIN_FilterSelect,"",$LSTC_ConfigFilter_FilterSelect)	; Get handle of filter selection list view
			_GUICtrlListView_ClickItem($fltSelhWd,0)											; Click on the first filter
			ControlClick($WIN_FilterSelect,"",$BTN_Add_FilterSelect)								; Click on Add button
			sleep(800)
			ControlClick($WIN_FilterSelect,"",$BTN_OK_FilterSelect)								; Click on OK button
			sleep(800)
		EndIf
	EndIf
	controlclick($WIN_CANLog,"",$BTN_OK_ConfigLOG)												; Click on 'OK' button
	sleep(500)
EndFunc

;===============================================================================
;Function Name : _ConfigureJ1939Log()
;Functionality : Configure Logging for J1939
;Input 		   : Log File Name, Time Mode, Channel #, Numeric Format, File Mode, Start Rec MsgID, Stop Rec MsgID, Filters={True,False}
;Output 	   :
;===============================================================================
Func _ConfigureJ1939Log($logFileName,$TimeMode,$Channel,$NumFormat,$FileMode,$Start_Rec,$Stop_Rec,$Filters)
	_J1939LogMenu()																					; Open Configure J1939 Logging
	Sleep(1000)
	ControlClick($WIN_J1939Log,"",$BTN_Add_ConfigLOG)												; Click on Add button
	sleep(250)
	ControlCommand ($WIN_J1939Log,"",$BTN_EnDisable_ConfigLOG,"Check")								; Check Enable/Disable Logging during connect/disconnect
	$LogFilePath=_OutputDataPath()
	ControlClick($WIN_J1939Log,"",$BTN_logFPath_ConfigLOG)											; Click on Log File Path button
	WinWaitActive($WIN_Select_LogFile,"",3)
	if winexists($WIN_Select_LogFile) Then
		controlsend($WIN_Select_LogFile,"",$TXT_FileName_SelLogFile,$LogFilePath & "\" & $logFileName)	; Enter the Log file Name
		controlclick($WIN_Select_LogFile,"",$BTN_SaveInst_SelLogFile)									; Click on 'Save' button
	EndIf
	controlcommand($WIN_J1939Log,"",$COMB_TimeMode_ConfigLOG,"SelectString", $TimeMode)				; Set the time mode
	controlcommand($WIN_J1939Log,"",$COMB_Channel_ConfigLOG,"SelectString", $Channel)				; Set the Channel
	if $NumFormat="Hex" Then
		Controlcommand($WIN_J1939Log,"",$RBTN_NumModeHex_ConfigLOG,"Check")							; Set the Numeric mode to Hex
	Else
		Controlcommand($WIN_J1939Log,"",$RBTN_NumModeDec_ConfigLOG,"Check")							; Set the Numeric mode to Decimal
	EndIf
	sleep(1000)
	if $FileMode="Overwrite" Then
		Controlcommand($WIN_J1939Log,"",$RBTN_FileModeOverWrite_ConfigLOG,"Check")					; Set the File mode
	Else
		Controlcommand($WIN_J1939Log,"",$RBTN_FileModeApp_ConfigLOG,"Check")						; Set the File mode
	EndIf
	if $Start_Rec<>"" and $Stop_Rec<>"" Then
		Controlcommand($WIN_J1939Log,"",$CHKB_StartTrig_ConfigLOG,"Check")							; enable the start trigger
		ControlSend($WIN_J1939Log,"",$TXT_StartMsgID_ConfigLOG,$Start_Rec)							; Set the Start trigger Msg ID
		Controlcommand($WIN_J1939Log,"",$CHKB_StopTrig_ConfigLOG,"Check")							; enable the stop trigger
		ControlSend($WIN_J1939Log,"",$TXT_StopMsgID_ConfigLOG,$Stop_Rec)							; Set the Stop trigger Msg ID
	EndIf
	if $Filters="True" Then
		controlclick($WIN_J1939Log,"",$BTN_Filter_ConfigLOG)										; Click on 'Filter' button
		WinWaitActive($WIN_FilterSelect,"",3)
		if winexists($WIN_FilterSelect) Then
			$fltSelhWd=controlgethandle($WIN_FilterSelect,"",$LSTC_ConfigFilter_FilterSelect)		; Get handle of filter selection list view
			_GUICtrlListView_ClickItem($fltSelhWd,0)												; Click on the first filter
			ControlClick($WIN_FilterSelect,"",$BTN_Add_FilterSelect)								; Click on Add button
			sleep(800)
			ControlClick($WIN_FilterSelect,"",$BTN_OK_FilterSelect)									; Click on OK button
			sleep(800)
		EndIf
	EndIf
	controlclick($WIN_J1939Log,"",$BTN_OK_ConfigLOG)												; Click on 'OK' button
	sleep(500)
EndFunc

;===============================================================================
;Function Name : _DeleteCANLog()
;Functionality : Deleting Log file from "configure logging for CAN" dialog
;Input 		   : File index in the list view
;Output 	   :
;===============================================================================
Func _DeleteCANLog($FileIndex)
	_CANLogMenu()
	winwaitactive($WIN_CANLog,"",3)															; Wait for "Configure Logging for CAN" dialog
	$LogFileHWD=ControlGetHandle($WIN_CANLog,"",$LSTC_LogFile_ConfigLOG)					; Fetch the handle of log file list view
	_GUICtrlListView_ClickItem($LogFileHWD,$FileIndex)										; Click on the index specified
	ControlClick($WIN_CANLog,"",$BTN_RemoveLog_ctrlID)										; Click on Remove button
	controlclick($WIN_CANLog,"",$BTN_OK_ConfigLOG)												; Click on 'OK' button
	sleep(750)
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
;Function Name : CANReplayMenu
;Functionality : Opens replay configuration window
;Input 		   :
;Output 	   :
;===============================================================================
Func _CANReplayMenu()
	WinMenuSelectItem($WIN_BUSMASTER,"",$CANMenu,$CANReplayMenu,$CANReplayConfigMenu)		; Select CAN->Replay->Configure
	WinWaitActive($WIN_CANReplayConfig,"",3)
EndFunc

;===============================================================================
;Function Name : CANReplayOptionsMenu
;Functionality : Selects the passed parameter menu
;Input 		   :
;Output 	   :
;===============================================================================
Func _CANReplayOptionsMenu($OptMenu)
	WinMenuSelectItem($WIN_BUSMASTER,"",$CANMenu,$CANReplayMenu,$OptMenu)						; Select CAN->Replay->$Menu
EndFunc

;===============================================================================
;Function Name : _ADDReplayFile
;Functionality : Add a replay file
;Input 		   :
;Output 	   :
;===============================================================================
Func _ADDReplayFile($ReplayFileName)
	ControlClick($WIN_CANReplayConfig,"",$BTN_ADDFile_CANReplayConfig)										; Click on ADD file button
	$RepFilePath=_TestDataPath()
	WinWaitActive($DLG_SelReplayFile,"",3)
	if WinWaitActive($DLG_SelReplayFile,"",3) Then
		controlsend($DLG_SelReplayFile,"",$TXT_FileName_SelReplayFile,$RepFilePath & "\" & $ReplayFileName)	; Enter the Log file Name
		controlclick($DLG_SelReplayFile,"",$BTN_OpenInstSelReplayFile)										; Click on 'Open' button
	EndIf
	sleep(500)
EndFunc

;===============================================================================
;Function Name : _DELReplayFile
;Functionality : Delete the replay file
;Input 		   :
;Output 	   :
;===============================================================================
Func _DELReplayFile()
	ControlClick($WIN_CANReplayConfig,"",$BTN_DELFile_CANReplayConfig)							; Click on Delete file button
	WinWaitActive($WIN_BUSMASTER,$TXT_DELReplayFile,3)
	ControlClick($WIN_BUSMASTER,"",$BTN_Yes_BM)													; Click on Yes button
EndFunc

;===============================================================================
;Function Name : _GetReplayWinHWD
;Functionality : fetches the handle of the replay window
;Input 		   :
;Output 	   :
;===============================================================================
Func _GetReplayWinHWD()
	$HWD=ControlGetHandle($WIN_BUSMASTER,"",$LVC_CID_ReplayWin)									; Fetch the handle of replay window list vieww
	Return $HWD
EndFunc

;===============================================================================
;Function Name : _GetReplayOptionsMenuState
;Functionality : fetches the state of the replay options(Go,Skip,Step,Stop)
;Input 		   : Menu index
;Output 	   :
;===============================================================================
Func _GetReplayOptionsMenuState($mIndex)
	send("!CR")																					; Select CAN->Replay from the menu
	sleep(1000)
	send("{ESC}")
	sleep(500)
	send("{ESC}")
	sleep(500)
	if winexists($WIN_CANMsgWind) Then
		$winhWnd = WinGetHandle($WIN_BUSMASTER)												; Fetch the window handle
		$hMain = _GUICtrlMenu_GetMenu($winhWnd)												; Fetch the handle of the menu
		ConsoleWrite("menu handle : "&$hMain& @CRLF)
		$hFile = _GUICtrlMenu_GetItemSubMenu($hMain, 2)										; Fetch the handle of CAN menu
		ConsoleWrite("Submenu handle : "&$hFile& @CRLF)
		$hSubmenu=_GUICtrlMenu_GetItemSubMenu($hFile,14)									; Fetch the handle of CAN->Replay menu
		ConsoleWrite("$hSubmenu handle : "&$hSubmenu& @CRLF)
		$MenuState=_GUICtrlMenu_GetItemEnabled($hSubmenu,$mIndex,True)						; Check whether CAN->Replay->$index is enabled
		$GoMenuStateText=_GUICtrlMenu_GetItemText($hSubmenu,$mIndex,True)					; Fetch the text of first item in CAN->Replay
		ConsoleWrite("$MenuState : "&$MenuState& @CRLF)
		ConsoleWrite("$$GoMenuStateText : "&$GoMenuStateText& @CRLF)
	Else
		$winhWnd = WinGetHandle($WIN_BUSMASTER)												; Fetch the window handle
		$hMain = _GUICtrlMenu_GetMenu($winhWnd)
		ConsoleWrite("menu handle : "&$hMain& @CRLF)
		$hFile = _GUICtrlMenu_GetItemSubMenu($hMain, 1)
		ConsoleWrite("Submenu handle : "&$hFile& @CRLF)
		$hSubmenu=_GUICtrlMenu_GetItemSubMenu($hFile,14)
		ConsoleWrite("$hSubmenu handle : "&$hSubmenu& @CRLF)
		$hSubmenuTxt=_GUICtrlMenu_GetItemText($hFile,14,True)
		ConsoleWrite("$hSubmenu Txt : "&$hSubmenuTxt& @CRLF)
		$MenuState=_GUICtrlMenu_GetItemEnabled($hSubmenu,$mIndex,True)						; Check whether CAN->Replay->$index is enabled
		$GoMenuStateText=_GUICtrlMenu_GetItemText($hSubmenu,$mIndex,True)					; Fetch the text of first item in CAN->Replay
		ConsoleWrite("$MenuState : "&$MenuState& @CRLF)
		ConsoleWrite("$$GoMenuStateText : "&$GoMenuStateText& @CRLF)
	EndIf
	return $MenuState
EndFunc

;===============================================================================
;Function Name : _AddReplayFilters
;Functionality : Adds a filter in the replay configuration window
;Input 		   :
;Output 	   :
;===============================================================================
Func _AddReplayFilters()
	ControlClick($WIN_CANReplayConfig,"",$BTN_Filters_CANReplayConfig)							; Click on 'Filters' button
	WinWaitActive($WIN_FilterSelect,"",5)
	if winexists($WIN_FilterSelect) Then
		$fltSelhWd=controlgethandle($WIN_FilterSelect,"",$LSTC_ConfigFilter_FilterSelect)		; Get handle of filter selection list view
		_GUICtrlListView_ClickItem($fltSelhWd,0)												; Click on the first filter
		ControlClick($WIN_FilterSelect,"",$BTN_Add_FilterSelect)								; Click on Add button
		sleep(750)
		ControlClick($WIN_FilterSelect,"",$BTN_OK_FilterSelect)									; Click on OK button
		sleep(750)
	EndIf
EndFunc

;===============================================================================
;Function Name : _ReplayFilterMenu
;Functionality : Selects CAN->Replay->Enable Filters menu
;Input 		   :
;Output 	   :
;===============================================================================
Func _ReplayFilterMenu()
	WinMenuSelectItem($WIN_BUSMASTER,"",$CANMenu,$CANReplayMenu,$CANEnableFilterReplayMenu)		; Select CAN->Replay->Enable Filters
	sleep(1000)
EndFunc

;===============================================================================
;Function Name : _SignalWatchMenu
;Functionality : Selects Signal Watch->Activate or Configure menu
;Input 		   : BusType, Sub Menu Item(Activate or Configure)
;Output 	   :
;===============================================================================
Func _SignalWatchMenu($BusType,$SubMenu)
	if $BusType="CAN" Then
		WinMenuSelectItem($WIN_BUSMASTER,"",$CANMenu,$CANSigWatchMenu,$SubMenu)							; Open the CAN Signal watch window
	elseif $BusType="J1939" Then
		WinMenuSelectItem($WIN_BUSMASTER,"",$J1939Menu,$J1939SigWatchMenu,$SubMenu)						; Open the J1939 Signal watch window

	elseif $BusType="LIN" Then
		ConsoleWrite("Lin signal watch config menu")
		WinMenuSelectItem($WIN_BUSMASTER,"",$linMenu,$LINSigWatchMenu,$SubMenu)							; Open the LIN Signal watch window

	EndIf

	if $SubMenu=$SigWatchConfigMenu Then
		winwaitactive($WIN_SigWatchConfig,"",2)															; Wait for the configure signal watch window to be active
		consolewrite("Configure Signal Watch Window exists"&@crlf)
	ElseIf $SubMenu=$SigWatchActivateMenu Then
		winwaitactive($WIN_SigWatch&$BusType,"",2)														; Wait for the signal watch list window to be active
		consolewrite("Signal Watch List Window exists"&@crlf)
	EndIf
	sleep(1000)
EndFunc

;===============================================================================
;Function Name : _ConfigCANSignalWatch
;Functionality : Configures CAN signal watch list
;Input 		   : BusType,No. of msgs to be selected for adding signals
;Output 	   :
;===============================================================================
Func _ConfigSignalWatch($BusType,$NumOfMsgs)
	_SignalWatchMenu($BusType,$SigWatchConfigMenu)														; open configure signal watch list window
	$MsgListHWD=ControlGetHandle($WIN_SigWatchConfig,"",$COMBO_MsgList_SigWatch)						; Fetch the handle of Message List combo box
	$MsgCount=_GUICtrlComboBox_GetCount($MsgListHWD)													; Fetch the no. of msgs in the combo box
	consolewrite("$MsgCount: "&$MsgCount&@crlf)
	if $MsgCount>=$NumOfMsgs Then
		for $i=0 to $NumOfMsgs-1
 			ControlCommand ($WIN_SigWatchConfig,"",$COMBO_MsgList_SigWatch,"SetCurrentSelection",$i)	; Select the msg based on the index
			sleep(1000)
			ControlClick($WIN_SigWatchConfig,"",$BTN_AddAll_SigWatch)									; Click on "Add All" button
			sleep(1000)
		Next
	EndIf
	ControlClick($WIN_SigWatchConfig,"",$BTN_OK_SigWatch)												; Click on "OK" button
EndFunc

;===============================================================================
;Function Name : _GetSigWatchWinInfo()
;Functionality : Fetches the signal watch window info
;Input 		   : Row No.
;Output 	   :
;===============================================================================
Func _GetSigWatchWinInfo($BusType,$Row)
	Dim $SigData[10]=["","","","","","","","","",""]
	$HWD=ControlGetHandle($WIN_SigWatch&$BusType,"",$LVC_Value_SigWatch)								; Fetch the handle of Signal watch window
	$ItemCount=_GUICtrlListView_GetItemCount($HWD)														; Fetch the count of items
	ConsoleWrite("$ItemCount "&$ItemCount&@CRLF)
	if $ItemCount>0 Then
		$SigDetails=_GUICtrlListView_GetItemTextArray($HWD,$Row)										; Fetch the text of the row
		$SigData[1]=$SigDetails[1]																		; Assign signal name
		$SigData[2]=$SigDetails[2]																		; Assign signal physical value
		$SigData[3]=$SigDetails[3]																		; Assign signal raw value
		$SigData[4]=$SigDetails[4]																		; Assign signal unit
	EndIf
	for $i=1 to 4
		ConsoleWrite("$SigData[" & $i &"] : "&$SigData[$i]&@CRLF)
	next
	Return $SigData																						; Return all the data
EndFunc


;===============================================================================
;Function Name : _GetSigWatchWinItemCount()
;Functionality : Fetches the no. of items in the signal watch window
;Input 		   :
;Output 	   : Count
;===============================================================================
Func _GetSigWatchWinItemCount()
	$HWD=ControlGetHandle($WIN_SigWatch&$CAN,"",$LVC_Value_SigWatch)									; Fetch the handle of Signal watch window
	$Count=_GUICtrlListView_GetItemCount($HWD)															; Fetch the count of items
	Return $Count
EndFunc


;===============================================================================
;Function Name : _GetMsgInterpretWinInfo()
;Functionality : Fetches the message interpretation window info
;Input 		   : Row No.
;Output 	   :
;===============================================================================
Func _GetMsgInterpretWinInfo($Row)
	Dim $MsgData[10]=["","","","","","","","","",""]
	$HWD=_GetCANMsgWinHWD()																		; Fetch the handle of CAN msg window
	ConsoleWrite("$HWD="&$HWD)
	_GUICtrlListView_ClickItem($HWD,$Row,"","",2)												; Double click on the row
		if WinWaitActive($WIN_MsgInterpret,"",3) Then
			$SigHWD=ControlGetHandle($WIN_MsgInterpret,"",$LVC_Signal_MsgIntWin)				; Fetch the handle of Message interpretation window list view
			$ItemCount=_GUICtrlListView_GetItemCount($SigHWD)									; fetch the count of items
			ConsoleWrite("$ItemCount "&$ItemCount&@CRLF)
			if $ItemCount>0 Then
				$MsgData[1]=ControlGetText($WIN_MsgInterpret,"",$TXT_MsgName_MsgIntWin)				; Fetch the message name
				$MsgData[2]=ControlGetText($WIN_MsgInterpret,"",$TXT_MsgID_MsgIntWin)				; Fetch the message ID
				$SigDetails=_GUICtrlListView_GetItemTextArray($SigHWD,0)							; Fetch the text of the first row
				$MsgData[3]=$SigDetails[1]															; Assign signal name
				$MsgData[4]=$SigDetails[2]															; Assign signal physical value
				$MsgData[5]=$SigDetails[3]															; Assign signal raw value
				$MsgData[6]=$SigDetails[4]															; Assign signal unit
			EndIf
		winclose($WIN_MsgInterpret)
		EndIf
		for $i=1 to 6
			ConsoleWrite("$MsgData[" & $i &"] : "&$MsgData[$i]&@CRLF)
		next
	Return $MsgData																				; Return all the data
EndFunc
;===============================================================================
;Function Name : _GetCANMsgWinItemCount
;Functionality : fetches the item count in the CAN message window
;Input 		   :
;Output 	   :
;===============================================================================
Func _GetCANMsgWinItemCount()
	$HWD=_GetCANMsgWinHWD()
	$ItemCount=_GUICtrlListView_GetItemCount($HWD)
	ConsoleWrite("$ItemCount "&$ItemCount&@CRLF)
	Return $ItemCount
EndFunc
;===============================================================================
;Function Name : _GetJ1939MsgWinItemCount
;Functionality : fetches the item count in the J1939 message window
;Input 		   :
;Output 	   :
;===============================================================================
Func _GetJ1939MsgWinItemCount()
	$HWD=_GetCANMsgWinHWD()
	$ItemCount=_GUICtrlListView_GetItemCount($HWD)
	Return $ItemCount
EndFunc


;===============================================================================
;Function Name : _GetCANMsgWinColCount
;Functionality : fetches the column count in the CAN message window
;Input 		   :
;Output 	   :
;===============================================================================
Func _GetCANMsgWinColCount()
	$HWD=_GetCANMsgWinHWD()
	$ItemCount=_GUICtrlListView_GetColumnCount($HWD)
	Return $ItemCount
EndFunc


;===============================================================================
;Function Name : _GetCANMsgWinHWD
;Functionality : fetches the handle of the CAN message window
;Input 		   :
;Output 	   :
;===============================================================================
Func _GetCANMsgWinHWD()
	MouseClick("left", @DesktopWidth / 2, @DesktopHeight / 4)
	$HWD=ControlGetHandle($WIN_BUSMASTER,"",$LVC_CID_CANMsgWin)									; Fetch the handle of Message window list view
	ConsoleWrite("$HWD "&$HWD&@CRLF)
	Return $HWD
EndFunc

;===============================================================================
;Function Name : _DisableJ1939Win
;Functionality : Checks whether J1939 message window is active. If J1939 message window is active then disable it.
;Input 		   :
;Output 	   :
;===============================================================================
Func _DisableJ1939Win()
	send("!JM")																					; Select J1939->View from the menu
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
		if $res=True then																	; If J1939->View->Message Window is enabled then uncheck it
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
		if $res=True then																	; If J1939->View->Message Window is enabled then uncheck it
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
	Send("!CM")																				; Select CAN->Message Window from the menu
	sleep(750)
	send("{ESC}")
	sleep(500)
	send("{ESC}")
	sleep(500)
	sleep(500)
	if winexists($WIN_CANMsgWind) Then
		$winhWnd = WinGetHandle($WIN_BUSMASTER)												; Fetch the window handle
		$hMain = _GUICtrlMenu_GetMenu($winhWnd)												; Fetch the handle of the menu
		ConsoleWrite("menu handle : "&$hMain& @CRLF)
		$hFile = _GUICtrlMenu_GetItemSubMenu($hMain, 5)										; Fetch the handle of CAN menu
		ConsoleWrite("Submenu handle : "&$hFile& @CRLF)
		$hSubmenu=_GUICtrlMenu_GetItemSubMenu($hFile, 9)									; Fetch the handle of CAN->Msg Window menu
		ConsoleWrite("$hSubmenu handle : "&$hSubmenu& @CRLF)
		$res=_GUICtrlMenu_GetItemChecked($hSubmenu,4)										; Check whether CAN->Message Window->Overwrite is checked or not
		$val=_GUICtrlMenu_GetItemText($hSubmenu,6)											; Fetch the text of first item in CAN->Message Window
		ConsoleWrite("$res : "&$res& @CRLF)
		ConsoleWrite("$val : "&$val& @CRLF)
		if $res=True then																	; If J1939->View->Message Window is enabled then uncheck it
	$OvrWriteToolhWd=ControlGetHandle($WIN_BUSMASTER,"",$ToolBar_OverWrite)					; Get handle of the 'Connect/Disconnect' toolbar
	_GUICtrlToolbar_ClickIndex($OvrWriteToolhWd,4)											; Click on 'Connect' icon
	sleep(1000)
		EndIf
	Else
		$winhWnd = WinGetHandle($WIN_BUSMASTER)												; Fetch the window handle
		$hMain = _GUICtrlMenu_GetMenu($winhWnd)												; Fetch the handle of the menu
		ConsoleWrite("menu handle :	"&$hMain& @CRLF)
		$hFile = _GUICtrlMenu_GetItemSubMenu($hMain, 1)										; Fetch the handle of CAN menu
		ConsoleWrite("Submenu handle : "&$hFile& @CRLF)
		$hSubmenu=_GUICtrlMenu_GetItemSubMenu($hFile, 9)									; Fetch the handle of CAN->Msg Window menu
		ConsoleWrite("$hSubmenu handle : "&$hSubmenu& @CRLF)
		$res=_GUICtrlMenu_GetItemChecked($hSubmenu,6)										; Check whether CAN->Message Window->Overwrite is checked or not
		$val=_GUICtrlMenu_GetItemText($hSubmenu,6)											; Fetch the text of first item in CAN->Message Window
		ConsoleWrite("$res : "&$res& @CRLF)
		ConsoleWrite("$val : "&$val& @CRLF)
		if $res=True then																	; If J1939->View->Message Window is enabled then uncheck it
			$OvrWriteToolhWd=ControlGetHandle($WIN_BUSMASTER,"",$ToolBar_OverWrite)			; Get handle of the 'Connect/Disconnect' toolbar
			_GUICtrlToolbar_ClickIndex($OvrWriteToolhWd,4)									; Click on 'Connect' icon
			sleep(1000)
		EndIf
	EndIf
EndFunc

;===============================================================================
;Function Name : _EnableOverwriteMode
;Functionality : Enables the Msg Window OVerwrite Mode
;Input 		   :
;Output 	   :
;===============================================================================
Func _EnableOverwriteMode()
	Send("!CM")																				; Select CAN->Message Window from the menu
	sleep(750)
	send("{ESC}")
	sleep(500)
	send("{ESC}")
	sleep(500)
	sleep(500)
	if winexists($WIN_CANMsgWind) Then
		$winhWnd = WinGetHandle($WIN_BUSMASTER)												; Fetch the window handle
		$hMain = _GUICtrlMenu_GetMenu($winhWnd)												; Fetch the handle of the menu
		ConsoleWrite("menu handle : "&$hMain& @CRLF)
		$hFile = _GUICtrlMenu_GetItemSubMenu($hMain, 2)										; Fetch the handle of CAN menu
		ConsoleWrite("Submenu handle : "&$hFile& @CRLF)
		$hSubmenu=_GUICtrlMenu_GetItemSubMenu($hFile, 9)									; Fetch the handle of CAN->Msg Window menu
		ConsoleWrite("$hSubmenu handle : "&$hSubmenu& @CRLF)
		$res=_GUICtrlMenu_GetItemChecked($hSubmenu,6)										; Check whether CAN->Message Window->Overwrite is checked or not
		$val=_GUICtrlMenu_GetItemText($hSubmenu,6)											; Fetch the text of first item in CAN->Message Window
		ConsoleWrite("$res : "&$res& @CRLF)
		ConsoleWrite("$val : "&$val& @CRLF)
		if $res=False then																	; If J1939->View->Message Window is enabled then uncheck it
			$OvrWriteToolhWd=ControlGetHandle($WIN_BUSMASTER,"",$ToolBar_OverWrite)			; Get handle of the 'Connect/Disconnect' toolbar
			_GUICtrlToolbar_ClickIndex($OvrWriteToolhWd,4)									; Click on 'Connect' icon
			sleep(1000)
		EndIf
	Else
		$winhWnd = WinGetHandle($WIN_BUSMASTER)												; Fetch the window handle
		$hMain = _GUICtrlMenu_GetMenu($winhWnd)												; Fetch the handle of the menu
		ConsoleWrite("menu handle : "&$hMain& @CRLF)
		$hFile = _GUICtrlMenu_GetItemSubMenu($hMain, 1)										; Fetch the handle of CAN menu
		ConsoleWrite("Submenu handle : "&$hFile& @CRLF)
		$hSubmenu=_GUICtrlMenu_GetItemSubMenu($hFile, 9)									; Fetch the handle of CAN->Msg Window menu
		ConsoleWrite("$hSubmenu handle : "&$hSubmenu& @CRLF)
		$res=_GUICtrlMenu_GetItemChecked($hSubmenu,6)										; Check whether CAN->Message Window->Overwrite is checked or not
		$val=_GUICtrlMenu_GetItemText($hSubmenu,6)											; Fetch the text of first item in CAN->Message Window
		ConsoleWrite("$res : "&$res& @CRLF)
		ConsoleWrite("$val : "&$val& @CRLF)
		if $res=False then																	; If J1939->View->Message Window is enabled then uncheck it
			$OvrWriteToolhWd=ControlGetHandle($WIN_BUSMASTER,"",$ToolBar_OverWrite)			; Get handle of the 'Connect/Disconnect' toolbar
			_GUICtrlToolbar_ClickIndex($OvrWriteToolhWd,4)									; Click on 'Connect' icon
			sleep(1000)
		EndIf
	EndIf
EndFunc

;===============================================================================
;Function Name : _EnableInterpretMode
;Functionality : Enables message interpretation
;Input 		   :
;Output 	   :
;===============================================================================

Func _EnableInterpretMode()
   _EnableOverwriteMode()
	Sleep(1000)
	$winhWnd = WinGetHandle($WIN_BUSMASTER)												; Fetch the window handle
		$hMain = _GUICtrlMenu_GetMenu($winhWnd)												; Fetch the handle of the menu
	 If winexists($WIN_CANMsgWind) Then
		$hFile = _GUICtrlMenu_GetItemSubMenu($hMain, 2)										; Fetch the handle of CAN menu
		ConsoleWrite("Submenu handle : "&$hFile& @CRLF)
	Else
		$hFile = _GUICtrlMenu_GetItemSubMenu($hMain, 1)									  ; Fetch the handle of CAN menu
		ConsoleWrite("Submenu handle : "&$hFile& @CRLF)
	EndIf
	$hSubmenu=_GUICtrlMenu_GetItemSubMenu($hFile, 9)									 ; Fetch the handle of CAN->Msg Window menu
	ConsoleWrite("$hSubmenu handle : "&$hSubmenu& @CRLF)
	$res=_GUICtrlMenu_GetItemChecked($hSubmenu,7)                                      ;Check whether Interpret mode is enabled
	$text=_GUICtrlMenu_GetItemText($hSubmenu,7)
	ConsoleWrite("$text"& $text & @CRLF)
	ConsoleWrite("res1" & $res & @CRLF)
	If($res=False) Then
		$OvrWriteToolhWd=ControlGetHandle($WIN_BUSMASTER,"",$ToolBar_OverWrite)			; Get handle of the 'Connect/Disconnect' toolbar
		_GUICtrlToolbar_ClickIndex($OvrWriteToolhWd,2)									; Click on 'Interpret' icon
		sleep(1000)
	EndIf
EndFunc

;===============================================================================
;Function Name : _DisableInterpretMode
;Functionality : Disables message interpretation
;Input 		   :
;Output 	   :
;===============================================================================

Func _DisableInterpretMode()
	Send("!CM")																				; Select CAN->Message Window from the menu
	sleep(750)
	send("{ESC}")
	sleep(500)
	send("{ESC}")
	sleep(500)
	sleep(500)
	$winhWnd = WinGetHandle($WIN_BUSMASTER)												; Fetch the window handle
	$hMain = _GUICtrlMenu_GetMenu($winhWnd)												; Fetch the handle of the menu
	ConsoleWrite("menu handle : "&$hMain& @CRLF)
	If winexists($WIN_CANMsgWind) Then
		$hFile = _GUICtrlMenu_GetItemSubMenu($hMain, 2)										; Fetch the handle of CAN menu
		ConsoleWrite("Submenu handle : "&$hFile& @CRLF)
	Else
		$hFile = _GUICtrlMenu_GetItemSubMenu($hMain, 1)										; Fetch the handle of CAN menu
		ConsoleWrite("Submenu handle : "&$hFile& @CRLF)
	EndIf
	$hSubmenu=_GUICtrlMenu_GetItemSubMenu($hFile, 9)									; Fetch the handle of CAN->Msg Window menu
	ConsoleWrite("$hSubmenu handle : "&$hSubmenu& @CRLF)
	$res=_GUICtrlMenu_GetItemChecked($hSubmenu,7)                                       ;Check whether Interpret mode is enabled
	Sleep(1000)
	$text=_GUICtrlMenu_GetItemText($hSubmenu,7)
	ConsoleWrite("$text"& $text & @CRLF)
	ConsoleWrite("res" & $res & @CRLF)
	If($res=True) Then
		$OvrWriteToolhWd=ControlGetHandle($WIN_BUSMASTER,"",$ToolBar_OverWrite)			; Get handle of the 'Connect/Disconnect' toolbar
		_GUICtrlToolbar_ClickIndex($OvrWriteToolhWd,2)									; Click on 'Connect' icon
		sleep(1000)
	EndIf
EndFunc


;===============================================================================
;Function Name : _EnableHex
;Functionality : Enables Hex mode
;Input 		   :
;Output 	   :
;===============================================================================
Func _EnableHex()
	Send("!V")																				; Select View the menu
	sleep(750)
	send("{ESC}")
	if winexists($WIN_CANMsgWind) Then
		$winhWnd = WinGetHandle($WIN_BUSMASTER)												; Fetch the window handle
		$hMain = _GUICtrlMenu_GetMenu($winhWnd)												; Fetch the handle of the menu
		ConsoleWrite("menu handle : "&$hMain& @CRLF)
		$hFile = _GUICtrlMenu_GetItemSubMenu($hMain, 6)										; Fetch the handle of View menu
		ConsoleWrite("Submenu handle : "&$hFile& @CRLF)
		$res1=_GUICtrlMenu_GetItemChecked($hFile,1)											; Check whether View->Hex is checked or not
		$val=_GUICtrlMenu_GetItemText($hFile,1)												; Fetch the text of first item in View->Hex
		ConsoleWrite("$res1 : "&$res1& @CRLF)
		ConsoleWrite("$val : "&$val& @CRLF)
		if $res1=False then																	; If View->Hex is disabled then check it
			$OvrWriteToolhWd=ControlGetHandle($WIN_BUSMASTER,"",$ToolBar_OverWrite)			; Get handle of the Message Display' toolbar
			_GUICtrlToolbar_ClickIndex($OvrWriteToolhWd,3)									; Click on 'Connect' icon
			sleep(1000)
		EndIf
	Else
		$winhWnd = WinGetHandle($WIN_BUSMASTER)												; Fetch the window handle
		$hMain = _GUICtrlMenu_GetMenu($winhWnd)												; Fetch the handle of the menu
		ConsoleWrite("menu handle : "&$hMain& @CRLF)
		$hFile = _GUICtrlMenu_GetItemSubMenu($hMain, 3)										; Fetch the handle of View menu
		$item_txt=_GUICtrlMenu_GetItemText($hMain,3)
		ConsoleWrite("item text :" & $item_txt)
		ConsoleWrite("Submenu handle : "&$hFile& @CRLF)
		$res1=_GUICtrlMenu_GetItemChecked($hFile,1)											; Check whether View->Hex is checked or not
		$val=_GUICtrlMenu_GetItemText($hFile,1)												; Fetch the text of first item in View->Hex
		ConsoleWrite("$res1 : "&$res1& @CRLF)
		ConsoleWrite("$val : "&$val& @CRLF)
		if $res1=False then																	; If View->Hex is disabled then check it
			$OvrWriteToolhWd=ControlGetHandle($WIN_BUSMASTER,"",$ToolBar_OverWrite)			; Get handle of the 'Message Display' toolbar
			_GUICtrlToolbar_ClickIndex($OvrWriteToolhWd,3)									; Click on 'Hex/Decimal Display' icon
			sleep(1000)
		EndIf
	EndIf
EndFunc



;===============================================================================
;Function Name : _DisableHex
;Functionality : Disables Hex mode
;Input 		   :
;Output 	   :
;===============================================================================
Func _DisableHex()
	Send("!V")																				; Select View the menu
	sleep(750)
	send("{ESC}")
	if winexists($WIN_CANMsgWind) Then
		$winhWnd = WinGetHandle($WIN_BUSMASTER)												; Fetch the window handle
		$hMain = _GUICtrlMenu_GetMenu($winhWnd)												; Fetch the handle of the menu
		ConsoleWrite("menu handle : "&$hMain& @CRLF)
		$hFile = _GUICtrlMenu_GetItemSubMenu($hMain, 5)										; Fetch the handle of View menu
		ConsoleWrite("Submenu handle : "&$hFile& @CRLF)
		$res1=_GUICtrlMenu_GetItemChecked($hFile,1)											; Check whether View->Hex is checked or not
		$val=_GUICtrlMenu_GetItemText($hFile,1)												; Fetch the text of first item in View->Hex
		ConsoleWrite("$res1 : "&$res1& @CRLF)
		ConsoleWrite("$val : "&$val& @CRLF)
		if $res1=True then																	; If View->Hex is disabled then check it
			$OvrWriteToolhWd=ControlGetHandle($WIN_BUSMASTER,"",$ToolBar_OverWrite)			; Get handle of the Message Display' toolbar
			_GUICtrlToolbar_ClickIndex($OvrWriteToolhWd,3)									; Click on 'Connect' icon
			sleep(1000)
		EndIf
	Else
		$winhWnd = WinGetHandle($WIN_BUSMASTER)												; Fetch the window handle
		$hMain = _GUICtrlMenu_GetMenu($winhWnd)												; Fetch the handle of the menu
		ConsoleWrite("menu handle : "&$hMain& @CRLF)
		$hFile = _GUICtrlMenu_GetItemSubMenu($hMain, 4)										; Fetch the handle of View menu
		$item_txt=_GUICtrlMenu_GetItemText($hMain,3)
		ConsoleWrite("item text :" & $item_txt)
		ConsoleWrite("Submenu handle : "&$hFile& @CRLF)
		$res1=_GUICtrlMenu_GetItemChecked($hFile,1)											; Check whether View->Hex is checked or not
		$val=_GUICtrlMenu_GetItemText($hFile,1)												; Fetch the text of first item in View->Hex
		ConsoleWrite("$res1 : "&$res1& @CRLF)
		ConsoleWrite("$val : "&$val& @CRLF)
		if $res1=True then																	; If View->Hex is disabled then check it
			$OvrWriteToolhWd=ControlGetHandle($WIN_BUSMASTER,"",$ToolBar_OverWrite)			; Get handle of the 'Message Display' toolbar
			_GUICtrlToolbar_ClickIndex($OvrWriteToolhWd,3)									; Click on 'Hex/Decimal Display' icon
			sleep(1000)
		EndIf
	EndIf
EndFunc





;===============================================================================
;Function Name : _CANNodeSimToolbarStatus
;Functionality : Checks whether CAN node simulation toolbar is active
;Input 		   :
;Output 	   : If active 1 else 0
;===============================================================================
Func _CANNodeSimToolbarActive()
	if winexists($WIN_CANMsgWind) Then
		$winhWnd = WinGetHandle($WIN_BUSMASTER)												; Fetch the window handle
		$hMain = _GUICtrlMenu_GetMenu($winhWnd)												; Fetch the handle of the menu
		ConsoleWrite("menu handle : "&$hMain& @CRLF)
		$hFile = _GUICtrlMenu_GetItemSubMenu($hMain, 6)										; Fetch the handle of View menu
		ConsoleWrite("Submenu handle : "&$hFile& @CRLF)
		$hSubmenu=_GUICtrlMenu_GetItemSubMenu($hFile, 3)									; Fetch the handle of View->Toolbar menu
		ConsoleWrite("$hSubmenu handle : "&$hSubmenu& @CRLF)
		$hSubmenu1=_GUICtrlMenu_GetItemSubMenu($hFile, 3)									; Fetch the handle of View->Toolbar->CAN menu
		$hSubmenu2=_GUICtrlMenu_GetItemSubMenu($hSubmenu1, 3)								; Fetch the handle of View->Toolbar->CAN->Node Simulation menu
		send("!vtc")
		send("{ESC}")
		send("{ESC}")
		send("{ESC}")
		$res=_GUICtrlMenu_GetItemChecked($hSubmenu2,3)										; Check whether CAN->Message Window->Overwrite is checked or not
		$val=_GUICtrlMenu_GetItemText($hSubmenu2,3)											; Fetch the text of first item in CAN->Message Window
		ConsoleWrite("$res : "&$res& @CRLF)
		ConsoleWrite("$val : "&$val& @CRLF)
	Else
		$winhWnd = WinGetHandle($WIN_BUSMASTER)												; Fetch the window handle
		$hMain = _GUICtrlMenu_GetMenu($winhWnd)												; Fetch the handle of the menu
		ConsoleWrite("menu handle : "&$hMain& @CRLF)
		$hFile = _GUICtrlMenu_GetItemSubMenu($hMain, 5)										; Fetch the handle of View menu
		ConsoleWrite("Submenu handle : "&$hFile& @CRLF)
		$hSubmenu=_GUICtrlMenu_GetItemSubMenu($hFile, 3)									; Fetch the handle of View->Toolbar menu
		ConsoleWrite("$hSubmenu handle : "&$hSubmenu& @CRLF)
		$hSubmenu1=_GUICtrlMenu_GetItemSubMenu($hFile, 3)									; Fetch the handle of View->Toolbar->CAN menu
		$hSubmenu2=_GUICtrlMenu_GetItemSubMenu($hSubmenu1, 3)								; Fetch the handle of View->Toolbar->CAN->Node Simulation menu
		send("!vtc")
		send("{ESC}")
		send("{ESC}")
		send("{ESC}")
		$res=_GUICtrlMenu_GetItemChecked($hSubmenu2,3)										; Check whether CAN->Message Window->Overwrite is checked or not
		$val=_GUICtrlMenu_GetItemText($hSubmenu2,3)											; Fetch the text of first item in CAN->Message Window
		ConsoleWrite("$res : "&$res& @CRLF)
		ConsoleWrite("$val : "&$val& @CRLF)
	EndIf
	Return $res
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
	return $TrimmedPath

EndFunc

;===============================================================================
;Function Name : _TestDataPath
;Functionality : Sets path to TestData folder.
;Input 		   : -
;Output 	   : Returns the path of the folder
;===============================================================================
Func _TestDataPath()
	$CurrentDirPath = _SetOneFolderUp()
	ConsoleWrite($CurrentDirPath&@CRLF)
	$TestDataFPath=$CurrentDirPath &  "\" & "TestData"
	ConsoleWrite($TestDataFPath&@CRLF)
	Return $TestDataFPath
EndFunc
;===============================================================================
;Function Name : _OutputDataPath
;Functionality : Sets path to Output folder.
;Input 		   : -
;Output 	   : Returns the path of the folder
;===============================================================================
Func _OutputDataPath()
	$CurrentDirPath = @ScriptDir
	if fileexists($CurrentDirPath & "\" & "Output")=1 Then
	  ConsoleWrite("$isFolderExists=" & fileexists($CurrentDirPath & "\" & "Output") & @CRLF)
	  $folderPath=$CurrentDirPath & "\" & "Output"
	  return $folderPath
	Else
	  DirCreate($CurrentDirPath & "\" & "Output")
	  $folderPath=$CurrentDirPath & "\" & "Output"
	  return $folderPath
	EndIf
EndFunc

;=================================================================================
;Function Name : _GetMsgWinCANInfo
;Functionality : Fetch the message window CAN information
;Input 		   : Row No.
;Output 	   : Returns the Message window data to calling function
;===============================================================================
Func _GetMsgWinCANInfo($Row)
	local $Data[10]=["","","","","","","","","",""]
		ControlFocus($WIN_BUSMASTER,"",$LSTC_CANMsgWin)
		$Count=_GetCANMsgWinItemCount()																; Fetch the item count in the msg window
		consolewrite("$Count :"&$Count&@CRLF)
		if $Row<=$Count Then
			$HWD= ControlGetHandle($WIN_BUSMASTER,"",$LSTC_CANMsgWin)
			$MsgEntry=_GUICtrlListView_GetItemTextString($HWD,$Row)							; Fetch the Msg window row item text
			ConsoleWrite("$MsgEntry:"&$MsgEntry&@CRLF)
			$Msg=StringTrimLeft($MsgEntry,1)												; Remove the first character
			ConsoleWrite("$Msg:"&$Msg&@CRLF)
			$RepMsg=StringReplace($Msg,"|"," ")
			ConsoleWrite("$RepMsg:"&$RepMsg&@CRLF)
			$SplitMsg=StringSplit($RepMsg," ")
			ConsoleWrite("$SplitMsg:"&$SplitMsg[1]&@CRLF)
			$MsgTime_MsgWin=$SplitMsg[1]													; Fetch the time
			ConsoleWrite("$MsgTime_MsgWin:"&$MsgTime_MsgWin&@CRLF)
			$MsgDir_MsgWin=$SplitMsg[2]														; Fetch the Direction
			ConsoleWrite("$MsgDir_MsgWin:"&$MsgDir_MsgWin&@CRLF)
			$MsgCh_MsgWin=$SplitMsg[3]														; Fetch the Channel
			ConsoleWrite("$MsgCh_MsgWin:"&$MsgCh_MsgWin&@CRLF)
			$MsgType_MsgWin=$SplitMsg[4]													; Fetch the Msg Type
			ConsoleWrite("$MsgType_MsgWin:"&$MsgType_MsgWin&@CRLF)
			$MsgID_MsgWin=$SplitMsg[5]														; Fetch the Msg ID
			ConsoleWrite("$MsgID_MsgWin:"&$MsgID_MsgWin&@CRLF)
			$MsgName_MsgWin=$SplitMsg[6]													; Fetch the Msg Name
			ConsoleWrite("$MsgName_MsgWin:"&$MsgName_MsgWin&@CRLF)
			$MsgDLC_MsgWin=$SplitMsg[7]														; Fetch the DLC
			ConsoleWrite("$MsgDLC_MsgWin:"&$MsgDLC_MsgWin&@CRLF)
			$MsgData_MsgWin=StringSplit($Msg,"|")											; Fetch the Data
			ConsoleWrite("$FourthMsgData_MsgWin:"&$MsgData_MsgWin[8]&@CRLF)
			$MsgDataRemSpace_MsgWin=StringStripWS($MsgData_MsgWin[8],8)						; Remove the Spaces
			ConsoleWrite("$MsgDataRemSpace_MsgWin:"&$MsgDataRemSpace_MsgWin&@CRLF)
		EndIf
		$Data[0]=$MsgTime_MsgWin
		$Data[1]=$MsgDir_MsgWin
		$Data[2]=$MsgCh_MsgWin
		$Data[3]=$MsgType_MsgWin
		$Data[4]=$MsgID_MsgWin
		$Data[5]=$MsgName_MsgWin
		$Data[6]=$MsgDLC_MsgWin
		$Data[7]=$MsgDataRemSpace_MsgWin
		Return $Data
EndFunc

Func _GetDBConfiMsgDisp($row)
	Local $Data[5]=["","","","",""]
	if WinExists("Configure Message Display- CAN","")=0 Then
		WinMenuSelectItem($WIN_BUSMASTER,"",$CANMenu,$CANMsgWinMenu,$CANMsgDispConfigMenu)
		Sleep(1000)
	EndIf
	$hWndConfigMsgDisp=controlgethandle($WIN_MsgDisplay,"",$TabConfigMsgDisp)                    ;Get handle for configure message display
	_GUICtrlTab_ClickTab($hWndConfigMsgDisp, 1)
	$handle=ControlGetHandle($WIN_MsgDisplay,"",$LSTDBMsgs)                                      ;Get handle for DB message list view
	$count=_GUICtrlListView_GetItemCount($handle)
	$text=_GUICtrlListView_GetItemTextString($handle,$row)
	ConsoleWrite("$text" & $text & @CRLF)
	ConsoleWrite("$count"& $count & @CRLF)
	$textTrim=StringTrimLeft($text,2)                                                        ; Remove the first two characters
	$textRep=StringReplace($textTrim,"|"," ")
	;ConsoleWrite("Replace text" & $textRep & @CRLF)
	$textSplit=StringSplit($textRep," ")                                                     ;Fetch message id
	$id=$textSplit[1]
	$msgText=$textSplit[2]
;	ConsoleWrite("id" & $id & @CRLF)
;ConsoleWrite("$text" & $msgText & @CRLF)
	$Data[0]=$id
	$Data[1]=$msgText
	Return $Data
EndFunc


Func _GetDBConfigMsgDispCount()
	WinMenuSelectItem($WIN_BUSMASTER,"",$CANMenu,$CANMsgWinMenu,$CANMsgDispConfigMenu)
	Sleep(1000)
	$hWndConfigMsgDisp=controlgethandle($WIN_MsgDisplay,"",$TabConfigMsgDisp)                    ;Get handle for configure message display
	_GUICtrlTab_ClickTab($hWndConfigMsgDisp, 1)
	$handle=ControlGetHandle($WIN_MsgDisplay,"",$LSTDBMsgs)                                  ;Get handle for DB message list view in configure message display
	$count=_GUICtrlListView_GetItemCount($handle)                                           ;Get count of DB messages list view
	send("{ESC}")
	Return $count
EndFunc

Func _GetNonDBConfigMsgDispCount()
	WinMenuSelectItem($WIN_BUSMASTER,"",$CANMenu,$CANMsgWinMenu,$CANMsgDispConfigMenu)
	Sleep(1000)
	$hWndConfigMsgDisp=controlgethandle($WIN_MsgDisplay,"",$TabConfigMsgDisp)                    ;Get handle for configure message display
	_GUICtrlTab_ClickTab($hWndConfigMsgDisp, 2)
	$handle=ControlGetHandle($WIN_MsgDisplay,"",$LSTNonDBMsgs)                                  ;Get handle for DB message list view in configure message display
	$count=_GUICtrlListView_GetItemCount($handle)                                           ;Get count of DB messages list view
	send("{ESC}")
	Return $count
EndFunc

;===============================================================================
;Function Name : _CANNWStatsMenu
;Functionality : Selects CAN->Network Statistics
;Input 		   :
;Output 	   :
;===============================================================================
Func _CANNWStatsMenu()
	WinMenuSelectItem($WIN_BUSMASTER,"",$CANMenu,$CANNWStatsMenu)						; Select CAN->Network Statistics
	sleep(1000)
EndFunc

;=================================================================================
;Function Name : _GetNWStatsInfo
;Functionality : Fetch the network statistics information
;Input 		   : Row No.
;Output 	   : Returns the network statistics data to calling function
;===============================================================================
Func _GetNWStatsInfo($Row)
	Local $Info[10]=["",0,0,"","","","","","",""]

	;---Updated below scripts to access Network statistics window since it is implemented as child window ----

	Opt("WinDetectHiddenText", 0)
	;$HWD= ControlGetHandle($WIN_NW_Stats,"",$LVC_Details_NW_Stats)
	Opt("WinSearchChildren", 1)
	Opt("WinTitleMatchMode", 1)
	WinWait("Network Statistics")
	$HWD = ControlGetHandle($WIN_NW_Stats,"",$LVC_Details_NW_Stats)


	;----------------------------------------------------------------------------
	if _GUICtrlListView_GetColumnCount($HWD)=3 Then
		$RowInfo=_GUICtrlListView_GetItemTextString($HWD,$Row)							; Fetch the NW statistics item text
		ConsoleWrite("$RowInfo:"&$RowInfo&@CRLF)
		$SplitInfo=StringSplit($RowInfo,"|")											; Split the text w.r.t delimiter '|'
		$Info[0]=StringStripWS($SplitInfo[1],8)											; Remove spaces from the "Parameter" row text
		$Info[1]=$SplitInfo[2]
		$Info[2]=$SplitInfo[3]
		ConsoleWrite("parameter:"&$Info[0]&@CRLF)
		ConsoleWrite("channel1:"&$Info[1]&@CRLF)
		ConsoleWrite("channel2:"&$Info[2]&@CRLF)
	EndIf
	Return $Info																		; Return the Data to the calling script
EndFunc

;=================================================================================
;Function Name : _GetReplayWinInfo
;Functionality : Fetch the replay window information
;Input 		   : Row No.
;Output 	   : Returns the Message window data to calling function
;===============================================================================
Func _GetReplayWinInfo($Row)
	$MsgEntry=""
	$HWD= ControlGetHandle($WIN_BUSMASTER,"",$LVC_ReplayWin)							; Fetch the handle of replay list view instance
	$RepWinMsgCount=_GUICtrlListView_GetItemCount($HWD)									; Fetch the Msg window count
	consolewrite("$RepWinMsgCount :"&$RepWinMsgCount&@CRLF)
	if $Row<=$RepWinMsgCount Then
		$MsgEntry=_GUICtrlListView_GetItemTextString($HWD,$Row)							; Fetch the Msg window row item text
	EndIf
	Return $MsgEntry
EndFunc

;=================================================================================
;Function Name : _GetTraceWinInfo
;Functionality : Fetch the trace window information
;Input 		   : Row No.
;Output 	   : Returns the trace window data to calling function
;===============================================================================
Func _GetTraceWinInfo($iIndex)
	Opt("WinDetectHiddenText", 0)
	Opt("WinSearchChildren", 1)
	Opt("WinTitleMatchMode", 1)
	$TraceHWD=ControlGetHandle("Trace Window","",$LSTB_Details_TraceWin)
	$Info=_GUICtrlListBox_GetText($TraceHWD, $iIndex)
;~ 	ConsoleWrite("$Info: "&$Info&@CRLF)
	Return $Info
EndFunc
;=================================================================================
;Function Name : _GetTraceWinItemCount
;Functionality : Fetch the count or no of items in trace window
;Input 		   :
;Output 	   : Returns the count
;===============================================================================
Func _GetTraceWinItemCount()
	Opt("WinDetectHiddenText", 0)
	Opt("WinSearchChildren", 1)
	Opt("WinTitleMatchMode", 1)
	$TraceHWD=ControlGetHandle("Trace Window","",$LSTB_Details_TraceWin)
	$count=_GUICtrlListBox_GetCount($TraceHWD)
	sleep(1000)
	Return $count
EndFunc



;=================================================================================
;Function Name : _GetTestExecutorInfo
;Functionality : Fetch the test executor window information
;Input 		   : Row No.
;Output 	   : Returns the test executor window data to calling function
;===============================================================================
Func _GetTestExecutorInfo($Row)
	local $TExeInfo[10]=["","","","","","","","","",""]
	$TExeHWD= ControlGetHandle($WIN_BUSMASTER,"",$LVC_TestCaseResult_TestExe)
	$TExeRowInfo=_GUICtrlListView_GetItemTextString($TExeHWD,$Row)							; Fetch the TestExecutor item text
	ConsoleWrite("$RowInfo:"&$TExeRowInfo&@CRLF)
	$SplitInfo=StringSplit($TExeRowInfo,"|")											; Split the text w.r.t delimiter '|'
;~ 	$TExeInfo[0]=StringStripWS($SplitInfo[1],8)											; Remove spaces from the "Parameter" row text
	$TExeInfo[0]=$SplitInfo[1]
	$TExeInfo[1]=$SplitInfo[2]
	$TExeInfo[2]=$SplitInfo[3]
	Return $TExeInfo																	; Return the Data to the calling script
EndFunc

;===============================================================================
;Function Name : _CANNodeSimConfigMenu
;Functionality : Selects CAN->Node Simulation->Configure menu
;Input 		   :
;Output 	   :
;===============================================================================
Func _CANNodeSimConfigMenu()
	WinMenuSelectItem($WIN_BUSMASTER,"",$CANMenu,$CANNodeSimMenu,$CANNodeSimConfigMenu)		; Select CAN->Node Simulation->Configure from the menu
	sleep(1000)
EndFunc

;===============================================================================
;Function Name : _J1939NodeSimConfigMenu
;Functionality : Selects J1939->Node Simulation->Configure menu
;Input 		   :
;Output 	   :
;===============================================================================

Func _J1939NodeSimConfigMenu()
	WinMenuSelectItem($WIN_BUSMASTER,"",$J1939Menu,$J1939NodeSimMenu,$J1939NodeSimConfigMenu)		; Select J1939->Node Simulation->Configure from the menu
	sleep(1000)
EndFunc

;=================================================================================
;Function Name : _AddNode()
;Functionality : Adds a node to a sim file
;Input 		   : -
;Output 	   : -
;=================================================================================
Func _AddNode($nodeName)
	$SimTVHWD=_GetCANCppTVHWD()																; Fetch the handle of tree view
	$SimFileHWD = _GUICtrlTreeView_GetNext($SimTVHWD, "") 									; Get handle of "Sim File" child item
	_GUICtrlTreeView_ClickItem($SimTVHWD, $SimFileHWD, "right") 							; Right click on the DB path item
	sleep(800)
	send("a")																				; Select "Add" from the right click menu
	if WinWaitActive($WIN_NodeDetails,"",5) Then
		ControlSend($WIN_NodeDetails,"",$TXT_NodeName_NodeDetails,$nodeName)			   	; Enter Filename
		sleep(1000)
		ControlClick($WIN_NodeDetails,"",$BTN_OK_NodeDetails) 								; Click on OK button
		sleep(1000)
	EndIf
EndFunc
;=================================================================================
;Function Name : _AddNodeJ1939($nodeName,$PrefAddr,$ECUName)
;Functionality : Adds a node to a sim file in J1939
;Input 		   : Address and ECU Name
;Output 	   : -
;=================================================================================
Func _AddNodeJ1939($nodeName,$PrefAddr,$ECUName)
	$SimTVHWD=_GetCANCppTVHWD()																; Fetch the handle of tree view
	$SimFileHWD = _GUICtrlTreeView_GetNext($SimTVHWD, "") 									; Get handle of "Sim File" child item
	_GUICtrlTreeView_ClickItem($SimTVHWD, $SimFileHWD, "right") 							; Right click on the DB path item
	sleep(800)
	send("a")																				; Select "Add" from the right click menu
	if WinWaitActive($WIN_NodeDetails,"",5) Then
		ControlSend($WIN_NodeDetails,"",$TXT_NodeName_NodeDetails,$nodeName)
		sleep(500)
		ControlSetText($WIN_NodeDetails,"",$J1939TXT_PrefAddress,$PrefAddr)                  ;Enter Preffered Address
		sleep(500)
		ControlSend($WIN_NodeDetails,"",$J1939TXT_ECUName,$ECUName)                          ;Enter ECU name
		sleep(500)
		ControlClick($WIN_NodeDetails,"",$BTN_OK_NodeDetails) 								; Click on OK button
		sleep(1000)
	EndIf
	sleep(1000)
EndFunc




;=================================================================================
;Function Name : _GetCANCppTVHWD
;Functionality : Fetch the handle of the CAN Cpp tree view control
;Input 		   :
;Output 	   : Returns the handle of the CAN Cpp tree view control
;=================================================================================
Func _GetCANCppTVHWD()
	$nodeHWD = ControlGetHandle($WIN_BUSMASTER, "",$TVC_Handlers_CPP)  		; Fetch the handle of tree view control
	Return $nodeHWD
EndFunc

;=================================================================================
;Function Name : _GetJ1939CppTVHWD
;Functionality : Fetch the handle of the J1939 Cpp tree view control
;Input 		   :
;Output 	   : Returns the handle of the J1939 Cpp tree view control
;=================================================================================

Func _GetJ1939CppTVHWD()
	$nodeHWDJ1939= ControlGetHandle($WIN_BUSMASTER, "",$TVC_Handlers_CPPJ1939)
	Return $nodeHWDJ1939
EndFunc


;=================================================================================
;Function Name : _AddCANCppFile()
;Functionality : Adds the Cpp file
;Input 		   : Cpp File Name
;Output 	   : -
;=================================================================================
Func _AddCANCppFile($CppFileName)
	$CPPFilePath = _OutputDataPath()																; Fetch the path of the output data folder
	ControlClick($WIN_BUSMASTER,"",$BTN_AddCppFile_ConfigCANNodeSim)								; Click on the "Add New File" button
	WinWaitActive($WIN_AddCPPFile,"",5)																; Wait for the window to get active
	if WinExists($WIN_AddCPPFile) Then																; If window exits then enter the file name
		ControlSend($WIN_AddCPPFile,"",$TXT_FileName_AddCPPFile,$CPPFilePath&"\"&$CppFileName)   	; Enter Filename
		sleep(1000)
		ControlClick($WIN_AddCPPFile,"",$BTN_Open_AddCPPFile) 										; Click on Open button
		sleep(1500)
	EndIf
	sleep(2000)
EndFunc
;=================================================================================
;Function Name : _AddJ1939CppFile()
;Functionality : Adds the J1939 Cpp file
;Input 		   : Cpp File Name
;Output 	   : -
;=================================================================================
Func _AddJ1939CppFile($CppFileName)
	$CPPFilePath = _OutputDataPath()																; Fetch the path of the output data folder
	ControlClick($WIN_BUSMASTER,"",$BTN_AddCppFile_ConfigJ1939NodeSim)								; Click on the "Add New File" button
	If WinExists($WIN_BUSMASTER) Then
			ControlClick($WIN_BUSMASTER,"","&Yes")
	EndIf
	WinWaitActive($WIN_AddCPPFile,"",5)																; Wait for the window to get active
	if WinExists($WIN_AddCPPFile) Then																; If window exits then enter the file name
		ControlSend($WIN_AddCPPFile,"",$TXT_FileName_AddCPPFile,$CPPFilePath&"\"&$CppFileName)   	; Enter Filename
		sleep(1000)
		ControlClick($WIN_AddCPPFile,"",$BTN_Open_AddCPPFile) 										; Click on Open button
		sleep(1500)
;~ 		If WinExists($WIN_BUSMASTER) Then
;~ 			ControlClick($WIN_BUSMASTER,"","&Yes")
;~ 		EndIf
	EndIf
	sleep(1000)
	EndFunc
;=================================================================================
;Function Name : _AddJ1939CppFile()
;Functionality : Adds the J1939 Cpp file in TestData folder
;Input 		   : Cpp File Name
;Output 	   : -
;=================================================================================
Func _AddJ1939CppFile1($CppFileName)
	$CPPFilePath = _TestDataPath()																; Fetch the path of the output data folder
	ControlClick($WIN_BUSMASTER,"",$BTN_AddCppFile_ConfigJ1939NodeSim)								; Click on the "Add New File" button
	If WinExists($WIN_BUSMASTER) Then
			ControlClick($WIN_BUSMASTER,"","&Yes")
	EndIf
	WinWaitActive($WIN_AddCPPFile,"",5)																; Wait for the window to get active
	if WinExists($WIN_AddCPPFile) Then																; If window exits then enter the file name
		ControlSend($WIN_AddCPPFile,"",$TXT_FileName_AddCPPFile,$CPPFilePath&"\"&$CppFileName)   	; Enter Filename
		sleep(1000)
		ControlClick($WIN_AddCPPFile,"",$BTN_Open_AddCPPFile) 										; Click on Open button
		sleep(1500)
;~ 		If WinExists($WIN_BUSMASTER) Then
;~ 			ControlClick($WIN_BUSMASTER,"","&Yes")
;~ 		EndIf
	EndIf
	sleep(2000)
	EndFunc


;=================================================================================
;Function Name : _EditCANCppFile()
;Functionality : Clicks on the edit file button in the Configure Simulated System windows
;Input 		   : -
;Output 	   : -
;=================================================================================
Func _EditCANCppFile()
	ControlClick($WIN_BUSMASTER,"",$BTN_EditCppFile_ConfigCANNodeSim)								; Click on the "Edit File" button
EndFunc

;=================================================================================
;Function Name : _AddCANKeyHandler()
;Functionality : Adds the key handler for the specified Key
;Input 		   : Key
;Output 	   : -
;=================================================================================
Func _AddKeyHandler($Key)
	if WinExists($WIN_BUSMASTER) Then
		$TVHWD=_GetCANCppTVHWD()															; Fetch the handle of tree view
		$KeyHWD=_GetCANHandlerHWD(2,4)														; Fetch the handle of the key handler item
		_GUICtrlTreeView_ClickItem($TVHWD, $KeyHWD, "Right") 								; Right click on Key Handler item in the tree view
		send("a")																			; Select 'Add' from the right click menu
		if WinWaitActive($WIN_KeyValue,"",5) Then
			send($Key)																		; Select 'Add' from the right click menu
			ControlClick($WIN_KeyValue,"",$BTN_OK_KeyValue)									; Click on 'OK' button
		EndIf
	EndIf
EndFunc
;=================================================================================
;Function Name : _AddJ1939KeyHandler()
;Functionality : Adds the key handler for the specified Key
;Input 		   : Key
;Output 	   : -
;=================================================================================

Func _AddJ1939KeyHandler($key,$WinInfo)
	If WinExists($WinInfo) Then
		$TVHWD=_GetJ1939CppTVHWD()															; Fetch the handle of tree view
		$KeyHWD=_GetJ1939HandlerHWD(2,4)
		_GUICtrlTreeView_ClickItem($TVHWD, $KeyHWD, "Right") 							; Right click on Key Handler item in the tree view
		send($key)
     If WinWaitActive($WIN_KeyValue,"",5) Then
			send($Key)																		; Select 'Add' from the right click menu
			ControlClick($WIN_KeyValue,"",$BTN_OK_KeyValue)									; Click on 'OK' button
		EndIf
	EndIf
EndFunc


;=================================================================================
;Function Name : _AddCANTimeHandler()
;Functionality : Creates the Time handler
;Input 		   : Timer Value
;Output 	   : -
;=================================================================================
Func _AddTimeHandler($Timername,$Timer)
	if WinExists($WIN_BUSMASTER) Then
		$TVHWD=_GetCANCppTVHWD()															; Fetch the handle of tree view
		$TimeHWD=_GetCANHandlerHWD(2,3)														; Fetch the handle of the Time handler item
		_GUICtrlTreeView_ClickItem($TVHWD, $TimeHWD, "Right") 								; Right click on Time Handler item in the tree view
		send("a")																			; Select 'Add' from the right click menu
		if WinWaitActive($WIN_TimeHandler,"",5) Then
			ControlSend($WIN_TimeHandler,"",$TXT_THname_TimeHandler,$Timername)				; Enter the Time handler name
			sleep(500)
			ControlSetText($WIN_TimeHandler,"",$TXT_TValue_TimeHandler,$Timer)					; Enter the Timer Value
			sleep(500)
			ControlClick($WIN_TimeHandler,"",$BTN_OK_TimeHandler)							; Click on 'OK' button
		EndIf
	EndIf
EndFunc
;=================================================================================
;Function Name : _AddJ1939TimeHandler($name,$Timer)
;Functionality : Creates the Time handler for J1939
;Input 		   : Timer name and Timer Value
;Output 	   : -
;=================================================================================
Func _AddJ1939TimeHandler($name,$Timer)
	if WinExists($WIN_BUSMASTER) Then
		$TVHWD=_GetJ1939CppTVHWD()															; Fetch the handle of tree view
		$TimeHWD=_GetJ1939HandlerHWD(2,3)														; Fetch the handle of the Time handler item
		_GUICtrlTreeView_ClickItem($TVHWD, $TimeHWD, "Right") 								; Right click on Time Handler item in the tree view
		send("a")																			; Select 'Add' from the right click menu
		if WinWaitActive($WIN_TimeHandler,"",5) Then
			ControlSend($WIN_TimeHandler,"",$TXT_THname_TimeHandler,$name)				   ; Enter the Time handler name
			sleep(500)
			ControlSetText($WIN_TimeHandler,"",$TXT_TValue_TimeHandler,$Timer)					; Enter the Timer Value
			sleep(500)
			ControlClick($WIN_TimeHandler,"",$BTN_OK_TimeHandler)							; Click on 'OK' button
		EndIf
		sleep(1000)
	EndIf
EndFunc



;=================================================================================
;Function Name : _AddMsgHandler()
;Functionality : Adds the key handler for the specified Key
;Input 		   : SCase No,$Param1(MsgID, FromMsgID, MsgName),$Param2(ToMsgID)
;Output 	   : -
;=================================================================================
Func _AddMsgHandler($HSCaseNo,$Param1,$Param2)
	$TVHWD=_GetCANCppTVHWD()															; Fetch the handle of tree view
	$MsgHWD=_GetCANHandlerHWD(2,2)														; Fetch the handle of the Msg handler item
	_GUICtrlTreeView_ClickItem($TVHWD, $MsgHWD, "Right") 								; Right click on Msg Handler item in the tree view
	sleep(500)
	send("a")																			; Select 'Add' from the right click menu
	WinWaitActive($WIN_AddMsgHandler,"",5)
	Switch $HSCaseNo
	Case 1
		; MsgID Handler
		ControlSend($WIN_AddMsgHandler,"",$TXT_MsgID_AddMsgHandler,$Param1)								; Enter the Msg ID
		ControlClick($WIN_AddMsgHandler,"",$BTN_OK_AddMsgHandler)										; Click on 'OK' button
	Case 2
		; MsgID Range Handler
		ControlCommand($WIN_AddMsgHandler,"",$RBTN_MsgRange_AddMsgHandler,"Check")						; Check "MsgID Range" radio button
		sleep(1000)
		ControlSend($WIN_AddMsgHandler,"",$TXT_MsgIDFrom_AddMsgHandler,$Param1)							; Enter the Msg ID From
		sleep(1000)
		ControlSend($WIN_AddMsgHandler,"",$TXT_MsgIDTO_AddMsgHandler,$Param2)							; Enter the Msg ID To
		sleep(1000)
		ControlClick($WIN_AddMsgHandler,"",$BTN_OK_AddMsgHandler)										; Click on 'OK' button
	Case 3
		; MsgID All Handler
		ControlCommand($WIN_AddMsgHandler,"",$RBTN_MsgAll_AddMsgHandler,"Check")						; Check "MsgID ALL" radio button
		sleep(500)
		ControlClick($WIN_AddMsgHandler,"",$BTN_OK_AddMsgHandler)										; Click on 'OK' button
	Case 4
		; Msg Name Handler
		ControlCommand($WIN_AddMsgHandler,"",$RBTN_MsgName_AddMsgHandler,"Check")						; Check "Msg Name" radio button
		sleep(500)
		ControlCommand($WIN_AddMsgHandler,"",$LBC_MsgList_AddMsgHandler,"SelectString",$Param1)			; Select the msg from the list
		sleep(500)
		ControlClick($WIN_AddMsgHandler,"",$BTN_OK_AddMsgHandler)										; Click on 'OK' button
	EndSwitch
EndFunc
;=================================================================================
;Function Name : _AddEventHandlerJ1939()
;Functionality : Adds the event handler for a node in J1939 simulated systems
;Input 		   : $Event
;Output 	   : -
;=================================================================================
Func _AddEventHandlerJ1939($Event)
	if WinExists($WIN_BUSMASTER) Then
		    $TVHWD=_GetJ1939CppTVHWD()															; Fetch the handle of tree view
			$EventHWD=_GetJ1939HandlerHWD(2,5)														; Fetch the handle of the Time handler item
		     _GUICtrlTreeView_ClickItem($TVHWD, $EventHWD, "Right") 								; Right click on Time Handler item in the tree view
		     send("a")
		if WinWaitActive($WIN_EventHandler,"",7) Then
			If($Event="DataConfirmation") Then
			   ;ControlSend($WIN_EventHandler,"",4007,"Check")
			     ControlClick($WIN_EventHandler,"",4007)
			   sleep(500)
			   ;ControlClick($WIN_EventHandler,"",1)
		     ElseIf($Event="AddressClaim") Then
				;ControlSend($WIN_EventHandler,"",4008,"Check")
				ControlClick($WIN_EventHandler,"",4008)
			    ;ControlClick($WIN_EventHandler,"",1)
				sleep(500)
			EndIf
			 ControlClick($WIN_EventHandler,"",1)
		EndIf
	EndIf
EndFunc
;=================================================================================
;Function Name : _GetCANHandlerHWD
;Functionality : Fetch the handle of the CAN handlers in Cpp file
;Input 		   : Switch Case no., Child Item no.
;Output 	   : Returns the handle of the specified child item
;=================================================================================
Func _GetCANHandlerHWD($SCaseno,$ChildItemNo)
	$nodeHWD = ControlGetHandle($WIN_BUSMASTER, "",$TVC_Handlers_CPP)  		; Fetch the handle of tree view control
	Switch $SCaseno
		Case 1
			$item1HWD = _GUICtrlTreeView_GetNext($nodeHWD, "") 				; Get handle of "include header" child item
			return $item1HWD
		Case 2
			$item1HWD1 = _GUICtrlTreeView_GetNext($nodeHWD, "")				; Get handle of "include header" child item
			$var1=$item1HWD1
			for $i=1 to $ChildItemNo
				$itemHWD = _GUICtrlTreeView_GetNextSibling($nodeHWD, $var1) ; Get handle of $ChildItemNo
				_GUICtrlTreeView_ClickItem($nodeHWD, $var1)
				$var1=$itemHWD
			Next
			return $itemHWD
	EndSwitch
EndFunc

;=================================================================================
;Function Name : _ClickNode
;Functionality :Clicks node in simulated system
;Input 		   : $count
;Output 	   : clicks node at $count position
;=================================================================================

Func _ClickNode($count)
	$nodeHWD = _GetJ1939CppTVHWD()  		                        ; Fetch the handle of tree view control
	$Parent=_GUICtrlTreeView_GetParentHandle($nodeHWD)                ;Handle to the parent node
	$item1HWD = _GUICtrlTreeView_GetNext($nodeHWD, $Parent) 				; Get handle of "include header" child item
	$item=_GUICtrlTreeView_GetItemByIndex($nodeHWD,$item1HWD,$count)
	_GUICtrlTreeView_ClickItem($nodeHWD, $item)
	sleep(1000)
EndFunc

;=================================================================================
;Function Name : _GetJ1939HandlerHWD
;Functionality : Fetch the handle of the J1939 handlers in Cpp file
;Input 		   : Switch Case no., Child Item no.
;Output 	   : Returns the handle of the specified child item
;=================================================================================

Func _GetJ1939HandlerHWD($SCaseno,$ChildItemNo)
	$nodeHWD = ControlGetHandle($WIN_BUSMASTER, "",$TVC_Handlers_CPPJ1939)  		; Fetch the handle of tree view control
	Switch $SCaseno
		Case 1
			$item1HWD = _GUICtrlTreeView_GetNext($nodeHWD, "") 				 ; Get handle of "include header" child item
			return $item1HWD
		Case 2
			$item1HWD1 = _GUICtrlTreeView_GetNext($nodeHWD, "")				 ; Get handle of "include header" child item
			$var1=$item1HWD1
			for $i=1 to $ChildItemNo
				$itemHWD = _GUICtrlTreeView_GetNextSibling($nodeHWD, $var1)  ; Get handle of $ChildItemNo
				_GUICtrlTreeView_ClickItem($nodeHWD, $var1)
				$var1=$itemHWD
			Next
			return $itemHWD
	EndSwitch
EndFunc



;=================================================================================
;Function Name : _DelIncludeFile
;Functionality : Deletes the database path from the include headers list
;Input 		   : -
;Output 	   : -
;=================================================================================
Func _DelIncludeFile()
	$TVHWD=_GetCANCppTVHWD()															; Fetch the handle of tree view
	$IncHWD=_GetCANHandlerHWD(1,0)														; Fetch the handle of the include handler item
	$DBPathHWD=_GUICtrlTreeView_GetItemByIndex($TVHWD, $IncHWD, 2)						; Fetch the handle of the DB path item
	_GUICtrlTreeView_ClickItem($TVHWD, $DBPathHWD, "right") 							; Right click on the DB path item
	sleep(800)
	send("d")																			; Select "Delete" from the right click menu
	WinWaitActive($WIN_BUSMASTER,"",5)
	if WinExists($WIN_BUSMASTER,$TXT_DelIncF) Then
		ControlClick($WIN_BUSMASTER,"",$BTN_Yes_DelIncF)								; Click on 'Yes' button
	EndIf
EndFunc
;=================================================================================
;Function Name : _DelIncludeFileJ1939
;Functionality : Deletes the database path from the include headers list
;Input 		   : -
;Output 	   : -
;=================================================================================
Func _DelIncludeFileJ1939()
	$TVHWD=_GetJ1939CppTVHWD()															; Fetch the handle of tree view
	$IncHWD=_GetJ1939HandlerHWD(1,0)														; Fetch the handle of the include handler item
	$DBPathHWD=_GUICtrlTreeView_GetItemByIndex($TVHWD, $IncHWD, 2)						; Fetch the handle of the DB path item
	_GUICtrlTreeView_ClickItem($TVHWD, $DBPathHWD, "right") 							; Right click on the DB path item
	sleep(800)
	send("d")																			; Select "Delete" from the right click menu
	WinWaitActive($WIN_BUSMASTER,"",5)
	if WinExists($WIN_BUSMASTER,$TXT_DelIncF) Then
		ControlClick($WIN_BUSMASTER,"",$BTN_Yes_DelIncF)								; Click on 'Yes' button
	EndIf
EndFunc


;=================================================================================
;Function Name : _AddIncludeDBFile()
;Functionality : Adds the database file to the include header
;Input 		   : $IncDBFName
;Output 	   : -
;=================================================================================
Func _AddIncludeDBFile($IncDBFName)
	$IncDBFPath = _TestDataPath()														; Fetch the test data path
	$TVHWD=_GetCANCppTVHWD()															; Fetch the handle of tree view
	$IncHWD=_GetCANHandlerHWD(1,0)														; Fetch the handle of the include handler item
	_GUICtrlTreeView_ClickItem($TVHWD, $IncHWD, "right") 								; Right click on the DB path item
	sleep(800)
	send("a")																			; Select "Add" from the right click menu
	WinWaitActive($WIN_EnterHeaderF,"",5)
	if WinExists($WIN_EnterHeaderF) Then
		ControlSend($WIN_EnterHeaderF,"",$TXT_DBFPath_EnterHeaderF,""""&$IncDBFPath&"\"&$IncDBFName&"""")   	; Enter Filename
		sleep(1000)
		ControlClick($WIN_EnterHeaderF,"",$BTN_OK_EnterHeaderF)								; Click on 'Yes' button
	EndIf
EndFunc

;===============================================================================
;Function Name : _Click_BuildLoad
;Functionality : Clicks on 'Build and Load' button in Simulated System window
;Input 		   :
;Output 	   :
;===============================================================================
Func _Click_BuildLoad()
	ControlClick($WIN_BUSMASTER,"",$BTN_BuildLoad_ConfigNodeSim)										; Click on build and load button
	sleep(3500)
EndFunc

;===============================================================================
;Function Name : _BuildLoadError
;Functionality : Clicks on the 'OK' button of "The file is either not properly built or Loaded" dialog
;Input 		   :
;Output 	   :
;===============================================================================
Func _BuildLoadError()
	ControlClick($WIN_BUSMASTER,"",$BTN_OK_BuildLoadError)												; Fetch the handle of Tree view in CAN simulated systems window
EndFunc

;===============================================================================
;Function Name : _SelectHandler
;Functionality : Select the handlers in the 'Handler Details' section
;Input 		   :
;Output 	   :
;===============================================================================
Func _SelectHandler($Index)
	$HWD=ControlGetHandle($WIN_BUSMASTER,"",$LVC_HandlerDetails_ConfigCANNodeSim)						; Fetch the list view instance of handler details section
	_GUICtrlListView_ClickItem($HWD,$Index)																; Click on the specified handler index
	sleep(700)
	Click_EnableHandler()																				; Click on 'Enable Handler' button
	sleep(1000)
EndFunc
;===============================================================================
;Function Name : _SelectHandlerJ1939
;Functionality : Select the handlers in the 'Handler Details' section
;Input 		   :
;Output 	   :
;===============================================================================
Func _SelectHandlerJ1939($Index)
	$HWD=ControlGetHandle($WIN_BUSMASTER,"",$LVC_HandlerDetails_ConfigJ1939NodeSim)						; Fetch the list view instance of handler details section
	_GUICtrlListView_ClickItem($HWD,$Index)																; Click on the specified handler index
	sleep(700)
	Click_EnableHandler()																				; Click on 'Enable Handler' button
	sleep(1000)
EndFunc


;==========================================================================================
;Function Name :_EnableAllHandlersforNode
;Functionality : Enables all the handlers present for a node by clicking Enable All Handlers button in Configure Simulated systems window
;Input 		   :
;Output 	   :
;===========================================================================================
Func _EnableAllHandlersforNode()
	ControlClick($WIN_BUSMASTER,"",1015)
	sleep(1000)
EndFunc
;==========================================================================================
;Function Name :_EnableAllKeyHandlersforJ1939
;Functionality : Enables all the key handlers in J1939 simulated systems
;Input 		   :
;Output 	   :
;===========================================================================================
Func _EnableAllKeyHandlersforJ1939()
	WinMenuSelectItem($WIN_BUSMASTER,"",$J1939Menu,$J1939NodeSimMenu,"All &Key Handlers")
	sleep(1000)
EndFunc
;==========================================================================================
;Function Name :_EnableAllMsgHandlersJ1939
;Functionality : Enables all the message handlers in J1939 simulated systems
;Input 		   :
;Output 	   :
;===========================================================================================

Func _EnableAllMsgHandlersJ1939()
	WinMenuSelectItem($WIN_BUSMASTER,"",$J1939Menu,$J1939NodeSimMenu,"All &Message Handlers")
	sleep(1000)
EndFunc
;==========================================================================================
;Function Name :_EnableAllTimeHandlersJ1939
;Functionality : Enables all the Time handlers in J1939 simulated systems
;Input 		   :
;Output 	   :
;===========================================================================================
Func _EnableAllTimeHandlersJ1939()
	WinMenuSelectItem($WIN_BUSMASTER,"",$J1939Menu,$J1939NodeSimMenu,"All &Timer Handlers")
	sleep(1000)
EndFunc
;==========================================================================================
;Function Name :_EnableAllEventHandlersJ1939
;Functionality : Enables all the event handlers in J1939 simulated systems
;Input 		   :
;Output 	   :
;===========================================================================================
Func _EnableAllEventHandlersJ1939()
	WinMenuSelectItem($WIN_BUSMASTER,$J1939Menu,$J1939NodeSimMenu,"All &Event Handlers")
	sleep(1000)
EndFunc


;===============================================================================
;Function Name : Click_EnableHandler
;Functionality : Clicks on Enable Handler button in Simulated System window
;Input 		   :
;Output 	   :
;===============================================================================
Func Click_EnableHandler()
	ControlClick($WIN_BUSMASTER,"",$BTN_EnableHandler_ConfigCANNodeSim)									; Click on Enable handler button
EndFunc



;===============================================================================
;Function Name : _Click_Unload
;Functionality : Clicks on 'Unload' button in Simulated System window
;Input 		   :
;Output 	   :
;===============================================================================
Func _Click_Unload()
	ControlClick($WIN_BUSMASTER,"",$BTN_Unload_ConfigCANNodeSim)										; Click on Unload button
EndFunc

;===============================================================================
;Function Name : _BuildLoadALL
;Functionality : Clicks on 'Build and Load All' icon in node simulation tool bar
;Input 		   :
;Output 	   :
;===============================================================================
Func _BuildLoadALL()
	$NodeSimToolhWd=ControlGetHandle($WIN_BUSMASTER,"",$ToolBar_NodeSim)								; Get handle of the 'Node Simulation' toolbar
	_GUICtrlToolbar_ClickIndex($NodeSimToolhWd,$Icon_BuildLoadAll_Index)								; Click on 'Build and Load All' icon
EndFunc
;===============================================================================
;Function Name : _BuildLoadALL
;Functionality : Clicks on 'Build and Load All' icon in node simulation menu for J1939
;Input 		   :
;Output 	   :
;===============================================================================

Func _BuildLoadAllJ1939()
	WinMenuSelectItem($WIN_BUSMASTER,"",$J1939Menu,$J1939NodeSimMenu,"Build and L&oad All")           ;Click on Build and Load All through menu item
	sleep(1000)
EndFunc
;===============================================================================
;Function Name : _UnLoadALLJ1939
;Functionality : Clicks on 'UnLoad All' icon in node simulation menu for J1939
;Input 		   :
;Output 	   :
;===============================================================================

Func _UnLoadALLJ1939()
	WinMenuSelectItem($WIN_BUSMASTER,"",$J1939Menu,$J1939NodeSimMenu,"&Unload All")
	Sleep(1000)
EndFunc


;===============================================================================
;Function Name : _UnLoadALL
;Functionality : Clicks on 'UnLoad All' icon in node simulation tool bar
;Input 		   :
;Output 	   :
;===============================================================================
Func _UnLoadALL()
	$NodeSimToolhWd=ControlGetHandle($WIN_BUSMASTER,"",$ToolBar_NodeSim)								; Get handle of the 'Node Simulation' toolbar
	_GUICtrlToolbar_ClickIndex($NodeSimToolhWd,$Icon_UnloadAll_Index)									; Click on 'Unload All' icon
EndFunc

;===============================================================================
;Function Name : _AllKeyHandlers
;Functionality : Clicks on 'Enable/Disable All key handlers' icon in node simulation tool bar
;Input 		   :
;Output 	   :
;===============================================================================
Func _AllKeyHandlers()
	$NodeSimToolhWd=ControlGetHandle($WIN_BUSMASTER,"",$ToolBar_NodeSim)								; Get handle of the 'Node Simulation' toolbar
	_GUICtrlToolbar_ClickIndex($NodeSimToolhWd,$Icon_AllKeyHandlers_Index)								; Click on 'Enable/Disable All key handlers' icon
EndFunc

;===============================================================================
;Function Name : _DeleteMsgBlock
;Functionality :Deletes a Message Block in TX window
;Input 		   :
;Output 	   :
;===============================================================================

Func _DeleteMsgBlock()
	_TxMsgMenu()
	sleep(500)
	$handle=ControlGetHandle($WIN_BUSMASTER,"",$LSTC_MsgBlk_NameID)
$Count=_GUICtrlListView_GetItemCount($handle)
ConsoleWrite("Count :" & $Count)
If $Count>0 Then
For $i=0 To $Count-1
	ControlClick($WIN_BUSMASTER,"",$BTN_DeleteBlock_ConfigTX)
	sleep(1000)
	ControlClick("BUSMASTER","",$BTN_Yes_DeleteBlk)
	Sleep(1000)
	Next
	EndIf
 EndFunc


 ;===============================================================================
;Function Name : _AllKeyHandlers
;Functionality : Clicks on 'Enable/Disable Msg handlers' icon in node simulation tool bar
;Input 		   :
;Output 	   :
;===============================================================================
Func _MsgHandlers()
	$NodeSimToolhWd=ControlGetHandle($WIN_BUSMASTER,"",$ToolBar_NodeSim)								; Get handle of the 'Node Simulation' toolbar
	_GUICtrlToolbar_ClickIndex($NodeSimToolhWd,$Icon_MsgHandlers_Index)									; Click on 'Enable/Disable msg handlers' icon
EndFunc

;===============================================================================
;Function Name : _AllKeyHandlers
;Functionality : Clicks on 'Enable/Disable Msg handlers' icon in node simulation tool bar
;Input 		   :
;Output 	   :
;===============================================================================
Func _TimeHandlers()
	$NodeSimToolhWd=ControlGetHandle($WIN_BUSMASTER,"",$ToolBar_NodeSim)								; Get handle of the 'Node Simulation' toolbar
	_GUICtrlToolbar_ClickIndex($NodeSimToolhWd,$Icon_TimeHandlers_Index)									; Click on 'Enable/Disable Time handlers' icon
EndFunc

;===============================================================================
;Function Name : _HandleSaveSimSysWin
;Functionality : Clicks on 'No' button in Save Simulated System window
;Input 		   :
;Output 	   :
;===============================================================================
Func _HandleSaveSimSysWin()
	if WinWaitActive("",$TXT_SaveSimFile_BUSMASTER,3) Then
		ConsoleWrite("Save Simulated System Window Exists")
		ControlClick($WIN_BUSMASTER,$TXT_SaveSimFile_BUSMASTER,$BTN_No_SaveSimFile)
	EndIf
EndFunc
;===============================================================================
;Function Name : _ExecuteDLLHandler
;Functionality : Executes DLL Handler for CAN and J1939 depending on the value of parameter
;Input 		   :$Parameter
;Output 	   :
;===============================================================================
Func _ExecuteLoadDLLHandler($Parameter)
	Local $MsgHandler_Trace0=0,$MsgHandler_Trace7=0
	_ClickNode(0)                                                                        ;Click on the node in the tree view
	sleep(1000)
	If ($Parameter="ForJ1939") Then                                                       ;Execute for J1939 DLL handler
		_ClickNode(0)
		sleep(1000)
		_J1939NodeSimConfigMenu()
		sleep(1000)
		$TVHWD1=_GetJ1939CppTVHWD()	                                                           ; Fetch the handle of tree view
	   _GUICtrlTreeView_Expand($TVHWD1,"",True)													; Expand the tree view items
	    sleep(1000)
       _ClickNode(0)
        sleep(1000)
	     _AddJ1939CppFile("DLLHandlerNS_22_J1939.cpp")
		sleep(1000)
		$TVHWD=_GetJ1939CppTVHWD()															; Fetch the handle of tree view
    	$DLLHWD=_GetJ1939HandlerHWD(2,6)														; Fetch the handle of the DLL handler item
    	_GUICtrlTreeView_ClickItem($TVHWD, $DLLHWD, "Right") 								; Right click on DLL Handler item in the tree view
    	sleep(500)
    	send("a")
        WinWaitActive("Add DLL Handler(s)","",5)
        ControlCommand("Add DLL Handler(s)","",1164,"Check")
        sleep(500)
        ControlClick("Add DLL Handler(s)","",1178)
        $TVHWD=_GetJ1939CppTVHWD()
    	$DLLHWD=_GetJ1939HandlerHWD(2,6)
    	_GUICtrlTreeView_ClickItem($TVHWD, $DLLHWD)					 							; Click on DLL Handler item in the tree view

	Else                                                                                         ;For CAN DLL handler
		_CANNodeSimConfigMenu()
		 sleep(1000)
	     $TVHWD1=_GetCANCppTVHWD()	                                                           ; Fetch the handle of tree view
	    _GUICtrlTreeView_Expand($TVHWD1,"",True)													; Expand the tree view items
	    sleep(1000)
        _ClickNode(0)
        sleep(1000)
		_AddCANCppFile("DLLHandlerNS_CAN.cpp")
		sleep(1000)
	    $TVHWD=_GetCANCppTVHWD()															; Fetch the handle of tree view
	    $DLLHWD=_GetCANHandlerHWD(2,6)														; Fetch the handle of the DLL handler item
	    _GUICtrlTreeView_ClickItem($TVHWD, $DLLHWD, "Right") 								; Right click on DLL Handler item in the tree view
	    sleep(500)
       _GUICtrlTreeView_ClickItem($TVHWD, $DLLHWD, "Right") 								; Right click on DLL Handler item in the tree view
 	    sleep(500)
	    send("a")
		WinWaitActive("Add DLL Handler(s)","",5)
        ControlCommand("Add DLL Handler(s)","",1164,"Check")
        sleep(500)
	    ControlClick("Add DLL Handler(s)","",1178)
        $TVHWD=_GetCANCppTVHWD()
	    $DLLHWD=_GetCANHandlerHWD(2,6)
	   _GUICtrlTreeView_ClickItem($TVHWD, $DLLHWD)
	EndIf
	send("{DOWN}")																			; Send the down key to select the Msg handler function
	send("{ENTER}")
	ControlSend($WIN_BUSMASTER, "", $TXT_TimeHandler_CPPFile, "Trace"  & "("  &  Chr(34) & "Loaded" & Chr(34) & ");")
	Send("^s")																				; Save the cpp file
	Send("^{F4}") 																			; Close the Cpp file window
	sleep(500)
	_Click_BuildLoad()
	sleep(500)
	WinWaitActive("",$Msg_BuildLoadError,5)
	if winexists("",$Msg_BuildLoadError) Then
		_BuildLoadError()																	; Close the pop up msg
		$OutputwinHWD=ControlGetHandle($WIN_BUSMASTER,"",$LSTB_Details_TraceWin)			; get the handle of output window
		$itemCount=_GUICtrlListBox_GetCount($OutputwinHWD)									; Get the output window item count
		for $i=0 to $itemCount-1
			$outtxt[$i]=_GUICtrlListBox_GetText($OutputwinHWD, $i)							; Fetch the text in the output window
			ConsoleWrite("$outtxt["&$i&"]:"&$outtxt[$i]&@CRLF)
		Next
		send("^{F4}")																		 ; Close the output window
	EndIf
	send("^{F4}")
	$MsgCount=_GetTraceWinItemCount()													    ; Fetch the Trace window msg count
	If $MsgCount>0 Then
		$DLLHandler_Trace0=_GetTraceWinInfo(0)	                                            ; Fetch the trace window row info
		$DLLHandler_Trace1=_GetTraceWinInfo(1)
	    $DLLHandler_Trace2=_GetTraceWinInfo(2)
	    $DLLHandler_Trace3=_GetTraceWinInfo(3)
	    $DLLHandler_Trace4=_GetTraceWinInfo(4)
	    $DLLHandler_Trace5=_GetTraceWinInfo(5)
	    $DLLHandler_Trace6=_GetTraceWinInfo(6)
	    $DLLHandler_Trace7=_GetTraceWinInfo(7)
	    $DLLHandler_Trace8=_GetTraceWinInfo(8)
	    $DLLHandler_Trace9=_GetTraceWinInfo(9)
	    $DLLHandler_Trace10=_GetTraceWinInfo(10)
	    $DLLHandler_Trace11=_GetTraceWinInfo(11)
	    $DLLHandler_Trace12=_GetTraceWinInfo(12)
	EndIf

		ConsoleWrite("$DLLHandler_Trace0 " & $DLLHandler_Trace0 & @CRLF)
		ConsoleWrite("$DLLHandler_Trace7" & $DLLHandler_Trace7 & @CRLF)

	    If ($Parameter="ForJ1939") Then                                                      ;Check for J1939 Trace window info
			If $DLLHandler_Trace7=("Loaded") Then
				_WriteResult("Pass","TS_J1939Node_22")
			    ConsoleWrite("Pass for J1939" & @CRLF)
			Else
				_WriteResult("Fail","TS_J1939Node_22")
 		        ConsoleWrite("Fail for J1939" & @CRLF)
 	        EndIf
	    Else
			If $DLLHandler_Trace0="Loaded" Then                                             ;Check for CAN Trace window info
				_WriteResult("Pass","TS_NS_129")
				ConsoleWrite("Pass for CAN" & @CRLF)
 	        Else
				_WriteResult("Fail","TS_NS_129")
 		        ConsoleWrite("Fail for CAN" & @CRLF)
 	        EndIf
		EndIf

EndFunc

Func _ExecuteUnloadDLLHandler($Parameter)
	Local $DLLHandler_Trace0=0,$DLLHandler_Trace7=0
	_ClickNode(0)                                                                        ;Click on the node in the tree view
	sleep(1000)
	If ($Parameter="ForJ1939") Then                                                       ;Execute for J1939 DLL handler
		_ClickNode(0)
		sleep(1000)
		_J1939NodeSimConfigMenu()
		sleep(1000)
		$TVHWD1=_GetJ1939CppTVHWD()	                                                           ; Fetch the handle of tree view
	   _GUICtrlTreeView_Expand($TVHWD1,"",True)													; Expand the tree view items
	    sleep(1000)
       _ClickNode(0)
        sleep(1000)
	     _AddJ1939CppFile("DLLHandlerNS_24_J1939.cpp")
		sleep(1000)
		$TVHWD=_GetJ1939CppTVHWD()															; Fetch the handle of tree view
    	$DLLHWD=_GetJ1939HandlerHWD(2,6)														; Fetch the handle of the DLL handler item
    	_GUICtrlTreeView_ClickItem($TVHWD, $DLLHWD, "Right") 								; Right click on DLL Handler item in the tree view
    	sleep(500)
    	send("a")
        WinWaitActive("Add DLL Handler(s)","",5)
        ControlCommand("Add DLL Handler(s)","",1165,"Check")
        sleep(500)
        ControlClick("Add DLL Handler(s)","",1178)
        $TVHWD=_GetJ1939CppTVHWD()
    	$DLLHWD=_GetJ1939HandlerHWD(2,6)
    	_GUICtrlTreeView_ClickItem($TVHWD, $DLLHWD)					 							; Click on DLL Handler item in the tree view

	Else                                                                                         ;For CAN DLL handler
		_CANNodeSimConfigMenu()
		 sleep(1000)
	     $TVHWD1=_GetCANCppTVHWD()	                                                           ; Fetch the handle of tree view
	    _GUICtrlTreeView_Expand($TVHWD1,"",True)													; Expand the tree view items
	    sleep(1000)
        _ClickNode(0)
        sleep(1000)
		_AddCANCppFile("DLLHandlerNS_CAN_139.cpp")
		sleep(1000)
	    $TVHWD=_GetCANCppTVHWD()															; Fetch the handle of tree view
	    $DLLHWD=_GetCANHandlerHWD(2,6)														; Fetch the handle of the DLL handler item
	    _GUICtrlTreeView_ClickItem($TVHWD, $DLLHWD, "Right") 								; Right click on DLL Handler item in the tree view
	    sleep(500)
       _GUICtrlTreeView_ClickItem($TVHWD, $DLLHWD, "Right") 								; Right click on DLL Handler item in the tree view
 	    sleep(500)
	    send("a")
		WinWaitActive("Add DLL Handler(s)","",5)
        ControlCommand("Add DLL Handler(s)","",1165,"Check")
        sleep(500)
	    ControlClick("Add DLL Handler(s)","",1178)
        $TVHWD=_GetCANCppTVHWD()
	    $DLLHWD=_GetCANHandlerHWD(2,6)
	   _GUICtrlTreeView_ClickItem($TVHWD, $DLLHWD)
	EndIf
	send("{DOWN}")																			; Send the down key to select the Msg handler function
	send("{ENTER}")
	ControlSend($WIN_BUSMASTER, "", $TXT_TimeHandler_CPPFile, "Trace"  & "("  &  Chr(34) & "UnLoaded" & Chr(34) & ");")
	Send("^s")																				; Save the cpp file
	Send("^{F4}") 																			; Close the Cpp file window
	sleep(500)
	_Click_BuildLoad()
	sleep(500)
	WinWaitActive("",$Msg_BuildLoadError,5)
	if winexists("",$Msg_BuildLoadError) Then
		_BuildLoadError()																	; Close the pop up msg
		$OutputwinHWD=ControlGetHandle($WIN_BUSMASTER,"",$LSTB_Details_TraceWin)			; get the handle of output window
		$itemCount=_GUICtrlListBox_GetCount($OutputwinHWD)									; Get the output window item count
		for $i=0 to $itemCount-1
			$outtxt[$i]=_GUICtrlListBox_GetText($OutputwinHWD, $i)							; Fetch the text in the output window
			ConsoleWrite("$outtxt["&$i&"]:"&$outtxt[$i]&@CRLF)
		Next
		send("^{F4}")																		 ; Close the output window
	EndIf
	send("^{F4}")
	_Click_Unload()
	sleep(500)
	send("^{F4}")
	$MsgCount=_GetTraceWinItemCount()													    ; Fetch the Trace window msg count
	ConsoleWrite("Message count" & $MsgCount & @CRLF)
	If $MsgCount>0 Then
		$DLLHandler_Trace0=_GetTraceWinInfo(0)	                                            ; Fetch the trace window row info
		$DLLHandler_Trace1=_GetTraceWinInfo(1)
	    $DLLHandler_Trace2=_GetTraceWinInfo(2)
	    $DLLHandler_Trace3=_GetTraceWinInfo(3)
	    $DLLHandler_Trace4=_GetTraceWinInfo(4)
	    $DLLHandler_Trace5=_GetTraceWinInfo(5)
	    $DLLHandler_Trace6=_GetTraceWinInfo(6)
	    $DLLHandler_Trace7=_GetTraceWinInfo(7)
	    $DLLHandler_Trace8=_GetTraceWinInfo(8)
	    $DLLHandler_Trace9=_GetTraceWinInfo(9)
	    $DLLHandler_Trace10=_GetTraceWinInfo(10)
	    $DLLHandler_Trace11=_GetTraceWinInfo(11)
	    $DLLHandler_Trace12=_GetTraceWinInfo(12)
	EndIf

		ConsoleWrite("$DLLHandler_Trace0 " & $DLLHandler_Trace0 & @CRLF)
;~ 		ConsoleWrite("$DLLHandler_Trace1 " & $DLLHandler_Trace1 & @CRLF)
;~ 		ConsoleWrite("$DLLHandler_Trace2 " & $DLLHandler_Trace2 & @CRLF)
 		ConsoleWrite("$DLLHandler_Trace7" & $DLLHandler_Trace7 & @CRLF)

	    If ($Parameter="ForJ1939") Then                                                      ;Check for J1939 Trace window info
			If $DLLHandler_Trace7=("UnLoaded") Then
				_WriteResult("Pass","TS_J1939Node_24")
			    ConsoleWrite("Pass for J1939" & @CRLF)
			Else
				_WriteResult("Fail","TS_J1939Node_24")
 		        ConsoleWrite("Fail for J1939" & @CRLF)
 	        EndIf
	    Else
			If $DLLHandler_Trace0="UnLoaded" Then                                             ;Check for CAN Trace window info
				_WriteResult("Pass","TS_NS_139")
				ConsoleWrite("Pass for CAN" & @CRLF)
 	        Else
				_WriteResult("Fail","TS_NS_139")
 		        ConsoleWrite("Fail for CAN" & @CRLF)
 	        EndIf
		EndIf

EndFunc



;===============================================================================
;Function Name : _NewSimSysFile
;Functionality : Creates a new SIM file
;Input 		   : Sim File Name
;Output 	   :
;===============================================================================
Func _NewSimSysFile($SimFName)
	$TVHWD=ControlGetHandle($WIN_BUSMASTER, "",$TVC_Handlers_CPP)  							; Fetch the handle of tree view control
	$ItemCount=_GUICtrlTreeView_GetCount($TVHWD)											; Get the item count
	consolewrite("$ItemCount:"&$ItemCount&@CRLF)
	consolewrite("$TVHWD:"&$TVHWD&@CRLF)
	if $ItemCount=1 Then
		$PHWD=_GUICtrlTreeView_GetItemHandle($TVHWD,"")										; Fetch the first item handle
		consolewrite("$PHWD:"&$PHWD&@CRLF)
		_GUICtrlTreeView_ClickItem($TVHWD,$PHWD,"Right")									; Right click on "CAN" item
		sleep(500)
		send("n")																			; Select New Simulated System
		$SimFilePath=_OutputDataPath()														; Fetch the output data path
		if WinWaitActive($WIN_NewSimSys,"",5) Then
			ControlSend($WIN_NewSimSys,"",$TXT_SimFile_NewSimSys,$SimFilePath&"\"&$SimFName)	; Enter the file name
			Sleep(500)
			ControlClick($WIN_NewSimSys,"",$BTN_Save_NewSimSys)									; Click on 'Save' button
			sleep(500)
		EndIf
	EndIf
	Sleep(1000)
EndFunc


;===============================================================================
;Function Name 		: _AssocJ1939DB
;Functionality 		: This function Associates J1939 database
;Input 		   		: Database filename
;Output		   		:
;===============================================================================



Func _AssocJ1939DB($J1939DbName)
	WinMenuSelectItem($WIN_BUSMASTER,"",$J1939Menu,$J1939DBMenu,$J1939AssocDB)						; Select File->Database->Associate
	sleep(1000)
	$DBFolderPath = _TestDataPath()																  ; Fetch the TestData path to associate dbf file
	WinWaitActive($WIN_AssocJ1939DB,"",5)
	if winexists($WIN_AssocJ1939DB) Then
	ControlSend($WIN_AssocJ1939DB,"",$TXT_FileName_AssoJ1939DB,$DBFolderPath&"\"&$J1939DbName)    ; Set Filename
		sleep(1000)
	ControlClick($WIN_AssocJ1939DB,"",$BTN_Open_AssocDBJ1939) 					                  ; Click on open button
		sleep(1000)
	EndIf

	EndFunc

;==================================================================================
;Function Name : _ActivateJ1939()
;Functionality : Activates or Deactivates J1939 transmission
;Input 		   : $menu('&Activate' or 'Deac&tivate') is passed from calling script
;Output 	   : returns return value of 'WinMenuSelectItem'
;==================================================================================

Func _ActivateJ1939()
	If winexists($WIN_BUSMASTER) Then
		send("!j")
		sleep(500)
		send("{ESC}")
		sleep(500)
		$hWnd = WinGetHandle($WIN_BUSMASTER)
		$hMain = _GUICtrlMenu_GetMenu($hWnd)
		$hFile = _GUICtrlMenu_GetItemSubMenu($hMain, 2)
		$J1939State=_GUICtrlMenu_GetItemText($hFile, 0)												; Fetch the text of first menu in J1939
		ConsoleWrite("$J1939State :"&$J1939State)
		if $J1939State="&Activate" then																; If menu is activate then click on it
			WinMenuSelectItem($WIN_BUSMASTER,"",$J1939Menu,$J1939MsgWinAct)
			sleep(3000)
			_ClearMsgWindow()																		; Clear msg window
			sleep(1000)
		EndIf
	EndIf
EndFunc

;===============================================================================
;Function Name : _GoOnline()
;Functionality : Selects 'Go Online' menu
;Input 		   : -
;Output 	   : returns return value of 'WinMenuSelectItem'
;===============================================================================

Func _GoOnline()
	if winexists($WIN_BUSMASTER) Then
		sleep(500)
		WinMenuSelectItem($WIN_BUSMASTER,"",$J1939Menu,$GoOnlineJ1939)
		sleep(1000)
	EndIf
EndFunc
;==================================================================================
;Function Name : _DeactJ1939()
;Functionality : Activates or Deactivates J1939 transmission
;Input 		   : $menu('&Activate' or 'Deac&tivate') is passed from calling script
;Output 	   : returns return value of 'WinMenuSelectItem'
;==================================================================================


Func _DeactJ1939()
	If winexists($WIN_BUSMASTER) Then
		send("!j")
		sleep(500)
		send("{ESC}")
		WinMenuSelectItem($WIN_BUSMASTER,"",$J1939Menu,$J1939MsgWinDeact)
		sleep(1000)
	EndIf
EndFunc

;===============================================================================
;Function Name : _GoOffline()
;Functionality : Selects 'Go Online' menu
;Input 		   : -
;Output 	   : returns return value of 'WinMenuSelectItem'
;===============================================================================

Func _GoOffline()
	If winexists($WIN_BUSMASTER) Then
		sleep(500)
		WinMenuSelectItem($WIN_BUSMASTER,"",$J1939Menu,$GoOfflineJ1939)
		sleep(1000)
	EndIf
EndFunc



;================================================================================
;Function Name : _J1939tMsgWin
;Functionality : Configures 'Transport Protocol Function' for J1939 transmission.
;Input 		   : $msgType('R-Request PGN' or 'D-Data' or 'B-Broadcast') is passed from calling script.
;Output 	   : returns return value of 'WinMenuSelectItem'

;=================================================================================
;~ Func _J1939tMsgWin($msgType,$PGN)
;~ 	consolewrite("$msgType : "&$msgType&@CRLF)
;~ 	if winexists($WIN_BUSMASTER) Then
;~ 		sleep(1000)
;~ 		WinMenuSelectItem($WIN_BUSMASTER,"",$J1939Menu,$J1939TransmitMenu)                                   	;Select J1939 Transmit window
;~ 		ControlCommand($WIN_J1939Transmit,"",$CHKB_J1939Transport,"Check")                                    	;Select Transport Protocol Radio Button
;~ 		if $msgType="Request PGN" then
;~ 			ControlCommand($WIN_J1939Transmit,"",$J1939PGNctrlID,"SelectString",$PGN)		                 	;Select Request PGN name
;~ 			ControlCommand($WIN_J1939Transmit,"",$J1939MsgtypeCtrlID,"SelectString",$msgType)                  	;Select Message type as Request PGN
;~ 			ControlSetText($WIN_J1939Transmit,"",$J1939PriorityCtrlID,6)                                       	;Set the priority
;~ 		Elseif $msgType="Data" then
;~ 			ControlCommand($WIN_J1939Transmit,"",$J1939PGNctrlID,"SelectString",$PGN)		                  	;Select Data PGN name
;~ 			ControlCommand($WIN_J1939Transmit,"",$J1939MsgtypeCtrlID,"SelectString",$msgType)                  	;Select message type as DATA
;~ 			ControlSetText($WIN_J1939Transmit,"",$J1939DLC_CtrlID,8)                                           	;set DLC
;~ 			ControlSetText($WIN_J1939Transmit,"",$J1939DataCtrlID,"100000000000000000000000")                  	;Set Data bytes
;~ 			ControlSetText($WIN_J1939Transmit,"",$J1939PriorityCtrlID,7)                                        ;Set Priority
;~ 		ElseIf $msgType="Broadcast" Then
;~ 			ControlCommand($WIN_J1939Transmit,"",$J1939PGNctrlID,"SelectString",$PGN)			                  ;Select Broadcast PGN
;~ 			ControlCommand($WIN_J1939Transmit,"",$J1939MsgtypeCtrlID,"SelectString",$msgType)                   ;Select message type as BROADCAST
;~ 			ControlSetText($WIN_J1939Transmit,"",$J1939FromAddCtrlID,0)                                         ;Set From Address
;~ 			ControlSetText($WIN_J1939Transmit,"",$J1939ToAddCtrlID,"AA")                                        ;Set To Address
;~ 			ControlSetText($WIN_J1939Transmit,"",$J1939DLC_CtrlID,9)                                            ;Set DLC
;~ 			ControlSetText($WIN_J1939Transmit,"",$J1939DataCtrlID,"100000000000000000000000000000000000")       ;Set Databytes
;~ 			ControlSetText($WIN_J1939Transmit,"",$J1939PriorityCtrlID,7)                                        ;Set Priority
;~ 		ElseIf $msgType="Claim Address" Then
;~ 		    ControlCommand($WIN_J1939Transmit,"",$J1939NMCtrlID,"Check")                                        ;Select Network Management Radio Button
;~ 			ControlSetText($WIN_J1939Transmit,"",$J1939NMAddCtrlID,"FE")                                        ;Set Claim Address
;~ 		ElseIf $msgType="Request Address" Then
;~ 			 sleep(1000)
;~ 	        ControlCommand($WIN_J1939Transmit,"",$J1939NMCtrlID,"Check")                                       	;Select Network Management Radio Button
;~ 		   ;sleep(1000)
;~ 			ControlCommand($WIN_J1939Transmit,"",$CHKB_J1939ReqAdd,"Check")                                     ;Select Request Address Radio Button
;~ 			sleep(1000)
;~ 	    Else
;~ 			sleep(1000)
;~ 			ControlCommand($WIN_J1939Transmit,"",$J1939NMCtrlID,"Check")				                      	; Check 'Network Management' Radio Button
;~ 			sleep(500)
;~ 			ControlCommand($WIN_J1939Transmit,"",$CHKB_J1939CommAdd,"Check")				                  	; Check 'Command Address' Radio Button
;~ 			ControlSetText($WIN_J1939Transmit,"",$J1939NMAddCtrlID,"AA")
;~         EndIf
;~ 		sleep(1000)
;~ 		ControlClick($WIN_J1939Transmit,"",$BTN_J1939Transmit,"left")                                      		;Click on Transmit button
;~ 		sleep(1000)
;~ 		send("{TAB}")
;~ 		send("{ENTER}")
;~ 		sleep(1000)
;~ 	EndIf
;~ EndFunc

;==============================================================================================
;Function Name : _J1939tMsgWinForNodeSim
;Functionality : Configures 'Transport Protocol Function' for J1939 transmission.
;Input 		   : $msgType('R-Request PGN' or 'D-Data' or 'B-Broadcast') and FromAddr,ToAddr is passed from calling script.
;Output 	   : returns return value of 'WinMenuSelectItem'

;==============================================================================================



Func _J1939tMsgWin($msgType,$PGN,$FromAddr,$ToAddr)
	consolewrite("$msgType : "&$msgType&@CRLF)
	if winexists($WIN_BUSMASTER) Then
		sleep(1000)
		WinMenuSelectItem($WIN_BUSMASTER,"",$J1939Menu,$J1939TransmitMenu)                                   	;Select J1939 Transmit window
		ControlCommand($WIN_J1939Transmit,"",$CHKB_J1939Transport,"Check")                                    	;Select Transport Protocol Radio Button
		if $msgType="Request PGN" then
			ControlCommand($WIN_J1939Transmit,"",$J1939PGNctrlID,"SelectString",$PGN)                 	;Select Request PGN name
			ControlCommand($WIN_J1939Transmit,"",$J1939MsgtypeCtrlID,"SelectString",$msgType)                  	;Select Message type as Request PGN
			ControlSetText($WIN_J1939Transmit,"",$J1939PriorityCtrlID,6)                                       	;Set the priority
		Elseif $msgType="Data" then
			ControlCommand($WIN_J1939Transmit,"",$J1939PGNctrlID,"SelectString",$PGN)                  	;Select Data PGN name
			ControlCommand($WIN_J1939Transmit,"",$J1939MsgtypeCtrlID,"SelectString",$msgType)                  	;Select message type as DATA
			ControlSetText($WIN_J1939Transmit,"",$J1939DLC_CtrlID,8)                                           	;set DLC
			ControlSetText($WIN_J1939Transmit,"",$J1939DataCtrlID,"100000000000000000000000")                  	;Set Data bytes
			ControlSetText($WIN_J1939Transmit,"",$J1939PriorityCtrlID,7)
			ControlSetText($WIN_J1939Transmit,"",$J1939FromAddCtrlID,$FromAddr)                                         ;Set From Address
			ControlSetText($WIN_J1939Transmit,"",$J1939ToAddCtrlID,$ToAddr)
				ElseIf $msgType="Broadcast" Then
			ControlCommand($WIN_J1939Transmit,"",$J1939PGNctrlID,"SelectString",$PGN)                  ;Select Broadcast PGN
			ControlCommand($WIN_J1939Transmit,"",$J1939MsgtypeCtrlID,"SelectString",$msgType)                   ;Select message type as BROADCAST
			ControlSetText($WIN_J1939Transmit,"",$J1939FromAddCtrlID,$FromAddr)                                         ;Set From Address
			ControlSetText($WIN_J1939Transmit,"",$J1939ToAddCtrlID,$ToAddr)                                        ;Set To Address
			ControlSetText($WIN_J1939Transmit,"",$J1939DLC_CtrlID,9)                                            ;Set DLC
			ControlSetText($WIN_J1939Transmit,"",$J1939DataCtrlID,"100000000000000000000000000000000000")       ;Set Databytes
			ControlSetText($WIN_J1939Transmit,"",$J1939PriorityCtrlID,7)                                        ;Set Priority
		ElseIf $msgType="Claim Address" Then
		    ControlCommand($WIN_J1939Transmit,"",$J1939NMCtrlID,"Check")                                        ;Select Network Management Radio Button
			ControlSetText($WIN_J1939Transmit,"",$J1939NMAddCtrlID,"FE")                                        ;Set Claim Address
		ElseIf $msgType="Request Address" Then
			 sleep(1000)
	        ControlCommand($WIN_J1939Transmit,"",$J1939NMCtrlID,"Check")                                       	;Select Network Management Radio Button
		   ;sleep(1000)
			ControlCommand($WIN_J1939Transmit,"",$CHKB_J1939ReqAdd,"Check")                                     ;Select Request Address Radio Button
			sleep(1000)
	    Else
			sleep(1000)
			ControlCommand($WIN_J1939Transmit,"",$J1939NMCtrlID,"Check")				                      	; Check 'Network Management' Radio Button
			sleep(500)
			ControlCommand($WIN_J1939Transmit,"",$CHKB_J1939CommAdd,"Check")				                  	; Check 'Command Address' Radio Button
			ControlSetText($WIN_J1939Transmit,"",$J1939NMAddCtrlID,"AA")
        EndIf
		sleep(1000)
		ControlClick($WIN_J1939Transmit,"",$BTN_J1939Transmit,"left")                                      		;Click on Transmit button
		sleep(1000)
		send("{TAB}")
		send("{ENTER}")
		sleep(1000)
	EndIf
EndFunc


;=================================================================================
;Function Name : _GetJ1939MsgWinInfo
;Functionality : Fetch the message window J1939 information
;Input 		   : Row No.
;Output 	   : Returns the Message window data to calling function
;===============================================================================

Func _GetJ1939MsgWinInfo($Row)
	local $J1939Data[15]=["","","","","","","","","","","","","","",""]
	$hWnd=ControlGetHandle ($WIN_BUSMASTER,"",$LSTC_J1939MsgWin)                                    ;Fetch J1939 Window handle
	$J1939MsgEntry=_GUICtrlListView_GetItemTextString($hWnd,$Row)                                   ; Fetch the Msg window row item text
	$MsgJ1939Split=StringSplit($J1939MsgEntry,"|")
	$MsgJ1939Time=$MsgJ1939Split[2]                                                                ;Fetch the time
	ConsoleWrite("time " &$MsgJ1939Time & @CRLF)
	$MsgJ1939Channel=$MsgJ1939Split[3]                                                             ;Fetch Channel
	ConsoleWrite("Channel " &$MsgJ1939Channel & @CRLF)
	$MsgJ1939CANId=$MsgJ1939Split[4]                                                               ;Fetch CANID
	ConsoleWrite("CANID " &$MsgJ1939CANId & @CRLF)
	$MsgJ1939PGNname=$MsgJ1939Split[5]                                                             ;Fetch PGN name
	ConsoleWrite("PGN name " &$MsgJ1939PGNname & @CRLF)
	$MsgJ1939PGN=$MsgJ1939Split[6]                                                                 ;Fetch PGN
	ConsoleWrite("PGN " &$MsgJ1939PGN & @CRLF)
	$MsgJ1939Type=$MsgJ1939Split[7]                                                                ;Fetch Message Type
	ConsoleWrite("type " &$MsgJ1939Type & @CRLF)
	$MsgJ1939Src=$MsgJ1939Split[8]                                                                 ;Fetch Src Address
	ConsoleWrite("Src address " &$MsgJ1939Src & @CRLF)
	$MsgJ1939Dest=$MsgJ1939Split[9]                                                                ;Fetch Dest Address
	ConsoleWrite("Dest address " &$MsgJ1939Dest & @CRLF)
	$MsgJ1939Priority=$MsgJ1939Split[10]                                                           ;Fetch Priority
	ConsoleWrite("Priority " &$MsgJ1939Priority & @CRLF)
	$MsgJ1939TxRx=$MsgJ1939Split[11]                                                               ;Fetch TX/RX message
	ConsoleWrite("TX/RX msg " &$MsgJ1939TxRx & @CRLF)
	$MsgJ1939DLC=$MsgJ1939Split[12]                                                                 ;Fetch DLC
	ConsoleWrite("DLC " &$MsgJ1939DLC & @CRLF)
	$MsgJ1939Data=$MsgJ1939Split[13]                                                                ;Fetch Data
	$MSgJ1939DataRemSpace=StringStripWS($MsgJ1939Data,8)                                       		;Remove spaces from Data bytes
	ConsoleWrite("Data bytes " &$MsgJ1939Data & @CRLF)
	$J1939Data[0]=$MsgJ1939Time
	$J1939Data[1]=$MsgJ1939Channel
	$J1939Data[2]=$MsgJ1939CANId
	$J1939Data[3]=$MsgJ1939PGNname
	$J1939Data[4]=$MsgJ1939PGN
	$J1939Data[5]=$MsgJ1939Type
	$J1939Data[6]=$MsgJ1939Src
	$J1939Data[7]=$MsgJ1939Dest
	$J1939Data[8]=$MsgJ1939Priority
	$J1939Data[9]=$MsgJ1939TxRx
	$J1939Data[10]=$MsgJ1939DLC
	$J1939Data[11]=$MSgJ1939DataRemSpace
	Return $J1939Data
EndFunc

;===============================================================================
;Function Name : _OpenTestAutomationEditor
;Functionality : open test automation editor window
;Input 		   :
;Output 	   :
;===============================================================================

Func _OpenTestAutomationEditor()
	WinMenuSelectItem($WIN_BUSMASTER,"",$CANMenu,$TestAutomationMenu,$TestAutomationEditMenu)				; Select CAN->Test Automation->Editor
	sleep(1500)
EndFunc

;===============================================================================
;Function Name : _OpenTestAutomationExecutor
;Functionality : open test automation executor window
;Input 		   :
;Output 	   :
;===============================================================================

Func _OpenTestAutomationExecutor()
	WinMenuSelectItem($WIN_BUSMASTER,"",$CANMenu,$TestAutomationMenu,$TestAutomationExeMenu)				; Select CAN->Test Automation->Executor
	sleep(1500)
EndFunc

;===============================================================================
;Function Name : _TestExeTreeMenu
;Functionality : selects the specified menu from the right click context menu
;Input 		   :
;Output 	   :
;===============================================================================
Func _TestExeTreeMenu($Menu)
	$TEHWD=ControlGetHandle($WIN_BUSMASTER, "", $TVC_TestSuite_TestExecutor)
	$TestSuiteHWD=_GUICtrlTreeView_GetFirstItem($TEHWD)
	_GUICtrlTreeView_ClickItem($TEHWD, $TestSuiteHWD, "Right")
	sleep(700)
	Send($Menu)
EndFunc

;===============================================================================
;Function Name : _AddTestExeFile
;Functionality : Opens xml file for execution
;Input 		   : Xml file name
;Output 	   :
;===============================================================================
Func _AddTestExeFile($FName)
	_TestExeTreeMenu($TestExe_AddMenu)																		; Select 'add' from right click menu
	$XmlPath=_TestDataPath()
	if WinWaitActive($WIN_TestExeOpen,"",3) Then
		ControlSend($WIN_TestExeOpen,"",$TXT_XmlFPath_TestExeOpen,$XmlPath&"\"&$FName)						; Set the filename
		sleep(500)
		ControlClick($WIN_TestExeOpen,"",$BTN_Open_TestExeOpen)												; Click on Open button
		sleep(1200)
	EndIf
EndFunc


;===============================================================================
;Function Name : _GetIconComId
;Functionality : Retrieves the command identifier associated with the button
;Input 		   :
;Output 	   : returns the command ID
;===============================================================================
Func _GetIconComId($hWnd,$Index)
	$ComId=_GUICtrlToolbar_IndexToCommand($hWnd, $Index)
	Return $ComId
EndFunc

;===============================================================================
;Function Name : _IsIconEnabled
;Functionality : Checks whether the icon in the toolbar is enabled
;Input 		   :
;Output 	   : returns the BTN state
;===============================================================================
Func _IsIconEnabled($hWnd,$ComId)
	$BTNState=_GUICtrlToolbar_IsButtonEnabled($hWnd,$ComId)
	Return $BTNState
EndFunc

;===============================================================================
;Function Name : _ClickToolBarIcon
;Functionality : Checks whether the icon in the toolbar is enabled
;Input 		   :
;Output 	   : returns the BTN state
;===============================================================================
Func _ClickToolBarIcon($hWnd,$Index)
	_GUICtrlToolbar_ClickIndex($hWnd,$Index)
	sleep(500)
EndFunc

;===============================================================================
;Function Name : _FormatConverterMenu
;Functionality : open format converter from the menu
;Input 		   :
;Output 	   :
;===============================================================================
Func _FormatConverterMenu()
	WinMenuSelectItem($WIN_BUSMASTER,"",$fileMenu,$FomatConverterMenu)
EndFunc
;=================================================================================
;Function Name : Capl2CppConversion()
;Functionality : Converts CAPL .can file to CPP file
;Input 		   : -
;Output 	   : Creates Cpp file
;===============================================================================

Func Capl2CppConversion($InputCAPLFile,$OutputCPPFile,$DBC_CAPLFile,$DBF_CAPLFile)
	local $ConversionRes[10]
	_FormatConverterMenu()																						   ; Open format converters
    if @error Then
		ConsoleWriteError("Format Converter didn't run" & @CRLF)
		Exit
    EndIf
	WinWaitActive($WIN_FormatConv, "", 5)                                                                          ; BUSMASTER Format Conversions
	if @error Then
	    ConsoleWriteError("Format Conversions didn't activate" & @CRLF)
		Exit
    EndIf

	If winexists($WIN_FormatConv) then                                                                             ; test case for capl to cpp conversion
		$hWndCAPL2CPP=controlgethandle($WIN_FormatConv,"",$Tab_CAPL2CPP)
		_GUICtrlTab_ClickTab($hWndCAPL2CPP, 0)                                                                     ; Click on CAPL2CPP converter tab
		controlclick($WIN_FormatConv,"",$BTN_CaplInput)                                                            ; Click input
        WinWaitActive($WIN_SelectCAN_CAPL2CPP, "", 5)
        $InputCAPLPath=_TestDataPath()                                                                             ; Input file path
		sleep(1000)
		ControlSend($WIN_SelectCAN_CAPL2CPP,"",$TXT_Open_File_CAPL2CPP,$InputCAPLPath & "\" & $InputCAPLFile)      ; Send input file path to the control
		sleep(750)
	    ControlClick($WIN_SelectCAN_CAPL2CPP,"",$BTN_Open_File_CAPL2CPP)
		sleep(750)
		WinWaitClose($WIN_SelectCAN_CAPL2CPP, "", 3)
		sleep(750)
		controlclick($WIN_FormatConv,"",$BTN_CPPOut_CAPL2CPP)                                                      ; Click on output
		sleep(750)
		WinWaitActive($WIN_SelectCpp_CAPL2CPP, "", 5)
		sleep(750)
		$OutputFolderPath=_OutputDataPath()                                                                        ; Output file path
        ControlSend($WIN_SelectCpp_CAPL2CPP,"",$TXT_Open_File_CAPL2CPP,$OutputFolderPath & "\" & $OutputCPPFile)   ; Send output file path to the control
		sleep(500)
		ControlClick($WIN_SelectCpp_CAPL2CPP,"",$BTN_Open_File_CAPL2CPP)
		controlclick($WIN_FormatConv,"",$CHKB_SaveDB_CAPL2CPP)                                                     ; Select Save DB Checkbox
		sleep(500)
        controlclick($WIN_FormatConv,"",$BTN_Add_DBC_CAPL2CPP)                                                     ; Click add dbc file
        WinWaitActive($SelectDBC_CAPL2CPP, "", 5)
        $Capl2cppDbcFilepath=_TestDataPath()                                                                       ; DBC file path
        ControlSend($SelectDBC_CAPL2CPP,"",$TXT_Open_File_CAPL2CPP,$Capl2cppDbcFilepath & "\" & $DBC_CAPLFile)     ; Sends dbc file path to the control
        ControlClick($SelectDBC_CAPL2CPP,"",$BTN_Open_File_CAPL2CPP)
        $hWd=ControlGetHandle($WIN_FormatConv,"",$LST_DBCFiles)
        _GUICtrlListView_ClickItem($hWd,0)
		ControlClick($WIN_FormatConv,"",$BTN_Change_DBF_Path)			                                              ; click on change dbf button
		sleep(1000)
        $Capl2cppDbfFilepath=_OutputDataPath()                                                                        ;DBF file path
		ControlSend($ChangeDBCCAPL2CPP,"",$TXT_Open_File_CAPL2CPP,$Capl2cppDbfFilepath & "\" &$DBF_CAPLFile)          ;Send dbf file path to the control
        ControlClick($ChangeDBCCAPL2CPP,"",$BTN_Open_File_CAPL2CPP)
        controlclick($WIN_FormatConv,"",$BTN_Convert_CAPL2CPP)                                                        ;Click on Convert button
		sleep(4000)
		$ConversionRes[0]=ControlGetText ($WIN_FormatConv,"",$TXT_ConversionRes)								   ; Fetch the conversion result
		controlclick($WIN_FormatConv,"",$BTN_Report)                                                               ; open log file to view warnings if any
		sleep(2000)
		$LogfName=StringTrimRight  ($OutputCPPFile, 4)															   ; trim ".cpp" from output fname
		ConsoleWriteError("$LogfName : " & $LogfName & @CRLF)
		$ConversionRes[1]=FileReadLine ($Capl2cppDbfFilepath & "\" & $LogfName &"log.txt",1)						; Fetch the input fPath in the report
		$ConversionRes[2]=FileReadLine ($Capl2cppDbfFilepath & "\" & $LogfName &"log.txt",2)						; Fetch the output fPath in the report
		$ConversionRes[3]=FileReadLine ($Capl2cppDbfFilepath & "\" & $LogfName &"log.txt",6)						; Fetch the conversion reslut fPath in the report
		ConsoleWriteError("$ConversionRes[0] : " & $ConversionRes[0] & @CRLF)
		ConsoleWriteError("$ConversionRes[1] : " & $ConversionRes[1] & @CRLF)
		ConsoleWriteError("$ConversionRes[2] : " & $ConversionRes[2] & @CRLF)
		ConsoleWriteError("$ConversionRes[3] : " & $ConversionRes[3] & @CRLF)
		Sleep(500)
		WinClose($Capl2CppLogFile)                                                                                 ; closes log file
		Sleep(500)
		WinClose($WIN_FormatConv)                                                                                  ; Closes Format Converter window
	EndIf
	Return $ConversionRes
EndFunc
;=================================================================================
;Function Name : Capl2CppConversion1()
;Functionality : Converts CAPL .can file to CPP file and cpp file is saved in the same folder
;Input 		   : -
;Output 	   : Creates Cpp file in the same folder
;===============================================================================

Func Capl2CppConversion1($InputCAPLFile,$OutputCPPFile,$DBC_CAPLFile,$DBF_CAPLFile)
	local $ConversionRes[10]
	_FormatConverterMenu()																						   ; Open format converters
    if @error Then
		ConsoleWriteError("Format Converter didn't run" & @CRLF)
		Exit
    EndIf
	WinWaitActive($WIN_FormatConv, "", 5)                                                                          ; BUSMASTER Format Conversions
	if @error Then
	    ConsoleWriteError("Format Conversions didn't activate" & @CRLF)
		Exit
    EndIf

	If winexists($WIN_FormatConv) then                                                                             ; test case for capl to cpp conversion
		$hWndCAPL2CPP=controlgethandle($WIN_FormatConv,"",$Tab_CAPL2CPP)
		_GUICtrlTab_ClickTab($hWndCAPL2CPP, 0)                                                                     ; Click on CAPL2CPP converter tab
		controlclick($WIN_FormatConv,"",$BTN_CaplInput)                                                            ; Click input
        WinWaitActive($WIN_SelectCAN_CAPL2CPP, "", 5)
        $InputCAPLPath=_TestDataPath()                                                                             ; Input file path
		sleep(1000)
		ControlSend($WIN_SelectCAN_CAPL2CPP,"",$TXT_Open_File_CAPL2CPP,$InputCAPLPath & "\" & $InputCAPLFile)      ; Send input file path to the control
		sleep(750)
	    ControlClick($WIN_SelectCAN_CAPL2CPP,"",$BTN_Open_File_CAPL2CPP)
		sleep(750)
		WinWaitClose($WIN_SelectCAN_CAPL2CPP, "", 3)
		sleep(750)
		controlclick($WIN_FormatConv,"",$CHKB_SaveDB_CAPL2CPP)                                                     ; Select Save DB Checkbox
		sleep(500)
        controlclick($WIN_FormatConv,"",$BTN_Add_DBC_CAPL2CPP)                                                     ; Click add dbc file
        WinWaitActive($SelectDBC_CAPL2CPP, "", 5)
        $Capl2cppDbcFilepath=_TestDataPath()                                                                       ; DBC file path
        ControlSend($SelectDBC_CAPL2CPP,"",$TXT_Open_File_CAPL2CPP,$Capl2cppDbcFilepath & "\" & $DBC_CAPLFile)     ; Sends dbc file path to the control
        ControlClick($SelectDBC_CAPL2CPP,"",$BTN_Open_File_CAPL2CPP)
        $hWd=ControlGetHandle($WIN_FormatConv,"",$LST_DBCFiles)
        _GUICtrlListView_ClickItem($hWd,0)
		ControlClick($WIN_FormatConv,"",$BTN_Change_DBF_Path)			                                              ; click on change dbf button
		sleep(1000)
        $Capl2cppDbfFilepath=_TestDataPath()                                                                        ;DBF file path
		ControlSend($ChangeDBCCAPL2CPP,"",$TXT_Open_File_CAPL2CPP,$Capl2cppDbfFilepath & "\" &$DBF_CAPLFile)          ;Send dbf file path to the control
        ControlClick($ChangeDBCCAPL2CPP,"",$BTN_Open_File_CAPL2CPP)
        controlclick($WIN_FormatConv,"",$BTN_Convert_CAPL2CPP)                                                        ;Click on Convert button
		sleep(4000)
		$ConversionRes[0]=ControlGetText ($WIN_FormatConv,"",$TXT_ConversionRes)								   ; Fetch the conversion result
		controlclick($WIN_FormatConv,"",$BTN_Report)                                                               ; open log file to view warnings if any
		sleep(2000)
		$LogfName=StringTrimRight  ($OutputCPPFile, 4)															   ; trim ".cpp" from output fname
		ConsoleWriteError("$LogfName : " & $LogfName & @CRLF)
		$ConversionRes[1]=FileReadLine ($Capl2cppDbfFilepath & "\" & $LogfName &"log.txt",1)						; Fetch the input fPath in the report
		$ConversionRes[2]=FileReadLine ($Capl2cppDbfFilepath & "\" & $LogfName &"log.txt",2)						; Fetch the output fPath in the report
		$ConversionRes[3]=FileReadLine ($Capl2cppDbfFilepath & "\" & $LogfName &"log.txt",6)						; Fetch the conversion reslut fPath in the report
		ConsoleWriteError("$ConversionRes[0] : " & $ConversionRes[0] & @CRLF)
		ConsoleWriteError("$ConversionRes[1] : " & $ConversionRes[1] & @CRLF)
		ConsoleWriteError("$ConversionRes[2] : " & $ConversionRes[2] & @CRLF)
		ConsoleWriteError("$ConversionRes[3] : " & $ConversionRes[3] & @CRLF)
		WinClose($Capl2CppLogFile)                                                                                 ; closes log file
		WinClose($WIN_FormatConv)                                                                                  ; Closes Format Converter window

	Return $ConversionRes
	EndIf
EndFunc




;=================================================================================
;Function Name : _OthrConversionsFormatConv
;Functionality : Converts asc to log,dbc to dbf and vice versa .J1939 dbc to dbf
;Input 		   : -
;Output 	   : Creates corresponding output files
;==================================================================================


Func _OthrConversionsFormatConv($TXT_ConvType,$InputFile,$OutputFile)
		_FormatConverterMenu()                                                                              ;Invoke Format converters menu
    if @error Then
		ConsoleWriteError("Format Converter didn't run" & @CRLF)
	Exit
    EndIf
		WinWaitActive($WIN_FormatConv, "", 5)                                                             ;Wait for Format converter window
    if @error Then
		ConsoleWriteError("Format Conversions didn't activate" & @CRLF)
    Exit
    EndIf
    If winexists($WIN_FormatConv) then
		sleep(2000)
		$hWndOthrConverters=controlgethandle($WIN_FormatConv,"",$TAB_OtherConvts)                       ;Get handle of the Format converter window
	    _GUICtrlTab_ClickTab($hWndOthrConverters, 2)                                                     ; Select Other Converters tab
	sleep(2000)
		ControlCommand($WIN_FormatConv,"",$COMBO_SelectConvType,"SelectString",$TXT_ConvType)		       ; Select 'DBC to DBF Conversion' from combo box
		Sleep(1000)
        ControlClick($WIN_FormatConv,"",$BTN_Input)                                                        ; Click on "Input" button
		sleep(4000)


		WinWaitActive("", $Dlg_OpenOthrConv, 8)                                    						;wait for open dialog
		if WinActive($Dlg_OpenOthrConv) Then

			;Opt("WinDetectHiddenText", 0)
			;Opt("WinSearchChildren", 1)
			;Opt("WinTitleMatchMode", 1)
			;WinWait($Dlg_OpenOthrConv)
			;WinSetState($Dlg_OpenOthrConv,"",@SW_MAXIMIZE)

			$InputFilePath=_TestDataPath()
			;ControlSend($Dlg_OpenOthrConv,"",$TXT_Open_FilePath_OthrConv,$InputFilePath &"\"& $InputFile)      ;Send path of the input file

			Send($InputFilePath &"\"& $InputFile)
			;Send("{TAB}")
			;Send("password")
			Send("{TAB}")
			Send("{TAB}")
			;Send("{TAB}")
			Send("{ENTER}")


			sleep(1000)
			;ControlClick($Dlg_OpenOthrConv,"",$BTN_Open_FilePath_OthrConv)
			sleep(1000)
			ControlClick($WIN_FormatConv,"",$BTN_Output)                                                       ; Click on "Output" button
			sleep(1000)
			WinWaitActive("", $Dlg_OpenOthrConv, 5)                                     					   ; wait for open dialog
			$OutputFilePath= _OutputDataPath()
			;ControlSend($Dlg_OpenOthrConv,"",$TXT_Open_FilePath_OthrConv,$OutputFilePath &"\"& $OutputFile)    ;Send path of the input file
			;ControlClick($Dlg_OpenOthrConv,"",$BTN_Open_FilePath_OthrConv)
			ConsoleWrite("$OutputFilePath"&$OutputFilePath)
			Send($OutputFilePath &"\"& $OutputFile)
			;Send("{TAB}")
			;Send("password")
			Send("{TAB}")
			Send("{TAB}")
			;Send("{TAB}")
			Send("{ENTER}")

			sleep(1000)
		EndIf
        ControlClick($WIN_FormatConv,"",$BTN_Convert_OtherConvts)                                          ; Click on "Convert" button
        sleep(4000)
		$OutputFileOthrConv=$OutputFilePath &"\"& $OutputFile

		If winexists("TODO: <File description>") then
			Send("{ENTER}")
			$Text = "Application crashed"
			ConsoleWrite("$Text = "&$Text)
		else
			;$Text=WinGetText($WIN_FormatConv,"Conversion completed Successfully")
			;$Text=WinGetText($WIN_FormatConv,"Conversion completed Successfully")
			autoitsetoption("WinTextMatchMode",2)
			$Text=WinGetText($WIN_FormatConv,"Conversion completed Successfully")
			if $Text = "" Then
				$Text=WinGetText($WIN_FormatConv,"Conversion Completed Successfully")
			EndIf
			;$Text=controlgettext($WIN_FormatConv, "Conversion completed Successfully","[CLASS:Button; INSTANCE:6]")


			;$Text = ControlGetText($WIN_FormatConv,"Conversion completed Successfully","[CLASS:Button; INSTANCE:6]")

			ConsoleWrite("$Text: " & $Text & @CRLF)
			ConsoleWrite("**********")
		EndIf
       Return $Text
     EndIf                                                                 ;Close Format Converter window
EndFunc
;=================================================================================
;Function Name : _LogToExcelConversions
;Functionality : Converts log file for CAN and J1939 to excel files.
;Input 		   : .log file,Bus type(CAN or J1939)
;Output 	   : Creates corresponding output file
;==================================================================================

Func _LogToExcelConversions($BusType,$InputFile,$OutputFile)
	_FormatConverterMenu()                                                                                      ;Invoke Format converters menu
    if @error Then
		ConsoleWriteError("Format Converter didn't run" & @CRLF)
	Exit
    EndIf
		WinWaitActive($WIN_FormatConv, "", 5)                                                                     ;Wait for Format converter window
    if @error Then
		ConsoleWriteError("Format Conversions didn't activate" & @CRLF)
    Exit
    EndIf
    If winexists($WIN_FormatConv) then
		sleep(2000)
		$hWndLogToExcel=controlgethandle($WIN_FormatConv,"",$Tab_CAPL2CPP)
		_GUICtrlTab_ClickTab($hWndLogToExcel, 1)
		$handle=ControlGetHandle($WIN_FormatConv,"",$COMBO_SelectConvTypeLogToExcel)
	    If($BusType="CAN") Then                                                                                   ;Check whether Bus type is CAN or J1939

           _GUICtrlComboBox_SelectString($handle,"CAN")
	    Else
           _GUICtrlComboBoxEx_ShowDropDown($handle,True)
           ControlClick($WIN_FormatConv,"",$COMBO_SelectConvTypeLogToExcel)
           Sleep(1000)
		   send("{DOWN}")
		EndIf
		sleep(1000)
		controlclick($WIN_FormatConv,"",$BTN_InputLogFile)
		WinWaitActive($WIN_SelectLogFile, "", 5)
		$InputFilePath=_TestDataPath()
		WinWaitActive($WIN_SelectLogFile, "", 5)
		ControlSend($WIN_SelectLogFile,"",$TXT_Open_File_Log2Excel,$InputFilePath &"\"& $InputFile)                  ;Send path of the input file
		ControlClick($WIN_SelectLogFile,"",$BTN_Open_FilePath_LogToExcel)
		sleep(1000)
		ControlClick($WIN_FormatConv,"",$BTN_OutputCSVFile)                                                          ; Click on "Output" button
		sleep(1000)
		WinWaitActive($WIN_SelectCSVFile,"", 5)                                     					              ; wait for open dialog
		$OutputFilePath= _OutputDataPath()
		;ControlSend($WIN_SelectCSVFile,"",1152,$OutputFilePath &"\"& $OutputFile)                                     ;Send path of the output file
		ControlSend($WIN_SelectCSVFile,"",$BTNExcSaveOutput,$OutputFilePath &"\"& $OutputFile)                                     ;Send path of the output file
		ControlClick($WIN_SelectCSVFile,"",$BTN_SaveCSVFile)
		sleep(1000)
		If WinExists($WIN_ConfirmSaveAs) Then
			ControlClick($WIN_ConfirmSaveAs,"",$BTN_Yes_SaveAs)                                          ;Click Yes button to replace the outfile if it already exists.
		EndIf
		$handle=ControlGetHandle($WIN_FormatConv,"",$LSTB_AvailableFields)
		_GUICtrlListBox_ClickItem($handle,0)
		sleep(500)
		ControlClick($WIN_FormatConv,"",$BTN_SelectAll)
		Sleep(500)
		ControlClick($WIN_FormatConv,"",$BTN_Export)
		Sleep(1000)
		If WinExists($WIN_BUSMASTER) Then
			$text=ControlGetText($WIN_BUSMASTER,"",65535)                                    ;Get the text associated with popup message
			ControlClick($WIN_BUSMASTER,"",$BTN_OK_LogToExcel)
		EndIf
		ConsoleWrite("$text " & $text & @CRLF)
		Return $text
	EndIf


EndFunc


;=================================================================================
;Function Name : _GetPopUpMenuText
;Functionality : Fetches the right click opup menu item texts
;Input 		   : -
;Output 	   : Returns all the items in the pop up menu.
;				$aPopUp_Text[0]=No. of items in the context menu
;==================================================================================

Func _GetPopUpMenuText()
	sleep(750)
	Dim $aPopUp_Text[10]=["","","","","","","","","",""]
    Local $aPopUp_List = _WinAPI_EnumWindowsPopup()
    Local $hWnd = $aPopUp_List[1][0]
    Local $sClass = $aPopUp_List[1][1]
    If $sClass = "#32768" Then
        $hMenu = _SendMessage($hWnd, $MN_GETHMENU, 0, 0)
        If _GUICtrlMenu_IsMenu($hMenu) Then
            $iCount = _GUICtrlMenu_GetItemCount($hMenu)
            Local $aPopUp_Text[1] = [0]
            For $j = 0 To $iCount - 1
                $aPopUp_Text[0] += 1
                ReDim $aPopUp_Text[$aPopUp_Text[0] + 1]
                $aPopUp_Text[$aPopUp_Text[0]] = _GUICtrlMenu_GetItemText($hMenu, $j)
				ConsoleWrite(" $aPopUp_Text[$aPopUp_Text[0]]: "& $aPopUp_Text[$aPopUp_Text[0]]&@CRLF )
			Next
        EndIf
    EndIf
	Return $aPopUp_Text
EndFunc

;=================================================================================
;Function Name : _GetTestEditorDetailsHWD
;Functionality : Fetches the handle of test editor details
;Input 		   : -
;Output 	   : Returns the handle
;==================================================================================

Func _GetTestEditorDetailsHWD()
	$HWD=ControlGetHandle($WIN_BUSMASTER,"",$LVC_TestAuto_TestEditor)								; Fetch the handle of test editor details
	Return $HWD
EndFunc

;=================================================================================
;Function Name : _ClickTestEditConfirmBTN
;Functionality : Clicks the confirm button in Test Automation editor window
;Input 		   : -
;Output 	   :
;==================================================================================

Func _ClickTestEditConfirmBTN()
	ControlClick($WIN_BUSMASTER,"",$BTN_Confirm_TEditor)											; Click on Confirm button
	sleep(750)
EndFunc

;=================================================================================
;Function Name : _SelectTestSetUpNode
;Functionality : Select the test setup item in the tree view(Parent Item) of Test Automation editor window
;Input 		   : -
;Output 	   : -
;==================================================================================

Func _SelectTestSetUpNode()
	ControlTreeView ($WIN_BUSMASTER,"",$TVC_TestAuto_TestEditor,"Select","#0")					; Select the test setup item in the tree view(Parent Item)
	sleep(700)
EndFunc

;=================================================================================
;Function Name : _SelectTestCaseNode
;Functionality : Select the test case item in the tree view of Test Automation editor window
;Input 		   : -
;Output 	   : -
;==================================================================================

Func _SelectTestCaseNode()
	ControlTreeView ($WIN_BUSMASTER,"",$TVC_TestAuto_TestEditor,"Select","#0|#0")				; Select the test case node
	sleep(700)
EndFunc


;=================================================================================
;Function Name : _SelectSendNode
;Functionality : Select the send item in the tree view of Test Automation editor window
;Input 		   : -
;Output 	   : -
;==================================================================================

Func _SelectSendNode()
	ControlTreeView ($WIN_BUSMASTER,"",$TVC_TestAuto_TestEditor,"Select","#0|#0|#0")			; Select the Send node
	sleep(700)
EndFunc

;=================================================================================
;Function Name : _SelectSendMsgNode
;Functionality : Select the first msg in the send node of Test Automation editor window
;Input 		   : -
;Output 	   : -
;==================================================================================

Func _SelectSendMsgNode()
	ControlTreeView ($WIN_BUSMASTER,"",$TVC_TestAuto_TestEditor,"Select","#0|#0|#0|#0")			; Select the Send msg node
	sleep(700)
EndFunc

;=================================================================================
;Function Name : _SelectWaitNode
;Functionality : Select the Wait item in the tree view of Test Automation editor window
;Input 		   : -
;Output 	   : -
;==================================================================================

Func _SelectWaitNode()
	ControlTreeView ($WIN_BUSMASTER,"",$TVC_TestAuto_TestEditor,"Select","#0|#0|#1")			; Select the Wait node
	sleep(700)
EndFunc

;=================================================================================
;Function Name : _SelectVerifyNode
;Functionality : Select the Verify item in the tree view of Test Automation editor window
;Input 		   : -
;Output 	   : -
;==================================================================================

Func _SelectVerifyNode()
	ControlTreeView ($WIN_BUSMASTER,"",$TVC_TestAuto_TestEditor,"Select","#0|#0|#2")			; Select the Verify node
	sleep(700)
EndFunc

;=================================================================================
;Function Name : _SelectVerifyMsgNode
;Functionality : Select the first msg in the verify node of Test Automation editor window
;Input 		   : -
;Output 	   : -
;==================================================================================

Func _SelectVerifyMsgNode()
	ControlTreeView ($WIN_BUSMASTER,"",$TVC_TestAuto_TestEditor,"Select","#0|#0|#2|#0")			; Select the verify msg node
	sleep(700)
EndFunc

;=================================================================================
;Function Name : _ExpandTestEditorTreeView
;Functionality : Expand the tree view in test editor window of Test Automation editor window
;Input 		   : -
;Output 	   : -
;==================================================================================

Func _ExpandTestEditorTreeView()
	$TVHWD= ControlGetHandle($WIN_BUSMASTER,"",$TVC_TestAuto_TestEditor)						; Fetch Test Editor tree view handle
	_GUICtrlTreeView_Expand($TVHWD,"",True)														; Expand the tree view
	sleep(1000)
EndFunc


;=================================================================================
;Function Name : _ExpandTestExeTreeView
;Functionality : Expand the tree view in test executor window
;Input 		   : -
;Output 	   : -
;==================================================================================

Func _ExpandTestExeTreeView()
;~ 	$TVHWD= ControlGetHandle($WIN_BUSMASTER,"",$TVC_TestSuite_TestExecutor)						; Fetch Test Editor tree view handle
;~ 	_GUICtrlTreeView_Expand($TVHWD,"",True)														; Expand the tree view
	ControlTreeView ($WIN_BUSMASTER,"",$TVC_TestSuite_TestExecutor,"Expand","#0")
	sleep(1000)
	ControlTreeView ($WIN_BUSMASTER,"",$TVC_TestSuite_TestExecutor,"Expand","#0|#0")
	sleep(1000)
EndFunc

;=================================================================================
;Function Name : _GetToolVersion()
;Functionality : Retrieves BUSMASTER version from About window
;Input 		   : -
;Output 	   : -
;==================================================================================

Func _GetToolVersion()
	WinMenuSelectItem($WIN_BUSMASTER,"",$HelpMenu,$AboutBusmaster)
	WinWaitActive($DLG_About,"",3)
	$Ver=controlgettext($DLG_About,"",$Static_Version_About)									; Fetch version
	$BMVer="BUSMASTER " & $Ver
	ConsoleWrite("$BMVer :" &$BMVer&@CRLF)
	ControlClick($DLG_About,"",$BTN_OK_About)
	Return $BMVer
EndFunc

Func _RunMe()

	Global $oExcel,$FilePath,$AppExePath
	Global $crashRes, $lConfig, $cConfig, $channelCount, $time

	; $BusMasterExeFPath has to be updated everytime before running the script on a new version.
	;$BusMasterExeFPath=@ProgramFilesDir&"\BUSMASTER_v1.9.0"




	$FilePath = _SetOneFolderUp()&"\Result\TestRunResults.xlsx" ; This file should already exist

;~ 	if WinExists("Microsoft Excel - TestRunResults.xlsx") Then  ; Check and close the result file if it is opened.
;~ 		WinClose("Microsoft Excel - TestRunResults.xlsx")
;~ 		sleep(1000)
;~ 	EndIf
	if FileExists($FilePath) Then
		;if WinExists("Microsoft Excel - TestRunResults.xlsx") Then
		if WinExists("TestRunResults.xlsx") Then
			Consolewrite(@crlf & "Result Excel sheet is opened" & @CRLF)
			;WinActivate("Microsoft Excel - TestRunResults.xlsx")
			WinActivate("TestRunResults.xlsx")
			$FuncRes=_ExcelBookSave($oExcel)
			Send("^s");
			Consolewrite("$FuncRes : " & $FuncRes & @CRLF)
			sleep(2000)
			;WinClose("Microsoft Excel - TestRunResults.xlsx")
			WinClose("TestRunResults.xlsx")
			sleep(1000)
		EndIf
		Global $oExcel = _ExcelBookOpen($FilePath) ; Open the Excel file
		If @error = 1 Then
			MsgBox(0, "Error!", "Unable to Create the Excel Object")
			Exit
		ElseIf @error = 2 Then
			MsgBox(0, "Error!", "File does not exist")
			Exit
		EndIf
		_ExcelSheetActivate($oExcel, "1.9.0")
		;_ExcelSheetActivate($oExcel, "2.0.0")
	EndIf

	; If output folder exists in the script directory then delete it
	$OutputFolderPath =  @ScriptDir
	if fileexists($OutputFolderPath & "\" & "Output")=1 Then
		DirRemove($OutputFolderPath & "\" & "Output",1)
	EndIf

	;~ ; Copy below Node simulation files from installed Busmaster folder to the script directory to avoid build,load errors
;~ 	FileCopy ( $BusMasterExeFPath & "\GCCDLLMakeTemplate_CAN", @ScriptDir, 1)
;~ 	DirCreate( @ScriptDir & "\SimulatedSystems")
;~ 	DirCopy ( $BusMasterExeFPath & "\SimulatedSystems", @ScriptDir& "\SimulatedSystems", 1)
;~ 	FileCopy ( $BusMasterExeFPath & "\GCCDLLMakeTemplate_J1939", @ScriptDir, 1)

;~ 	_launchApp()																					; Check if the App is open else launch it


	$oIE = _IECreate()											; Create an Internet Explorer Browser Window
	consolewrite("object " & $oIE & @CRLF)
	_IEAction($oIE, "visible")									; Sets an object state to hidden

	 consolewrite("object aft " & $oIE & @CRLF)

	 ; Save the HTML File
	;~ _SaveHTMLReport()

EndFunc

;....//---------Added By srinibas Das-------------------------

;=================================================================================
;Function Name : _openLINClusterConfig
;Functionality : This function opens a LIN Cluster configuration window
;Input 		   : -
;Output 	   : -
;==================================================================================

Func _openLINClusterConfig()
	$sMenu = WinMenuSelectItem($WIN_BUSMASTER, "", $linMenu, $ClusterConfigMenu) ; Select LIN->Cluster Configuration
	Sleep(1000)
EndFunc


;=================================================================================
;Function Name :
;Functionality : - This function imports LDF file for LIN.
;Input 		   : - LDF file Name
;Output 	   : -
;==================================================================================
Func _importLDF($ldfFileName)

	$DirPath =_TestDataPath()

	if winexists($WIN_ClusterConfiguration) then
		ControlClick($WIN_ClusterConfiguration,"",1378)												; click on brows for selection of LDF file.
		Sleep(1000)
	EndIf

	WinWaitActive($WIN_SelectLDFFile,"",3)
	if WinExists($WIN_SelectLDFFile,"") Then
		ControlSend($WIN_SelectLDFFile,"",$TXT_FilePath_SaveConfigFile1,$DirPath&"\"&$ldfFileName)
		ControlClick($WIN_SelectLDFFile,"",$BTN_Open_FilePath_OthrConv)
	EndIf

EndFunc   ;==>_importLDF
;=================================================================================
;Function Name : _linDriverSelection
;Functionality : - This function select hardware from hadrware selection window for LIN.
;Input 		   : -
;Output 	   : -
;==================================================================================


Func _linDriverSelection()

	WinMenuSelectItem($WIN_BUSMASTER, "",$linMenu,$LindriverSelectMenu,$Lindriver) ;Select Lin Driver

	sleep(1000)
	WinWaitActive($WinHwSelection)
	if winexists($WinHwSelection) Then
		$LinSelhWd=controlgethandle($WinHwSelection,"",$LinHwSelect)		; Get handle of Lin-Hardware selection list view [CLASS:SysListView32; INSTANCE:1]
		_GUICtrlListView_ClickItem($LinSelhWd,0)							; Click on the first hardware
		sleep(1000)
		ControlClick($WinHwSelection,"","[CLASS:Button; INSTANCE:1]")					; Click on Select button
		sleep(750)
		ControlClick($WinHwSelection,"","[CLASS:Button; INSTANCE:3]")					; Click on OK button
		sleep(750)
	EndIf

	ConsoleWrite("Selected hardware for LIN..."& @CRLF)


EndFunc
;=================================================================================
;Function Name : _GetLINecuListWinHWD
;Functionality : - fetches the handle of the ECU List under cluster configuration for LIN
;Input 		   : -
;Output 	   : -
;==================================================================================


Func _GetLINecuListWinHWD()
	MouseClick("left", @DesktopWidth / 2.4, @DesktopHeight / 2)
	$HWD=ControlGetHandle($WIN_ClusterConfiguration,"",1380)									; Fetch the handle of Message window list view
	ConsoleWrite("$HWD "&$HWD&@CRLF)
	Return $HWD
EndFunc


;=================================================================================
;Function Name : _selectECU
;Functionality : - This function selects check box of ECUs under ECU List in cluster configuration for LIN.
;Input 		   : - Index
;Output 	   : -
;==================================================================================

Func _selectECU($index)

	if $index = "ALL" Then
		$index = -1
	EndIf
	$ecuListHWD=ControlGetHandle($WIN_ClusterConfiguration,"",$LinEcuListView)
	_GUICtrlListView_SetItemChecked($ecuListHWD,$index, True)


	ConsoleWrite("ECU HAS BEEN SELECTED"& @CRLF)

EndFunc
;===============================================================================
;Function Name : _GetLINMsgWinHWD
;Functionality : fetches the handle of the LIN message window
;Input 		   :
;Output 	   :
;===============================================================================
Func _GetLINMsgWinHWD()
	MouseClick("left", @DesktopWidth / 2, @DesktopHeight / 4)
	$HWD=ControlGetHandle($WIN_BUSMASTER,"",$LVC_CID_LINMsgWin)									; Fetch the handle of Message window list view
	ConsoleWrite("$HWD "&$HWD&@CRLF)
	Return $HWD
EndFunc
;===============================================================================
;Function Name : _GetLINMsgWinColCount
;Functionality : fetches the column count in the LIN message window
;Input 		   :
;Output 	   :
;===============================================================================
Func _GetLINMsgWinColCount()
	$HWD=_GetLINMsgWinHWD()
	$ItemCount=_GUICtrlListView_GetColumnCount($HWD)
	Return $ItemCount
EndFunc

;===============================================================================
;Function Name : _TxMsgMenuLIN
;Functionality : Selects Tx Messages from the menu of LIN
;Input 		   :
;Output 	   :
;===============================================================================
Func _TxMsgMenuLIN()
	WinMenuSelectItem($WIN_BUSMASTER,"",$linMenu,$TxMsgMenuLin,$configTxmenuLin)							; Select Configure->Tx
EndFunc


;===============================================================================
;Function Name : _selectMasterSlaveLIN
;Functionality : Select radion button for master/slave in LIN tx config window
;Input 		   : master/slave
;Output 	   :
;===============================================================================
func _selectMasterSlaveLIN($masterSlave)

	if $masterSlave = "master" Then

		ControlCommand($WIN_BUSMASTER, "",$enableMaster_ConfigTXLIN, "Check") 						;Enable radio button for master in LIN tx Config Window.

	ElseIf $masterSlave = "slave" Then

		ControlCommand($WIN_BUSMASTER, "",$enableSlave_ConfigTXLIN, "Check")						;;Enable radio button for salve in LIN tx Config Window.

	EndIf

EndFunc

;===============================================================================
;Function Name : _selectMsgID
;Functionality : Select message id from the drop down list in TX config window for LIN.
;Input 		   : index of dropdown messagebox
;Output 	   :
;===============================================================================
Func _selectMsgID($Index)

	ControlCommand($WIN_BUSMASTER,"",$COMB_MsgID_ConfigTX,"SetCurrentSelection",$Index)				; Select message id from drop down in LIN TxConfig window

	sleep(500)
EndFunc

;===============================================================================
;Function Name : _AddMsg2TxListLin
;Functionality : Adds a message to the TX list for LIN.
;Input 		   : -
;Output 	   :
;===============================================================================
Func _AddMsg2TxListLin()

	ControlClick($WIN_BUSMASTER,"",$BTN_AddMsg_ConfigTXLIN)											;Click on ADD Message button

	sleep(500)
EndFunc
;===============================================================================
;Function Name : _ConnectDisconnect_LIN()
;Functionality : Clicks on the connect button in the toolbar for LIN
;Input 		   :
;Output 	   :
;===============================================================================
Func _ConnectDisconnect_LIN()
	$cntToolLINhWd=ControlGetHandle($WIN_BUSMASTER,"",$ToolBar_Con_Disconnect_LIN)					; Get handle of the 'Connect/Disconnect' toolbar for LIN
	_GUICtrlToolbar_ClickIndex($cntToolLINhWd,$Icon_ConDiscon_Index)								; Click on 'Connect/Disconnect' icon
	ConsoleWrite("clicked on Connect/Disconnect tool bar for LIN...")
	sleep(1500)
EndFunc

;===============================================================================
;Function Name : select_rep_TxFrameList_LIN()
;Functionality : Select repetation check boxes in TX frame list for LIN
;Input 		   :
;Output 	   :
;===============================================================================
Func _select_rep_TxFrameList_LIN($index)

	MouseClick("left", @DesktopWidth / 2, @DesktopHeight / 4)
	;$HWD=ControlGetHandle($WIN_BUSMASTER,"",$LVC_CID_LINMsgWin)									; Fetch the handle of Message window list view
	;ConsoleWrite("$HWD "&$HWD&@CRLF)

	$cntToolLINhWd=ControlGetHandle($WIN_BUSMASTER,"",$LinTxFrameList)					; Get handle of the 'Connect/Disconnect' toolbar for LIN
	;$return = _GUICtrlListBox_GetItemRect($cntToolLINhWd, $index)

	ConsoleWrite("$cntToolLINhWd = "&@CRLF)
	;ConsoleWrite("$return="&$return&@CRLF)

	$sixthColH=_GUICtrlListView_GetColumn($cntToolLINhWd,5)
	ConsoleWrite("$ColTxconfList---"&$sixthColH &@CRLF)
	$ColTxconfList=StringStripWS($sixthColH[5],2)
	ConsoleWrite("$ColTxconfList---"&$ColTxconfList &@CRLF)
	;$ecuListHWD=ControlGetHandle($WIN_ClusterConfiguration,"",$LinEcuListView)
	;_GUICtrlListView_SetItemChecked($sixthColH,$index, True)
	sleep(1500)
EndFunc

;===============================================================================
;Function Name : _GetLINMsgWinItemCount
;Functionality : fetches the item count in the LIN message window
;Input 		   :
;Output 	   :
;===============================================================================
Func _GetLINMsgWinItemCount()
	$HWD=_GetLINMsgWinHWD()
	$ItemCount=_GUICtrlListView_GetItemCount($HWD)
	ConsoleWrite("$ItemCount "&$ItemCount&@CRLF)
	Return $ItemCount
EndFunc
;=================================================================================
;Function Name : _GetMsgWinLINInfo
;Functionality : Fetch the message window LIN information
;Input 		   : Row No.
;Output 	   : Returns the Message window data to calling function
;===============================================================================
Func _GetMsgWinLINInfo($Row)

	;WinActivate(, 3)
	local $Data[10]=["","","","","","","","","",""]
	ControlFocus($WIN_BUSMASTER,"",$LSTC_LINMsgWin)
	$Count=_GetLINMsgWinItemCount()													; Fetch the item count in the msg window
	$GetLINMsgWinCloCount=_GUICtrlListView_GetColumnCount(_GetLINMsgWinHWD())
	consolewrite("$Count :"&$Count&@CRLF)
	consolewrite("$Row :"&$Row&@CRLF)
	consolewrite("$GetLINMsgWinCloCount :"&$GetLINMsgWinCloCount&@CRLF)
	if $Row<=$Count Then
		$HWD= _GetLINMsgWinHWD()
		$MsgEntry=_GUICtrlListView_GetItemTextString($HWD,$Row)						; Fetch the Msg window row item text
		ConsoleWrite("$MsgEntry:"&$MsgEntry&@CRLF)
		$Msg=StringTrimLeft($MsgEntry,1)											; Remove the first character
		ConsoleWrite("$Msg:"&$Msg&@CRLF)
;~ 		$RepMsg=StringReplace($Msg,"|"," ")
;~ 		ConsoleWrite("$RepMsg:"&$RepMsg&@CRLF)
		$SplitMsg=StringSplit($Msg,"|")
		ConsoleWrite("$SplitMsg:"&$SplitMsg[1]&@CRLF)
		$MsgTime_MsgWin=$SplitMsg[1]												; Fetch the time
		ConsoleWrite("$MsgTime_MsgWin:"&$MsgTime_MsgWin&@CRLF)
		$MsgName_MsgWin=$SplitMsg[2]												; Fetch the msg name
		ConsoleWrite("$MsgName_MsgWin:"&$MsgName_MsgWin&@CRLF)
		$MsgType_MsgWin=$SplitMsg[3]												; Fetch the msg type
		ConsoleWrite("$MsgType_MsgWin:"&$MsgType_MsgWin&@CRLF)
		$MsgDir_MsgWin=$SplitMsg[4]													; Fetch the Msg direction
		ConsoleWrite("$MsgDir_MsgWin:"&$MsgDir_MsgWin&@CRLF)
		$MsgChannel_MsgWin=$SplitMsg[5]												; Fetch the channel no
		ConsoleWrite("$MsgChannel_MsgWin:"&$MsgChannel_MsgWin&@CRLF)
		$MsgDLC_MsgWin=$SplitMsg[6]	 												; Fetch the DLC
		ConsoleWrite("$MsgDLC_MsgWin:"&$MsgDLC_MsgWin&@CRLF)
		$MsgID_MsgWin=$SplitMsg[7]	 												; Fetch the msg id
		ConsoleWrite("$MsgID_MsgWin:"&$MsgID_MsgWin&@CRLF)
		$MsgData_MsgWin=StringSplit($Msg,"|")	 									; Fetch the msg data bytes
		ConsoleWrite("$MsgData_MsgWin[9]:"&$MsgData_MsgWin[8]&@CRLF)
		$MsgDataRemSpace_MsgWin=StringStripWS($MsgData_MsgWin[8],8)					; Remove the Spaces in DLC
		ConsoleWrite("$MsgDataRemSpace_MsgWin:"&$MsgDataRemSpace_MsgWin&@CRLF)
		$MsgChksum_MsgWin=$SplitMsg[9]	 											; Fetch the Checksum
		ConsoleWrite("$MsgChksum_MsgWin:"&$MsgChksum_MsgWin&@CRLF)
	EndIf
	$Data[0]=$MsgTime_MsgWin
	$Data[1]=$MsgName_MsgWin
	$Data[2]=$MsgType_MsgWin
	$Data[3]=$MsgDir_MsgWin
	$Data[4]=$MsgChannel_MsgWin
	$Data[5]=$MsgDLC_MsgWin
	$Data[6]=$MsgID_MsgWin
	$Data[7]=$MsgDataRemSpace_MsgWin
	$Data[8]=$MsgChksum_MsgWin
	Return $Data
	EndFunc
;=================================================================================
;Function Name : _GetMsgWinInfo
;Functionality : Fetch the message window information
;Input 		   : instance in Message window,Row No.
;Output 	   : Returns the Message window data to calling function as object list.
;===============================================================================

Func _GetMsgWinInfo($LSTC_MsgWin,$Row,$col)

		ControlFocus($WIN_BUSMASTER,"",$LSTC_MsgWin)
		;$MsgWinColCount=_GetLINMsgWinColCount()
		;$HWD=_GetLINMsgWinHWD()
		MouseClick("left", @DesktopWidth / 2, @DesktopHeight / 4)
		$HWD=ControlGetHandle($WIN_BUSMASTER,"",$LSTC_MsgWin)									; Fetch the handle of Message window list view
		ConsoleWrite("$HWD "&$HWD&@CRLF)

		$MsgWinColCount=_GUICtrlListView_GetColumnCount($HWD)

		$Count=_GUICtrlListView_GetItemCount($HWD)
		ConsoleWrite("$item Count - "&$Count&@CRLF)
		ConsoleWrite("$MsgWinColCount - "&$MsgWinColCount&@CRLF)

		$ObjList = ObjCreate("System.Collections.ArrayList")

		if($MsgWinColCount = $col) Then															;validate message window columnn

			consolewrite("$MsgWinColCount :"&$MsgWinColCount&@CRLF)
			;ControlFocus($WIN_BUSMASTER,"",$LSTC_MsgWin)
			consolewrite("$LSTC_MsgWin :"&$LSTC_MsgWin&@CRLF)

			consolewrite("$MsgWinColCount :"&$MsgWinColCount&@CRLF)

			;$Count=_GetLINMsgWinItemCount()

			;$ObjList = ObjCreate("System.Collections.ArrayList") 								; create object list

			if $Row<=$Count Then


				$HWD=ControlGetHandle($WIN_BUSMASTER,"",$LSTC_MsgWin)							; Fetch the handle of Message window list view
				ConsoleWrite("$HWD "&$HWD&@CRLF)

				$MsgEntry=_GUICtrlListView_GetItemTextString($HWD,$Row)							; Fetch the Msg window row item text
				ConsoleWrite("$MsgEntry:"&$MsgEntry&@CRLF)

				$Msg=StringTrimLeft($MsgEntry,1)												; Remove the first character
				ConsoleWrite("$Msg:"&$Msg&@CRLF)

				$SplitMsg=StringSplit($Msg,"|")

				for $i=1 to $MsgWinColCount-1
					ConsoleWrite("$SplitMsg[" & $i &"] : "&$SplitMsg[$i]&@CRLF)
					$value = StringStripWS($SplitMsg[$i],3)
					$ObjList.Add($value)														;Save all msg info into object list
				next

			EndIf
			ConsoleWrite("$ObjList=" & $ObjList.Item(0))
		EndIf
		Return $ObjList																		;return object list

EndFunc

;=================================================================================
;Function Name :  _GetMsgInterpretWinInfo_LIN
;Functionality : Fetch the message window information
;Input 		   : instance in Message window,Row No.
;Output 	   : Returns the Message window data to calling function as object list.
;===============================================================================

	Func _GetMsgInterpretWinInfo_LIN($Row)
	Dim $MsgData[10]=["","","","","","","","","",""]

	ControlFocus($WIN_BUSMASTER,"",$LSTC_LINMsgWin)
	$HWD=_GetLINMsgWinHWD()
	ConsoleWrite("$HWD =" &$HWD)

	;sleep(2000)
	_GUICtrlListView_ClickItem($HWD,$Row,"LEFT","",2)											; Double click on the row
		if WinWaitActive($WIN_MsgInterpret,"",3) Then
			$SigHWD=ControlGetHandle($WIN_MsgInterpret,"",$LVC_Signal_MsgIntWin)				; Fetch the handle of Message interpretation window list view
			$ItemCount=_GUICtrlListView_GetItemCount($SigHWD)									; fetch the count of items
			ConsoleWrite("$ItemCount "&$ItemCount&@CRLF)
			if $ItemCount>0 Then
				$MsgData[1]=ControlGetText($WIN_MsgInterpret,"",$TXT_MsgName_MsgIntWin)				; Fetch the message name
				$MsgData[2]=ControlGetText($WIN_MsgInterpret,"",$TXT_MsgID_MsgIntWin)				; Fetch the message ID
				$SigDetails=_GUICtrlListView_GetItemTextArray($SigHWD,0)							; Fetch the text of the first row
				$MsgData[3]=$SigDetails[1]															; Assign signal name
				$MsgData[4]=$SigDetails[2]															; Assign signal physical value
				$MsgData[5]=$SigDetails[3]															; Assign signal raw value
				$MsgData[6]=$SigDetails[4]															; Assign signal unit
			EndIf
		winclose($WIN_MsgInterpret)
		EndIf
		for $i=1 to 6
			ConsoleWrite("$MsgData[" & $i &"] : "&$MsgData[$i]&@CRLF)
		next
	Return $MsgData																				; Return all the data
EndFunc
;===============================================================================
;Function Name : _AppFilterMenu_LIN
;Functionality : Selects App filter from the menu
;Input 		   :
;Output 	   :
;===============================================================================
Func _AppFilterMenu_LIN()
	WinMenuSelectItem($WIN_BUSMASTER,"",$linMenu,$AppFilterMenu)								; Select Configure->App Filters
	winwaitactive($WIN_Config_filter,"",3)
EndFunc
;=================================================================================
;Function Name :  _AddFilter_LIN
;Functionality : Configures Filter details for LIN
;Input 		   :
;Output 	   :
;===============================================================================
Func _AddFilter_LIN($filterType,$msgID,$FromID,$ToID,$Dir,$Channel)
	_AppFilterMenu_LIN()																		; Select LIN>Filter menu
	WinWaitActive($WIN_Config_filter,"",2)														; Wait for the window to be active
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
	if $msgID<>"" Then
		ControlCommand($WIN_Config_filter,"",$COMB_MsgID_ConfigFilter,"SelectString", $msgID)	; Select the MSG from ID combo box
		sleep(200)
	Else
		ControlCommand($WIN_Config_filter,"",$RBTN_Range_FilterSelect,"Check")					; Select the Range radio button
		sleep(200)
		ControlSetText($WIN_Config_filter,"",$EDIT_From_FilterSelect,$FromID)					; Enter the From ID
		sleep(200)
		ControlSetText($WIN_Config_filter,"",$EDIT_To_FilterSelect,$ToID)						; Enter the To ID
	EndIf

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
;Function Name : _AddFiltertoMsgDisp_LIN
;Functionality : Adds the filter for message display for LIN
;Input 		   :
;Output 	   :
;===============================================================================
Func _AddFiltertoMsgDisp_LIN()
	sleep(100)
	ControlClick($WIN_MsgDisplay_LIN,"",$BTN_Configure_MsgDisplay)								; Click Configure button in msg display
	sleep(750)
	if winexists($WIN_FilterSelect) Then
		$fltSelhWd=controlgethandle($WIN_FilterSelect,"",$LSTC_ConfigFilter_FilterSelect)		; Get handle of filter selection list view
		_GUICtrlListView_ClickItem($fltSelhWd,0)												; Click on the first filter
		$FilterItem = _GUICtrlListView_GetItem($fltSelhWd, 0)
		ControlClick($WIN_FilterSelect,"",$BTN_Add_FilterSelect)								; Click on Add button
		sleep(750)
		ControlClick($WIN_FilterSelect,"",$BTN_OK_FilterSelect)									; Click on OK button
		sleep(750)
	EndIf
	;ControlClick($WIN_MsgDisplay,"",$BTN_OK_MsgDisplay)											; Click on OK button
	ControlClick($WIN_MsgDisplay_LIN,"",$BTN_OK_MsgDisplay)
EndFunc

;===============================================================================
;Function Name : _OpenFilterConfig_LIN
;Functionality : open filter config window for LIN
;Input 		   :
;Output 	   :
;===============================================================================
Func _OpenFilterConfig_LIN()
	WinMenuSelectItem($WIN_BUSMASTER,"",$linMenu,$LINMsgWinMenu,$configMsgWindowLIN)
EndFunc
;===============================================================================
;Function Name : _OpenMshWindConfig_LIN
;Functionality : open  config message window for LIN
;Input 		   :
;Output 	   :
;===============================================================================
Func _OpenMsgWindConfig_LIN()

;open menu LIN->Message Window -> configure
	WinMenuSelectItem($WIN_BUSMASTER,"",$linMenu,$LINMsgWinMenu,$configMsgWindowLIN)
	sleep(2000)
EndFunc
;===============================================================================
;Functionality : Fetch the handle of the LIN-signalWatch-Config  tree view control
;Input 		   :
;Output 	   : Returns the handle of the Lin_SignalWatch Configure tree view control
;===============================================================================

Func _GetLINSWConfigTVHWD()
	$HWD = ControlGetHandle($WinSigwatchConfigLIN,"",$LSTC_LINSigWatConfigWin)
	Return $HWD
EndFunc
;==========================================================================================================
;Function Name 		: _saveAsConfig
;Functionality 		: This function save As the configuration
;Input 		   		: -config file name
;Output		   		: -
;==========================================================================================================
Func _saveAsConfig($cfxFName)
	WinMenuSelectItem($WIN_BUSMASTER,"",$fileMenu,$saveAsMenu)					; Select File->Save As

	sleep(1000)
	$DirPath =_OutputDataPath()													; Set the DirPath to save the dbf file
	WinWaitActive($WIN_DefaultCfxSave,"",3)
	if WinExists($WIN_DefaultCfxSave,"") Then
		ControlSend($WIN_DefaultCfxSave,"",$TXT_FilePath_SaveConfigFile,$DirPath&"\defaultFile");~
		ControlClick($WIN_DefaultCfxSave,"",$BTN_Save_SaveConfigFile)
	EndIf
	_HandleSaveSimSysWin()															; Check for Save Simulated System window
	WinWaitActive($WIN_SaveAsCfx,"",5)
	if winexists($WIN_SaveAsCfx) Then
		ControlSend($WIN_SaveAsCfx,"",$TXT_FileName_NewCfx,$DirPath&"\"&$cfxFName)	; Set the filename in 'SaveAs configuration filename' dialog
		sleep(500)
		$cConfig=ControlClick($WIN_SaveAsCfx,"",$BTN_SaveAsInst_Cfx)				; Click on Save button
		sleep(2500)
	EndIf
	_HandleSaveSimSysWin()
EndFunc


;===============================================================================
;Functionality : Fetch the handle of the LIN-signalWatch-  tree view control
;Input 		   :
;Output 	   : Returns the handle of the Lin_SignalWatch Configure tree view control
;===============================================================================

Func _GetLINSWTVHWD()

	$HWD = 0
	if WinExists($WIN_SigWatchLIN) Then
		$HWD = ControlGetHandle($WIN_SigWatchLIN,"",$LVC_Value_SigWatch)
		ConsoleWrite("HWD for signal watch-"&$HWD&@CRLF)

	EndIf
	Return $HWD

EndFunc
;===============================================================================
;Function Name : _ConfigureLINLog()
;Functionality : Configure Logging for LIN
;Input 		   : Log File Name, Time Mode, Channel #, Numeric Format, File Mode, Start Rec MsgID, Stop Rec MsgID, Filters={True,False}
;Output 	   :
;===============================================================================

Func _ConfigureLINLog($logFileName,$TimeMode,$Channel,$NumFormat,$FileMode,$Start_Rec,$Stop_Rec,$Filters)
	WinMenuSelectItem($WIN_BUSMASTER,"",$linMenu,$LINLogMenu,$configureMenu)					; Select Configure->Log
	Sleep(1000)
	ControlClick($WIN_LINLog,"",$BTN_Add_ConfigLOG)												; Click on Add button
	sleep(250)
	ControlCommand ($WIN_LINLog,"",$BTN_EnDisable_ConfigLOG,"Check")							; Check Enable/Disable Logging during connect/disconnect
	$LogFilePath=_OutputDataPath()
	ControlClick($WIN_LINLog,"",$BTN_logFPath_ConfigLOG)										; Click on Log File Path button
	sleep(1000)
	if winexists($WIN_Select_LogFile) Then
		controlsend($WIN_Select_LogFile,"",$TXT_FileName_SelLogFile,$LogFilePath & "\" & $logFileName)	; Enter the Log file Name
		controlclick($WIN_Select_LogFile,"",$BTN_SaveInst_SelLogFile)									; Click on 'Save' button
	EndIf
	controlcommand($WIN_LINLog,"",$COMB_TimeMode_ConfigLOG,"SelectString", $TimeMode)			; Set the time mode
	controlcommand($WIN_CANLog,"",$COMB_Channel_ConfigLOG,"SelectString", $Channel)				; Set the Channel
	sleep(500)
	if $NumFormat="Hex" Then
		Controlcommand($WIN_LINLog,"",$RBTN_NumModeHex_ConfigLOG,"Check")						; Set the Numeric mode to Hex
	Else
		Controlcommand($WIN_LINLog,"",$RBTN_NumModeDec_ConfigLOG,"Check")						; Set the Numeric mode to Decimal
	EndIf
	sleep(1000)
	if $FileMode="Overwrite" Then
		Controlcommand($WIN_LINLog,"",$RBTN_FileModeOverWrite_ConfigLOG,"Check")				; Set the File mode
	Else
		Controlcommand($WIN_LINLog,"",$RBTN_FileModeApp_ConfigLOG,"Check")						; Set the File mode
	EndIf
	if $Start_Rec<>"" and $Stop_Rec<>"" Then
		Controlcommand($WIN_LINLog,"",$CHKB_StartTrig_ConfigLOG,"Check")						; enable the start trigger
		ControlSend($WIN_LINLog,"",$TXT_StartMsgID_ConfigLOG,$Start_Rec)						; Set the Start trigger Msg ID
		Controlcommand($WIN_LINLog,"",$CHKB_StopTrig_ConfigLOG,"Check")							; enable the stop trigger
		ControlSend($WIN_LINLog,"",$TXT_StopMsgID_ConfigLOG,$Stop_Rec)							; Set the Stop trigger Msg ID
	EndIf
	sleep(500)
	if $Filters="True" Then
		controlclick($WIN_LINLog,"",$BTN_Filter_ConfigLOG)										; Click on 'Filter' button
		sleep(1000)
		if WinWaitActive($WIN_FilterSelect,"",3) Then
			$fltSelhWd=controlgethandle($WIN_FilterSelect,"",$LSTC_ConfigFilter_FilterSelect)	; Get handle of filter selection list view
			_GUICtrlListView_ClickItem($fltSelhWd,0)											; Click on the first filter
			ControlClick($WIN_FilterSelect,"",$BTN_Add_FilterSelect)								; Click on Add button
			sleep(800)
			ControlClick($WIN_FilterSelect,"",$BTN_OK_FilterSelect)								; Click on OK button
			sleep(800)
		EndIf
	EndIf
	controlclick($WIN_LINLog,"",$BTN_OK_ConfigLOG)												; Click on 'OK' button
	sleep(500)
EndFunc

;===============================================================================
;Function Name : _EnableFilterLogLIN
;Functionality : Enable filters for Logging
;Input 		   :
;Output 	   :
;===============================================================================
Func _EnableFilterLogLIN()
	WinMenuSelectItem($WIN_BUSMASTER,"",$linMenu,$LINLogMenu,$EnableFilterLogMenu)			; Select CAN->Logging->Enable Filter log
EndFunc

;===============================================================================
;Function Name : _EnableInterpretMode_LIN
;Functionality : Enables message interpretation
;Input 		   :
;Output 	   :
;===============================================================================

Func _EnableInterpretMode_LIN()
   _EnableOverwriteMode()
	Sleep(1000)
	$winhWnd = WinGetHandle($WIN_BUSMASTER)												; Fetch the window handle
		$hMain = _GUICtrlMenu_GetMenu($winhWnd)												; Fetch the handle of the menu
	 If winexists($WIN_LINMsgWind) Then
		$hFile = _GUICtrlMenu_GetItemSubMenu($hMain, 2)										; Fetch the handle of CAN menu
		ConsoleWrite("Submenu handle : "&$hFile& @CRLF)
	Else
		$hFile = _GUICtrlMenu_GetItemSubMenu($hMain, 1)									  ; Fetch the handle of CAN menu
		ConsoleWrite("Submenu handle : "&$hFile& @CRLF)
	EndIf
	$hSubmenu=_GUICtrlMenu_GetItemSubMenu($hFile, 9)									 ; Fetch the handle of CAN->Msg Window menu
	ConsoleWrite("$hSubmenu handle : "&$hSubmenu& @CRLF)
	$res=_GUICtrlMenu_GetItemChecked($hSubmenu,7)                                      ;Check whether Interpret mode is enabled
	$text=_GUICtrlMenu_GetItemText($hSubmenu,7)
	ConsoleWrite("$text"& $text & @CRLF)
	ConsoleWrite("res1" & $res & @CRLF)
	If($res=False) Then
		$OvrWriteToolhWd=ControlGetHandle($WIN_BUSMASTER,"",$ToolBar_OverWrite)			; Get handle of the 'Connect/Disconnect' toolbar
		_GUICtrlToolbar_ClickIndex($OvrWriteToolhWd,2)									; Click on 'Interpret' icon
		sleep(1000)
	EndIf
EndFunc


;=================================================================================
;Function Name :  _AddNewNodeNS
;Functionality : Adds a node to a sim file
;Input 		   : Nodename,cppFileName,treeview handler of node sim window.
;Output 	   : Returns the Message window data to calling function as object list.
;===============================================================================
Func _AddNewNodeNS($nodeName,$CppFileName,$TVC_Handler)
	$SimTVHWD= ControlGetHandle($WIN_BUSMASTER, "",$TVC_Handler)							; Fetch the handle of tree view
	$SimFileHWD=_GUICtrlTreeView_GetItemHandle($SimTVHWD,"")								; Fetch the first item handle

	_GUICtrlTreeView_ClickItem($SimTVHWD,$SimFileHWD, "right") 								; Right click on the DB path item
	sleep(800)
	send("a")																				; Select "Add" from the right click menu
	sleep(1000)
	if WinWaitActive($WIN_NodeDetails,"",5) Then
		ControlSend($WIN_NodeDetails,"",$TXT_NodeName_NodeDetails,$nodeName)			   	; Enter Filename
		sleep(1000)
		if $cppFileName = "" Then
			ControlClick($WIN_NodeDetails,"",$BTN_OK_NodeDetails) 							; Click on OK button
			sleep(1000)
		Else
			$CPPFilePath = _OutputDataPath()																; Fetch the path of the output data folder
			ControlClick($WIN_NodeDetails,"",$BTN_Brows_NodeDetails)										; Click on the "Add New File" button
			;WinWaitActive($WIN_AddCPPFile,"",5)															; Wait for the window to get active
			Sleep(1000)
			if WinExists($WinselectFile) Then																; If window exits then enter the file name
			ConsoleWrite("Select File -----")
				ControlSend($WinselectFile,"",$TXT_FileName_AddCPPFile,$CPPFilePath&"\"&$CppFileName)   	; Enter Filename
				sleep(1000)
				ControlClick($WinselectFile,"",$BTN_Open_AddCPPFile) 										; Click on Open button
				sleep(500)
			EndIf
		EndIf
		sleep(500)
		ControlClick($WIN_NodeDetails,"",$BTN_OK_NodeDetails) 									; Click on OK button
	EndIf
	sleep(1000)
	;Opt("WinDetectHiddenText", 0)
	;Opt("WinSearchChildren", 1)
	;Opt("WinTitleMatchMode", 1)
	;WinWaitClose($CppFileName&".cpp","",5)
	Send("!-")
	Send("C")

	sleep(1000)
EndFunc
;=================================================================================
;Function Name : _GetCppTVHWD
;Functionality : Fetch the Window handle of the  Cpp tree view control /node sim tree view control
;Input 		   : Tree view control of node sim Window
;Output 	   : Returns the handle of the CAN Cpp tree view control
;=================================================================================
Func _GetCppTVHWD($TVC_Handlers)
	$nodeHWD = ControlGetHandle($WIN_BUSMASTER, "",$TVC_Handlers)  		; Fetch the handle of tree view control
	Return $nodeHWD
EndFunc

;=================================================================================
;Functionality : Fetch the handle handlers in Cpp file
;Input 		   : Switch Case no., Child Item no.
;Output 	   : Returns the handle of the specified child item
;=================================================================================
Func _GetHandlerHWD($TVC_Handlers,$SCaseno,$ChildItemNo)
	$nodeHWD = ControlGetHandle($WIN_BUSMASTER, "",$TVC_Handlers)  		; Fetch the handle of tree view control
	Switch $SCaseno
		Case 1
			$item1HWD = _GUICtrlTreeView_GetNext($nodeHWD, "") 				; Get handle of "include header" child item
			return $item1HWD
		Case 2
			$item1HWD1 = _GUICtrlTreeView_GetNext($nodeHWD, "")				; Get handle of "include header" child item
			$var1=$item1HWD1
			for $i=1 to $ChildItemNo
				$itemHWD = _GUICtrlTreeView_GetNextSibling($nodeHWD, $var1) ; Get handle of $ChildItemNo
				_GUICtrlTreeView_ClickItem($nodeHWD, $var1)
				$var1=$itemHWD
			Next
			return $itemHWD
	EndSwitch
EndFunc
;===============================================================================
;Function Name : _LINNodeSimConfigMenu
;Functionality : Selects LIN->Node Simulation->Configure menu
;Input 		   :
;Output 	   :
;===============================================================================
Func _LINNodeSimConfigMenu()
	WinMenuSelectItem($WIN_BUSMASTER,"",$linMenu,$NodeSimMenu,$NodeSimConfigMenu)		; Select CAN->Node Simulation->Configure from the menu
	sleep(1000)
EndFunc

;===============================================================================
;Function Name : _Connect_CAN_Menu()
;Functionality : Clicks on the connect menu for CAN.
;Input 		   :
;Output 	   :
;===============================================================================
Func _Connect_CAN_Menu()
	WinMenuSelectItem($WIN_BUSMASTER,"",$CANMenu,"&Connect")
	sleep(500)
EndFunc
;===============================================================================
;Function Name : _Maximize_childWindow($WinTitleChild)
;Functionality : maximize child window .
;Input 		   : Title of child window.
;Output 	   :
;===============================================================================
Func _Maximize_childWindow($WinTitleChild)
	Opt("WinDetectHiddenText", 0)
	Opt("WinSearchChildren", 1)
	Opt("WinTitleMatchMode", 1)
	WinWait($WinTitleChild)
	WinSetState($WinTitleChild,"",@SW_MAXIMIZE)
EndFunc


;=================================================================================
;Function Name : _SetViewToolBarLIN
;Functionality : Select toolbar from view for all protocol
;Input 		   :
;Output 	   : -
;=================================================================================
Func _SetViewToolBarLIN()

	Send("!VT")
	sleep(500)
	$winhWnd = WinGetHandle($WIN_BUSMASTER)												; Fetch the window handle
	$hMain2 = _GUICtrlMenu_GetMenu($winhWnd)											; Fetch the handle of the menu
	ConsoleWrite("menu handle2 : "&$hMain2& @CRLF)

	$hFile2 = _GUICtrlMenu_GetItemSubMenu($hMain2, 4)
	$hSubmenu2=_GUICtrlMenu_GetItemSubMenu($hFile2, 3)									; Fetch the handle of View->ToolBar menu
	;ConsoleWrite("$hSubmenu handle : "&$hSubmenu& @CRLF)
	$res2=_GUICtrlMenu_GetItemChecked($hSubmenu2,4)										; Check whether View->ToolBar->LIN is checked or not
	$val2=_GUICtrlMenu_GetItemText($hSubmenu2,4)										; Fetch the text of first item in View->ToolBar->LIN
	ConsoleWrite("$res2 : "&$res2& @CRLF)
	ConsoleWrite("$val2 : "&$val2& @CRLF)
	if $res2=False then																	;
		WinMenuSelectItem($WIN_BUSMASTER,"",$View,$Toolbar,$LINMenu)					; Select tool bar from View
	EndIf


EndFunc
;=================================================================================
;Function Name : _GetWinHndlrNSTV
;Functionality : Get Win handler of TreeView for nodesimulation
;Input 		   :
;Output 	   : -
;=================================================================================
Func _GetWinHndlrNSTV()
	$tempWinHW=ControlGetHandle($WIN_BUSMASTER, "",$TVC_Handlers_CPP)								; Fetch the handle of tree view
	Return $tempWinHW
EndFunc
;=================================================================================
;Function Name : _RightClickonChildItemSelect
;Functionality : Right click on the first child item in node sim ans select the option from menu
;Input 		   : $option
;Output 	   : -
;=================================================================================
Func _RightClickonChildItemSelect($option)

	sleep(1000)
	$tempSimTVHWD=_GetWinHndlrNSTV()
	$tempSimFileHWD=_GUICtrlTreeView_GetItemHandle($tempSimTVHWD,"")									; Fetch the first item handle
	$hndle=_GUICtrlTreeView_GetFirstChild($tempSimTVHWD, $tempSimFileHWD)
	_GUICtrlTreeView_ClickItem($tempSimTVHWD,$hndle, "right") 									; Right click on the First child node
	sleep(800)
	send($option)																				 ; Select "Remove" from the right click menu
	sleep(1000)
EndFunc

;=================================================================================
;Function Name : _CountFunInMsgHndler()
;Functionality : Adds the message handler for the  node sim.
;Input 		   : $HSCaseNoForType(Data Frame,null Frame), $HSCaseNoForBasedOn,$Param1(MsgID, FromMsgID, MsgName),$Param2(ToMsgID)
;Output 	   : -
;=================================================================================
Func _CountFunInMsgHndler()
	$TVHWD=_GetCppTVHWD($TVC_Handlers_CPP)														; Fetch the handle of tree view
	$MsgHWD=_GetHandlerHWD($TVC_Handlers_CPP,2,2)												; Fetch the handle of the Msg handler item
	$FunCount =_GUICtrlTreeView_GetChildCount($TVHWD, $MsgHWD)
	Return $FunCount
EndFunc
;=================================================================================
;Function Name : _SaveAndCloseFunEditor()
;Functionality : Close Function Editor in Node simulation
;Input 		   : $HSCaseNoForType(Data Frame,null Frame), $HSCaseNoForBasedOn,$Param1(MsgID, FromMsgID, MsgName),$Param2(ToMsgID)
;Output 	   : -
;=================================================================================
Func _SaveAndCloseFunEditor()
	Send("!-")
	Send("C")
	if WinWaitActive("","Save changes to",2) Then									; wait for save configuration dialog
		ControlClick($WIN_BUSMASTER,"Save changes to","[CLASS:Button; INSTANCE:1]")						; Click on No button
	EndIf
EndFunc
;=================================================================================
;Function Name : _OutputWindowText()
;Functionality : Return the Tetx contains in Output window using the row no for listBox.
;Input 		   : $Row
;Output 	   : -
;=================================================================================
Func _OutputWindowText($Row)
	Opt("WinDetectHiddenText", 0)
	Opt("WinSearchChildren", 1)
	Opt("WinTitleMatchMode", 1)
	WinWait("Output Window")
	$WinHWOutput=ControlGetHandle($WIN_BUSMASTER, "","[CLASS:ListBox; INSTANCE:1]")			; Fetch the handle of ListbOx for Output Window in busmaster.
	$text = _GUICtrlListBox_GetText($WinHWOutput, $Row)
	Return $text
EndFunc
;=================================================================================
;Function Name : _NodeConfigure
;Functionality : configure Node details (Node Name,cpp file)
;Input 		   : $nodename,$cppfile
;Output 	   : -
;=================================================================================
Func _NodeConfigure($nodename,$cppfile)
	$SimTVHWD= ControlGetHandle($WIN_BUSMASTER, "",$TVC_Handlers_CPP)							; Fetch the handle of tree view
	$SimFileHWD=_GUICtrlTreeView_GetItemHandle($SimTVHWD,"")								; Fetch the first item handle

	if WinWaitActive($WIN_NodeDetails,"",5) Then

		ControlSetText($WIN_NodeDetails, "", $TXT_NodeName_NodeDetails, "")
		ControlSend($WIN_NodeDetails,"",$TXT_NodeName_NodeDetails,$nodeName)			   	; Enter Filename
		sleep(1000)

		$CPPFilePath = _OutputDataPath()																; Fetch the path of the output data folder
		ControlClick($WIN_NodeDetails,"",$BTN_Brows_NodeDetails)										; Click on the "Add New File" button
			;WinWaitActive($WIN_AddCPPFile,"",5)															; Wait for the window to get active
		Sleep(1000)
		if WinExists($WinselectFile) Then																; If window exits then enter the file name
			ConsoleWrite("Select File -----")
			ControlSend($WinselectFile,"",$TXT_FileName_AddCPPFile,$CPPFilePath&"\"&$cppfile)   	; Enter Filename
			sleep(1000)
			ControlClick($WinselectFile,"",$BTN_Open_AddCPPFile) 										; Click on Open button
			sleep(500)
		EndIf

		sleep(500)
		ControlClick($WIN_NodeDetails,"",$BTN_OK_NodeDetails) 									; Click on OK button
	EndIf
	sleep(1000)
	if WinWaitActive("","You are about to change all the Node details!",2) Then									; wait for save configuration dialog
		ControlClick($WIN_BUSMASTER,"You are about to change all the Node details!","[CLASS:Button; INSTANCE:1]")						; Click on No button
	EndIf
	Send("!-")
	Send("C")
EndFunc
;=================================================================================
;Function Name : _GetChildItemDetailsNS
;Functionality : Return the first child item in tree view
;Input 		   :
;Output 	   : -
;=================================================================================
Func _GetChildItemDetailsNS()

	sleep(1000)
	$tempSimTVHWD=_GetWinHndlrNSTV()
	;$tempSimFileHWD=_GUICtrlTreeView_GetItemHandle($tempSimTVHWD,"")									; Fetch the first item handle
	;$hndle=_GUICtrlTreeView_GetFirstChild($tempSimTVHWD, $tempSimFileHWD)
	$childNode=ControlTreeView ( $WIN_BUSMASTER, "", $tempSimTVHWD,"GetText","#0|#0" )						; Select the node
	Return $childNode
EndFunc
;=================================================================================
;Function Name : _RightClickonRootNode
;Functionality : Right click on the Root node sim and select the option from menu
;Input 		   : $option
;Output 	   : -
;=================================================================================
Func _RightClickonRootNode($option)

	$SimTVHWD= ControlGetHandle($WIN_BUSMASTER, "",$TVC_Handlers_CPP)						; Fetch the handle of tree view
	$SimFileHWD=_GUICtrlTreeView_GetItemHandle($SimTVHWD,"")								; Fetch the first item handle
	_GUICtrlTreeView_ClickItem($SimTVHWD,$SimFileHWD, "right") 								; Right click on the DB path item
	sleep(800)
	send($option)																			; Select "Add" from the right click menu
	sleep(1000)
EndFunc
;=================================================================================
;Function Name : _CountNodeInTVNS()
;Functionality : Count the no. of nodes preent in the trre view - Node sim
;Input 		   :
;Output 	   : No. of child node exist
;=================================================================================
Func _CountNodeInTVNS()
	$TVHWD=_GetWinHndlrNSTV()														; Fetch the handle of tree view
	$SimFileHWD=_GUICtrlTreeView_GetItemHandle($TVHWD,"")						; Fetch the first item handle
	$NodeChildCount =_GUICtrlTreeView_GetChildCount($TVHWD,$SimFileHWD)
	Return $NodeChildCount
EndFunc
;=================================================================================
;Function Name : _SelectNodesCheckBox1()
;Functionality : Select the check box es of node using index in node simulation.
;Input 		   :
;Output 	   : Return one/Zero if its checked/Unchecked
;=================================================================================
Func _SelectNodesCheckBox1($index)
	$TVHWD=_GetWinHndlrNSTV()														; Fetch the handle of tree view
	$SimFileHWD=_GUICtrlTreeView_GetItemHandle($TVHWD,"")						; Fetch the first item handle
	_GUICtrlTreeView_SetCheckedByIndex($TVHWD,$SimFileHWD,$index)
	sleep(1000)
	if $index=0 Then
		$handler1=_GUICtrlTreeView_GetFirstChild($TVHWD,$SimFileHWD)
		$chkdUnchkd =_GUICtrlTreeView_SetSelected($TVHWD, $handler1,True)

		;$chkdUnchkd = _GUICtrlTreeView_GetChecked($TVHWD,$handler1)
		;_GUICtrlTreeView_SetSelected($TVHWD, $handler1)
		;Send("{SPACE}")
		;sleep(1000)
		$chkdUnchkd = _GUICtrlTreeView_GetChecked($TVHWD,$handler1)
	Else
		$handler=_GetHandlerHWD($TVHWD,2,$index)
		;$chkdUnchkd =_GUICtrlTreeView_SetSelected($TVHWD, $handler,True)

		;$chkdUnchkd = _GUICtrlTreeView_GetChecked($TVHWD,$handler)
		;_GUICtrlTreeView_SetChecked($hTreeView, $hItem[$hRandomItem])
		;$chkdUnchkd =_GUICtrlTreeView_SetSelected($TVHWD, $handler,True)
		;_GUICtrlTreeView_SelectItem($TVHWD, $handler)
		;Send("{SPACE}")
		;sleep(1000)
		$chkdUnchkd = _GUICtrlTreeView_GetChecked($TVHWD,$handler)
	EndIf
	Return $chkdUnchkd
EndFunc

;=================================================================================
;Function Name : _SelectNodesCheckBox()
;Functionality : Select the check box es of node using index in node simulation.
;Input 		   :
;Output 	   : Return one/Zero if its checked/Unchecked
;=================================================================================
Func _SelectNodesCheckBox($item)
	$TVHWD=_GetWinHndlrNSTV()														; Fetch the handle of tree view
	$SimFileHWD=_GUICtrlTreeView_GetItemHandle($TVHWD,"")						; Fetch the first item handle

	ControlTreeView ( "Configure Simulated Systems - LIN Bus", "", $TVHWD, "Check" ,$item)

EndFunc

;=================================================================================
;Function Name : _ValidateNodecheckBox()
;Functionality : Select the check box es of node using index in node simulation.
;Input 		   :
;Output 	   : Return one/Zero if its checked/Unchecked
;=================================================================================
Func _ValidateNodecheckBox($index)
	$TVHWD=_GetWinHndlrNSTV()														; Fetch the handle of tree view
	$SimFileHWD=_GUICtrlTreeView_GetItemHandle($TVHWD,"")						; Fetch the first item handle

	sleep(1000)
	if $index=0 Then
		$handler1=_GUICtrlTreeView_GetFirstChild($TVHWD,$SimFileHWD)
		$chkdUnchkd = _GUICtrlTreeView_GetChecked($TVHWD,$handler1)
	Else
		$handler=_GetHandlerHWD($TVHWD,2,$index)
		$chkdUnchkd = _GUICtrlTreeView_GetChecked($TVHWD,$handler)
	EndIf
	Return $chkdUnchkd
EndFunc

;=================================================================================
;Function Name : _AddBusevent()
;Functionality : Adds Busevent handler function
;Input 		   : $HSCaseNo(preconnect=1,Connect-2,Disconnect-3)
;Output 	   :
;=================================================================================
Func _AddBusevent($HSCaseNo)
	$TVHWD=_GetCANCppTVHWD()															; Fetch the handle of tree view
	$BusevntHWD=_GetCANHandlerHWD(2,1)													; Fetch the handle of the Msg handler item
	_GUICtrlTreeView_ClickItem($TVHWD, $BusevntHWD, "Right") 							; Right click on Busevent handler item in the tree view
	sleep(500)
	send("a")																			; Select 'Add' from the right click menu
	WinWaitActive($WIN_BusEventhandler,"",5)
	Switch $HSCaseNo
	Case 1
		; preconnect
		ControlCommand($WIN_BusEventhandler,"",$BTN_Preconnect,"Check")	   				; Check preconnect radio Button is enabled
	Case 2
		; connect
		ControlCommand($WIN_BusEventhandler,"",$BTN_connect,"Check")	   				; Check connect radio Button is enabled
	Case 3
		; Disconnect
		ControlCommand($WIN_BusEventhandler,"",$BTN_Disconnect,"Check")	   				; Check Disconnect radio Button is enabled

    EndSwitch
	ControlClick($WIN_BusEventhandler,"",$BTN_OK_BusEvntHandler)		   				; Click on 'OK' button
	sleep(1000)

	;
EndFunc
;=================================================================================
;Function Name : _SelectNodesCheckBox()
;Functionality : Select the chile node in tree view
;Input 		   :
;Output 	   : return text
;=================================================================================
Func _SelectNodes($item)
	$TVHWD=_GetWinHndlrNSTV()														; Fetch the handle of tree view
	$SimFileHWD=_GUICtrlTreeView_GetItemHandle($TVHWD,"")						; Fetch the first item handle

	$text =ControlTreeView ( $WIN_LINNodesim, "", $TVHWD, "GetText" ,$item)
	return $text
EndFunc
;=================================================================================
;Function Name : _AddDLLHandler()
;Functionality :Create DLL Handler function in Node Simulation
;Input 		   : $index [1-Load,2-Unload]
;Output 	   : -
;=================================================================================
Func _AddDLLHandler($index)
	if $index=1 Then
		$dllhndlrChckbox=$BTN_LoadDLLHandler
	ElseIf $index=2 Then
		$dllhndlrChckbox=$BTN_UnLoadDLLHandler
	EndIf
	if WinExists($WIN_BUSMASTER) Then
		$TVHWD=_GetWinHndlrNSTV()															; Fetch the handle of tree view
		$KeyHWD=_GetCANHandlerHWD(2,6)														; Fetch the handle of the key handler item
		_GUICtrlTreeView_ClickItem($TVHWD, $KeyHWD, "Right") 								; Right click on Key Handler item in the tree view
		send("a")																			; Select 'Add' from the right click menu

		if WinWaitActive($WIN_DLLHandler,"",5) Then
			;ControlCommand($WIN_DLLHandler,"",$dllhndlrChckbox,"Check")	   				; Check preconnect radio Button is enabled
			ControlCommand($WIN_DLLHandler,"",$dllhndlrChckbox,"Check")	   								; Check preconnect radio Button is enabled
			ControlClick($WIN_DLLHandler,"",$BTN_OK_DLLHandler)								; Click on 'OK' button
		EndIf
	EndIf
EndFunc
;=================================================================================
;Function Name : _AddUtilityFun()
;Functionality : Create Utility function in Node Simulation
;Input 		   : $FunName
;Output 	   : -
;=================================================================================
Func _AddUtilityFun($Retvalue,$FunName)

	if WinExists($WIN_BUSMASTER) Then
		$TVHWD=_GetWinHndlrNSTV()															; Fetch the handle of tree view
		$KeyHWD=_GetCANHandlerHWD(2,7)														; Fetch the handle of the key handler item
		_GUICtrlTreeView_ClickItem($TVHWD, $KeyHWD, "Right") 								; Right click on Key Handler item in the tree view
		send("a")																			; Select 'Add' from the right click menu
		Sleep(1000)
		if WinWaitActive($WIN_UtilityFun,"",5) Then
			;$hComBo=ControlGetHandle($WIN_UtilityFun,"","[CLASS:Edit; INSTANCE:1]")
			;$aList = _GUICtrlComboBox_SelectString($hComBo,$Retvalue,0)
			ControlCommand ($WIN_UtilityFun, "",$COMB_UtilFun_RetValue,"SelectString",$Retvalue)
			;controlcommand($WIN_LINLog,"",$COMB_TimeMode_ConfigLOG,"SelectString", $TimeMode)			; Set the time mode
			sleep(1000)
			ControlSend($WIN_UtilityFun,"",$TXT_UtilityFun,$FunName)			   			; Type Filename
			ControlClick($WIN_UtilityFun,"",$BTN_OK_UtilFun)								; Click on 'OK' button
		EndIf
	EndIf
EndFunc
;=================================================================================
;Function Name : _AddDLLHandler()
;Functionality : Create Error  Handler function in Node Simulation
;Input 		   : $index [1-ErrChksm,2-ErrRcvfrm,3-ErrSlvNoRspns,4-ErrSynch]
;Output 	   : -
;=================================================================================
Func _AddErrHandler($index)
	if $index=1 Then
		$ErrhndlrChckbox=$BTN_ErrChksmERRHandler
	ElseIf $index=2 Then
		$ErrhndlrChckbox=$BTN_ErrRcvfrmERRHandler

	ElseIf $index=3 Then
		$ErrhndlrChckbox=$BTN_ErrSlvNoRspnsERRHandler

	ElseIf $index=4 Then
		$ErrhndlrChckbox=$BTN_ErrErrSynchERRHandler
	EndIf
	if WinExists($WIN_BUSMASTER) Then
		$TVHWD=_GetWinHndlrNSTV()															; Fetch the handle of tree view
		$ErrHWD=_GetCANHandlerHWD(2,5)														; Fetch the handle of the Error handler item
		_GUICtrlTreeView_ClickItem($TVHWD, $ErrHWD, "Right") 								; Right click on Error Handler item in the tree view
		send("a")																			; Select 'Add' from the right click menu

		if WinWaitActive($WIN_ERRHandler,"",5) Then

			ControlCommand($WIN_ERRHandler,"",$ErrhndlrChckbox,"Check")	   					; Check check boxes in dialog box .
			ControlClick($WIN_ERRHandler,"",$BTN_OK_ERRHandler)								; Click on 'OK' button
		EndIf
	EndIf
EndFunc
;===============================================================================
;Function Name : _MsgDisplayMenuLIN
;Functionality : Selects message display from the menu - LIN
;Input 		   :
;Output 	   :
;===============================================================================
Func _MsgDisplayMenuLIN()
	WinMenuSelectItem($WIN_BUSMASTER,"",$linMenu,$LINMsgWinMenu,$configMsgWindowLIN)
EndFunc
;===============================================================================
;Function Name : _EnableFilterDispMenuLIN
;Functionality : Enable filters for message display from menu of LIN
;Input 		   :
;Output 	   :
;===============================================================================
Func _EnableFilterDispMenuLIN()
	WinMenuSelectItem($WIN_BUSMASTER,"",$linMenu,$LINMsgWinMenu,$EnableFilterLin)			; Select CAN->MessageWin->Enable Filter
	sleep(750)
EndFunc
;---Can Tx Window---------------------------------------------------------------
;===============================================================================
;Function Name : _GetCanTxWinsHWD
;Functionality : Get Window handle of Tx Window
;Input 		   :
;Output 	   :
;===============================================================================
Func _GetTxWindHWD()
	$HWD=ControlGetHandle($WIN_BUSMASTER,"",$LVC_TxWin)								; Fetch the handle of test editor details
	Return $HWD
EndFunc
;===============================================================================
;Function Name : _GetRowPosTxWin
;Functionality : Get Row position in CAN->TxWin->TxFrameList
;Input 		   : $rowNo
;Output 	   :
;===============================================================================
Func _GetRowPosTxWin($hWndCanTx,$rowNo)
	$MsgItemPos=_GUICtrlListView_GetItemPosition($hWndCanTx,$rowNo)											; Fetch the First row position in Tx Window
	Return $MsgItemPos
EndFunc
;===============================================================================
;Function Name : _GetColWidthTxWin
;Functionality : Get Col Width in CAN->TxWin->TxFrameList
;Input 		   : $colNo
;Output 	   :
;===============================================================================
Func _GetColWidthTxWin($hWndCanTx,$colNo)
	$CtgColWidth=_GUICtrlListView_GetColumnWidth($hWndCanTx,$colNo)										; Fetch the first coulmn width
	Return $CtgColWidth
EndFunc
;===============================================================================
;Function Name : _SelectChnlTxWin
;Functionality : Select Channel For message in tx frame List.
;Input 		   : $chnlNo,$pos_X,$pos_Y
;Output 	   :
;===============================================================================
Func _SelectChnlTxWin($chnlNo,$pos_X,$pos_Y)
	$LVhWndCanTx1=_GetTxWindHWD()
	;ControlClick($WIN_BUSMASTER,"",$LVhWndCanTx,"Left",2,$VerifyMsgItemPos[0]+$ColWidth+262,$VerifyMsgItemPos[1])					; Double click on the Message Name Column
	ControlClick($WIN_BUSMASTER,"",$LVhWndCanTx1,"Left",2,$pos_X,$pos_Y)					; Double click on the Message Name Column
	sleep(1000)
	if $chnlNo>1 Then
		for $i=0 to $chnlNo-1
			send("{DOWN}")
			sleep(1000)
		Next
		send("{ENTER}")
		sleep(1000)

	EndIf
EndFunc
;===============================================================================
;Function Name : _SetDtaLngthTxWin
;Functionality : Select datalength For message in tx frame List.
;Input 		   : $colNo
;Output 	   :
;===============================================================================
Func _SetTextTxWin($datalngth,$pos_X,$pos_Y)
	$LVhWndCanTx1=_GetTxWindHWD()
	ControlClick($WIN_BUSMASTER,"",$LVhWndCanTx1,"Left",2,$pos_X,$pos_Y)					; Double click on the datalength Column
	sleep(200)
	Send($datalngth)
	sleep(200)
	send("{ENTER}")
	sleep(500)

EndFunc
;===============================================================================
;Function Name : _ComboBoxSelectTxWin
;Functionality : Select datalength For message in tx frame List.
;Input 		   : $colNo
;Output 	   :
;===============================================================================
Func _ComboBoxSelectTxWin($messageid,$pos_X,$pos_Y)
	$LVhWndCanTx1=_GetTxWindHWD()
	ControlClick($WIN_BUSMASTER,"",$LVhWndCanTx1,"Left",2,$pos_X,$pos_Y)					; Double click on the Message Name Column
	sleep(200)
	send("{DOWN "&$messageid&"}")																			        ; Select the msg
	sleep(200)
	send("{ENTER}")
	sleep(200)
EndFunc
;===============================================================================
;Function Name : _ConfigTXWinDetails
;Functionality : Configure Tx Window details
;Input 		   : $Rownum,$HSCaseNo(db,ndb),$messageid/Index(For dbMessage),chnl(e.g-1,2),datalength,Messageype,RTR("Yes"/"No"),Repetition,Key
;Output 	   :
;===============================================================================
Func  _ConfigTXWinDetails($Rownum,$HSCaseNo,$messageid,$chnl,$datalength,$MessageType,$RTR,$Reption,$Key)
   ;_Maximize_childWindow($WinCANTxWindow)

	if WinActive($WIN_BUSMASTER) Then

		$LVhWndCanTx=_GetTxWindHWD()																						; Get Window handle of Tx Window.
		ConsoleWrite("$LVhWndCanTx"&$LVhWndCanTx&@CRLF)
		$VerifyMsgItemPos=_GetRowPosTxWin($LVhWndCanTx,$Rownum)
		$ColWidth=_GetColWidthTxWin($LVhWndCanTx,0)
		ConsoleWrite("$VerifyMsgItemPos0="&$VerifyMsgItemPos[0]&@CRLF)
		ConsoleWrite("$VerifyMsgItemPos1="&$VerifyMsgItemPos[1]&@CRLF)
		sleep(1000)

		Switch $HSCaseNo
		Case "db"

			_ComboBoxSelectTxWin($messageid,$VerifyMsgItemPos[0],$VerifyMsgItemPos[1])

		Case "ndb"

			ControlClick($WIN_BUSMASTER,"",$LVhWndCanTx,"Left",2,$VerifyMsgItemPos[0],$VerifyMsgItemPos[1])					; Double click on the Message Name Column
			sleep(200)
			send($messageid)																			        ; Select the msg
			sleep(200)
			send("{ENTER}")
			sleep(200)

		EndSwitch
		Sleep(1000)
		if $chnl<>"" Then
			$ColWidth=_GetColWidthTxWin($LVhWndCanTx,2)
			_SelectChnlTxWin($chnl,$VerifyMsgItemPos[0]+$ColWidth+262,$VerifyMsgItemPos[1])

		EndIf
		if $datalength<>"" Then
			$ColWidth=_GetColWidthTxWin($LVhWndCanTx,3)
			_SetTextTxWin($datalength,$VerifyMsgItemPos[0]+$ColWidth+362,$VerifyMsgItemPos[1])
		EndIf
		if $MessageType<>"" Then
			$ColWidth=_GetColWidthTxWin($LVhWndCanTx,4)
			ConsoleWrite("$ColWidth---"&$ColWidth&@CRLF)
			if $MessageType="Master" Then
				$MessageType=1
				_ComboBoxSelectTxWinMstSlv($MessageType,$VerifyMsgItemPos[0]+$ColWidth+462,$VerifyMsgItemPos[1])
			ElseIf $MessageType="Slave" Then
				$MessageType=2
				_ComboBoxSelectTxWinMstSlv($MessageType,$VerifyMsgItemPos[0]+$ColWidth+462,$VerifyMsgItemPos[1])
			EndIf
			_SelectChnlTxWin($MessageType,$VerifyMsgItemPos[0]+$ColWidth+462,$VerifyMsgItemPos[1])
			sleep(1000)
		EndIf
		if $RTR="Yes" Then
			$ColWidth2=_GetColWidthTxWin($LVhWndCanTx,5)
			ConsoleWrite("$ColWidth2---"&$ColWidth&@CRLF)
			ControlClick($WIN_BUSMASTER,"",$LVhWndCanTx,"Left",2,$VerifyMsgItemPos[0]+$ColWidth2+540,$VerifyMsgItemPos[1])					; Double click on the Message Name Column
			sleep(1000)
		EndIf
		if $Reption<>"" Then
			$ColWidth=_GetColWidthTxWin($LVhWndCanTx,6)
			ConsoleWrite("$ColWidth3---"&$ColWidth&@CRLF)
			ControlClick($WIN_BUSMASTER,"",$LVhWndCanTx,"Left",2,$VerifyMsgItemPos[0]+$ColWidth+520,$VerifyMsgItemPos[1])					; Double click on the Message Name Column
			sleep(200)
			_SetTextTxWin($Reption,$VerifyMsgItemPos[0]+$ColWidth+562,$VerifyMsgItemPos[1])
			sleep(200)
		EndIf
		if $Key<>"" Then
			ConsoleWrite("$MessageType="&$MessageType&@CRLF)
			if $MessageType=1 Or $MessageType=2 Then
				ConsoleWrite("-------------------------------------------"&@CRLF)
				$ColWidthK=_GetColWidthTxWin($LVhWndCanTx,6)
				ConsoleWrite("$ColWidth44---"&$ColWidthK&@CRLF)
				ControlClick($WIN_BUSMASTER,"",$LVhWndCanTx,"Left",2,$VerifyMsgItemPos[0]+$ColWidthK+620,$VerifyMsgItemPos[1])					; Double click on the Message Name Column
				sleep(200)
				_SetTextTxWin($Key,$VerifyMsgItemPos[0]+$ColWidthK+670,$VerifyMsgItemPos[1])
				sleep(200)
			Else
				$ColWidth1=_GetColWidthTxWin($LVhWndCanTx,7)
				;$ColWidth1=_GetColWidthTxWin($LVhWndCanTx,7)
				ControlClick($WIN_BUSMASTER,"",$LVhWndCanTx,"","",$VerifyMsgItemPos[0]+$ColWidth1,$VerifyMsgItemPos[1])					; Double click on the Message Name Column
				sleep(1000)
				_SetTextTxWin($Key,$VerifyMsgItemPos[0]+$ColWidth1+662,$VerifyMsgItemPos[1])
				sleep(1000)
			EndIf
		EndIf

	EndIf

EndFunc


;===============================================================================
;Function Name : _CloseTxWindowCAN
;Functionality : Closes the TX window
;Input 		   :
;Output 	   :
;===============================================================================
;Func _CloseTxWindowArg($protcolname)
Func _CloseTxWindowArg($protcolname)
	Opt("WinDetectHiddenText", 0)
	Opt("WinSearchChildren", 1)
	Opt("WinTitleMatchMode", 1)
	;WinWait($WindTxWindow&"$protcolname")
	;WinSetState($WindTxWindow&"$protcolname","",@SW_MAXIMIZE)
	WinClose($WindTxWindow&$protcolname)
EndFunc
;===============================================================================
;Function Name : _ActivatechildWindow
;Functionality : Closes the TX window
;Input 		   : $WinTitle
;Output 	   :
;===============================================================================
Func _ActivatechildWindow($WinTitle)
	Opt("WinDetectHiddenText", 0)
	Opt("WinSearchChildren", 1)
	Opt("WinTitleMatchMode", 1)
	WinActivate($WinTitle)
EndFunc
;===============================================================================
;Function Name : _SendMsgTxWindow
;Functionality : Click on Send Message button in Tx Window
;Input 		   : $WinTitleTXwin
;Output 	   :
;===============================================================================
Func _SendMsgTxWindow()
	ControlClick($WIN_BUSMASTER, "Send Message", $BTN_SendTxWin)
EndFunc
;===============================================================================
;Function Name : _DelMsgTxWindow
;Functionality : Delete Msg from TX Window
;Input 		   : $RowNo
;Output 	   :
;===============================================================================
Func _DelMsgTxWindow($RowNo)
	$LVhWndTx=_GetTxWindHWD()																						; Get Window handle of Tx Window.
	ConsoleWrite("$LVhWndCanTx="&$LVhWndTx&@CRLF)
	$VerifyMsgItemPoss=_GetRowPosTxWin($LVhWndTx,$RowNo)
	ControlClick($WIN_BUSMASTER,"",$LVhWndTx,"Left",1,$VerifyMsgItemPoss[0]+200,$VerifyMsgItemPoss[1])				; Double click on the Message Name Column
	sleep(1000)
	ControlClick($WIN_BUSMASTER, "Delete", $BTN_Del_TxWindow)
	sleep(1000)
EndFunc
;===============================================================================
;Function Name : _CountMsgTxFrameList
;Functionality : Coount no. of Messages are available in Tx Frame List.
;Input 		   :
;Output 	   : Return No.of Messages in List._DelMsgTxWindow
;===============================================================================
Func _CountMsgTxFrameList()
	$LVhWndTx=_GetTxWindHWD()
	$countMsgList=_GUICtrlListView_GetItemCount($LVhWndTx)
	ConsoleWrite("$countMsgList="&$countMsgList&@CRLF)
	return $countMsgList-1
EndFunc
;===============================================================================
;Function Name : _DelMsgALLTxWindow
;Functionality : Click on DeleteALL button in TxWindow and ensure all the message are deleted.
;Input 		   : $evnt="Yes"/"No"
;Output 	   : Return No.of Messages in List.
;===============================================================================
Func _DelMsgALLTxWindow()
	ControlClick($WIN_BUSMASTER, "Delete", $BTN_DelALL_TxWindow)
	sleep(1000)
EndFunc
;===============================================================================
;Function Name : _ComboBoxSelectTxWinMstSlv
;Functionality : Select datalength For message in tx frame List.
;Input 		   : $colNo
;Output 	   :
;===============================================================================
Func _ComboBoxSelectTxWinMstSlv($messageid,$pos_X,$pos_Y)
	$LVhWndCanTx1=_GetTxWindHWD()
	ControlClick($WIN_BUSMASTER,"",$LVhWndCanTx1,"Left",2,$pos_X,$pos_Y)					; Double click on the Message Name Column
	sleep(200)
	if $messageid=1 Then
		send("{UP "&$messageid&"}")
		send("{ENTER}")
		sleep(200)
	Else
		send("{DOWN "&$messageid&"}")																			        ; Select the msg
		sleep(200)
		send("{ENTER}")
		sleep(200)
	EndIf
EndFunc
;===============================================================================
;Function Name : _SelectRowTxWindow
;Functionality : Select row in Tx Window
;Input 		   : $RowNo
;Output 	   :
;===============================================================================
Func _SelectRowTxWindow($RowNo)
	$LVhWndTx=_GetTxWindHWD()																						; Get Window handle of Tx Window.
	ConsoleWrite("$LVhWndCanTx="&$LVhWndTx&@CRLF)
	$VerifyMsgItemPoss=_GetRowPosTxWin($LVhWndTx,$RowNo)
	ControlClick($WIN_BUSMASTER,"",$LVhWndTx,"Left",1,$VerifyMsgItemPoss[0]+200,$VerifyMsgItemPoss[1])				; Double click on the Message Name Column
	sleep(1000)
EndFunc
;===============================================================================
;Function Name : _updateDataByte
;Functionality : Update for selected Message.
;Input 		   : $RowNo[index-0..n],$ByteNo [1 to 8],$Data
;Output 	   :
;===============================================================================
Func _updateDataByte($RowNo,$ByteNo,$Data)

	if $RowNo<>"" Then
		$LVhWndTx=_GetTxWindHWD()																						; Get Window handle of Tx Window.
		ConsoleWrite("$LVhWndCanTx="&$LVhWndTx&@CRLF)
		$VerifyMsgItemPoss=_GetRowPosTxWin($LVhWndTx,$RowNo)
		ControlClick($WIN_BUSMASTER,"",$LVhWndTx,"Left",1,$VerifyMsgItemPoss[0]+200,$VerifyMsgItemPoss[1])				; Double click on the Message Name Column
	EndIf
	$HWD=ControlGetHandle($WIN_BUSMASTER,"","[CLASS:SysListView32; INSTANCE:3]")
	$ItemPos=_GetRowPosTxWin($HWD,0)															; get Item Position
	sleep(500)
	if $ByteNo=1 Then
		$Value=100
	Else
		$Value=100+90*($ByteNo-1)
	EndIf
	ControlClick($WIN_BUSMASTER,"",$HWD,"Left",2,$ItemPos[0]+$Value,$ItemPos[1])                    	; Double Click on databyte
	sleep(500)
	Send($Data)																		; Write databyte Value in Text Box.
	Send("{ENTER}")
	sleep(500)
	;$Value=0
EndFunc
;---------------- Added by Shilpa-----------------------------------------
;===============================================================================
;Function Name : LogFileExist
;Functionality : Check whether the log file exists
;Input 		   : $logfileName
;Output 	   :
;===============================================================================
Func LogFileExist($logfileName)
	$FileExist=FileExists(_OutputDataPath()&"\"&$logfileName&".log")
	Return $FileExist
EndFunc


;===============================================================================
;Function Name : _ConfigureCANLogWithLogpath
;Functionality : Configure Can log with only log path
;Input 		   : $logFileName
;Output 	   :
;===============================================================================
Func _ConfigureCANLogWithLogpath($logFileName)
	WinMenuSelectItem($WIN_BUSMASTER,"",$CANMenu,$CANLogMenu,$configureMenu)					; Select Configure->Log
	Sleep(1000)
	ControlClick($WIN_CANLog,"",$BTN_Add_ConfigLOG)												; Click on Add button
	sleep(250)
	ControlCommand ($WIN_CANLog,"",$BTN_EnDisable_ConfigLOG,"Check")							; Check Enable/Disable Logging during connect/disconnect
	$LogFilePath=_OutputDataPath()
	ControlClick($WIN_CANLog,"",$BTN_logFPath_ConfigLOG)										; Click on Log File Path button
	sleep(1000)
	if winexists($WIN_Select_LogFile) Then
		controlsend($WIN_Select_LogFile,"",$TXT_FileName_SelLogFile,$LogFilePath & "\" & $logFileName)	; Enter the Log file Name
		controlclick($WIN_Select_LogFile,"",$BTN_SaveInst_SelLogFile)									; Click on 'Save' button
	EndIf
EndFunc
;===============================================================================
;Function Name : _GetlogFileData
;Functionality : Configure Can log with only log path
;Input 		   : $logFileName
;Output 	   :
;===============================================================================
Func _GetlogFileData($lofFile,$RowNo)
            $GetLogFile_Path=_OutputDataPath()
            $data=FileReadLine ($GetLogFile_Path &"\"& $lofFile,$RowNo)                                                                           ; Read the 17th line from the Log file
            consolewrite("$data :"&$data&@CRLF)
            $Retdata=StringSplit($data," ")
			return $Retdata
EndFunc

;~ Func _log("",CAN,Driver,hex,channel)
;~ 	Return TRUE
;~ EndFunc


;===============================================================================
;Function Name : _LogfileTimeCompare
;Functionality : Configure Can log with only log path
;Input 		   : time1,time 2 (time in the 24-hour format (hh:mm:ss))
;Output 	   :
;===============================================================================
Func _LogfileTimeCompare($time1,$time2)
	$output =0
	$T1 =StringSplit($time1,":")
	$T2 =StringSplit($time2,":")
	if ($T1[1] > $T2[1] ) Then
		$output=1
		return $output
	EndIf

	if ($T1[1] < $T2[1] ) Then
		$output=-1
		return $output
	EndIf

	if ($T1[1] = $T2[1] ) Then
		if($T1[2] > $T2[2] ) Then
		$output=1
		return $output
		EndIf

		if($T1[2] < $T2[2] ) Then
		$output=-1
		return $output
		EndIf

		if($T1[2] = $T2[2] ) Then
		   if($T1[3] > $T2[3] ) Then
		   $output=1
		   return $output
		   EndIf

		   if($T1[3] = $T2[3] ) Then
		   $output=0
		   return $output
		   EndIf

		   if($T1[3] < $T2[3] ) Then
		   $output=-1
		   return $output
		   EndIf

		EndIf
	EndIf
EndFunc

;===============================================================================
;Function Name : _CheckStartAndEndOfLogfile
;Functionality : To check the start of comment and end of comment entry in the logfile.
;Input 		   : filename
;Output 	   :
;===============================================================================
Func _CheckStartAndEndOfLogfile($filename)
  $retValue=0;
  $logfileStartContentToCompare="";
  $startline="";
  $logfileEndContentToCompare="";
  $endline="";
  $logfileStartContent = _GetlogFileData($filename,"1")
  if $logfileStartContent[0]>=3 Then
	  $temp=StringTrimLeft ( $logfileStartContent[3], 5 )
	  $version=StringReplace ( $logfileStartContent[3], $temp,"")
	  $logfileStartContentToCompare =$logfileStartContent[1]&" "&$logfileStartContent[2]&" "&$version
	  $BusmasterVersion=StringSplit(_GetSoftVersion(),'v')
	  $startline1 ="***BUSMASTER Ver "&$BusmasterVersion[2]
	  Local $startline = StringStripWS($startline1, 2)
	  ;$startline ="***BUSMASTER Ver "&$version
	  ConsoleWrite("$logfileStartContentToCompare  :" &$logfileStartContentToCompare & @crlf)
	  ConsoleWrite("$startline  :" &$startline & @crlf)
  EndIf
  $NoOfLines=_FileCountLines ( _OutputDataPath()&"\" &$filename )
  ConsoleWrite("$NoOfLines  :" &$NoOfLines & @crlf)
  $logfileEndContent = _GetlogFileData($filename,$NoOfLines-1)
  $endline="***END DATE AND TIME"
  if $logfileEndContent[0]>=4 Then
	  $logfileEndContentToCompare =$logfileEndContent[1]&" "&$logfileEndContent[2]&" "&$logfileEndContent[3]&" "&$logfileEndContent[4]
	  ConsoleWrite("$logfileEndContentToCompare  :" &$logfileEndContentToCompare & @crlf)
  EndIf
  if Not $logfileStartContentToCompare="" And Not $logfileEndContentToCompare="" then
	  if $logfileStartContentToCompare=$startline And $logfileEndContentToCompare=$endline Then
		  $retValue=1
	  EndIf
  EndIf
  return $retValue
EndFunc


;===============================================================================
;Function Name : _CheckLogfileTimeIsEqual
;Functionality : To check logfile created times are same or not
;Input 		   : time1,time 2 (time in the 24-hour format (hh:mm:ss))
;Output 	   :1 - equal  : 0 - Not equal
;===============================================================================
Func _CheckLogfileTimeIsEqual($time1,$time2)
	$output =1
	if Not $time1=0 And Not $time2=0 Then
		$T1 =StringSplit($time1,":")
		$T2 =StringSplit($time2,":")
		if Not($T1[1] = $T2[1] ) Or Not($T1[2] = $T2[2] ) Or Not($T1[3] = $T2[3] ) Then
			$output=0
		EndIf
	EndIf
	return $output
EndFunc

;===============================================================================
;Function Name : _ConfigureLINLogWithLogpath
;Functionality : Configure Can log with only log path
;Input 		   : $logFileName
;Output 	   :
;===============================================================================
Func _ConfigureLINLogWithLogpath($logFileName)
	WinMenuSelectItem($WIN_BUSMASTER,"",$linMenu,$LINLogMenu,$configureMenu)				    ; Select Configure->Log
	Sleep(1000)
	ControlClick($WIN_LINLog,"",$BTN_Add_ConfigLOG)												; Click on Add button
	sleep(250)
	ControlCommand ($WIN_LINLog,"",$BTN_EnDisable_ConfigLOG,"Check")							; Check Enable/Disable Logging during connect/disconnect
	$LogFilePath=_OutputDataPath()
	ControlClick($WIN_LINLog,"",$BTN_logFPath_ConfigLOG)										; Click on Log File Path button
	sleep(1000)
	if winexists($WIN_Select_LogFile) Then
		controlsend($WIN_Select_LogFile,"",$TXT_FileName_SelLogFile,$LogFilePath & "\" & $logFileName)	; Enter the Log file Name
		controlclick($WIN_Select_LogFile,"",$BTN_SaveInst_SelLogFile)									; Click on 'Save' button
	EndIf
EndFunc
;==========================================================================================================
;Function Name 		: _loadConfigFileAnyloc
;Functionality 		: loads cfx file
;Input 		   		: Config File Name with Path
;Output		   		: -
;==========================================================================================================

Func _loadConfigFileAnyloc($pathwithCFX)
	$sMenu=WinMenuSelectItem($WIN_BUSMASTER,"",$fileMenu,$loadMenu)					; Select File->Configuration->Load
	sleep(1000)
	if WinWait($WIN_BUSMASTER,$saveConfigtxt,2) Then								; wait for save configuration dialog
		ControlClick($WIN_BUSMASTER,"",$BTN_No_SaveConfig)							; Click on No button
	EndIf
	sleep(1000)
	;$DirPath = _TestDataPath()														; Set the DirPath to save the dbf file
	WinWaitActive($WIN_LoadCfx,"",5)
	ConsoleWrite("$pathwithCFX " &$pathwithCFX & @CRLF)
	if winexists($WIN_LoadCfx) Then
		$FileExt=StringInStr($pathwithCFX,".cfx")
		if $FileExt=0 Then
			$pathwithCFX=$pathwithCFX&".cfx"
		EndIf
		ControlSetText($WIN_LoadCfx,"",$TXT_FileName_LoadCfx,$pathwithCFX)	; Set the filename in 'Load Configuration Filename...' dialog
		sleep(500)
		$cConfig=ControlClick($WIN_LoadCfx,"",$BTN_LoadInst_LoadCfx)				; Click on Load button
		sleep(2500)
	EndIf
	_HandleSaveSimSysWin()															; Check for Save Simulated System window
EndFunc

