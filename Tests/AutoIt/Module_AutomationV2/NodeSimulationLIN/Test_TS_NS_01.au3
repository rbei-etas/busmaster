;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_NS_01
; TestCases			:		Execute Key handler
; Test Data			:		cfxNSCAN_01.cfx
; Test Setup		:		1.Open a file.
;~ 							2.Add key handler for the key 'a'  as:STCAN_MSG sMsg;

;~ 							// Initialise message structure

;~ 							sMsg.m_unMsgID = 0x100;                     // Message ID
;~ 							sMsg.m_ucEXTENDED = FALSE;           // Standard Message type
;~ 							sMsg.m_ucRTR = FALSE;                      // Not RTR type
;~ 							sMsg.m_ucDLC = 8;                              // Length is 8 Bytes
;~ 							sMsg.m_sWhichBit.m_aulData[0] = 10;   // Lower 4 Bytes
;~ 							sMsg.m_sWhichBit.m_aulData[1] = 20;   // Upper 4 Bytes
;~ 							sMsg.m_ucChannel = 1;                         // First CAN channel
;~ 							// Send the message
;~ 							SendMsg(sMsg);
;~ 							3.Attach the file to any node
;~ 							4.Build and load the DLL

;~ 							5.Enable key handlers
;~ 							6.Set the message window as active window
;~ 							7.Press key 'a'

; Expected Results  : 		When key  'a' is pressed first time a message of message id 100 will send from the application. This message TX should be displayed on the message window.
;==========================================================================Test Procedure =========================================================================
#include <IncludeFiles.au3>
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : Test_TS_NS_01.au3****"&@CRLF)


_launchApp()


WinActivate($WIN_BUSMASTER)


local $KeyHandler_A=0,$KeyHandler_Trace=0
local $outtxt[200]
if winexists($WIN_BUSMASTER) then

	_createConfig("cfxNodeSimCAN_02")															; Load Configuration
	;_FLXRNodeSimConfigMenu()
	For $i = 1 To 3 Step +1

		if $i = 1 Then
			_CANNodeSimConfigMenu()																			; Open CAN node Simulation window

		ElseIf $i = 2 Then
			_LINNodeSimConfigMenu()																			; Open CAN node Simulation window
		ElseIf $i = 3 Then
		_ImportFibex("TestFlexRay.xml")                                                                   ;Import fibex file
		_selectECUFlexRay("ALL")
		ControlClick($WIN_ClusterConfiguration,"",$BTN_OK_ClusterConfig)
		sleep(1000)
		_FLXRNodeSimConfigMenu()

	EndIf

		$TVHWD=_GetCANCppTVHWD()																		; Fetch the handle of tree view

		ControlFocus ( $WIN_BUSMASTER, "", $TVHWD )

		;---Add New node in node sim--------

		_AddNewNodeNS("TestNode","TestNode_cppFile_"&$i,$TVC_Handlers_CPP)									; Add a new Cpp file

		;--Add Key handler -------------
		If $i = 3 Then
			_AddFLXRKeyHandler("a","BUSMASTER - TestNode_cppFile_"&$i&".cpp")
		Else
			_AddKeyHandler("a","BUSMASTER - TestNode_cppFile_"&$i&".cpp")											; Add key handler for key "a"

		EndIf
		sleep(1000)

		$KeyHWD=_GetHandlerHWD($TVC_Handlers_CPP,2,4)
		_GUICtrlTreeView_ClickItem($TVHWD, $KeyHWD)					 									; Right click on Key Handler item in the tree view

		send("{DOWN}")																					; Send the down key to select the key handler function

		send("{ENTER}")

		ControlSend($WIN_BUSMASTER, "", $TXT_KeyHandler_CPPFile, "Trace" & "(" & Chr(34) & "Key Handler" & Chr(34) & ");")

		Send("^s")																						; Save the cpp file

		;Send("^{F4}") 																					; Close the C file window

		;-------Add Message handler-------------------------
		sleep(1000)
		For $k = 1 To 4 Step +1
			if $i = 3 Then
				_AddFLXRMsgHandler(1,1,"","","","")
				_AddFLXRMsgHandler(1,2,"","","",1)
				_AddFLXRMsgHandler(1,3,"","","","")

				_AddFLXRMsgHandler(2,1,"","","","")
				_AddFLXRMsgHandler(2,2,"","","",1)
				_AddFLXRMsgHandler(2,3,"","","","")

			Else
				_AddMsgHandler($k,$k+1,$k+4)
			sleep(2000)
			EndIf
		Next

		;-------Add Time handler-------------------------
		If $i = 3 Then
			_AddFLXRTimeHandler(2000)
		Else
			_AddCANTimeHandler(2000)
		EndIf
		sleep(2000)

		Send("^s")																						; Save the cpp file
		Send("^{F4}")
		sleep(1000)
		Send("^{F4}")
	Next

EndIf
ConsoleWrite("****End : TS_NS_01.au3****"&@CRLF)
ConsoleWrite(@CRLF)