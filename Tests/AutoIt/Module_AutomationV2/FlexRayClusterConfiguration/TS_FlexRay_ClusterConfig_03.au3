;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_FLX_DBE_03
; TestCases			:		Fibex File Selection
; Test Data			:
; Test Setup		:		1. Import a fibex file
;                           2. Select a cluster
;                           3. Select some ECU's
;                           4. Click the browse file button and import other fibex file

; Expected Results  : 	After step4, the newly added fibex file path should be displayed and cluster combo box list and ECU data should be updated w.r.t the new file
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_FLX_DBE_03.au3****"&@CRLF)
Local $TestDataPath=_TestDataPath()

_launchApp()

WinActivate($WIN_BUSMASTER)

if winexists($WIN_BUSMASTER) then

	_loadConfig("cfx_FlexRay_Cluster_01.cfx")										                     ; Load Configuration
	_ImportFibex("TestFlexRay.xml")                                                                   ;Import fibex file
	$handle=ControlGetHandle($WIN_ClusterConfiguration,"",$LSTC_SelectECU)                           ;Get handle of Select ECUs list view
 	$count=_GUICtrlListView_GetItemCount($handle)                                                    ;Get the no of ECUs
	Sleep(1000)
	ConsoleWrite("Item count" & $count & @CRLF)
	Sleep(1000)
	ControlClick($WIN_ClusterConfiguration,"",$BTN_OK_ClusterConfig)
	Sleep(1000)
	_ImportFibex("TestFlexRayNew.xml")
	Sleep(1000)
	$handle1=ControlGetHandle($WIN_ClusterConfiguration,"",$LSTC_SelectECU)                           ;Get handle of Select ECUs list view
	$count1=_GUICtrlListView_GetItemCount($handle1)
	ConsoleWrite("$count" & $count & @CRLF)
	ConsoleWrite("$count1" & $count1 & @CRLF)

	If($Count=5 And $count1=6) Then
		_WriteResult("Pass","TS_FLX_DBE_03")
	Else
		_WriteResult("Fail","TS_FLX_DBE_03")
	EndIf
 EndIf

$isAppNotRes=_CloseApp()																				; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_FLX_DBE_03")
EndIf



ConsoleWrite("****End : TS_FLX_DBE_03.au3****"&@CRLF)
ConsoleWrite(@CRLF)

