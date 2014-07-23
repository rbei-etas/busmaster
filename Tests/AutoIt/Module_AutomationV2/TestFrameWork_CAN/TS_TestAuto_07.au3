;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_TestAuto_07
; TestCases			:		Saving the configuration data
; Test Data			:
; Test Setup		:		1. Open the Busmaster and Executor.
;~ 							2. Add Some Test Setup Files.
;~ 							3. Select some test setup files and some Test Case
;~ 							4. Note down the Selected,deselected Nodes and the position of the window.
;~ 							6. Try to Close the Busmaster
;~ 							7. Select yes to save the changes.
;~ 							8. Close and open the Busmaster and open the Executor

; Expected Results  : 		The Busmaster will ask the user to save the changes made or not.
;~ 							After Step 8 the Details of the node selection and Position of the executor will be saved.

; Critical (C)		:		Y
; TestCase No.		:		TS_TestAuto_28
; TestCases			:		Invoke The TestAutomation Executor
; Test Data			:
; Test Setup		:		1.Open Busmaster
;~ 							2. Select Automation->Executor.
;~ 							3. Check the GUI elements in Executor.
;~ 							4. Double Click on the <Untitled TestSuite> Menu item
;~ 							5. Give Test Suite Name and press enter

; Expected Results  : 		1.After Step2 The Executor will Appear

;~ 							2.After step3,the executor will contain a tree control on left side and a list control on right side.A untitled testsuite will be added to the tree view.

;~ 							After step4,the Tree Item will give access to change its name
;~ 							After step5 the Name of The Test Suite will changed to new name

; Critical (C)		:		Y
; TestCase No.		:		TS_TestAuto_29
; TestCases			:		Delete a test setup node
; Test Data			:
; Test Setup		:		1.Right Click on any Test setup File.
;~ 							2. Select "Delete" Menu Item

; Expected Results  : 		1.After Step 2 the selected testsetup file will be deleted from the test suite.

; Critical (C)		:		Y
; TestCase No.		:		TS_TestAuto_30
; TestCases			:		Select\Deselct a test case
; Test Data			:
; Test Setup		:		1. Click on a check box of any Test case to select or deselect it.

; Expected Results  : 		1.Aftet step1 if the check box is empty it will be selected or if it is selected it will ber deselected from the execution

; Critical (C)		:		Y
; TestCase No.		:		TS_TestAuto_31
; TestCases			:		Select\Deselect a test setup node
; Test Data			:
; Test Setup		:		1. Click on a check box of any Test setup node select or deselect it.

; Expected Results  : 		1.Aftet step1 if the check box is empty it will be selected or if it is selected it will be deselected from the execution

; Critical (C)		:		Y
; TestCase No.		:		TS_TestAuto_32
; TestCases			:		Context Menu states
; Test Data			:
; Test Setup		:		1. Continue from the abover test case.
;~ 							2. Deselect all the test setup files from the execution.
;~ 							3. Right click on the Test suite name.
;~ 							4. observe the menu states

; Expected Results  : 		1. The menu Add will be in enabled state.
;~ 							2. The Execution will be in disabled state

; Critical (C)		:		Y
; TestCase No.		:		TS_TestAuto_33
; TestCases			:		Loading New configuration
; Test Data			:
; Test Setup		:		1. Add some test setup files to executor.
;~ 							2. save the configuration with a name.
;~ 							3. Open a new configuration file.
;~ 							4. observe the executor.
;~ 							5. continue fom the above test case.
;~ 							6. Load the previously saved configuration.
;~ 							7. observe the executor

; Expected Results  : 		1. The executor will go to its default view with zero test setup files.
;~ 							2. After step 7 the executor will go to previous view with all the test setup files

; Critical (C)		:		Y
; TestCase No.		:		TS_TestAuto_34
; TestCases			:		Deleted test setup files.
; Test Data			:
; Test Setup		:		1. Continue from the above test case.
;~ 							2. Load a new configuration.
;~ 							3. Delete a test setup file that is added in the previous configuration.
;~ 							4. Load the previous configuration.
;~ 							5. Check the executor.

; Expected Results  : 		1. The executor will load all the existing test setup files with out any exception.

;==========================================================================Test Procedure =========================================================================


ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("***********Test Automation CAN Script Execution Started************"&@CRLF)
ConsoleWrite("****Start : TS_TestAuto_07.au3****"&@CRLF)

_launchApp()

ProcessWait("BUSMASTER.exe")																	; Pauses script execution until a given process exists.
sleep(1000)

WinActivate($WIN_BUSMASTER,3)

local $TestSuiteName="",$IsTestSuiteChecked=0,$IsTestSetUpChecked=1,$IsTestCaseChecked=1,$TestSetUpCount=1
Dim $TestSuiteMenus[10]=["","","","","","","","","",""]

