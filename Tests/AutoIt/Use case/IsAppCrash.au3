

AdlibRegister("_IsAppCrash",250)							;Every 250 ms (or time ms) the specified "function" is called--typically to check for unforeseen errors

Func _IsAppCrash()
	Global $crashRes
if winexists("BUSMASTER","") then
	;global $HWND=WinGetHandle("BUSMASTER")
	;$aHungApp = DllCall("user32.dll", "int", "IsHungAppWindow", "hwnd", $HWND) ; handle of app window must be global for use with Adlib* functions
	;consolewrite("dllcall result "&$aHungApp[0])
	;If @error Then
	;consolewrite("DllCall Error " &@error)
	;EndIf
	;If $aHungApp[0]=0 Then													   ; If app is hung then close the application
	if winexists("BUSMASTER.exe","") then
	$crashRes=ControlClick("BUSMASTER.exe","&Close","")
	endif
	return $crashRes																   ; pass the return value of ControlClick to calling function
Endif
EndFunc