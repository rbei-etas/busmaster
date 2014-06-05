/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file      FEALData.h
 * \brief     Definition file for FIBEX data types.
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Definition file for FIBEX data types.
 */

#pragma once

#include <afxtempl.h>
#include "Typecode_fbx_entities.h"

/* C++ includes */
#include <string>
#include <vector>
#include <list>

#define MAX_VAL_OF_BYTE         256

enum CATEGORY_GROUP { CATERGORY_NONE = 0};

typedef struct tagAbsSWAKEUP
{
    short m_shWAKE_UP_SYMBOL_RX_IDLE;   // 14 to 59 inclusive
    short m_shWAKE_UP_SYMBOL_RX_LOW;    // 10 to 55 inclusive
    short m_shWAKE_UP_SYMBOL_RX_WINDOW; // 76 to 301 inclusive
    short m_shWAKE_UP_SYMBOL_TX_IDLE;   // 45 to 180 inclusive
    short m_shWAKE_UP_SYMBOL_TX_LOW;    // 15 to 60 inclusive
    void DoCleanup();
} ABS_WAKEUP, *PABS_WAKEUP;

typedef struct tagAbsSFlexrayCluster
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
    ABS_WAKEUP m_ouWAKEUP;

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

} ABS_FLEXRAY_CLUSTER, *PABS_FLEXRAY_CLUSTER;

typedef struct tagAbsSBusInfo
{
    PABS_FLEXRAY_CLUSTER m_pouFlexRayCluster;

} tagAbsUBusInfo;

// CLUSTER
typedef struct tagAbsSCluster
{
    CString         m_omClusterID;
    CString         m_omClusterName;
    unsigned long   m_ulSpeed;
    bool            m_bIS_HIGH_LOW_BIT_ORDER;
    CString         m_omBIT_COUNTING_POLICY; // MONOTONE or SAWTOOTH
    CString         m_omProtocol;
    CStringArray    m_omChannelRefs;
    // Protocol specific data
    tagAbsUBusInfo  m_ouBusInfo;

    tagAbsSCluster();
    ~tagAbsSCluster();
    tagAbsSCluster& operator=(tagAbsSCluster& RefObj);
    void DoCleanup();

} ABS_CLUSTER, *PABS_CLUSTER;

typedef CList<ABS_CLUSTER, ABS_CLUSTER&> AbsCClusterList;

// CHANNEL

typedef union tagAbsUIDENTIFIER
{
    long     m_lIDENTIFIER_VALUE;
    char     m_acMFR_ID_EXTN[256];
} ABS_UIDENTIFIER;

typedef struct tagAbsSSIGNAL_TRIGGER
{
    /**
     * One of -
     * CHANNEL_ACTIVE_ON, CHANNEL_ACTIVE_OFF, CLAMP_15_ON, CLAMP_15_OFF, CLAMP_30_ON,
     * CLAMP_30_OFF, CLAMP_87_ON, CLAMP_87_OFF, CLAMP_RADIO_ON, CLAMP_RADIO_OFF,
     * KEY_PRESSED, KEY_RELEASED, NO_OTHER_TRIGGERING_RUNNING,
     * OTHER_TRIGGERING_RUNNING, NONE, OTHER
     */
    CString m_omSIGNAL_STATE;
    CString m_omSIGNAL_INSTANCE;

} ABS_SIGNAL_TRIGGER, *PABS_SIGNAL_TRIGGER;

typedef struct tagAbsSCONDITION
{
    /**
     * One of -
     * CHANNEL_ACTIVE_ON, CHANNEL_ACTIVE_OFF, CLAMP_15_ON, CLAMP_15_OFF, CLAMP_30_ON,
     * CLAMP_30_OFF, CLAMP_87_ON, CLAMP_87_OFF, CLAMP_RADIO_ON, CLAMP_RADIO_OFF,
     * KEY_PRESSED, KEY_RELEASED, NO_OTHER_TRIGGERING_RUNNING,
     * OTHER_TRIGGERING_RUNNING, NONE, OTHER
     */
    CString           m_omSYSTEM_STATE;
    ABS_SIGNAL_TRIGGER   m_sSignal_Trigger;

} ABS_CONDITION, *PABS_CONDITION;

