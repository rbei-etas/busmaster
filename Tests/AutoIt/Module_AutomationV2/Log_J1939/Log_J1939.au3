#cs ----------------------------------------------------------------------------

 AutoIt Version: 3.3.6.1
 Author:         Pramod Krishna

 Script Function:
	Parent File containing 'j1939 Log' module test scripts.

#ce ----------------------------------------------------------------------------

#include <IncludeFiles.au3>

_RunMe()

; Create the HTML table to save the result
_StartoHTMLTable("J1939 Log Module Test Report")

;Run all the Database Editor module scripts

; --------------------------------------------------
; J1939 Log P1 Case's

#include "TS_J1939Log_01.au3"

#include "TS_J1939Log_02.au3"
#include "TS_J1939Log_03.au3"
;~ ; "TS_J1939Log_04" and "TS_J1939Log_05" are P2 case's
#include "TS_J1939Log_06.au3"
;~ ; "TS_J1939Log_07" case needs hardware.
;~ ; "TS_J1939Log_08" - cannot automate until issue is fixed

;~ ; --------------------------------------------------

;~ ; ---------------------------------------------------
;~ ; J1939 Log P2 Case's
;~ ; "TS_J1939Log_04" and "TS_J1939Log_05" to be tested manually
;~ ; "TS_J1939Log_09" covered in "TS_J1939Log_01"
;~ ; "TS_J1939Log_10" covered in "TS_J1939Log_01"
;~ ; "TS_J1939Log_11" covered in "TS_J1939Log_01"
;~ ; "TS_J1939Log_12" covered in "TS_J1939Log_01"
#include "TS_J1939Log_13.au3"
#include "TS_J1939Log_14.au3"
;~ ; "TS_J1939Log_15" covered in "TS_J1939Log_14"
; "TS_J1939Log_16" covered in "TS_J1939Log_14"
; "TS_J1939Log_17" covered in "TS_J1939Log_14"
; "TS_J1939Log_18" covered in "TS_J1939Log_14"
; "TS_J1939Log_19" covered in "TS_J1939Log_14"
; "TS_J1939Log_20" covered in "TS_J1939Log_14"

_EndoHTMLTable()

_SaveHTMLReport()

_IEQuit($oIE)

_ExcelBookClose($oExcel, 1, 0)