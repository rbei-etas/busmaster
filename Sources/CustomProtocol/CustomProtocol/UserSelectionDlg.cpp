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

static string sg_omstrSaveFileName = "";
// CUserSelectionDlg dialog
extern unsigned char hex_digit_value(char c);

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
	//ON_BN_CLICKED(IDC_BTN_SAVE_PROTOCOL, OnBnClickedBtnSaveProtocol)
	ON_BN_CLICKED(IDC_BTN_SAVE_CONFIG, OnBnClickedBtnSaveConfig)
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



BOOL CUserSelectionDlg::SaveProtocolConfiguration()
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
		 //const string str = omstrFileName.c_str();
		 xmlSaveFormatFileEnc(sg_omstrSaveFileName.c_str(), pXMLDocPtr, "UTF-8", 1);
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
		xmlNodePtr pProtocolName = xmlNewChild(pProtocol, NULL, BAD_CAST NAME, BAD_CAST itrList->strName.c_str());
		xmlAddChild(pProtocol, pProtocolName);

		xmlNodePtr pHeader = xmlNewNode(NULL, BAD_CAST HEADER);
		xmlAddChild(pProtocol, pHeader);

		for(int nHdrCnt = 0;nHdrCnt < itrList->nHeaderCount; nHdrCnt++)
		{
			//Header name
			xmlNodePtr pHeaderName = xmlNewChild(pProtocol, NULL, BAD_CAST NAME, BAD_CAST itrList->sHeaders[nHdrCnt].strName.c_str());
			 xmlAddChild(pProtocol, pHeaderName);
			//No. of bytes
			sprintf(pchData, "%d",itrList->sHeaders[nHdrCnt].nHeaderLength);
			xmlNodePtr pNoOfBytes = xmlNewChild(pProtocol, NULL, BAD_CAST NO_OF_BYTES, BAD_CAST pchData); 
			 xmlAddChild(pProtocol, pNoOfBytes);

			//Value of Header
			sprintf(pchData, "%x",itrList->sHeaders[nHdrCnt].chValue);
			xmlNodePtr pValue = xmlNewChild(pProtocol, NULL, BAD_CAST VALUE, BAD_CAST pchData);
			xmlAddChild(pProtocol, pValue);
		}
		itrList++;
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
		xmlNodePtr pDerProtocolName = xmlNewChild(pProtocol, NULL, BAD_CAST NAME, BAD_CAST itrListDerived->strName.c_str());
		xmlAddChild(pProtocol, pDerProtocolName);

		for(int nBasePCnt = 0;nBasePCnt < itrListDerived->nBaseProtocolCnt; nBasePCnt++)
		{
			//Base Protocol name
			xmlNodePtr pBaseProtocolName = xmlNewChild(pProtocol, NULL, BAD_CAST BASE_PROTOCOL, BAD_CAST itrListDerived->BaseProtocol[nBasePCnt].strName.c_str());
			xmlAddChild(pProtocol, pBaseProtocolName);		
		}
		itrListDerived++;
	}
	return S_OK;
}

void CUserSelectionDlg::OnBnClickedBtnLoadProtocol()
{
	CFileDialog oLoadFile(true,".xml","Custom Protocol",  OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, "ProtocolConfig (*.xml)|*.xml||", this);
	oLoadFile.m_ofn.lpstrTitle = "Load Protocol Configuration";
	if(oLoadFile.DoModal() == IDOK)
	{
		string omstrFileName = oLoadFile.GetPathName();
		LoadProtocolConfiguration(omstrFileName);
	}
}

BOOL CUserSelectionDlg::LoadProtocolConfiguration(string omstrFileName)
{
	BOOL nRetValue = S_FALSE;
	xmlDocPtr xmlConfigFiledoc;
	xmlConfigFiledoc = xmlReadFile(omstrFileName.c_str(), "UTF-8", 0);

    CString strEncoding = "UTF-8";

	if ( nullptr != xmlConfigFiledoc )           //1. Try to Load as a XML file
	{
		nRetValue = nLoadBaseProtocol(xmlConfigFiledoc);
		xmlFreeDoc(xmlConfigFiledoc);
		xmlConfigFiledoc = nullptr;
	}
	return S_OK;
}

BOOL CUserSelectionDlg::nLoadBaseProtocol(xmlDocPtr xmlConfigFiledoc)
{
	xmlNodeSetPtr pNodeSet;
	xmlXPathObjectPtr pPathObject;
	xmlNodePtr pNode = nullptr;

	// Updating Global configuration parameters in to xml
	xmlChar* pchPath = (xmlChar*)"//Protocol_Configuration/Base_Protocols";
	pPathObject = xmlUtils::pGetNodes(xmlConfigFiledoc, pchPath);

	if( nullptr != pPathObject )
	{
		pNodeSet = pPathObject->nodesetval;
		//No. of Base protocols
		 if( nullptr != pNodeSet )
        {
            pNode = pNodeSet->nodeTab[0];       //Take First One only
        }
		 INT nCount = pNodeSet->nodeNr;
		 for(int i = 0; i < nCount; i++)
		 {
			 //This node will point to the individual Protocol node
			 //Parse it and store it Datastore
			  LoadBaseProtocol(pNode);
		 }
		
	}

	// Updating Global configuration parameters in to xml
	pchPath = (xmlChar*)"//Protocol_Configuration/Derived_Protocol";
	pPathObject = xmlUtils::pGetNodes(xmlConfigFiledoc, pchPath);
	if( nullptr != pPathObject )
	{
		pNodeSet = pPathObject->nodesetval;
		pNodeSet = pPathObject->nodesetval;
		//No. of Base protocols
		 if( nullptr != pNodeSet )
        {
            pNode = pNodeSet->nodeTab[0];       //Take First One only
        }
		  INT nCount = pNodeSet->nodeNr;
		 for(int i = 0; i < nCount; i++)
		 {
			 //This node will point to the individual Protocol node
			 //Parse it and store it Datastore
			  LoadDerivedProtocol(pNode);
		 }
		
	}
	return S_OK;
}

