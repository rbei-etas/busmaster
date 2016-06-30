#cs
;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_NS_LIN_31
; TestCases			:		Busevent function validation
; Test Data			:
; Test Setup		:		----------Test case-30-------------
								1. open the function editor for a node .
								2.Select Bus events and righ click on it.Select "Add" option.
								3.check "All Bus event Handlers(s)" window will be opened.
								4.select All the below listed check boxes.
									a.preconnect
									b.connect
									c.Disconnect
									press "OK".
							Test Case -31

							1.Repeate steps of above test case.
							2.Configure Trace mesasges for all the Bus event functions.[As per test data provided.]
							3.Connect busmaster
							4.Disconnect.


; Expected Results  :		----------Test case-30-------------

							1.After step3,"All Bus event Handlers(s)" window should be displayed with three check boxes for "preconnect","connect" and disconnect.

							2.After step 4,All above check boxes should be in checked state.
							On pressing "OK" window should get closed.

							The below listed functions should be created under BusEvent handler.
								a.void OnBus_Connect()
								b.void OnBus_DisConnect()
								c.void OnBus_Pre_Connect()

							----------Test case-31-------------

							1.After step3,The below Trace messages should be displayed in trace window.
							A.preconnect
							connect

							2.After step4,The below Trace message should be displayed in trace window.
							A.Disconnect
;==========================================================================Test Procedure =========================================================================
#ce
;#include <IncludeFiles.au3>
ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_NS_LIN_31.au3****"&@CRLF)

_launchApp()
ProcessWait("BUSMASTER.exe")
;Global $RemoveDialogBox = 0,$childNode="",$CheckBoxNode1="",$CheckBoxNode2="",$CheckLoadednode=0
Global $BusevntFun[4]=["","","",""],$infoTrace[4]=["","","",""]
Global $BusevntFunCount=0,$BuseventhandlerCheck=0,$traceInfoBusevent=0
Global $evntFunction[3]=[" Trace(""OnBus_Pre_Connect"");"," Trace(""OnBus_Connect"");"," Trace(""OnBus_DisConnect"");"]

WinActivate($WIN_BUSMASTER)

if winexists($WIN_BUSMASTER) then
	sleep(500)

	_createConfig("cfxLINNS_31")															       ; Create new Configuration
	_SetViewToolBarLIN()																	   ; Select LIN Tool Bar to view.
	_linDriverSelection()																   ; Select LIN driver selection from xml configuration File.
	sleep(1000)

	_LINNodeSimConfigMenu()																		; Open Node Sim from menu
	sleep(500)

	Opt("WinDetectHiddenText", 0)
	Opt("WinSearchChildren", 1)
	Opt("WinTitleMatchMode", 1)
	WinWait("Configure Simulated Systems - LIN Bus")

	if winexists("Configure Simulated Systems - LIN Bus") then

		_AddNewNodeNS("TestNode31","LINCPP31",$TVC_Handlers_CPP)							;Create new Node with new CPP file
		$SimTVHWD= ControlGetHandle($WIN_BUSMASTER, "",$TVC_Handlers_CPP)					; Fetch the handle of tree view
		$SimFileHWD=_GUICtrlTreeView_GetItemHandle($SimTVHWD,"")							; Fetch the first item handle
		_GUICtrlTreeView_SetSelected($SimTVHWD, $SimFileHWD)
		Send("{ENTER}")
		WinWaitActive("","",5)
		_AddBusevent(1)
		_AddBusevent(2)
		_AddBusevent(3)

		$TVHWD=_GetWinHndlrNSTV()															; Fetch the handle of tree view
		$BusevntHWD=_GetCANHandlerHWD(2,1)													; Fetch the handle of the Event handler item
		_GUICtrlTreeView_ClickItem($TVHWD, $BusevntHWD, "Right") 							; Right click on Busevent handler item in the tree view
		$BusevntFunCount = _GUICtrlTreeView_GetChildCount($TVHWD, $BusevntHWD)
		ConsoleWrite("$BusevntFunCount="&$BusevntFunCount&@CRLF)							; Count the no. function cereated under busevent handler

		if $BusevntFunCount=3 Then
			for $i=0 to $BusevntFunCount-1
				$BusevntFun[$i]=_SelectNodes("#0|#1|#"&$i)
				ConsoleWrite("$BusevntFun="&$BusevntFun[$i]&@CRLF)
			Next
		EndIf
		if $BusevntFun[0]="void OnBus_Pre_Connect()" and $BusevntFun[1] ="void OnBus_Connect()" and $BusevntFun[2]="void OnBus_DisConnect()" Then
			$BuseventhandlerCheck = 1
		EndIf



		for $j=0 to $BusevntFunCount-1
			ControlTreeView ( $WIN_BUSMASTER, "", $TVHWD, "Select" ,"#0|#1|#"&$j)
			sleep(1000)
			ControlSend($WIN_BUSMASTER, "", $TXT_EventHandler_CPPFile, "" & @CRLF)					; Write in the editor of Node sim
			ControlSend($WIN_BUSMASTER, "", $TXT_EventHandler_CPPFile, $evntFunction[$j] & @CRLF)  ; Write in the editor of Node sim
		Next


		sleep(500)

		_SaveAndCloseFunEditor()																	; save and Close Function editor
		;Send("^s")																				    ; Save the cpp file
		;Send("^{F4}") 																			    ; Close the C file window
		sleep(1000)
		WinActivate($WIN_BUSMASTER)
		_ConnectDisconnect_LIN()																	; Connect LIN
		Sleep(2000)
		_ConnectDisconnect_LIN()																	; Disconnect LIN

		$CountTraceInfo= _GetTraceWinItemCount()													; Count info in trace Window.
		ConsoleWrite("$CountTraceInfo="&$CountTraceInfo&@CRLF)

		if $CountTraceInfo =3 Then
			For $i=0 to $CountTraceInfo-1
				$infoTrace[$i] = _GetTraceWinInfo($i)
				ConsoleWrite("$infoTrace="&$infoTrace[$i]&@CRLF)
			Next
		EndIf

		if $infoTrace[0]="OnBus_Pre_Connect" and $infoTrace[1] = "OnBus_Connect" and $infoTrace[2]="OnBus_DisConnect" Then
			$traceInfoBusevent = 1
		EndIf

		sleep("1000")
	EndIf


EndIf

	ConsoleWrite("$BusevntFunCount="&$BusevntFunCount&@CRLF)
	ConsoleWrite("$BuseventhandlerCheck="&$BuseventhandlerCheck&@CRLF)
	ConsoleWrite("$traceInfoBusevent="&$traceInfoBusevent&@CRLF)


	If $BusevntFunCount = 3  and $BuseventhandlerCheck =1 and $traceInfoBusevent=1 Then
		_WriteResult("Pass","TS_NS_LIN_31")
		ConsoleWrite("PASS")
	Else
	   _WriteResult("Fail","TS_NS_LIN_31")
	   ConsoleWrite("FAIL")
   EndIf

$isAppNotRes=_CloseApp()																	    ; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_NS_LIN_31")
EndIf

ConsoleWrite("****End : TS_NS_LIN_31.au3****"&@CRLF)
ConsoleWrite(@CRLF)

