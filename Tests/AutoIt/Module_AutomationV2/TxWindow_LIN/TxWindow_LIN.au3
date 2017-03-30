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
;If(@OSVersion = "WIN_7") Then
#include "TS_TX_LIN_01.au3"
#include "TS_TX_LIN_02.au3"   ;PASS

#include "TS_TX_LIN_03.au3"   ;PASS
#include "TS_TX_LIN_04.au3"   ;PASS


;;;----- "TS_TX_LIN_05.au3"  -  Covered in "TS_TX_LIN_03.au3";;;---

#include "TS_TX_LIN_06.au3"	 ;PASS
#include "TS_TX_LIN_07.au3"
#include "TS_TX_LIN_09.au3"		;------cfx_TS_TX_LIN_09-----------------
;#include "TS_TX_LIN_16.au3"		;------cfx_TS_TX_LIN_16-----------------


;----------------P2 test cases-----------------------------------

;EndIf

_EndoHTMLTable()

_SaveHTMLReport()

_IEQuit($oIE)

_ExcelBookClose($oExcel, 1, 0)