typedef struct tagAbsSTIME_RANGE
{
    CString m_omValue;
    CString m_omMinValue;
    CString m_omMaxValue;

} ABS_TIME_RANGE, *PABS_TIME_RANGE;

typedef struct tagAbsSCYCLIC_TIMING
{
    ABS_TIME_RANGE    m_sREPEATING_TIME_RANGE;
    ABS_TIME_RANGE    m_sSTARTING_TIME_RANGE;
    /**
     * One of -
     * CHANNEL_ACTIVE, CLAMP_15, CLAMP_30, CLAMP_87, CLAMP_RADIO, NONE, OTHER
     */
    CString             m_omACTIVE_CONDITION;
    ABS_CONDITION       m_sSTART_CONDITION;
    ABS_CONDITION       m_sEND_CONDITION;
    unsigned short m_ushFINAL_REPETITIONS;

} ABS_CYCLIC_TIMING, *PABS_CYCLIC_TIMING;

typedef struct tagAbsSREQUEST_CONTROLLED_TIMING
{
    ABS_TIME_RANGE    m_sRESPONSE_TIME_RANGE;
    /**
     * One among -
     * CHANNEL_ACTIVE, CLAMP_15, CLAMP_30, CLAMP_87, CLAMP_RADIO, NONE, OTHER
     */
    CString        m_omACTIVE_CONDITION;
    unsigned short m_ushFINAL_REPETITIONS;
} ABS_REQUEST_CONTROLLED_TIMING, *PABS_REQUEST_CONTROLLED_TIMING;

typedef struct tagAbsSEVENT_CONTROLLED_TIMING
{
    ABS_TIME_RANGE    m_sDEBOUNCE_TIME_RANGE;
    /**
     * One among -
     * CHANNEL_ACTIVE, CLAMP_15, CLAMP_30, CLAMP_87, CLAMP_RADIO, NONE, OTHER
     */
    CString        m_omACTIVE_CONDITION;
    ABS_CONDITION     m_sSEND_CONDITION;
    unsigned short m_ushFINAL_REPETITIONS;
} ABS_EVENT_CONTROLLED_TIMING, *PABS_EVENT_CONTROLLED_TIMING;

typedef struct tagAbsSRELATIVELY_SCHEDULED_TIMING
{
    CString        m_omSCHEDULE_TABLE_NAME;
    short          m_shPOSITION_IN_TABLE;
    CString        m_omDelay;
} ABS_RELATIVELY_SCHEDULED_TIMING, *PABS_RELATIVELY_SCHEDULED_TIMING;

typedef struct tagAbsSBASE_CYCLE
{
    short   m_shBaseCycle;
    short   m_shCycleRepetition;
    CString m_omCycleRepetitionType;
} ABS_SBASE_CYCLE;

typedef struct tagAbsSCYCLE
{
    short           m_shCycleCounter;
    tagAbsSBASE_CYCLE m_sBaseCycle;
} ABS_SCYCLE;

typedef struct tagAbsSABSOLUTELY_SCHEDULED_TIMING
{
    int            m_nSLOT_ID;
    CString        m_omSlotIdType;
    tagAbsSCYCLE     m_uCycle;
} ABS_ABSOLUTELY_SCHEDULED_TIMING, *PABS_ABSOLUTELY_SCHEDULED_TIMING;

typedef struct tagAbsSTIMINGS
{
    ABS_CYCLIC_TIMING                   m_sCyclicTiming;
    ABS_REQUEST_CONTROLLED_TIMING       m_sReqCntrlTiming;
    ABS_EVENT_CONTROLLED_TIMING         m_sEventCntrlTiming;
    BYTE                                m_byFrameTimeMode; // Choice between Relative and Absolute
    ABS_RELATIVELY_SCHEDULED_TIMING     m_sRelScheduledTiming;
    ABS_ABSOLUTELY_SCHEDULED_TIMING     m_sAbsScheduledTiming;
} ABS_TIMINGS, *PABS_TIMINGS;

