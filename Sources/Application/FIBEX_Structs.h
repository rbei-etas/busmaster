#include "Stdafx.h"
#include <string>
#include <map>
#include <list>
#include <afxtempl.h>
#import <msxml6.dll>
#include "DataTypes\MsgSignal_Datatypes.h"

using namespace std;

#define EMPTY_VALUE             -1

enum ENDIANNESS_ENUM
{
    INTEL_ENUM,
    MOTOROLA_ENUM
};

class CCompuMethodEx_FIBEX
{
public:
    //SRange            m_sNotValidRange;
    //SRange            m_sNotAvailableRange;
    COMPU_EXPRESSION_MSGSIG  m_eCompuType;
    COMPU_METHOD_EX   m_uMethod;

    // To get the engineering value
    //string omGetEnggValue(DWORD dwRawValue);
    bool omGetEnggValue(DWORD dwRawValue, string& omEnggValue); // TO BE REMOVED CHOU
    BOOL bGetEnggValue(DWORD dwRawValue, string& omEnggValue) const;
};

struct SPHYSICAL_TYPE_v2
{
public:
    /* One of ~
     * A_UINT8, A_INT8, A_UINT16, A_INT16, A_UINT32, A_INT32, A_UINT64, A_INT64,
     * A_FLOAT32, A_FLOAT64, A_ASCIISTRING, A_UNICODE2STRING, A_BYTEFIELD, A_BITFIELD,
     * OTHER */
    string    m_ouToken;
    float           m_fPrecision;

    SPHYSICAL_TYPE_v2();
    ~SPHYSICAL_TYPE_v2();
    void DoCleanup(void);

    HRESULT Load(MSXML2::IXMLDOMNodePtr& pIDomNode);
    HRESULT Process_Load(MSXML2::IXMLDOMNodePtr& pCurrChild);
    HRESULT DoIterate_ForLoad(MSXML2::IXMLDOMNodePtr& pCurrChild);
    // HRESULT Save(MSXML2::IXMLDOMElementPtr& pParentDomNode);
    //BOOL bTranslate(eENTITY_FIBEX eEntity, BYTE* Buffer, BOOL bToAbstract);
};
struct SMIN_MAX_LEN_v2
{
public:
    unsigned int m_unMinLen;
    unsigned int m_unMaxLen;
};
typedef CList<SMIN_MAX_LEN_v2, SMIN_MAX_LEN_v2&> CMIN_MAXList_v2;
struct SLENGTH_v2
{
public:
    unsigned int m_unLength;
    CMIN_MAXList_v2 m_odMinMaxList;

    SLENGTH_v2& operator=(SLENGTH_v2& sLength);
    SLENGTH_v2();
    ~SLENGTH_v2();
    void DoCleanup(void);
};

struct SCODED_TYPE_v2
{
public:
    /**
     * One of ~
     * A_UINT8, A_INT8, A_UINT16, A_INT16, A_UINT32, A_INT32, A_UINT64, A_INT64,
     * A_FLOAT32, A_FLOAT64, A_ASCIISTRING, A_UNICODE2STRING, A_BYTEFIELD, A_BITFIELD,
     * OTHER
     */
    string m_ouDataType;
    /**
     * One of ~
     * LEADING-LENGTH-INFO-TYPE, END-OF-PDU, MIN-MAX-LENGTH-TYPE, STANDARD-LENGTH-TYPE
     */
    string m_ouCategory;
    /**
     * One of ~
     * SIGNED, UNSIGNED, BIT, IEEE-FLOATING-TYPE, BCD, DSP-FRACTIONAL, SM, BCD-P, BCD-
     * UP,1C, 2C, UTF-8, UCS-2, ISO-8859-1, ISO-8859-2, WINDOWS-1252
     */
    string m_ouEncoding;
    /**
     * One of ~
     * NONE, ZERO, HEX-FF, LENGTH
     */
    string m_ouTermination;

    BYTE m_bChoice;// choice between BIT_LEN or Sequence of MIN_MAX_LEN
    SLENGTH_v2 m_sLength;

