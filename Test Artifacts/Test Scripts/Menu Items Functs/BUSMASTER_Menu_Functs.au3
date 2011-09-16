#include <GUIConstantsEx.au3>
#include <EditConstants.au3>
#include <WindowsConstants.au3>
#include <Array.au3>

; === Test Cases/Test Data ===
; Test Case Number:	TS_XX_001
; Feature:			Menu Item functionalities
; Critical (C):		Y
; Test Cases:		File->Configuration->New
; Test Strategy:	Black Box
; Test Data:		-

; === Test Procedure ===

MsgBox(0, "User Action-TS 1", "Run BUSMASTER Application and press OK.")

$SuccessCount = 0
$FileName   = "TS_XX"
$Number = "TS_XX_001"
$Result = "Test did not finish"
$file = FileOpen($FileName & ".txt", 2)

; --- Test SetUp ---
WinActivate("BUSMASTER","")

If (WinWaitActive("BUSMASTER","",5) ) Then
	
Else
	MsgBox(0, "GUI Event", "BUSMASTER not active")
	FileWriteLine($file,  "BUSMASTER application is not opened.")	
	Exit
EndIf
 
; 1. Select New Configuration
Send("!fgn") ; File->Configuration->New
Sleep(1000) ; 1 sec
if (WinActive("BUSMASTER", "Configuration File is modified")) Then
	Send("!y") ; Yes
EndIf

if WinWaitActive("New Configuration Filename...", "", 2) Then ; wait till its open
	Send("test.cfx") ; Filename
	Send("!s") ; Save
	if (WinWaitActive("New Configuration Filename...", "Do you want to replace it?", 2)) Then
		Send("!y") ; Yes
	EndIf
	$Result = "ok"
Else
	$Result = "Failed"
EndIf

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else		
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf

; === Test Cases/Test Data ===
; Test Case Number:	TS_XX_002
; Feature:			Menu Item functionalities
; Critical (C):		Y
; Test Cases:		File->Configuration->Load...
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
$Number = "TS_XX_002"
$Result = "Test did not finish"

; --- Test SetUp ---
WinActivate("BUSMASTER","")

; 1. Select Load Configuration
Send("!fgl") ; File->Configuration->Load...
Sleep(1000) ; 1 sec
if (WinActive("BUSMASTER", "Configuration File is modified")) Then	
	Send("!y") ; Yes	
EndIf

if WinWaitActive("Load Configuration Filename...", "", 2) Then ; wait till its open
	Send("test.cfx") ; Filename
	ControlClick("Load Configuration Filename...", "","Button2")
	$strCurrConfigFile = StatusbarGetText("BUSMASTER", "", 2)
	$blnResult = StringInStr( $strCurrConfigFile, "test.cfx")	
	if $blnResult > 0 Then
		$Result = "ok"
	Else
		$Result = "Failed"
	EndIf
Else
	$Result = "Failed"
EndIf

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else		
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf

; === Test Cases/Test Data ===
; Test Case Number:	TS_XX_003
; Feature:			Menu Item functionalities
; Critical (C):		Y
; Test Cases:		File->Configuration->Save
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
$Number = "TS_XX_003"
$Result = "Test did not finish"

WinActivate("BUSMASTER","")
; 1. Select Save Configuration
Send("!fgs") ; File->Configuration->Save
$Result = "ok"

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else		
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf

; === Test Cases/Test Data ===
; Test Case Number:	TS_XX_004
; Feature:			Menu Item functionalities
; Critical (C):		Y
; Test Cases:		File->Configuration->Save As...
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
$Number = "TS_XX_004"
$Result = "Test did not finish"

; --- Test SetUp ---
WinActivate("BUSMASTER","")

; 1. Select 'Save As...' menu option in Configuration
Send("!fga") ; File->Configuration->Save As...
Sleep(1000) ; 1 sec


if WinWaitActive("SaveAs Configuration Filename...", "", 2) Then ; wait till its open
	Send("test_new.cfx") ; Filename
	ControlClick("SaveAs Configuration Filename...", "Save","Button2")

	Sleep(500)
	if (WinActive("SaveAs Configuration Filename...","Do you want to replace it?")) Then			
			ControlClick("SaveAs Configuration Filename...","Do you want to replace it?","Button1")
	EndIf
	Sleep(250)
	$strCurrConfigFile = StatusbarGetText("BUSMASTER", "", 2)
	$blnResult = StringInStr( $strCurrConfigFile, "test_new.cfx")		
	if $blnResult > 0 Then
		$Result = "ok"		
	Else
		$Result = "Failed"
	EndIf
