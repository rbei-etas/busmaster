;=== Test Cases/Test Data ===
; UseCase 9:		J1939-Command Address
; Critical (C):		Y
; TestCase No.:		TC_UC10_04
; TestCases:		Transmit Window
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===

ConsoleWrite("****Start : TC_UC10_04.au3****"&@CRLF)
Dim $BAM, $TPDT1, $TPDT2, $BRDCST, $BAMDB, $TPDT1DB, $TPDT2DB, $BRDCSTDB, $r1, $r2, $r3, $r4
if winexists("BUSMASTER") Then
	WinMenuSelectItem("BUSMASTER","","&J1939","&View","&Transmit Window")		; Select J1939-->View-->Transmit Window
	ControlCommand("J1939 Transmit Message Window","",1335,"Check")				; Check 'Network Management' Radio Button
	sleep(500)
	ControlCommand("J1939 Transmit Message Window","",1009,"Check")				; Check 'Command Address' Radio Button
	ControlSetText("J1939 Transmit Message Window","",1336,"AA")
	;_overWriteMode()															; Call _overWriteMode function to disable overWriteMode
	WinMenuSelectItem("BUSMASTER","","F&unctions","&Connect")					; Select 'Connect' button
	sleep(1000)
	ControlClick("J1939 Transmit Message Window","",1011,"left")				; Click 'transmit' button
	sleep(3000)
	Send("{TAB}")
	Send("{ENTER}")
	$hWnd=ControlGetHandle("BUSMASTER","","SysListView321")
	$rCount=_GUICtrlListView_GetItemCount($hWnd)
	ConsoleWrite("$rCount : "&$rCount & @CRLF)
	if $rCount=4 then
		$r1=_GUICtrlListView_GetItemTextString($hWnd,0)							; fetch the row entries
		$r2=_GUICtrlListView_GetItemTextString($hWnd,1)
		$r3=_GUICtrlListView_GetItemTextString($hWnd,2)
		$r4=_GUICtrlListView_GetItemTextString($hWnd,3)
		$hWd=ControlGetHandle("BUSMASTER","",128)
		_GUICtrlToolbar_ClickIndex($hWd,4,"left")
		sleep(1000)
	EndIf
	$hWd=ControlGetHandle("BUSMASTER","",128)
	_GUICtrlToolbar_ClickIndex($hWd,4,"left")
	sleep(1000)
	;ControlClick("J1939 Transmit Message Window","",1001,"left")
EndIf
ConsoleWrite("$r1 : "&$r1 & @CRLF)
ConsoleWrite("$r2 : "&$r2 & @CRLF)
ConsoleWrite("$r3 : "&$r3 & @CRLF)
ConsoleWrite("$r4 : "&$r4 & @CRLF)
$BAM=StringSplit($r1,"|")														; Split the row entries into columns
$TPDT1=StringSplit($r2,"|")
$TPDT2=StringSplit($r3,"|")
$BRDCST=StringSplit($r4,"|")
$BAMDB=StringSplit($BAM[13]," ")
$TPDT1DB=StringSplit($TPDT1[13]," ")
$TPDT2DB=StringSplit($TPDT2[13]," ")
$BRDCSTDB=StringSplit($BRDCST[13]," ")
if ($BAM[3] and $TPDT1[3] and $TPDT2[3]=1) and ($BAM[8] and $TPDT1[8] and $TPDT2[8]=170) and ($BAM[11] and $TPDT1[11] and $TPDT2[11]="Tx") Then
	$match=1
Else
	$match=0
EndIf
ConsoleWrite("$BAM[7] : "& $BAM[7] & @CRLF)
ConsoleWrite("$TPDT1[7]: "& $TPDT1[7] & @CRLF)
ConsoleWrite("$TPDT2[7] : "& $TPDT2[7] & @CRLF)
ConsoleWrite("$BRDCST[7] : "& $BRDCST[7] & @CRLF)
if $BAM[7]="BAM" and ($TPDT1[7] and $TPDT2[7]="TPDT") and $BRDCST[7]="BROADCAST" Then
	$type=1
Else
	$type=0
EndIf
ConsoleWrite("$BAMDB[0] : "& $BAMDB[0] & @CRLF)
ConsoleWrite("$BAMDB[1] : "& $BAMDB[1] & @CRLF)
ConsoleWrite("$BAMDB[3] : "& $BAMDB[3] & @CRLF)
ConsoleWrite("$TPDT1DB[0]: "& $TPDT1DB[0] & @CRLF)
ConsoleWrite("$TPDT2DB[0]: "& $TPDT2DB[0] & @CRLF)
if $BAMDB[1]=032 and $BAMDB[2]=009 and $BAMDB[4]=002 and $TPDT1DB[1]=001 and $TPDT2DB[1]=002 Then
	$db=1
Else
	$db=0
EndIf
ConsoleWrite("$match: "&$match & @CRLF)
ConsoleWrite("$type: "&$type & @CRLF)
ConsoleWrite("$db: "&$db & @CRLF)
if $match=1 and $type=1 and $db=1 Then
	_ExcelWriteCell($oExcel, "Pass", 52, 10)
Else
	_ExcelWriteCell($oExcel, "Fail", 52, 10)
EndIf
ConsoleWrite("****End : TC_UC10_04.au3****"&@CRLF)
ConsoleWrite("***********UseCase 10 Script Execution Started************"&@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)