    SCODED_TYPE_v2();
    ~SCODED_TYPE_v2();
    void DoCleanup(void);
    SCODED_TYPE_v2& operator=(SCODED_TYPE_v2& sCodedType);
    HRESULT Load(MSXML2::IXMLDOMNodePtr& pIDomNode);
    HRESULT Process_Load(MSXML2::IXMLDOMNodePtr& pCurrChild);
    HRESULT DoIterate_ForLoad(MSXML2::IXMLDOMNodePtr& pCurrChild);
    //HRESULT Save(MSXML2::IXMLDOMElementPtr& pParentDomNode);
    // BOOL bTranslate(eENTITY_FIBEX eEntity, BYTE* Buffer, BOOL bToAbstract);
};


struct SCOMPANY_REV_INFO_v2
{
    string m_omRev_Level;
    string m_omState;
    string m_omCompany_Data_Ref;

    HRESULT Load(MSXML2::IXMLDOMNodePtr& pIDomNode);
    HRESULT Process_Load(MSXML2::IXMLDOMNodePtr& pCurrChild);
    HRESULT DoIterate_ForLoad(MSXML2::IXMLDOMNodePtr& pCurrChild);
    SCOMPANY_REV_INFO_v2();
    void DoCleanup(void);
    //HRESULT Save(MSXML2::IXMLDOMElementPtr& pParentNode);
};
typedef CList<SCOMPANY_REV_INFO_v2, SCOMPANY_REV_INFO_v2&> CCompanyRevInfoList_v2;

struct SMODIFICATION_v2
{
    string m_omChange;
    string m_omReason;

    HRESULT Load(MSXML2::IXMLDOMNodePtr& pIDomNode);
    HRESULT Process_Load(MSXML2::IXMLDOMNodePtr& pCurrChild);
    HRESULT DoIterate_ForLoad(MSXML2::IXMLDOMNodePtr& pCurrChild);
    SMODIFICATION_v2();
    void DoCleanup(void);
    //HRESULT Save(MSXML2::IXMLDOMElementPtr& pParentNode);
};
typedef CList<SMODIFICATION_v2, SMODIFICATION_v2&> CModificationList_v2;

struct SELEMENT_REVISION_v2
{
    CCompanyRevInfoList_v2  m_odCompanyRevInfoList;
    CModificationList_v2    m_odModificationList;
    string              m_omDate;
    string              m_omREVISION_LABEL;
    string              m_omSTATE;
    string              m_omTEAM_MEMBER_REF;

    SELEMENT_REVISION_v2();
    ~SELEMENT_REVISION_v2();
    void DoCleanup(void);
    SELEMENT_REVISION_v2& operator=(SELEMENT_REVISION_v2& RefObj);
    HRESULT Load(MSXML2::IXMLDOMNodePtr& pIDomNode);
    // HRESULT Save(MSXML2::IXMLDOMElementPtr& pParentNode);
    HRESULT Process_Load(MSXML2::IXMLDOMNodePtr& pCurrChild);
    HRESULT DoIterate_ForLoad(MSXML2::IXMLDOMNodePtr& pCurrChild);

};
typedef CList<SELEMENT_REVISION_v2, SELEMENT_REVISION_v2&> CElementRevList_v2;

struct CSignal
{
    string m_omSigId;
    string m_omShortName;
    string m_omdeDesc;
    string m_omenDesc;
    string m_omCodingRef;

    //map<string, CCODING> m_mapCoding;
};

struct CSigInstance
{
    string m_omSigInstId;
    UINT m_unBitPos;
    BOOL m_bByteOrder;
    string m_omSigRef;
};

struct CAbsSchdTiming
{
    UINT m_nSlotId;
    UINT m_nBaseCycle;
    UINT m_nCycleRepetition;
};

struct CFrameTrig
{
    string m_omFramTrigId;
    string m_omFrameRef;

    int m_nCurrentSlot;

    // Slot Id to Absolute scheduled timing
    map<int, CAbsSchdTiming> mapAbsSchdTiming;
    ~CFrameTrig()
    {

    }

};

