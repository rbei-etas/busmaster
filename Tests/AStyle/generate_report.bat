@echo off

:ASTYLE_FIND
set ASTYLE_HOME=%ProgramFiles%\AStyle
if exist "%ASTYLE_HOME%" goto DIFF_FIND

:ASTYLE_NOT_FOUND
echo AStyle not found. Build failed!
goto END

:DIFF_FIND
set DIFF_HOME=C:\MinGW\msys\1.0
if exist "%DIFF_HOME%" goto PERL_FIND
set DIFF_HOME=C:\cygwin
if exist "%DIFF_HOME%" goto PERL_FIND

:DIFF_NOT_FOUND
echo Diff not found. Build failed!
goto END

:PERL_FIND
set PERL_HOME=C:\Perl\bin
if exist "%PERL_HOME%" goto BUILD

:PERL_NOT_FOUND
echo Perl not found. Build failed!
goto END

:BUILD
echo Using AStyle found in %ASTYLE_HOME%
echo Using Diff found in %DIFF_HOME%
echo Using Perl found in %PERL_HOME%
set PATH=%ASTYLE_HOME%\bin;%PERL_HOME%\bin;%DIFF_HOME%\bin;%PATH%
perl generate_report.pl

:END
