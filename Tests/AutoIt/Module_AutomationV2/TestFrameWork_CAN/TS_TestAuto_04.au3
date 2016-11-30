;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_TestAuto_04
; TestCases			:		Execute all test suite
; Test Data			:
; Test Setup		:		1.Select all test setups
;~ 							2. Connect the tool to Bus
;~ 							3. Right click on the test suite node.
;~ 							4. Select the "Execute" Command.
;~ 							5. Observe the right view results.

; Expected Results  : 		1. After Step 4 the Exector will execute all TestSetup files
;~ 							The result view will show the testcases executed and the result of each test cases

; Critical (C)		:		Y
; TestCase No.		:		TS_TestAuto_05
; TestCases			:		Executing selected test suite
; Test Data			:
; Test Setup		:		1.Select one or morel test setups
;~ 							2. Deselect some test suites
;~ 							3. Connect the tool to Bus
;~ 							4  Right click on the test suite node.
;~ 							5. Select the "Execute" Command.

; Expected Results  : 		1. After Step 4 the Exector will execute onlyselected TestSetup files

; Critical (C)		:		Y
; TestCase No.		:		TS_TestAuto_06
; TestCases			:		Report File.
; Test Data			:
; Test Setup		:		1. Right click on the test suite node.
;~ 							2. Select the "Execute" Command.
;~ 							3. Open the Report File of a test setup.

; Expected Results  : 		1. The Report file will contailns The following Details.
;~ 							A.Busmaster Version.
;~ 							B.Test Setup Name
;~ 							C. Header Details
;~ 							D. Result of Each Verification node of each Test Case

;==========================================================================Test Procedure =========================================================================


ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("***********Test Automation CAN Script Execution Started************"&@CRLF)
ConsoleWrite("****Start : TS_TestAuto_04.au3****"&@CRLF)

_launchApp()

ProcessWait("BUSMASTER.exe")																	; Pauses script execution until a given process exists.
sleep(1000)

WinActivate($WIN_BUSMASTER,3)

local $TestExeRes=0, $MsgTransmission=0,$Count=0,$ReportHeader=0,$TestSetupInfo=0,$ResultInfo=0

