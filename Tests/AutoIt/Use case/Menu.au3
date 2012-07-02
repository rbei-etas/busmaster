#Include <GuiTreeView.au3>


 if WinExists("BUSMASTER") Then
	 sleep(2000)
;~ 	 File Menu options
	;~ DataBase options
	;~ New
	WinMenuSelectItem("BUSMASTER","","&File","&Database","&New")
	Sleep(2000)
	if WinExists("Save As") Then
	   ConsoleWrite("Checking the Save As windows labells"&@CRLF)
;~        $Var1 = WinGetText("Save As")
		 $Var1 = ControlGetText("Save As", "", 2)
;~ 	    ConsoleWrite($Var1&@CRLF)
	   If $Var1="Cancel" Then
		  ConsoleWriteError("Cancel button was found"&@CRLF)
	   Else
		  ConsoleWriteError("Cancel button was not found"&@CRLF)
	   EndIf
	   $Var2 = ControlGetText("Save As", "", 1)
	   If $Var2="&Save" Then
		  ConsoleWriteError("Save button was found"&@CRLF)
	   Else
		  ConsoleWriteError("Save button was not found"&@CRLF)
	   EndIf
	   controlclick("Save As","","[CLASS:Button; INSTANCE:3]")
	EndIf

   	;~ Open
   WinMenuSelectItem("BUSNASTER","","&File","&Database","&Open")
   Sleep (2000)
   if WinExists("Select BUSMASTER Database FileNmae...") Then
	  ConsoleWrite("Checking the Select BM DB")
	  $Var1 = ControlGetText("Select BUSMASTER Database FileName...", "", 2)
;~ 	    ConsoleWrite($Var1&@CRLF)
	   If $Var1="Cancel" Then
		  ConsoleWriteError("Cancel button was found"&@CRLF)
	   Else
		  ConsoleWriteError("Cancel button was not found"&@CRLF)
	   EndIf
	   $Var2 = ControlGetText("Select BUSMASTER Database FileName...", "", 1)
	   If $Var2="&Open" Then
		  ConsoleWriteError("Open button was found"&@CRLF)
	   Else
		  ConsoleWriteError("Open button was not found"&@CRLF)
	   EndIf
	   controlclick("Select BUSMASTER Database FileName...","","[CLASS:Button; INSTANCE:3]")
   EndIf




;~     Associate
   WinMenuSelectItem("BUSMASTER","","&File","&Database","&Associate")
   Sleep(1000)
   if WinExists("Select Active Database Filename")Then
	  ConsoleWrite("Checking the labels in Associate databse Window"&@CRLF)
	  $Var1 = ControlGetText("Select Active Database Filename", "", 3)
	  If $Var1="Cancel" Then
		  ConsoleWriteError("Cancel button was found"&@CRLF)
	   Else
		  ConsoleWriteError("Cancel button was not found"&@CRLF)
	   EndIf
	   $Var2 = ControlGetText("Select Active Database Filename", "", 2)
	  If $Var2="&Open" Then
		  ConsoleWriteError("Open button was found"&@CRLF)
	   Else
		  ConsoleWriteError("Open button was not found"&@CRLF)
	   EndIf
	   ControlClick("Select Active Database Filename","","[CLASS:Button; INSTANCE:3]")
   EndIf

;~    Dis Associate
   WinMenuSelectItem("BUSMASTER","","&File","&Database","&Disassociate")
   Sleep(1000)
   if WinExists("Dissociate Database")Then
	  ConsoleWrite("Checking the labels in Dis associate databse Window"&@CRLF)
	  $Var1 = ControlGetText("Dissociate Database", "", 2)
	  If $Var1="Cancel" Then
		  ConsoleWriteError("Cancel button was found"&@CRLF)
	   Else
		  ConsoleWriteError("Cancel button was not found"&@CRLF)
	   EndIf
	   $Var2 = ControlGetText("Select Active Database Filename", "", 1285)
	  If $Var2="Dissociate" Then
		  ConsoleWriteError("Dissociate button was found"&@CRLF)
	   Else
		  ConsoleWriteError("Dissociate button was not found"&@CRLF)
	   EndIf
	   ControlClick("Dissociate Database","","[CLASS:Button; INSTANCE:2]")
   EndIf

