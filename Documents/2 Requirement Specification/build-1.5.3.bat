set JAVA_DIR=C:\Program Files\Java\jdk1.6.0_27
set DITA_DIR=C:\Program Files\Syntext\Serna Free 4.3\plugins\dita\DITA-OT1.5.3

set PATH=%JAVA_DIR%\bin;%DITA_DIR%\tools\ant\bin;%PATH%

set CLASSPATH=%CLASSPATH%;%DITA_DIR%\lib
set CLASSPATH=%CLASSPATH%;%DITA_DIR%\lib\dost.jar
set CLASSPATH=%CLASSPATH%;%DITA_DIR%\lib\commons-codec-1.4.jar
set CLASSPATH=%CLASSPATH%;%DITA_DIR%\lib\dost.jar
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

set ANT_HOME=%DITA_DIR%\tools\ant
set ANT_OPTS=-Xmx512m %ANT_OPTS%
set ANT_OPTS=%ANT_OPTS% -Djavax.xml.transform.TransformerFactory=net.sf.saxon.TransformerFactoryImpl

ant -Ddita.dir="%DITA_DIR%" -f build.xml all