Else
	$Result = "Failed"
EndIf

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else		
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf

; === Test Cases/Test Data ===
; Test Case Number:	TS_XX_005
; Feature:			Menu Item functionalities
; Critical (C):		Y
; Test Cases:		File->Database->New
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
$Number = "TS_XX_005"
$Result = "Test did not finish"

; --- Test SetUp ---
WinActivate("BUSMASTER","")

; 1. Select 'New' menu option in Database
Send("!fdn") ; File->Database->New
Sleep(500) ; 0.5 sec


if WinWaitActive("Save As", "", 2) Then ; wait till its open
	$Result = "ok"		
	ControlClick("Save As", "Cancel", "Button3")	
Else
	$Result = "Failed"
EndIf

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else		
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf

; === Test Cases/Test Data ===
; Test Case Number:	TS_XX_006
; Feature:			Menu Item functionalities
; Critical (C):		Y
; Test Cases:		File->Database->Open...
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
$Number = "TS_XX_006"
$Result = "Test did not finish"

; --- Test SetUp ---
WinActivate("BUSMASTER","")

; 1. Select 'Open...' menu option in Database
Send("!fdo") ; File->Database->Open...
Sleep(500) ; 0.5 sec


if WinWaitActive("Select BUSMASTER Database Filename...", "", 2) Then ; wait till its open
	$Result = "ok"		
	Send("d:\BUSMASTER\ConfigFiles\New_Database1.DBF")
	Sleep(250)
	ControlClick("Select BUSMASTER Database Filename...", "","Button2")	
Else
	$Result = "Failed"
EndIf

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else		
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf

; === Test Cases/Test Data ===
; Test Case Number:	TS_XX_007
; Feature:			Menu Item functionalities
; Critical (C):		Y
; Test Cases:		File->Database->Save
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
$Number = "TS_XX_007"
$Result = "Test did not finish"

; --- Test SetUp ---
WinActivate("BUSMASTER","")

WinActivate("BUSMASTER","")
Send("!fds") ; File -> Database -> Save
$Result = "ok"

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else		
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf

; === Test Cases/Test Data ===
; Test Case Number:	TS_XX_008
; Feature:			Menu Item functionalities
; Critical (C):		Y
; Test Cases:		File->Database->Save and Import
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
$Number = "TS_XX_008"
$Result = "Test did not finish"

; --- Test SetUp ---
WinActivate("BUSMASTER","")

WinActivate("BUSMASTER","")
Send("!fdp") ; File -> Database -> Save and Import
$Result = "ok"

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else		
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf

; === Test Cases/Test Data ===
; Test Case Number:	TS_XX_009
; Feature:			Menu Item functionalities
; Critical (C):		Y
; Test Cases:		File->Database->Disassociate
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
$Number = "TS_XX_009"
$Result = "Test did not finish"

; --- Test SetUp ---
WinActivate("BUSMASTER","")
Send("!fdd") ; File -> Database -> Disassociate

if WinWaitActive("Dissociate Database", "", 2) Then ; wait till it opens for 2 seconds
	$Result = "ok"
	ControlClick("Dissociate Database", "","Button2")
Else
	$Result = "Failed"
EndIf

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else		
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf

; === Test Cases/Test Data ===
; Test Case Number:	TS_XX_010
; Feature:			Menu Item functionalities
; Critical (C):		Y
; Test Cases:		File->Database->Save As...
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
$Number = "TS_XX_010"
$Result = "Test did not finish"

; --- Test SetUp ---
WinActivate("BUSMASTER","")

Send("!fdv") ; File -> Database -> Save As...

if WinWaitActive("Save As...", "", 2) Then ; wait till it opens for 2 seconds
	$Result = "ok"
	ControlClick("Save As...", "Cancel","Button3")
Else
	$Result = "Failed"
EndIf

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else		
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf

; === Test Cases/Test Data ===
; Test Case Number:	TS_XX_011
; Feature:			Menu Item functionalities
; Critical (C):		Y
; Test Cases:		File->Database->Close
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
$Number = "TS_XX_011"
$Result = "Test did not finish"

; --- Test SetUp ---
WinActivate("BUSMASTER","")

if WinWaitActive("BUSMASTER", "DatabaseEditor - CAN", 2) Then ; wait till it opens for 2 seconds	
	Send("!fdc") ; File -> Database -> Close
	$Result = "ok"	
Else
	$Result = "Failed"
