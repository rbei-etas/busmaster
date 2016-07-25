@echo off

:DOTNET_FIND
set DOTNET=%SystemRoot%\Microsoft.NET\Framework\v4.0.30319
if exist "%DOTNET%\MSBuild.exe" goto BUILD
set DOTNET=%SystemRoot%\Microsoft.NET\Framework\v3.5
if exist "%DOTNET%\MSBuild.exe" goto BUILD

:DOTNET_NOT_FOUND
echo .NET Framework not found. Build failed!
goto END

:BUILD
echo Using MSBuild found in %DOTNET%
set PATH=%DOTNET%;%PATH%
MSBuild "Kernel\BusmasterKernel.sln" /property:Configuration=Release /p:VisualStudioVersion=12.0
MSBuild "BUSMASTER\BUSMASTER.sln" /property:Configuration=Release /p:VisualStudioVersion=12.0 

REM CAN PEAK USB.
MSBuild "BUSMASTER\CAN_PEAK_USB\CAN_PEAK_USB.vcxproj" /p:VisualStudioVersion=12.0 /property:Configuration=Release  

MSBuild "BUSMASTER\Language Dlls\Language Dlls.sln" /property:Configuration=Release /p:VisualStudioVersion=12.0 
MSBuild "BUSMASTER\LDFEditor\LDFEditor.sln" /property:Configuration=Release /p:VisualStudioVersion=12.0 
MSBuild "BUSMASTER\LDFViewer\LDFViewer.sln" /property:Configuration=Release /p:VisualStudioVersion=12.0

REM Asc Log
cd ..\Tools\flex 
"flex.exe" -i -L -o"..\..\Sources\BUSMASTER\Format Converter\AscLogConverter\Asc_Log_Lexer.c" "..\..\Sources\BUSMASTER\Format Converter\AscLogConverter\Asc_Log_Lexer.l"
cd ..\bison
"bison.exe" -d -l -o"..\..\Sources\BUSMASTER\Format Converter\AscLogConverter\Asc_Log_Parser.c" "..\..\Sources\BUSMASTER\Format Converter\AscLogConverter\Asc_Log_Parser.y"

REM Log Asc
cd ..\flex 
"flex.exe" -i -L -o"..\..\Sources\BUSMASTER\Format Converter\LogAscConverter\Log_Asc_Lexer.c" "..\..\Sources\BUSMASTER\Format Converter\LogAscConverter\Log_Asc_Lexer.l"
cd ..\bison
"bison.exe" -d -l -o"..\..\Sources\BUSMASTER\Format Converter\LogAscConverter\Log_Asc_Parser.c" "..\..\Sources\BUSMASTER\Format Converter\LogAscConverter\Log_Asc_Parser.y"

cd ..\..\Sources
MSBuild "BUSMASTER\Format Converter\FormatConverter.sln" /property:Configuration=Release /p:VisualStudioVersion=12.0 

:END

