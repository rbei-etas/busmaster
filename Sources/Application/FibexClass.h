// FibexClass.h : main header file for the FibexClass DLL
//

#pragma once

#ifndef __AFXWIN_H__
#error include 'FibexClass_stdafx.h' before including this file for PCH
#endif

#include "stdafx.h"         // Standard header include file
#include "FIBEX_Structs.h"
#include "DataTypes/Cluster.h"
#include <afxtempl.h>
#include <map>

typedef enum
{
    IDENTICAL_E = 0,              //No change
    LINEAR_E,                     //Linear expression N0,N1,D1
    SCALE_LINEAR_E,               //More than 1 Linear exp
    TEXTTABLE_E,                  //Description for values
    SCALE_LINEAR_TEXTTABLE_E,     //Linear with Descriptin
    TAB_NOINTP_E,                 //1 to 1 mapping for some values
    FORMULA_E                     //User defined formula
} COMPU_EXPRESSION_ENUM;

struct SLIMIT_v2
{
public:
    /* One of - OPEN, CLOSED[default], INFINITE */
    std::string         m_ouIntervalType;
    float           m_fValue;

    SLIMIT_v2();
    ~SLIMIT_v2();
    void DoCleanup(void);
};

struct SCONSTRS_v2
{
public:
    /* One of -
     * VALID[default], NOT-VALID, NOT-AVAILABLE, NOT-DEFINED, ERROR, OTHER */

    std::string         m_ouValidity;
    SLIMIT_v2          m_sLowerLimit;
    SLIMIT_v2          m_sUpperLimit;

    SCONSTRS_v2();
    ~SCONSTRS_v2();
    void DoCleanup(void);
    HRESULT Load(MSXML2::IXMLDOMNodePtr& pIDomNode);
    HRESULT Process_Load(MSXML2::IXMLDOMNodePtr& pCurrChild);
    HRESULT DoIterate_ForLoad(MSXML2::IXMLDOMNodePtr& pCurrChild);
    SCONSTRS_v2& operator=(SCONSTRS_v2& objConstr);
    SCONSTRS_v2(SCONSTRS_v2& RefObj);
    //BOOL bTranslate(eENTITY_FIBEX eEntity, BYTE* Buffer, BOOL bToAbstract);
};
typedef CList<SCONSTRS_v2, SCONSTRS_v2&> CScaleContrsList_v2;

class SCOMPU_CONST_v2
{
public:
    bool    m_bIsDouble;
    float   m_fV;
    std::string m_omVT;

    SCOMPU_CONST_v2();
    ~SCOMPU_CONST_v2();

    void DoCleanup(void);
    HRESULT Load(MSXML2::IXMLDOMNodePtr& pIDomNode);
    HRESULT Process_Load(MSXML2::IXMLDOMNodePtr& pCurrChild);
    HRESULT DoIterate_ForLoad(MSXML2::IXMLDOMNodePtr& pCurrChild);
    SCOMPU_CONST_v2& operator=(SCOMPU_CONST_v2& Refobj)
    {
        m_bIsDouble = Refobj.m_bIsDouble;
        m_fV = Refobj.m_fV;
        m_omVT = Refobj.m_omVT;
        return *this;
    }
    SCOMPU_CONST_v2(SCOMPU_CONST_v2& objRef)
    {
        m_bIsDouble = objRef.m_bIsDouble;
        m_fV = objRef.m_fV;
        m_omVT = objRef.m_omVT;
    }
};
typedef CList <float, float&> CDoubleArray;
struct SCOMPU_RATIONAL_COEFFS_v2
{
public:
    CDoubleArray m_faCompuNuma;
    CDoubleArray m_faCompuDeno;

    SCOMPU_RATIONAL_COEFFS_v2();
    void DoCleanup(void);
    HRESULT Load(MSXML2::IXMLDOMNodePtr& pIDomNode);
    HRESULT Process_Load(MSXML2::IXMLDOMNodePtr& pCurrChild);
    HRESULT DoIterate_ForLoad(MSXML2::IXMLDOMNodePtr& pCurrChild);
};