EndIf

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else		
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf

; === Test Cases/Test Data ===
; Test Case Number:	TS_XX_012
; Feature:			Menu Item functionalities
; Critical (C):		Y
; Test Cases:		File->Database->Associate
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
$Number = "TS_XX_012"
$Result = "Test did not finish"

; --- Test SetUp ---
WinActivate("BUSMASTER","")

; 3. Associate database
Send("!fda") ; File -> Database -> Associate...

if WinWaitActive("Select Active Database Filename...", "", 2) Then ; wait till it opens for 2 seconds		
	$Result = "ok"	
	ControlClick("Select Active Database Filename...", "Cancel","Button3")
Else
	$Result = "Failed"
EndIf

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else		
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf

; === Test Cases/Test Data ===
; Test Case Number:	TS_XX_013
; Feature:			Menu Item functionalities
; Critical (C):		Y
; Test Cases:		File->Properties
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
$Number = "TS_XX_013"
$Result = "Test did not finish"

; --- Test SetUp ---
WinActivate("BUSMASTER","")

; Select Properties
Send("!fp") ; File -> Properties

if WinWaitActive("Properties", "", 2) Then ; wait till it opens for 2 seconds		
	$Result = "ok"	
	WinClose("Properties")
Else
	$Result = "Failed"
EndIf

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else		
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf

; === Test Cases/Test Data ===
; Test Case Number:	TS_XX_014
; Feature:			Menu Item functionalities
; Critical (C):		Y
; Test Cases:		View->Message Window
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
$Number = "TS_XX_014"
$Result = "Test did not finish"

; --- Test SetUp ---
WinActivate("BUSMASTER","")

; Hide message window
Send("!vm") ; View -> Message Window

; Should be in hidden state
if ControlCommand("BUSMASTER", "Message Window - CAN", "", "IsVisible") =  0 Then 
	; Show message window
	Sleep(200)
	Send("!vm") ; View -> Message Window
	
	; Should be in shown state
	if ControlCommand("BUSMASTER", "Message Window - CAN", "", "IsVisible") =  1 Then
		$Result = "ok"
	Else
		$Result = "Failed"
	EndIf
Else
	$Result = "Failed"
EndIf

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else		
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf

; === Test Cases/Test Data ===
; Test Case Number:	TS_XX_015
; Feature:			Menu Item functionalities
; Critical (C):		Y
; Test Cases:		View->Signal Watch Window
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
$Number = "TS_XX_015"
$Result = "Test did not finish"

; --- Test SetUp ---
WinActivate("BUSMASTER","")

Send("!vw") ; View -> Signal Watch Window

; Should be in shown state
if ControlCommand("Signal Watch - CAN", "", "", "IsVisible") =  1 Then 
	; Show message window
	Sleep(200)
	WinActivate("BUSMASTER","")
	Send("!vw") ; View -> Signal Watch Window
	
	; Should be in hidden state
	if ControlCommand("Signal Watch - CAN",  "", "", "IsVisible") =  0 Then
		$Result = "ok"
	Else
		$Result = "Failed"
	EndIf
Else
	$Result = "Failed"
EndIf

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else		
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf

; === Test Cases/Test Data ===
; Test Case Number:	TS_XX_016
; Feature:			Menu Item functionalities
; Critical (C):		Y
; Test Cases:		View->Network Statistics
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
$Number = "TS_XX_016"
$Result = "Test did not finish"

; --- Test SetUp ---
WinActivate("BUSMASTER","")

Send("!vn") ; View -> Network Statistics

; Should be in shown state
if ControlCommand("Network Statistics", "", "", "IsVisible") =  1 Then 
	; Show message window
	Sleep(200)
	WinActivate("BUSMASTER","")
	Send("!vn") ; View -> Network Statistics
	
	; Should be in hidden state
	if ControlCommand("Network Statistics",  "", "", "IsVisible") =  0 Then
		$Result = "ok"
	Else
		$Result = "Failed"
	EndIf
Else
	$Result = "Failed"
EndIf

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else		
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf

; === Test Cases/Test Data ===
; Test Case Number:	TS_XX_017
; Feature:			Menu Item functionalities
; Critical (C):		Y
; Test Cases:		View->Trace Window
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
$Number = "TS_XX_017"
$Result = "Test did not finish"

; --- Test SetUp ---
WinActivate("BUSMASTER","")

Send("!vr") ; View -> Trace Window

