set JAVA_DIR=C:\Program Files\Java\jdk1.6.0_27
set DITA_DIR=C:\Program Files\Syntext\Serna Free 4.3\plugins\dita\DITA-OT1.4

set PATH=%JAVA_DIR%\bin;%DITA_DIR%\tools\ant\bin;%PATH%

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

set ANT_HOME=%DITA_DIR%\tools\ant
set ANT_OPTS=-Xmx512m %ANT_OPTS%

ant -Ddita.dir="%DITA_DIR%" -f build.xml all