struct CSwitch
{
    string m_omSwitchName;
    int m_nSwitchBitPos;
    ENDIANNESS_ENUM m_ouSwitchByteOrder;
    int m_nSwitchBitLen;

    CSwitch()
    {
        m_omSwitchName = "";
        m_nSwitchBitPos = -1;
        m_nSwitchBitLen = 0;
    }
    ~CSwitch()
    {

    }

};

struct CDynPart
{
    string m_ouDynPartId;
    string m_ouDynPartName;
    int m_nDynPartBitPos;
    ENDIANNESS_ENUM m_ouDynPartByteOrder;
    int m_nDynPartBitLen;

    string m_ouCurrSwitchCode;
    string m_ouCurrPduRef;

    // Switch code to Pdu object
    map<string, string> m_mapSwitchToPdu;
    ~CDynPart()
    {

    }

};

struct CStaticPart
{
    string m_ouStaticPartId;
    string m_ouStaticPartName;
    int m_StaticPartBitPos;
    ENDIANNESS_ENUM m_ouStPartByteOrder;
    int m_nStPartBitLen;

    // PduId to Pdu object
    map<string, string> m_mapPduIdToPdu;
    ~CStaticPart()
    {

    }

};

struct CPdu
{
public:
    string m_strPduId;
    string m_strPDUName;
    int m_nStartBit;
    int m_nLength;
    string m_strPduType;
    ENDIANNESS_ENUM m_ouEndianness;
    BOOL m_bIsStaticPart;
    CSwitch m_ouMuxSwitch;
    CDynPart m_ouDynPart;
    CStaticPart m_ouStaticPart;
    map<string, CSigInstance> m_mapSigInstDetails;

    string m_omCurrSigInstId;
    ~CPdu()
    {

    }

};

struct CPdu_Instance
{
public:
    string m_strPduId;
    int m_nStartBit;
    string m_omPduRef;
    ENDIANNESS_ENUM m_ouEndianess;
    map<string, CSigInstance> m_mapSigInstDetails;
    ~CPdu_Instance()
    {

    }

};

struct CFrame
{
    string m_omFrameId;
    string m_omShortName;
    UINT m_unFrameLen;
    string m_omFrameType;

    string m_omCurrPduId;
    // For Filling data structure
    string m_omCurrSigInstId;

    CPdu m_objPdu;

    CFrame();
    ~CFrame();
    CFrame(CFrame& RefObj);

    CFrame& operator=(CFrame& RefObj);
    //CList<CSigInstance, CSigInstance> m_lstSigInstance;

    map<string, CPdu_Instance> m_mapPduInstList;
    map<string, CSigInstance> m_mapSigInstDetails;
};

struct CFrameTriggering
{
    string m_omFrameRef;
    map<string,string> m_mapSigInst;
    ~CFrameTriggering()
    {

    }

};

struct CConnector
{
    string m_omChnlRef;
    string m_omCntrlRef;
    string m_omCurrFrameTrigId; // Used to fill in the list

    map<string, CFrameTriggering> m_mapRxFrameTrigs;
    map<string, CFrameTriggering> m_mapTxFrameTrigs;

    void clear()
    {
        m_omChnlRef = "";
        m_omCntrlRef = "";
        m_omCurrFrameTrigId = "";
        m_mapRxFrameTrigs.clear();
        m_mapTxFrameTrigs.clear();
    }

    CConnector();
    ~CConnector();
    CConnector(CConnector& objRef);

    CConnector& operator=(CConnector& objRef);
};

// ECU
typedef struct tagSKEY_SLOT_USAGE
{
    int m_nStartUpSync;
    int m_nSync;
    string m_omNone;

} KEY_SLOT_USAGE;

struct CController
{
    CController();
    ~CController();
    CController(CController& Refobj);

    CController& operator=(CController& Refobj);

