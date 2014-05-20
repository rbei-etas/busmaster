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
 * \file      MsgSignal_Datatypes.h
 * \brief     Definition file for signal watch data types.
 * \author    Pradeep Kadoor
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Definition file for signal watch data types.
 */

#if !defined MSGSIGNAL_H__INCLUDED_
#define MSGSIGNAL_H__INCLUDED_

#include "Include/BaseDefs.h"
#include <sstream>
#include <string>
#include <vector>
#include <list>

using namespace std;
//DB params
typedef struct _tagDBPARAMS
{
    ETYPE_BUS   m_eBus;
    CString     m_omBusName;
    CString     m_omDBPath;
    CString     m_omIdFieldName;
    UINT        m_unMaxMsgLen;
    void**      m_ppvActiveDB;
    void**      m_ppvImportedDBs;
} SDBPARAMS;

static SDBPARAMS sg_asDbParams[BUS_TOTAL] =
{
    {CAN,     "CAN",    "", "Message ID :",  8,0,0},
    {MCNET,   "MCNET",  "", "Message Code :",  MAX_DATA_LEN_MCNET,0,0},
    {J1939,   "J1939",  "", "PGN :",  MAX_DATA_LEN_J1939,0,0},
    {FLEXRAY, "FLEXRAY","", "FRAME ID :",  MAX_DATA_LEN_FLEXRAY,0,0},
    {LIN,     "LIN",    "", "Msg ID :",  0,0,0},
    {MOST,    "MOST",   "", "Msg ID :",  0,0,0}
};

enum eWAVEFORMTYPE
{
    eWave_NONE = 0,
    eWave_SINE,
    eWave_TRIANGLE,
    eWave_COS,
    eWave_SAWTOOTH
};
enum {UNDEFINED_TIMING = 0, CYCLIC_TIMING, EVENT_CONTROLLED_TIMING, REQUEST_CONTROLLED_TIMING, RELATIVELY_SCHEDULED_TIMING, ABSOLUTELY_SCHEDULED_TIMING, IDENTIFIER};

struct sINTERPRETSIGNALINFO
{
    CString m_omStrSignalName;
    __int64 m_un64RawValue;
    double m_dPhyValue;
    CString m_omStrSignalDescValue;
    CString m_omStrUnit;
    unsigned char   m_ucSigLength;

    sINTERPRETSIGNALINFO(): m_omStrSignalName(""),
        m_un64RawValue(0),
        m_dPhyValue(0),
        m_omStrSignalDescValue(""),
        m_omStrUnit(""),
        m_ucSigLength(0)
    {
        // Do Additional Init here
    }
};

typedef sINTERPRETSIGNALINFO SINTERPRETSIGNALINFO;
typedef SINTERPRETSIGNALINFO* PINTERPRETSIGNALINFO;
typedef CArray<SINTERPRETSIGNALINFO,SINTERPRETSIGNALINFO> CSignalInfoArray;


typedef union _SIG_VALUE
{
    __int64  n64Value;
    unsigned __int64  un64Value;
} SIG_VALUE;

class CSignalDescVal
{
public:
    CSignalDescVal();
    ~CSignalDescVal();
    CSignalDescVal& operator=(const CSignalDescVal& RefObj);
    void vClearNext(void);

    CString m_omStrSignalDescriptor;                // Name of the Signal ID
    SIG_VALUE m_DescValue;
    CSignalDescVal* m_pouNextSignalSignalDescVal;   // Next Signal desc and value
};


struct sWaveformInfo
{
    eWAVEFORMTYPE m_eSignalWaveType;
    float m_fAmplitude;   // Maximum amplitude
    float m_fFrequency;
    float m_fGranularity; // Of each time amplitude calculation. Value depends
    // on the sampling frequency.

    sWaveformInfo();

    sWaveformInfo& operator=(const sWaveformInfo& RefObj);
    static CString omGetWaveformName(eWAVEFORMTYPE eWaveform);
};

struct sSigWaveMap
{
    CString m_omSigName;
    sWaveformInfo sWaveInfo;

    sSigWaveMap();

    sSigWaveMap& operator=(const sSigWaveMap& RefObj);
    BOOL operator==(const sSigWaveMap& RefObj) const;
};

