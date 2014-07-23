#cs ----------------------------------------------------------------------------

 AutoIt Version: 3.3.8.1
 Author:         myName

 Script Function:
	Template AutoIt script.

#ce ----------------------------------------------------------------------------

; Script Start - Add your code below here
#include <Data_DeclarationModV2.au3>
#include <Excel.au3>
#include <GuiStatusBar.au3>
#include <globalFunc4ModuleV2.au3>												; Global Function
#include <GuiComboBoxEx.au3>
#include <GuiTreeView.au3>
#include <GuiListView.au3>
#include <GuiToolBar.au3>
#include <GuiMenu.au3>
#Include <GuiEdit.au3>
#Include <GuiListBox.au3>
#include <GuiMenu.au3>
#include <File.au3>
#include <Date.au3>
#include <IE.au3>
#include <GuiTab.au3>
#include <GuiComboBox.au3>
#include <GuiButton.au3>
#include <Misc.au3>




WinActivate($WIN_BUSMASTER)
;WinActivate($WIN_FormatConv)
$BusMasterExeFPath=@ProgramFilesDir&"\BUSMASTER_v1.9.0"
;~   $HWD=ControlGetHandle($WIN_BUSMASTER,"",1281)
 ;$item=_GUICtrlListView_GetItemCount($HWD)
  ;ConsoleWrite("$item"&$item & @CRLF)
;~  $count=_GUICtrlTreeView_GetCount($HWD)
;~  ConsoleWrite("count: "&$count & @CRLF)
;~  $handle=_GUICtrlTreeView_GetItemHandle($HWD)
 ;$item2=_GUICtrlTreeView_GetItemByIndex($HWD,$handle,0)
 ;$firstItem=_GUICtrlTreeView_GetFirstItem($handle)
;~   $Parent=_GUICtrlTreeView_GetParentHandle($HWD,$handle)
;~   $itemText=_GUICtrlTreeView_GetText($HWD,$Parent)
;~   ConsoleWrite("itemText: " & $itemText & @CRLF)
;~   $itemTextArr=StringStripWS($itemText,4)
;~   ConsoleWrite("$itemTextArr:" & $itemTextArr & @CRLF)
  ;$Signame1= $itemTextArr[0]
 ; ConsoleWrite("$Signame1: " & $Signame1 & @CRLF)
;~   $Signame2= $itemTextArr[2]
;~    ConsoleWrite("$Signame2: " & $Signame2 & @CRLF)
; ConsoleWrite("First item" & $firstItem & @CRLF)
 ;ConsoleWrite("$item2 "&$item2 & @CRLF)
; $item3=_GUICtrlTreeView_GetNext($HWD,$handle)
 ;ConsoleWrite("item3 "&$item3 & @CRLF)
