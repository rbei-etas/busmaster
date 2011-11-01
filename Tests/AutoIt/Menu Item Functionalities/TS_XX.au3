#include <GUIConstantsEx.au3>
#include <EditConstants.au3>
#include <WindowsConstants.au3>
#include <Array.au3>

$FileName = "TS_XX"
$file = FileOpen($FileName & ".txt", 2)
$SuccessCount = 0

#include "TS_XX_001.au3"
#include "TS_XX_002.au3"
#include "TS_XX_003.au3"
#include "TS_XX_004.au3"
#include "TS_XX_005.au3"
#include "TS_XX_006.au3"
#include "TS_XX_007.au3"
#include "TS_XX_008.au3"
#include "TS_XX_009.au3"
#include "TS_XX_010.au3"
#include "TS_XX_011.au3"
#include "TS_XX_012.au3"
#include "TS_XX_013.au3"
#include "TS_XX_014.au3"
#include "TS_XX_015.au3"
#include "TS_XX_016.au3"
#include "TS_XX_017.au3"
#include "TS_XX_018.au3"
#include "TS_XX_019.au3"
#include "TS_XX_020.au3"
#include "TS_XX_021.au3"
#include "TS_XX_022.au3"
#include "TS_XX_023.au3"
#include "TS_XX_024.au3"
#include "TS_XX_025.au3"
#include "TS_XX_026.au3"
#include "TS_XX_027.au3"
#include "TS_XX_028.au3"
#include "TS_XX_029.au3"
#include "TS_XX_030.au3"
#include "TS_XX_031.au3"
#include "TS_XX_032.au3"
#include "TS_XX_033.au3"
#include "TS_XX_034.au3"
#include "TS_XX_035.au3"
#include "TS_XX_036.au3"
#include "TS_XX_037.au3"
#include "TS_XX_038.au3"
#include "TS_XX_039.au3"
#include "TS_XX_040.au3"
#include "TS_XX_041.au3"
#include "TS_XX_042.au3"
#include "TS_XX_043.au3"
#include "TS_XX_044.au3"
#include "TS_XX_045.au3"

; --- Write Summary ---
FileWriteLine($file,  $SuccessCount& " out of " & "45 test cases successful.")	
FileClose($file)

; --- Show Summary ---
$strRunPath = "notepad.exe " & @ScriptDir & "\" & $FileName & ".txt"
Run($strRunPath)

;End of Script
