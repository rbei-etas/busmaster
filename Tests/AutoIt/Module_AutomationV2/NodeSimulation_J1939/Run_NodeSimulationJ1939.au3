#include <Constants.au3>
#include <IncludeFiles.au3>

;~ ; Copy below Node simulation files from installed Busmaster folder to the script directory to avoid build,load errors
FileCopy ( $BusMasterExeFPath & "\GCCDLLMakeTemplate_J1939", "C:\Windows\SysWOW64", 1)
DirCreate( "C:\Windows\SysWOW64\SimulatedSystems")
DirCopy ( $BusMasterExeFPath & "\SimulatedSystems", "C:\Windows\SysWOW64\SimulatedSystems", 1)

Local $foo = Run('"C:\Program Files (x86)\AutoIt3\SciTE\AutoIt3Wrapper\AutoIt3Wrapper.exe" /run /prod /ErrorStdOut /in "D:\Module_AutomationV2\NodeSimulation_J1939\NodeSimulationJ1939.au3" /UserParams', @SystemDir, @SW_SHOW, $STDERR_CHILD + $STDOUT_CHILD)

Local $line

While 1
    ProcessWaitClose($foo)
    $line = StdoutRead($foo)
    If @error Then ExitLoop
;~     MsgBox(0, "STDOUT read:", $line)
	if FileExists ( @ScriptDir&"\Log.txt") Then
		FileDelete ( @ScriptDir&"\Log.txt")
	EndIf
	FileWrite ( @ScriptDir&"\Log.txt", $line )


Wend

Sleep(1000)