;~  $handle=WinGetHandle("BUSMASTER")
;~  $MenuHandle=_GUICtrlMenu_GetMenu($handle)
;~  $Data=StringTrimLeft(_GUICtrlMenu_GetItemText($MenuHandle,1),1)
;~  ConsoleWrite("Menu" & $Data & @CRLF)
;~  ;$DataTrim=StringTrimLeft($Data,1)
;~  ;ConsoleWrite("DataTrim" & $DataTrim & @CRLF)
;~ 	$Submenuhandle=_GUICtrlMenu_GetItemSubMenu($MenuHandle,0)
;~        $SubMenuData1=_GUICtrlMenu_GetItemText($Submenuhandle,0)
;~        ConsoleWrite("Submenu" & $SubMenuData1 & @CRLF)
;~ 	   ;$SubMenuData1=StringTrimLeft(_GUICtrlMenu_GetItemText($Submenuhandle,0),1)
;~ 	   $SubMenuData2=_GUICtrlMenu_GetItemText($Submenuhandle,1)
;~ 	   $SubMenuData3=_GUICtrlMenu_GetItemText($Submenuhandle,2)
;~ 	   $SubMenuData4=_GUICtrlMenu_GetItemText($Submenuhandle,3)
;~ 	   $SubMenuData5=_GUICtrlMenu_GetItemText($Submenuhandle,4)
;~ 	   $SubMenuData6=_GUICtrlMenu_GetItemText($Submenuhandle,5)
;~ 	   $SubMenuData7=_GUICtrlMenu_GetItemText($Submenuhandle,6)
;~ 	   $SubMenuData8=_GUICtrlMenu_GetItemText($Submenuhandle,7)
;~ 	   $SubMenuData9=_GUICtrlMenu_GetItemText($Submenuhandle,8)
;~ 	    $SubMenuData10=_GUICtrlMenu_GetItemText($Submenuhandle,9)
;~ 		 $SubMenuData11=_GUICtrlMenu_GetItemText($Submenuhandle,10)
;~ 	    ConsoleWrite("submenu1" & $SubMenuData1 & @CRLF)
;~ 	    ConsoleWrite("submenu2" & $SubMenuData2 & @CRLF)
;~ 	    ConsoleWrite("submenu3" & $SubMenuData3 & @CRLF)
;~ 	    ConsoleWrite("submenu4" & $SubMenuData4 & @CRLF)
;~ 	    ConsoleWrite("submenu5" & $SubMenuData5 & @CRLF)
;~ 	    ConsoleWrite("submenu6" & $SubMenuData6 & @CRLF)
;~ 	    ConsoleWrite("submenu7" & $SubMenuData7 & @CRLF)
;~ 	    ConsoleWrite("submenu8" & $SubMenuData8 & @CRLF)
;~ 		ConsoleWrite("submenu9" & $SubMenuData9 & @CRLF)
;~ 		ConsoleWrite("submenu10" & $SubMenuData10 & @CRLF)
;~ 		ConsoleWrite("submenu11" & $SubMenuData11 & @CRLF)

;~ 		$split=StringSplit($SubMenuData11,"&",1)
;~ 		ConsoleWrite("Splitted data" & $split & @CRLF)

;~ Local $text = "This\nline\ncontains\nC-style breaks."
;~ Local $array = StringSplit($text, '\n', 1)
;~ ConsoleWrite("Array" & $array & @CRLF)

;~ $handle=ControlGetHandle($WIN_BUSMASTER,"","[CLASS:ToolbarWindow32; INSTANCE:4]")
;~ ConsoleWrite("handle" & $handle & @CRLF)
;~ $toolTip=_GUICtrlToolbar_GetToolTips($handle)
;~ ConsoleWrite("tooltip handle" & $toolTip & @CRLF)
;~ $string=_GUICtrlToolbar_GetString($toolTip,0)
;~ ConsoleWrite("tooltip" & $string & @CRLF)





;~   $handle=ControlGetHandle($WIN_BUSMASTER,"","[CLASS:msctls_statusbar32; INSTANCE:2]")
;~ 		  ConsoleWrite("handle" & $handle & @CRLF)
;~ 		  $text=_GUICtrlStatusBar_GetText($handle,1)
;~ 		  ConsoleWrite("text" & $text & @CRLF)
 ;WinMenuSelectItem($WIN_BUSMASTER,"",$CANMenu,$CANNodeSimMenu,$CANNodeSimConfigMenu)
;~ 		  $Handle=ControlGetHandle($WIN_BUSMASTER,"",$TVC_NodeSim_ConfigCANNodeSim)
;~ 		  ConsoleWrite("$Handle" & $Handle & @CRLF)
;~ 		  $ParentHandle=_GUICtrlTreeView_GetItemHandle($Handle)
;~ 		  ConsoleWrite("$ParentHandle" & $ParentHandle & @CRLF)
;~ 		  $ParentText=_GUICtrlTreeView_GetText($Handle,$ParentHandle)
;~ 		  ConsoleWrite("$ParentText" & $ParentText & @CRLF)
;~ 		  $nextItem=_GUICtrlTreeView_GetNext($Handle,$ParentHandle)
;~ 		  ConsoleWrite("$nextItem" & $nextItem & @CRLF)
;~ 		  _GUICtrlTreeView_ClickItem($Handle,$nextItem)
;~ 		  _GUICtrlTreeView_Expand($Handle,$nextItem)

