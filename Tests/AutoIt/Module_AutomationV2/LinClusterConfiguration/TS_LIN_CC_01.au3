;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_LIN_CC_01
; TestCases			:		Cluster Configuration [Importing LDF file]
; Test Data			:
; Test Setup		:		1. Select LIN>Cluster Configuration
;							2. Select browse button under import LIN database
;							3. Select a ldf file.

; Expected Results  : 		1. After step1, "Cluster Configuration" window will open.
;							2. After step2, dialog will open to select and associate a database.
;							And it should allow to select  LDF files of extension .ldf only.
;							3. After step3, verify that the following items are displayed :
;							a) LDF file path
;							b) Channel: Physical Channel. Available channels listed in the LDF file should be listed in a combo box for user selection.
;							c) Cluster and ECU selection field.
 ;							ECUs to be associated for the selected channel. Available ECUs in the LDF file should be displayed for the user to select the interested ECUs.
;							d) LIN Network Settings: Options to overwrite the LDF File setting like baud rate, protocol version.
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_LIN_CC_01.au3****"&@CRLF)


_launchApp()


WinActivate($WIN_BUSMASTER)


local $clstrWinLinOpened=0,$LDFFilepath1=0,$LDFFilePathCC="",$chnlSel=0,$countItemECU=0
Global $ECUList[10]=["","","","","","","","","",""]
local $outtxt[200]
if winexists($WIN_BUSMASTER) then

	_createConfig("cfxClstrCnfigLIN_01")																; Load Configuration

	_openLINClusterConfig()

	;1. After step1, "Cluster Configuration" window will open.

	if winexists($WIN_ClustrConfigLIN) then																; Check cluster config window is opened
			$clstrWinLinOpened = 1
			_importLDF("TestLIN2_1.ldf")															; Import LDF file in Cluster config

			sleep(1000)

			$LDFFilePathCC = ControlGetText($WIN_ClustrConfigLIN, "", $LDFFilePath)
			ConsoleWrite("$LDFFilePathCC="&$LDFFilePathCC&@CRLF)

			;$PathLDF=
			if $LDFFilePathCC = _TestDataPath()&"\TestLIN2_1.ldf" Then
				$LDFFilepath1 = 1																		; Check LDF File Path
			EndIf

			ControlCommand($WIN_ClustrConfigLIN, "",$ChannelSelInCCLIN, "SelectString", "Channel 1") 	; Select signal type from the combobox

			$ChannelSel = ControlGetText($WIN_ClustrConfigLIN, "", $ChannelSelInCCLIN)
			ConsoleWrite("$ChannelSel="&$ChannelSel&@CRLF)
			if $ChannelSel = "Channel 1" Then
				$chnlSel = 1
			Endif

			$HWDECUlist=_GetLINecuListWinHWD()
			;$countItemECU=_GUICtrlListBox_GetCount($HWDECUlist)
			$countItemECU=_GUICtrlListView_GetItemCount($HWDECUlist)
			ConsoleWrite("$countItemECU"&$countItemECU&@CRLF)

			if $countItemECU = 3 Then
				for $i=0 to $countItemECU-1
					;$ECUList[$i]=_GUICtrlListBox_GetText($HWDECUlist, $i)
					$ECUList[$i]=_GUICtrlListView_GetItemText($HWDECUlist, $i)
					ConsoleWrite("$ECUList="&$ECUList[$i]&@CRLF)
				Next
			EndIf
			Sleep(1000)
			ControlClick($WIN_ClustrConfigLIN,"","OK")

	EndIf


EndIf

ConsoleWrite("$clstrWinLinOpened="&$clstrWinLinOpened&@CRLF)
ConsoleWrite("$LDFFilepath1="&$LDFFilepath1&@CRLF)
ConsoleWrite("$countItemECU="&$countItemECU&@CRLF)
ConsoleWrite("$chnlSel="&$chnlSel&@CRLF)
;ConsoleWrite("$ECUList="&$ECUList[$i]&@CRLF)

if $clstrWinLinOpened=1 and $LDFFilepath1=1 and $countItemECU=3 and $chnlSel=1 Then									; Validate the result
	_WriteResult("Pass","TS_LIN_CC_01")
Else
	_WriteResult("Fail","TS_LIN_CC_01")
EndIf

$isAppNotRes=_CloseApp()															; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_LIN_CC_01")
EndIf


ConsoleWrite("****End : TS_LIN_CC_01****"&@CRLF)
ConsoleWrite(@CRLF)