#cs ----------------------------------------------------------------------------

 AutoIt Version: 3.3.6.1
 Author:         Pramod Krishna

 Script Function:
	Parent File containing 'Log_Filter_Replay' module test scripts.

#ce ----------------------------------------------------------------------------


; Create the HTML table to save the result
_StartoHTMLTable("Log_Filter_Replay Module Test Report")

;Run all the Log_Filter_Replay module scripts

; --------------------------------------------------
; Filter P1 Case's
#include "TS_Filter_01.au3"
#include "TS_Filter_02.au3"
#include "TS_Filter_03.au3"
#include "TS_Filter_04.au3"
#include "TS_Filter_05.au3"
#include "TS_Filter_06.au3"
#include "TS_Filter_07.au3"

; Filter P2 Case's
;	TS_Filter_07- TS_Filter_12 is covered in P1 case
;	TS_Filter_12 is Not suitable for regression
#include "TS_Filter_14.au3"
;	TS_Filter_15 is Not suitable for regression
#include "TS_Filter_16.au3"
;	TS_Filter_17 is covered in P1 case
#include "TS_Filter_18.au3"
;	TS_Filter_19 & TS_Filter_20 is covered in P1 case
; --------------------------------------------------

; --------------------------------------------------
; Logging P1 case's
#include "TS_Log_01.au3"
#include "TS_Log_02.au3"
#include "TS_Log_03.au3"
#include "TS_Log_04.au3"
#include "TS_Log_05.au3"
#include "TS_Log_06.au3"
#include "TS_Log_08.au3"
#include "TS_Log_11.au3"
#include "TS_Log_10.au3"
#include "TS_Log_12.au3"
#include "TS_Log_13.au3"
#include "TS_Log_14.au3"
#include "TS_Log_15.au3"
#include "TS_Log_16.au3"
#include "TS_Log_17.au3"
#include "TS_Log_18.au3"
#include "TS_Log_19.au3"
#include "TS_Log_20.au3"
#include "TS_Log_21.au3"
#include "TS_Log_22.au3"
#include "TS_Log_23.au3"
#include "TS_Log_24.au3"
#include "TS_Log_25.au3"
#include "TS_Log_26.au3"
;	'TS_Log_27" to TS_Log_31 - Covered in TS_Log_01
; Logging P2 case's
#include "TS_Log_32.au3"
#include "TS_Log_33.au3"
;	"TS_Log_34" and "TS_Log_35" to be tested manually
;	"TS_Log_36" - can't automate until issue is fixed
;	"TS_Log_37" - Invalid testcase
#include "TS_Log_38.au3"
;	'TS_Log_039" to TS_Log_044 - Covered in TS_Log_038
; --------------------------------------------------

; --------------------------------------------------
; Replay P1 case's
#include "TS_Replay_01.au3"
#include "TS_Replay_02.au3"
#include "TS_Replay_03.au3"
#include "TS_Replay_04.au3"
#include "TS_Replay_05.au3"
#include "TS_Replay_06.au3"
#include "TS_Replay_07.au3"
#include "TS_Replay_08.au3"
#include "TS_Replay_09.au3"
#include "TS_Replay_10.au3"
#include "TS_Replay_11.au3"
#include "TS_Replay_12.au3"
#include "TS_Replay_13.au3"
#include "TS_Replay_14.au3"
#include "TS_Replay_15.au3"
#include "TS_Replay_16.au3"
#include "TS_Replay_17.au3"
#include "TS_Replay_18.au3"
#include "TS_Replay_19.au3"
#include "TS_Replay_20.au3"
#include "TS_Replay_21.au3"
#include "TS_Replay_22.au3"
#include "TS_Replay_23.au3"
#include "TS_Replay_24.au3"
#include "TS_Replay_25.au3"
#include "TS_Replay_26.au3"
#include "TS_Replay_27.au3"
#include "TS_Replay_28.au3"
#include "TS_Replay_29.au3"
#include "TS_Replay_30.au3"
#include "TS_Replay_31.au3"
#include "TS_Replay_32.au3"
#include "TS_Replay_33.au3"
#include "TS_Replay_34.au3"
#include "TS_Replay_35.au3"
#include "TS_Replay_36.au3"
#include "TS_Replay_37.au3"
#include "TS_Replay_38.au3"
; Delete "TS_Replay_39.au3"  - duplicate of "TS_Replay_38.au3"
#include "TS_Replay_40.au3"
#include "TS_Replay_41.au3"
#include "TS_Replay_42.au3"
#include "TS_Replay_43.au3"
#include "TS_Replay_44.au3"
#include "TS_Replay_45.au3"
#include "TS_Replay_46.au3"
#include "TS_Replay_47.au3"
;~ ; "TS_Replay_48" tc is covered in "TS_Replay_47.au3" script

; Replay P2 case's

;	"TS_Replay_049" - covered in TS_Replay_03
#include "TS_Replay_50.au3"
;	"TS_Replay_51" and "TS_Replay_52" - covered in TS_Replay_50
#include "TS_Replay_53.au3"
#include "TS_Replay_54.au3"
;	"TS_Replay_55" - covered in TS_Replay_08
;	"TS_Replay_56" and "TS_Replay_57"- covered in TS_Replay_07
;	"TS_Replay_58" - covered in TS_Replay_08
;	"TS_Replay_59" - covered in TS_Replay_09
#include "TS_Replay_60.au3"
;	"TS_Replay_61" - "TS_Replay_63"- covered in TS_Replay_60
;	"TS_Replay_64" - "TS_Replay_67"- not suitable for regression
#include "TS_Replay_68.au3"
;	"TS_Replay_69" - covered in TS_Replay_60

_EndoHTMLTable()
