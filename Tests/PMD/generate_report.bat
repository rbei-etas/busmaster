@echo off

:JAVA_FIND
for /d %%i in ("%ProgramFiles%\Java\jdk*") do set JAVA_HOME=%%i
if exist "%JAVA_HOME%\lib\tools.jar" goto SERNA_FIND

:JAVA_NOT_FOUND
echo Java Development Kit not found. Build failed!
goto END

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
set CLASSPATH=%CLASSPATH%;%DITA_HOME%\lib
set CLASSPATH=%CLASSPATH%;%DITA_HOME%\lib\dost.jar
set CLASSPATH=%CLASSPATH%;%DITA_HOME%\lib\commons-codec-1.4.jar
set CLASSPATH=%CLASSPATH%;%DITA_HOME%\lib\resolver.jar
set CLASSPATH=%CLASSPATH%;%DITA_HOME%\lib\icu4j.jar
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
set ANT_OPTS=-Xmx512m
set ANT_OPTS=%ANT_OPTS% -Djavax.xml.transform.TransformerFactory=net.sf.saxon.TransformerFactoryImpl

:PMD_FIND
for /d %%i in ("%ProgramFiles%\PMD*") do set PMD_HOME=%%i
if exist "%PMD_HOME%" goto MINGW

:PMD_NOT_FOUND
echo PMD not found. Build failed!
goto END

:MINGW
set MINGW_HOME=C:\MinGW
if exist "%MINGW_HOME%" goto MSYS

:MINGW_NOT_FOUND
echo MinGW not found. Build failed!
goto END

:MSYS
set MSYS_HOME=%MINGW_HOME%\msys\1.0
if exist "%MSYS_HOME%" goto BUILD

:MSYS_NOT_FOUND
echo MSYS not found. Build failed!
goto END

:BUILD
echo Using Java found in %JAVA_HOME%
echo Using PMD found in %PMD_HOME%
echo Using MSYS found in %MSYS_HOME%
set ANT_HOME=%DITA_HOME%\tools\ant
set PATH=%JAVA_HOME%\bin;%ANT_HOME%\bin;%MSYS_HOME%\bin;%PATH%
set CLASSPATH=%CLASSPATH%;%PMD_HOME%\lib\pmd-5.0-alpha.jar
call ant -Dpmd.dir="%PMD_HOME%" -v -f build.xml

:END
pause
exit 0