typedef CList<ABS_TIMINGS, ABS_TIMINGS&> AbsCFrameTimingList;

typedef struct tagAbsSFRAME_TRIGGERING
{
    CString             m_omID;
    AbsCFrameTimingList m_odTimingList;
    CString             m_omFrameRef;
    ABS_UIDENTIFIER     m_uIdentifier;
    tagAbsSFRAME_TRIGGERING& operator=(tagAbsSFRAME_TRIGGERING& RefObj);
} ABS_FRAME_TRIGGERING, *PABS_FRAME_TRIGGERING;

typedef CList<ABS_FRAME_TRIGGERING, ABS_FRAME_TRIGGERING&> AbsCFrameTriggeringList;

typedef struct tagAbsSPDUTRIGGTIMING
{
    BYTE                                m_byTimeMode;
    ABS_CYCLIC_TIMING                  m_psCyclicTiming;
    ABS_EVENT_CONTROLLED_TIMING        m_psEventControlledTiming;
    ABS_REQUEST_CONTROLLED_TIMING      m_psRequestControlledTiming;

    tagAbsSPDUTRIGGTIMING& operator=(tagAbsSPDUTRIGGTIMING& RefObj);
} ABS_PDUTRIGGTIMING, *PABS_PDUTRIGGTIMING;

typedef CList<ABS_PDUTRIGGTIMING, ABS_PDUTRIGGTIMING&> AbsCPduTimingList;

typedef struct tagAbsSPDU_TRIGGERING
{
    CString             m_omID;
    AbsCPduTimingList   m_odTimingList;
    CString             m_omPDU_REF;
    tagAbsSPDU_TRIGGERING& operator=(tagAbsSPDU_TRIGGERING& RefObj);
} ABS_PDU_TRIGGERING, *PABS_PDU_TRIGGERING;

typedef CList<ABS_PDU_TRIGGERING, ABS_PDU_TRIGGERING&> AbsCPduTriggeringList;

typedef struct tagAbsSChannel
{
    CString                 m_omChannelID;
    CString                 m_omShortName;
    CString                 m_omChannelName;

    AbsCFrameTriggeringList m_odFrameTriggeringList;
    AbsCPduTriggeringList  m_pPDUTriggeringList;
    tagAbsSChannel();
    ~tagAbsSChannel();
    tagAbsSChannel& operator=(tagAbsSChannel& RefObj);
    void DoCleanup();
} ABS_CHANNEL, *PABS_CHANNEL;

typedef CList<ABS_CHANNEL, ABS_CHANNEL&> AbsCChannelList;

// ECU
typedef struct tagAbsSKEY_SLOT_USAGE
{
    int m_nStartUpSync;
    int m_nSync;
    std::string m_omNone;

} ABS_KEY_SLOT_USAGE, *PABS_KEY_SLOT_USAGE;

typedef struct tagAbsSFLEXRAY_SPECIFIC_CTRL
{
    ABS_KEY_SLOT_USAGE  m_sKeySlotUsage;
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

    void DoCleanup();

} ABS_FLEXRAY_SPEC_CNTLR, *PABS_FLEXRAY_SPEC_CNTLR;


typedef struct tagAbsSDEVICECONFIG
{
    ABS_FLEXRAY_CLUSTER             m_ouFlxClusterConfig;
    ABS_FLEXRAY_SPEC_CNTLR          m_ouFlxSpecCntlr;
} ABS_DEVICE_CONFIG, *PABS_DEVICE_CONFIG;

typedef struct tagAbsSCONTROLLER
{
    CString                     m_omID;
    CString                     m_omType;
    CString                     m_omContrName;
    CString                     m_omVENDOR_NAME;
    CString                     m_omCHIP_NAME;
    unsigned long               m_ulTERMINATION_IMPEDANCE;
    CString                     m_omTRANSCEIVER;
    ABS_FLEXRAY_SPEC_CNTLR      m_sFlexraySpecControllerData;
    void  DoCleanup();

} ABS_CONTROLLER, *PABS_CONTROLLER;

typedef CList<ABS_CONTROLLER, ABS_CONTROLLER&> AbsCControllerList;

typedef struct tagBaseConnector
{
    CString      m_omID;
    CString      m_omType;
    CString      m_omCHANNEL_REF;
    CString      m_omCONTROLLER_REF;
} SBASE_CONNECTOR;

typedef std::vector<SBASE_CONNECTOR> AbsConnectorInfoList;
//typedef CList<SBASE_CONNECTOR, SBASE_CONNECTOR&> AbsConnectorInfoList;

typedef struct tagAbsSECU
{
    CString                 m_omECUID;
    CString                 m_omECUName;
    AbsCControllerList      m_odControllerList;
    PVOID                   m_odConnectorList;
    AbsConnectorInfoList    m_odConnectorInfo;
    tagAbsSECU();
    ~tagAbsSECU();
    tagAbsSECU& operator=(tagAbsSECU& RefObj);
    void DoCleanUp();

} ABS_ECU, *PABS_ECU;

typedef CList<ABS_ECU, ABS_ECU&> AbsCECUList;
//Common
typedef struct AbsSSTART_POS
{
    unsigned short m_ushBIT_POSITION;
    bool           m_bIS_HIGH_LOW_BYTE_ORDER;
} ABS_START_POS, *PABS_START_POS;

typedef union tagAbsUSTART_POS
{
    AbsSSTART_POS   m_sSigStartPos;
    unsigned short  m_ushSequenceNum;
} ABS_USTART_POS;
typedef struct tagAbsSSignal_Instance
{
    CString                 m_omID;
    tagAbsUSTART_POS        m_uStartPos;
    CString                 m_omSigRef;
    unsigned int            m_unSIG_UPDATE_BIT_POS;

} ABS_SIGNAL_INSTANCE, *PABS_SIGNAL_INSTANCE;

typedef CList<ABS_SIGNAL_INSTANCE, ABS_SIGNAL_INSTANCE&> AbsCSigInstanceList;

// FRAME

typedef struct tagAbsSPdu_Instance
{
    CString             m_omPduRef;
    tagAbsUSTART_POS    m_uSigStartPos;

} ABS_PDU_INSTANCE, *PABS_PDU_INSTANCE;

typedef CList<ABS_PDU_INSTANCE, ABS_PDU_INSTANCE&> AbsCPduInstanceList;

typedef struct tagAbsSFrame
{
    CString                 m_omFrameID;
    CString                 m_omFrameName;
    unsigned int            m_unByteLength;
    CString                 m_omFrameType;
    AbsCSigInstanceList     m_odSigInstList;
    AbsCPduInstanceList    m_podPduInstList;

    tagAbsSFrame();
    ~tagAbsSFrame();
    tagAbsSFrame& operator=(tagAbsSFrame& RefObj);
    void DoCleanup();

} ABS_FRAME, *PABS_FRAME;

typedef CList<ABS_FRAME, ABS_FRAME&> AbsCFrameList;

//SIGNAL
typedef struct tagAbsSSignal
{
    CString         m_omSignalID;
    CString         m_omSignalName;
    double          m_dDEFAULT_VALUE;
    CString         m_omCoding;
    unsigned int    m_unPriority;
    void DoCleanup();

} ABS_SIGNAL, *PABS_SIGNAL;

typedef CList<ABS_SIGNAL, ABS_SIGNAL&> AbsCSignalList;

// CODINGS
typedef struct tagAbsSPHYSICAL_TYPE
{
    /* One of ~
     * A_UINT8, A_INT8, A_UINT16, A_INT16, A_UINT32, A_INT32, A_UINT64, A_INT64,
     * A_FLOAT32, A_FLOAT64, A_ASCIISTRING, A_UNICODE2STRING, A_BYTEFIELD, A_BITFIELD,
     * OTHER */
    CString         m_omBaseType;
    float           m_fPrecision;

} ABS_PHYSICAL_TYPE, *PABS_PHYSICAL_TYPE;