;~     Configure
WinMenuSelectItem("BUSMASTER","","&File","&Configuration","Save &As...")
   Sleep(1000)
   if WinExists("SaveAs Configuration Filename...")Then
	  ConsoleWrite("Checking the labels in Save As window"&@CRLF)
	  $Var1 = ControlGetText("SaveAs Configuration Filename...", "",2)
	  If $Var1="Cancel" Then
		  ConsoleWriteError("Cancel button was found"&@CRLF)
	   Else
		  ConsoleWriteError("Cancel button was not found")
	   EndIf
	   $Var2 = ControlGetText("Save As", "", 1)
	   If $Var2="&Save" Then
		  ConsoleWriteError("Save button was found"&@CRLF)
	   Else
		  ConsoleWriteError("Save button was not found"&@CRLF)
	   EndIf
	  ControlClick("SaveAs Configuration Filename...","","[CLASS:Button; INSTANCE:3]")
   EndIf

;~    Utilities
WinMenuSelectItem("BUSMASTER","","&File","&Utilities","&Format Converters")
   Sleep(5000)
   if WinExists("BUSMASTER Format Conversions")Then
	  ConsoleWrite("Checking the labels in Select Active databse filename"&@CRLF)
	  $Var1 = ControlGetText("BUSMASTER Format Conversions", "", 2)
	  If $Var1="Close" Then
		  ConsoleWriteError("Close button was found"&@CRLF)
	   Else
		  ConsoleWriteError("Close button was not found"&@CRLF)
	   EndIf
	  ControlClick("BUSMASTER Format Conversions","","[CLASS:Button; INSTANCE:8]")
   EndIf

;~ Properties
WinMenuSelectItem("BUSMASTER","","&File","&Properties")
   Sleep(1000)
   if WinExists("Properties")Then
	  ConsoleWrite("Checking the labels in Properties window"&@CRLF)
	  $Var1 = ControlGetText("Properties", "", 1246)
	  $pptHWD=ControlGetHandle("Properties","","[CLASS:SysTreeView32; INSTANCE:1]")
	  _GUICtrlTreeView_Expand($pptHWD,0)
	  sleep(2000)
	  If $Var1="Tree1" Then
		  ConsoleWriteError("Properties window was found"&@CRLF)
	   Else
		  ConsoleWriteError("Properties windows was not found"&@CRLF)
	   EndIf
	   WinClose("Properties")
   EndIf

;~ Signal watch Window
WinMenuSelectItem("BUSMASTER","","&View","Signal &Watch Window")
Sleep(1000)
if WinExists("Signal Watch - CAN")Then
   WinClose("Signal Watch - CAN")
EndIf

; Signal Graph Window
send("!vg")
sleep(1000)
send("^{F4}")


;~ Network Statistics
WinMenuSelectItem("BUSMASTER","","&View","&Network Statistics")
   Sleep(1000)
   if WinExists("Network Statistics")Then
	  ConsoleWrite("Checking the labels in Network statistics window"&@CRLF)
	  $Var1 = ControlGetText("Network Statistics","",1226)
	  If $Var1="List1" Then
		  ConsoleWriteError("Network statistics window was found"&@CRLF)
	   Else
		  ConsoleWriteError("Network Statistics window was not found"&@CRLF)
	   EndIf
	   WinClose("Network Statistics")
   EndIf

;~ Configure
   ;~ Message Display
