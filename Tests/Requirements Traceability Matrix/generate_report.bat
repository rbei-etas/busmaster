@echo off

:PERL_FIND
set PERL_HOME=C:\Perl\bin
if exist "%PERL_HOME%" goto BUILD

:PERL_NOT_FOUND
echo Perl not found. Build failed!
goto END

:BUILD
echo Using Perl found in %PERL_HOME%
set PATH=%PERL_HOME%\bin;%PATH%
perl generate_report.pl

:END