typedef CList<sSigWaveMap, sSigWaveMap&> CSigWaveMapList;

struct SSigGeneration
{
    UINT m_nMsgID;
    float m_fDefaultAmplitude;
    CSigWaveMapList m_omSigWaveMapList;

    SSigGeneration();
    ~SSigGeneration();

    void Reset(void);
    BOOL operator ==(const SSigGeneration& RefObj) const;
    SSigGeneration& operator=(const SSigGeneration& RefObj);
};

typedef CList<SSigGeneration, SSigGeneration&> CSigGenerationInfoList;

struct sSIGNALS
{
    CString m_omStrSignalName;      // Signal Name
    UINT    m_unStartByte;          // Signal definition start byte in Msg, Zero index
    UINT    m_unSignalLength;       // Signal Length, One indexed
    BYTE    m_byStartBit;           // Signal start bit, Zero indexed
    BYTE    m_bySignalType;         // Signal Type

    SIG_VALUE m_SignalMinValue;
    SIG_VALUE m_SignalMaxValue;

    float       m_fSignalFactor;    // Signal Multiplication Factor
    float       m_fSignalOffset;    // Signal Offset value.
    CString m_omStrSignalUnit;      // Signal Measurement Unit
    EFORMAT_DATA m_eFormat;         // Signal endianness.

    CSignalDescVal*  m_oSignalIDVal;// Signal Type ID and value List
    sSIGNALS* m_psNextSignalList;   // Next Signal

    // Call this function to set the data bits of a signal with the given value
    static void vSetSignalValue(sSIGNALS* pouCurrSignal, UCHAR aucData[8],
                                UINT64 u64SignVal);
    // Call this function to get the bit mask for a signal. In a bit mask all
    // bits petaining to but this signal is 0.
    static UINT64 un64GetBitMask(sSIGNALS* pouCurrSignal);
};


struct sMESSAGE
{
    CString m_omStrMessageName; // Message Name
    UINT m_unMessageCode;       // Message Code
    BYTE m_byMessageChannel;    // Channel on which message is transported /* derka */
    UINT m_unNumberOfSignals;   // Number Of Signals a Message has
    UINT m_unMessageLength;     // Message Length
    BOOL m_bMessageFrameFormat; // Message Frame Format
    sSIGNALS* m_psSignals;      // Signal associated with Message
    BYTE m_bySignalMatrix[1785];   // Matrix to validate overlapping signals
    int m_nMsgDataFormat;       // 1-Intel, 0-Motorola
};

typedef struct tagSSIGNALINFO
{
    CString     m_omSigName;
    CString     m_omRawValue;
    CString     m_omEnggValue;
    CString     m_omUnit;
    CString     m_msgName;
} SSignalInfo;

typedef CArray<SSignalInfo, SSignalInfo> SSignalInfoArray;


struct Flexray_SSIGNALINFO
{
    string     m_omSigName;
    string     m_omRawValue;
    string     m_omEnggValue;
    string     m_omUnit;
};

//typedef list<SSIGNALINFO> signalInfoList;

typedef struct tagSMSGENTRY
{
    sMESSAGE*  m_psMsg;
    tagSMSGENTRY*    m_psNext;

    tagSMSGENTRY();
    ~tagSMSGENTRY();

    static void vClearMsgList(tagSMSGENTRY*& psMsgRoot);
    static void vClearSignalList(sSIGNALS* psSignals);

    static sMESSAGE* psCopyMsgVal(sMESSAGE* psMsg);
    static sSIGNALS* psCopySignalVal(sSIGNALS* psSignal);

    static BOOL bUpdateMsgList(tagSMSGENTRY*& Root, sMESSAGE* psMsg);
    static sSIGNALS* psCopySignalList(sSIGNALS* psSignal);
    static BOOL bGetMsgPtrFromMsgId(const tagSMSGENTRY* psRoot,UINT unMsgId, sMESSAGE*& pMsg);


} SMSGENTRY;
#ifndef _FLEXRAY_
#define _FLEXRAY_
#define MAX_TEXT_NAME_ID        128
#define MAX_FRAME_NAME_ID       256
#define MAX_SCALE_LINEAR_RNG    10
#define MAX_OPERANDS_RNG        20
#define EMPTY_VALUE             -1
typedef enum
{
    IDENTICAL_ENUM = 0,              //No change
    LINEAR_ENUM,                     //Linear expression N0,N1,D1
    SCALE_LINEAR_ENUM,               //More than 1 Linear exp
    TEXTTABLE_ENUM,                  //Description for values
    SCALE_LINEAR_TEXTTABLE_ENUM,     //Linear with Descriptin
    TAB_NOINTP_ENUM,                 //1 to 1 mapping for some values
    FORMULA_ENUM                     //User defined formula
} COMPU_EXPRESSION_MSGSIG;