; Should be in shown state
if ControlCommand("BUSMASTER", "Trace Window", "", "IsVisible") =  1 Then 
	; Show message window
	Sleep(200)
	WinActivate("BUSMASTER","")
	Send("!vr") ; View -> Trace Window
	Sleep(200)
	; Should be in hidden state
	if ControlCommand("BUSMASTER",  "Trace Window", "", "IsVisible") =  0 Then
		$Result = "ok"
	Else
		$Result = "Failed"
	EndIf
Else
	$Result = "Failed"
EndIf

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else		
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf

; === Test Cases/Test Data ===
; Test Case Number:	TS_XX_018
; Feature:			Menu Item functionalities
; Critical (C):		Y
; Test Cases:		View->Toolbar->Main
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
$Number = "TS_XX_018"
$Result = "Test did not finish"

; --- Test SetUp ---
WinActivate("BUSMASTER","")

Send("!vtm") ; View -> Toolbar -> Main

; Should be in shown state
if ControlCommand("BUSMASTER", "Main", 128, "IsVisible") =  0 Then 
	; Show message window
	Sleep(200)
	WinActivate("BUSMASTER","")
	Send("!vtm") ; View -> Toolbar -> Main
	
	; Should be in hidden state
	if ControlCommand("BUSMASTER", "Main", 128, "IsVisible") =  1 Then 
		$Result = "ok"
	Else
		$Result = "Failed"
	EndIf
Else
	$Result = "Failed"
EndIf

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else		
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf

; === Test Cases/Test Data ===
; Test Case Number:	TS_XX_019
; Feature:			Menu Item functionalities
; Critical (C):		Y
; Test Cases:		View->Toolbar->Message Window
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
$Number = "TS_XX_019"
$Result = "Test did not finish"

; --- Test SetUp ---
WinActivate("BUSMASTER","")

Send("!vtw") ; View -> Toolbar -> Message Window

; Should be in shown state
if ControlCommand("BUSMASTER", "Message Window", 314, "IsVisible") =  0 Then 
	; Show message window
	Sleep(200)
	WinActivate("BUSMASTER","")
	Send("!vtw") ; View -> Toolbar -> Message Window
	
	; Should be in hidden state
	if ControlCommand("BUSMASTER", "Message Window", 314, "IsVisible") =  1 Then 
		$Result = "ok"
	Else
		$Result = "Failed"
	EndIf
Else
	$Result = "Failed"
EndIf

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else		
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf

; === Test Cases/Test Data ===
; Test Case Number:	TS_XX_020
; Feature:			Menu Item functionalities
; Critical (C):		Y
; Test Cases:		View->Toolbar->Node Simulation
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
$Number = "TS_XX_020"
$Result = "Test did not finish"

; --- Test SetUp ---
WinActivate("BUSMASTER","")

Send("!vtn") ; View -> Toolbar -> Node Simulation

; Should be in shown state
if ControlCommand("BUSMASTER", "Node Simulation", 238, "IsVisible") =  0 Then 
	; Show message window
	Sleep(200)
	WinActivate("BUSMASTER","")
	Send("!vtn") ; View -> Toolbar -> Node Simulation
	
	; Should be in hidden state
	if ControlCommand("BUSMASTER", "Node Simulation", 238, "IsVisible") =  1 Then 
		$Result = "ok"
	Else
		$Result = "Failed"
	EndIf
Else
	$Result = "Failed"
EndIf

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else		
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf

; === Test Cases/Test Data ===
; Test Case Number:	TS_XX_021
; Feature:			Menu Item functionalities
; Critical (C):		Y
; Test Cases:		View->Toolbar->Configure
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
$Number = "TS_XX_021"
$Result = "Test did not finish"

; --- Test SetUp ---
WinActivate("BUSMASTER","")

Send("!vtc") ; View -> Toolbar -> Configure

; Should be in shown state
if ControlCommand("BUSMASTER", "Configure", 276, "IsVisible") =  0 Then 
	; Show message window
	Sleep(200)
	WinActivate("BUSMASTER","")
	Send("!vtc") ; View -> Toolbar -> Configure
	
	; Should be in hidden state
	if ControlCommand("BUSMASTER", "Configure", 276, "IsVisible") =  1 Then 
		$Result = "ok"
	Else
		$Result = "Failed"
	EndIf
Else
	$Result = "Failed"
EndIf

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else		
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf

; === Test Cases/Test Data ===
; Test Case Number:	TS_XX_022
; Feature:			Menu Item functionalities
; Critical (C):		Y
; Test Cases:		View->Toolbar->CAN Database
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
$Number = "TS_XX_022"
$Result = "Test did not finish"

