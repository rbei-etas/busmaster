; === Test Cases/Test Data ===
; Test Case Number:	TS_20_001
; Feature:			Filter
; Critical (C):		Y
; Test Cases:		Invoking Filter Dialog
; Test Strategy:	Black Box
; Test Data:		-

; === Test Procedure ===
$Number = "TS_20_001"
$Result = "Test did not finish"

GUICreate("BUSMASTER App Browse", 420, 100)
GUICtrlCreateLabel("Please Browse the BUSMASTER Application Executable.", 30, 10)
$h_fileIn = GUICtrlCreateEdit("", 30, 30, 300, 20, BitOR($ES_WANTRETURN, $WS_HSCROLL, $ES_AUTOHSCROLL))
$okbutton1 = GUICtrlCreateButton("Browse", 330, 30, 60,20)
$okbutton2 = GUICtrlCreateButton("Go------>", 140, 60, 60)
;GUISetState(@SW_SHOW)

$initialDir = "C:\"

While 0
  $msg = GUIGetMsg()
  Select
    Case $msg = $okbutton1
      $filepath = FileOpenDialog("Select BUSMASTER executable.", $initialDir, "Exe files (*.exe)", 1)
	  $initialDir = StringTrimRight($filepath, StringInStr($filepath, "\", "-1"))
	  GUICtrlSetData($h_fileIn, $filepath)	  
	  
    Case $msg = $okbutton2
		; 1. Start the application		
		Run($filepath)      			
		; Close the GUI
		GUIDelete(); 
		ExitLoop
		
    Case $msg = $GUI_EVENT_CLOSE
      MsgBox(0, "GUI Event", "You clicked CLOSE! Script is not executed.")
      Exit	  
  EndSelect
WEnd

MsgBox(0, "User Action-TS 1", "Run BUSMASTER Application and press OK.")
; --- Test SetUp ---
WinActivate("BUSMASTER","")

If (WinWaitActive("BUSMASTER","",5) ) Then
	
Else
	MsgBox(0, "GUI Event", "BUSMASTER not active")
	FileWriteLine($file,  "BUSMASTER application is not opened.")	
	Exit
EndIf
 
WinActivate("BUSMASTER","")
; 2. Select New Configuration
Send("!fgn") ; File->Configuration->New
Sleep(1000) ; 1 sec
if (WinActive("BUSMASTER", "Configuration File is modified")) Then
	Send("!y") ; Yes
EndIf
WinWaitActive("New Configuration Filename...", "", 2) ; wait till its open
Send("test.cfx") ; Filename
Send("!s") ; Save
if (WinWaitActive("New Configuration Filename...", "Do you want to replace it?", 2)) Then
	Send("!y") ; Yes
EndIf

WinActivate("BUSMASTER","")
; 3. Associate database
;Send("!fda") ; File -> Database -> Associate...

;WinWaitActive("Select Active Database Filename...", "BUSMASTER Datatbase File(*.dbf)", 2) ; wait till its open
;WinWaitActive("BUSMASTER","")

WinActivate("BUSMASTER","")
Send("!fda") ; File -> Database -> Associate...

WinWaitActive("Select Active Database Filename...", "BUSMASTER Datatbase File(*.dbf)", 2) ; wait till its open
Send("d:\BUSMASTER\Tests\AutoIt\Common\test.dbf")
Sleep(1000)
ControlClick("Select Active Database Filename...", "","Button2")

WinActivate("BUSMASTER","")
; 4. Select Configuration -> App Filters.
Send("!ca") ; Configure->App Filters
; 5. Check for Filter Configuration dialog
WinWaitActive("Configure Filter List", "")
; --- Expected Results ---
; 1. A dialog with "Configure Filter list" will be shown
If WinActive("Configure Filter List", "") Then
	$Result = "ok"
Else
	$Result = "failed"
EndIf
; --- Finish ---
;WinClose("Configure Filter List")
;Send("!n") ; Save: No
; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else		
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf
