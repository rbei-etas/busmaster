// UserSelectionDlg.cpp : implementation file
//
#pragma once
#include "CustomProtocol_stdafx.h"
#include "CustomProtocol.h"
#include "ProtocolAdderDlg.h"
#include "CreateFrameDlg.h"
#include "UserSelectionDlg.h"


#define PROTOCOL_CONFIGURATION  "Protocol_Configuration"
#define LAST_UPDATED			"Last_Updated"
#define BASE_PROTOCOLS			"Base_Protocols"  
#define BASE_PROTOCOL			"Base_Protocol" 
#define NAME					"Name" 
#define PROTOCOL				"Protocol"
#define HEADER					"Header"
#define HEADER_NAME				"Header_Name"
#define NO_OF_BYTES				"NoOfBytes"
#define VALUE					"Value"
#define DERIVED_PROTOCOL		"Derived_Protocol"


// CUserSelectionDlg dialog

IMPLEMENT_DYNAMIC(CUserSelectionDlg, CDialog)

CUserSelectionDlg::CUserSelectionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUserSelectionDlg::IDD, pParent)
{
	m_nUserSelectedDerivedProtocol = -1;
}

CUserSelectionDlg::~CUserSelectionDlg()
{
}

void CUserSelectionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CUserSelectionDlg, CDialog)
	ON_BN_CLICKED(IDC_RADIO_ADD_PRTOCOL, OnBnClickedRadioAddPrtocol)
	ON_BN_CLICKED(IDC_RADIO_CREATE_FRAME, OnBnClickedRadioCreateFrame)
	ON_BN_CLICKED(IDC_BTN_SAVE_PROTOCOL, OnBnClickedBtnSaveProtocol)
END_MESSAGE_MAP()


// CUserSelectionDlg message handlers


void CUserSelectionDlg::OnBnClickedRadioAddPrtocol()
{
	CProtocolAdderDlg omProtocolAddDlg;
	if(omProtocolAddDlg.DoModal() == true)
	{

	}
}


void CUserSelectionDlg::OnBnClickedRadioCreateFrame()
{
	CCreateFrameDlg omCreateFrame;
	if(omCreateFrame.DoModal() == true)
	{
		m_nUserSelectedDerivedProtocol = omCreateFrame.m_SelectedDerivedProtocolIndex;

	}
}


void CUserSelectionDlg::OnBnClickedBtnSaveProtocol()
{
	CFileDialog oSaveFile(false,".xml","Custom Protocol",  OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, "ProtocolConfig (*.xml)|*.xml||", this);
	oSaveFile.m_ofn.lpstrTitle = "Save Protocol Configuration";
	if(oSaveFile.DoModal() == IDOK)
	{
		string omstrFileName = oSaveFile.GetPathName();
		SaveProtocolConfiguration(omstrFileName);

	}

}

BOOL CUserSelectionDlg::SaveProtocolConfiguration(string omstrFileName)
{
	xmlDocPtr pXMLDocPtr = NULL;       /* document pointer */
    xmlNodePtr pRtNodePtr = NULL;
    xmlDtdPtr dtd = NULL;       /* DTD pointer */

    xmlKeepBlanksDefault(0);

    // Create the document with version 1.0
    pXMLDocPtr = xmlNewDoc(BAD_CAST "1.0");

    CString strEncoding = "UTF-8";

    // Creating the Root node "BUSMAST
    pRtNodePtr = xmlNewNode(NULL, BAD_CAST PROTOCOL_CONFIGURATION);
	xmlDocSetRootElement(pXMLDocPtr, pRtNodePtr);

	//Add last updated time
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	char chTime[20];
	sprintf(chTime," %u/%u/%u  %u:%u:%u:%u",
                     sysTime.wYear, sysTime.wMonth, sysTime.wDay,
                     sysTime.wHour, sysTime.wMinute, sysTime.wSecond,
                     sysTime.wMilliseconds);

	string strData;
	strData = chTime;
	xmlNewChild(pRtNodePtr, NULL, BAD_CAST LAST_UPDATED, BAD_CAST strData.c_str());

	//Add base protocols
	 xmlNodePtr pBaseProtocol = xmlNewNode(NULL, BAD_CAST BASE_PROTOCOLS);
	 xmlAddChild(pRtNodePtr, pBaseProtocol);

	 AddBaseProtocolToConfig(pBaseProtocol );

	 xmlNodePtr pDerivedProtocol = xmlNewNode(NULL, BAD_CAST DERIVED_PROTOCOL);
	 xmlAddChild(pRtNodePtr, pDerivedProtocol);

	 AddDerivedProtocol(pDerivedProtocol);

	 xmlIndentTreeOutput = 1;
	 xmlThrDefIndentTreeOutput(TRUE);

	 if(pXMLDocPtr != NULL)
	 {
		 xmlSaveFormatFileEnc(omstrFileName.c_str(), pXMLDocPtr, "UTF-8", 1);
		 xmlFreeDoc(pXMLDocPtr);
	 }

	 xmlCleanupParser();

	 return S_OK;
}

