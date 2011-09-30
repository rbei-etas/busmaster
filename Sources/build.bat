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
MSBuild TOOLS_EXTERNAL\CAPL_2_C\Language_ConverterDSW.sln /property:Configuration=Release
MSBuild TOOLS_EXTERNAL\EXT_DBC_2_DBF\CANDBConverterDSW.sln /property:Configuration=Release
MSBuild TOOLS_EXTERNAL\EXT_DBF_2_DBC\CANDBConverterDSW.sln /property:Configuration=Release

:END