; --- Test SetUp ---
WinActivate("BUSMASTER","")

Send("!vtd") ; View -> Toolbar -> CAN Database

; Should be in shown state
if ControlCommand("BUSMASTER", "CAN Database", 291, "IsVisible") =  0 Then 
	; Show message window
	Sleep(200)
	WinActivate("BUSMASTER","")
	Send("!vtd") ; View -> Toolbar -> Configure
	
	; Should be in hidden state
	if ControlCommand("BUSMASTER", "CAN Database", 291, "IsVisible") =  1 Then 
		$Result = "ok"
	Else
		$Result = "Failed"
	EndIf
Else
	$Result = "Failed"
EndIf

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else		
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf

; === Test Cases/Test Data ===
; Test Case Number:	TS_XX_023
; Feature:			Menu Item functionalities
; Critical (C):		Y
; Test Cases:		View->Statusbar
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
$Number = "TS_XX_023"
$Result = "Test did not finish"

; --- Test SetUp ---
WinActivate("BUSMASTER","")

Send("!vs") ; View -> Statusbar

Sleep(100)
; Should be in hidden state
if ControlCommand("BUSMASTER", "", 59393, "IsVisible") = 0 Then 
	; Show message window
	
	WinActivate("BUSMASTER","")
	Send("!vs") ; View -> Statusbar
	Sleep(100)
	; Should be in shown state
	if ControlCommand("BUSMASTER", "", 59393, "IsVisible") = 1 Then		
		$Result = "ok"
	Else
		$Result = "Failed"
	EndIf
Else
	$Result = "Failed"
EndIf

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else		
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf

; === Test Cases/Test Data ===
; Test Case Number:	TS_XX_024
; Feature:			Menu Item functionalities
; Critical (C):		Y
; Test Cases:		Configure->Message Display
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
$Number = "TS_XX_024"
$Result = "Test did not finish"

; --- Test SetUp ---
WinActivate("BUSMASTER","")

Send("!cm") ; Configure -> Message Display

Sleep(100)
; Should be in shown state
if ControlCommand("Configure Message Display- CAN", "","", "IsVisible") = 1 Then 
	$Result = "ok"
	WinClose("Configure Message Display- CAN")
Else
	$Result = "Failed"
EndIf

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else		
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf

; === Test Cases/Test Data ===
; Test Case Number:	TS_XX_025
; Feature:			Menu Item functionalities
; Critical (C):		Y
; Test Cases:		Configure->Log
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
$Number = "TS_XX_025"
$Result = "Test did not finish"

; --- Test SetUp ---
WinActivate("BUSMASTER","")

Send("!co") ; Configure -> Log

Sleep(100)
; Should be in shown state
if ControlCommand("Configure Logging for CAN", "","", "IsVisible") = 1 Then 
	$Result = "ok"
	WinActivate("Configure Logging for CAN","")
	ControlClick("Configure Logging for CAN", "Add", 1303)
	ControlClick("Configure Logging for CAN", "OK",  1)
Else
	$Result = "Failed"
EndIf

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else		
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf

; === Test Cases/Test Data ===
; Test Case Number:	TS_XX_026
; Feature:			Menu Item functionalities
; Critical (C):		Y
; Test Cases:		Configure->Tx Messages
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
$Number = "TS_XX_026"
$Result = "Test did not finish"

; --- Test SetUp ---
WinActivate("BUSMASTER","")

Send("!ct") ; Configure -> Tx Messages

Sleep(100)
; Should be in shown state
if ControlCommand("BUSMASTER", "Configure Transmission Messages","", "IsVisible") = 1 Then 
	$Result = "ok"
	ControlClick("BUSMASTER","Configure Transmission Messages", 1015) ;1015 is the ID for 'Close' button in 'Configure Transmission Messages' Dialog.
Else
	$Result = "Failed"
EndIf

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else		
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf

; === Test Cases/Test Data ===
; Test Case Number:	TS_XX_027
; Feature:			Menu Item functionalities
; Critical (C):		Y
; Test Cases:		Configure->Replay
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
$Number = "TS_XX_027"
$Result = "Test did not finish"

; --- Test SetUp ---
WinActivate("BUSMASTER","")

Send("!cr") ; Configure -> Replay

Sleep(50)
; Should be in shown state
if ControlCommand("Replay Configuration", "","", "IsVisible") = 1 Then 
	$Result = "ok"
	WinClose("Replay Configuration")