struct SDECRIPTION_v2
{
    std::string m_omDescription;
    std::string m_omDescriptType;
    std::string m_omDescriptionLang;
    SDECRIPTION_v2();
    //HRESULT Save(MSXML2::IXMLDOMElementPtr& pParentNode);
};


struct SNAMED_ELEMENT_TYPE_v2
{
    SDECRIPTION_v2 m_omDESC;
    std::string m_omLongName;
    std::string m_omLNameLang;
    std::string m_omShortName;
    std::string m_omProductRef;

    SNAMED_ELEMENT_TYPE_v2();
    void DoCleanup(void);
    HRESULT Load(MSXML2::IXMLDOMNodePtr& pIDomNode);
    HRESULT Process_Load(MSXML2::IXMLDOMNodePtr& pCurrChild);
    HRESULT DoIterate_ForLoad(MSXML2::IXMLDOMNodePtr& pCurrChild);

    //HRESULT Save(MSXML2::IXMLDOMElementPtr pParentNode);
};

typedef struct tagSUnit
{
    std::string     m_omID;
    std::string     m_omUnitName;
    std::string     m_omDisplayName;
    float       m_fFACTOR_SI_TO_UNIT;
    float       m_fOFFSET_SI_TO_UNIT;
    std::string     m_omPHYSICAL_DIMENSION_REF;

} UNIT_EX;

class SCOMPU_SCALE_v2
{
public:
    SLIMIT_v2                  m_sLower;
    SLIMIT_v2                  m_sUpper;
    SCOMPU_CONST_v2            m_sCompuConst;
    SCOMPU_RATIONAL_COEFFS_v2  m_sCompuRationalCoeffs;
    std::string                    m_omCompuGenMath;

    SCOMPU_SCALE_v2();
    ~SCOMPU_SCALE_v2();

    void DoCleanup(void);
    HRESULT Load(MSXML2::IXMLDOMNodePtr& pIDomNode);
    HRESULT Process_Load(MSXML2::IXMLDOMNodePtr& pCurrChild);
    HRESULT DoIterate_ForLoad(MSXML2::IXMLDOMNodePtr& pCurrChild);
    // BOOL bTranslate(eENTITY_FIBEX eEntity, BYTE* Buffer, BOOL bToAbstract);
    // HRESULT Save(MSXML2::IXMLDOMElementPtr& pIDomNode);
    SCOMPU_SCALE_v2& operator=(SCOMPU_SCALE_v2& RefObj)
    {
        m_sLower = RefObj.m_sLower;
        m_sUpper = RefObj.m_sUpper;
        m_sCompuConst = RefObj.m_sCompuConst;
        m_sCompuRationalCoeffs.m_faCompuDeno.RemoveAll();
        m_sCompuRationalCoeffs.m_faCompuDeno.AddTail(&(RefObj.m_sCompuRationalCoeffs.m_faCompuDeno));
        m_sCompuRationalCoeffs.m_faCompuNuma.RemoveAll();
        m_sCompuRationalCoeffs.m_faCompuNuma.AddHead(&(RefObj.m_sCompuRationalCoeffs.m_faCompuNuma));
        m_omCompuGenMath = RefObj.m_omCompuGenMath;
        return *this;
    }

    SCOMPU_SCALE_v2(SCOMPU_SCALE_v2& RefObj)
    {
        m_sLower = RefObj.m_sLower;
        m_sUpper = RefObj.m_sUpper;
        m_sCompuConst = RefObj.m_sCompuConst;
        m_sCompuRationalCoeffs.m_faCompuDeno.RemoveAll();
        m_sCompuRationalCoeffs.m_faCompuDeno.AddTail(&(RefObj.m_sCompuRationalCoeffs.m_faCompuDeno));
        m_sCompuRationalCoeffs.m_faCompuNuma.RemoveAll();
        m_sCompuRationalCoeffs.m_faCompuNuma.AddHead(&(RefObj.m_sCompuRationalCoeffs.m_faCompuNuma));
        m_omCompuGenMath = RefObj.m_omCompuGenMath;
    }
};
typedef CList<SCOMPU_SCALE_v2, SCOMPU_SCALE_v2&> CCompuScaleList_v2;