WinMenuSelectItem("BUSMASTER","","&Configure","&Message Display")
   Sleep(1000)
   if WinExists("Configure Message Display- CAN")Then
	  ConsoleWrite("Checking the labels in Message display window"&@CRLF)
	  $Var1 = ControlGetText("Configure Message Display- CAN", "", 1031)
	  If $Var1="Configure" Then
		  ConsoleWriteError("Configure button was found")
		  if WinExists("Filter Selection Dialog")Then
			 ConsoleWrite("Checking the labels in Network statistics window"&@CRLF)
			 $Var2 = ControlGetText("Network Statistics","",2)
			 If $Var2="&Cancel" Then
		     ConsoleWriteError("Cancel button was found"&@CRLF)
			 Else
			 ConsoleWriteError("Cancel button was not found"&@CRLF)
			 EndIf
		  EndIf
		  $Var3 = ControlGetText("Network Statistics","",1)
			 If $Var3="&Ok" Then
		     ConsoleWriteError("Ok button was found"&@CRLF)
			 Else
			 ConsoleWriteError("Ok button was not found"&@CRLF)
			 EndIf
	   Else
		  ConsoleWriteError("Configure button was not found"&@CRLF)
	  	   WinClose("Network Statistics")
	   EndIf
	   $Var4 = ControlGetText("Network Statistics","",1)
	   If $Var4="Ok" Then
		  ConsoleWriteError("Ok button was found"&@CRLF)
	   Else
		  ConsoleWriteError("Ok button window was not found"&@CRLF)
	   EndIf
	   $Var5 = ControlGetText("Network Statistics","",2)
	   If $Var5="Cancel" Then
		  ConsoleWriteError("Cancel button was found"&@CRLF)
	   Else
		  ConsoleWriteError("Cancel button window was not found"&@CRLF)
	   EndIf
	   $Var6 = ControlGetText("Network Statistics","",9)
	   If $Var6="Help" Then
		  ConsoleWriteError("Help button was found"&@CRLF)
	   Else
		  ConsoleWriteError("Help button was not found"&@CRLF)
	   EndIf
	   ControlClick("Configure Message Display- CAN","","[CLASS:Button; INSTANCE:3]")
   EndIf

;~    Log
WinMenuSelectItem("BUSMASTER","","&Configure","&Log")
   Sleep(1000)
   if WinExists("Configure Logging for CAN")Then
	  ConsoleWrite("Checking the labels in Configure Logging for CAN window"&@CRLF)
	  $Var1 = ControlGetText("Configure Logging for CAN", "","[CLASS:Button;INSTANCE:16]")
	  If $Var1="Log Blocks" Then
		  ConsoleWriteError("Add button was found"&@CRLF)
	   Else
		  ConsoleWriteError("Add button was not found"&@CRLF)
	   EndIf
	   $Var2 = ControlGetText("Configure Logging for CAN", "",1)
	   If $Var2="&OK" Then
		  ConsoleWriteError("OK button was found"&@CRLF)
	   Else
		  ConsoleWriteError("OK button was not found"&@CRLF)
	   EndIf
	   $Var3 = ControlGetText("Configure Logging for CAN", "",2)
	   If $Var3="&Cancel" Then
		  ConsoleWriteError("Cancel button was found"&@CRLF)
	   Else
		  ConsoleWriteError("Cancel button was not found"&@CRLF)
	   EndIf
	   controlclick("Configure Logging for CAN","","[CLASS:Button; INSTANCE:14]")
   EndIf

;Tx Window
send("!ct")
sleep(1000)
ControlClick("BUSMASTER","","[CLASS:Button; INSTANCE:25]")

;~    Replay
WinMenuSelectItem("BUSMASTER","","&Configure","&Replay")
   Sleep(1000)
   if WinExists("Replay Configuration")Then
	  ConsoleWrite("Checking the labels in Replay Configuration window"&@CRLF)
	  $Var1 = ControlGetText("Replay Configuration", "","[CLASS:Button;INSTANCE:11]")
	  If $Var1="Replay Files" Then
		  ConsoleWriteError("Add button was found"&@CRLF)
		  $Var4 = ControlGetText("Select Replay File Name", "",1)
		  If $Var4="&Open" Then
		  ConsoleWriteError("Open button was found"&@CRLF)
		  Else
		  ConsoleWriteError("Open button was not found"&@CRLF)
		  EndIf
		  $Var5 = ControlGetText("Select Replay File Name", "",2)
		  If $Var5="Cancel" Then
		  ConsoleWriteError("Cancel button was found"&@CRLF)
		  Else
		  ConsoleWriteError("Cancel button was not found"&@CRLF)
		  EndIf
	  Else
		  ConsoleWriteError("Add button was not found"&@CRLF)
	  EndIf
	   WinClose("Select Replay File Name")
	   $Var2 = ControlGetText("Replay Configuration", "",1)
	   If $Var2="OK" Then
		  ConsoleWriteError("OK button was found"&@CRLF)
	   Else
		  ConsoleWriteError("OK button was not found"&@CRLF)
	   EndIf
	   $Var3 = ControlGetText("Replay Configuration", "",2)
	   If $Var3="Cancel" Then
		  ConsoleWriteError("Cancel button was found"&@CRLF)
	   Else
		  ConsoleWriteError("Cancel button was not found"&@CRLF)
	   EndIf
	   ControlClick("Replay Configuration","","[CLASS:Button; INSTANCE:9]")
   EndIf



