#cs
;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_NS_LIN_03
; TestCases			:		Adding a Node in Node SImulation
; Test Data			:
; Test Setup		:		1.Select Lin->Node simulation -> Configure
							2.Active window


; Expected Results  : 		1.The root node name should be “LinBus”.
							2.The title of the window should be Configure Simulated systems – LinBus.
							3.A toolbar should be available inside the configuration window with the following toolbar buttons and tooltips for each of the toolbar button:
								a.Add Node
								b.Delete Node
								c.Edit Node
								d.Configure Node
								e.Build Node
;==========================================================================Test Procedure =========================================================================
#ce
;#include <IncludeFiles.au3>
ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_NS_LIN_03.au3****"&@CRLF)

_launchApp()

Global $WinMsgIntrptVisible =0,$WinMsgIntrptVisible1 =0


WinActivate($WIN_BUSMASTER)

if winexists($WIN_BUSMASTER) then

	_createConfig("cfxLINNS_01")															 ; Load Configuration
	_SetViewToolBarLIN()																	 ; Select LIN Tool Bar to view.

	_linDriverSelection()																 ; Select LIN driver selection from xml configuration File.
	sleep(1000)

	_LINNodeSimConfigMenu()

	if winexists("Configure Simulated Systems - LIN Bus") then
		ConsoleWrite("Nodesim Lin window exists")
		_AddNewNodeNS("TestNode","TestNode_cppFile_"&$i,$TVC_Handlers_CPP)

	EndIf

	#cs
	_ConnectDisconnect_LIN()																 ; Click connect/Disconnect in tool bar-LIN
	sleep(1000)

	_EnableInterpretMode_LIN()
     Sleep(1000)

	 WinActivate($WIN_LINMsgWind)															;Disconnect
	 Sleep(500)

	_ConnectDisconnect_LIN()

	$HWD=_GetLINMsgWinHWD()                                                                 ;Get handle for LIN message window
	Sleep(1000)

	ControlFocus($WIN_BUSMASTER,"",$LSTC_LINMsgWin)
	$count1= _GetLINMsgWinItemCount()                                                       ;Get message count before expansion
	Sleep(1000)

	$HWD=_GetLINMsgWinHWD()
	ConsoleWrite("$HWD =" &$HWD)
	_GUICtrlListView_ClickItem($HWD,0,"LEFT","",2)



	Sleep(1000)
	if winexists("Message Interpretation") Then
		ConsoleWrite("'Message Interpretation' dialog is visible...")						;check Message Interpretation' dialog is visible
		$WinMsgIntrptVisible =1
		;$HWD=ControlGetHandle($WIN_BUSMASTER,"","[CLASS:Edit; INSTANCE:1]")
		$name=ControlGetText ( "Message Interpretation", "", "[CLASS:Edit; INSTANCE:1]" )	; Fetch the ECU name from Message Interpretation window
		;$name=_GUICtrlEdit_GetText($hWnd)
		ConsoleWrite("$name="&$name&@CRLF)

	EndIf

	ControlFocus($WIN_BUSMASTER,"",$LSTC_LINMsgWin)
	$HWD1=_GetLINMsgWinHWD()
	_GUICtrlListView_ClickItem($HWD1,1,"LEFT","True",1)										;Click on the another message when Interpretation is opened.
	sleep(2000)


	if winexists("Message Interpretation") Then
		ConsoleWrite("'Message Interpretation' dialog is visible1...")						;check Message Interpretation' dialog is visible
		$WinMsgIntrptVisible1 =1
		$name1=ControlGetText ( "Message Interpretation", "", "[CLASS:Edit; INSTANCE:1]" )	; Fetch the ECU name from Message Interpretation window
		;$name=_GUICtrlEdit_GetText($hWnd)
		ConsoleWrite("$name1="&$name1&@CRLF)
	EndIf
	;_GUICtrlListView_ClickItem($HWD1,1)                                                     ;Expand second message
	Sleep(2000)
	;Send("{ENTER}")
	sleep(2000)


	winclose($WIN_MsgInterpret)

	ConsoleWrite("Close the 'Message Interpretation' dialog.")


	;_GUICtrlListView_ClickItem($HWD,2)                                                     ;Expand message in fourth row
	_GUICtrlListView_ClickItem($HWD,2,"RIGHT","",1)
	;Send("{ENTER}")
	Send("{DOWN}")
	Send("{DOWN}")
	Send("{ENTER}")
	sleep(1000)
	$count2=_GetLINMsgWinItemCount()                                                       ;Count after expanding messages
	Sleep(1000)
	_DisableOverwriteMode()                                                                ;Switch to append mode
	ConsoleWrite("Switch to Append mode")

	Sleep(1000)
	_EnableOverwriteMode()                                                                 ;Enable overwrite mode
	ConsoleWrite("Back to overwrite  mode")

	$count3=_GetLINMsgWinItemCount()                                                       ;Count after switching overwrite mode from append mode
	Sleep(500)
	ConsoleWrite("Count1:" & $count1 & @CRLF)

	ConsoleWrite("Count before switching to append mode ..." &@CRLF)
	ConsoleWrite("Count2:" & $count2 & @CRLF)

	ConsoleWrite("Count after switching to overwrite mode ..." &@CRLF)
	ConsoleWrite("Count3:" & $count3 & @CRLF)

	If $name="ALT_LINALT_R_01" and $name1="ALT_LINALT_R_02" and ($count2==$count3) and $count2 = 5 and $count3 = 5 Then
		_WriteResult("Pass","TS_NS_LIN_03")
		ConsoleWrite("PASS")
	Else
	   _WriteResult("Fail","TS_NS_LIN_03")
	   ConsoleWrite("FAIL")
	EndIf
	#ce
EndIf
$isAppNotRes=_CloseApp()																	; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_NS_LIN_03")
EndIf

ConsoleWrite("****End : TS_NS_LIN_03.au3****"&@CRLF)
ConsoleWrite(@CRLF)

