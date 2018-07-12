#cs ----------------------------------------------------------------------------

 AutoIt Version: 3.3.6.1
 Author:         Srinibas Das

 Script Function:
	Parent File containing 'LIN_Filter" module test scripts.

#ce ----------------------------------------------------------------------------

#include <IncludeFiles.au3>

_RunMe()

; Create the HTML table to save the result
_StartoHTMLTable("LIN_Filter Module Test Report")

;Run all the Database Editor module scripts

; --------------------------------------------------
; LIN_Filter P1 Case's
;If(@OSVersion = "WIN_7") Then

#include "TS_LIN_Filter_01.au3"
#include "TS_LIN_Filter_02.au3"
#include "TS_LIN_Filter_03.au3"
#include "TS_LIN_Filter_04.au3"
#include "TS_LIN_Filter_05.au3"
#include "TS_LIN_Filter_06.au3"
#include "TS_LIN_Filter_07.au3"

; LIN_Filter P2 Case's

#include "TS_LIN_Filter_08.au3"
#include "TS_LIN_Filter_09.au3"
#include "TS_LIN_Filter_10.au3"
; "TS_LIN_Filter_11" covered in TS_LIN_Filter_10.au3
#include "TS_LIN_Filter_12.au3"
#include "TS_LIN_Filter_13.au3"
#include "TS_LIN_Filter_14.au3"
#include "TS_LIN_Filter_15.au3"
#include "TS_LIN_Filter_16.au3"
;TS_LIN_Filter_17 is covered in TS_LIN_Filter_12.au3
#include "TS_LIN_Filter_18.au3"
;TS_LIN_Filter_19 and TS_LIN_Filter_20 is covered in TS_LIN_Filter_18.au3


; LIN_Filter P3 Case's




;~ ; --------------------------------------------------

;EndIf


_EndoHTMLTable()

_SaveHTMLReport()

_IEQuit($oIE)

_ExcelBookClose($oExcel, 1, 0)