@echo off

:SERNA_FIND
for /d %%i in ("%ProgramFiles%\Syntext\Serna Free*") do set SERNA_HOME=%%i
if exist "%SERNA_HOME%" goto DITA_FIND

:SERNA_NOT_FOUND
echo Serna not found. Build failed!
goto END

:DITA_FIND
for /d %%i in ("%SERNA_HOME%\plugins\dita\DITA-OT1.5*") do set DITA_HOME=%%i
if exist "%DITA_HOME%" goto DITA_1_5

:DITA_NOT_FOUND
echo DITA not found. Build failed!
goto END

:DITA_1_5
set CLASSPATH=%CLASSPATH%;%DITA_HOME%\lib\saxon
set CLASSPATH=%CLASSPATH%;%DITA_HOME%\lib\saxon\saxon9.jar
set CLASSPATH=%CLASSPATH%;%DITA_HOME%\lib\saxon\saxon9-dom.jar
set CLASSPATH=%CLASSPATH%;%DITA_HOME%\lib\saxon\saxon9-dom4j.jar
set CLASSPATH=%CLASSPATH%;%DITA_HOME%\lib\saxon\saxon9-jdom.jar
set CLASSPATH=%CLASSPATH%;%DITA_HOME%\lib\saxon\saxon9-s9api.jar
set CLASSPATH=%CLASSPATH%;%DITA_HOME%\lib\saxon\saxon9-sql.jar
set CLASSPATH=%CLASSPATH%;%DITA_HOME%\lib\saxon\saxon9-xom.jar
set CLASSPATH=%CLASSPATH%;%DITA_HOME%\lib\saxon\saxon9-xpath.jar
set CLASSPATH=%CLASSPATH%;%DITA_HOME%\lib\saxon\saxon9-xqj.jar
set OPTS=-Xmx512m

:PMD_FIND
for /d %%i in ("%ProgramFiles%\PMD*") do set PMD_HOME=%%i
if exist "%PMD_HOME%" goto CPD

:PMD_NOT_FOUND
echo PMD not found. Build failed!
goto END

:CPD
echo Using PMD found in %PMD_HOME%
set ARGS=--minimum-tokens 75 --files ..\..\Sources --language cpp --format xml --encoding utf-8
java %OPTS% -Djava.ext.dirs="%PMD_HOME%\lib" net.sourceforge.pmd.cpd.CPD %ARGS% > cpd.xml

:END
exit 0