typedef struct tagSRange
{
    DWORD    m_dwLowerLimit;
    DWORD    m_dwUpperLimit;

    BOOL bIsWithinRange(DWORD dwValue);

    tagSRange();

} SRange;

typedef struct tagIdenticalCodeVarEx
{
    SRange   m_sRange;
} IDENTICAL_CODE_VAR_EX, *PIDENTICAL_CODE_VAR_EX;

typedef struct tagLinearCodeVarEx
{
    SRange   m_sRange;
    double   m_dN0;   // AppValue =
    double   m_dN1;   // (m_dN0 // offset + m_dN1 //factor * Trans. Val) / m_dD0
    double   m_dD0;

    tagLinearCodeVarEx()
    {
    }
    BOOL operator==(const tagLinearCodeVarEx& RefObj) const
    {
        return false;
    }
    tagLinearCodeVarEx& operator=(const tagLinearCodeVarEx& RefObj)
    {
        m_sRange.m_dwLowerLimit =  RefObj.m_sRange.m_dwLowerLimit;
        m_sRange.m_dwUpperLimit = RefObj.m_sRange.m_dwUpperLimit;
        m_dN0 = RefObj.m_dN0;
        m_dN1 = RefObj.m_dN1;
        m_dD0 = RefObj.m_dD0;

        return *this;
    }
    ~tagLinearCodeVarEx()
    {
    }
} LINEAR_CODE_EX;

typedef std::list<LINEAR_CODE_EX> OBJ_SCALELINEAR;

//For scale linear
typedef struct tagScaleLinearCodeVarEx
{
    // LINEAR_CODE_VAR_EX m_sLinearType;
    //tagScaleLinearCodeVarEx* m_pNextLinearType;

    tagScaleLinearCodeVarEx();
    tagScaleLinearCodeVarEx& operator=(const tagScaleLinearCodeVarEx& RefObj);
    ~tagScaleLinearCodeVarEx();

} SCALE_LINEAR_CODE_VAR_EX, *PSCALE_LINEAR_CODE_VAR_EX;

//For textable
typedef struct tagTextCodeVarEx
{
    SRange   m_sRange;
    //CHAR     m_aTextName[MAX_TEXT_NAME_ID];
    string  m_aTextName;
    //tagTextCodeVarEx* m_pNextTextCodeType;

    tagTextCodeVarEx();
    tagTextCodeVarEx& operator=(const tagTextCodeVarEx& RefObj);
    ~tagTextCodeVarEx();

} TEXT_CODE_VAR_EX, *PTEXT_CODE_VAR_EX;

typedef std::list<TEXT_CODE_VAR_EX> OBJ_TEXTCODEVAR_EX;

//For TAB-NOINTP
typedef struct tagTabCodeVarEx
{
    SRange   m_sRange;
    double   m_dPhysVal;
    //tagTabCodeVarEx* m_pNextTabCodeType;

    tagTabCodeVarEx();
    tagTabCodeVarEx& operator=(const tagTabCodeVarEx& RefObj);
    ~tagTabCodeVarEx();

} TAB_CODE_VAR_EX, *PTAB_CODE_VAR_EX;

typedef std::list<TAB_CODE_VAR_EX> OBJ_TABCODVAR;

typedef struct tagOperands
{
    char        m_cOpredandName;
    float       m_fOperandValue;
    tagOperands* m_pNextOpearnd;

    tagOperands();
    tagOperands& operator=(const tagOperands& RefObj);
    ~tagOperands();

} OPERANDS, *POPERANDS;