typedef struct tagAbsSMIN_MAX_LEN
{
    unsigned int m_unMinLen;
    unsigned int m_unMaxLen;

} ABS_MIN_MAX_LEN, *PABS_MIN_MAX_LEN;

typedef CList<ABS_MIN_MAX_LEN, ABS_MIN_MAX_LEN&> AbsCMIN_MAXList;

typedef struct tagAbsSLENGTH
{
    unsigned int m_unLength;
    AbsCMIN_MAXList m_odMinMaxList;
} ABS_LENGTH, *PABS_LENGTH;

typedef struct tagAbsSCODED_TYPE
{
    /**
     * One of ~
     * A_UINT8, A_INT8, A_UINT16, A_INT16, A_UINT32, A_INT32, A_UINT64, A_INT64,
     * A_FLOAT32, A_FLOAT64, A_ASCIISTRING, A_UNICODE2STRING, A_BYTEFIELD, A_BITFIELD,
     * OTHER
     */
    CString m_omDataType;
    /**
     * One of ~
     * LEADING-LENGTH-INFO-TYPE, END-OF-PDU, MIN-MAX-LENGTH-TYPE, STANDARD-LENGTH-TYPE
     */
    CString m_omCategory;
    /**
     * One of ~
     * SIGNED, UNSIGNED, BIT, IEEE-FLOATING-TYPE, BCD, DSP-FRACTIONAL, SM, BCD-P, BCD-
     * UP,1C, 2C, UTF-8, UCS-2, ISO-8859-1, ISO-8859-2, WINDOWS-1252
     */
    CString m_omEncoding;
    /**
     * One of ~
     * NONE, ZERO, HEX-FF, LENGTH
     */
    CString m_omTermination;

    BYTE m_bChoice;// choice between BIT_LEN or Sequence of MIN_MAX_LEN
    ABS_LENGTH m_sLength;
    tagAbsSCODED_TYPE& operator=(tagAbsSCODED_TYPE& RefObj);
    void DoCleanup();

} ABS_CODED_TYPE, *PABS_CODED_TYPE;

typedef struct tagAbsSLIMIT
{
    /* One of - OPEN, CLOSED[default], INFINITE */
    CString m_omIntervalType;
    float   m_fValue;
} ABS_LIMIT, *PABS_LIMIT;

typedef struct tagAbsSCONSTRS
{
    /* One of -
     * VALID[default], NOT-VALID, NOT-AVAILABLE, NOT-DEFINED, ERROR, OTHER */
    CString     m_omValidity;
    ABS_LIMIT   m_sLowerLimit;
    ABS_LIMIT   m_sUpperLimit;

} ABS_CONSTRS, *PABS_CONSTRS;

typedef CList<ABS_CONSTRS, ABS_CONSTRS&> AbsCConstrList;

typedef struct tagAbsSCOMPU_CONST
{
    bool    m_bIsDouble;
    float   m_fV;
    CString m_omVT;

} ABS_SCOMPU_CONST, *PABS_SCOMPU_CONST;

typedef CList<float, float&> CFloatArray;

typedef struct tagAbsSCOMPU_RATIONAL_COEFFS
{
    CFloatArray m_faCompuNuma;
    CFloatArray m_faCompuDeno;
} ABS_COMPU_RATIONAL_COEFFS, *PABS_COMPU_RATIONAL_COEFFS;

typedef struct tagAbsSCOMPU_SCALE
{
    ABS_LIMIT                   m_sLower;
    ABS_LIMIT                   m_sUpper;
    ABS_SCOMPU_CONST            m_sCompuConst;
    ABS_COMPU_RATIONAL_COEFFS   m_sCompuRationalCoeffs;
    CString                     m_omCompuGenMath;

    tagAbsSCOMPU_SCALE& operator=(tagAbsSCOMPU_SCALE& RefObj);

} ABS_COMPU_SCALE, *PABS_COMPU_SCALE;

typedef CList<ABS_COMPU_SCALE, ABS_COMPU_SCALE&> AbsCCompuScaleList;

