#cs ----------------------------------------------------------------------------

 AutoIt Version: 3.3.6.1
 Author:         Srinibas

 Script Function:
	Parent File containing 'Txwindow' module test scripts.

#ce ----------------------------------------------------------------------------

#include <IncludeFiles.au3>

_RunMe()


;Run all the Tx window module scripts
_StartoHTMLTable("Tx Window Module Test Report")

;~ ;-------------P1 test cases------------------------------
#include "TS_TxWin_CAN_01.au3"                             ;TS_TxWin_CAN_01 cfx file used
#include "TS_TxWin_CAN_02.au3"
#include "TS_TxWin_CAN_03.au3"
#include "TS_TxWin_CAN_04.au3"								;TS_TxWin_CAN_04 cfx file used
#include "TS_TxWin_CAN_05.au3"								;TxWin_CAN_36 is covered in TS_TxWin_CAN_05.au3
#include "TS_TxWin_CAN_06.au3"
#include "TS_TxWin_CAN_07.au3"
#include "TS_TxWin_CAN_08.au3"
#include "TS_TxWin_CAN_10.au3"								;TS_TxWin_CAN_01.cfx - The script will fail till Physical Value issues are resolved.
#include "TS_TxWin_CAN_12.au3"
;13
;~ ; ---------- TS_TxWin_CAN_17 ;  - need to be tested manually.;

#include "TS_TxWin_CAN_17.au3"
#include "TS_TxWin_CAN_18.au3"
#include "TS_TxWin_CAN_19.au3"								; TS_TxWin_CAN_19.cfx

;~ ; ---------- TS_TxWin_CAN_24 ;  - need to be tested manually.;

#include "TS_TxWin_CAN_26.au3"								; TS_TxWin_CAN_26.cfx
#include "TS_TxWin_CAN_27.au3"
#include "TS_TxWin_CAN_28.au3"
#include "TS_TxWin_CAN_34.au3"
#include "TS_TxWin_CAN_35.au3"

;~ ;-----;TxWin_CAN_36 is covered in TS_TxWin_CAN_05.au3;---------

#include "TS_TxWin_CAN_43.au3"

;~ ;---- TS_TxWin_CAN_44.au3 is covered in TS_TxWin_CAN_43;------

#include "TS_TxWin_CAN_54.au3"



;----------------P2 test cases-----------------------------------



_EndoHTMLTable()

_SaveHTMLReport()

_IEQuit($oIE)

_ExcelBookClose($oExcel, 1, 0)
