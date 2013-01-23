
Global $crashRes
AdlibRegister("_IsAppCrash",250)							;Every 250 ms (or time ms) the specified "function" is called--typically to check for unforeseen errors

;~ Func _IsAppCrash()
;~ if winexists($mWin,"") then
;~ 	global $HWND=WinGetHandle($mWin)
;~ 	$aHungApp = DllCall("user32.dll", "int", "IsHungAppWindow", "hwnd", $HWND) ; handle of app window must be global for use with Adlib* functions
;~ 	;consolewrite("dllcall result "&$aHungApp[0])
;~ 	If @error Then
;~ 	consolewrite("DllCall Error " &@error)
;~ 	EndIf
;~ 	If $aHungApp[0]=0 Then													   ; If app is hung then close the application
;~ 	$crashRes=ControlClick($appCrashtitle,"&Close","")
;~ 	return $crashRes																   ; pass the return value of ControlClick to calling function
;~ 	EndIf
;~ Endif
;~ EndFunc

; Latest Crash pop ups

Func _IsAppCrash()

	$crashRes=0
	; if $appCrashtxt dialog appears then Click Yes
	if winexists($mWin,$appCrashtxt) Then
		$crashRes=1
		; click 'Yes'
		Controlclick($mWin,"",6)
		sleep(1000)
		; Saved dump file to "temp path"  (Saved dump file to C:\DOCUME~1\RKR7KOR\LOCALS~1\Temp\BUSMASTER.dmp)
		if winexists($mWin,"") then
			; Click 'ok'
			Controlclick($mWin,"",2)
		EndIf
		sleep(1000)
		if winexists($appCrashtitle,"") then
			; Click on 'Close' button
			Controlclick($appCrashtitle,$closeBTN,"")
		EndIf
		return $crashRes
	EndIf

EndFunc