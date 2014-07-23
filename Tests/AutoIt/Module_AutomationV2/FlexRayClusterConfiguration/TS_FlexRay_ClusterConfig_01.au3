;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_FLX_DBE_01
; TestCases			:		Importing Fibex File
; Test Data			:
; Test Setup		:		1. Select FlexRay->Cluster Configuration
;                           2. Select browse button under import flexray database
;                           3. Select a fibex files

; Expected Results  : 		1. After step1, "Import Fibex" window will open.
;                           2. After step2, dialog will open to select and associate a database.
;                           And it should allow to select  fibex files of extension .xml only.
;                           3. After step3, verify that the following items are displayed :
;                              a) Fibex file path
;                              b) All the available cluster should be listed in the combo box
;                              c) Available ECUs in the FIBEX file should be displayed for the user to select the interested ECUs.
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_FLX_DBE_01.au3****"&@CRLF)
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
	_GUICtrlListView_SetItemChecked($handle,1)                                                      ;Select second ECU
	_GUICtrlListView_SetItemChecked($handle,2)                                                      ;Select third ECU
	Sleep(1000)
	$temp1=_GUICtrlListView_GetItemChecked($handle,1)                                                ;Check if item is in checked state
	$temp2=_GUICtrlListView_GetItemChecked($handle,2)
	Sleep(1000)
	$text=ControlGetText($WIN_ClusterConfiguration,"",$TXT_EditFibexPath)                            ;Get the path of fibex file
	ConsoleWrite("$text" & $text & @CRLF)
	ControlClick($WIN_ClusterConfiguration,"",$BTN_OK_ClusterConfig)                                 ;Click on OK button of the cluster configuration window
	Sleep(1000)
	WinMenuSelectItem($WIN_BUSMASTER,"",$FlexRayMenu,$ClusterConfigMenu)                              ;Cluster configuration menu item
	Sleep(1000)
	$handle1=ControlGetHandle($WIN_ClusterConfiguration,"",$LSTC_SelectECU)
	$count1=_GUICtrlListView_GetItemCount($handle1)                                                  ;Get item count after opening the cluster configuration window second time
    $temp3=_GUICtrlListView_GetItemChecked($handle1,1)
	$temp4=_GUICtrlListView_GetItemChecked($handle1,2)
	ConsoleWrite("$count1" & $count1 & @CRLF)
	If($count=5 And $count1=5) Then
		$Count_Pass=1
	Else
		$Count_Pass=0
	EndIf

	If($temp1=1 And $temp2=1 And $temp3=1 And $temp4=1) Then
		$Item_Selected=1
	Else
		$Item_Selected=0
	EndIf

	If($text=$TestDataPath &"\" & "TestFlexRay.xml") Then
		$text_pass=1
	Else
		$text_pass=0
	EndIf


	ConsoleWrite("$Count_pass" & $Count_Pass & @CRLF)
	ConsoleWrite("$Item_Selected" & $Item_Selected & @CRLF)

	If($Count_Pass=1 And $Item_Selected=1 And $text_pass=1) Then
		_WriteResult("Pass","TS_FLX_DBE_01")
	Else
		_WriteResult("Fail","TS_FLX_DBE_01")
	EndIf
 EndIf

$isAppNotRes=_CloseApp()																				; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_FLX_DBE_01")
EndIf



ConsoleWrite("****End : TS_FLX_DBE_01.au3****"&@CRLF)
ConsoleWrite(@CRLF)