    void clear()
    {
        memset(this, 0, sizeof(CController));
        /*m_sKeySlotUsage.m_nStartUpSync  = EMPTY_VALUE;
        m_sKeySlotUsage.m_nSync = EMPTY_VALUE;
        m_sKeySlotUsage.m_omNone = EMPTY_VALUE;
        m_shMaxDynPayloadLgt  = EMPTY_VALUE;

        m_nKeySlotId = EMPTY_VALUE;

        m_shClusterDriftDamping  = EMPTY_VALUE;


        m_nDecodingCorr  = EMPTY_VALUE;



        m_nListenTimeOut  = EMPTY_VALUE;


        m_shMaxDrift  = EMPTY_VALUE;


        m_shExternOffsetCorr  = EMPTY_VALUE;


        m_shExternRateCorr  = EMPTY_VALUE;


        m_shLatestTx  = EMPTY_VALUE;


        m_nMicroPreCycle  = EMPTY_VALUE;


        m_shOffsetCorrOut  = EMPTY_VALUE;


        m_shRateCorrOut  = EMPTY_VALUE;


        m_shSamplePerMicrotick  = EMPTY_VALUE;


        m_shDelayCompensationA  = EMPTY_VALUE;


        m_shDelayCompensationB  = EMPTY_VALUE;


        m_shWakeUpPattern  = EMPTY_VALUE;


        m_bAllowHaltDewToClock  = false;


        m_shAllowPassiveToActive  = EMPTY_VALUE;


        m_shAcceptedStartUpRange  = EMPTY_VALUE;


        m_shMacroInitialOffsetA  = EMPTY_VALUE;


        m_shMacroInitialOffsetB  = EMPTY_VALUE;


        m_shMicroInitialOffsetA  = EMPTY_VALUE;


        m_shMicroInitialOffsetB  = EMPTY_VALUE;


        m_bSingleSlotEnable  = false;
        m_fMicrotick  = EMPTY_VALUE;
        m_fMicroPerMacroNom  = EMPTY_VALUE;*/
    }

    /*int m_nStartUpSync;
    int m_nSync;
    string m_omNone;*/
    KEY_SLOT_USAGE  m_sKeySlotUsage;

    short               m_shMaxDynPayloadLgt;
    /**
    */
    short               m_shClusterDriftDamping;
    /**
    */
    int                 m_nDecodingCorr;
    /**
    *
    */
    int                 m_nListenTimeOut;
    /**
    */
    short               m_shMaxDrift;
    /**
    */
    short               m_shExternOffsetCorr;
    /**
    */
    short               m_shExternRateCorr;
    /**
    */
    short               m_shLatestTx;
    /**
    */
    int                 m_nMicroPreCycle;
    /**
    */
    short               m_shOffsetCorrOut;
    /**
    */
    short               m_shRateCorrOut;
    /**
    */
    short               m_shSamplePerMicrotick;
    /**
    */
    short               m_shDelayCompensationA;
    /**
    */
    short               m_shDelayCompensationB;
    /**
    */
    short               m_shWakeUpPattern;
    /**
    */
    bool                m_bAllowHaltDewToClock;
    /**
    */
    short               m_shAllowPassiveToActive;
    /**
    */
    short               m_shAcceptedStartUpRange;
    /**
    */
    short               m_shMacroInitialOffsetA;
    /**
    */
    short               m_shMacroInitialOffsetB;
    /**
    */
    short               m_shMicroInitialOffsetA;
    /**
    */
    short               m_shMicroInitialOffsetB;
    /**
    */
    bool                m_bSingleSlotEnable;
    float               m_fMicrotick;
    float               m_fMicroPerMacroNom;

    int                 m_nChannels;        //Channels to which the node is connected (A, B or both).

    int                 m_nKeySlotId;       //ID of the slot used to transmit the startup frame, sync frame, or designated single slot frame.

    int                 m_nWakeUpChannel;   //Channel used by the node to send a wakeup pattern (A or B).
};

struct CECU
{
    string m_omECUId;
    string m_omShortName;

    CECU();
    ~CECU();
    CECU(CECU& RefObj);

    CECU& operator=(CECU& RefObj);

    string m_strCurrConnectorRef;
    string m_strCurrControllerRef;

    // Map channel Ref to Connector details
    map<string, CConnector> m_mapConnector;
    map<string, CController> m_mapController;
    CController objController;
    CConnector  objConnector;
};

