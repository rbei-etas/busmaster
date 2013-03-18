;=== Test Cases/Test Data ===
; Module			:		transmit CAN Messages
; Critical (C)		:		Y
; TestCase No.		:		1
; TestCases			:		Add a signal to message in database
; Test Strategy		:		Black Box
; Test Data			:		-
; === Test Procedure ===

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_DBE_02.au3****"&@CRLF)

_launchApp()																; Check if the App is open else launch it

; If the return value of _launchApp function is 0 then perform all the steps of TS_DBE_01 script
if $app=0 Then
	_createConfig("cfxDBE_01")												; Create New Configuration
	_createCANDB("testDBE_01")												; Create New Database File
	sleep(1500)
	_DBmessage("n")															; Select 'New Message' from right click menu
	sleep(1000)
	_addMsgDetails("Msg1",15,8)												; Add the message details
EndIf
WinActivate($WIN_BUSMASTER,3)
if winexists($WIN_BUSMASTER) then
	$sigDetlvPos=ControlGetPos($WIN_BUSMASTER,"",$LVC_SigDet_DBeditor)						; Get handle of signal details list view
	MouseClick("right",$sigDetlvPos[0]+150,$sigDetlvPos[1]+100)				; Right Click on the Signal details list view
	send("{DOWN}")															; Select 'New Signal' menu
	sleep(1500)
	send("{ENTER}")
	_addSigDetails("int","Signal1",32,0,0)										; Add the signal details
	$sigDetlvhwd=controlgethandle($WIN_BUSMASTER,"",$LVC_SigDet_DBeditor)						; Get handle of signal details list view
	$sigName=_GUICtrlListView_GetItemText($sigDetlvhwd,0,0)						; Fetch the signal name
	$sigByteInd=_GUICtrlListView_GetItemText($sigDetlvhwd,0,1)					; Fetch the Byte Index
	$sigBitNo=_GUICtrlListView_GetItemText($sigDetlvhwd,0,2)					; Fetch the Bit No.
	$sigLen=_GUICtrlListView_GetItemText($sigDetlvhwd,0,3)						; Fetch the signal length
	$sigType=_GUICtrlListView_GetItemText($sigDetlvhwd,0,4)						; Fetch the signal type
	consolewrite("$sigName="&$sigName&@CRLF)
	consolewrite("$sigLen="&$sigLen&@CRLF)
	consolewrite("$sigType="&$sigType&@CRLF)
	consolewrite("$sigByteInd="&$sigByteInd&@CRLF)
	consolewrite("$sigBitNo="&$sigBitNo&@CRLF)
EndIf
if $sigName="Signal1" and $sigLen=32 and $sigType="int" and $sigByteInd=0 and $sigBitNo=0 Then
	_ExcelWriteCell($oExcel, "Pass", 7, 2)
Else
	_ExcelWriteCell($oExcel, "Fail", 7, 2)
EndIf

ConsoleWrite("****End : TS_DBE_02.au3****"&@CRLF)
ConsoleWrite(@CRLF)