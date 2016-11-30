;==================================================Test Cases/Test Data ========================================
; Critical (C)		:		Y
; TestCase No.		:		TS_NS_J1939_Func_001
; TestCases			:		Transmit window enabling and Cyclic option in Transmit Window
; Test Data			:		cfxNSJ1939_12.cfx
; Test Setup		:		1. Make sure BUSMASTER is in connect state.
;                           2. Activate J1939 using menu option "J1939 -> Activate".
;                           3. Go online using menu option "J1939 -> Function -> Go Online".
;                           4. Open J1939 Tx window using menu option "J1939 -> View -> Transmit Window".


; Expected Results  : 		1. After Step 4, "J1939 Transmit Message Window" should be shown.
;                           2. The "Cyclic" check box and the edit box next to it should be in disabled state.
;
;==================================================Test Procedure =======================================================
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_NS_J1939_Func_002.au3****"&@CRLF)


_launchApp()
WinActivate($WIN_BUSMASTER)
Local $ReqstPGNResult=0,$DataResult=0,$BroadCastResult=0,$ClaimAddResult=0,$ReqAddResult=0,$CommAddResult=0

if winexists($WIN_BUSMASTER) Then

	_loadConfig("cfx_J1939Func_01.cfx")																								; Load cfx
	Sleep(1000)

	_ActivateJ1939()                                                                                                                ;Activate J1939 Message window

	_AssocJ1939DB("J1939DB.dbf")                                                                                                    ;Associate DB
    Sleep(1000)

	_ConnectDisconnect()																											; Connect

	_ClearMsgWindowJ1939()																												; Clear msg window

	_J1939tMsgWin("Request PGN","[0x0]Msg_Rqst","","")																				; Transmit request PGN msg
	$rCount=_GetJ1939MsgWinItemCount()																								; Fetch msg wind item count
	ConsoleWrite("$rCount : "&$rCount&@CRLF)

	If($rCount=1) Then
		$ReqstData1=_GetJ1939MsgWinInfo(0)                                                                                          ; Fetch the column entries of message window

		for $i=0 to 11                                                                                                              ; Print all the values
			ConsoleWrite("$RequestData1 : "&$ReqstData1[$i]&@CRLF)
		next

		if $ReqstData1[5]="RQST" and $ReqstData1[8]=006 and $ReqstData1[10]=3 Then
			$ReqstPGNResult=1
			ConsoleWrite("Request PGN is transmitted")
		Else
			$ReqstPGNResult=0
			ConsoleWrite("Request PGN is transmitted is not transmitted")
		EndIf
	EndIf

	_J1939tMsgWin("DATA","[0x400]Msg_Data","","")  	                                                                                  ;Transmit Data
	sleep(2000)
    $rCount1=_GetJ1939MsgWinItemCount()
	ConsoleWrite("$rCount1 : "&$rCount1&@CRLF)
	If $rCount1=2 Then
		$Data1=_GetJ1939MsgWinInfo(1)                                                                                                 ;Fetch the column entries of message window

		for $idata=0 to 11
			ConsoleWrite("$Data1 : "&$Data1[$idata]&@CRLF)                                                                             ;Print all the values
		next
		ConsoleWrite("$Data1[5]" & $Data1[5] & @CRLF)

		if $Data1[5]="DATA" and $Data1[8]=007 and $Data1[10]=8 Then
			$DataResult=1
			ConsoleWrite("DATA is transmitted")
		Else
			$DataResult=0
			ConsoleWrite("DATA is not transmitted")
		EndIf
	EndIf

	_J1939tMsgWin("Broadcast","[0xfeff]Msg_Broadcast","","")                                                                           ;Transmit Broadcast message
	sleep(2000)
	$rCount2=_GetJ1939MsgWinItemCount()
	ConsoleWrite("$rCount2 : "&$rCount2&@CRLF)
	If $rCount2=6 Then
		$BroadcastData1=_GetJ1939MsgWinInfo(2)                                                                                 		 	;Fetch all the column entries
		$BroadcastData2=_GetJ1939MsgWinInfo(3)
		$BroadcastData3=_GetJ1939MsgWinInfo(4)
		$BroadcastData4=_GetJ1939MsgWinInfo(5)

		For $iBroad=0 to 11
			ConsoleWrite("$BroadcastData1 : "&$BroadcastData1[$iBroad]&@CRLF)                                                  			;Print all the values
			ConsoleWrite("$BroadcastData2 : " &$BroadcastData2[$iBroad] & @CRLF)
			ConsoleWrite("$BroadcastData3 : "&$BroadcastData3[$iBroad]&@CRLF)
			ConsoleWrite("$BroadcastData4 : " &$BroadcastData4[$iBroad] & @CRLF)
		Next

		If $BroadcastData1[5]="BAM" and $BroadcastData2[5] ="TPDT" and $BroadcastData3[5]="TPDT" and $BroadcastData4[5] = "BROADCAST" Then
			$Type=1
		else
			$Type=0
		EndIf

		if $BroadcastData1[10]=8 and $BroadcastData2[10]=8 and $BroadcastData3[10]=8 and $BroadcastData4[10]=9 Then
			$DLC=1
		Else
			$DLC=0
		EndIf

		if $BroadcastData1[7]="FF " Then
			$DestAddBAM=1
		Else
			$DestAddBAM=0
		EndIf

		if $BroadcastData2[7]="00 " and $BroadcastData3[7]="00 " and $BroadcastData4[7]="00 " Then
			$DestBroadcastAddMsg=1
		Else
			$DestBroadcastAddMsg=0
		EndIf

		If $BroadcastData1[11]="20090002FF00FE00" Then
			$BAMData=1
		Else
			$BAMData=0
		EndIf

		If $BroadcastData2[11]="0110000000000000" Then
			$TPDT_Data1=1
		Else
			$TPDT_Data1=0
		EndIf

		If $BroadcastData3[11]="020000FFFFFFFFFF" Then
			$TPDT_Data2=1
		Else
			$TPDT_Data2=0
		EndIf

		If $BroadcastData4[11]= "100000000000000000" Then
			$Broadcast_Data=1
		Else
			$Broadcast_Data=0
		EndIf

		ConsoleWrite("BAMData: " &$BAMData & @CRLF)
		ConsoleWrite("TPDT Data:" &$TPDT_Data1 & "" & $TPDT_Data2)
		ConsoleWrite("Broadcast:" &$Broadcast_Data & @CRLF)
		ConsoleWrite("BAM address:" &$DestAddBAM & @CRLF)
		ConsoleWrite("Broadcast address " & $DestBroadcastAddMsg & @CRLF)


		if ($Type=1 And $DLC=1  and $BAMData=1 and $Broadcast_Data=1 and $TPDT_Data1=1 and $TPDT_Data2=1 and $DestAddBAM=1 and $DestBroadcastAddMsg=1) Then
			$BroadCastResult=1
			ConsoleWrite("BROADCAST message is transmitted")
		else
			$BroadCastResult=0
			ConsoleWrite("BROADCAST message is Not transmitted")
		EndIf
	EndIf

	_J1939tMsgWin("Claim Address","","","")                                                                                             ;Transmit Claim address
	sleep(2000)

	$rCount3=_GetJ1939MsgWinItemCount()
	ConsoleWrite("$rCount3 : "&$rCount3&@CRLF)

	If $rCount3=7 Then
		$ClaimAdd1=_GetJ1939MsgWinInfo(6)                                                                                  			;Fetch all the column entries

		For $iClaim=0 to 11
			ConsoleWrite("$ClaimAdd1 : "&$ClaimAdd1[$iClaim]&@CRLF)                                                       				;Print all the values
		Next

		If $ClaimAdd1[5]="ACL" Then
			$TypeClaim=1
		Else
			$TypeClaim=0
		EndIf

		If $ClaimAdd1[6]="FE " Then
			$SrcAddClaim=1
		Else
			$SrcAddClaim=0
		EndIf

		If $ClaimAdd1[7]="FF " Then
			$DestAddClaim=1
		Else
			$DestAddClaim=0
		EndIf

		If $ClaimAdd1[11]="0100000000000080" Then
			$ClaimAddData=1
		Else
			$ClaimAddData=0
		EndIf

		ConsoleWrite("$type" & $TypeClaim & @CRLF)
		ConsoleWrite("Src " & $SrcAddClaim & @CRLF)
		ConsoleWrite("Dest " & $DestAddClaim & @CRLF)
		ConsoleWrite("Data :" & $ClaimAddData & @CRLF)

		IF ($TypeClaim=1 and $SrcAddClaim=1 and $DestAddClaim=1 and $ClaimAddData=1) Then
			$ClaimAddResult=1
			ConsoleWrite("Claim address transmitted !! ")
		Else
			$ClaimAddResult=0
			ConsoleWrite("Claim address is not transmitted!!")
		EndIf
	EndIf


	_J1939tMsgWin("Request Address","","","")                                                                                              ;Transmit Request address
	sleep(2000)

	$rCount2=_GetJ1939MsgWinItemCount()
	ConsoleWrite("$rCount2 : "&$rCount2&@CRLF)

	If $rCount2=8 Then
		$ReqAdd1=_GetJ1939MsgWinInfo(7)                                                                                      ;Fetch column entries

		For $iReqAdd=0 to 11
			ConsoleWrite("$ReqAdd1 : "&$ReqAdd1[$iReqAdd]&@CRLF)                                                              ;Print all the values
		next

		if $ReqAdd1[5]="RQST_ACL" Then
			$ReqAddType=1
		Else
			$ReqAddType=0
		EndIf

		ConsoleWrite("$ReqAddType" & $ReqAddType & @CRLF)

		If $ReqAdd1[6] ="FE "Then
	        $ReqAddSrc=1
		Else
			$ReqAddSrc=0
		EndIf

		ConsoleWrite("$ReqAddSrc" & $ReqAddSrc & @CRLF)

		If $ReqAdd1[7] ="FF " Then
			$ReqAddDest=1
		Else
			$ReqAddDest=0
		EndIf

		ConsoleWrite("$ReqAddDest" & $ReqAddDest & @CRLF)

        If $ReqAdd1[10] =3 Then
			$ReqAddDLC=1
		Else
	         $ReqAddDLC=0
		EndIf

		ConsoleWrite("$ReqAddDLC" & $ReqAddDLC & @CRLF)

	    If $ReqAdd1[11] ="00EE00" Then
			$ReqAddData=1
		Else
			$ReqAddData=0
		EndIf

		ConsoleWrite("$ReqAddData" & $ReqAddData & @CRLF)

		If($ReqAddType =1 and $ReqAddSrc=1 and $ReqAddDest=1 and $ReqAddDLC=1 and $ReqAddData=1) Then
			$ReqAddResult=1
			ConsoleWrite("Req Address transmitted!!" & @CRLF)
		Else
			$ReqAddResult=0
			ConsoleWrite("Req Address is not transmitted!!" & @CRLF)
		EndIf

	EndIf

    _J1939tMsgWin("Command Address","","","")                                                                                                  ;Transmit Command address
	sleep(2000)

	_ConnectDisconnect()																											; Disconnect

	$rCount4=_GetJ1939MsgWinItemCount()
	ConsoleWrite("$rCount4 : "&$rCount4&@CRLF)

	If $rCount4=12 Then
		$CommandAdd1=_GetJ1939MsgWinInfo(8)                                                                                      ;Fetch all the column entries
		$CommandAdd2=_GetJ1939MsgWinInfo(9)
		$CommandAdd3=_GetJ1939MsgWinInfo(10)
		$CommandAdd4=_GetJ1939MsgWinInfo(11)

		For $iCommAdd=0 to 11

			ConsoleWrite("$CommandAdd1 : "&$CommandAdd1[$iCommAdd]&@CRLF)                                                          ;Print all the values

			ConsoleWrite("$CommandAdd2 : " &$CommandAdd2[$iCommAdd] & @CRLF)

			ConsoleWrite("$CommandAdd3 : " &$CommandAdd3[$iCommAdd] & @CRLF)

			ConsoleWrite("$CommandAdd4 : " &$CommandAdd4[$iCommAdd] & @CRLF)

		next

		If $CommandAdd1[5]="BAM" Then
			$CommAddBAM=1
		Else
			$CommAddBAM=0
		EndIf
		ConsoleWrite("$CommAddBAM " & $CommAddBAM & @CRLF)

		If $CommandAdd2[5]="TPDT" and $CommandAdd3[5]="TPDT" Then
			$CommAddTPDT=1
		Else
			$CommAddTPDT=0
		EndIf

		ConsoleWrite("$CommandAdd3[5] : " & $CommandAdd3[5] & @CRLF)
		ConsoleWrite("$CommandAdd4[5] : " & $CommandAdd4[5] & @CRLF)
		ConsoleWrite("$CommAddTPDT : " & $CommAddTPDT & @CRLF)

		If $CommandAdd4[5]="BROADCAST" Then
			$CommAddBroad=1
		Else
			$CommAddBroad=0
		EndIf
		ConsoleWrite("$CommAddBroad " & $CommAddBroad & @CRLF)

		If $CommandAdd1[6]="AA " and $CommandAdd2[6]="AA " and $CommandAdd3[6]="AA " and $CommandAdd4[6]="AA " Then
			$CommAddSrc=1
		Else
			$CommAddSrc=0
		EndIf

		ConsoleWrite("$CommandAdd1[6] : " & $CommandAdd1[6] & @CRLF)
		ConsoleWrite("$CommandAdd2[6] : " & $CommandAdd2[6] & @CRLF)
		ConsoleWrite("$CommandAdd3[6] : " & $CommandAdd3[6] & @CRLF)
		ConsoleWrite("$CommandAdd4[6] : " & $CommandAdd4[6] & @CRLF)
		ConsoleWrite("$CommAddSrc : " & $CommAddSrc & @CRLF)

		If $CommandAdd1[7]="FF " and $CommandAdd2[7]="FF " and $CommandAdd3[7]="FF " and $CommandAdd4[7]="00 " Then
			$CommAddDest=1
		Else
			$CommAddDest=0
		EndIf

		ConsoleWrite("$CommAddDest " & $CommAddDest & @CRLF)

		If $CommandAdd1[10]=8 and $CommandAdd2[10]=8 and $CommandAdd3[10]=8 and $CommandAdd4[10]=9 Then
			$CommAddDLC=1
		Else
			$CommAddDLC=0
		EndIf

		ConsoleWrite("$CommAddDLC " & $CommAddDLC & @CRLF)
		ConsoleWrite("$CommandAdd1[7]" & $CommandAdd1[7])
		ConsoleWrite("$CommandAdd4[7]" & $CommandAdd4[7])

		If $CommandAdd1[11]="20090002FF00EE00" and $CommandAdd2[11]="0101000000000000" and $CommandAdd3[11]="0280AAFFFFFFFFFF" and $CommandAdd4[11]="0100000000000080AA" Then
			$CommAddData=1
		Else
			$CommAddData=0
		EndIf
		ConsoleWrite("$CommAddData " & $CommAddData & @CRLF)

		If($CommAddBAM=1 and $CommAddTPDT=1 and $CommAddBroad=1 and $CommAddSrc=1 and $CommAddDest=1 and $CommAddDLC=1 and $CommAddData=1) Then
			$CommAddResult=1
			ConsoleWrite("Command address is transmitted!!")
		Else
			$CommAddResult=0
			ConsoleWrite("Command address is not transmitted")
		EndIf
	EndIf

	ConsoleWrite("Request PGN :" &$ReqstPGNResult & @CRLF & "DATA " & $DataResult & @CRLF & "Broadcast " & $BroadCastResult & @CRLF & "Claim add " & $ClaimAddResult & @CRLF & "Req add " &$ReqAddResult&@CRLF& "Comm add " & $CommAddResult)

	_DeactJ1939()																													; Deactivate J1939

	If($ReqstPGNResult=1 And $DataResult=1 And $BroadCastResult=1 And $ReqAddResult=1 And $ClaimAddResult=1 And $CommAddResult=1) Then
		ConsoleWrite("Pass!!" & @CRLF)
		_WriteResult("Pass","STS_J1939_002")
	Else
		_WriteResult("Fail","STS_J1939_002")
		ConsoleWrite("Fail!!" & @CRLF)
	EndIf
EndIf

$isAppNotRes=_CloseApp()																			; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","STS_J1939_002")
EndIf

ConsoleWrite("****End : TS_NS_J1939_Func_002****"&@CRLF)
ConsoleWrite(@CRLF)

