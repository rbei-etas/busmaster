#cs ----------------------------------------------------------------------------

 AutoIt Version: 3.3.6.1
 Author:         Pramod Krishna

 Script Function:
	Parent File containing 'Test Automation CAN' module test scripts.

#ce ----------------------------------------------------------------------------

#include <IncludeFiles.au3>

_RunMe()

;Run all the Test Automation CAN module scripts

; Create the HTML table to save the result
_StartoHTMLTable("Test Automation CAN Module Test Report")

#include "TS_TestAuto_01.au3"

#include "TS_TestAuto_04.au3"

; "TS_TestAuto_05 and "TS_TestAuto_06" covered in "TS_TestAuto_04.au3"
;#include "TS_TestAuto_01.au3"
; "TS_TestAuto_02" covered in "TS_TestAuto_04.au3"
; "TS_TestAuto_03" is P3
#include "TS_TestAuto_07.au3"
; "TS_TestAuto_08 - TS_TestAuto_27" covered in "TS_TestAuto_01.au3"
; "TS_TestAuto_28 - TS_TestAuto_34" covered in "TS_TestAuto_07.au3"


_EndoHTMLTable()

_SaveHTMLReport()

_IEQuit($oIE)

_ExcelBookClose($oExcel, 1, 0)