struct SCOMPU_INTERNAL_TO_PHYS_v2
{
public:
    CCompuScaleList_v2 m_odCompuScaleList;
    SCOMPU_CONST_v2    m_sCompuDefValue;

    SCOMPU_INTERNAL_TO_PHYS_v2();
    ~SCOMPU_INTERNAL_TO_PHYS_v2();
    void DoCleanup(void);
    SCOMPU_INTERNAL_TO_PHYS_v2& operator=(SCOMPU_INTERNAL_TO_PHYS_v2& RefObj);
    SCOMPU_INTERNAL_TO_PHYS_v2(SCOMPU_INTERNAL_TO_PHYS_v2& RefObj);
    HRESULT Load(MSXML2::IXMLDOMNodePtr& pIDomNode);
    HRESULT Process_Load(MSXML2::IXMLDOMNodePtr& pCurrChild);
    HRESULT DoIterate_ForLoad(MSXML2::IXMLDOMNodePtr& pCurrChild);


    //BOOL bTranslate(eENTITY_FIBEX eEntity, BYTE* Buffer, BOOL bToAbstract);
    // HRESULT Save(MSXML2::IXMLDOMElementPtr& pIDomNode);
};

struct SCOMPU_METHOD_v2
{
public:
    SNAMED_ELEMENT_TYPE_v2     m_sNamedElemType;
    /* One from -
     * IDENTICAL, LINEAR, SCALE-LINEAR, TEXTTABLE, TAB-NOINTP, FORMULA */
    std::string                 m_ouCategory;
    COMPU_EXPRESSION_MSGSIG        m_eCategory;
    std::string                 m_omUnitRef;
    //CScaleContrsList_v2        m_odPhysConstrs;
    //CScaleContrsList_v2        m_odInternalConstrs;
    SCOMPU_INTERNAL_TO_PHYS_v2 m_sCompu_Internal_To_Phys;

    SCOMPU_METHOD_v2();
    ~SCOMPU_METHOD_v2();
    void DoCleanup(void);
    HRESULT Load(MSXML2::IXMLDOMNodePtr& pIDomNode);
    HRESULT Process_Load(MSXML2::IXMLDOMNodePtr& pCurrChild);
    HRESULT DoIterate_ForLoad(MSXML2::IXMLDOMNodePtr& pCurrChild);
    //HRESULT Save(MSXML2::IXMLDOMElementPtr& pIDomNode);
    //BOOL bTranslate(eENTITY_FIBEX eEntity, BYTE* Buffer, BOOL bToAbstract);
    SCOMPU_METHOD_v2& operator=(SCOMPU_METHOD_v2& RefObj);
    SCOMPU_METHOD_v2(SCOMPU_METHOD_v2&  Refobj);
};

typedef CList<SCOMPU_METHOD_v2, SCOMPU_METHOD_v2&> CCompuMethodList_v2;


struct CCODING_v2
{
public:

    std::string              m_omID;
    SNAMED_ELEMENT_TYPE_v2  m_sNamedElemType;
    SPHYSICAL_TYPE_v2   m_sPhysicalType;
    SCODED_TYPE_v2      m_sCodedType;
    std::string          m_omMFR_CODING_EXTN;
    CCompuMethodList_v2 m_odCompuMethodList;
    CElementRevList_v2      m_odElementRevList;

    CCODING_v2();
    ~CCODING_v2();
    CCODING_v2(CCODING_v2& RefObj);

