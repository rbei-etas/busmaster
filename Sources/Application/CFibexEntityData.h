///////////////////////////////////////////////////////////
//  CFibexEntityData.h
//  Implementation of the Class CFibexEntityData
//  Created on:      11-Sep-2008 6:20:48 PM
///////////////////////////////////////////////////////////

#if !defined CFIBEXENTITYDATA_H__INCLUDED_
#define CFIBEXENTITYDATA_H__INCLUDED_

#include <afxtempl.h>
#import <msxml6.dll>
using namespace MSXML2;


class CFibexEntityData;

enum {LOAD, SAVE, VALIDATE};

#define INVALID_CHOICE      0x0
#define INVALID_STRING      _T("<Empty>")
#define UNDEFINED_CHOICE    0x0
#define INVALID_VALUE       -1

class CBaseFibexData
{
public:
    CBaseFibexData();
    virtual ~CBaseFibexData();

    virtual HRESULT Load(MSXML2::IXMLDOMNodePtr& pIDomNode) = 0;
    //virtual HRESULT Save(MSXML2::IXMLDOMNodePtr& pIDomNode) const = 0;
    virtual bool Validate(CBaseFibexData* pouRefObj) const;

    virtual HRESULT Process_Load(MSXML2::IXMLDOMNodePtr& pCurrChild) = 0;
    virtual void DoCleanup(void) = 0;
    virtual HRESULT DoIterate_ForLoad(MSXML2::IXMLDOMNodePtr& pParent);
};

typedef CList<std::string, std::string> CStringTokenList;

enum ETYPE_ELEMENT_ENTITY
{
    UNDEFINED = 0,
    CLUSTER,
    CHANNEL,
    GATEWAY,
    ECU,
    PDU,
    FRAME,
    FUNCTION,
    SIGNAL,
    COMPOSITE
};

union UIDENTIFIER
{
    long     m_lIDENTIFIER_VALUE;
    char     m_acMFR_ID_EXTN[256];
    UIDENTIFIER();
};

struct STIME_RANGE : public CBaseFibexData
{
    CString m_omValue;
    CString m_omMinValue;
    CString m_omMaxValue;

    STIME_RANGE();
    void DoCleanup(void);
    HRESULT Load(MSXML2::IXMLDOMNodePtr& pCurrDOMNode);
    HRESULT Save(MSXML2::IXMLDOMElementPtr pParentNode);
    HRESULT Process_Load(MSXML2::IXMLDOMNodePtr& pCurrDOMNode);
    STIME_RANGE& operator=(STIME_RANGE& RefObj);
};

struct SDECRIPTION
{
    CString        m_omDescription;
    CString        m_omDescriptType;
    CString        m_omDescriptionLang;
    SDECRIPTION();
    HRESULT Save(MSXML2::IXMLDOMElementPtr& pParentNode);
};
struct SNAMED_ELEMENT_TYPE : public CBaseFibexData
{
    SDECRIPTION m_omDESC;
    CString     m_omLongName;
    CString     m_omLNameLang;
    CString     m_omShortName;
    CString     m_omProductRef;

    SNAMED_ELEMENT_TYPE();
    void DoCleanup(void);
    HRESULT Load(MSXML2::IXMLDOMNodePtr& pIDomNode);
    HRESULT Process_Load(MSXML2::IXMLDOMNodePtr& pCurrChild);
    HRESULT Save(MSXML2::IXMLDOMElementPtr pParentNode);
};

struct SCOMPANY_REV_INFO : public CBaseFibexData
{
    CString m_omRev_Level;
    CString m_omState;
    CString m_omCompany_Data_Ref;

    HRESULT Load(MSXML2::IXMLDOMNodePtr& pIDomNode);
    HRESULT Process_Load(MSXML2::IXMLDOMNodePtr& pCurrChild);
    SCOMPANY_REV_INFO();
    void DoCleanup(void);
    HRESULT Save(MSXML2::IXMLDOMElementPtr& pParentNode);
};
typedef CList<SCOMPANY_REV_INFO, SCOMPANY_REV_INFO&> CCompanyRevInfoList;

struct SMODIFICATION : public CBaseFibexData
{
    CString m_omChange;
    CString m_omReason;

    HRESULT Load(MSXML2::IXMLDOMNodePtr& pIDomNode);
    HRESULT Process_Load(MSXML2::IXMLDOMNodePtr& pCurrChild);
    SMODIFICATION();
    void DoCleanup(void);
    HRESULT Save(MSXML2::IXMLDOMElementPtr& pParentNode);
};
typedef CList<SMODIFICATION, SMODIFICATION&> CModificationList;

struct SELEMENT_REVISION : public CBaseFibexData
{
    CCompanyRevInfoList  m_odCompanyRevInfoList;
    CModificationList    m_odModificationList;
    CString              m_omDate;
    CString              m_omREVISION_LABEL;
    CString              m_omSTATE;
    CString              m_omTEAM_MEMBER_REF;

    SELEMENT_REVISION();
    ~SELEMENT_REVISION();
    void DoCleanup(void);
    SELEMENT_REVISION& operator=(SELEMENT_REVISION& RefObj);
    HRESULT Load(MSXML2::IXMLDOMNodePtr& pIDomNode);
    HRESULT Save(MSXML2::IXMLDOMElementPtr& pParentNode);
    HRESULT Process_Load(MSXML2::IXMLDOMNodePtr& pCurrChild);

};
typedef CList<SELEMENT_REVISION, SELEMENT_REVISION&> CElementRevList;

struct SDOC_REVISION : public CBaseFibexData
{
    CString              m_omTEAM_MEMBER_REF;
    CString              m_omRevisionLabel;
    CString              m_omState;
    CString              m_omDate;
    CCompanyRevInfoList  m_odCompanyRevInfoList;
    CModificationList    m_odModificationList;

    SDOC_REVISION();
    ~SDOC_REVISION();
    void DoCleanup(void);
    SDOC_REVISION& operator=(SDOC_REVISION& RefObj);

    HRESULT Load(MSXML2::IXMLDOMNodePtr& pCurrDOMNode);
    HRESULT Process_Load(MSXML2::IXMLDOMNodePtr& pCurrDOMNode);
    HRESULT Save(MSXML2::IXMLDOMElementPtr& pParentNode);

};
typedef CList<SDOC_REVISION, SDOC_REVISION&> CDocRevList;


struct SADMIN_DATA : public CBaseFibexData
{
    CDocRevList m_odDocRevList;

    SADMIN_DATA& operator=(SADMIN_DATA& RefObj);
    ~SADMIN_DATA();
    void DoCleanup(void);

    HRESULT Load(MSXML2::IXMLDOMNodePtr& pCurrDOMNode);
    HRESULT Process_Load(MSXML2::IXMLDOMNodePtr& pCurrDOMNode);
    HRESULT Save(MSXML2::IXMLDOMElementPtr& pParentNode);

};

class CFibexEntityData
{
public:
    CString              m_omID;
    CString              m_omType;
    SNAMED_ELEMENT_TYPE  m_sNamedElemType;
    CElementRevList      m_odElementRevList;

    CString GetID();
    void vSetID(CString omID);
    CFibexEntityData();
    virtual ~CFibexEntityData();

    CFibexEntityData& operator=(CFibexEntityData& RefObj);
};

#endif // !defined CFIBEXENTITYDATA_H__INCLUDED_
