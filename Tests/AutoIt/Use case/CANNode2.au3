;=== Test Cases/Test Data ===
; Critical (C):		Y
; TestCase No.:		TC_03
; Test Cases:		Node Simultion - Delete
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===

#Include <GuiTreeView.au3>
#Include <GuiListView.au3>
#include <globalFunc.au3>
#Include <GuiToolBar.au3>
#Include <GuiListBox.au3>

winactivate("BUSMASTER",3)
if WinExists("BUSMASTER") Then
	sleep(2000)
	$nodeHWD=ControlGetHandle("BUSMASTER","","[CLASS:SysTreeView32; INSTANCE:1]")	; Get tree view control handle
	_GUICtrlTreeView_Expand($nodeHWD,"",True)
	$item1HWD=_GUICtrlTreeView_GetNext($nodeHWD,"")									; Get handle of first item
	$MSG1HWD=_GUICtrlTreeView_GetFirstChild($nodeHWD,$item1HWD)
	_GUICtrlTreeView_ClickItem($nodeHWD,$MSG1HWD)
	sleep(1000)
	controlclick("BUSMASTER","Edit File...",1002)									; click edit file
	; Delete message handler child items
	$nodeHWD=ControlGetHandle("BUSMASTER","","[CLASS:SysTreeView32; INSTANCE:1]")	; Get tree view control handle
	$item1HWD=_GUICtrlTreeView_GetNext($nodeHWD,"")									; Get handle of First item
	$item2HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item1HWD)					; Get handle of second item
	$msghndlChildCount=_GUICtrlTreeView_GetChildCount($nodeHWD, $item2HWD)			; Count the no. of child items in the second node
	for $i=0 to $msghndlChildCount-1
		$item1HWD=_GUICtrlTreeView_GetNext($nodeHWD,"")									; Get handle of First item
		$item2HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item1HWD)					; Get handle of second item
		$msgHWD=_GUICtrlTreeView_GetFirstChild($nodeHWD,$item2HWD)						; Get handle of First Child item in the second node
		_GUICtrlTreeView_ClickItem($nodeHWD,$msgHWD,"Right")							; Right Click on the First Child item
		send("d")																		; Delete the First Child item
		sleep(1000)
		if winexists("BUSMASTER") Then
			send("{ENTER}")																; Click Yes
		EndIf
	Next

	;Delete Time handler child items
	$item1HWD=_GUICtrlTreeView_GetNext($nodeHWD,"")									; Get handle of First item
	$item2HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item1HWD)					; Get handle of second item
	$item3HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item2HWD)					; Get handle of third item
	$timehndlChildCount=_GUICtrlTreeView_GetChildCount($nodeHWD, $item3HWD)			; Count the no. of child items in the third node
	for $i=0 to $timehndlChildCount-1
		$item1HWD=_GUICtrlTreeView_GetNext($nodeHWD,"")									; Get handle of First item
		$item2HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item1HWD)					; Get handle of second item
		$item3HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item2HWD)					; Get handle of third item
		$timeHWD=_GUICtrlTreeView_GetFirstChild($nodeHWD,$item3HWD)						; Get handle of First Child item in the third node
		_GUICtrlTreeView_ClickItem($nodeHWD,$timeHWD,"Right")							; Right Click on the First Child item
		send("d")																		; Delete the First Child item
		sleep(1000)
		if winexists("BUSMASTER") Then
			send("{ENTER}")																; Click Yes
		EndIf
	Next

	;Delete Key handler child items
	$item1HWD=_GUICtrlTreeView_GetNext($nodeHWD,"")									; Get handle of First item
	$item2HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item1HWD)					; Get handle of second item
	$item3HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item2HWD)					; Get handle of third item
	$item4HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item3HWD)					; Get handle of Fourth item
	$keyhndlChildCount=_GUICtrlTreeView_GetChildCount($nodeHWD, $item4HWD)			; Count the no. of child items in the fourth node
	for $i=0 to $keyhndlChildCount-1
		$item1HWD=_GUICtrlTreeView_GetNext($nodeHWD,"")									; Get handle of First item
		$item2HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item1HWD)					; Get handle of second item
		$item3HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item2HWD)					; Get handle of third item
		$item4HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item3HWD)					; Get handle of Fourth item
		$keyHWD=_GUICtrlTreeView_GetFirstChild($nodeHWD,$item4HWD)						; Get handle of First Child item in the fourth node
		_GUICtrlTreeView_ClickItem($nodeHWD,$keyHWD,"Right")							; Right Click on the First Child item
		send("d")																		; Delete the First Child item
		sleep(1000)
		if winexists("BUSMASTER") Then
			send("{ENTER}")																; Click Yes
		EndIf
	Next

	;Delete Error handlers child items
	$item1HWD=_GUICtrlTreeView_GetNext($nodeHWD,"")									; Get handle of First item
	$item2HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item1HWD)					; Get handle of second item
	$item3HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item2HWD)					; Get handle of third item
	$item4HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item3HWD)					; Get handle of Fourth item
	$item5HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item4HWD)					; Get handle of Fifth item
	$errhndlChildCount=_GUICtrlTreeView_GetChildCount($nodeHWD, $item5HWD)			; Count the no. of child items in the Fifth node
	for $i=0 to $errhndlChildCount-1
		$item1HWD=_GUICtrlTreeView_GetNext($nodeHWD,"")									; Get handle of First item
		$item2HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item1HWD)					; Get handle of second item
		$item3HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item2HWD)					; Get handle of third item
		$item4HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item3HWD)					; Get handle of Fourth item
		$item5HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item4HWD)					; Get handle of Fifth item
		$errHWD=_GUICtrlTreeView_GetFirstChild($nodeHWD,$item5HWD)						; Get handle of First Child item in the Fifth node
		_GUICtrlTreeView_ClickItem($nodeHWD,$errHWD,"Right")							; Right Click on the First Child item
		send("d")																		; Delete the First Child item
		sleep(1000)
		if winexists("BUSMASTER") Then
			send("{ENTER}")																; Click Yes
		EndIf
	Next

	;Delete DLL handlers child items
	$item1HWD=_GUICtrlTreeView_GetNext($nodeHWD,"")									; Get handle of First item
	$item2HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item1HWD)					; Get handle of second item
	$item3HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item2HWD)					; Get handle of third item
	$item4HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item3HWD)					; Get handle of Fourth item
	$item5HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item4HWD)					; Get handle of Fifth item
	$item6HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item5HWD)					; Get handle of Sixth item
	$dllhndlChildCount=_GUICtrlTreeView_GetChildCount($nodeHWD, $item6HWD)			; Count the no. of child items in the Sixth node
	for $i=0 to $dllhndlChildCount-1
		$item1HWD=_GUICtrlTreeView_GetNext($nodeHWD,"")									; Get handle of First item
		$item2HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item1HWD)					; Get handle of second item
		$item3HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item2HWD)					; Get handle of third item
		$item4HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item3HWD)					; Get handle of Fourth item
		$item5HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item4HWD)					; Get handle of Fifth item
		$item6HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item5HWD)					; Get handle of Sixth item
		$dllHWD=_GUICtrlTreeView_GetFirstChild($nodeHWD,$item6HWD)						; Get handle of First Child item in the Sixth node
		_GUICtrlTreeView_ClickItem($nodeHWD,$dllHWD,"Right")							; Right Click on the First Child item
		send("d")																		; Delete the First Child item
		sleep(1000)
		if winexists("BUSMASTER") Then
			send("{ENTER}")																; Click Yes
		EndIf
		sleep(1000)
	Next

	;Delete Utility handlers child items
	$item1HWD=_GUICtrlTreeView_GetNext($nodeHWD,"")									; Get handle of First item
	$item2HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item1HWD)					; Get handle of second item
	$item3HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item2HWD)					; Get handle of third item
	$item4HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item3HWD)					; Get handle of Fourth item
	$item5HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item4HWD)					; Get handle of Fifth item
	$item6HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item5HWD)					; Get handle of Sixth item
	$item7HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item6HWD)					; Get handle of Seventh item
	$utilhndlChildCount=_GUICtrlTreeView_GetChildCount($nodeHWD, $item7HWD)			; Count the no. of child items in the Seventh node
	for $i=0 to $utilhndlChildCount-1
		$item1HWD=_GUICtrlTreeView_GetNext($nodeHWD,"")									; Get handle of First item
		$item2HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item1HWD)					; Get handle of second item
		$item3HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item2HWD)					; Get handle of third item
		$item4HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item3HWD)					; Get handle of Fourth item
		$item5HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item4HWD)					; Get handle of Fifth item
		$item6HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item5HWD)					; Get handle of Sixth item
		$item7HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item6HWD)					; Get handle of Seventh item
		$utilHWD=_GUICtrlTreeView_GetFirstChild($nodeHWD,$item7HWD)						; Get handle of First Child item in the Seventh node
		_GUICtrlTreeView_ClickItem($nodeHWD,$utilHWD,"Right")							; Right Click on the First Child item
		send("d")																		; Delete the First Child item
		sleep(1000)
		if winexists("BUSMASTER") Then
			send("{ENTER}")																; Click Yes
		EndIf
	Next

	$item1HWD=_GUICtrlTreeView_GetNext($nodeHWD,"")									; Get handle of First item
	$item2HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item1HWD)					; Get handle of second item
	$item3HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item2HWD)					; Get handle of third item
	$item4HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item3HWD)					; Get handle of Fourth item
	$item5HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item4HWD)					; Get handle of Fifth item
	$item6HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item5HWD)					; Get handle of Sixth item
	$item7HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item6HWD)
	$item8HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item7HWD)
	$utilhwdnewchildcount=_GUICtrlTreeView_GetChildCount($nodeHWD,$item8HWD)
	for $i=0 to $utilhwdnewchildcount-1
		$item1HWD=_GUICtrlTreeView_GetNext($nodeHWD,"")									; Get handle of First item
		$item2HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item1HWD)					; Get handle of second item
		$item3HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item2HWD)					; Get handle of third item
		$item4HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item3HWD)					; Get handle of Fourth item
		$item5HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item4HWD)					; Get handle of Fifth item
		$item6HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item5HWD)					; Get handle of Sixth item
		$item7HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item6HWD)					; Get handle of Seventh item
		$item8HWD=_GUICtrlTreeView_GetNextSibling($nodeHWD,$item7HWD)
		$util1HWD=_GUICtrlTreeView_GetFirstChild($nodeHWD,$item8HWD)
		_GUICtrlTreeView_ClickItem($nodeHWD,$util1HWD,"Right")							; Right Click on the First Child item
		send("d")																		; Delete the First Child item
		sleep(1000)
		if winexists("BUSMASTER") Then
			send("{ENTER}")																; Click Yes
		EndIf
	Next
	send("^s")
	if winexists("BUSMASTER") Then
		send("{ENTER}")																; Click Yes
	EndIf
	send("^{F4}")
