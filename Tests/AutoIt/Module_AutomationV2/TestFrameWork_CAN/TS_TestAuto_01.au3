;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_TestAuto_01
; TestCases			:		Addition of Test Case Node
; Test Data			:
; Test Setup		:		1. Right click on the test setup node.
;~ 							2. Click On "New Test Case Menu Item"

; Expected Results  : 		1.After step 1 a pop up menu is displayed
;~ 							2. After step 2 A New Test case Named "Untitled TestCase" will be added to the Tree View

; Critical (C)		:		Y
; TestCase No.		:		TS_TestAuto_08
; TestCases			:		Invoking Test Setup Editor
; Test Data			:
; Test Setup		:		1.Open the Busmaster Application.
;~ 							2. Find the Menu "Function->Test Automation->Test Setup Editor"
;~ 							3. Click on the menu.
;~ 							4. Check the GUI parts
;~ 							5. Check the menu Bar of the Busmaster Application.
;~ 							6. Close the Test setup editor or Select any other window in Busmaster(Tx Window, etc).
;~ 							7.Check the menu Bar of the Busmaster Application.
;~ 							8. Close the Test setup editor or Select any other window in Busmaster(Tx Window, etc).

; Expected Results  : 		After the step 3  The Empty test setup editor will appear

;~ 							After the step 4 Editor will contain the following three parts.
;~ 							1. The left side will be a tree control at the first time it will be empty.
;~ 							2. The upper right side part will contain a List control and two buttons named confirm and cancel.
;~ 							3. The bottom side contain a Help Text window
;~ 							4.After the step 4 the menu of mainframe will be changed to Test setp editor menu.
;~ 							5 After the step menu Bar will be changed to Busmaster Menu Item

; Critical (C)		:		Y
; TestCase No.		:		TS_TestAuto_09
; TestCases			:		Creating New Test Setup File
; Test Data			:
; Test Setup		:		1.Open the Busmaster Application.
;~ 							2. Find the Menu "Function->Test Automation->Test Setup Editor"
;~ 							3. Click on the menu.
;~ 							4. Select File->New Menu Item
;~ 							5. type a file name or select a existing file

; Expected Results  : 		After Step 2 a file browser will appear
;~ 							1. In step 3 if the existing file is selected a warning message will come.
;~ 							2. After step 2, the focus will return to test setup editor.
;~ 							3. The file path will be displayed as the title of Busmaster Window.
;~ 							4. Tree View(left side view)Will show a tree node "<New Test Setup File>

; Critical (C)		:		Y
; TestCase No.		:		TS_TestAuto_11
; TestCases			:		Addition of Test Case Sub Node
; Test Data			:
; Test Setup		:		1. Right Click on the test Case Node.
;~ 							2. Go to Menu "New" and select any sub node, Like Send, Verify, Wait and VerifyResponse

; Expected Results  : 		1. After  step 2 depending on the node selected a new tree item will be added to the tree.

; Critical (C)		:		Y
; TestCase No.		:		TS_TestAuto_12
; TestCases			:		Addition of Send Node
; Test Data			:
; Test Setup		:		1. Right Click on the Test Case Node.
;~ 							2. Go to Menu "New" and select send Node
;~ 							3.Go to Menu "New" and select verify Node
;~ 							4. Go to Menu "New" and select verifyRespone Node

; Expected Results  : 		1. After Step 2 the send node will be added to the test case

;~ 							2.After Step3 the verify node will be added to the test case

;~ 							3. After Step 4 the verifyResponse node will be added to the test case

; Critical (C)		:		Y
; TestCase No.		:		TS_TestAuto_13
; TestCases			:
; Test Data			:
; Test Setup		:		1. Select the Test Case Sub Node Send.
;~ 							2. Observe the Details in List Control.
;~ 							3. Select the Test Case Sub Node Verify.
;~ 							4. Observe the Details in List Control.

; Expected Results  : 		1.The First Row in the List Contol will display the number of Messages associated with SEND node.
;~ 							2. The Remaining rows will display all the messages.The first column contain the Message ID and second column will contain the message name
;~ 							After step 4,
;~ 							3.The First Row in the List Contol will display the number of Messages associated with verify node.
;~ 							4. The second row contains the failure classification.
;~ 							5 The Remaining rows will display all the messages.The first column contain the Message ID and second column will contain the message name

; Critical (C)		:		Y
; TestCase No.		:		TS_TestAuto_14
; TestCases			:		Addition of SEND_MESSAGE Node
; Test Data			:
; Test Setup		:		1. Select the Test Case Sub Node "Send"
;~ 							2. Go to the last row of the list and double click on "[Add Message]" column.
;~ 							3. Select a message to add.
;~ 							4. Repeat the above steps to add more messages.
;~ 							5. Click on the button Confirm.

; Expected Results  : 		1.After Step 2 a combo box will be show,which contains all the database messages
;~ 							2. After step 3 the selected message will be added in list box. Also a new row containing "[Add Message]" column is added to the list.
;~ 							3. After step 5 the new messages will be added to the tree viewe

; Critical (C)		:		Y
; TestCase No.		:		TS_TestAuto_15
; TestCases			:		Details of SEND_MESSAGE node
; Test Data			:
; Test Setup		:		1. Select the TestCase sub node SEND_MESSAGE.
;~ 							2. Observe the details in List control.

; Expected Results  : 		1. The fisrt row of the list will display the Signal unit type
;~ 							2. The Remaining row will display all the signals associated with message

