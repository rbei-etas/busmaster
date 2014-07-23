;==================================================Test Cases/Test Data =============================================
; Critical (C)		:		Y
; TestCase No.		:		TS_TxWin_22
; TestCases			:		Value assignement for the database messages
; Test Data			:		TS_TxWin_01.cfx
; Test Setup		:		1. Select any database message from the combo box.
;                           2. Select any signal and  double click the RAW value or Physical value filed.
;                           3. Change the value in the spin control and click on outside of the list control.
;                           4. observe the values in physical filed and data byte filed

; Expected Results  : 		1. After step 2 a spin control will appear in the selected field.
;                           2.After step 3 the physical value of the signal will change according to the equation defined in database.
 ;                          The data byte filed will change according to the RAW value.
;
;==================================================Test Procedure =======================================================
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_TxWin_22.au3****"&@CRLF)
    _launchApp()
	WinActivate($WIN_BUSMASTER)
	if winexists($WIN_BUSMASTER) Then
		_loadConfig("TS_TxWin_01.cfx")
		Sleep(1000)
		_TxMsgMenu()
		sleep(1000)
		_DeleteMsgBlock()
		Sleep(1000)
		_ConfigCANTXBlockDetails("Cyclic","Yes",1000,"No","","","No","")						          ; Configure TX block details
	    _AddMsg2TxList(0)																			      ; Add the first msg to Tx list
	    _AddMsg2TxList(1)
        sleep(1000)
		$handle=ControlGetHandle($WIN_BUSMASTER,"",$LSTC_MsgDetailsID)                                  ;Get the handle of message details list control
		_GUICtrlListView_ClickItem($handle,1)                                                           ;Click on second message
		Sleep(1000)
		$hWnd = ControlGetHandle($WIN_BUSMASTER,"",$LSTC_Sig_DetailsID)                                ;Get the handle of signal details list control
        $a=_GUICtrlListView_GetItemPosition($hWnd, 0)                                                   ;Get  the position of item(x,y coordinates)
	    ControlClick($WIN_BUSMASTER,"",$LSTC_Sig_DetailsID,"Left",2,$a[0]+200,$a[1])                    ;Double Click on Physical value
	   sleep(500)
	   send("{UP 2}")
	   sleep(1000)
	   send("{ENTER}")
	   _CloseTxWindow()
	   Sleep(500)
	   _ConnectDisconnect()																		        ; Connect the tool
	 _TransmitMsgsMenu()																			    ; Transmit normal blocks
	  Sleep(4000)
	 _ConnectDisconnect()																		        ; Disconnect the tool
	 Sleep(1000)
	 _DisableOverwriteMode()                                                                            ;Disable overwrite mode
     $rCount=_GetCANMsgWinItemCount()                                                                   ;Fetch the no of entries in message window
	 If $rCount>=7 Then
		$Data1=_GetMsgWinCANInfo(0)                                                                     ;Fetch messages from message window
	    $Data2=_GetMsgWinCANInfo(1)
	    $Data3=_GetMsgWinCANInfo(2)
	    $Data4=_GetMsgWinCANInfo(3)

     EndIf
	For $i=0 to 7                                                                                       ;Write messages with all information like(time,Channel,ID) to console
		ConsoleWrite("Data1 :" &$Data1[$i] & @CRLF)
		ConsoleWrite("Data2 :" &$Data2[$i] & @CRLF)
		ConsoleWrite("Data3 :" &$Data3[$i] & @CRLF)
	    ConsoleWrite("Data4 :" &$Data4[$i] & @CRLF)
	next
	If($Data3[7]="02000000000000") Then                                                                  ;Check data byte value for second message to cross verify for the raw value which got updated when																									;when physical value is changed
		_WriteResult("Pass","TS_Tx_22")
	Else
		_WriteResult("Fail","TS_Tx_22")
	EndIf

EndIf
$isAppNotRes=_CloseApp()														                         ; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_Tx_22")
EndIf

ConsoleWrite("****End : TS_TxWin_22.au3****"&@CRLF)
ConsoleWrite(@CRLF)



