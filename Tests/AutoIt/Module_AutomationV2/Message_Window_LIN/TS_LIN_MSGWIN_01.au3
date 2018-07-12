;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_LINMsgWin_01.au3
; TestCases			:		Addition of message
; Test Data			:		-
; Test Setup		:		 1.Start the Busmaster application.
							;2.Select LIN->Driver Selection->Vector XL.
							;3.Import LDF file and configure Cluster.
							;4.Add frames to the Tx window

; Expected Results  : 		1. Message windowfor LIN should appear in the front as an active window.
							;2. Message window list should contain following columns by default - "Time,Message, Tx/Rx, Channel,Message Type,DLC,ID,,Data Bytes, checksum
;==========================================================================Test Procedure =========================================================================
;#include <IncludeFiles.au3>
ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("***********LIN Message Window Script Execution Started************" & @CRLF)
ConsoleWrite("****Start : TS_LIN_MSGWIN_01.au3****" & @CRLF)


_launchApp() 																	; Check if the App is open else launch it
ConsoleWrite("started bus master application")


ProcessWait("BUSMASTER.exe") 													; Pauses script execution until a given process exists.
sleep(1000)

WinActivate($WIN_BUSMASTER, 3)
if winexists($WIN_BUSMASTER) then

	_createConfig("cfxLMW_01") 													; Create New Configuration
	Sleep(1000)

	if winexists($WIN_ConfirmSaveAs) Then

		ControlClick($WIN_ConfirmSaveAs,"",$BTN_Yes_SaveAs)

	EndIf

	;_linDriverSelection()
	_linDriverSelection()
	ConsoleWrite("Driver selection is done" & @CRLF)

	_openLINClusterConfig()														;open cluster config menu for LIN
	ConsoleWrite("Opened cluster configuration window" & @CRLF)

	_importLDF("TestLIN2_1.ldf")												;import LDF file
	ConsoleWrite("LDF file imported for LIN" & @CRLF)


	_selectECU("ALL")															;Select ECU

	ControlCommand($WIN_ClusterConfiguration, "", $LinEnableMasterMode , "Check");select check box for "Enable master mode"

	ControlClick($WIN_ClusterConfiguration,"",$BTN_OK_ClusterConfig)			;click on OK button


EndIf

Local $MsgWinColCount=0,$ColTime="",$ColMsg="",$ColMsgType="",$ColDir="",$ColChnl="",$ColDLC="",$ColID="",$ColDB="",$ColChecksum=""
if winexists($mWin) then

	;_loadConfig("cfxCANMsgWin_01")													; Load Configuration

	$MsgWinColCount=_GetLINMsgWinColCount()											; Fetch the Column count of the message window

	if $MsgWinColCount=10 Then

		$HWD=_GetLINMsgWinHWD()
		$firstColH=_GUICtrlListView_GetColumn($HWD,1)										; Fetch the 1st column header text
		$ColTime=StringStripWS($firstColH[5],2)											; Remove trailing spaces from 2nd column header text

		$SecColH=_GUICtrlListView_GetColumn($HWD,2)									; Fetch the 2nd column header text
		$ColMsg=StringStripWS($SecColH[5],2)											; Remove trailing spaces from 3rd column header text

		$ThirdColH=_GUICtrlListView_GetColumn($HWD,3)									; Fetch the 3rd column header text
		$ColMsgType=StringStripWS($ThirdColH[5],2)											; Remove trailing spaces from 4th column header text

		$FourthColH=_GUICtrlListView_GetColumn($HWD,4)									; Fetch the 4th column header text
		$ColDir=StringStripWS($FourthColH[5],2)										; Remove trailing spaces from 5th column header text

		$FifthColH=_GUICtrlListView_GetColumn($HWD,5)									; Fetch the 5th column header text
		$ColChnl=StringStripWS($FifthColH[5],2)										; Remove trailing spaces from 6th column header text

		$SixthColH=_GUICtrlListView_GetColumn($HWD,6)									; Fetch the 6th column header text
		$ColDLC=StringStripWS($SixthColH[5],2)										; Remove trailing spaces from 7th column header text

		$SeventhColH=_GUICtrlListView_GetColumn($HWD,7)									; Fetch the 7th column header text
		$ColID=StringStripWS($SeventhColH[5],2)											; Remove trailing spaces from 8th column header text

		$EighthColH=_GUICtrlListView_GetColumn($HWD,8)									; Fetch the 8th column header text
		$ColDB=StringStripWS($EighthColH[5],2)											; Remove trailing spaces from 9th column header text

		$NinthColH=_GUICtrlListView_GetColumn($HWD,9)									; Fetch the 9th column header text
		$ColChecksum=StringStripWS($NinthColH[5],2)											; Remove trailing spaces from 9th column header text

	EndIf

EndIf

consolewrite("$MsgWinColCount: "&$MsgWinColCount&@CRLF)
consolewrite("$ColTime: "&$ColTime&@CRLF)
consolewrite("$ColMsg: "&$ColMsg&@CRLF)
consolewrite("$ColMsgType: "&$ColMsgType&@CRLF)
consolewrite("$ColDir: "&$ColDir&@CRLF)
consolewrite("$ColChnl: "&$ColChnl&@CRLF)
consolewrite("$ColDLC: "&$ColDLC&@CRLF)
consolewrite("$ColID: "&$ColID&@CRLF)
consolewrite("$ColDB: "&$ColDB&@CRLF)
consolewrite("$ColChecksum: "&$ColChecksum&@CRLF)
if $ColTime="Time" and $ColMsg="Message" and $ColMsgType="Message Type" and  $ColDir="Tx/Rx" and $ColChnl="Channel" and $ColDLC="DLC" and $ColID="ID" and  $ColDB = "Data Byte(s)" and $ColChecksum = "Checksum" Then
	_WriteResult("Pass","TS_LIN_MSGWIN_01")
	consolewrite("PASS")
Else
	_WriteResult("Fail","TS_LIN_MSGWIN_01")
	consolewrite("FAIL")
EndIf

$isAppNotRes=_CloseApp()																				; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_LIN_MSGWIN_01")
EndIf



ConsoleWrite("****End : TS_LIN_MSGWIN_01.au3****" & @CRLF)
ConsoleWrite(@CRLF)

