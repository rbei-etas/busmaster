@echo off

set DOTNET=C:\Windows\Microsoft.NET\Framework\v4.0.30319
if exist %DOTNET%\MSBuild.exe goto BUILD

set DOTNET=C:\Windows\Microsoft.NET\Framework\v3.5
if exist %DOTNET%\MSBuild.exe goto BUILD

echo .NET Framework not found. Build failed!
goto END

:BUILD
echo Using MSBuild found in %DOTNET%
set PATH=%DOTNET%;%PATH%
MSBuild BUSMASTER.sln /property:Configuration=Release
MSBuild "Format Converter\FormatConverter.sln" /property:Configuration=Release

:END
