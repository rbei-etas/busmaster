;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_LIN_CC_02
; TestCases			:		Cluster Configuration [Enabling 'Overwrite LDF Settings']
; Test Data			:
; Test Setup		:		1. Select LIN>Cluster Configuration
;							2. Select browse button under import LIN database
;							3. Select a ldf file.
;							4. Enable Overwrite LDF Settings. Change the protocol version and baud rate.
;							5. Click on Ok button

; Expected Results  : 		After step5, modified overwrite LDF settings will be updated.
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_LIN_CC_02.au3****"&@CRLF)


_launchApp()


WinActivate($WIN_BUSMASTER)


local $clstrWinLinOpened=0,$LDFFilepath1=0,$LDFFilePathCC="",$chnlSel=0,$countItemECU=0,$txtBaudrate=0,$LINVers=1,$res="",$res1=""
Global $ECUList[10]=["","","","","","","","","",""]
local $outtxt[200]
if winexists($WIN_BUSMASTER) then

	_createConfig("cfxClstrCnfigLIN_02")																; Load Configuration

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

			;ControlCommand($WIN_ClustrConfigLIN, "",$ChannelSelInCCLIN, "SelectString", "Channel 1") 	; Select signal type from the combobox
			sleep(1000)
			Send("{TAB}")

			sleep(1000)
			Send("{TAB}")

			Send("{DOWN}")

			autoitsetoption("WinTextMatchMode",2)
			$Text=WinGetText($WIN_ClustrConfigLIN,"LIN 1.3")
			ConsoleWrite("$Text="&$Text&@CRLF)
			$res=StringInStr($Text,"9600")
			$res1=StringInStr($Text,"LIN 1.3")
			ConsoleWrite("$res="&$res&@CRLF)
			ConsoleWrite("$res1="&$res1&@CRLF)
			if $res>1 Then
				$txtBaudrate = 1
			EndIf
			if $res1>1 Then
				$LINVers = 1
			EndIf





	EndIf


EndIf

ConsoleWrite("$clstrWinLinOpened="&$clstrWinLinOpened&@CRLF)
ConsoleWrite("$LDFFilepath1="&$LDFFilepath1&@CRLF)
;ConsoleWrite("$countItemECU="&$countItemECU&@CRLF)
ConsoleWrite("$txtBaudrate="&$txtBaudrate&@CRLF)
ConsoleWrite("$LINVers="&$LINVers&@CRLF)
;ConsoleWrite("$ECUList="&$ECUList[$i]&@CRLF)

if $clstrWinLinOpened=1 and $LDFFilepath1=1 and $txtBaudrate=1 and $LINVers=1 Then									; Validate the result
	_WriteResult("Pass","TS_LIN_CC_02")
Else
	_WriteResult("Fail","TS_LIN_CC_02")
EndIf

$isAppNotRes=_CloseApp()															; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_LIN_CC_02")
EndIf


ConsoleWrite("****End : TS_LIN_CC_02****"&@CRLF)
ConsoleWrite(@CRLF)