;~    App Filters
WinMenuSelectItem("BUSMASTER","","&Configure","&App Filters")
   Sleep(1000)
   if WinExists("Configure Filter List")Then
	  ConsoleWrite("Checking the labels in Configure Filter List window"&@CRLF)
	  $Var1 = ControlGetText("Configure Filter List","", "[CLASS:Button;INSTANCE:10]")
	  If $Var1="Filter Attributes" Then
		  ConsoleWriteError("Add button was not found"&@CRLF)
	  Else
		  ConsoleWriteError("Add button was not found"&@CRLF)
	  EndIf
	   $Var2 = ControlGetText("Configure Filter List", "",1256)
	  If $Var2="OK" Then
		  ConsoleWriteError("OK button was found"&@CRLF)
	  Else
		  ConsoleWriteError("OK button was not found"&@CRLF)
	  EndIf
	   $Var3 = ControlGetText("Configure Filter List", "",2)
	  If $Var3="Cancel" Then
		  ConsoleWriteError("Cancel button was found"&@CRLF)
	  Else
		  ConsoleWriteError("Cancel button was not found"&@CRLF)
	   EndIf
	   ControlClick("Configure Filter List","","[CLASS:Button; INSTANCE:8]")
   EndIf


;~    Simulated Systems
WinMenuSelectItem("BUSMASTER","","&Configure","&Simulated Systems")
   Sleep(1000)
   if WinExists("Configure Simulated Systems - CAN Bus")Then
	  ConsoleWrite("Configure Simulated Systems - CAN Bus was found"&@CRLF)
	Else
	  ConsoleWrite("Configure Simulated Systems - CAN Bus was found"&@CRLF)
	EndIf
   send("^{F4}")
   EndIf

;~    Signal Graph
WinMenuSelectItem("BUSMASTER","","&Configure","Signal &Graph")
   Sleep(1000)
   if WinExists("Configure Signal Graph Window")Then
	  ConsoleWrite("Checking the labels in Configure Signal Graph WIndow"&@CRLF)
	  $Var1 = ControLGetText("Configure Signal Graph WIndow","","[CLASS:Button;INSTANCE:10]")
	  If $Var1="Configure Elements" Then
		  ConsoleWriteError("Configure Signals button was not found"&@CRLF)
		  $Var2 = ControlGetText("Select Element","",1)
		  If $Var2="Ok" Then
		  ConsoleWriteError("Ok button was not found"&@CRLF)
		  Else
		  ConsoleWriteError("Ok button was not found"&@CRLF)
		  EndIf
		  $Var3 = ControlGetText("Select Element","",2)
		  If $Var2="Cancel" Then
		  ConsoleWriteError("Cancel button was not found"&@CRLF)
		  Else
		  ConsoleWriteError("Cancel button was not found"&@CRLF)
		  EndIF
	   EndIf
	   $Var4 = ControlGetText("Configure Signal Graph WIndow","",1)
	   If $Var4="OK" Then
		  ConsoleWriteError("OK button was found"&@CRLF)
	   Else
		  ConsoleWriteError("OK button was not found"&@CRLF)
	   EndIf
	   $Var5 = ControlGetText("Configure Filter List", "",2)
	  If $Var5="Cancel" Then
		  ConsoleWriteError("Cancel button was found"&@CRLF)
	  Else
		  ConsoleWriteError("Cancel button was not found"&@CRLF)
	   EndIf
	  ControlClick("Configure Signal Graph Window","","[CLASS:Button; INSTANCE:1]")
   EndIf


;~ Functions

;connect
send("{F2}")
sleep(1000)
send("{ESC}")
sleep(1000)

;Hex
send("!uh")  	;check
sleep(500)
send("!uh")		;uncheck