    void DoCleanup(void);
    CCODING_v2& operator=(CCODING_v2& RefObj);
    HRESULT Load(MSXML2::IXMLDOMNodePtr& pIDomNode);
    HRESULT Process_Load(MSXML2::IXMLDOMNodePtr& pCurrChild);
    HRESULT DoIterate_ForLoad(MSXML2::IXMLDOMNodePtr& pCurrChild);
    //HRESULT Save(MSXML2::IXMLDOMElementPtr& pParentDomNode);
    //BOOL bTranslate(eENTITY_FIBEX eEntity, BYTE* Buffer, BOOL bToAbstract);
};

char* ConvertBSTR(BSTR pSrc);

/* Implementation class for FIBEX file */
class CPARSER_FIBEX
{
public:
    /* Constructor */
    CPARSER_FIBEX() {};

    /* Destructor */
    ~CPARSER_FIBEX() {};


    //CCompuScaleList_v2 m_sample;

    // FrameId + PduName to Signal details
    std::map<std::string, std::list<SIGNAL_STRUCT>> m_mapFrameIdToSignalDetails;
    // Coding Ref to coding object
    std::map<std::string, CCODING_v2&> m_mapCoding;

    std::map<std::string, UNIT_EX> m_mapUnitRefToUnit;

    CList<CCODING_v2, CCODING_v2&> lstCoding;

    // Position of particular coding in lstCoding
    std::map<std::string, POSITION> mapCodingPosition;

    std::map<std::string, CSignal> m_mapSignalDetails;

    // Frame Id to Frame details
    std::map<std::string, CFrame> m_mapFrameDetails;

    // Pdu Id to Pdu details
    std::map<std::string, CPdu> m_mapPduDetails;

    std::map<std::string, CECU> m_mapECUDetails;

    std::map<std::string, std::map<std::string, std::string>> m_mapChnlToECUDetails;

    std::map<std::string, CClusterv2> m_mapClusterDetails;

    // FrameTrig Id to Frame details
    std::map<std::string, CFrameTrig> m_mapFramTrigIdToFrame;

    std::map<std::string, std::string> m_mapChannelRefToChnlName;

    std::map<std::string, std::map<std::string, std::string>> m_ECUIdToChnlRefs;

    std::map<std::string, std::string> m_SubFrameToFrame;

    // SubFrame Id to Signal Instances
    std::map<std::string, std::map<std::string, CSigInstance>>  m_mapSubFrameToSigInst;

    std::string m_strFIBEXVersion;

    ECHANNEL GetChnlEnumFromChnlName(std::string strChannelName);

    void Initialize();

    void GetSignalStruct(std::map<std::string, CSigInstance> mapSigInstance, std::list<SIGNAL_STRUCT>& objSignal);

    /* implementation for baseclass APIs */
    // To load the Fibex file
    int LoadFibexFile(std::string omFibexFilePath,
                      PABS_FIBEX_CONTAINER pAbsFibexContainer);

    int LoadFibexFile_Generic(std::string omFibexFilePath, std::map<std::string, Cluster>& lstCluster);

    HRESULT LoadFibexFile_v2_0(std::string omFibexFilePath);

    void CopyCodingDetails(SIGNAL_STRUCT& objSignal, SCOMPU_METHOD_v2 objCompuMthd);

    void sCopyCompuMethod(CCompuMethodEx& ouCompuMethodEx, SCOMPU_INTERNAL_TO_PHYS_v2& ouCompuInToPhys);

    COMPU_EXPRESSION_MSGSIG sCopyCompuType(std::string ouCompuType);
    RANGE_VALID eGetRangeValid(std::string omValidity);

    void FillFibexStructurev2(std::map<std::string, Cluster>& lstCluster);

    void CopyClusterDetails(Cluster& objCluster);

    void CopyPduDetails(CPdu objPdu, PDU_STRUCT& objActPdu);

    void CopyECUDetails(CECU objECU, Cluster& objCluster);

    HRESULT DoIterate_ForLoad(MSXML2::IXMLDOMNodePtr& pParent, CPdu& objPdu);

    HRESULT DoIterate_ForLoad(MSXML2::IXMLDOMNodePtr& pParent, CSignal& objSignal);

    HRESULT DoIterate_ForLoad(MSXML2::IXMLDOMNodePtr& pParent, CFrame& objFrame);

