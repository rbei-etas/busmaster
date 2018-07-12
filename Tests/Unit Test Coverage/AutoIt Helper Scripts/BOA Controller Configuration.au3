#cs ----------------------------------------------------------------------------

 AutoIt Version: 3.3.6.1
 Author:         myName

 Script Function:
	Template AutoIt script.

#ce ----------------------------------------------------------------------------

; Script Start - Add your code below here

#Include <GuiListView.au3>

Sleep(2000)

WinActivate("Configure CAN Controller")
if WinExists("Configure CAN Controller") Then
	ControlSetText("Configure CAN Controller","",1000,250)
	ControlCommand("Configure CAN Controller","",1019,"SetCurrentSelection",1)
	sleep(500)
	ControlCommand("Configure CAN Controller","",3011,"SetCurrentSelection",3)
	sleep(500)
	ControlCommand("Configure CAN Controller","",1023,"SetCurrentSelection",2)
	sleep(500)
	$CANCtrlHWD=ControlGetHandle("Configure CAN Controller","",1246)
	_GUICtrlListView_ClickItem($CANCtrlHWD,1)
	sleep(1000)
	controlclick("Configure CAN Controller","",1060)
	sleep(2000)
endif
	