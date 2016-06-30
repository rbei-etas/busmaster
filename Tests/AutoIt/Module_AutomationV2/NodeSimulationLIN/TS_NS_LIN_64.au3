#cs
;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_NS_LIN_64
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
ConsoleWrite("****Start : TS_NS_LIN_64.au3****"&@CRLF)

_launchApp()
ProcessWait("BUSMASTER.exe")
;Global $RemoveDialogBox = 0,$childNode="",$CheckBoxNode1="",$CheckBoxNode2="",$CheckLoadednode=0
Global $infoTrace[10]=["","","","","","","","","",""]
Global $TimeFunCount=0,$TimeDelay=0,$traceInfoMsgHndler=0,$SecndMsg=0,$FirstMsg=0,$Thirdmsg=0,$FourthMsg=0,$FifthMsg=0,$SixthMsg=0
Global $time1[4]=["","","",""],$time2[4]=["","","",""],$time3[4]=["","","",""],$time4[4]=["","","",""]
Global $tm1=0,$tm2=0,$tm3=0,$tm4=0

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

		_AddNewNodeNS("TestNode64","LINCPP64",$TVC_Handlers_CPP)						;Create new Node with new CPP file

		$TVHWD=_GetWinHndlrNSTV()															; Fetch the handle of tree view
		ControlTreeView ( $WIN_LINNodesim, "", $TVHWD, "Select" ,"#0|#1")
		Send("{ENTER}")

		sleep(1000)
		;_AddMsgHandler(3,"","")																; Add msg handler with msg-id-8
		_AddTimeHandler("Timer1","1000")
		_AddTimeHandler("Timer2","2000")
		_AddTimeHandler("Timer3","3000")
		_AddTimeHandler("Timer4","4000")
		sleep(1000)

		$TimeHndlrHWD=_GetCANHandlerHWD(2,3)													; Fetch the handle of the Time handler item
		_GUICtrlTreeView_ClickItem($TVHWD, $TimeHndlrHWD, "Left") 							;  click on Time handler item in the tree view
		$TimeFunCount = _GUICtrlTreeView_GetChildCount($TVHWD, $TimeHndlrHWD)
		ConsoleWrite("$TimeFunCount="&$TimeFunCount)
		if $TimeFunCount=4 Then
			for $i=0 to $TimeFunCount-1
				ControlTreeView ( $WIN_LINNodesim, "", $TVHWD, "Select" ,"#0|#3|#"&$i)
				sleep(1000)
				ControlSend($WIN_BUSMASTER, "", $TXT_TimeHandler_CPPFile, ""   & @CRLF)														; Write in the editor of Node sim
				ControlSend($WIN_BUSMASTER, "", $TXT_TimeHandler_CPPFile, "Trace(""TimeHandler"&$i+1&""");"   & @CRLF)								; Write in the editor of Node sim
				ControlSend($WIN_BUSMASTER, "", $TXT_TimeHandler_CPPFile, "STLIN_MSG sMsgM1;" & @CRLF)										; Write in the editor of Node sim
				ControlSend($WIN_BUSMASTER, "", $TXT_TimeHandler_CPPFile, "sMsgM1.m_ucMsgTyp = 0;    // Slave Response" & @CRLF)				; Write in the editor of Node sim
				ControlSend($WIN_BUSMASTER, "", $TXT_TimeHandler_CPPFile, "sMsgM1.m_ucChksumTyp = 0; // Classic" & @CRLF)						; Write in the editor of Node sim
				ControlSend($WIN_BUSMASTER, "", $TXT_TimeHandler_CPPFile, "sMsgM1.m_ucDataLen = 10; // DLC"& @CRLF)							; Write in the editor of Node sim
				ControlSend($WIN_BUSMASTER, "", $TXT_TimeHandler_CPPFile, "sMsgM1.m_ucMsgID = "&$i+1&";  // Message Identifier" & @CRLF)			; Write in the editor of Node sim
				ControlSend($WIN_BUSMASTER, "", $TXT_TimeHandler_CPPFile, "sMsgM1.m_sWhichBit.m_aulData[0] = 10;  // Lower 4 Bytes" & @CRLF)	; Write in the editor of Node sim
				ControlSend($WIN_BUSMASTER, "", $TXT_TimeHandler_CPPFile, "sMsgM1.m_sWhichBit.m_aulData[1] = 20;  // Upper 4 Bytes" & @CRLF)	; Write in the editor of Node sim
				ControlSend($WIN_BUSMASTER, "", $TXT_TimeHandler_CPPFile, "sMsgM1.m_ucChannel = 1;" & @CRLF)									; Write in the editor of Node sim
				ControlSend($WIN_BUSMASTER, "", $TXT_TimeHandler_CPPFile, "SendMsg(sMsgM1);" & @CRLF)											; Write in the editor of Node sim
				;ControlSend($WIN_LINNodesim, "", $TXT_EventHandler_CPPFile, "Trace(""MsgId-8 Transmitted"");" & @CRLF)							; Write in the editor of Node sim
			Next
		endif


		_SaveAndCloseFunEditor()																	; save and Close Function editor

		WinActivate($WIN_BUSMASTER)
		_ConnectDisconnect_LIN()																	; Connect LIN
		Sleep(4000)



		_ConnectDisconnect_LIN()																	; Disconnect LIN
		sleep(2000)


		$CountTraceInfo= _GetTraceWinItemCount()													; Count info in trace Window.
		ConsoleWrite("$CountTraceInfo="&$CountTraceInfo&@CRLF)

		if $CountTraceInfo =8 Then
			For $i=0 to $CountTraceInfo-1
				$infoTrace[$i] = _GetTraceWinInfo($i)
				ConsoleWrite("$infoTrace="&$infoTrace[$i]&@CRLF)
			Next
		EndIf

		if $infoTrace[0]="TimeHandler1" and $infoTrace[2]="TimeHandler2" and $infoTrace[4]="TimeHandler3" and $infoTrace[7]="TimeHandler4"   Then
			$traceInfoMsgHndler = 1
		EndIf

		$rCount=_GetLINMsgWinItemCount()																				;count no. of items in msgwindow
		$CountCol=_GetLINMsgWinColCount()																					;count no. of columns in msgwindow
		ConsoleWrite("$rCount="&$rCount&@CRLF)
		ConsoleWrite("$Count1="&$CountCol&@CRLF)

		Local $Msg0 = ObjCreate("System.Collections.ArrayList") 														; create object
		Local $Msg1 = ObjCreate("System.Collections.ArrayList") 														; create object



		if $rCount = 8 Then


			$Msg0=_GetMsgWinInfo($LVC_CID_LINMsgWin,0,10)															; Fetch the first row data in the msg window
			$Msg1=_GetMsgWinInfo($LVC_CID_LINMsgWin,2,10)															; Fetch the second row data in the msg window
			$Msg2=_GetMsgWinInfo($LVC_CID_LINMsgWin,4,10)															; Fetch the second row data in the msg window
			$Msg3=_GetMsgWinInfo($LVC_CID_LINMsgWin,7,10)															; Fetch the second row data in the msg window
			;$Msg4=_GetMsgWinInfo($LVC_CID_LINMsgWin,4,10)															; Fetch the second row data in the msg window

			sleep(2000)

			Local $Count1=0,$tm1=0
			Local $Count2=0,$tm2=0
			Local $Count3=0,$tm3=0
			Local $Count4=0,$tm4=0


			for $i=0 to $rCount-1
				$Msg=_GetMsgWinInfo($LVC_CID_LINMsgWin,$i,10)
				for $j=0 to $CountCol-2
					$Value =$Msg.Item($j)
					ConsoleWrite("$Msg[" & $j &"] : "& $Value &@CRLF)
					ConsoleWrite("--------------------------------" &@CRLF)
				Next
				if $Msg.Item(0) <>"" and $Msg.Item(1)="0x1" and $Msg.Item(2)="LIN Message" and $Msg.Item(3)="Tx" and $Msg.Item(4)=1 and $Msg.Item(5)=8 and $Msg.Item(6)=0x001 and $Msg.Item(7)="0A 00 00 00 14 00 00 00" and $Msg.Item(8)="0x20 (""Enhanced"")" Then			; Compare the Direction, Channel and Msg ID of the first row
					$FirstMsg=1
					ConsoleWrite("$FirstMsg = "& $FirstMsg &@CRLF)
					$Count1 = $Count1+1
					if $Count1=1 Then
						$tm1=$Msg.Item(0)
					EndIf
				Endif
				if $Msg.Item(0) <>"" and $Msg.Item(1)="0x2" and $Msg.Item(2)="LIN Message" and $Msg.Item(3)="Tx" and $Msg.Item(4)=1 and $Msg.Item(5)=7 and $Msg.Item(6)=0x002 and $Msg.Item(7)="0A 00 00 00 14 00 00" and $Msg.Item(8)="0x9F (""Enhanced"")" Then			; Compare the Direction, Channel and Msg ID of the first row
					$SecndMsg=1
					ConsoleWrite("$SecndMsg = "& $SecndMsg &@CRLF)
					$Count2 = $Count2+1
					if $Count2=1 Then
						$tm2=$Msg.Item(0)
					EndIf
				EndIf
				if $Msg.Item(0) <>"" and $Msg.Item(1)="0x3" and $Msg.Item(2)="LIN Message" and $Msg.Item(3)="Tx" and $Msg.Item(4)=1 and $Msg.Item(5)=8 and $Msg.Item(6)=0x003 and $Msg.Item(7)="0A 00 00 00 14 00 00 00" and $Msg.Item(8)="0xDE (""Enhanced"")" Then			; Compare the Direction, Channel and Msg ID of the first row
					$Thirdmsg=1
					ConsoleWrite("$Thirdmsg = "& $Thirdmsg &@CRLF)
					$Count3 = $Count3+1
					if $Count3=1 Then
						$tm3=$Msg.Item(0)
					EndIf
				EndIf
				if $Msg.Item(0) <>"" and $Msg.Item(1)="0x4" and $Msg.Item(2)="LIN Message" and $Msg.Item(3)="Tx" and $Msg.Item(4)=1 and $Msg.Item(5)=7 and $Msg.Item(6)=0x004 and $Msg.Item(7)="0A 00 00 00 14 00 00" and $Msg.Item(8)="0x1D (""Enhanced"")" Then			; Compare the Direction, Channel and Msg ID of the first row
					$FourthMsg=1
					ConsoleWrite("$FourthMsg = "& $FourthMsg &@CRLF)
					$Count4 = $Count4+1
					if $Count4=1 Then
						$tm4=$Msg.Item(0)
					EndIf
				EndIf


			;EndIf
			Next
			$time1 = StringSplit($tm1, ":")
			$time2 = StringSplit($tm2, ":")
			$time3 = StringSplit($tm3, ":")
			$time4 = StringSplit($tm4, ":")

			ConsoleWrite("$time1="&$time1[3]&@CRLF)
			ConsoleWrite("$time2="&$time2[3]&@CRLF)
			if ($time2[3]-$time1[3])=1 and ($time3[3]-$time2[3]) and ($time4[3]-$time3[3]) Then
				$TimeDelay =1
			EndIf

		EndIf
	EndIf


EndIf
	ConsoleWrite("$tm1="&$tm1&@CRLF)
	ConsoleWrite("$tm2="&$tm2&@CRLF)
	ConsoleWrite("$tm3="&$tm3&@CRLF)
	ConsoleWrite("$tm4="&$tm4&@CRLF)



	ConsoleWrite("$TimeFunCount="&$TimeFunCount&@CRLF)
	ConsoleWrite("$traceInfoMsgHndler="&$traceInfoMsgHndler&@CRLF)
	ConsoleWrite("$rCount="&$rCount&@CRLF)
	ConsoleWrite("$FirstMsg="&$FirstMsg&@CRLF)
	ConsoleWrite("$SecndMsg="&$SecndMsg&@CRLF)
	ConsoleWrite("$Thirdmsg="&$Thirdmsg&@CRLF)
	ConsoleWrite("$FourthMsg="&$FourthMsg&@CRLF)
	ConsoleWrite("$TimeDelay="&$TimeDelay&@CRLF)
	;ConsoleWrite("$Thirdmsg="&$Thirdmsg&@CRLF)
	;ConsoleWrite("$FourthMsg="&$FourthMsg&@CRLF)
	;ConsoleWrite("$FifthMsg="&$FifthMsg&@CRLF)
	;ConsoleWrite("$SixthMsg="&$SixthMsg&@CRLF)



	If $TimeFunCount = 4   and $traceInfoMsgHndler=1 and $TimeDelay=1 and $rCount=8 and $FirstMsg=1 and $SecndMsg=1 and $Thirdmsg=1 and $FourthMsg=1  Then
		_WriteResult("Pass","TS_NS_LIN_64")
		ConsoleWrite("PASS")
	Else
	   _WriteResult("Fail","TS_NS_LIN_64")
	   ConsoleWrite("FAIL")
   EndIf

$isAppNotRes=_CloseApp()																	    ; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_NS_LIN_64")
EndIf

ConsoleWrite("****End : TS_NS_LIN_64.au3****"&@CRLF)
ConsoleWrite(@CRLF)

