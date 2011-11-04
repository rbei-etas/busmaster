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
MSBuild TOOLS_EXTERNAL\CAPL_2_C\CAPL_2_C.sln /property:Configuration=Release
MSBuild TOOLS_EXTERNAL\DBC_2_DBF\DBC_2_DBF.sln /property:Configuration=Release
MSBuild TOOLS_EXTERNAL\DBF_2_DBC\DBF_2_DBC.sln /property:Configuration=Release

:END
