@echo off

:CHECK_CMAKE
set CMAKE="C:\Program Files\CMake 2.8"
if exist %CMAKE%\bin\cmake.exe goto CHECK_MSBUILD
echo CMake not found. Build failed!
goto END

:CHECK_MSBUILD
set DOTNET=C:\Windows\Microsoft.NET\Framework\v4.0.30319
if exist %DOTNET%\MSBuild.exe goto CHECK_END
set DOTNET=C:\Windows\Microsoft.NET\Framework\v3.5
if exist %DOTNET%\MSBuild.exe goto CHECK_END
echo .NET Framework not found. Build failed!
goto END

:CHECK_END
echo Using MSBuild found in %DOTNET%
echo Using CMake found in %CMAKE%

:CMAKE
set PATH=%CMAKE%\bin;%PATH%
mkdir build
cd build
cmake -G "Visual Studio 9 2008" ..

:BUILD
set PATH=%DOTNET%;%PATH%
MSBuild DBC_2_DBF.sln /property:Configuration=Release

:END
pause
