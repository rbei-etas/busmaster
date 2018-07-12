#cs ----------------------------------------------------------------------------

 AutoIt Version: 3.3.6.1
 Author:         Pramod Krishna

 Script Function:
	Parent File containing 'CAN Network Statistics' module test scripts.

#ce ----------------------------------------------------------------------------

#include <IncludeFiles.au3>

_RunMe()

;Run all the CAN Network Statistics module scripts

; Create the HTML table to save the result
_StartoHTMLTable("CAN Network Statistics  Module Test Report")

;~ "TS_NwSt_01" and "TS_NwSt_02" are P2 case's
#include "TS_NwSt_03_04.au3"

; "TS_NwSt_03" and "TS_NwSt_04" case's included in above script
#include "TS_NwSt_05_06_07.au3"
; "TS_NwSt_05", "TS_NwSt_06" and "TS_NwSt_07" case's included in above script
; "TS_NwSt_06" is not a valid testcase

_EndoHTMLTable()

_SaveHTMLReport()

_IEQuit($oIE)

_ExcelBookClose($oExcel, 1, 0)