typedef struct tagFormulaCodeVarEx
{
    SRange      m_sRange;
    CString     m_omFormula;
    POPERANDS   m_pFirstOperand;

    tagFormulaCodeVarEx();
    tagFormulaCodeVarEx& operator=(const tagFormulaCodeVarEx& RefObj);
    ~tagFormulaCodeVarEx();

    CString omGetEnggValue(DWORD dwRawValue); // CHOU -- PREFER TO BE REMOVED
    BOOL bGetEnggValue(DWORD dwRawValue, CString& omEnggVal) const;

} FORMULA_CODE_VAR_EX, *PFORMULA_CODE_VAR_EX;



//Only one of the above struct will be assigned to each element

//Only one of the above struct will be assigned to each element
typedef struct tagCompuMethodEx
{
    //PTAB_CODE_VAR_EX            m_pTabCode;
    OBJ_TABCODVAR               m_objTabCode;
    //PTEXT_CODE_VAR_EX           m_pTextCode;
    OBJ_TEXTCODEVAR_EX          m_objTextCode;
    tagLinearCodeVarEx          m_LinearCode;
    IDENTICAL_CODE_VAR_EX       m_IdenticalCode;
    //PSCALE_LINEAR_CODE_VAR_EX   m_pScaleLinear;
    OBJ_SCALELINEAR             m_objScaleLinear;
    FORMULA_CODE_VAR_EX         m_Formula;

    tagCompuMethodEx()
    {
        // m_pScaleLinear = NULL;
        //m_pTextCode    = NULL;
        //m_pTabCode     = NULL;
    }
    ~tagCompuMethodEx();
    tagCompuMethodEx& operator =(const tagCompuMethodEx& RefObj);
    tagCompuMethodEx(tagCompuMethodEx& RefObj);

} COMPU_METHOD_EX, *PCOMPU_METHOD_EX;

class CCompuMethodEx
{
public:
    //SRange            m_sNotValidRange;
    //SRange            m_sNotAvailableRange;
    COMPU_EXPRESSION_MSGSIG  m_eCompuType;
    COMPU_METHOD_EX   m_uMethod;

    // To get the engineering value
    //CString omGetEnggValue(DWORD dwRawValue);
    bool omGetEnggValue(DWORD dwRawValue, CString& omEnggValue); // TO BE REMOVED CHOU
    bool omGetRawValue(CString& omRawValue, double omEnggValue);
    BOOL bGetEnggValue(DWORD dwRawValue, CString& omEnggValue) const;
    CCompuMethodEx& operator=(const CCompuMethodEx& RefObj);
};

// CHOU TRY CArray<CCompuMethodEx*, CCompuMethodEx*> CSigCompuMethodsArray
typedef CArray<CCompuMethodEx, CCompuMethodEx> CSigCompuMethodsArray;

typedef enum
{
    INVALID = 0, NOT_AVAILABLE, VALID
} RANGE_VALID;

typedef struct tagSIG_INT_CONSTRAINTS_EX
{
    RANGE_VALID m_eValid;
    SRange      m_sRange;

    tagSIG_INT_CONSTRAINTS_EX();
    ~tagSIG_INT_CONSTRAINTS_EX();

    tagSIG_INT_CONSTRAINTS_EX(tagSIG_INT_CONSTRAINTS_EX& Refobj);
    tagSIG_INT_CONSTRAINTS_EX& operator=(const tagSIG_INT_CONSTRAINTS_EX& Refobj);
} SIG_INT_CONSTRAINT_EX, *PSIG_INT_CONSTRAINT_EX;

// CHOU TRY CArray<SIG_INT_CONSTRAINT_EX*, SIG_INT_CONSTRAINT_EX*> CSigConstraintsArray
typedef CArray<SIG_INT_CONSTRAINT_EX, SIG_INT_CONSTRAINT_EX> CSigConstraintsArray;

//typedef struct tagSSIGNALINFO
//{
//    CString     m_omSigName;
//    CString     m_omRawValue;
//    CString     m_omEnggValue;
//    CString     m_omUnit;
//
//} SSignalInfo;

// CHOU TRY CArray<SSignalInfo*, SSignalInfo*> SSignalInfoArray
//typedef CArray<SSignalInfo, SSignalInfo> SSignalInfoArray;

