#cs
;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_NS_LIN_63
; TestCases			:		Execute Time handler
; Test Data			:		1.Time name = TimerName
							2.Timer Value=2000 msec
; Test Setup		:		1. open the function editor for a node and right click on Timer handler.Select "Add" option.
							2.check a window will be opened with title "Define New Time handler".
							3.Give Timer name and timer value as per test data provided.Press "OK" button.
							4. Write below code in the created timer function.
							e.g-

							Trace("TimerHandler");
							STLIN_MSG sMsgM1;
							sMsgM1.m_ucMsgTyp = 0;    // Slave Response
							sMsgM1.m_ucChksumTyp = 0; // Classic
							sMsgM1.m_ucDataLen = 10; // DLC
							sMsgM1.m_ucMsgID = 0x12;  // Message Identifier
							sMsgM1.m_sWhichBit.m_aulData[0] = 10;  // Lower 4 Bytes
							sMsgM1.m_sWhichBit.m_aulData[1] = 20;  // Upper 4 Bytes
							sMsgM1.m_ucChannel = 1;
							SendMsg(sMsgM1);

							4. Save and buid it.
							5.Then Connect.


; Expected Results  :		1.After step 2, The window should be opened with title "Define New Time handler".
							2.After step 3,The time function will be created with function name "void OnTimer_TimerName_2000".
							3.Transmission messages will be shown in transmission window in every 2000 ms..
							4.Trace messges will be displayed in out put window inevery 2000ms..
;==========================================================================Test Procedure =========================================================================
#ce
;#include <IncludeFiles.au3>
ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_NS_LIN_63.au3****"&@CRLF)

_launchApp()
ProcessWait("BUSMASTER.exe")
;Global $RemoveDialogBox = 0,$childNode="",$CheckBoxNode1="",$CheckBoxNode2="",$CheckLoadednode=0
Global $infoTrace[4]=["","","",""]
Global $TimeFunCount=0,$TimeDelay=0,$traceInfoMsgHndler=0,$SecndMsg=0,$FirstMsg=0,$Thirdmsg=0,$FourthMsg=0,$FifthMsg=0,$SixthMsg=0


WinActivate($WIN_BUSMASTER)

