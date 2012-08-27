#Include <Data_KIT.au3>
#Include <GuiListView.au3>
#Include <GuiTreeView.au3>
#Include <GuiEdit.au3>
#Include <GuiListBox.au3>
#Include <GuiComboBox.au3>
#include <Functions.au3>
if Winexists($mWin) Then
	sleep(1000)
	_loadConfig("saveChge")
	winclose($mWin)
	if WinExists("","You have made changes to the configuration.") Then
		ControlClick("","You have made changes to the configuration.","[CLASS:Button; INSTANCE:1]")
	EndIf
EndIf
_launchApp()
sleep(5000)
$res=0
$cfxSbar=StatusbarGetText($mWin,"",2)
$cfxFName=StringInStr($cfxSbar,"saveChge.cfx")
winclose($mWin)
if WinExists("","You have made changes to the configuration.") Then
	ControlClick("","You have made changes to the configuration.","[CLASS:Button; INSTANCE:1]")
	$res=-1
EndIf
FileDelete(@DesktopDir&"\saveChge.cfx")
ConsoleWrite("$cfxSbar : "&$cfxSbar&@CRLF)
ConsoleWrite("$res : "&$res&@CRLF)
if $cfxFName>0 and $res<>-1 Then
	msgbox(0,"","pass")
Else
	msgbox(0,"","fail")
EndIf