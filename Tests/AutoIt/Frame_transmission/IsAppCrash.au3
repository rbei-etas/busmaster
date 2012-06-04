

AdlibRegister("_IsAppCrash", 250) ;Every 250 ms (or time ms) the specified "function" is called--typically to check for unforeseen errors

Func _IsAppCrash()
	Global $crashRes
	If WinExists("BUSMASTER", "") Then
		Global $HWND = WinGetHandle("BUSMASTER")
		$aHungApp = DllCall("user32.dll", "int", "IsHungAppWindow", "hwnd", $HWND) ; handle of app window must be global for use with Adlib* functions
		;consolewrite("dllcall result "&$aHungApp[0])
		If @error Then
			ConsoleWrite("DllCall Error " & @error)
		EndIf
		If $aHungApp[0] = 0 Then ; If app is hung then close the application
			$crashRes = ControlClick("BUSMASTER.exe", "&Close", "")
			Return $crashRes ; pass the return value of ControlClick to calling function
		EndIf
	EndIf
EndFunc   ;==>_IsAppCrash