Else
	$Result = "Failed"
EndIf

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else		
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf

; === Test Cases/Test Data ===
; Test Case Number:	TS_XX_028
; Feature:			Menu Item functionalities
; Critical (C):		Y
; Test Cases:		Configure->App Filters
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
$Number = "TS_XX_028"
$Result = "Test did not finish"

; --- Test SetUp ---
WinActivate("BUSMASTER","")

Send("!ca") ; Configure -> App Filters

Sleep(50)
; Should be in shown state
if ControlCommand("Configure Filter List", "","", "IsVisible") = 1 Then 
	$Result = "ok"
	WinClose("Configure Filter List")
Else
	$Result = "Failed"
EndIf

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else		
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf

; === Test Cases/Test Data ===
; Test Case Number:	TS_XX_029
; Feature:			Menu Item functionalities
; Critical (C):		Y
; Test Cases:		Configure->Simulated Systems
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
$Number = "TS_XX_029"
$Result = "Test did not finish"

; --- Test SetUp ---
WinActivate("BUSMASTER","")

Send("!cs") ; Configure -> Simulated Systems

; Should be in shown state
if MsgBox(1, "Verify Test Case: TS- 29", "If Simulated Systems window appears, close it and press 'OK' else press 'Cancel'.") = 1 Then 
	$Result = "ok"	
Else	; Cancel selected
	$Result = "Failed"
EndIf

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else		
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf

; === Test Cases/Test Data ===
; Test Case Number:	TS_XX_030
; Feature:			Menu Item functionalities
; Critical (C):		Y
; Test Cases:		Configure->Signal Watch
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
$Number = "TS_XX_030"
$Result = "Test did not finish"

; --- Test SetUp ---
WinActivate("BUSMASTER","")

Send("!cn") ; Configure -> Signal Watch

Sleep(50)
; Should be in shown state
if ControlCommand("Signal Watch List ", "","", "IsVisible") = 1 Then 
	$Result = "ok"
	WinClose("Signal Watch List ")
Else
	$Result = "Failed"
EndIf

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else		
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf

; === Test Cases/Test Data ===
; Test Case Number:	TS_XX_031
; Feature:			Menu Item functionalities
; Critical (C):		Y
; Test Cases:		Functions -> Connect
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
$Number = "TS_XX_031"
$Result = "Test did not finish"

; --- Test SetUp ---
WinActivate("BUSMASTER","")

; Connect 
Send("!uc")	; Functions -> Connect

$Result = "ok"

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else		
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf

; === Test Cases/Test Data ===
; Test Case Number:	TS_XX_032
; Feature:			Menu Item functionalities
; Critical (C):		Y
; Test Cases:		Functions -> Disconnect
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
$Number = "TS_XX_032"
$Result = "Test did not finish"

; --- Test SetUp ---
WinActivate("BUSMASTER","")

; Disconnect
Send("!ui")	; Functions -> Disconnect
$Result = "ok"

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else		
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf

; === Test Cases/Test Data ===
; Test Case Number:	TS_XX_033
; Feature:			Menu Item functionalities
; Critical (C):		Y
; Test Cases:		Functions -> Start Logging
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
$Number = "TS_XX_033"
$Result = "Test did not finish"

; Enable Start Logging
Send("!ul")	; Functions -> Start Logging
Sleep(500)
; Disable Start Logging
Send("!ul")	; Functions -> Start Logging
$Result = "ok"

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else		
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf

; === Test Cases/Test Data ===
; Test Case Number:	TS_XX_034
; Feature:			Menu Item functionalities
; Critical (C):		Y
; Test Cases:		Functions -> Transmit -> Normal Blocks
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
$Number = "TS_XX_034"
$Result = "Test did not finish"

; --- Test SetUp ---
WinActivate("BUSMASTER","")

; Add messages to transmission window.
WinWaitActive("BUSMASTER","",5) 
Send("!ct") ; Configure->Tx Messages.
WinWaitActive("BUSMASTER","Configure Transmission Messages",5) 
ControlClick("BUSMASTER","Configure Transmission Messages", 1226) ;1226 is the ID for 'Add' button in 'Configure Transmission Messages' Dialog.

; Add Hex messages ox12(18) and 0x13(19) to message list
ControlSetText("BUSMASTER","Configure Transmission Messages", 1001, "18")	;1001 is the ID for "Name / ID" ComboBox in 'Configure Transmission Messages' Dialog.
ControlClick("BUSMASTER","Configure Transmission Messages", 1122) ;1122 is the ID for 'Add Message' button in 'Configure Transmission Messages' Dialog.