;~ 		  $nextText=_GUICtrlTreeView_GetText($Handle,$nextItem)
;~ 		  ConsoleWrite("$nextText" & $nextText & @CRLF)
;~ 		  $ChildHandle=_GUICtrlTreeView_GetNext($Handle,$nextItem)
;~ 		   ConsoleWrite("$ChildHandle" & $ChildHandle & @CRLF)
;~ 		   $ChildItemText=_GUICtrlTreeView_GetText($Handle,$ChildHandle)
;~ 		   ConsoleWrite("$ChildItemText" & $ChildItemText & @CRLF)
	;EndIf

;~ 	WinMenuSelectItem($WIN_BUSMASTER,"",$fileMenu,$FomatConverterMenu)
;~ 	$hWndCAPL2CPP=controlgethandle($WIN_FormatConv,"",$Tab_CAPL2CPP)
;~ 		_GUICtrlTab_ClickTab($hWndCAPL2CPP, 0)
;~ 			$Tabtext1=_GUICtrlTab_GetItemText($hWndCAPL2CPP,0)                                               ;Get text of the tab
;~ 		ConsoleWrite("text1" & $Tabtext1 & @CRLF)
;~ 		$Tabtext2=_GUICtrlTab_GetItemText($hWndCAPL2CPP,1)
;~ 	    ConsoleWrite("text2" & $Tabtext2 & @CRLF)
;~          $Tabtext3=_GUICtrlTab_GetItemText($hWndCAPL2CPP,2)
;~ 		  ConsoleWrite("text3" & $Tabtext3 & @CRLF)

;~ $handle=WinGetHandle($WIN_BUSMASTER)                                                    ;Get the handlle of the window of menu
;~ 		$MenuHandle=_GUICtrlMenu_GetMenu($handle)                                                ;Get Menu handle
;~ 		send("!f")
;~ 		send("{ESC}")
;~         $MenuData1=StringTrimLeft(_GUICtrlMenu_GetItemText($MenuHandle,0),1)                       ;Retrieves menu item titles by removing '&' at the ledt side of the name
;~         ConsoleWrite("Menu Data1" & $MenuData1 & @CRLF)



;~ 		$Submenuhandle=_GUICtrlMenu_GetItemSubMenu($MenuHandle,0)
;~        $SubMenuData1=_GUICtrlMenu_GetItemText($Submenuhandle,0)
;~        ConsoleWrite("Submenu" & $SubMenuData1 & @CRLF)
;~ 	   $SubMenuState=_GUICtrlMenu_GetItemGrayed($Submenuhandle,0,True)
;~ 	   ConsoleWrite("Submenu state" & $SubMenuState & @CRLF)

;~ $handle=ControlGetHandle($WIN_BUSMASTER,"",128)
;~ ConsoleWrite("handle" & $handle & @CRLF)
;~ $ButtonCommand_id=_GUICtrlToolbar_IndexToCommand($handle,11)
;~ ConsoleWrite("Command id is" & $ButtonCommand_id & @CRLF)
;~ $text=_GUICtrlToolbar_GetButtonText($handle,32824)
;~ $string=_GUICtrlToolbar_GetString($handle, 11)
;~ ;_GUICtrlToolbar_ClickIndex($handle,11)
;~ $state=_GUICtrlToolbar_IsButtonPressed($handle,$ButtonCommand_id)
;~ ConsoleWrite("State" & $state & @CRLF)

;~ ConsoleWrite("toolbar string is" &$string & @CRLF)
;~ ConsoleWrite("button text" & $text & @CRLF)
;~ $info=_GUICtrlToolbar_GetButtonInfo($handle,$ButtonCommand_id)
;~ ConsoleWrite("button info" & $info[0] & @CRLF)
;~ _TxMsgMenu()																				    ; Select CAN->Transmit->Configure menu