; Critical (C)		:		Y
; TestCase No.		:		TS_TestAuto_16
; TestCases			:		Updation of SEND_MESSAGE node
; Test Data			:
; Test Setup		:		1. Select the TestCase sub node SEND_MESSAGE.
;~ 							 2. Double Click on the second column of first row.
;~ 							 3. Select the required option

; Expected Results  : 		1. After Step 2 a combo box will appear and it contains the values ENG, RAW
;~ 							2. After the step 3 the value in the list box will be change to the selected value.

; Critical (C)		:		Y
; TestCase No.		:		TS_TestAuto_17
; TestCases			:		Updation of SEND_MESSAGE node
; Test Data			:
; Test Setup		:		1. Select the TestCase sub node SEND_MESSAGE.
;~ 							2. Double click on the second columns of the signal rows

; Expected Results  : 		1. After step 2 a edit box will appear where user can enter a numeric value of the signal

; Critical (C)		:		Y
; TestCase No.		:		TS_TestAuto_19
; TestCases			:		Addition of VERIFY_MESSAGE  Node
; Test Data			:
; Test Setup		:		1. Select the Test Case Sub Node "Verirfy"
;~ 							2. Go to the last row of the list and double click on "[Add Message]" column.
;~ 							3. Select a message to add.
;~ 							4. Repeat the above steps to add more messages.
;~ 							5. Click on the button Confirm.

; Expected Results  : 		1. After Step 2 a combo box will be show,which contains all the database messages
;~ 							2. After step 3 the selected message will be added in list box. Also a new row containing "[Add Message]" column is added to the list.
;~ 							3. After step 5 the new messages will be added to the tree view

; Critical (C)		:		Y
; TestCase No.		:		TS_TestAuto_20
; TestCases			:		Details of VERIFY_MESSAGE node
; Test Data			:
; Test Setup		:		1. Select the TestCase sub node VERIFY_MESSAGE.
;~ 							2. Observe the details in List control.
;~ 							3. Double Click on the second column of first row.
;~ 							4. Select the required option
;~ 							5. Select the TestCase sub node VERIFY_MESSAGE.Double click on the second columns of the signal rows

; Expected Results  : 		1. The Fisrt row of the list will display the Signal unit type
;~ 							2. The Remaining row will display all the signals associated with message

;~ 							 After Step3 a combo box will appear and it contain the values ENG, RAW

;~ 							 After the step 4 the value in the list box will be change to the selected value.

;~ 							After step 5 a edit box will appear where user can enter a signal verification condition.

; Critical (C)		:		Y
; TestCase No.		:		TS_TestAuto_22
; TestCases			:		Updation of SEND Node
; Test Data			:
; Test Setup		:		1. Select the Test Case Sub Node Send.
;~ 							2.Add Some Messages To the Send Node.
;~ 							3. Click On confirm Button.
;~ 							4. Select the Test Case Sub Node Verify.
;~ 							5.Add Some Messages To the Verify Node.
;~ 							6.Select the Test Case Sub Node WAIT.
;~ 							7. Change the details such as WAIT period and WAIT purpose.
;~ 							8.Select the Test Case Sub Node VerifyResponse.
;~ 							9 Change the details such as wait time, failure classification
;~ 							10 Click on confirm button

; Expected Results  : 		1.After Step 10 The SEND and VERIFY node will be updated with the new Messages
;~ 								The details of the WAIT Node and Verifyresponse Node will be updated with the changes

; Critical (C)		:		Y
; TestCase No.		:		TS_TestAuto_24
; TestCases			:		TestCase Node - Exception Handle
; Test Data			:
; Test Setup		:		1. Select any Test Case Node.
;~ 							2. Double click on the Second column of the row whose title is "Time Mode"
;~ 							3. Select the required option

; Expected Results  : 		1. After step 3,
;~ 							The List Box will Show the selected Time Format

; Critical (C)		:		Y
; TestCase No.		:		TS_TestAuto_25
; TestCases			:		Verification Classification
; Test Data			:
; Test Setup		:		1. Select any Verify
;~ 							2. Double click on the Second column of the row whose title is "Failure Classification."
;~ 							3. Select the required option

; Expected Results  : 		1. After Second step,
;~ 							a combo box will be displayed with the following details
;~ 							A. WARNING
;~ 							B. ERROR
;~ 							C. FATAL

;~ 							 After step 3,
;~ 							The List Box will Show the selected option


; Critical (C)		:		Y
; TestCase No.		:		TS_TestAuto_26
; TestCases			:		Load a Test Setup File
; Test Data			:
; Test Setup		:		1.Open the busmaster Application.
;~ 							2. Select File->open Menu.
;~ 							3. Select a test Setup File to Load
;~ 							4. Change some values in header and note down the changes.
;~ 							5. Click on menu item File->Save.

; Expected Results  : 		1.After Step 2 a File browser Will appear.
;~ 							2.After Selecting File the Editor will be updated with the details of the Selected Automation File

;~ 							After Step5 the File will be updated with current changes.

; Critical (C)		:		Y
; TestCase No.		:		TS_TestAuto_27
; TestCases			:		Header Details -  Database File
; Test Data			:
; Test Setup		:		1.Select the Top most tree node on th right side of the editor.
;~ 							2. In list box, Double click on the second column in the row Whose title is "DataBase path"
;~ 							3. Click on the Button.
;~ 							4. Select the New Database file
;~ 							5. Double click on the second column in the row Whose title is "Version"
;~ 							6. Double click on the second column in the row Whose title is "Module"
;~ 							7. Double click on the second column in the row Whose title is "Engineer Name"
;~ 							8. Double click on the second column in the row Whose title is "Engineer Role"

