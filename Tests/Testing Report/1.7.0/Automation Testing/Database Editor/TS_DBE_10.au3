;=== Test Cases/Test Data ===
; Module			:		Database Editor
; Critical (C)		:		Y
; TestCase No.		:		1
; TestCases			:		Database Usage in Busmaster
; Test Strategy		:		Black Box
; Test Data			:		-
; === Test Procedure ===

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_DBE_10.au3****"&@CRLF)


WinActivate($mWin,3)
if winexists($mWin) then
	_openCANDB("testDBE_01")															; Open "testDBE_01" DBF file
	$delDescBTNStatus1=ControlCommand($mWin,"",$delSigDescctrlID,"IsEnabled")			; Fetch the status of 'New Desc' button
	ConsoleWrite("$delDescBTNStatus1 :" & $delDescBTNStatus1 &@CRLF)
	$sigDetlvhwd=controlgethandle($mWin,"",$sigDetlvInst1)								; Get handle of signal details list view
	_GUICtrlListView_ClickItem($sigDetlvhwd,0)											; Click on the first item in the Signal details list view
	$delDescBTNStatus2=ControlCommand($mWin,"",$delSigDescctrlID,"IsEnabled")			; Fetch the status of 'New Desc' button
	ConsoleWrite("$newDescBTNStatus2 :" & $delDescBTNStatus2 &@CRLF)
	sleep(500)
	$DelSignalDescRes=_DelSignalDesc()													; Delete signal desc
	$sigDesclvhwd=controlgethandle($mWin,"",$sigDesclvInst1)							; Get handle of signal details list view
	$signalDescCount=_GUICtrlListView_GetItemCount($sigDesclvhwd)						; Fetch the count of signal desc items
	consolewrite("$signalDescCount : " & $signalDescCount & @Crlf)
EndIf
if $DelSignalDescRes=1 and $signalDescCount=0 Then
	_ExcelWriteCell($oExcel, "Pass", 15, 2)
Else
	_ExcelWriteCell($oExcel, "Fail", 15, 2)
EndIf

ConsoleWrite("****End : TS_DBE_10.au3****"&@CRLF)
ConsoleWrite(@CRLF)