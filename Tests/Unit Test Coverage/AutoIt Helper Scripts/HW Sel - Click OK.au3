#cs ----------------------------------------------------------------------------

 AutoIt Version: 3.3.6.1
 Author:         myName

 Script Function:
	Template AutoIt script.

#ce ----------------------------------------------------------------------------
#Include <GuiListView.au3>

Sleep(1500)

; Script Start - Add your code below here
WinActivate("Hardware Selection")
if winexists("Hardware Selection") Then

	$CtrlHWSel=ControlGetHandle("Hardware Selection","",1226)
	; Select an item from configured channel list
	_GUICtrlListView_ClickItem($CtrlHWSel, 1, "left", False, 2)

	; Click on Remove button
	ControlClick("Hardware Selection","&Remove","[CLASS:Button; INSTANCE:3]")
	Sleep(500)
	ControlClick("Hardware Selection","&Select","[CLASS:Button; INSTANCE:2]")
	sleep(1000)
	ControlClick("Hardware Selection","&OK","[CLASS:Button; INSTANCE:4]")
	sleep(1000)
Endif