if winexists($WIN_BUSMASTER) then

	_loadConfig("cfxCANTestAuto_04")															; Load configuration

	_OpenTestAutomationEditor()																	; Open test automation editor window
	sleep(1000)
	_LoadTestSetupFile(_TestDataPath()&"\TestAuto_04")
	$hWnd = _GetTestEditorDetailsHWD()															; Fetch Test Editor list view handle
	$TVHWD= ControlGetHandle($WIN_TestAutomationEditorTitle,"",$TVC_TestAuto_TestEditor)						; Fetch Test Editor tree view handle

	_SelectTestSetUpNode()																		; Select Test Set up node

	$DBPath=_TestDataPath()																		; Fetch the test data path

	$ReportPath= _OutputDataPath()																; Fetch the output data path

	_GUICtrlListView_SetItemText($hWnd,7,$DBPath & "\StdExtDB.dbf",1)							; Set the DB file path
	sleep(1000)

	_GUICtrlListView_SetItemText($hWnd,8,$ReportPath & "\Report_04.TXT",1)						; Set the report file path
	sleep(1000)

	_ClickTestEditConfirmBTN()																	; Click on Confirm button

	_ExpandTestEditorTreeView()

	_SelectSendNode()																			; Select Send node

	$SendItemPos=_GUICtrlListView_GetItemPosition($hWnd, 1)										; Fetch the "[Add Message]" row position

	$CtgColWidth=_GUICtrlListView_GetColumnWidth($hWnd,0)										; Fetch the first coulmn width

	ControlClick($WIN_TestAutomationEditorTitle,"",$LVC_TestAuto_TestEditor,"Left",2,$SendItemPos[0]+$CtgColWidth+3,$SendItemPos[1])	; Double click on the "[Add Message]"
	sleep(1000)

	send("{DOWN 2}")																		; Select the msg
	sleep(1000)

	send("{ENTER}")
	sleep(1000)

	_ClickTestEditConfirmBTN()																	; Click on Confirm button

	_ExpandTestEditorTreeView()																	; Expand the tree view

	_SelectSendMsgNode()																		; Select msg in the send node

	_GUICtrlListView_SetItemText($hWnd,1,1,1)													; Set the report file path
	sleep(1000)

	_ClickTestEditConfirmBTN()																	; Click on Confirm button

	_SelectVerifyNode()																			; Select verify node

	$VerifyItemPos=_GUICtrlListView_GetItemPosition($hWnd, 2)									; Fetch the "[Add Message]" row position

	$CtgColWidth=_GUICtrlListView_GetColumnWidth($hWnd,0)										; Fetch the first coulmn width

	ControlClick($WIN_TestAutomationEditorTitle,"",$LVC_TestAuto_TestEditor,"Left",2,$VerifyItemPos[0]+$CtgColWidth+3,$VerifyItemPos[1])	; Double click on the "[Add Message]"
	sleep(1000)

	send("{DOWN 2}")																			; Select the msg
	sleep(1000)

	send("{ENTER}")
	sleep(1000)

	_ClickTestEditConfirmBTN()																	; Click on Confirm button

	_ExpandTestEditorTreeView()																	; Expand the tree view

	_SelectVerifyMsgNode()																		; Select the verify msg node

	$VerifyMsgItemPos=_GUICtrlListView_GetItemPosition($hWnd, 0)								; Fetch the "Signal Unit Type" row position

	$CtgColWidth=_GUICtrlListView_GetColumnWidth($hWnd,0)										; Fetch the first coulmn width

	ControlClick($WIN_TestAutomationEditorTitle,"",$LVC_TestAuto_TestEditor,"Left",2,$VerifyMsgItemPos[0]+$CtgColWidth+3,$VerifyMsgItemPos[1])	; Double click on the "Signal Unit Type"
	sleep(1000)

	send("{DOWN}")																				; Select the type
	sleep(1000)

	send("{ENTER}")
	sleep(1000)

	_GUICtrlListView_SetItemText($hWnd,1,"X==1",1)												; Set the ENG value
	sleep(1000)

	_ClickTestEditConfirmBTN()																	; Click on Confirm button

	Send("!{F4}")																				; Close Test Editor window

	_OpenTestAutomationExecutor()																; Open Test executor window

	_AddTestExeFile("TestAuto_04.xml")															; Add test setup file

	_ConnectDisconnect()																		; Connect

	_TestExeTreeMenu($TestExe_ExecuteMenu)														; Execute the test

     sleep(2000)


	$TExeHWD= ControlGetHandle($WIN_BUSMASTER,"",$LVC_TestCaseResult_TestExe)
	$TExeCount=_GUICtrlListView_GetItemCount($TExeHWD)											; Fetch the TestExecutor item count
	ConsoleWrite("$TExeCount :"&$TExeCount&@CRLF)

	if $TExeCount=6 Then
		$TestCaseName=_GetTestExecutorInfo(0)
		ConsoleWrite("$TestCaseName:"&$TestCaseName[0]&@CRLF)
		$StartMsg=_GetTestExecutorInfo(1)
		ConsoleWrite("$StartMsg[1]:"&$StartMsg[1]&@CRLF)
		$SendMsg1=_GetTestExecutorInfo(2)
		ConsoleWrite("$SendMsg1[1]:"&$SendMsg1[1]&@CRLF)
		$VerifyStart=_GetTestExecutorInfo(4)
		ConsoleWrite("$VerifyStart[1]:"&$VerifyStart[1]&@CRLF)
		$VerifyMsg1=_GetTestExecutorInfo(5)
		ConsoleWrite("$VerifyMsg1[1]:"&$VerifyMsg1[1]&@CRLF)
		ConsoleWrite("$VerifyMsg1[2]:"&$VerifyMsg1[2]&@CRLF)
		if $TestCaseName[0]="Test_4" and $StartMsg[1]="Sending Messages Started" and $SendMsg1[1]="Sending Message MsgStdLil" and $VerifyStart[1]="Verifying Started" and $VerifyMsg1[1]="Verifying Message MsgStdLil" and $VerifyMsg1[2]="SUCCESS" then
			$TestExeRes=1
		EndIf
	EndIf


	ConsoleWrite("$TExeCount:"&$TExeCount&@CRLF)
	ConsoleWrite("$TestExeRes:"&$TestExeRes&@CRLF)


    Opt("WinDetectHiddenText", 0)                                                                  ; Close Test executor window
	Opt("WinSearchChildren", 1)
	Opt("WinTitleMatchMode", 1)
	WinClose("Test Suite Executor")
	sleep(1000)
	_ConnectDisconnect()																		; Disconnect

	_OpenTestAutomationEditor()																	; Open test automation editor window
	_LoadTestSetupFile(_TestDataPath()&"\TestAuto_04")

	ControlFocus ($WIN_TestAutomationEditorTitle,"",$TVC_TestAuto_TestEditor)									; Focus on the tree view

	_SelectTestSetUpNode()																		; Select the test setup node

	_ExpandTestEditorTreeView()																	; Expand the tree view

	send("{DOWN 3}")																			; Select msg in Send node
	sleep(1000)

	$HWD=_GUICtrlTreeView_GetSelection($TVHWD)													; Fetch the handle of the selected node

	_GUICtrlTreeView_ClickItem($TVHWD,$HWD,"Right")												; right click on the selected node
	sleep(1000)

	send("d")																					; Select "Delete" from context menu
	sleep(1000)

	send("{DOWN 3}")																		; Select Msg in verify node

	$HWD=_GUICtrlTreeView_GetSelection($TVHWD)													; Fetch the handle of the selected node

	_GUICtrlTreeView_ClickItem($TVHWD,$HWD,"Right")												; right click on the selected node
	sleep(1000)

	send("d")																					; Select "Delete" from context menu
	sleep(1000)

	_ClickTestEditConfirmBTN()																	; Click on Confirm button

	Send("!{F4}")																				; Close Test executor window
	sleep(1000)

	;$Msg1=_GetMsgWinCANInfo(0)

	;if $Msg1[1]="Tx" and $Msg1[3]="s" And $Msg1[4]="0x101" And $Msg1[5]="MsgStdLil" And $Msg1[7]= "0100000000000000" Then
	;	$MsgTransmission=1
	;EndIf
	;---Added below scripts to check Tx message in message window ------
	for $i=0 To 1
		$Msg1=_GetMsgWinCANInfo($i)
		if $Msg1[1]="Tx" and $Msg1[3]="s" And $Msg1[4]="0x101" And $Msg1[5]="MsgStdLil" And $Msg1[7]= "0100000000000000" Then
			$MsgTransmission=1
			ConsoleWrite("$i="&$i&@CRLF)
		EndIf
	Next

	ConsoleWrite("$MsgTransmission:"&$MsgTransmission&@CRLF)

	$GetReportFile_Path=_OutputDataPath()

	$Count=_FileCountLines($GetReportFile_Path & "\Report_04.TXT")								; Fetch the no. of lines in the report file
	ConsoleWrite("$Count :"&$Count&@CRLF)

	if $Count=26 Then
		$ToolVersion=StringStripWS (FileReadLine ($GetReportFile_Path & "\Report_04.TXT",2),1)	; Fetch the tool version in the report file
		$ReportFile=StringStripWS (FileReadLine ($GetReportFile_Path & "\Report_04.TXT",3),1)	; Fetch the Report file name in the report file
		$TestFileinfo=StringStripWS (FileReadLine ($GetReportFile_Path & "\Report_04.TXT",5),1) ; Fetch the test file info in the report file
		$TestTitle=StringStripWS (FileReadLine ($GetReportFile_Path & "\Report_04.TXT",8),1)	; Fetch the title in the report file
		$TestModName=StringStripWS (FileReadLine ($GetReportFile_Path & "\Report_04.TXT",9),1)	; Fetch the test module name in the report file
		$TestVersion=StringStripWS (FileReadLine ($GetReportFile_Path & "\Report_04.TXT",10),1)	; Fetch the test editor/exe version in the report file
		$TestDBInfo=StringStripWS (FileReadLine ($GetReportFile_Path & "\Report_04.TXT",11),1)	; Fetch the DB path in the report file
		$Result=StringStripWS (FileReadLine ($GetReportFile_Path & "\Report_04.TXT",17),1)		; Fetch the Result info in the report file
		$TestCase=StringStripWS (FileReadLine ($GetReportFile_Path & "\Report_04.TXT",20),1)
		$ResultStatus=StringStripWS (FileReadLine ($GetReportFile_Path & "\Report_04.TXT",21),1)
		$VerifyStatus=StringStripWS (FileReadLine ($GetReportFile_Path & "\Report_04.TXT",24),1)
		$MsgName=StringStripWS (FileReadLine ($GetReportFile_Path & "\Report_04.TXT",25),1)
		$Signal=StringStripWS (FileReadLine ($GetReportFile_Path & "\Report_04.TXT",26),8)

		consolewrite("$ToolVersion :"&$ToolVersion&@CRLF)
		consolewrite("$ReportFile :"&$ReportFile&@CRLF)
		consolewrite("$TestFileinfo :"&$TestFileinfo&@CRLF)
		consolewrite("$TestTitle :"&$TestTitle&@CRLF)
		consolewrite("$TestModName :"&$TestModName&@CRLF)
		consolewrite("$TestVersion :"&$TestVersion&@CRLF)
		consolewrite("$TestDBInfo :"&$TestDBInfo&@CRLF)
		consolewrite("$Result :"&$Result&@CRLF)
		consolewrite("$TestCase :"&$TestCase&@CRLF)
		consolewrite("$ResultStatus :"&$ResultStatus&@CRLF)
		consolewrite("$VerifyStatus :"&$VerifyStatus&@CRLF)
		consolewrite("$MsgName :"&$MsgName&@CRLF)
		consolewrite("$Signal :"&$Signal&@CRLF)

		Local $ToolVer3[2]
		ConsoleWrite("Tool version-------------"&_GetToolVersion()&@CRLF)
		$ToolVer1=StringReplace(_GetToolVersion()," Ver","")																; Fetch the Busmaster Version
		ConsoleWrite("$ToolVer :" &$ToolVer1&@CRLF)
		$ToolVer2=StringReplace($ToolVer1,"Nightly",",")																		; Fetch the Busmaster Version
		ConsoleWrite("$ToolVer2 :" &$ToolVer2&@CRLF)
		Local $ToolVer3 = StringSplit($ToolVer2, ",")
		$ToolVer4 =$ToolVer3[1]



		$ToolVer=StringLeft($ToolVer4,15)
		ConsoleWrite("$ToolVer :" &$ToolVer&@CRLF)
		if $ToolVersion="BUSMASTER Version:: "& $ToolVer and $ReportFile="Report File For:: Test" Then
			$ReportHeader=1
		EndIf

		if $TestFileinfo="TestSetup File Information:" and $TestTitle="Title: Test" and $TestModName="Module Name:Test Automation Editor" and $TestVersion="Version: 1.0" and $TestDBInfo="Database Information:"&$DBPath & "\StdExtDB.dbf" Then
			$TestSetupInfo=1
		EndIf

		if $Result="Results:" and $TestCase="TestCase:  Test_4" and $ResultStatus="Result:	SUCCESS" and $VerifyStatus="Verify - 1 (Severity - SUCCESS)" and $MsgName="MsgStdLil:" and $Signal="Sigstd1X==1(x=1)SUCCESS" Then
			$ResultInfo=1
		EndIf

	EndIf
	consolewrite("$Count :"&$Count&@CRLF)
	consolewrite("$ReportHeader :"&$ReportHeader&@CRLF)
	consolewrite("$TestSetupInfo :"&$TestSetupInfo&@CRLF)
	consolewrite("$ResultInfo :"&$ResultInfo&@CRLF)

EndIf

;if $TestExeRes=1 and $MsgTransmission=1 and $ReportHeader=1 and $TestSetupInfo=1 and $ResultInfo=1 Then				; Validate the result
if $TestExeRes=1 and $MsgTransmission=1 and $TestSetupInfo=1 and $ResultInfo=1 Then				; Validate the result
	_WriteResult("Pass","TS_TestAuto_04")
Else
	_WriteResult("Fail","TS_TestAuto_04")
EndIf

$isAppNotRes=_CloseApp()															; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_Log_05")
EndIf
ConsoleWrite("****End : TS_TestAuto_04.au3****"&@CRLF)
ConsoleWrite(@CRLF)