;Enable Filter
send("!ufl")	;check
send("!ufd")	;check
send("!ufl")	;uncheck
send("!ufd")	;uncheck

;msg window
send("!umms")	;System mode
send("!umma")	;Absolute mode
send("!ummr")	;Relative mode

;msg window
	;overwrite
send("!umo")

;Test Automation
;~ send("!uae")		;crashes
;~ send("^{F4}")

send("!uax")		; Test Executor
send("^{F4}")		; close window

;~ J1939

;Activate
Send("!ja")
sleep(2000)

;deactivate
;~ send("!jt")

;~ DataBase
;~ New
WinMenuSelectItem("BUSMASTER", "", "&J1939", "&Database","&New")
Sleep(1000)
if WinExists("Save As")Then
	  ConsoleWrite("Checking the labels in Save As window"&@CRLF)
	  $Var1 = ControlGetText("Save As", "",2)
	  If $Var1="Cancel" Then
		  ConsoleWriteError("Cancel button was found"&@CRLF)
	   Else
		  ConsoleWriteError("Cancel button was not found")
	   EndIf
	   $Var2 = ControlGetText("Save As", "", 1)
	   If $Var2="&Save" Then
		  ConsoleWriteError("Save button was found"&@CRLF)
	   Else
		  ConsoleWriteError("Save button was not found"&@CRLF)
	   EndIf
	   ControlClick("Save As","","[CLASS:Button; INSTANCE:3]")
   EndIf

;~ Open
WinMenuSelectItem("BUSMASTER", "", "&J1939", "&Database","&Open")
Sleep(1000)
if WinExists("Select J1939 Database Filename...")Then
	  ConsoleWrite("Checking the labels in Open window"&@CRLF)
	  $Var1 = ControlGetText("Select J1939 Database Filename...", "",2)
	  If $Var1="Cancel" Then
		  ConsoleWriteError("Cancel button was found"&@CRLF)
	   Else
		  ConsoleWriteError("Cancel button was not found")
	   EndIf
	   $Var2 = ControlGetText("Select J1939 Database Filename...", "", 1)
	   If $Var2="&Open" Then
		  ConsoleWriteError("Open button was found"&@CRLF)
	   Else
		  ConsoleWriteError("Open button was not found"&@CRLF)
	   EndIf
	   ControlClick("Select J1939 Database Filename...","","[CLASS:Button; INSTANCE:3]")
   EndIf

;~ Associate
WinMenuSelectItem("BUSMASTER","","&J1939", "&Database","&Associate")
sleep(1000)
   if WinExists("Select Active Database Filename...")Then
	  ConsoleWrite("Checking the labels in Associate database window"&@CRLF)
	  $Var1 = ControlGetText("Select Active Database Filename...", "",2)
	   If $Var1="Cancel" Then
		  ConsoleWriteError("Cancel button was found"&@CRLF)
	   Else
		  ConsoleWriteError("Cancel button was not found")
	   EndIf
	   $Var2 = ControlGetText("Select Active Database Filename...", "", 1)
	   If $Var2="&Open" Then
		  ConsoleWriteError("Open button was found"&@CRLF)
	   Else
		  ConsoleWriteError("Open button was not found"&@CRLF)
	   EndIf
	   ControlClick("Select Active Database Filename...","","[CLASS:Button; INSTANCE:3]")
   EndIf

;~ Disassociate
WinMenuSelectItem("BUSMASTER","","&J1939", "&Database","&Dissociate")
sleep(1000)
   if WinExists("Dissociate Database")Then
	  ConsoleWrite("Checking the labels in Dissociate Database window"&@CRLF)
	  $Var1 = ControlGetText("Dissociate Database", "",2)
	   If $Var1="Cancel" Then
		  ConsoleWriteError("Cancel button was found"&@CRLF)
	   Else
		  ConsoleWriteError("Cancel button was not found")
	   EndIf
	   $Var2 = ControlGetText("Dissociate Database", "", 1285)
	   If $Var2="Dissociate" Then
		  ConsoleWriteError("Dissociate button was found"&@CRLF)
	   Else
		  ConsoleWriteError("Dissociate button was not found"&@CRLF)
	   EndIf
	   ControlClick("Dissociate Database","","[CLASS:Button; INSTANCE:2]")
   EndIf

   ;Msg window

   send("!jvm")		;uncheck
   sleep(1000)
   send("!jvm")		;check

   ; Go Online
   send("!jfg")

   ; Transmit window
	send("!jvt")
	sleep(2000)
	send("{ESC}")

	; signal watch window
	send("!jvs")
	winclose("Signal Watch - J1939")

	;~ Log
