#!/bin/sh

JAVA_DIR=/usr/lib/jvm/java-6-sun
DITA_DIR=/opt/DITA-OT1.5.3

export PATH="$JAVA_DIR/bin":"$DITA_DIR/tools/ant/bin":"$PATH"

export CLASSPATH="$CLASSPATH":"$DITA_DIR/lib"
export CLASSPATH="$CLASSPATH":"$DITA_DIR/lib/dost.jar"
export CLASSPATH="$CLASSPATH":"$DITA_DIR/lib/commons-codec-1.4.jar"
export CLASSPATH="$CLASSPATH":"$DITA_DIR/lib/resolver.jar"
export CLASSPATH="$CLASSPATH":"$DITA_DIR/lib/icu4j.jar"
export CLASSPATH="$CLASSPATH":"$DITA_DIR/lib/saxon/saxon9.jar"
export CLASSPATH="$CLASSPATH":"$DITA_DIR/lib/saxon/saxon9-dom.jar"
export CLASSPATH="$CLASSPATH":"$DITA_DIR/lib/saxon/saxon9-dom4j.jar"
export CLASSPATH="$CLASSPATH":"$DITA_DIR/lib/saxon/saxon9-jdom.jar"
export CLASSPATH="$CLASSPATH":"$DITA_DIR/lib/saxon/saxon9-s9api.jar"
export CLASSPATH="$CLASSPATH":"$DITA_DIR/lib/saxon/saxon9-sql.jar"
export CLASSPATH="$CLASSPATH":"$DITA_DIR/lib/saxon/saxon9-xom.jar"
export CLASSPATH="$CLASSPATH":"$DITA_DIR/lib/saxon/saxon9-xpath.jar"
export CLASSPATH="$CLASSPATH":"$DITA_DIR/lib/saxon/saxon9-xqj.jar"

export ANT_HOME="$DITA_DIR/tools/ant"
export ANT_OPTS="-Xmx512m $ANT_OPTS"
export ANT_OPTS="$ANT_OPTS -Djavax.xml.transform.TransformerFactory=net.sf.saxon.TransformerFactoryImpl"

ant -Ddita.dir="$DITA_DIR" -f build.xml all
