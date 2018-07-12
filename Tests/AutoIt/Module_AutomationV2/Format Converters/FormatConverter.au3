#cs ----------------------------------------------------------------------------#cs ----------------------------------------------------------------------------

 AutoIt Version: 3.3.6.1
 Author:         Anita Nadgir

 Script Function:
	Parent File containing 'Format converters' module test scripts.

#ce ----------------------------------------------------------------------------

#include <IncludeFiles.au3>

_RunMe()

;Run all the Format Converters module scripts

; Create the HTML table to save the result
_StartoHTMLTable("Format Converters Module Test Report")

       ;P1 test cases
  #include "TS_FormatConv_001.au3"

;~   ; "TS_FormatConv_002" covered in "TS_FormatConv_001"
  #include "TS_FormatConv_003.au3"
  #include "TS_FormatConv_004.au3"
  #include "TS_FormatConv_005.au3"
  #include "TS_FormatConv_006.au3"
  #include "TS_FormatConv_007.au3"
  #include "TS_FormatConv_008.au3"
  #include "TS_FormatConv_009.au3"
  #include "TS_FormatConv_010.au3"

 ;------------------------------------
       ;P2 test cases
#include "TS_FormatConv_011.au3"
#include "TS_FormatConv_020.au3"
#include "TS_FormatConv_021.au3"
#include "TS_FormatConv_022.au3"
#include "TS_FormatConv_023.au3"

_EndoHTMLTable()

_SaveHTMLReport()

_IEQuit($oIE)

_ExcelBookClose($oExcel, 1, 0)