class CSignalDef
{
public:
    UINT     m_unStartbit;  // The prime identifier of a signal
    UINT     m_unLength;
    CString  m_omNameSignal;
    BOOL     m_bIsBigEndian;
    CString  m_omUnit;
    CString  m_omPhylVal;

    CSigConstraintsArray      m_ouSigConstrnt;
    CSigCompuMethodsArray     m_ouCompuMethods; // Engg. value computing methods

    // Given the data byte array, this function returns the raw value
    UINT64 un64GetRawValue(WORD* pwData);

    // Search engg value from compu method blocks
    CString omSearchValueFromCompuBlks(DWORD dwRawValue);

    // Given the raw value, this returns the engineering value
    CString omGetEnggValue(DWORD dwRawValue); // CHOU -- REMOVE
    BOOL bGetEnggValue(DWORD dwRawValue, CString& omEnggValue) const;
    CSignalDef();
    CSignalDef(CSignalDef& RefObj);
    virtual ~CSignalDef();
    CSignalDef& operator=(const CSignalDef& RefObj);
};

// CHOU -- TRY typedef CArray<CSignalDef*, CSignalDef*> CSignalDefArray;
typedef CArray<CSignalDef, CSignalDef> CSignalDefArray;

typedef struct tagSFrameRef
{
    int      m_omSlotId;
    CString  m_omFrameRef;

} SFrameRef;

// CHOU -- TRY CArray<SFrameRef*, SFrameRef*> CFrameRefArray
typedef CArray<SFrameRef, SFrameRef> CFrameRefArray;

class CFrameDef
{
public:
    UINT              m_unFrameID;   // The prime identifier of a frame
    CString           m_omNameFrame;
    int               m_nFrameLength;
    CSignalDefArray   m_ouSignalArray;


    CFrameDef();
    CFrameDef(CFrameDef& RefObj);
    virtual ~CFrameDef();
    CFrameDef& operator =(const CFrameDef& RefObj);

    // Given the data byte array, this function returns the signal info array
    BOOL bGetSignalInfoSet(WORD* pwData, SSignalInfoArray& sSignalInfoSet);
};

typedef CMap<UINT, UINT, CFrameDef, CFrameDef&> CFrameMap;


// A frame entry = frame id, frame name and signal list
// A signal entry = start bit, length, signal name, endianness, unit, constraint list, computation method list
// A computation method = compu. type, compu method, range


/* UTILITY FUNCTION */
// Given the frame map object, frame id and data byte array, this returns the
// signal info array along with the frame name
BOOL bGetSignalInfoSet(CFrameMap& ouDataSet, UINT unFrameID, WORD* pwData,
                       CString& omFrameName, SSignalInfoArray& sSignalInfoSet);

// Given the frame map object and the frame id, this returns the frame name
BOOL bGetFrameName(CFrameMap& ouDataSet, UINT unFrameID, CString& omFrameName);

// Function to load a fibex file.

// Function to copy a CFrameMap object into another
void vCopyFrameData(CFrameMap& ouFrameTarget, const CFrameMap& ouFrameSource);

// Function to read the contents in a processed form
int nGetFrameDataSet(CFrameMap& ouDataSet);

// Clear the repository
void vCleanFrameDataSet(CFrameMap& ouDataSet);

struct SIGNAL_STRUCT;
struct FRAME_STRUCT;


CString omSearchValueFromCompuBlks(SIGNAL_STRUCT& ouSignal, DWORD dwRawValue);
string omGetEnggValue( SIGNAL_STRUCT& ouSignal, DWORD dwRawValue);
UINT64 un64GetRawValue( SIGNAL_STRUCT& ouStruct, CByteArray& pwData);
bool bGetSignalInfo(FRAME_STRUCT& ouFrame, unsigned char uchBytes[], int nByteSize, list<Flexray_SSIGNALINFO>& ouSignalInfoList, BOOL bIsHex = FALSE);
void GetSignalNames(list<Flexray_SSIGNALINFO> lstSignalInfo, CStringList& lstSignalNames);


#endif

#endif //MSGSIGNAL_H__INCLUDED_