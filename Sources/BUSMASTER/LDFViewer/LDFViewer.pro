QT += core widgets

INCLUDEPATH += ../../Kernel/ProtocolDefinitions

RESOURCES += \
    LDFViewer.qrc

FORMS += \
    LDFViewer.ui

HEADERS += \
    LDFHighlighter.h \
    LDFViewer.h

SOURCES += \
    LDFHighlighter.cpp \
    main.cpp \
    LFDViewer.cpp