struct SWAKEUP
{
    short m_shWAKE_UP_SYMBOL_RX_IDLE;   // 14 to 59 inclusive
    short m_shWAKE_UP_SYMBOL_RX_LOW;    // 10 to 55 inclusive
    short m_shWAKE_UP_SYMBOL_RX_WINDOW; // 76 to 301 inclusive
    short m_shWAKE_UP_SYMBOL_TX_IDLE;   // 45 to 180 inclusive
    short m_shWAKE_UP_SYMBOL_TX_LOW;    // 15 to 60 inclusive
};


typedef struct tagSFlexrayCluster
{
    short m_shCOLD_START_ATTEMPTS;
    short m_shACTION_POINT_OFFSET;
    short m_shDYNAMIC_SLOT_IDLE_PHASE;
    short m_shMINISLOT;
    short m_shMINISLOT_ACTION_POINT_OFFSET;
    short m_shNIT;
    /**
    * float enum - .05,.025,.0125
    */
    float m_fSAMPLE_CLOCK_PERIOD;
    /**
    * 4 to 659 inclusive
    */
    short m_shSTATIC_SLOT;
    /**
    * 0 to 139 inclusive
    */
    short m_shSYMBOL_WINDOW;
    /**
    * 3 to 15 inclusive
    */
    short m_shTSS_TRANSMITTER;
    SWAKEUP m_ouWAKEUP;
    /**
    * 2 to 16 inclusive
    */
    short m_shLISTEN_NOISE;
    /**
    * 10 to 16000 inclusive
    */
    short m_shMACRO_PER_CYCLE;
    /**
    * Min 1
    */
    float m_fMACROTICK;
    /**
    * 0 to 11.7 inclusive
    */
    float m_fMAX_INITIALIZATION_ERROR;
    /**
    * 1 to 15
    */
    short m_shMAX_WITHOUT_CLOCK_CORRECTION_FATAL;
    /**
    * 1 to 15
    */
    short m_shMAX_WITHOUT_CLOCK_CORRECTION_PASSIVE;
    /**
    * Min 0, default 0, 0 to 12
    */
    short m_shNETWORK_MANAGEMENT_VECTOR_LENGTH;
    /**
    * 0 to 7986
    */
    short m_shNUMBER_OF_MINISLOTS;
    /**
    * 2 to 1023
    */
    short m_shNUMBER_OF_STATIC_SLOTS;
    /**
    * 9 to 15999
    */
    short m_shOFFSET_CORRECTION_START;
    /**
    * 0 to 127
    */
    short m_shPAYLOAD_LENGTH_STATIC;
    /**
    * 2 to 15
    */
    short m_shSYNC_NODE_MAX;
    /**
    * Min. 0, short 67 to 99
    */
    short m_shCAS_RX_LOW_MAX;
    /**
    * Min 0, float enum .1,.2 or .4
    */
    float m_fBIT;
    /**
    * 10 - 16000
    */
    short m_shCYCLE;
    /**
    * 0 to 5
    */
    short m_shCLUSTER_DRIFT_DAMPING;
    void DoCleanup();

} FLEXRAY_CLUSTER;

struct CClusterv2
{
    string m_omClusterId;
    string m_omProjectType;
    string m_omFlxClusterName;

    map<string, string> m_mapChnls;
    BOOL m_bFlexray;
    FLEXRAY_CLUSTER m_absFlxCluster;

    CClusterv2& operator=(CClusterv2& RefObj)
    {
        //m_ouWAKEUP = RefObj.m_ouWAKEUP;
        m_omClusterId = RefObj.m_omClusterId;
        m_omProjectType = RefObj.m_omProjectType;
        m_omFlxClusterName = RefObj.m_omFlxClusterName;
        m_mapChnls = RefObj.m_mapChnls;
        m_bFlexray = RefObj.m_bFlexray;
        m_absFlxCluster = RefObj.m_absFlxCluster;
        return *this;
    }

    CClusterv2()
    {
        m_bFlexray = FALSE;
    }
    ~CClusterv2()
    {
    }
};