typedef struct tagAbsSCOMPU_INTERNAL_TO_PHYS
{
    AbsCCompuScaleList  m_odCompuScaleList;
    ABS_SCOMPU_CONST    m_sCompuDefValue;

    tagAbsSCOMPU_INTERNAL_TO_PHYS& operator=(tagAbsSCOMPU_INTERNAL_TO_PHYS& RefObj);

} ABS_COMPU_INTERNAL_TO_PHYS, *PABS_COMPU_INTERNAL_TO_PHYS;

typedef struct tagAbsSCompuMethod
{
    CString m_omID;
    CString m_omCompuName;
    /* One from -
     * IDENTICAL, LINEAR, SCALE-LINEAR, TEXTTABLE, TAB-NOINTP, FORMULA */
    CString                     m_omCategory;
    CString                     m_omUnitRef;
    AbsCConstrList              m_odPhysConstrs;
    AbsCConstrList              m_odInternalConstrs;
    ABS_COMPU_INTERNAL_TO_PHYS  m_sCompu_Internal_To_Phys;

    tagAbsSCompuMethod& operator=(tagAbsSCompuMethod& RefObj);

} ABS_COMPU_METHOD, *PABS_COMPU_METHOD;

typedef CList<ABS_COMPU_METHOD, ABS_COMPU_METHOD&> AbsCCompuMethodList;

typedef struct tagAbsSCoding
{
    CString             m_omCodingID;
    CString             m_omCodingName;
    // coded type, physical type and compu methods are optional
    ABS_PHYSICAL_TYPE   m_sPhysicalType;
    ABS_CODED_TYPE      m_sCodedType;
    AbsCCompuMethodList m_odCompuMethodList;

    tagAbsSCoding& operator=(tagAbsSCoding& RefObj);
    void DoCleanup();

} ABS_CODING, *PABS_CODING;

typedef CList<ABS_CODING, ABS_CODING&> AbsCCodingList;

// FUNCTION
typedef struct tagAbsSFUNCTION_PORT_TYPE
{
    CString     m_omID;
    CString     m_omName;
    CString     m_omSigRef;

} ABS_FUNCTION_PORT, *PABS_FUNCTION_PORT;

typedef CList<ABS_FUNCTION_PORT, ABS_FUNCTION_PORT&> AbsCFuncPortList;

typedef struct tagAbsSFunction
{
    CString             m_omFunctionID;
    CString             m_omFunctionName;
    AbsCFuncPortList    m_odInputPorts;
    AbsCFuncPortList    m_odOutputPorts;

    tagAbsSFunction& operator=(tagAbsSFunction& RefObj);
    void DoCleanup();

} ABS_FUNCTION, *PABS_FUNCTION;

typedef CList<ABS_FUNCTION, ABS_FUNCTION&> AbsCFunctionList;

// COMPOSITE
typedef struct tagAbsSComposite
{
    CString         m_omCompositeID;
    CString         m_omCompositeName;
    CString         m_omCompositionLevel;
    CStringArray    m_omRefValues;

    tagAbsSComposite& operator=(tagAbsSComposite& RefObj);
    void Docleanup();

} ABS_COMPOSITE, *PABS_COMPOSITE;

typedef CList<ABS_COMPOSITE, ABS_COMPOSITE&> AbsCCompositeList;

// PDU
typedef struct tagAbsSPdu
{
    CString             m_omPduID;
    CString             m_omPduName;
    UINT                m_unByteLen;
    CString             m_omPduType;
    AbsCSigInstanceList m_odSigInstList;

    tagAbsSPdu& operator=(tagAbsSPdu& RefObj);
    void DoCleanup();

} ABS_PDU, *PABS_PDU;

typedef CList<ABS_PDU, ABS_PDU&> AbsCPDUList;

