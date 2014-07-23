#cs ----------------------------------------------------------------------------

 AutoIt Version: 3.3.6.1
 Author:         Pramod Krishna

 Script Function:
	Parent File containing 'DataBaseEditor' module test scripts.

#ce ----------------------------------------------------------------------------

#include <IncludeFiles.au3>

_RunMe()

; Create the HTML table to save the result
_StartoHTMLTable("CAN Database Editor Module Test Report")

;Run all the Database Editor module scripts

; --------------------------------------------------
; CAN DB P1 Case's

#include "TS_DBE_01.au3"
#include "TS_DBE_02.au3"
#include "TS_DBE_03.au3"
#include "TS_DBE_04.au3"
#include "TS_DBE_05.au3"
#include "TS_DBE_06.au3"
#include "TS_DBE_07.au3"
#include "TS_DBE_08.au3"
#include "TS_DBE_09.au3"
#include "TS_DBE_10.au3"
#include "TS_DBE_11.au3"
#include "TS_DBE_12.au3"

;~ ; ---------------------------------------------------

;~ ; ---------------------------------------------------
;~ ; CAN DB P2 Case's

;	"TS_DBE_13.au3" covered in "TS_DBE_01"
#include "TS_DBE_14.au3"
;~ ;	"TS_DBE_15" covered in "TS_DBE_16"
#include "TS_DBE_16.au3"
;~ ;	"TS_DBE_17" covered in "TS_DBE_16"
#include "TS_DBE_18.au3"
;~ ;	"TS_DBE_19" - "TS_DBE_20" not fit for regression
;~ ;	"TS_DBE_21" - Invalid testcase
;~ ;	"TS_DBE_22" covered in "TS_DBE_18"
;~ ;	"TS_DBE_23" covered in "TS_DBE_18"
#include "TS_DBE_24.au3"
;~ ;	"TS_DBE_25" covered in "TS_DBE_24"
;~ ;	"TS_DBE_26" covered in "TS_DBE_24"
;~ ;	"TS_DBE_27" covered in "TS_DBE_02"
;~ ;	"TS_DBE_28" covered in "TS_DBE_24"
;~ ;	"TS_DBE_29" covered in P1 case
#include "TS_DBE_30.au3"
;~ ;	"TS_DBE_31" covered in "TS_DBE_30"
;~ ;	"TS_DBE_32" covered in "TS_DBE_30"
;~ ;	"TS_DBE_33" covered in "TS_DBE_30"
;~ ;	"TS_DBE_34" covered in "TS_DBE_30"
;~ ;	"TS_DBE_35" covered in "TS_DBE_30"
;~ ;	"TS_DBE_36" covered in "TS_DBE_03"
;~ ;	"TS_DBE_37" is not suitable for regression
#include "TS_DBE_38.au3"
;~ ;	"TS_DBE_39" covered in "TS_DBE_38"
;~ ;	"TS_DBE_40" and "TS_DBE_41" - not suitable for regression
;~ ;	"TS_DBE_42" covered in "TS_DBE_38"
;~ ;	"TS_DBE_43" covered in "TS_DBE_03"
#include "TS_DBE_44.au3"
#include "TS_DBE_45.au3"
;~ ;	"TS_DBE_46" covered in "TS_DBE_45"
;~ ;	"TS_DBE_47" covered in P1 case
;~ ;	"TS_DBE_48" covered in "TS_DBE_03"
;~ ;	"TS_DBE_49" and "TS_DBE_50" not fit for regression
;~ ;	"TS_DBE_51" - Duplicate of TS_DBE_12
#include "TS_DBE_52.au3"
;~ ;	"TS_DBE_53" covered in "TS_DBE_46"
;~ ;	"TS_DBE_54" - not fit for regression

_EndoHTMLTable()

_SaveHTMLReport()

_IEQuit($oIE)