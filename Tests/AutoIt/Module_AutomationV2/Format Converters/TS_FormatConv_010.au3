;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_FormatConv_010
; TestCases			:		CAPL_2_CPP converter
; Test Data			:		cfxFormatConv_01.cfx
; Test Setup		:		1. Select File->Utilities->Format converter
;                           2.click on CAPL_2_CPP converter tab
;                           3.click on Browse of .can file and select any .can file and add.
;                           4.click on Browse of .cpp file to add a new cpp file which will hold contents converted .can file to cpp file.
;                           5.Click convert
; Expected Results  : 		After step3, when .can file is added, in the .cpp file, the same path is added with same file name except the file extension is .cpp.
;                           If you want to change this follow step4.
;                           After step5, since dbc database file is not added,a warning message  pops up  saying "No DBC files are added. If CAPL file contains any dbc messages,
;                           converted file cpp may not compile. Do you want to continue?"
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_FormatConv_010****"&@CRLF)

_launchApp()
WinActivate($WIN_BUSMASTER)

$OutputFolderPath=_OutputDataPath()
$InputFolderPath=_TestDataPath()
$InputCAPLFile="CAPL2CPP.can"
$OutputCPPFile="CAPL2CPP.cpp"

if winexists($WIN_BUSMASTER) then
	_loadConfig("cfxFormatConv_01.cfx")
		local $ConversionRes[10]
	_FormatConverterMenu()																						   ; Open format converters
    if @error Then
		ConsoleWriteError("Format Converter didn't run" & @CRLF)
		Exit
    EndIf
	WinWaitActive($WIN_FormatConv, "", 5)                                                                          ; BUSMASTER Format Conversions
	if @error Then
	    ConsoleWriteError("Format Conversions didn't activate" & @CRLF)
		Exit
    EndIf

	$hWndCAPL2CPP=controlgethandle($WIN_FormatConv,"",$Tab_CAPL2CPP)                                            ; test case for capl to cpp conversion
	_GUICtrlTab_ClickTab($hWndCAPL2CPP, 0)                                                                     ; Click on CAPL2CPP converter tab
	controlclick($WIN_FormatConv,"",$BTN_CaplInput)                                                            ; Click input
	WinWaitActive($WIN_SelectCAN_CAPL2CPP, "", 5)
	$InputCAPLPath=_TestDataPath()                                                                             ; Input file path
	sleep(1000)
	ControlSend($WIN_SelectCAN_CAPL2CPP,"",$TXT_Open_File_CAPL2CPP,$InputCAPLPath & "\" & $InputCAPLFile)      ; Send input file path to the control
	sleep(750)
	ControlClick($WIN_SelectCAN_CAPL2CPP,"",$BTN_Open_File_CAPL2CPP)
	sleep(750)
	WinWaitClose($WIN_SelectCAN_CAPL2CPP, "", 3)
	sleep(750)
	ControlSend($WIN_SelectCpp_CAPL2CPP,"",$TXT_Open_File_CAPL2CPP,$OutputFolderPath & "\" & $OutputCPPFile)   ; Send output file path to the control
	sleep(500)
    ControlClick($WIN_SelectCpp_CAPL2CPP,"",$BTN_Open_File_CAPL2CPP)
	controlclick($WIN_FormatConv,"",$BTN_Convert_CAPL2CPP)                                                        ;Click on Convert button
	sleep(4000)
	If WinExists($WIN_Warning) Then
		$text=ControlGetText($WIN_Warning,"",65535)
	EndIf
	ConsoleWrite("text " &$text & @CRLF)
	$res=StringInStr($text,"No DBC Files are Added.If the CAPL File have any DBC messages converted file cpp may not compile.")

     If($res>0) Then
		_WriteResult("Pass","STS_FORMAT_CONV_010")
	Else
		_WriteResult("Fail","STS_FORMAT_CONV_010")
	EndIf
	ControlClick($WIN_Warning,"",$BTN_NO_Continue)
	WinClose($WIN_FormatConv)
EndIf




