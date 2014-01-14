#cs ----------------------------------------------------------------------------

 AutoIt Version: 3.3.6.1
 Author:         Pramod Krishna

 Script Function:
	Parent File containing 'J1939 DataBaseEditor' module test scripts.

#ce ----------------------------------------------------------------------------


; Create the HTML table to save the result
_StartoHTMLTable("J1939 Database Editor Module Test Report")

;Run all the Database Editor module scripts
; --------------------------------------------------
; J1939 DB P1 Case's

#include "TS_J1939_DBE_01.au3"
#include "TS_J1939_DBE_02.au3"
#include "TS_J1939_DBE_03.au3"
#include "TS_J1939_DBE_04.au3"
#include "TS_J1939_DBE_05.au3"
#include "TS_J1939_DBE_06.au3"
#include "TS_J1939_DBE_07.au3"
#include "TS_J1939_DBE_08.au3"
#include "TS_J1939_DBE_09.au3"
#include "TS_J1939_DBE_10.au3"
#include "TS_J1939_DBE_11.au3"
#include "TS_J1939_DBE_12.au3"

; ---------------------------------------------------

; ---------------------------------------------------
; J1939 DB P2 Case's

;	"TS_J1939_DBE_13.au3" covered in "TS_J1939_DBE_01"
#include "TS_J1939_DBE_14.au3"
;	"TS_J1939_DBE_15" covered in "TS_J1939_DBE_16"
#include "TS_J1939_DBE_16.au3"
;	"TS_J1939_DBE_17" covered in "TS_J1939_DBE_16"
#include "TS_J1939_DBE_18.au3"
;	"TS_J1939_DBE_19" - "TS_J1939_DBE_20" not fit for regression
;	"TS_J1939_DBE_21" - Invalid testcase
;	"TS_J1939_DBE_22" covered in "TS_J1939_DBE_18"
;	"TS_J1939_DBE_23" covered in "TS_J1939_DBE_18"
#include "TS_J1939_DBE_24.au3"
;	"TS_J1939_DBE_25" covered in "TS_J1939_DBE_24"
;	"TS_J1939_DBE_26" covered in "TS_J1939_DBE_24"
;	"TS_J1939_DBE_27" covered in "TS_J1939_DBE_02"
;	"TS_J1939_DBE_28" covered in "TS_J1939_DBE_24"
;	"TS_J1939_DBE_29" covered in P1 case
#include "TS_J1939_DBE_30.au3"
;	"TS_J1939_DBE_31" covered in "TS_J1939_DBE_30"
;	"TS_J1939_DBE_32" covered in "TS_J1939_DBE_30"
;	"TS_J1939_DBE_33" covered in "TS_J1939_DBE_30"
;	"TS_J1939_DBE_34" covered in "TS_J1939_DBE_30"
;	"TS_J1939_DBE_35" covered in "TS_J1939_DBE_30"
;	"TS_J1939_DBE_36" covered in "TS_J1939_DBE_03"
;	"TS_J1939_DBE_37" covered in "TS_J1939_DBE_03"
#include "TS_J1939_DBE_38.au3"
;	"TS_J1939_DBE_39" covered in "TS_J1939_DBE_38"
;	"TS_J1939_DBE_40" and "TS_J1939_DBE_41" - not suitable for regression
;	"TS_J1939_DBE_42" covered in "TS_J1939_DBE_38"
;	"TS_J1939_DBE_43" covered in "TS_J1939_DBE_03"
#include "TS_J1939_DBE_44.au3"
#include "TS_J1939_DBE_45.au3"
;	"TS_J1939_DBE_46" covered in "TS_J1939_DBE_45"
;	"TS_J1939_DBE_47" covered in P1 case
;	"TS_J1939_DBE_48" covered in "TS_J1939_DBE_03"
;	"TS_J1939_DBE_49" and "TS_J1939_DBE_50" not fit for regression
;	"TS_J1939_DBE_51" - Duplicate of TS_J1939_DBE_12
#include "TS_J1939_DBE_52.au3"
;	"TS_J1939_DBE_53" covered in "TS_J1939_DBE_46"
;	"TS_J1939_DBE_54" - not fit for regression


_EndoHTMLTable()