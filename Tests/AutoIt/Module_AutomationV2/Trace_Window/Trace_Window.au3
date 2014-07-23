#cs ----------------------------------------------------------------------------

 AutoIt Version: 3.3.6.1
 Author:         Pramod Krishna

 Script Function:
	Parent File containing 'Trace Window' module test scripts.

#ce ----------------------------------------------------------------------------

#include <IncludeFiles.au3>

_RunMe()

;Run all the Trace Window module scripts

; Create the HTML table to save the result
_StartoHTMLTable("Trace Window Module Test Report")

#include "TS_TraceWin_01.au3"
#include "TS_TraceWin_02.au3"

_EndoHTMLTable()

_SaveHTMLReport()

_IEQuit($oIE)