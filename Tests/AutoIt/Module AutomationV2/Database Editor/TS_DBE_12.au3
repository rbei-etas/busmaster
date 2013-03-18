;=== Test Cases/Test Data ===
; Module			:		Database Editor
; Critical (C)		:		Y
; TestCase No.		:		1
; TestCases			:		Delete Message
; Test Strategy		:		Black Box
; Test Data			:		-
; === Test Procedure ===

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_DBE_12.au3****"&@CRLF)

_launchApp()
if $app=0 Then
	_openCANDB("testDBE_01")
	$sigDetlvhwd=controlgethandle($WIN_BUSMASTER,"",$LVC_SigDet_DBeditor)								; Get handle of signal details list view
	_GUICtrlListView_ClickItem($sigDetlvhwd,0)											; Click on the first item in the Signal details list view
	sleep(500)
	$DelSignalDescRes=_DelSignalDesc()													; Delete signal desc
	$sigDesclvhwd=controlgethandle($WIN_BUSMASTER,"",$LVC_SigDesc_SigDesc)							; Get handle of signal details list view
	$signalDescCount=_GUICtrlListView_GetItemCount($sigDesclvhwd)						; Fetch the count of signal desc items
	$sigDetlvhwd=controlgethandle($WIN_BUSMASTER,"",$LVC_SigDet_DBeditor)								; Get handle of signal details list view
	_GUICtrlListView_ClickItem($sigDetlvhwd,0)											; Click on the first item in the Signal details list view
	controlclick($WIN_BUSMASTER,"",$BTN_DeleteSig_DBEditor)												; Click Yes button
	sleep(1000)
	if winexists($DLG_Busmaster,$sigDelmsgtxt) Then
		ControlClick($DLG_Busmaster,"",$BTN_Yes_BM)									; Click on 'Yes' button
	EndIf
EndIf
WinActivate($WIN_BUSMASTER,3)
if winexists($WIN_BUSMASTER) then
	$DBTreeVwHWD = ControlGetHandle($WIN_DBEditor_CAN, "", $TVC_DBFile_DBEditor) 					; Fetch tree view control handle
	$DbtreeVwparent = _GUICtrlTreeView_GetFirstItem($DBTreeVwHWD) 					; Fetch Parent node handle
	$childHWD = _GUICtrlTreeView_GetNext($DBTreeVwHWD, 0)
	ConsoleWrite("$DbtreeVwparent :"&$DbtreeVwparent & @CRLF)
	ConsoleWrite("$childHWD :"&$childHWD & @CRLF)
	_GUICtrlTreeView_ClickItem($DBTreeVwHWD, $childHWD, "Right") 					; Right Click on the msg node
	Send("d") 																		; Select Delete message menu
	sleep(1000)
	if winexists($DLG_Busmaster,$msgDeltxt) Then
		controlclick($DLG_Busmaster,"",$BTN_Yes_BM)									; Click on 'Yes' button
	EndIf
	$DBTreeVwHWD = ControlGetHandle($WIN_DBEditor_CAN, "", $TVC_DBFile_DBEditor) 					; Fetch tree view control handle
	$DbtreeVwparent = _GUICtrlTreeView_GetFirstItem($DBTreeVwHWD) 					; Fetch Parent node handle
	$msgCount=_GUICtrlTreeView_GetChildCount($DBTreeVwHWD,0)
	ConsoleWrite("$msgCount :"&$msgCount & @CRLF)
	_saveCloseCANDB()																; Save and Close DB
EndIf
if $msgCount=-1 Then
	_ExcelWriteCell($oExcel, "Pass", 17, 2)
Else
	_ExcelWriteCell($oExcel, "Fail", 17, 2)
EndIf

ConsoleWrite("****End : TS_DBE_12.au3****"&@CRLF)
ConsoleWrite(@CRLF)