BOOL CUserSelectionDlg::LoadBaseProtocol(xmlNodePtr pNode)
{
	xmlNodePtr pChildNode = nullptr;
	string strVar;
	char* pchData;

	if( nullptr == pNode )
	{
		AfxMessageBox("Configuration file has been changed manually. Base protocol data is missing");
		return S_FALSE;
	}

	if ((!xmlStrcmp(pNode->name, (const xmlChar*)PROTOCOL))) 
	{
		while(pNode != nullptr)
		{
			pChildNode = pNode->xmlChildrenNode;
			int nHeaderCnt = 0;
			SBASEPROTOCOL    sBaseProtocol;
			while (pChildNode != nullptr)           //loop through the node of "PROTOCOL"
			{
				
				if (xmlUtils::GetDataFrmNode(pChildNode,NAME, strVar))
				{
					sBaseProtocol.strName = strVar;
				}

				if (xmlUtils::GetDataFrmNode(pChildNode,HEADER, strVar))   
				{
					xmlNodePtr   pChildHeader = pChildNode->xmlChildrenNode;
					//Loop through headers and 
					while (pChildHeader != nullptr) 
					{
						if (xmlUtils::GetDataFrmNode(pChildNode,HEADER_NAME, strVar))
						{
							sBaseProtocol.sHeaders[nHeaderCnt].strName   = strVar;
						}
						if (xmlUtils::GetDataFrmNode(pChildNode,NO_OF_BYTES, strVar))
						{
							sBaseProtocol.sHeaders[nHeaderCnt].nHeaderLength   = atoi(strVar.c_str());
						}
						//Get data here
						if (xmlUtils::GetDataFrmNode(pChildNode,VALUE, strVar))
						{
							strcpy(pchData, strVar.c_str());
							char* pch;
							pch = strtok (pchData," ,.-");
							int i =0;
							u_char   BYTE[30];
							while (pch != NULL)
							{
								 BYTE[i]= (hex_digit_value(pch[0]) << 4) | ( hex_digit_value(pch[1]));
								pch = strtok (NULL, " ,.-");
								i++;
							}
							memcpy(sBaseProtocol.sHeaders[nHeaderCnt].chValue, &BYTE, i);
						}
						
						pChildHeader = pChildHeader->next;
						nHeaderCnt++;
					}
				}
				
				pChildNode = pChildNode->next;
			}
			CTxCustomDataStore::ouGetTxEthernetDataStoreObj().m_BaseProtocolList.push_back(sBaseProtocol);
			pNode = pNode->next;
		}
	}
	return S_OK;
}

BOOL CUserSelectionDlg::LoadDerivedProtocol(xmlNodePtr pNode)
{
	xmlNodePtr pChildNode = nullptr;
	string strVar;
	

	if( nullptr == pNode )
	{
		AfxMessageBox("Configuration file has been changed manually. Base protocol data is missing");
		return S_FALSE;
	}

	if ((!xmlStrcmp(pNode->name, (const xmlChar*)PROTOCOL))) 
	{
		while(pNode != nullptr)
		{
			pChildNode = pNode->xmlChildrenNode;
			int nHeaderCnt = 0;
			SDERIVEDPROTOCOL    sDerivedProtocol;
			while (pChildNode != nullptr)           //loop through the node of "CAN_Tx_Window"
			{
				
				if (xmlUtils::GetDataFrmNode(pChildNode,NAME, strVar))
				{
					sDerivedProtocol.strName = strVar;
				}

				if (xmlUtils::GetDataFrmNode(pChildNode,BASE_PROTOCOL, strVar))
				{
					sDerivedProtocol.BaseProtocol[nHeaderCnt].strName   = strVar;
				}
				
				pChildNode = pChildNode->next;
			}
			CTxCustomDataStore::ouGetTxEthernetDataStoreObj().m_DerivedProtocolList.push_back(sDerivedProtocol);
			pNode = pNode->next;
		}
	}
	return S_OK;
}


void CUserSelectionDlg::OnBnClickedBtnSaveConfig()
{
	CFileDialog oSaveFile(false,".xml","Custom Protocol",  OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, "ProtocolConfig (*.xml)|*.xml||", this);
	oSaveFile.m_ofn.lpstrTitle = "Save Protocol Configuration";
	if(oSaveFile.DoModal() == IDOK)
	{
		
		SaveProtocolConfiguration();

	}
}
