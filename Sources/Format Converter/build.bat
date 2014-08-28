@echo off

:DOTNET_SEARCH
set DOTNET=%SystemRoot%\Microsoft.NET\Framework\v4.0.30319
if exist "%DOTNET%\MSBuild.exe" goto DOTNET_FOUND

set DOTNET=%SystemRoot%\Microsoft.NET\Framework\v3.5
if exist "%DOTNET%\MSBuild.exe" goto DOTNET_FOUND

:DOTNET_NOT_FOUND
echo .NET Framework not found. Build failed!
goto END

:DOTNET_FOUND
echo Using MSBuild found in %DOTNET%

:CMAKE_SEARCH
set CMAKE=%ProgramFiles%\CMake\bin
if exist "%CMAKE%\cmake.exe" goto CMAKE_FOUND

:CMAKE_NOT_FOUND
echo CMake not found. Build failed!
goto END

:CMAKE_FOUND
echo Using CMake found in %CMAKE%

:BUILD
set PATH=%CMAKE%;%DOTNET%;%PATH%
REM define your build directory here:
mkdir build
cd build
REM define your compiler/IDE here:
cmake -G "Visual Studio 11 2012" ..
REM automatically compile solution:
MSBuild "FormatConverter.sln" /property:Configuration=Release

:END
REM pause
exit 0