ControlSetText("BUSMASTER","Configure Transmission Messages", 1001, "19")	;1001 is the ID for "Name / ID" ComboBox in 'Configure Transmission Messages' Dialog.
ControlClick("BUSMASTER","Configure Transmission Messages", 1122) ;1122 is the ID for 'Add Message' button in 'Configure Transmission Messages' Dialog.
; Click on Update button.
ControlClick("BUSMASTER","Configure Transmission Messages", 1014) ;1014 is the ID for 'Update' button in 'Configure Transmission Messages' Dialog.
ControlClick("BUSMASTER","Configure Transmission Messages", 1015) ;1015 is the ID for 'Close' button in 'Configure Transmission Messages' Dialog.

; Connect 
Send("!uc")	; Functions -> Connect

WinActivate("BUSMASTER","")
;6. Send some messages across CAN bus, which shall contain a few messages from the filter list"

Send("!utn")	; Functions -> Transmit -> Normal Blocks
Sleep(1000)	;Sleep for 1 second

; Stop message transmission
Send("!utn")	; Functions -> Transmit -> Normal Blocks
; Disconnect 
Send("!ui")	; Functions -> Disconnect

; Message window should mcontain essages which are configured in Tx Window.
If ControlListView("BUSMASTER", "Message Window - CAN", 200, "GetItemCount") > 0 Then 
	$Result = "ok"		
Else
	$Result = "Failed"	
EndIf	

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else		
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf

; === Test Cases/Test Data ===
; Test Case Number:	TS_XX_035
; Feature:			Menu Item functionalities
; Critical (C):		Y
; Test Cases:		Functions -> Enable Filter -> Log
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
$Number = "TS_XX_035"
$Result = "Test did not finish"

; --- Test SetUp ---
WinActivate("BUSMASTER","")

; Enable Filter for Logging
Send("!ufl")	; Functions -> Enable Filter -> Log
Sleep(500)
; Disable Filter for Logging
Send("!ufl")	; Functions -> Enable Filter -> Log
$Result = "ok"	

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else		
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf

; === Test Cases/Test Data ===
; Test Case Number:	TS_XX_036
; Feature:			Menu Item functionalities
; Critical (C):		Y
; Test Cases:		Functions -> Enable Filter -> Display
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
$Number = "TS_XX_036"
$Result = "Test did not finish"

; --- Test SetUp ---
WinActivate("BUSMASTER","")

; Enable Filter for Logging
Send("!ufd")	; Functions -> Enable Filter -> Display
Sleep(500)
; Disable Filter for Logging
Send("!ufd")	; Functions -> Enable Filter -> Display
$Result = "ok"	

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else		
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf

; === Test Cases/Test Data ===
; Test Case Number:	TS_XX_037
; Feature:			Menu Item functionalities
; Critical (C):		Y
; Test Cases:		Functions -> Test Automation -> Editor
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
$Number = "TS_XX_037"
$Result = "Test did not finish"

; --- Test SetUp ---
WinActivate("BUSMASTER","")

; Enable Filter for Logging
Send("!uae")	; Functions -> Test Automation -> Editor

; Should be in shown state
if MsgBox(1, "Verify Test Case: TS- 37", "If Test Automation Editor window appears, close it and press 'OK' else press 'Cancel'.") = 1 Then 
	$Result = "ok"	
Else	; Cancel selected
	$Result = "Failed"
EndIf

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else		
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf

; === Test Cases/Test Data ===
; Test Case Number:	TS_XX_038
; Feature:			Menu Item functionalities
; Critical (C):		Y
; Test Cases:		Functions -> Test Automation -> Executor
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
$Number = "TS_XX_038"
$Result = "Test did not finish"

; --- Test SetUp ---
WinActivate("BUSMASTER","")

; Enable Filter for Logging
Send("!uax")	; Functions -> Test Automation -> Executor

; Should be in shown state
if MsgBox(1, "Verify Test Case: TS- 38", "If Test Suite Executor window appears, close it and press 'OK' else press 'Cancel'.") = 1 Then 
	$Result = "ok"	
Else	; Cancel selected
	$Result = "Failed"
EndIf

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else		
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf

; === Test Cases/Test Data ===
; Test Case Number:	TS_XX_039
; Feature:			Menu Item functionalities
; Critical (C):		Y
; Test Cases:		Functions -> Message Window-> Overwrite
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
$Number = "TS_XX_039"
$Result = "Test did not finish"