if winexists($WIN_BUSMASTER) then
	sleep(500)

	_loadConfig("cfxLINNS_63")															    ; Load the  Configuration
	_SetViewToolBarLIN()																	; Select LIN Tool Bar to view.
	_linDriverSelection()																; Select LIN driver selection from xml configuration File.
	sleep(1000)

	_LINNodeSimConfigMenu()																	; Open Node Sim from menu
	sleep(500)

	Opt("WinDetectHiddenText", 0)
	Opt("WinSearchChildren", 1)
	Opt("WinTitleMatchMode", 1)
	WinWait("Configure Simulated Systems - LIN Bus")

	if winexists("Configure Simulated Systems - LIN Bus") then

		_AddNewNodeNS("TestNode63","LINCPP63",$TVC_Handlers_CPP)						;Create new Node with new CPP file

		$TVHWD=_GetWinHndlrNSTV()															; Fetch the handle of tree view
		ControlTreeView ( $WIN_LINNodesim, "", $TVHWD, "Select" ,"#0|#1")
		Send("{ENTER}")

		sleep(1000)
		;_AddMsgHandler(3,"","")																; Add msg handler with msg-id-8
		_AddTimeHandler("Timer1","2000")
		sleep(1000)

		$TimeHndlrHWD=_GetCANHandlerHWD(2,3)													; Fetch the handle of the Time handler item
		_GUICtrlTreeView_ClickItem($TVHWD, $TimeHndlrHWD, "Left") 							;  click on Time handler item in the tree view
		$TimeFunCount = _GUICtrlTreeView_GetChildCount($TVHWD, $TimeHndlrHWD)

		if $TimeFunCount=1 Then
			ControlTreeView ( $WIN_LINNodesim, "", $TVHWD, "Select" ,"#0|#3|#"&0)
			sleep(1000)
			ControlSend($WIN_BUSMASTER, "", $TXT_EventHandler_CPPFile, ""   & @CRLF)														; Write in the editor of Node sim
			ControlSend($WIN_BUSMASTER, "", $TXT_EventHandler_CPPFile, "Trace(""TimeHandler"");"   & @CRLF)								; Write in the editor of Node sim
			ControlSend($WIN_BUSMASTER, "", $TXT_EventHandler_CPPFile, "STLIN_MSG sMsgM1;" & @CRLF)										; Write in the editor of Node sim
			ControlSend($WIN_BUSMASTER, "", $TXT_EventHandler_CPPFile, "sMsgM1.m_ucMsgTyp = 0;    // Slave Response" & @CRLF)				; Write in the editor of Node sim
			ControlSend($WIN_BUSMASTER, "", $TXT_EventHandler_CPPFile, "sMsgM1.m_ucChksumTyp = 0; // Classic" & @CRLF)						; Write in the editor of Node sim
			ControlSend($WIN_BUSMASTER, "", $TXT_EventHandler_CPPFile, "sMsgM1.m_ucDataLen = 10; // DLC"& @CRLF)							; Write in the editor of Node sim
			ControlSend($WIN_BUSMASTER, "", $TXT_EventHandler_CPPFile, "sMsgM1.m_ucMsgID = 0x12;  // Message Identifier" & @CRLF)			; Write in the editor of Node sim
			ControlSend($WIN_BUSMASTER, "", $TXT_EventHandler_CPPFile, "sMsgM1.m_sWhichBit.m_aulData[0] = 10;  // Lower 4 Bytes" & @CRLF)	; Write in the editor of Node sim
			ControlSend($WIN_BUSMASTER, "", $TXT_EventHandler_CPPFile, "sMsgM1.m_sWhichBit.m_aulData[1] = 20;  // Upper 4 Bytes" & @CRLF)	; Write in the editor of Node sim
			ControlSend($WIN_BUSMASTER, "", $TXT_EventHandler_CPPFile, "sMsgM1.m_ucChannel = 1;" & @CRLF)									; Write in the editor of Node sim
			ControlSend($WIN_BUSMASTER, "", $TXT_EventHandler_CPPFile, "SendMsg(sMsgM1);" & @CRLF)											; Write in the editor of Node sim
			;ControlSend($WIN_LINNodesim, "", $TXT_EventHandler_CPPFile, "Trace(""MsgId-8 Transmitted"");" & @CRLF)							; Write in the editor of Node sim
		endif


		_SaveAndCloseFunEditor()																	; save and Close Function editor

		WinActivate($WIN_BUSMASTER)
		_ConnectDisconnect_LIN()																	; Connect LIN
		Sleep(4000)



		_ConnectDisconnect_LIN()																	; Disconnect LIN
		sleep(2000)


		$CountTraceInfo= _GetTraceWinItemCount()													; Count info in trace Window.
		ConsoleWrite("$CountTraceInfo="&$CountTraceInfo&@CRLF)

		if $CountTraceInfo =2 Then
			For $i=0 to $CountTraceInfo-1
				$infoTrace[$i] = _GetTraceWinInfo($i)
				ConsoleWrite("$infoTrace="&$infoTrace[$i]&@CRLF)
			Next
		EndIf

		if $infoTrace[0]="TimeHandler" and $infoTrace[1]="TimeHandler"  Then
			$traceInfoMsgHndler = 1
		EndIf

		$rCount=_GetLINMsgWinItemCount()																				;count no. of items in msgwindow
		$Count1=_GetLINMsgWinColCount()																					;count no. of columns in msgwindow
		ConsoleWrite("$rCount="&$rCount&@CRLF)
		ConsoleWrite("$Count1="&$Count1&@CRLF)

		Local $Msg0 = ObjCreate("System.Collections.ArrayList") 														; create object
		Local $Msg1 = ObjCreate("System.Collections.ArrayList") 														; create object



		if $rCount = 2 Then


			$Msg0=_GetMsgWinInfo($LVC_CID_LINMsgWin,0,10)															; Fetch the first row data in the msg window
			$Msg1=_GetMsgWinInfo($LVC_CID_LINMsgWin,1,10)															; Fetch the second row data in the msg window
			;$Msg2=_GetMsgWinInfo($LVC_CID_LINMsgWin,2,10)															; Fetch the second row data in the msg window
			;$Msg3=_GetMsgWinInfo($LVC_CID_LINMsgWin,3,10)															; Fetch the second row data in the msg window
			;$Msg4=_GetMsgWinInfo($LVC_CID_LINMsgWin,4,10)															; Fetch the second row data in the msg window

			sleep(2000)

			for $i=0 to $rCount-1
				$Msg=_GetMsgWinInfo($LVC_CID_LINMsgWin,$i,10)
				for $j=0 to $Count1-2
					$Value =$Msg.Item($j)
					ConsoleWrite("$Msg[" & $i &"] : "& $Value &@CRLF)
				Next
			Next

			if $Msg0.Item(0) <>" " and $Msg0.Item(1)="0x12" and $Msg0.Item(2)="LIN Message" and $Msg0.Item(3)="Tx" and $Msg0.Item(4)=1 and $Msg0.Item(5)=8 and $Msg0.Item(6)=0x012 and $Msg0.Item(7)="0A 00 00 00 14 00 00 00" and $Msg0.Item(8)="0x4F (""Enhanced"")" Then			; Compare the Direction, Channel and Msg ID of the first row
				$FirstMsg=1
				ConsoleWrite("$FirstMsg = "& $FirstMsg &@CRLF)
			Endif

			if $Msg1.Item(0) <>"" and $Msg1.Item(1)="0x12" and $Msg1.Item(2)="LIN Message" and $Msg1.Item(3)="Tx" and $Msg1.Item(4)=1 and $Msg1.Item(5)=8 and $Msg1.Item(6)=0x012 and $Msg1.Item(7)="0A 00 00 00 14 00 00 00" and $Msg1.Item(8)="0x4F (""Enhanced"")" Then			; Compare the Direction, Channel and Msg ID of the first row
				$SecndMsg=1
				ConsoleWrite("$SecndMsg = "& $Thirdmsg &@CRLF)
			EndIf

			ConsoleWrite("$Msg0.Item(0) = "&$Msg0.Item(0)&@CRLF)
			ConsoleWrite("$Msg1.Item(0) = "&$Msg1.Item(0)&@CRLF)


			;Local $result = StringTrimRight($Msg1.Item(0), 4)
			Local $time1 = StringSplit($Msg0.Item(0), ":")
			Local $time2 = StringSplit($Msg1.Item(0), ":")

			ConsoleWrite("$time1="&$time1[3]&@CRLF)
			ConsoleWrite("$time2="&$time2[3]&@CRLF)

			$TimeDelayMsg1msg2 =$time2[3] - $time1[3]

			ConsoleWrite("$TimeDelayMsg1msg2 = "&$TimeDelayMsg1msg2&@CRLF)
			if $TimeDelayMsg1msg2=2 Then
				$TimeDelay = 1
			EndIf
