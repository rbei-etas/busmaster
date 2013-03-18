#include <GuiTreeView.au3>
#include <GuiListView.au3>
#include <globalFunc4Module.au3>
#include <GuiToolBar.au3>
#include <GuiListBox.au3>
#include <GuiMenu.au3>
#include <Data_Declaration.au3>
#include <File.au3>
#include <Date.au3>

winactivate("BUSMASTER")

if winexists($mWin) then
	WinMenuSelectItem($mWin,"",$HelpMenu,$AboutBusmaster)
	$Ver=controlgettext($DlgAboutBM,"",$BMVersionStaticctrlID)
	$ToolVer=StringSplit($Ver," ")
	ConsoleWrite("$ToolVer :" &$ToolVer[2]&@CRLF)
	$GetLogFile_Path=_SetOneFolderUp()												; Fetch the Log file path
	ConsoleWrite("$GetLogFile_Path:"&$GetLogFile_Path&@CRLF)
	$VersionFromLog=FileReadLine ($GetLogFile_Path & "\Log_04_Sample.log",1)				; Read the 1st line from the Log file
	consolewrite("$VersionFromLog :"&$VersionFromLog&@CRLF)
	$Version=StringSplit($VersionFromLog," ")
	consolewrite("$Version:"&$Version[3]&@CRLF)
	$VersionNo=StringTrimRight($Version[3],3)										; Remove last three characters.
	consolewrite("$VersionNo:"&$VersionNo&@CRLF)
	$Protocol=FileReadLine ($GetLogFile_Path & "\Log_04_Sample.log",2)				; Read the 2nd line from the Log file
	$NoEdit=FileReadLine ($GetLogFile_Path & "\Log_04_Sample.log",3)				; Read the 3rd line from the Log file
	$StartLog=FileReadLine ($GetLogFile_Path & "\Log_04_Sample.log",4)				; Read the 4rd line from the Log file
	$StartDateTime=FileReadLine ($GetLogFile_Path & "\Log_04_Sample.log",5)				; Read the 5rd line from the Log file
	$DateTime=StringSplit($StartDateTime," ")
	$PCDate=_NowDate()
	if $DateTime[1]="***START" and $DateTime[2]="DATE" and $DateTime[3]="AND" and $DateTime[4]="TIME" then
		$StartDateTimeInLog="Match"
	Else
		$StartDateTimeInLog="No Match"
	EndIf
	$NumericMode=FileReadLine ($GetLogFile_Path & "\Log_04_Sample.log",6)				; Read the 6rd line from the Log file
	$TimeMode=FileReadLine ($GetLogFile_Path & "\Log_04_Sample.log",7)				; Read the 7rd line from the Log file
EndIf
$SetVer="***BUSMASTER Ver "&$ToolVer[2]&"***"
consolewrite("$SetVer:"&$SetVer&@CRLF)
consolewrite("$VersionFromLog :"&$VersionFromLog&@CRLF)
consolewrite("$Protocol:"&$Protocol&@CRLF)
consolewrite("$NoEdit:"&$NoEdit&@CRLF)
consolewrite("$StartLog:"&$StartLog&@CRLF)
consolewrite("$PCDate:"&$PCDate&@CRLF)
consolewrite("$StartDateTime:"&$StartDateTime&@CRLF)
consolewrite("$StartDateTimeInLog:"&$StartDateTimeInLog&@CRLF)
consolewrite("$NumericMode:"&$NumericMode&@CRLF)
consolewrite("$TimeMode:"&$TimeMode&@CRLF)

if $ToolVer[2]=$VersionNo and $VersionFromLog=$SetVer and $Protocol="***PROTOCOL CAN***" and $NoEdit="***NOTE: PLEASE DO NOT EDIT THIS DOCUMENT***" and $StartLog="***[START LOGGING SESSION]***" and $StartDateTimeInLog="Match" and $NumericMode="***HEX***" and $TimeMode="***SYSTEM MODE***" Then
	msgbox(0,"","Pass")
Else
	msgbox(0,"","Fail")
EndIf

