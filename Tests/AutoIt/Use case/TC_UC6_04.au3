;=== Test Cases/Test Data ===
; UseCase 1:		transmit J1939 Message Transmission - Broadcast
; Critical (C):		Y
; TestCase No.:		TC_UC6_04
; TestCases:		Transmit Window
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===

ConsoleWrite("****Start : TC_UC6_04.au3****"&@CRLF)
Dim $info[1000],$row[1000],$type[1000],$time[1000],$TxRx[1000],	$dataByte[1000], $fByte[1000],$sByte[1000], $tByte[1000],$DLC[1000]
local $chkDB=0
local $match=0
_launchApp()
if $app=0 Then
	_loadConfig("UseCase6")
	_loadJ1939Database("J1939BrdCast",255,1024,64)
	_associateDB("&J1939","J1939BrdCast.dbf")
EndIf
_J1939tMsgWin("Broadcast")
if winexists("BUSMASTER") Then
	sleep(2000)
	$hWnd=ControlGetHandle ("BUSMASTER","","SysListView321")
	$rCount=_GUICtrlListView_GetItemCount($hWnd)
	ConsoleWrite("$rCount : "&$rCount & @CRLF)
	if $rCount=9 then
		for $i=0 to $rCount-1
			$row[$i]=_GUICtrlListView_GetItemTextString($hWnd,$i)
			ConsoleWrite("$row"&"["&$i&"] : "&$row[$i] & @CRLF)
			$info=StringSplit($row[$i],"|")
			ConsoleWrite("info : "&$info[7] & @CRLF)
			$time[$i]=$info[2]
			ConsoleWrite("$time : "&$time[$i] & @CRLF)
			$type[$i]=$info[7]
			ConsoleWrite("$type : "&$type[$i] & @CRLF)
			$TxRx[$i]=$info[11]
			ConsoleWrite("$TxRx: "&$TxRx[$i] & @CRLF)
			$DLC[$i]=$info[12]
			ConsoleWrite("$DLC: "&$DLC[$i] & @CRLF)
			$dataByte[$i]=$info[13]
			ConsoleWrite("$dataByte: "&$dataByte[$i] & @CRLF)
			$byte=StringSplit($dataByte[$i]," ")
			$fByte[$i]=$byte[1]
			ConsoleWrite("$fbyte : "&$fByte[$i] & @CRLF)
			$sByte[$i]=$byte[2]
			ConsoleWrite("$sbyte : "&$sByte[$i] & @CRLF)
			$tByte[$i]=$byte[4]
			ConsoleWrite("$tbyte : "&$tByte[$i] & @CRLF)
		next
		if $type[0]="BAM" and ($type[2] and $type[4]="TPDT") and ($TxRx[0] and $TxRx[2] and $TxRx[4]="Tx") and ($TxRx[1] and $TxRx[3] and $TxRx[5]="Rx") then
			for $i=0 to 5 step 2
				ConsoleWrite("$type"&"["&$i&"]  : "&$type[$i] & @CRLF)
				ConsoleWrite("$type"&"["&$i+1&"]  : "&$type[$i+1] & @CRLF)
				ConsoleWrite("$time"&"["&$i&"]  : "&$time[$i] & @CRLF)
				ConsoleWrite("$time"&"["&$i+2&"]  : "&$time[$i+2] & @CRLF)
				ConsoleWrite("$TxRx"&"["&$i&"]  : "&$TxRx[$i] & @CRLF)
				ConsoleWrite("$TxRx"&"["&$i+2&"]  : "&$TxRx[$i+2] & @CRLF)
				ConsoleWrite("$TxRx"&"["&$i+1&"]  : "&$TxRx[$i+1] & @CRLF)
				ConsoleWrite("$TxRx"&"["&$i+3&"]  : "&$TxRx[$i+3] & @CRLF)
				if ($type[$i]=$type[$i+1]) and ($time[$i]<$time[$i+2]) Then
					$match=1
				Else
					$match=0
				EndIf
				if $match=0 Then
					ExitLoop
				EndIf
			Next
		EndIf
		ConsoleWrite("$match : "&$match & @CRLF)
		if ($fByte[0] and $fByte[1]=032) and  ($sByte[0] and $sByte[1]=021) and ($tByte[0] and $tByte[1]=003) and ($fByte[2]=001 and $fByte[4]=002 and $fByte[6]=003) then
			$chkDB=1
		EndIf
	EndIf
EndIf
$hWd=ControlGetHandle("BUSMASTER","",128)
_GUICtrlToolbar_ClickIndex($hWd,4,"left")
sleep(1000)

ConsoleWrite("****End : TC_UC6_04.au3****"&@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
