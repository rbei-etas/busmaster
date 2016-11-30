#include <Constants.au3>
#include <IncludeFiles.au3>

Local $foo = Run('"C:\Program Files (x86)\AutoIt3\SciTE\AutoIt3Wrapper\AutoIt3Wrapper.exe" /run /prod /ErrorStdOut /in "C:\Module_AutomationV2\Database Editor_CAN\DBEditor.au3" /UserParams', @SystemDir, @SW_SHOW, $STDERR_CHILD + $STDOUT_CHILD)

Local $line
_monitorScript($foo)
;~ While 1
;~     ProcessWaitClose($foo)
;~     $line = StdoutRead($foo)
;~     If @error Then ExitLoop
;~     MsgBox(0, "STDOUT read:", $line)
;~ 	if FileExists ( @ScriptDir&"\PrintLog.txt") Then
;~ 		FileDelete ( @ScriptDir&"\PrintLog.txt")
;~ 	EndIf
;~ 	FileWrite ( @ScriptDir&"\PrintLog.txt", $line )


;~ Wend

Sleep(5000)

