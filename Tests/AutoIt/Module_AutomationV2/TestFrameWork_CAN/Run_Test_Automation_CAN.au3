#include <Constants.au3>

Local $foo = Run('"C:\Program Files (x86)\AutoIt3\SciTE\AutoIt3Wrapper\AutoIt3Wrapper.exe" /run /prod /ErrorStdOut /in "D:\Module AutomationV2\TestFrameWork_CAN\Test_Automation_CAN.au3" /UserParams', @SystemDir, @SW_SHOW, $STDERR_CHILD + $STDOUT_CHILD)

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