;~ 			if $Msg2.Item(0) <>"" and $Msg2.Item(1)="0x8" and $Msg2.Item(2)="LIN Message" and $Msg2.Item(3)="Tx" and $Msg2.Item(4)=1 and $Msg2.Item(5)=8 and $Msg2.Item(6)=0x008 and $Msg2.Item(7)="0A 00 00 00 14 00 00 00" and $Msg2.Item(8)="0xD9 (""Enhanced"")" Then			; Compare the Direction, Channel and Msg ID of the first row
;~ 				$Thirdmsg=1
;~ 				ConsoleWrite("$Thirdmsg = "& $FifthMsg &@CRLF)
;~ 			EndIf
;~ 			if $Msg3.Item(0) <>"" and $Msg3.Item(1)="0x10" and $Msg3.Item(2)="LIN Message" and $Msg3.Item(3)="Tx" and $Msg3.Item(4)=1 and $Msg3.Item(5)=8 and $Msg3.Item(6)=0x010 and $Msg3.Item(7)="0A 00 00 00 14 00 00 00" and $Msg3.Item(8)="0x91 (""Enhanced"")" Then			; Compare the Direction, Channel and Msg ID of the first row
;~ 				$FourthMsg=1
;~ 				ConsoleWrite("$FourthMsg = "& $SixthMsg &@CRLF)
;~ 			EndIf
;~ 			if $Msg4.Item(0) <>"" and $Msg4.Item(1)="0x12" and $Msg4.Item(2)="LIN Message" and $Msg4.Item(3)="Tx" and $Msg4.Item(4)=1 and $Msg4.Item(5)=8 and $Msg4.Item(6)=0x012 and $Msg4.Item(7)="0A 00 00 00 14 00 00 00" and $Msg4.Item(8)="0x4F (""Enhanced"")" Then			; Compare the Direction, Channel and Msg ID of the first row
;~ 				$FifthMsg=1
;~ 				ConsoleWrite("$FifthMsg = "& $FourthMsg &@CRLF)
;~ 			EndIf

		EndIf
	EndIf


EndIf

	ConsoleWrite("$TimeFunCount="&$TimeFunCount&@CRLF)
	ConsoleWrite("$traceInfoMsgHndler="&$traceInfoMsgHndler&@CRLF)
	ConsoleWrite("$rCount="&$rCount&@CRLF)
	ConsoleWrite("$FirstMsg="&$FirstMsg&@CRLF)
	ConsoleWrite("$SecndMsg="&$SecndMsg&@CRLF)
	ConsoleWrite("$TimeDelay="&$TimeDelay&@CRLF)
	;ConsoleWrite("$Thirdmsg="&$Thirdmsg&@CRLF)
	;ConsoleWrite("$FourthMsg="&$FourthMsg&@CRLF)
	;ConsoleWrite("$FifthMsg="&$FifthMsg&@CRLF)
	;ConsoleWrite("$SixthMsg="&$SixthMsg&@CRLF)



	If $TimeFunCount = 1   and $traceInfoMsgHndler=1 and $TimeDelay=1 and $rCount=2 and $FirstMsg=1 and $SecndMsg=1   Then
		_WriteResult("Pass","TS_NS_LIN_63")
		ConsoleWrite("PASS")
	Else
	   _WriteResult("Fail","TS_NS_LIN_63")
	   ConsoleWrite("FAIL")
   EndIf

$isAppNotRes=_CloseApp()																	    ; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_NS_LIN_63")
EndIf

ConsoleWrite("****End : TS_NS_LIN_63.au3****"&@CRLF)
ConsoleWrite(@CRLF)

