QT += core widgets

RC_FILE = LDFEditor.rc

INCLUDEPATH += ../../Kernel/ProtocolDefinitions

RESOURCES += \
    ldfeditor.qrc \
    LDFEditor.qrc \
    ldfeditor.qrc

FORMS += \
    AboutLDFEditor.ui \
    CodingDlg.ui \
    EventFrameEditDlg.ui \
    FaultSignalsDlg.ui \
    LDFCLusterPropsDlg.ui \
    LDFEditor.ui \
    MapSignalsDlg.ui \
    MasterEditDlg.ui \
    NodeConfiguration.ui \
    ScheduleTableDlg.ui \
    SignalDlg.ui \
    SignalGroupDlg.ui \
    SlaveDlg_layout.ui \
    SlaveDlg.ui \
    SporadicFrameEditDlg.ui \
    UnconditionalFrameEditDlg.ui

DISTFILES += \
    LDFEditor.ico \
    LDFEditor.rc \
    LDFEditor.sln \
    LDFEditor.vcxproj \
    LDFEditor.vcxproj.filters

HEADERS += \
    AboutLDFEditor.h \
    CodingDlg.h \
    Defines.h \
    EventFrameEditDlg.h \
    FaultSignalsDlg.h \
    ldfadditionalview.h \
    LDFCLusterPropsDlg.h \
    LDFCommonTableWidget.h \
    LDFDatabaseManager.h \
    LDFDefines.h \
    LDFElementView.h \
    LDFNetworkView.h \
    LDFPropertyView.h \
    LDFTableWidget.h \
    LDFUtility.h \
    LineEditWidget.h \
    MapSignalsDlg.h \
    MasterEditDlg.h \
    NodeConfigurationDlg.h \
    resource.h \
    ScheduleComboWidget.h \
    ScheduleTableDlg.h \
    SignalDlg.h \
    SignalGroupDlg.h \
    SlaveDlg.h \
    SporadicFrameEditDlg.h \
    UnconditionalFrameEditDlg.h \
    LDFEditor.h

SOURCES += \
    AboutLDFEditor.cpp \
    CodingDlg.cpp \
    EventFrameEditDlg.cpp \
    FaultSignalsDlg.cpp \
    ldfadditionalview.cpp \
    LDFCLusterPropsDlg.cpp \
    LDFCommonTableWidget.cpp \
    LDFDatabaseManager.cpp \
    LDFElementView.cpp \
    LDFNetworkView.cpp \
    LDFPropertyView.cpp \
    LDFTableWidget.cpp \
    LDFUtility.cpp \
    LineEditWidget.cpp \
    main.cpp \
    MapSignalsDlg.cpp \
    MasterEditDlg.cpp \
    NodeConfigurationDlg.cpp \
    ScheduleComboWidget.cpp \
    ScheduleTableDlg.cpp \
    SignalDlg.cpp \
    SignalGroupDlg.cpp \
    SlaveDlg.cpp \
    SporadicFrameEditDlg.cpp \
    UnconditionalFrameEditDlg.cpp \
    LDFEditor.cpp