// ELEMENT
typedef struct tagAbsSElement
{
    AbsCClusterList     m_omClusterList;
    AbsCChannelList     m_omChannelList;
    AbsCECUList         m_omECUList;
    AbsCFrameList       m_omFrameList;
    AbsCSignalList      m_omSignalList;
    AbsCFunctionList    m_omFunctionList;
    AbsCCompositeList   m_omCompositeList;
    AbsCPDUList         m_omPDUList;

    void DoCleanup();
    BOOL RemoveSignal(eENTITY_FIBEX eEntity, CString omID, CATEGORY_GROUP Category);
    BOOL RemoveCluster(eENTITY_FIBEX eEntity, CString omID, CATEGORY_GROUP Category);
    BOOL RemoveChannel(eENTITY_FIBEX eEntity, CString omID, CATEGORY_GROUP Category);
    BOOL RemoveECU(eENTITY_FIBEX eEntity, CString omID, CATEGORY_GROUP Category);
    BOOL RemoveFrame(eENTITY_FIBEX eEntity, CString omID, CATEGORY_GROUP Category);
    BOOL RemoveFunction(eENTITY_FIBEX eEntity, CString omID, CATEGORY_GROUP Category);
    BOOL RemoveComposite(eENTITY_FIBEX eEntity, CString omID, CATEGORY_GROUP Category);
    BOOL RemovePDU(eENTITY_FIBEX eEntity, CString omID, CATEGORY_GROUP Category);
    POSITION GetFramePos(CString omID);
    POSITION GetCompositePos(CString omID);
    POSITION GetECUPos(CString omID);
    POSITION GetChannelPos(CString omID);
    POSITION GetClusterPos(CString omID);
    POSITION GetSignalPos(CString omID);
    POSITION GetPDUPos(CString omID);
    POSITION GetFunctionPos(CString omID);
} ABS_ELEMENT, *PABS_ELEMENT;


// PROCESSING INFORMATION
typedef struct tagAbsSUnit
{
    CString     m_omID;
    CString     m_omUnitName;
    CString     m_omDisplayName;
    float       m_fFACTOR_SI_TO_UNIT;
    float       m_fOFFSET_SI_TO_UNIT;
    CString     m_omPHYSICAL_DIMENSION_REF;

} ABS_UNIT, *PABS_UNIT;

typedef CList<ABS_UNIT, ABS_UNIT&> AbsCUnitList;

typedef struct tagAbsSUnit_Spec
{
    AbsCUnitList m_odUnitList;

    void DoCleanup();

} ABS_UNIT_SPEC, *PABS_UNIT_SPEC;

typedef struct tagAbsSProcInfo
{
    AbsCCodingList  m_omCodingList;
    ABS_UNIT_SPEC   m_ouUnitSpec;

    void DoCleanup();

} ABS_PROC_INFO, *PABS_PROC_INFO;

// REQUIREMENTS
typedef struct tagAbsSRequirements
{

    void DoCleanup();

} ABS_REQUIREMENTS, *PABS_REQUIREMENTS;

// PROTOCOL
typedef struct tagAbsSProtocol
{
    UINT unProtocol; // ETYPE_BUS 3 - FLEXRAY
    void DoCleanup();

} ABS_PROTOCOL, *PABS_PROTOCOL;

// PROJECT
typedef struct tagAbsSProject
{
    CString      m_omID;
    CString      m_omProjectName;

    void DoCleanup();

} ABS_PROJECT, *PABS_PROJECT;

/* Customized structures */
typedef std::vector<CString> BaseControllerIDs;
/* Cluster - Controller Map */
typedef struct tagClusterInfo
{
    CString             m_omClusterID;
    BaseControllerIDs   m_omControllerIDs;

} ABS_CLUSTER_INFO, *PABS_CLUSTER_INFO;

typedef std::list<ABS_CLUSTER_INFO> AbsClusterInfoList;

// FIBEX CONTAINER
typedef struct tagAbsSFibexContainer
{
    ABS_PROJECT         m_omProject;
    ABS_PROTOCOL        m_omProtocol;
    ABS_ELEMENT         m_omElement;
    ABS_PROC_INFO       m_omProcInfo;
    ABS_REQUIREMENTS    m_omRequirements;

    CString             m_omstrFibexFilePath;

    void DoCleanup();

} ABS_FIBEX_CONTAINER, *PABS_FIBEX_CONTAINER;