if winexists($WIN_BUSMASTER) then

	_CreateConfig("cfxCANTestAuto_07")																; Create configuration

	_OpenTestAutomationExecutor()																	; Open test automation executor window

	_AddTestExeFile("TestAuto_07.xml")																; add test setup file

	_ExpandTestExeTreeView()																		; Expand the tree view

	ControlTreeView ($WIN_BUSMASTER,"",$TVC_TestSuite_TestExecutor,"Check","#0")					; Check the test suite item in tree view
	sleep(750)

	ControlTreeView ($WIN_BUSMASTER,"",$TVC_TestSuite_TestExecutor,"Uncheck","#0|#0")				; Uncheck the test setup file
	sleep(750)

	ControlTreeView ($WIN_BUSMASTER,"",$TVC_TestSuite_TestExecutor,"Uncheck","#0|#0|#1")			; Uncheck the first testcase
	sleep(750)

	ControlTreeView ($WIN_BUSMASTER,"",$TVC_TestSuite_TestExecutor,"Select","#0")					; Select test suite
	sleep(750)

	send("{F2}")																					; Send F2 from keyboard to rename the test suite
	sleep(750)

	send("SampleTestSuite")
	sleep(750)

	send("{ENTER}")
	Sleep(1000)

	send("^{F4}")																					; Close Test executor window
	sleep(1000)

	_saveConfig()																					; Save configuration

	_CloseApp()																						; Close the application

	_launchApp()																					; Launch the application

	_OpenTestAutomationExecutor()																	; Open test automation executor window

	$TestSuiteName=ControlTreeView ($WIN_BUSMASTER,"",$TVC_TestSuite_TestExecutor,"GetText","#0")	; Fetch the test suite name

	$IsTestSuiteChecked=ControlTreeView ($WIN_BUSMASTER,"",$TVC_TestSuite_TestExecutor,"IsChecked","#0")				; Check the test suite item in tree view
	sleep(100)

	$IsTestSetUpChecked=ControlTreeView ($WIN_BUSMASTER,"",$TVC_TestSuite_TestExecutor,"IsChecked","#0|#0")				; Uncheck the test setup file
	sleep(100)

	$IsTestCaseChecked=ControlTreeView ($WIN_BUSMASTER,"",$TVC_TestSuite_TestExecutor,"IsChecked","#0|#0|#1")			; Uncheck the first testcase

	$TEHWD=ControlGetHandle($WIN_BUSMASTER, "", $TVC_TestSuite_TestExecutor)
	$TestSuiteHWD=_GUICtrlTreeView_GetFirstItem($TEHWD)
	_GUICtrlTreeView_ClickItem($TEHWD, $TestSuiteHWD, "Right")
	sleep(500)

	$TestSuiteMenus=_GetPopUpMenuText()

	send("{ESC}")

	_ExpandTestExeTreeView()																		; Expand the tree view

	ControlTreeView ($WIN_BUSMASTER,"",$TVC_TestSuite_TestExecutor,"Select","#0|#0")				; Select the test setup file
	sleep(1000)

	$TVHWD= ControlGetHandle($WIN_BUSMASTER,"",$TVC_TestSuite_TestExecutor)							; Fetch Test executor tree view handle

	$TestSetUpHWD=_GUICtrlTreeView_GetSelection($TVHWD)												; Fetch the handle of test setup file

	_GUICtrlTreeView_ClickItem($TVHWD,$TestSetUpHWD,"Right")										; Right click on test setup file
	sleep(1000)

	send("d")																						; Select "Delete" from context menu
	sleep(500)

	$TestSetUpCount=ControlTreeView ($WIN_BUSMASTER,"",$TVC_TestSuite_TestExecutor,"GetItemCount","#0")				; Fetch the child item count of test suite
	sleep(1000)

	send("^{F4}")																					; Close Test executor window
	sleep(1000)

	ConsoleWrite("$TestSuiteName:"&$TestSuiteName&@CRLF)
	ConsoleWrite("$IsTestSuiteChecked:"&$IsTestSuiteChecked&@CRLF)
	ConsoleWrite("$IsTestSetUpChecked:"&$IsTestSetUpChecked&@CRLF)
	ConsoleWrite("$IsTestCaseChecked:"&$IsTestCaseChecked&@CRLF)
	ConsoleWrite("$TestSuiteMenus[0]:"&$TestSuiteMenus[0]&@CRLF)
	ConsoleWrite("$TestSuiteMenus[1]:"&$TestSuiteMenus[1]&@CRLF)
	ConsoleWrite("$TestSuiteMenus[2]:"&$TestSuiteMenus[2]&@CRLF)
	ConsoleWrite("$TestSetUpCount:"&$TestSetUpCount&@CRLF)

EndIf

if $TestSuiteName="SampleTestSuite" and $IsTestSuiteChecked=1 and $IsTestSetUpChecked=0 and $IsTestCaseChecked=0 and $TestSuiteMenus[0]=2 and $TestSuiteMenus[1]="Add..." and $TestSuiteMenus[2]="Execute" and $TestSetUpCount=0 Then					; Validate the result
	_WriteResult("Pass","TS_TestAuto_07")
Else
	_WriteResult("Fail","TS_TestAuto_07")
EndIf

ConsoleWrite("****End : TS_TestAuto_07.au3****"&@CRLF)
ConsoleWrite(@CRLF)