;~ 	; Edit node and delete simulated systems
	if winexists("BUSMASTER") Then
		send("{ENTER}")																; Click Yes
	EndIf
	sleep(2000)
	WinActivate("BUSMASTER")
	$nodeHWD=ControlGetHandle("BUSMASTER","","[CLASS:SysTreeView32; INSTANCE:1]")	; Get tree view control handle
	$item1HWD=_GUICtrlTreeView_GetNext($nodeHWD,"")									; Get handle of first item
	$MSG1HWD=_GUICtrlTreeView_GetFirstChild($nodeHWD,$item1HWD)
	_GUICtrlTreeView_ClickItem($nodeHWD,$MSG1HWD,"Right","",2)					    ; Click on First Header
	sleep(3000)
	send("e")																		; select 'Edit Node'
	send("{ENTER}")																	; Press Enter
	sleep(500)
	if WinExists("BUSMASTER") Then
		ControlClick("BUSMASTER","",2)												; Click 'Enter' in Unload DLL error pop up msg
	EndIf
	ControlClick("BUSMASTER","",1005)												; Click 'Unload' button
	sleep(1000)
	$item1HWD=_GUICtrlTreeView_GetNext($nodeHWD,"")									; Get handle of first item
	$MSG1HWD=_GUICtrlTreeView_GetFirstChild($nodeHWD,$item1HWD)
	_GUICtrlTreeView_ClickItem($nodeHWD,$MSG1HWD,"Right")					    	; Click on First Header
	sleep(1000)
	send("e")																		; select 'Edit Node'
	sleep(3000)
	if winexists("Node Details") Then
		ControlSetText("Node Details","",1278,"Edittestnode")						; Change the node name
		ControlClick("Node Details","",1)											; Click 'OK'
	EndIf
	$item1HWD=_GUICtrlTreeView_GetNext($nodeHWD,"")
	_GUICtrlTreeView_ClickItem($nodeHWD,$item1HWD,"Right")							; Right click on sim file
	sleep(1000)
	send("ss")																		; Select save as from the context menu
	send("{ENTER}")
	sleep(1000)
	if WinExists("SaveAs Configuration Filename...") Then
		controlsettext("SaveAs Configuration Filename...","",1152,"testsaveastest")	; Enter file name in SaveAs Configuration Filename dialog
		send("{ENTER}")																; Click Save
	EndIf
	$item1HWD=_GUICtrlTreeView_GetNext($nodeHWD,"")
	_GUICtrlTreeView_ClickItem($nodeHWD,$item1HWD,"Right")							; Right click on sim file
	sleep(3000)
	send("r")																		; Select 'Remove Node' from menu
	if WinExists("BUSMASTER") Then
		ControlClick("BUSMASTER","",7)												; Click 'No' in pop up msg
	EndIf
	$item1HWD=_GUICtrlTreeView_GetNext($nodeHWD,"")
	_GUICtrlTreeView_ClickItem($nodeHWD,$item1HWD,"Right")							; Right click on sim file
	sleep(3000)
	send("r")																		; Select 'Remove Node' from menu
	if WinExists("BUSMASTER") Then
		ControlClick("BUSMASTER","",6)												; Click 'Yes' in pop up msg
	EndIf
	$item1HWD=_GUICtrlTreeView_GetNext($nodeHWD,"")
	_GUICtrlTreeView_ClickItem($nodeHWD,$item1HWD,"Right")							; Right click on sim file
	sleep(3000)
	send("d")																		; Select 'Delete Simulated System' from menu
	if WinExists("BUSMASTER") Then
		ControlClick("BUSMASTER","",7)												; Click 'No' in pop up msg
	EndIf
	$item1HWD=_GUICtrlTreeView_GetNext($nodeHWD,"")
	_GUICtrlTreeView_ClickItem($nodeHWD,$item1HWD,"Right")							; Right click on sim file
	sleep(3000)
	send("d")																		; Select 'Remove Node' from menu
	sleep(2000)
	if WinExists("BUSMASTER") Then
		ControlClick("BUSMASTER","",6)												; Click 'Yes' in pop up msg
	EndIf
;~ 	;sleep(5000)
;~ 	send("!FGS")																; Save Config
EndIf


