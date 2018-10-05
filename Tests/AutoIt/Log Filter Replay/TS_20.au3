#include <GUIConstantsEx.au3>
#include <EditConstants.au3>
#include <WindowsConstants.au3>
#include <Array.au3>

$FileName = "TS_20"
$file = FileOpen($FileName & ".txt", 2)
$SuccessCount = 0

#include "TS_20_001.au3"
#include "TS_20_002.au3"
#include "TS_20_003.au3"
#include "TS_20_004.au3"
#include "TS_20_005.au3"
#include "TS_20_006.au3"
#include "TS_20_007.au3"
#include "TS_20_008.au3"
#include "TS_20_009.au3"
#include "TS_20_010.au3"
#include "TS_20_011.au3"
#include "TS_20_012.au3"
#include "TS_20_013.au3"
#include "TS_20_014.au3"
#include "TS_20_015.au3"
#include "TS_20_021.au3"
#include "TS_20_022.au3"
#include "TS_20_024.au3"
#include "TS_20_025.au3"
#include "TS_20_026.au3"
#include "TS_20_029.au3"
#include "TS_20_031.au3"
#include "TS_20_032.au3"
#include "TS_20_033.au3"
#include "TS_20_034.au3"

; --- Write Summary ---
FileWriteLine($file,  $SuccessCount& " out of " & "25 test cases successful.")	
FileClose($file)

; --- Show Summary ---
$strRunPath = "notepad.exe " & @ScriptDir & "\" & $FileName & ".txt"
Run($strRunPath)

;End of Script
