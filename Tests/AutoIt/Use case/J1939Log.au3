#Include <Excel.au3>
#Include <GuiStatusBar.au3>
#include <globalFunc.au3>
#Include <GuiComboBoxEx.au3>
#Include <GuiListView.au3>
#Include <GuiToolBar.au3>
#Include <GuiMenu.au3>

WinActivate("BUSMASTER",3)
if WinExists("BUSMASTER") then
	_loadConfig("ConfJ1939.cfx")
	_loadJ1939Database("J1939Test",25,10,8)
	send("!JDC")																; Close the database
	_associateDB("&J1939","J1939Test.dbf")
	$hWd=ControlGetHandle("BUSMASTER","",128)
	_GUICtrlToolbar_ClickIndex($hWd,4,"left")	; connect
	_act_dctJ1939("&Activate")
	_goOnline()
	WinMenuSelectItem("BUSMASTER","","&J1939","&Configure","&Log")
	if winexists("Configure Logging for J1939") Then
		ControlClick("Configure Logging for J1939","&Add",1303)
		sleep(1000)
;~ 		ControlCommand("Configure Logging for J1939","",1092,"Check")			; Check Enable/Disable Logging during tool Connect/Disconnect
		sleep(1000)
		ControlClick("Configure Logging for J1939","&Add",1303)
		sleep(1000)
		ControlCommand("Configure Logging for J1939","",1101,"SelectString","Relative")
		sleep(1000)
		ControlCommand("Configure Logging for J1939","",1081,"Check")				; select decimal mode
		sleep(2000)
		ControlCommand("Configure Logging for J1939","",1351,"Check")				; select overwrite
		sleep(2000)
		ControlClick("Configure Logging for J1939","&Add",1303)
		sleep(100)
		ControlCommand("Configure Logging for J1939","",1101,"SelectString","Absolute")
		sleep(1000)
		ControlCommand("Configure Logging for J1939","",1116,"Check")
		sleep(1000)
		ControlSetText("Configure Logging for J1939","",1095,32)
		ControlCommand("Configure Logging for J1939","",1117,"Check")
		sleep(500)
		ControlSetText("Configure Logging for J1939","",1096,37)
		sleep(1000)
		ControlCommand("Configure Logging for J1939","",1350,"Check")
		sleep(1000)
		ControlClick("Configure Logging for J1939","&OK",1)
	EndIf
	send("!JFL")			; start logging
	_J1939tMsgWin("Data")
	sleep(3000)
	$hWd=ControlGetHandle("BUSMASTER","",128)
	_GUICtrlToolbar_ClickIndex($hWd,4,"left")	; Disconnect
	sleep(1000)
	send("!JVM")			; close J1939 message window
	send("!FGS")			; save the config
EndIf