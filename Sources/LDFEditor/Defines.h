#pragma once

#define LIN_MAX_FRAME_TYPES             4
#define LIN_UNCONDITIONAL_FRAME_INDEX   0
#define LIN_EVENT_FRAME_INDEX           1
#define LIN_SPORADIC_FRAME_INDEX        2
#define LIN_DIAGNOSTIC_FRAME_INDEX      3

#define LIN_MAX_SIGNAL_TYPES            2
#define LIN_NORMAL_SIGNAL               0
#define LIN_DIAGNOSTICS_SIGNAL          1

#define LIN_MAX_ECU_TYPES               2
#define LIN_MASTER_ECU                  0
#define LIN_SLAVE_ECU                   1

#define MAX_UNCOND_FRAME_ID             63

#define defECU_ICON_INDEX               0
#define defFRAME_ICON_INDEX             1
#define defSIGNAL_ICON_INDEX            2
#define defCODING_ICON_INDEX            3
#define defSCHEDULETABLE_ICON_INDEX     4
#define defSIGNAL_RX_ICON_INDEX         5
#define defSIGNAL_TX_ICON_INDEX         6
#define defFRAME_RX_ICON_INDEX          7
#define defFRAME_TX_ICON_INDEX          8
#define defNODE_COMPOSITION_INDEX       9
#define def_ICON_TOTAL                  10
#define defMAX_SIGNAL_STARTBIT          63
#define defCLASSIC_CHECKSUM     "Classic"
#define defENHANCED_CHECKSUM    "Enhanced"
#define defNEW                  "&New"
#define defEDIT                 "&Edit"
#define defDELETE               "&Delete"
#define defADD_DIAGSUPPORT      "&Add Diagnostic Support"
#define defREMOVE_DIAGSUPPORT   "&Remove Diagnostic Support"

#define defLINMasterFrameName   "MasterReq"
#define defLINSlaveFrameName    "SlaveResp"
#define defLINMasterSignalName  "MasterReqB%d"
#define defLINSlaveSignalName   "SlaveRespB%d"
#define defNONE                 " "
enum eMode
{
    eNew,
    eEdit
};

enum eHeaderPopupMenus
{
    eLinClusterHeaderPopup,
    eSlaveHeaderPopup,
    eUnconditionalFrameHeaderPopup,
    eEventTriggeredFrameHeaderPopup,
    eSporadicFrameHeaderPopup,
    eDiagnosticFrameHeaderPopup,
    eSignalHeaderPopup,
    eCodingHeaderPopup,
    eScheduleTableHeaderPopup,
    eSignalGroupHeaderPopUp,
    eNodeConfigHeaderPopup,
    eTotalHeaderPopup
};

#define defTOTAL_ELEMENT_ACTIONS        27
enum eElementPopupMenus
{
    eMasterElementPopup,
    eSlaveElementPopup,
    eUnconditionalFrameElementPopup,
    eEventTriggeredFrameElementPopup,
    eSporadicFrameElementPopup,
    eDiagnosticFrameElementPopup,
    eSignalElementPopup,
    eDiagnosticSignalElementPopup,
    eCodingElementPopup,
    eScheduleTableElementPopup,
    eSignalGroupElementPopup,
    eNodeConfigElementPopup,
    eTotalElementPopup
};