; --- Test SetUp ---
WinActivate("BUSMASTER","")

; Disable overwrite mode
Send("!umo")	; Functions -> Message Window-> Overwrite
Sleep(500)
; Enable overwrite mode
Send("!umo")	; Functions -> Message Window-> Overwrite
$Result = "ok"	

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else		
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf

; === Test Cases/Test Data ===
; Test Case Number:	TS_XX_040
; Feature:			Menu Item functionalities
; Critical (C):		Y
; Test Cases:		Functions -> Message Window-> Time Mode -> Absolute Time
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
$Number = "TS_XX_040"
$Result = "Test did not finish"

; --- Test SetUp ---
WinActivate("BUSMASTER","")

; Set Time mode as 'Absolute Time'
Send("!umma")	; Functions -> Message Window-> Time Mode -> Absolute Time
Sleep(200)
$Result = "ok"	

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else		
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf

; === Test Cases/Test Data ===
; Test Case Number:	TS_XX_041
; Feature:			Menu Item functionalities
; Critical (C):		Y
; Test Cases:		Functions -> Message Window-> Time Mode -> Relative Time
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
$Number = "TS_XX_041"
$Result = "Test did not finish"

; --- Test SetUp ---
WinActivate("BUSMASTER","")

; Set Time mode as 'Relative Time'
Send("!ummr")	; Functions -> Message Window-> Time Mode -> Relative Time
Sleep(200)
$Result = "ok"	

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else		
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf

; === Test Cases/Test Data ===
; Test Case Number:	TS_XX_042
; Feature:			Menu Item functionalities
; Critical (C):		Y
; Test Cases:		Functions -> Message Window-> Time Mode -> System Time
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
$Number = "TS_XX_042"
$Result = "Test did not finish"

; --- Test SetUp ---
WinActivate("BUSMASTER","")

; Set Time mode as 'System Time'
Send("!umms")	; Functions -> Message Window-> Time Mode -> System Time
Sleep(200)
$Result = "ok"	

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else		
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf

; === Test Cases/Test Data ===
; Test Case Number:	TS_XX_043
; Feature:			Menu Item functionalities
; Critical (C):		Y
; Test Cases:		Functions -> Message Window-> Interpret
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
$Number = "TS_XX_043"
$Result = "Test did not finish"

; --- Test SetUp ---
WinActivate("BUSMASTER","")

; Enable 'Interpret' mode
Send("!umi")	; Functions -> Message Window-> Interpret
Sleep(200)
; Disable 'Interpret' mode
Send("!umi")	; Functions -> Message Window-> Interpret
$Result = "ok"	

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else		
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf

; === Test Cases/Test Data ===
; Test Case Number:	TS_XX_044
; Feature:			Menu Item functionalities
; Critical (C):		Y
; Test Cases:		Functions -> Message Window-> Reset columns
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
$Number = "TS_XX_044"
$Result = "Test did not finish"

; --- Test SetUp ---

MsgBox(0, "User action needed", "Change the order of columns in Message Window and press 'OK'.")
WinActivate("BUSMASTER","")
Send("!umr")	; Functions -> Message Window-> Reset columns

if MsgBox(1, "Verify Test Case: TS- 44", "If Columns retain their default positions, click 'OK' else 'Cancel'.") = 1 Then 
	$Result = "ok"	
Else	; Cancel selected
	$Result = "Failed"
EndIf

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else		
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf

; === Test Cases/Test Data ===
; Test Case Number:	TS_XX_045
; Feature:			Menu Item functionalities
; Critical (C):		Y
; Test Cases:		Help -> About
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
$Number = "TS_XX_045"
$Result = "Test did not finish"

; --- Test SetUp ---

WinActivate("BUSMASTER","")
Send("!ha")	; Help -> About
Sleep(200)
; About dialog should be in shown state
if ControlCommand("About", "","", "IsVisible") = 1 Then 
	$Result = "ok"
	WinClose("About")
Else
	$Result = "Failed"
EndIf

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else		
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf

; --- Write Summary ---
FileWriteLine($file,  $SuccessCount& " out of " & "45 test cases successful.")	
FileClose($file)

; --- Show Summary ---
$strRunPath = "notepad.exe " & @ScriptDir & "\" & $FileName & ".txt"
Run($strRunPath)

; --- Closure ---
;WinClose("BUSMASTER")
;ControlClick("BUSMASTER", "", 7)	;7 is the ID for 'No' button


;End of Script
