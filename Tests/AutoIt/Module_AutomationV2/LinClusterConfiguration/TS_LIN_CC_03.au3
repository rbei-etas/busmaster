;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_LIN_CC_03
; TestCases			:		Cluster Configuration [Enabling Master Mode]
; Test Data			:
; Test Setup		:		1. Select LIN>Cluster Configuration
;							2. Select browse button under import LIN database
;							3. Select a ldf file.
;							4. Enable Master Mode.
;							5. Click on Ok button

; Expected Results  : 		After step5, master mode will be enabled
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_LIN_CC_03.au3****"&@CRLF)


_launchApp()


WinActivate($WIN_BUSMASTER)


local $clstrWinLinOpened=0,$LDFFilepath1=0,$LDFFilePathCC="",$chnlSel=0,$enablemasterChecked=0


if winexists($WIN_BUSMASTER) then

	_createConfig("cfxClstrCnfigLIN_03")																; Load Configuration

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

			;ControlCommand($WIN_ClustrConfigLIN, "",$ChannelSelInCCLIN, "SelectString", "Channel 1") 	; Select chaneel from the combobox

			$ChannelSel = ControlGetText($WIN_ClustrConfigLIN, "", $ChannelSelInCCLIN)
			ConsoleWrite("$ChannelSel="&$ChannelSel&@CRLF)
			if $ChannelSel = "Channel 1" Then
				$chnlSel = 1
			Endif

			;Enable master mode

			ControlCommand($WIN_ClustrConfigLIN, "",$chckBoxEnableMaster, "Check", "") 					; Select Check box for enable Master mode.
			Sleep(1000)
			$enablemasterChecked = ControlCommand($WIN_ClustrConfigLIN, "",$chckBoxEnableMaster, "IsChecked", "")
			ControlClick($WIN_ClustrConfigLIN,"","OK")

	EndIf


EndIf

ConsoleWrite("$clstrWinLinOpened="&$clstrWinLinOpened&@CRLF)
ConsoleWrite("$LDFFilepath1="&$LDFFilepath1&@CRLF)
ConsoleWrite("$enablemasterChecked="&$enablemasterChecked&@CRLF)


if $clstrWinLinOpened=1 and $LDFFilepath1=1 and $enablemasterChecked=1 Then									; Validate the result
	_WriteResult("Pass","TS_LIN_CC_03")
Else
	_WriteResult("Fail","TS_LIN_CC_03")
EndIf

$isAppNotRes=_CloseApp()															; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_LIN_CC_03")
EndIf


ConsoleWrite("****End : TS_LIN_CC_03****"&@CRLF)
ConsoleWrite(@CRLF)