;~      sleep(1000)
;~  	_EnableAutoUpdate()                                                                             ;Enable Auto update
;~  	sleep(1000)
;~ 	_DeleteMsgBlock()                                                                               ;Delete message blocks if there are any
;~ 	sleep(1000)
;~ _ConfigCANTXBlockDetails("Cyclic","Yes",2000,"No","","","No","")							    ; Configure TX block details

;~ 	_AddMsg2TxList(0)																			    ; Add the first msg to Tx list
;~ 	sleep(500)
;~ 	_AddMsg2TxList(1)
;~ 	sleep(1000)

;~ 	_ConfigCANTXBlockDetails("Cyclic","Yes",2000,"No","","","No","")							    ; Configure TX block details

;~ 	_AddMsg2TxList(2)																			    ; Add the first msg to Tx list
;~ 	sleep(500)
;~ 	_AddMsg2TxList(3)
;~    _ClickNode(0)
;~       sleep(1000)

;~ 	  _AddJ1939CppFile("DLLHandlerNS_22_J1939.cpp")
;~ 	  sleep(1000)
;~ 	  $TVHWD=_GetJ1939CppTVHWD()															; Fetch the handle of tree view
;~ 	$MsgHWD=_GetJ1939HandlerHWD(2,6)														; Fetch the handle of the Msg handler item
;~ 	_GUICtrlTreeView_ClickItem($TVHWD, $MsgHWD, "Right") 								; Right click on Msg Handler item in the tree view
;~ 	sleep(500)
;~ 	send("a")
;~     WinWaitActive("Add DLL Handler(s)","",5)
;~ ControlCommand("Add DLL Handler(s)","",1164,"Check")
;~ sleep(500)
;~ ControlClick("Add DLL Handler(s)","",1178)
;~ $TVHWD=_GetJ1939CppTVHWD()
;~ 	$DLLHWD=_GetJ1939HandlerHWD(2,6)
;~ 	_GUICtrlTreeView_ClickItem($TVHWD, $DLLHWD)					 							; Click on Msg Handler item in the tree view

;~ 	send("{DOWN}")																			; Send the down key to select the Msg handler function

;~ 	send("{ENTER}")
;~ 	ControlSend($WIN_BUSMASTER, "", $TXT_TimeHandler_CPPFile, "Trace"  & "("  &  Chr(34) & "Loaded" & Chr(34) & ");")
;~ 	sleep(500)
;~    ; Send("^s")																				; Save the cpp file

	;Send("^{F4}") 																			; Close the C file window
	;sleep(500)

;_ExecuteDLLHandler("ForJ1939")
;~  $TraceHWD=ControlGetHandle($WIN_BUSMASTER,"",$LSTB_Details_TraceWin)
;~ _GUICtrlListView_ClickItem($TraceHWD,0)
;~  	$count=_GUICtrlListView_GetItemCount($TraceHWD)
	;sleep(1000)
;$count=_GetTraceWinItemCount()
;ConsoleWrite("count" & $count & @CRLF)

;$text=ControlGetText("BUSMASTER Format Conversions","",1016)


;~ _ClickNode(0)
;~       sleep(1000)

;~ 	  _AddJ1939CppFile("DLLHandlerNS_CAN.cpp")
;~ 	  sleep(1000)
;~ 	  $TVHWD=_GetCANCppTVHWD()															; Fetch the handle of tree view
;~ 	$MsgHWD=_GetCANHandlerHWD(2,6)														; Fetch the handle of the Msg handler item
;~ 	_GUICtrlTreeView_ClickItem($TVHWD, $MsgHWD, "Right") 								; Right click on Msg Handler item in the tree view
;~ 	sleep(500)
;~ 	send("a")
;~     WinWaitActive("Add DLL Handler(s)","",5)
;~ ControlCommand("Add DLL Handler(s)","",1164,"Check")
;~ sleep(500)
;~ ControlClick("Add DLL Handler(s)","",1178)
;~ $TVHWD=_GetCANCppTVHWD()
;~ 	$DLLHWD=_GetCANHandlerHWD(2,6)
;~ 	_GUICtrlTreeView_ClickItem($TVHWD, $DLLHWD)					 							; Click on Msg Handler item in the tree view

