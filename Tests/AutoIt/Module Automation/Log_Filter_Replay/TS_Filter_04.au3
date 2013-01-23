;=== Test Cases/Test Data ===
; Module			:		Filters
; Critical (C)		:		Y
; TestCase No.		:		TS_Filter_04
; TestCases			:		Check for GUI - Message Filter
; Test Strategy		:		Black Box
; Test Data			:		-
; === Test Procedure ===

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_Filter_04.au3****"&@CRLF)

_launchApp()

if winexists($mWin) then
	_createConfig("cfxFilter_04")													; Create New Configuration

	_AppFilterMenu()																; Open Configure Filter dialog

	ControlClick($dlgConfigfilter,"",$addFilterBTNctrlID)							; Click on 'Add' filter button
	sleep(1000)

	$hWnd = ControlGetHandle($dlgConfigfilter,"",$filterList)
	$FilterTypeTxt1=_GUICtrlListView_GetItemText($hWnd,0,1)							; Fetch Stop filter type Text
	ConsoleWrite(@CRLF & $FilterTypeTxt1 & @CRLF)
	$FilTypePos=_GUICtrlListView_GetItemPosition($hWnd, 0)							; Get position of Signal details list view control
	ControlClick($dlgConfigfilter,"",1000,"Left",2,$FilTypePos[0]+100,$FilTypePos[1])	; Double click on the 'Filter Type'
	sleep(500)
	send("{DOWN}")
	sleep(500)
	send("{ENTER}")																	; Select Pass filter
	sleep(500)
	$FilterTypeTxt2=_GUICtrlListView_GetItemText($hWnd,0,1)							; Fetch Pass filter type text
	ConsoleWrite(@CRLF & $FilterTypeTxt2 & @CRLF)
	ControlClick($dlgConfigfilter,"",$OKFilterBTNctrlID)							; Click on OK button
	sleep(500)
EndIf

if $FilterTypeTxt1="Stop" and  $FilterTypeTxt2="Pass" Then							; Validate the result
	_ExcelWriteCell($oExcel, "Pass", 9, 2)
Else
	_ExcelWriteCell($oExcel, "Fail", 9, 2)
EndIf

ConsoleWrite("****End : TS_Filter_04.au3****"&@CRLF)
ConsoleWrite(@CRLF)