; Expected Results  : 		1. After Second step, a edit box along with the button will be displayed.The Edit control will have the focus and it shows the currently selected database path

;~ 							After Step 3 a file browser will be displayed.
;~ 							After Step 4 the List box row will show the selected database file path.

;~ 							After step5 a edit box will be displayed where user can change the version information

;~ 							After step6 a edit box will be displayed where user can change the Module information

;~ 							After step7 a edit box will be displayed where user can give the Engineer name

;~ 							After step8 a edit box will be displayed where user can give the Engineer Role


; Critical (C)		:		Y
; TestCase No.		:		TS_TestAuto_10
; TestCases			:		Deletion of Test Case Node
; Test Data			:
; Test Setup		:		1. Right click on the Test Case Node.
;~ 							2. Click on the Delete Menu Item

; Expected Results  : 		1.After Step 2 the Test Case will be deleted from the tree node

; Critical (C)		:		Y
; TestCase No.		:		TS_TestAuto_18
; TestCases			:		Deletion of SEND_MESSAGE node
; Test Data			:
; Test Setup		:		1. Select the Send node
;~ 							2. In the list box select a message row.
;~ 							3. Double click on the second column.
;~ 							4. Select the value item "[Delete Message]"
;~ 							5. Click the confirm button

; Expected Results  : 		1. After Step 4 the selected send Message Node will be deleted.
;~ 							2. After Step 5 the tree node will be updated with the undeleted send nodes

; Critical (C)		:		Y
; TestCase No.		:		TS_TestAuto_21
; TestCases			:		Deletion of VERIFY_MESSAGE node
; Test Data			:
; Test Setup		:		1. Select the Verify node
;~ 							2. In the list box select a message row.
;~ 							3. Double click on th second column.
;~ 							4. Select the value item "[Delete Message]"
;~ 							5. Click the confirm button

; Expected Results  : 		1. After Step 4 The selected Verify Message Node will be deleted.
;~ 							2. After Step 5 the tree node will be updated with the undeleted verify nodes

; Critical (C)		:		Y
; TestCase No.		:		TS_TestAuto_23
; TestCases			:		Deletion of TestCase Sub Node
; Test Data			:
; Test Setup		:		1. select any Test Case sub node like SEND, VERFIY, WAIT, REPLAY.
;~ 							2. Right click on the Node
;~ 							3. select DELETE command.
;~ 							4. Select any Test Case Node. and observe List box
;~ 							5. Double click on the Second column of the row whose title is "Test Case Name"
;~ 							6. Change the name of the Test Case node and press enter
;~ 							7. select any test case and double click on the Second column of the row whose title is "On Exception"

; Expected Results  : 		1.After Step 2 a Context Menu Will appear
;~ 							2.After Step 3 the selected node will be deleted from the Tree control

;~ 							 After Step 4 List box will show the testcase details
;~ 							 After the step 5 an edit box will displayed and it contains the Testcase name.

;~ 							 After the step6 The list box will show the updated Test case name

;~ 							 After  step 7,
;~ 							a combo box will be displayed with the following details
;~ 							A. CONTINUE
;~ 							B. EXIT

;==========================================================================Test Procedure =========================================================================


ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("***********Test Automation CAN Script Execution Started************"&@CRLF)
ConsoleWrite("****Start : TS_TestAuto_01.au3****"&@CRLF)

_launchApp()

ProcessWait("BUSMASTER.exe")																	; Pauses script execution until a given process exists.
sleep(1000)

WinActivate($WIN_BUSMASTER)

Local $TestSetupInfo=0,$TestCaseInfo=0,$SendItemInfo=0,$SendMsgItemInfo=0,$WaitItemInfo=0, $TCNodeChildItemInfo=0, $VerifyItemInfo=0, $VerifyMsgItemInfo=0, $SendChildItems=1, $VerifyChildItems=1, $TCChildItems=1, $TestSetUpItems=1

