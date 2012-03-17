@echo off

set JAVA_DIR=%ProgramFiles%\Java\jdk1.6.0_27
if exist "%JAVA_DIR%\lib\tools.jar" goto DITA

echo Java Development Kit not found. Build failed!
goto END

:DITA
set DITA_DIR=%ProgramFiles%\Syntext\Serna Free 4.3\plugins\dita\DITA-OT1.5.3
if exist "%DITA_DIR%" goto DITA_1_5_3

set DITA_DIR=%ProgramFiles%\Syntext\Serna Free 4.3\plugins\dita\DITA-OT1.4
if exist "%DITA_DIR%" goto DITA_1_4

echo DITA not found. Build failed!
goto END

:DITA_1_4
echo Please consider an upgrade to the recommended DITA version 1.5 or higher
set CLASSPATH=%CLASSPATH%;%DITA_DIR%\lib
set CLASSPATH=%CLASSPATH%;%DITA_DIR%\lib\dost.jar
set CLASSPATH=%CLASSPATH%;%DITA_DIR%\lib\resolver.jar
set CLASSPATH=%CLASSPATH%;%DITA_DIR%\lib\fop.jar
set CLASSPATH=%CLASSPATH%;%DITA_DIR%\lib\avalon-framework-cvs-20020806.jar
set CLASSPATH=%CLASSPATH%;%DITA_DIR%\lib\batik.jar
set CLASSPATH=%CLASSPATH%;%DITA_DIR%\lib\xalan.jar
set CLASSPATH=%CLASSPATH%;%DITA_DIR%\lib\xercesImpl.jar
set CLASSPATH=%CLASSPATH%;%DITA_DIR%\lib\xml-apis.jar
set CLASSPATH=%CLASSPATH%;%DITA_DIR%\lib\icu4j.jar
set ANT_OPTS=-Xmx512m %ANT_OPTS%
goto BUILD

:DITA_1_5_3
set CLASSPATH=%CLASSPATH%;%DITA_DIR%\lib
set CLASSPATH=%CLASSPATH%;%DITA_DIR%\lib\dost.jar
set CLASSPATH=%CLASSPATH%;%DITA_DIR%\lib\commons-codec-1.4.jar
set CLASSPATH=%CLASSPATH%;%DITA_DIR%\lib\resolver.jar
set CLASSPATH=%CLASSPATH%;%DITA_DIR%\lib\icu4j.jar
set CLASSPATH=%CLASSPATH%;%DITA_DIR%\lib\saxon
set CLASSPATH=%CLASSPATH%;%DITA_DIR%\lib\saxon\saxon9.jar
set CLASSPATH=%CLASSPATH%;%DITA_DIR%\lib\saxon\saxon9-dom.jar
set CLASSPATH=%CLASSPATH%;%DITA_DIR%\lib\saxon\saxon9-dom4j.jar
set CLASSPATH=%CLASSPATH%;%DITA_DIR%\lib\saxon\saxon9-jdom.jar
set CLASSPATH=%CLASSPATH%;%DITA_DIR%\lib\saxon\saxon9-s9api.jar
set CLASSPATH=%CLASSPATH%;%DITA_DIR%\lib\saxon\saxon9-sql.jar
set CLASSPATH=%CLASSPATH%;%DITA_DIR%\lib\saxon\saxon9-xom.jar
set CLASSPATH=%CLASSPATH%;%DITA_DIR%\lib\saxon\saxon9-xpath.jar
set CLASSPATH=%CLASSPATH%;%DITA_DIR%\lib\saxon\saxon9-xqj.jar
set ANT_OPTS=-Xmx512m %ANT_OPTS%
set ANT_OPTS=%ANT_OPTS% -Djavax.xml.transform.TransformerFactory=net.sf.saxon.TransformerFactoryImpl
goto BUILD

:BUILD
echo Using JDK found in %JAVA_DIR%
echo Using DITA found in %DITA_DIR%
set ANT_HOME=%DITA_DIR%\tools\ant
set PATH=%JAVA_DIR%\bin;%ANT_HOME%\bin;%PATH%
ant -Ddita.dir="%DITA_DIR%" -f build.xml all

:END
