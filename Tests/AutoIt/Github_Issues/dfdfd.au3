#Include <Data_KIT.au3>
#Include <GuiListView.au3>
#Include <GuiTreeView.au3>
#Include <GuiMenu.au3>

if Winexists("BUSMASTER") Then
	$RES=StatusbarGetText("BUSMASTER","",5)
ConsoleWrite($RES&@CRLF)
$fndTxt=StringInStr($RES,"Vector XL")
ConsoleWrite($fndTxt&@CRLF)
EndIf