    HRESULT DoIterate_ForLoad(MSXML2::IXMLDOMNodePtr& pParent, CECU& objECU);

    HRESULT DoIterate_ForLoad(MSXML2::IXMLDOMNodePtr& pParent, CClusterv2& objCluster);

    HRESULT DoIterate_ForLoad(MSXML2::IXMLDOMNodePtr& pParent, CFrameTrig& objFrameTrig, std::string strChannelRef);

    HRESULT Process_Load(MSXML2::IXMLDOMNodePtr& pCurrDOMNode, CPdu& objPdu);

    HRESULT Process_Load(MSXML2::IXMLDOMNodePtr& pCurrDOMNode, CSignal& objSignal);

    HRESULT Process_Load(MSXML2::IXMLDOMNodePtr& pCurrDOMNode, CFrame& objFrame);

    HRESULT Process_Load(MSXML2::IXMLDOMNodePtr& pCurrDOMNode, CECU& objECU);

    HRESULT Process_Load(MSXML2::IXMLDOMNodePtr& pCurrDOMNode, CClusterv2& objCluster);

    HRESULT Process_Load(MSXML2::IXMLDOMNodePtr& pCurrDOMNode, CFrameTrig& objFrameTrig, std::string strChannelRef);

    HRESULT Process_Load(MSXML2::IXMLDOMNodePtr pParent);

    HRESULT DoIterate_ForLoad(MSXML2::IXMLDOMNodePtr& pCurrDOMNode, UNIT_EX& objUnit);

    HRESULT Process_Load(MSXML2::IXMLDOMNodePtr pCurrDOMNode, UNIT_EX& objUnit);

    void LoadElementsv2(MSXML2::IXMLDOMNodePtr);

    void LoadCodings(MSXML2::IXMLDOMNodePtr pIDomNode);

    void CollectUnits(MSXML2::IXMLDOMNodePtr pParent);

    void LoadCodingsUnit(MSXML2::IXMLDOMNodePtr pIDomNode);


    void CollectSignalInfov2(MSXML2::IXMLDOMNodePtr);

    void CollectFrameInfov2(MSXML2::IXMLDOMNodePtr);

    void CollectFrameTrigInfov2(MSXML2::IXMLDOMNodePtr pIDomNode);

    void CollectCodings(MSXML2::IXMLDOMNodePtr pIDomNode);

    void CollectECUInfov2(MSXML2::IXMLDOMNodePtr);

    void CollectClusterinfov2(MSXML2::IXMLDOMNodePtr);

    // To create new Fibex Entity
    int ResetFibexEntity(eENTITY_FIBEX eEntity, PVOID pFibexEntity);


    // To delete the entity
    int Remove(eENTITY_FIBEX eEntity, std::string omID);


    // Save contents into a Fibex file.
    BOOL WriteFibexFile(std::string omFibexFilePath, PABS_FIBEX_CONTAINER pAbsFibexContainer);


    // To convert Fibex specific data into the abstraction layer data and vice
    // versa. 'bToAbstract' denotes the direction; TRUE means from Fibex specific
    // data into the abstract data. Subject to the value of 'bToAbstract', Buffer
    // will be interpreted.
    int bTranslate(eENTITY_FIBEX eEntity, std::string om_ID, BYTE* Buffer, BOOL bToAbstract);

    // Call this API to get the cluster information like, number of Flexray clusters, controllers are associated.
    HRESULT hGetFLEXRAYClustersInfo(PABS_FIBEX_CONTAINER pAbsFibexContainer, AbsClusterInfoList& omAvailableClustersInfo);

    /* Helper APIs */
    // To retrieve version information of the Fibex file in consideration
    // This will render the version number, for eg 1.0.0, 1.1.5a , 1.2.0a, 2.0.0d, 3.0.0
    std::string GetVersionInfo(const std::string& omFibexFilePath);

    std::string GetActiveVersion(void);

    int SetActiveVersion(std::string& omVersion);

    int UnloadLibrary(void);
};