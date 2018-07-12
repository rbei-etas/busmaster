#cs ----------------------------------------------------------------------------

 AutoIt Version: 3.3.6.1
 Author:         Shilpa Murali

 Script Function:
	Parent File containing 'Advanced logging for CAN' module test scripts.

#ce ----------------------------------------------------------------------------

#include <IncludeFiles.au3>

_RunMe()

; Create the HTML table to save the result
_StartoHTMLTable("Advanced logging for CAN Module Test Report")

;Run all the Advanced logging for CAN module scripts

; --------------------------------------------------
; Advanced logging for CAN P1 Case's

#include "TS_Log_001.au3"
#include "TS_Log_002.au3"
#include "TS_Log_003.au3"
#include "TS_Log_004.au3"
#include "TS_Log_005.au3"
#include "TS_Log_006.au3"
#include "TS_Log_007.au3"

; --------------------------------------------------
; Advanced logging for CAN P2 Case's
#include "TS_Log_008.au3"
#include "TS_Log_009.au3"
#include "TS_Log_010.au3"
#include "TS_Log_011.au3"


_EndoHTMLTable()

_SaveHTMLReport()

_IEQuit($oIE)

_ExcelBookClose($oExcel, 1, 0)