;~ 	send("{DOWN}")																			; Send the down key to select the Msg handler function

;~ 	send("{ENTER}")
;~ 	ControlSend($WIN_BUSMASTER, "", $TXT_TimeHandler_CPPFile, "Trace"  & "("  &  Chr(34) & "Loaded" & Chr(34) & ");")


 ;WinActivate("BUSMASTER Format Conversions")
;~ $Text=WinGetText("BUSMASTER Format Conversions")

;~ ;For $i=0 To 10
;~ 	ConsoleWrite("$Text: " & $Text & @CRLF)
;~ ;Next

;~ $res=StringInStr($Text,"Conversion completed with warnings.")
;~ ConsoleWrite("$res" & $res & @CRLF)

;~ $temp=ControlCommand($WIN_FormatConv,"",$BTN_ViewLog,"IsEnabled")
;~ ConsoleWrite("$temp" & $temp & @CRLF)
;~ If($temp=1) Then
;~ 	controlclick($WIN_FormatConv,"",$BTN_ViewLog)                                                               ; open log file to view warnings if any
;~ 	sleep(2000)
;~ 	 $LogfName=StringTrimRight  ($OutputFile, 4)															   ; trim ".cpp" from output fname
;~ 			ConsoleWriteError("$LogfName : " & $LogfName & @CRLF)
;~ 		    $ConversionRes[1]=FileReadLine ($OutputFilePath & "\" & $LogfName &"log.txt",1)						; Fetch the input fPath in the report
;~  		    $ConversionRes[2]=FileReadLine ($OutputFilePath & "\" & $LogfName &"log.txt",2)						; Fetch the output fPath in the report
;~  		    $ConversionRes[3]=FileReadLine ($OutputFilePath & "\" & $LogfName &"log.txt",6)						; Fetch the conversion reslut fPath in the report
;~  		    ConsoleWriteError("$ConversionRes[0] : " & $ConversionRes[0] & @CRLF)
;~  		    ConsoleWriteError("$ConversionRes[1] : " & $ConversionRes[1] & @CRLF)
;~  		    ConsoleWriteError("$ConversionRes[2] : " & $ConversionRes[2] & @CRLF)
;~  		    ConsoleWriteError("$ConversionRes[3] : " & $ConversionRes[3] & @CRLF)
;~ EndIf

;~ $handle=ControlGetHandle("BUSMASTER Format Conversions","",1015)
;~ _GUICtrlComboBoxEx_ShowDropDown($handle,True)
;~ ControlClick("BUSMASTER Format Conversions","",1015)
;~ Sleep(1000)
;~ send("{DOWN}")
;send("{DOWN}")
;send("{ENTER}")
;~ ControlClick("BUSMASTER Format Conversions","",1015)
;~ _GUICtrlComboBox_SelectString($handle,"J1939")
;~ Sleep(1000)
;~ ControlClick("BUSMASTER Format Conversions","",1015)


;~ $OutputFolderPath=_OutputDataPath()
;~ $InputFolderPath=_TestDataPath()
;~ $Result=_LogToExcelConversions("CAN","TestLogToExcel.log","TestLogToExcel.xls")
;~ $OutputCppFilePath=$OutputFolderPath &"\"& "TestLogToExcel.xls"
;~ If FileExists($OutputCppFilePath) Then
;~ 		 $OutputFileExist=1
;~ 	 Else
;~ 		 $OutputFileExist=0
;~ 	 EndIf
;~ 	 If($Result="Successfully converted text file to Excel file!") Then
;~ 		 $Conv=1
;~ 	 Else
;~ 		 $Conv=0
;~ 	 EndIf
;~ 	 ConsoleWrite("$OutputFileExist" & $OutputFileExist & @CRLF)
;~ 	 ConsoleWrite("$Conv" & $Conv & @CRLF)
;_LogToExcelConversions("J1939","TestLogToExcel_J1939.log","TestLogToExcel_J1939.xls")
;$RBTN_Monoshot_ConfigTX
;~ $text=ControlGetText($WIN_BUSMASTER,"",1134)
;~ 	ConsoleWrite("$text" & $text & @CRLF)
;~ 	$handle=ControlGetHandle($WIN_BUSMASTER,"","[CLASS:Button; INSTANCE:21]")
;~ 	$state=_GUICtrlButton_Getstate($handle)
;~ 	ConsoleWrite("$state" & $state & @CRLF)
;~ $hWnd = ControlGetHandle("BUSMASTER","",1232)
;~ $a=_GUICtrlListView_GetItemPosition($hWnd, 0)
;~ ControlClick("BUSMASTER","",1232,"Left",2,$a[0]+100,$a[1])
;~ $spinCtrlRaw=ControlCommand("BUSMASTER","",74565,"IsVisible")
;~ ControlClick("BUSMASTER","",1232,"Left",2,$a[0]+200,$a[1])
;~ $spinCtrlPhy=ControlCommand("BUSMASTER","",74565,"IsVisible")

;~     $handle=ControlGetHandle($WIN_BUSMASTER,"",$LSTC_MsgDetailsID)
;~ 	;ControlClick($WIN_BUSMASTER,"",$LSTC_MsgDetailsID,"Right")
;~ 	_GUICtrlListView_ClickItem($handle,1,"Right")
;~       Sleep(500)
;~ 	  _GUICtrlListView_ClickItem($handle,1)
;~ 	  sleep(500)
;~ 	  Send("{D}")
;~ 	  If WinExists($WIN_BUSMASTER) Then
;~ 		  $WinExists=1
;~ 		  ControlClick($WIN_BUSMASTER,"",6)
;~ 	  Else
;~ 		  $WinExists=0
;~ 	  EndIf
;~ 	  ConsoleWrite("$WinExists" & $WinExists & @CRLF)
;~ 	  Sleep(500)
;~ 	  $count=_GUICtrlListView_GetItemCount($handle)
;~ 	  ConsoleWrite("No if items" & $count & @CRLF)



;~ If WinExists($WIN_ConfirmSaveAs) Then
;~ 			ControlClick($WIN_ConfirmSaveAs,"","[CLASS:Button; INSTANCE:1]")
;~ 		EndIf
;WinMove("Network Statistics","",5,5)
;~ ControlClick($WIN_BUSMASTER,"",1015)
;~ If WinWaitActive("","Do you want to save changes?",2) Then
;~ 	$msg=1
;~ 	ControlClick($WIN_BUSMASTER,"",7)
;~ Else
;~ 	$msg=0
;~ EndIf
;~ ConsoleWrite("$msg" & $msg & @CRLF)
;ControlClick($WIN_BUSMASTER,"",1213)
;~ $handle=ControlGetHandle($WIN_BUSMASTER,"",1215)
;~ ;$state=_GUICtrlButton_GetCheck($handle)

;~ $state=_GUICtrlButton_GetState($handle)
;~ ConsoleWrite("state : "& $state & @CRLF)
;~         $handle1=ControlGetHandle($WIN_BUSMASTER,"",1213)
;~ 		$handle2=ControlGetHandle($WIN_BUSMASTER,"",1215)
;~ 		$handle3=ControlGetHandle($WIN_BUSMASTER,"",1216)
;~ 		$state1=_GUICtrlButton_GetState($handle1)
;~ 		$state2=_GUICtrlButton_GetCheck($handle2)
;~ 		$state3=_GUICtrlButton_GetState($handle3)

;~         ConsoleWrite("state1 : "& $state1 & @CRLF)
;~ 		ConsoleWrite("state2 : "& $state2 & @CRLF)
;~ 		ConsoleWrite("state3 : "& $state3 & @CRLF)
;~ $handle=ControlGetHandle($WIN_BUSMASTER,"",$LSTC_MsgDetailsID)
;~ 		_GUICtrlListView_ClickItem($handle,1)
;~ 		Sleep(1000)
;~ 		$handle1=ControlGetHandle($WIN_BUSMASTER,"",1213)
;~ 		$handle2=ControlGetHandle($WIN_BUSMASTER,"",1215)
;~ 		$handle3=ControlGetHandle($WIN_BUSMASTER,"",1216)
;~ 		$state1=_GUICtrlButton_GetCheck($handle1)
;~ 		$state2=_GUICtrlButton_GetCheck($handle2)
;~ 		$state3=_GUICtrlButton_GetCheck($handle3)

;~         ConsoleWrite("state1 : "& $state1 & @CRLF)
;~ 		ConsoleWrite("state2 : "& $state2 & @CRLF)
;~ 		ConsoleWrite("state3 : "& $state3 & @CRLF)
;~ 		Sleep(1000)
;~ 		ControlClick($WIN_BUSMASTER,"",1213)
;~ 		$StateNew=_GUICtrlButton_GetCheck($handle1)
;		ConsoleWrite("$StateNew" & $StateNew & @CRLF)

;~ 	$hWnd=ControlGetHandle ($WIN_BUSMASTER,"",$LSTC_J1939MsgWin)
;~ 	$count=_GUICtrlListView_GetColumnCount($hWnd)
;~ 	ConsoleWrite("Column count" & $count & @CRLF)
;~     WinMenuSelectItem($WIN_BUSMASTER,"",$CANMenu,$CANNWStatsMenu)                             ; Select CAN->Network Statistics
;~ 	Sleep(500)
;~     $pos=WinGetPos($WIN_NW_Stats)
;~ 	ConsoleWrite("Original position of the window X cord" & $pos[0] & @CRLF)
;~ 	ConsoleWrite("Original position of the window Y cord" & $pos[1] & @CRLF)
;~ 	MouseClickDrag("left",$pos[0],$pos[1],740,260)
;~ 	Sleep(500)
;~ 	MouseDown ("left" )
	$HWD=ControlGetHandle($WIN_BUSMASTER,"","[CLASS:SysListView32; INSTANCE:1]")
;~ 	$SecColH=_GUICtrlListView_GetColumn($HWD,1)										; Fetch the 2nd column header text
;~ 	;$ColTime=StringStripWS($SecColH[5],2)
;~ 	ConsoleWrite("Column header info" & $SecColH[5] & @CRLF)
;~ 		$count= _GUICtrlListView_GetColumnCount($HWD)
;~         ConsoleWrite("column count" & $count & @CRLF)
;~ If WinExists($WIN_BUSMASTER,"Dissociation will clear Signal Watch List.Do you want to Clear the List?") Then
;~ 			ControlClick($WIN_BUSMASTER,"",6)
;~ 			Sleep(1000)
;~ 		EndIf
;~ $ConfigureToolhWd=ControlGetHandle($WIN_BUSMASTER,"",276)					; Get handle of the 'Connect/Disconnect' toolbar
;~ 	_GUICtrlToolbar_ClickIndex($ConfigureToolhWd,7)
;~


;~ 	$handle=ControlGetHandle($WIN_BUSMASTER,"",128)
;~ 	$id=_GUICtrlToolbar_IndexToCommand($handle,6)
;~ 	;$text=_GUICtrlToolbar_GetButtonText($handle,$id)
;~ 	$state=_GUICtrlToolbar_IsButtonHighlighted($handle,$id)
;~ 	;ConsoleWrite("Button text:" &$text & @CRLF)
;~ 	ConsoleWrite("Button state" & $state & @CRLF)
;~ ;	_GUICtrlToolbar_ClickIndex($handle,6)
;~ $handle= ControlGetHandle($WIN_NW_Stats,"",$LVC_Details_NW_Stats)
;~ $count2= _GUICtrlListView_GetColumnCount($handle)
;~ 	ConsoleWrite("Column count after" & $count2& @CRLF)
$handle=WinGetHandle($WIN_BUSMASTER)
	$menuHandle=_GUICtrlMenu_GetMenu($handle)

	$sub=_GUICtrlMenu_GetItemSubMenu($menuHandle,3)
	$text=_GUICtrlMenu_GetItemText($sub,0)
	ConsoleWrite("Item text" & $text & @CRLF)