if winexists($WIN_BUSMASTER) then

	_loadConfig("cfxCANTestAuto_01")																; Load configuration

	_OpenTestAutomationEditor()																		; Open Test Automation editor

	$TestDataPath=_TestDataPath()									 								; Fetch the test data path

	$OutPutPath=_OutputDataPath()																	; Fetch the output data path

	WinMenuSelectItem($WIN_BUSMASTER,"",$TestAutomationFileMenu,$TestAutomationNewMenu)				; Select File->New from menu

	WinWaitActive($Win_TestSetup_TestEditor,"",3)

	ControlSend($Win_TestSetup_TestEditor,"",$Edit_FName_NewTestSetupFile,$OutPutPath&"\TestAuto_01")	; Enter the File name
	sleep(750)

	ControlClick($Win_TestSetup_TestEditor,"",$BTN_Save_NewTestSetupFile)							; Click on Save button
	sleep(1000)

	_SelectTestSetUpNode()																			; Select Test Set up node

	$TVHWD= ControlGetHandle($WIN_BUSMASTER,"",$TVC_TestAuto_TestEditor)							; Fetch Test Editor tree view handle

	$TestSetUpNodeHWD=_GUICtrlTreeView_GetSelection($TVHWD)											; Fetch the handle of the selected node

	$LVhWnd = _GetTestEditorDetailsHWD()															; Fetch Test Editor list view handle

	_GUICtrlListView_SetItemText($LVhWnd,0, "TestTitle",1)											; Set the Title
	sleep(1000)

	_GUICtrlListView_SetItemText($LVhWnd,1, "TestDescription",1)									; Set the Description
	sleep(1000)

	_GUICtrlListView_SetItemText($LVhWnd,4, "TestModule",1)											; Set the Module Name
	sleep(1000)

	_GUICtrlListView_SetItemText($LVhWnd,5, "TestEngName",1)										; Set the Eng Name
	sleep(1000)

	_GUICtrlListView_SetItemText($LVhWnd,6, "TestEngRole",1)										; Set the Eng Role
	sleep(1000)

	_GUICtrlListView_SetItemText($LVhWnd,7,$TestDataPath & "\StdExtDB.dbf",1)						; Set the DB file path
	sleep(1000)

	send("{ENTER}")
	sleep(500)

	_GUICtrlListView_SetItemText($LVhWnd,8,$OutPutPath & "\Report_01.TXT",1)						; Set the report file path
	sleep(1000)

	_ClickTestEditConfirmBTN()																		; Click on Confirm button

	Send("^{F4}")																					; Close Test Editor window
	sleep(1000)

	_OpenTestAutomationEditor()																		; Open Test Automation editor

	_SelectTestSetUpNode()																			; Select Test Set up node

	$TestSetUpItemCount=_GUICtrlListView_GetItemCount($LVhWnd)										; Fetch the item count in test setup details
	ConsoleWrite("$TestSetUpItemCount:"&$TestSetUpItemCount&@CRLF)

	if $TestSetUpItemCount=10 Then

		$Title=_GUICtrlListView_GetItemText($LVhWnd,0,1)												; Fetch the title

		$Description=_GUICtrlListView_GetItemText($LVhWnd,1,1)											; Fetch the description

		$Version=_GUICtrlListView_GetItemText($LVhWnd,2,1)												; Fetch the version

		$BusType=_GUICtrlListView_GetItemText($LVhWnd,3,1)												; Fetch bus type

		$ModName=_GUICtrlListView_GetItemText($LVhWnd,4,1)												; Fetch module name

		$EngName=_GUICtrlListView_GetItemText($LVhWnd,5,1)												; Fetch engineer name

		$EngRole=_GUICtrlListView_GetItemText($LVhWnd,6,1)												; Fetch engineer role

		$DBPath=_GUICtrlListView_GetItemText($LVhWnd,7,1)												; Fetch database path

		$ReportPath=_GUICtrlListView_GetItemText($LVhWnd,8,1)											; Fetch report path

		$TimeMode=_GUICtrlListView_GetItemText($LVhWnd,9,1)												; Fetch time mode

		ConsoleWrite("$Title:"&$Title&@CRLF)
		ConsoleWrite("$Description:"&$Description&@CRLF)
		ConsoleWrite("$Version:"&$Version&@CRLF)
		ConsoleWrite("$BusType:"&$BusType&@CRLF)
		ConsoleWrite("$ModName:"&$ModName&@CRLF)
		ConsoleWrite("$EngName:"&$EngName&@CRLF)
		ConsoleWrite("$EngRole:"&$EngRole&@CRLF)
		ConsoleWrite("$DBPath:"&$DBPath&@CRLF)
		ConsoleWrite("$ReportPath:"&$ReportPath&@CRLF)
		ConsoleWrite("$TimeMode:"&$TimeMode&@CRLF)

		if $Title="TestTitle" and $Description="TestDescription" and $Version=1.0 and $BusType="CAN" and $ModName="TestModule" and $EngName="TestEngName" and $EngRole="TestEngRole" and $DBPath=$TestDataPath&"\StdExtDB.dbf" and $ReportPath=$OutPutPath&"\Report_01.TXT" and $TimeMode="ABSOLUTE" Then
			$TestSetupInfo=1
		EndIf

	EndIf

	_SelectTestSetUpNode()																			; Select Test Set up node

	$TVHWD= ControlGetHandle($WIN_BUSMASTER,"",$TVC_TestAuto_TestEditor)							; Fetch Test Editor tree view handle

	$TestSetUpNodeHWD=_GUICtrlTreeView_GetSelection($TVHWD)											; Fetch the handle of the selected node

	_GUICtrlTreeView_ClickItem($TVHWD,$TestSetUpNodeHWD,"Right")									; Right click on the selected node
	sleep(1000)

	send("n")																						; Select "New TestCase" from context menu
	sleep(1000)

	_ClickTestEditConfirmBTN()																		; Click on Confirm button

	_ExpandTestEditorTreeView()																		; Expand the tree view

	_SelectTestCaseNode()																			; Select test case node

	_GUICtrlListView_SetItemText($LVhWnd,0,001,1)													; Set the test case ID
	sleep(1000)

	_GUICtrlListView_SetItemText($LVhWnd,1,"Test_001",1)											; Set the test case name
	sleep(1000)

	_ClickTestEditConfirmBTN()																		; Click on Confirm button

	$TestCaseItemCount=_GUICtrlListView_GetItemCount($LVhWnd)										; Fetch the item count in test case details
	ConsoleWrite("$TestCaseItemCount:"&$TestCaseItemCount&@CRLF)

	if $TestCaseItemCount=3 Then

		$TCIDTxt=_GUICtrlListView_GetItemText($LVhWnd,0,0)												; Fetch test case ID text

		$TCID=_GUICtrlListView_GetItemText($LVhWnd,0,1)													; Fetch test case ID

		$TCNameTxt=_GUICtrlListView_GetItemText($LVhWnd,1,0)											; Fetch test case name text

		$TCName=_GUICtrlListView_GetItemText($LVhWnd,1,1)												; Fetch test case name

		$ExcpHandTxt=_GUICtrlListView_GetItemText($LVhWnd,2,0)											; Fetch exception handler text

		$ExcpHand=_GUICtrlListView_GetItemText($LVhWnd,2,1)												; Fetch exception handler value

		ConsoleWrite("$TCIDTxt:"&$TCIDTxt&@CRLF)
		ConsoleWrite("$TCID:"&$TCID&@CRLF)
		ConsoleWrite("$TCNameTxt:"&$TCNameTxt&@CRLF)
		ConsoleWrite("$TCName:"&$TCName&@CRLF)
		ConsoleWrite("$ExcpHandTxt:"&$ExcpHandTxt&@CRLF)
		ConsoleWrite("$ExcpHand:"&$ExcpHand&@CRLF)

		If $TCIDTxt="Test Case ID" and $TCID=1 and $TCNameTxt="Test Case Name" and $TCName="Test_001" and $ExcpHandTxt="Exception Handler" and $ExcpHand="CONTINUE" Then
			$TestCaseInfo=1
		EndIf

	EndIf

	_ExpandTestEditorTreeView()																		; Expand the tree view

	_SelectTestCaseNode()																			; Select the test case item in the tree view

	$TestCaseHWD=_GUICtrlTreeView_GetSelection($TVHWD)												; Fetch the handle of the selected node

	_GUICtrlTreeView_ClickItem($TVHWD,$TestCaseHWD,"Right")											; Right click on the selected node
	sleep(1000)

	send("n")																						; Select "New" from context menu
	sleep(500)

	send("s")																						; Select "Send" from context menu
	sleep(500)

	_ClickTestEditConfirmBTN()																		; Click on Confirm button

	_ExpandTestEditorTreeView()																		; Expand the tree view

	_SelectSendNode()																				; Select msg in the send node

	$SendItemPos=_GUICtrlListView_GetItemPosition($LVhWnd, 1)										; Fetch the "[Add Message]" row position

	$CtgColWidth=_GUICtrlListView_GetColumnWidth($LVhWnd,0)											; Fetch the first coulmn width

	ControlClick($WIN_BUSMASTER,"",$LVC_TestAuto_TestEditor,"Left",2,$SendItemPos[0]+$CtgColWidth+3,$SendItemPos[1])	; Double click on the "[Add Message]"
	sleep(1000)

	send("{DOWN 2}")																				; Select the msg
	sleep(1000)

	send("{ENTER}")
	sleep(1000)

	_ClickTestEditConfirmBTN()																		; Click on Confirm button

	$SendItemCount=_GUICtrlListView_GetItemCount($LVhWnd)											; Fetch the item count in Send node
	ConsoleWrite("$SendItemCount:"&$SendItemCount&@CRLF)

	if $SendItemCount=3 Then

		$SendMsgID=_GUICtrlListView_GetItemText($LVhWnd,1,0)											; Fetch the Send Msg ID

		$SendMsgName=_GUICtrlListView_GetItemText($LVhWnd,1,1)											; Fetch the Send Msg Name

		ConsoleWrite("$SendMsgID:"&$SendMsgID&@CRLF)
		ConsoleWrite("$SendMsgName:"&$SendMsgName&@CRLF)

		if $SendMsgID=257 and $SendMsgName="MsgStdLil" Then
			$SendItemInfo=1
		EndIf

	EndIf

	_ExpandTestEditorTreeView()																		; Expand the tree view

	_SelectSendMsgNode()																			; Select msg in the send node

	_GUICtrlListView_SetItemText($LVhWnd,1,1,1)														; Set the report file path
	sleep(1000)

	_ClickTestEditConfirmBTN()																		; Click on Confirm button

	$SendMsgItemCount=_GUICtrlListView_GetItemCount($LVhWnd)										; Fetch the item count in Send Msg node
	ConsoleWrite("$SendMsgItemCount:"&$SendMsgItemCount&@CRLF)

	if $SendMsgItemCount=2 then

		$SigUnit=_GUICtrlListView_GetItemText($LVhWnd,0,0)												; Fetch Signal Unit type text

		$SigUnitType=_GUICtrlListView_GetItemText($LVhWnd,0,1)											; Fetch unit type

		$MsgStdLilSig=_GUICtrlListView_GetItemText($LVhWnd,1,0)											; Fetch the Signal

		$MsgStdLilSigVal=_GUICtrlListView_GetItemText($LVhWnd,1,1)										; Fetch the Signal Value

		ConsoleWrite("$SigUnit:"&$SigUnit&@CRLF)
		ConsoleWrite("$SigUnitType:"&$SigUnitType&@CRLF)
		ConsoleWrite("$MsgStdLilSig:"&$MsgStdLilSig&@CRLF)
		ConsoleWrite("$MsgStdLilSigVal:"&$MsgStdLilSigVal&@CRLF)

		if $SigUnit="Signal Unit Type" and $SigUnitType="ENG" and $MsgStdLilSig="Sigstd1" and $MsgStdLilSigVal=1 Then
			$SendMsgItemInfo=1
		EndIf
	EndIf

	_GUICtrlTreeView_ClickItem($TVHWD,$TestCaseHWD,"Right")											; Right click on the selected node
	sleep(1000)

	send("n")																						; Select "New" from context menu
	sleep(500)

	send("w")																						; Select "Wait" from context menu
	sleep(500)

	send("{ENTER}")
	sleep(1000)

	_ClickTestEditConfirmBTN()																		; Click on Confirm button

	_ExpandTestEditorTreeView()																		; Expand the tree view

	_SelectWaitNode()																				; Select wait node

	_GUICtrlListView_SetItemText($LVhWnd,0,"Test",1)												; Set the purpose
	sleep(1000)

	_ClickTestEditConfirmBTN()																		; Click on Confirm button

	$WaitItemCount=_GUICtrlListView_GetItemCount($LVhWnd)										; Fetch the item count in Send Msg node
	ConsoleWrite("$WaitItemCount:"&$WaitItemCount&@CRLF)

	if $WaitItemCount=2 then

		$PurposeVal=_GUICtrlListView_GetItemText($LVhWnd,0,1)											; Fetch the purpose value

		$DelayVal=_GUICtrlListView_GetItemText($LVhWnd,1,1)												; Fetch the delay value

		ConsoleWrite("$PurposeVal:"&$PurposeVal&@CRLF)
		ConsoleWrite("$DelayVal:"&$DelayVal&@CRLF)

		if $PurposeVal="Test" and $DelayVal=100 Then
			$WaitItemInfo=1
		EndIf
	EndIf

	_GUICtrlTreeView_ClickItem($TVHWD,$TestCaseHWD,"Right")											; Right click on the selected node
	sleep(1000)

	send("n")																						; Select "New" from context menu
	sleep(500)

	send("v")																						; Select "Verify" from context menu
	sleep(500)

	send("{ENTER}")
	sleep(1000)

	_ClickTestEditConfirmBTN()																		; Click on Confirm button

	_ExpandTestEditorTreeView()																		; Expand the tree view

	_SelectVerifyNode()																				; Select verify node

	$FailureClassPos=_GUICtrlListView_GetItemPosition($LVhWnd, 1)									; Fetch the "Failure classification" row position
	$VerifyItemPos=_GUICtrlListView_GetItemPosition($LVhWnd, 2)										; Fetch the "[Add Message]" row position
	$CtgColWidth=_GUICtrlListView_GetColumnWidth($LVhWnd,0)											; Fetch the first coulmn width

	ControlClick($WIN_BUSMASTER,"",$LVC_TestAuto_TestEditor,"Left",2,$FailureClassPos[0]+$CtgColWidth+3,$FailureClassPos[1])	; Double click on the "Failure classification dropdown box"
	sleep(1000)

	send("{UP}")																					; Select Warning
	sleep(1000)

	$WarningTxt=ControlCommand ($WIN_BUSMASTER,"","","GetCurrentSelection")							; Fetch the dropdown item text

	send("{DOWN}")																					; Select error
	sleep(1000)

	$ErrorTxt=ControlCommand ($WIN_BUSMASTER,"","","GetCurrentSelection")

	send("{DOWN}")																					; Select Fatal
	sleep(1000)

	$FatalTxt=ControlCommand ($WIN_BUSMASTER,"","","GetCurrentSelection")

	send("{ENTER}")
	sleep(1000)

	ConsoleWrite("$WarningTxt:"&$WarningTxt&@CRLF)
	ConsoleWrite("$ErrorTxt:"&$ErrorTxt&@CRLF)
	ConsoleWrite("$FatalTxt:"&$FatalTxt&@CRLF)

	ControlClick($WIN_BUSMASTER,"",$LVC_TestAuto_TestEditor,"Left",2,$VerifyItemPos[0]+$CtgColWidth+3,$VerifyItemPos[1])	; Double click on the "[Add Message]"
	sleep(1000)

	send("{DOWN 2}")																				; Select the msg
	sleep(1000)

	send("{ENTER}")
	sleep(1000)

	_ClickTestEditConfirmBTN()																		; Click on Confirm button

	$TCNodeChildItems=ControlTreeView ($WIN_BUSMASTER,"",$TVC_TestAuto_TestEditor,"GetItemCount","#0|#0")		; fetch the no. of child items for test case node
	sleep(500)

	ConsoleWrite("$TCNodeChildItems:"&$TCNodeChildItems&@CRLF)

	if $TCNodeChildItems=3 Then
		$SendItemTxt=ControlTreeView ($WIN_BUSMASTER,"",$TVC_TestAuto_TestEditor,"GetText","#0|#0|#0")
		$WaitItemTxt=ControlTreeView ($WIN_BUSMASTER,"",$TVC_TestAuto_TestEditor,"GetText","#0|#0|#1")
		$VerifyItemTxt=ControlTreeView ($WIN_BUSMASTER,"",$TVC_TestAuto_TestEditor,"GetText","#0|#0|#2")
		ConsoleWrite("$SendItemTxt:"&$SendItemTxt&@CRLF)
		ConsoleWrite("$WaitItemTxt:"&$WaitItemTxt&@CRLF)
		ConsoleWrite("$VerifyItemTxt:"&$VerifyItemTxt&@CRLF)

		if $SendItemTxt="Send" and $WaitItemTxt="Wait" and $VerifyItemTxt="Verify" Then
			$TCNodeChildItemInfo=1
		EndIf

	EndIf

	$VerifyItemCount=_GUICtrlListView_GetItemCount($LVhWnd)											; Fetch the item count in Verify node
	ConsoleWrite("$VerifyItemCount:"&$VerifyItemCount&@CRLF)

	if $VerifyItemCount=4 Then

		$MsgCountVal=_GUICtrlListView_GetItemText($LVhWnd,0,1)											; Fetch Msg count val

		$FailureClassificationVal=_GUICtrlListView_GetItemText($LVhWnd,1,1)								; Fetch failure classification value

		$VerifyMsgID=_GUICtrlListView_GetItemText($LVhWnd,2,0)											; Fetch msg ID

		$VerifyMsgName=_GUICtrlListView_GetItemText($LVhWnd,2,1)										; Fetch msg name

		$AddMsgTxt=_GUICtrlListView_GetItemText($LVhWnd,3,1)										; Fetch Add message text

		ConsoleWrite("$MsgCountVal:"&$MsgCountVal&@CRLF)
		ConsoleWrite("$FailureClassificationVal:"&$FailureClassificationVal&@CRLF)
		ConsoleWrite("$VerifyMsgID:"&$VerifyMsgID&@CRLF)
		ConsoleWrite("$VerifyMsgName:"&$VerifyMsgName&@CRLF)
		ConsoleWrite("$AddMsgTxt:"&$AddMsgTxt&@CRLF)


		if $MsgCountVal=1 and $FailureClassificationVal="FATAL" and $VerifyMsgID=257 and $VerifyMsgName="MsgStdLil" and $AddMsgTxt="[Add Message]" and $WarningTxt="WARNING" and $ErrorTxt="ERROR" and $FatalTxt="FATAL" Then
			$VerifyItemInfo=1
		EndIf

	EndIf

	_ExpandTestEditorTreeView()																		; Expand the tree view

	_SelectVerifyMsgNode()																			; Select the verify msg node

	$VerifyMsgItemPos=_GUICtrlListView_GetItemPosition($LVhWnd, 0)									; Fetch the "Signal Unit Type" row position

	$CtgColWidth=_GUICtrlListView_GetColumnWidth($LVhWnd,0)											; Fetch the first coulmn width

	ControlClick($WIN_BUSMASTER,"",$LVC_TestAuto_TestEditor,"Left",2,$VerifyMsgItemPos[0]+$CtgColWidth+3,$VerifyMsgItemPos[1])	; Double click on the "Signal Unit Type"
	sleep(1000)

	send("{DOWN}")																					; Select the type
	sleep(1000)

	send("{ENTER}")
	sleep(1000)

	_GUICtrlListView_SetItemText($LVhWnd,1,"X==1",1)												; Set the ENG value
	sleep(1000)

	_ClickTestEditConfirmBTN()																		; Click on Confirm button

	$VerifyMsgItemCount=_GUICtrlListView_GetItemCount($LVhWnd)											; Fetch the item count in Verify msg node
	ConsoleWrite("$VerifyMsgItemCount:"&$VerifyMsgItemCount&@CRLF)

	if $VerifyMsgItemCount=2 Then

		$VerifySigType=_GUICtrlListView_GetItemText($LVhWnd,0,1)										; Fetch the Signal type

		$VerifySigName=_GUICtrlListView_GetItemText($LVhWnd,1,0)										; Fetch the Signal Name

		$VerifySigVal=_GUICtrlListView_GetItemText($LVhWnd,1,1)											; Fetch the Signal val

		ConsoleWrite("$VerifySigType:"&$VerifySigType&@CRLF)
		ConsoleWrite("$VerifySigName:"&$VerifySigName&@CRLF)
		ConsoleWrite("$VerifySigVal:"&$VerifySigVal&@CRLF)

		if $VerifySigType="ENG" and $VerifySigName="Sigstd1" and $VerifySigVal="X==1" Then
			$VerifyMsgItemInfo=1
		EndIf
	EndIf

	_SelectSendNode()																				; Select msg in the send node

	$SendItemPos=_GUICtrlListView_GetItemPosition($LVhWnd, 1)										; Fetch the "[Add Message]" row position

	$CtgColWidth=_GUICtrlListView_GetColumnWidth($LVhWnd,0)											; Fetch the first coulmn width

	ControlClick($WIN_BUSMASTER,"",$LVC_TestAuto_TestEditor,"Left",2,$SendItemPos[0]+$CtgColWidth+3,$SendItemPos[1])	; Double click on the "[Add Message]"
	sleep(1000)

	send("{UP}")																					; Select Delete Msg from the dropdown list
	sleep(1000)

	$SelectedSendDropDownitem=ControlCommand ($WIN_BUSMASTER,"","","GetCurrentSelection")				; Fetch the dropdown item text

	ConsoleWrite("$SelectedSendDropDownitem:"&$SelectedSendDropDownitem&@CRLF)

	if $SelectedSendDropDownitem="[Delete Message]" Then
		send("{ENTER}")
		sleep(1000)
	Else
		send("{ESC}")
	EndIf

	_ClickTestEditConfirmBTN()																		; Click on Confirm button

	_SelectVerifyNode()																				; Select verify node

	$VerifyItemPos=_GUICtrlListView_GetItemPosition($LVhWnd, 2)										; Fetch the "[Add Message]" row position
	$CtgColWidth=_GUICtrlListView_GetColumnWidth($LVhWnd,0)											; Fetch the first coulmn width

	ControlClick($WIN_BUSMASTER,"",$LVC_TestAuto_TestEditor,"Left",2,$VerifyItemPos[0]+$CtgColWidth+3,$VerifyItemPos[1])	; Double click on the "[Add Message]"
	sleep(1000)

	send("{UP}")																				; Select the msg
	sleep(1000)

	$SelectedVerifyDropDownitem=ControlCommand ($WIN_BUSMASTER,"","","GetCurrentSelection")				; Fetch the dropdown item text

	ConsoleWrite("$SelectedVerifyDropDownitem:"&$SelectedVerifyDropDownitem&@CRLF)

	if $SelectedVerifyDropDownitem="[Delete Message]" Then
		send("{ENTER}")
		sleep(1000)
	Else
		send("{ESC}")
	EndIf

	_ClickTestEditConfirmBTN()																		; Click on Confirm button

	$SendChildItems=ControlTreeView ($WIN_BUSMASTER,"",$TVC_TestAuto_TestEditor,"GetItemCount","#0|#0|#0")		; fetch the no. of child items in send node
	sleep(500)

	$VerifyChildItems=ControlTreeView ($WIN_BUSMASTER,"",$TVC_TestAuto_TestEditor,"GetItemCount","#0|#0|#1")	; fetch the no. of child items in verify node
	sleep(500)

	ConsoleWrite("$SendChildItems:"&$SendChildItems&@CRLF)
	ConsoleWrite("$VerifyChildItems:"&$VerifyChildItems&@CRLF)

	_SelectVerifyNode()																				; Select verify node

	$TVHWD= ControlGetHandle($WIN_BUSMASTER,"",$TVC_TestAuto_TestEditor)							; Fetch Test Editor tree view handle

	$VerifyNodeHWD=_GUICtrlTreeView_GetSelection($TVHWD)											; Fetch the handle of the selected node

	_GUICtrlTreeView_ClickItem($TVHWD,$VerifyNodeHWD,"Right")										; Right click on the selected node
	sleep(1000)

	send("d")																						; Select "Delete" from context menu
	sleep(500)

	_ClickTestEditConfirmBTN()																		; Click on Confirm button

	_SelectWaitNode()																				; Select wait node

	$WaitNodeHWD=_GUICtrlTreeView_GetSelection($TVHWD)												; Fetch the handle of the selected node

	_GUICtrlTreeView_ClickItem($TVHWD,$WaitNodeHWD,"Right")											; Right click on the selected node
	sleep(1000)

	send("d")																						; Select "Delete" from context menu
	sleep(500)

	_ClickTestEditConfirmBTN()																		; Click on Confirm button

	_SelectSendNode()																				; Select send node

	$SendNodeHWD=_GUICtrlTreeView_GetSelection($TVHWD)												; Fetch the handle of the selected node

	_GUICtrlTreeView_ClickItem($TVHWD,$SendNodeHWD,"Right")											; Right click on the selected node
	sleep(1000)

	send("d")																						; Select "Delete" from context menu
	sleep(500)

	_ClickTestEditConfirmBTN()																		; Click on Confirm button

	$TCChildItems=ControlTreeView ($WIN_BUSMASTER,"",$TVC_TestAuto_TestEditor,"GetItemCount","#0|#0")	; fetch the no. of child items in test case node
	sleep(500)

	ConsoleWrite("$TCChildItems:"&$TCChildItems&@CRLF)

	_SelectTestCaseNode()																			; Select test case node

	$TCNodeHWD=_GUICtrlTreeView_GetSelection($TVHWD)												; Fetch the handle of the selected node

	_GUICtrlTreeView_ClickItem($TVHWD,$TCNodeHWD,"Right")											; Right click on the selected node
	sleep(1000)

	send("d")																						; Select "Delete" from context menu
	sleep(500)

	_ClickTestEditConfirmBTN()																		; Click on Confirm button

	$TestSetUpItems=ControlTreeView ($WIN_BUSMASTER,"",$TVC_TestAuto_TestEditor,"GetItemCount","#0")	; fetch the no. of child items in test set up node
	sleep(500)

	ConsoleWrite("$TestSetUpItems:"&$TestSetUpItems&@CRLF)

	Send("^{F4}")																					; Close Test Editor window
	sleep(1000)

	ConsoleWrite("$TestSetupInfo:"&$TestSetupInfo&@CRLF)
	ConsoleWrite("$TestCaseInfo:"&$TestCaseInfo&@CRLF)
	ConsoleWrite("$SendItemInfo:"&$SendItemInfo&@CRLF)
	ConsoleWrite("$SendMsgItemInfo:"&$SendMsgItemInfo&@CRLF)
	ConsoleWrite("$WaitItemInfo:"&$WaitItemInfo&@CRLF)
	ConsoleWrite("$TCNodeChildItemInfo:"&$TCNodeChildItemInfo&@CRLF)
	ConsoleWrite("$VerifyItemInfo:"&$VerifyItemInfo&@CRLF)
	ConsoleWrite("$VerifyMsgItemInfo:"&$VerifyMsgItemInfo&@CRLF)

EndIf


if $TestSetupInfo=1 and $TestCaseInfo=1 and $SendItemInfo=1 and $SendMsgItemInfo=1 and $WaitItemInfo=1 and $TCNodeChildItemInfo=1 And $VerifyItemInfo=1 And $VerifyMsgItemInfo=1 and $SendChildItems=0 and $VerifyChildItems=0 and $TCChildItems=0 Then				; Validate the result
	_WriteResult("Pass","TS_TestAuto_01")
Else
	_WriteResult("Fail","TS_TestAuto_01")
EndIf

ConsoleWrite("****End : TS_TestAuto_01.au3****"&@CRLF)
ConsoleWrite(@CRLF)