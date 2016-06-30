#cs ----------------------------------------------------------------------------

 AutoIt Version: 3.3.6.1
 Author:         Shilpa Murali

 Script Function:
	Parent File containing 'Advanced logging for LIN' module test scripts.

#ce ----------------------------------------------------------------------------

#include <IncludeFiles.au3>

_RunMe()

; Create the HTML table to save the result
_StartoHTMLTable("Advanced logging for LIN Module Test Report")

;Run all the Database Editor module scripts

; --------------------------------------------------
; Advanced logging for LIN P1 Case's

#include "TS_Log_LIN_001.au3"
#include "TS_Log_LIN_002.au3"
#include "TS_Log_LIN_003.au3"
#include "TS_Log_LIN_004.au3"
#include "TS_Log_LIN_005.au3"
#include "TS_Log_LIN_006.au3"
#include "TS_Log_LIN_007.au3"


; --------------------------------------------------
; Advanced logging for LIN P2 Case's

#include "TS_Log_LIN_008.au3"
#include "TS_Log_LIN_009.au3"
#include "TS_Log_LIN_010.au3"
#include "TS_Log_LIN_011.au3"


_EndoHTMLTable()

_SaveHTMLReport()

_IEQuit($oIE)

_ExcelBookClose($oExcel, 1, 0)