BOOL CUserSelectionDlg::AddBaseProtocolToConfig(xmlNodePtr pBaseProtocol )
{
	char pchData[1024];
	list<SBASEPROTOCOL>::iterator itrList = CTxCustomDataStore::ouGetTxEthernetDataStoreObj().m_BaseProtocolList.begin();
	while(itrList !=  CTxCustomDataStore::ouGetTxEthernetDataStoreObj().m_BaseProtocolList.end())
	{
		xmlNodePtr pProtocol = xmlNewNode(NULL, BAD_CAST PROTOCOL);
		xmlAddChild(pBaseProtocol, pProtocol);

		//Base Protocol name
		xmlNewChild(pProtocol, NULL, BAD_CAST NAME, BAD_CAST itrList->strName.c_str());

		xmlNodePtr pHeader = xmlNewNode(NULL, BAD_CAST HEADER);
		xmlAddChild(pProtocol, pHeader);

		for(int nHdrCnt = 0;nHdrCnt < itrList->nHeaderCount; nHdrCnt++)
		{
			//Header name
			xmlNewChild(pProtocol, NULL, BAD_CAST NAME, BAD_CAST itrList->sHeaders[nHdrCnt].strName.c_str());

			//No. of bytes
			sprintf(pchData, "%d",itrList->sHeaders[nHdrCnt].nHeaderLength);
			xmlNewChild(pProtocol, NULL, BAD_CAST NO_OF_BYTES, BAD_CAST pchData);  

			//Value of Header
			sprintf(pchData, "%x",itrList->sHeaders[nHdrCnt].nHeaderLength);
			xmlNewChild(pProtocol, NULL, BAD_CAST VALUE, BAD_CAST pchData);
		}
	}
	return S_OK;
}

BOOL CUserSelectionDlg::AddDerivedProtocol(xmlNodePtr pDerivedProtocol)
{
	char pchData[1024];
	list<SDERIVEDPROTOCOL>::iterator itrListDerived = CTxCustomDataStore::ouGetTxEthernetDataStoreObj().m_DerivedProtocolList.begin();
	while(itrListDerived !=  CTxCustomDataStore::ouGetTxEthernetDataStoreObj().m_DerivedProtocolList.end())
	{
		xmlNodePtr pProtocol = xmlNewNode(NULL, BAD_CAST PROTOCOL);
		xmlAddChild(pDerivedProtocol, pProtocol);

		//Derived Protocol name
		xmlNewChild(pProtocol, NULL, BAD_CAST NAME, BAD_CAST itrListDerived->strName.c_str());

		for(int nBasePCnt = 0;nBasePCnt < itrListDerived->nBaseProtocolCnt; nBasePCnt++)
		{
			//Base Protocol name
			xmlNewChild(pProtocol, NULL, BAD_CAST BASE_PROTOCOL, BAD_CAST itrListDerived->BaseProtocol[nBasePCnt].strName.c_str());
		}
	}
	return S_OK;
}