;=== Test Cases/Test Data ===
; Module			:		transmit CAN Messages
; Critical (C)		:		Y
; TestCase No.		:		1
; TestCases			:		Add signal description
; Test Strategy		:		Black Box
; Test Data			:		-
; === Test Procedure ===

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_DBE_03.au3****"&@CRLF)

_launchApp()																; Check if the App is open else launch it

; If the return value of _launchApp function is 0 then perform all the steps of TS_DBE_01 script
if $app=0 Then
	_createConfig("cfxDBE_01")												; Create New Configuration
	_createCANDB("testDBE_01")												; Create New Database File
	sleep(1500)
	_DBmessage("n")															; Select 'New Message' from right click menu
	sleep(1000)
	_addMsgDetails("Msg1",15,8)												; Add the message details
	$sigDetlvPos=ControlGetPos($WIN_BUSMASTER,"",$LVC_SigDet_DBeditor)						; Get handle of signal details list view
	MouseClick("right",$sigDetlvPos[0]+150,$sigDetlvPos[1]+100)				; Right Click on the Signal details list view
	send("{DOWN}")															; Select 'New Signal' menu
	sleep(1500)
	send("{ENTER}")
	_addSigDetails("int","Signal1",32,0,0)									; Add the signal details
EndIf
WinActivate($WIN_BUSMASTER,3)
if winexists($WIN_BUSMASTER) then
	$sigDetlvhwd=controlgethandle($WIN_BUSMASTER,"",$LVC_SigDet_DBeditor)							; Get handle of signal details list view
	_GUICtrlListView_ClickItem($sigDetlvhwd,0)										; Click on the first item in the Signal details list view
	sleep(250)
	ControlClick($WIN_BUSMASTER,"",$BTN_SigDes_DBEditor)											; Click the 'New Desc...' button
	sleep(1000)
	_addSigDesc("Desc1",1)																; Call _addSigDesc function from 'globalFunc4Module.au3' file to add signal description.
	sleep(500)
	$sigDesclvhwd=controlgethandle($WIN_BUSMASTER,"",$LVC_SigDesc_SigDesc)							; Get handle of signal description list view
	$sigDescName=_GUICtrlListView_GetItemText($sigDesclvhwd,0,0)						; Fetch the signal desc name
	$sigDescVal=_GUICtrlListView_GetItemText($sigDesclvhwd,0,1)							; Fetch the signal desc value
	ConsoleWrite("$sigDescName :"&$sigDescName&@CRLF)
	ConsoleWrite("$sigDescVal :"&$sigDescVal&@CRLF)
	_saveCloseCANDB()
EndIf
if $sigDescName="Desc1" and $sigDescVal=1 Then
	_ExcelWriteCell($oExcel, "Pass", 8, 2)
Else
	_ExcelWriteCell($oExcel, "Fail", 8, 2)
EndIf

ConsoleWrite("****End : TS_DBE_03.au3****"&@CRLF)
ConsoleWrite(@CRLF)