WinMenuSelectItem("BUSMASTER","","&J1939", "&Configure","&Log")
sleep(1000)
   if WinExists("Configure Logging for J1939")Then
	  ConsoleWrite("Checking the labels in Log window"&@CRLF)
	  $Var1 = ControlGetText("Configure Logging for J1939", "",2)
	   If $Var1="&Cancel" Then
		  ConsoleWriteError("Cancel button was found"&@CRLF)
	   Else
		  ConsoleWriteError("Cancel button was not found")
	   EndIf
	   $Var2 = ControlGetText("Configure Logging for J1939", "", 1)
	   If $Var2="&OK" Then
		  ConsoleWriteError("Dissociate button was found"&@CRLF)
	   Else
		  ConsoleWriteError("Dissociate button was not found"&@CRLF)
	   EndIf
	   ControlClick("Configure Logging for J1939","","[CLASS:Button; INSTANCE:14]")
   EndIf

;~ 	Time Outs.
WinMenuSelectItem("BUSMASTER","","&J1939", "&Configure","&Timeouts")
sleep(1000)
   if WinExists("Configure Timeouts")Then
	  ConsoleWrite("Checking the labels in Configure Timeouts window"&@CRLF)
	  $Var1 = ControlGetText("Configure Timeouts", "",2)
	   If $Var1="Cancel" Then
		  ConsoleWriteError("Cancel button was found"&@CRLF)
	   Else
		  ConsoleWriteError("Cancel button was not found")
	   EndIf
	   $Var2 = ControlGetText("Configure Timeouts", "", 1)
	   If $Var2="Apply" Then
		  ConsoleWriteError("Dissociate button was found"&@CRLF)
	   Else
		  ConsoleWriteError("Dissociate button was not found"&@CRLF)
	   EndIf
	   ControlClick("Configure Timeouts","","[CLASS:Button; INSTANCE:1]") ;apply
	   ControlClick("Configure Timeouts","","[CLASS:Button; INSTANCE:2]") ;Cancel
   EndIf

;Configure signal watch
send("!jcw")
sleep(1000)
if WinExists("BUSMASTER") Then
	ControlClick("BUSMASTER","","[CLASS:Button; INSTANCE:1]")
EndIf


;~ 	Message Display
WinMenuSelectItem("BUSMASTER","","&J1939", "&Configure","&Message Display")
sleep(1000)
   if WinExists("Configure Message Display - J1939")Then
	  ConsoleWrite("Checking the labels in Configure Timeouts window"&@CRLF)
	  $Var1 = ControlGetText("Configure Message Display - J1939", "",2)
	   If $Var1="Cancel" Then
		  ConsoleWriteError("Cancel button was found"&@CRLF)
	   Else
		  ConsoleWriteError("Cancel button was not found")
	   EndIf
	   $Var3 = ControlGetText("Configure Message Display - J1939", "", 1)
	   If $Var3="Help" Then
		  ConsoleWriteError("Help button was found"&@CRLF)
	   Else
		  ConsoleWriteError("Help button was not found"&@CRLF)
	   EndIf
	   $Var2 = ControlGetText("Configure Message Display - J1939", "", 1)
	   If $Var2="Apply" Then
		  ConsoleWriteError("Ok button was found"&@CRLF)
	   Else
		  ConsoleWriteError("Ok button was not found"&@CRLF)
	   EndIf
	   ControlClick("Configure Message Display - J1939","","[CLASS:Button; INSTANCE:4]")
   EndIf

	;~ 	Simulated Systems
WinMenuSelectItem("BUSMASTER","","&J1939", "&Configure","&Simulated Systems")
sleep(1000)
send("^{F4}")

  ; Go offline
   send("!jfg")

 ; Deactivate
 send("!jt")


