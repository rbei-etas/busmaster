// This is the main DLL file.

#pragma once

#define USAGE_EXPORT
#include "CustomProtocol_Stdafx.h"
#include "CustomProtocol_Extern.h"
#include "Base_EthernetAppLayerProtocol.h"
#include "Base_TxEAppLayerProtocol.h"
#include "CustomProtocol.h"
#include "UserSelectionDlg.h"
#include "CustomTxDataStore.h"


//Class for interpreting received messages

/**
 * CEthernet_Interface construction
 */
CCustomProtocol::CCustomProtocol()
{
    // TODO: add construction code here,
    // Place all significant initialization in InitInstance
}

// The one and only CEthernet_Interface object
CCustomProtocol theApp;

/**
 * CEthernet_Interface initialization
 */
static HINSTANCE ghLangInst=NULL;

BOOL CCustomProtocol::InitInstance()
{
    //// Begin of Multiple Language support
    //if ( CMultiLanguage::m_nLocales <= 0 )    // Not detected yet
    //{
    //    CMultiLanguage::DetectLangID(); // Detect language as user locale
    //    CMultiLanguage::DetectUILanguage();    // Detect language in MUI OS
    //}
    //TCHAR szModuleFileName[MAX_PATH];        // Get Module File Name and path
    //int ret = ::GetModuleFileName(theApp.m_hInstance, szModuleFileName, MAX_PATH);
    //if ( ret == 0 || ret == MAX_PATH )
    //{
    //    ASSERT(FALSE);
    //}
    //// Load resource-only language DLL. It will use the languages
    //// detected above, take first available language,
    //// or you can specify another language as second parameter to
    //// LoadLangResourceDLL. And try that first.
    //ghLangInst = CMultiLanguage::LoadLangResourceDLL( szModuleFileName );
    //if (ghLangInst)
    //{
    //    AfxSetResourceHandle( ghLangInst );
    //}
    //// End of Multiple Language support

    CWinApp::InitInstance();

    return TRUE;
}


class CProtocol : public Base_EAppLayerProtocol
{
	//This function should return the name of the Potocol that is to displayed.
	string GetProtocolName() ;

	//Parse the data recived from the device in the function below and form the return structure
	EMSGDETAILS ParseEthernetData(ERAWMSG strRawMsg);

	//Create the byte array from the EMSGDETAILS structure and form the retuns structure
	EDATATOSEND FormEDataPacket(EMSGDETAILS sMsg) ;
};

static CProtocol* sg_pouProtocol = NULL;

USAGEMODE HRESULT GetCustomAppLayerProtocol(void** ppvInterface)
{
    HRESULT hResult = S_OK;
    if ( NULL == sg_pouProtocol )
    {
        if ((sg_pouProtocol = new CProtocol) == NULL)
        {
            hResult = S_FALSE;
        }
    }
    *ppvInterface = (void*) sg_pouProtocol;  

    return hResult;
}

//This function should return the name of the Potocol that is to displayed.
string CProtocol :: GetProtocolName() 
{
	return "CustomProtocol";
}

//Parse the data recived from the device in the function below and form the return structure
EMSGDETAILS CProtocol :: ParseEthernetData(ERAWMSG strRawMsg)
{
	EMSGDETAILS  oMsgDetails;
	strcpy((char*)oMsgDetails.ucMsgName,"From Dll");
	oMsgDetails.lMsgID = 1234;
	oMsgDetails.nDatalength = 5;
	strcpy((char*)oMsgDetails.ucData , "abcde");
	strcpy((char*)oMsgDetails.ucProtocolName , "From Dll");

	return oMsgDetails;
}

//Create the byte array from the EMSGDETAILS structure and form the retuns structure
EDATATOSEND CProtocol :: FormEDataPacket(EMSGDETAILS sMsg) 
{
	EDATATOSEND oData;
	return oData;
}

class CTxCusotmHeader: public CBase_TxEAppLayerProtocol
{
	//Adds headers of respective protocols which will be sent to ethernet bus.
	HRESULT AddHeaderToData(STDATA& sData);
};

static CBase_TxEAppLayerProtocol* sg_TxCustomProtocol = NULL;

USAGEMODE HRESULT GetCustomTxAppLayerProtocol(void** ppvInterface)
{
	 HRESULT hResult = S_OK;
    if ( NULL == sg_TxCustomProtocol )
    {
        if ((sg_TxCustomProtocol = new CTxCusotmHeader) == NULL)
        {
            hResult = S_FALSE;
        }
    }
    *ppvInterface = (void*) sg_TxCustomProtocol;  

    return hResult;
}

unsigned char hex_digit_value(char c)
{
    if ('0' <= c && c <= '9') { return c - '0'; }
    if ('a' <= c && c <= 'f') { return c + 10 - 'a'; }
    if ('A' <= c && c <= 'F') { return c + 10 - 'A'; }
	c= 0xff;
    return -1;
}

//Adds headers of respective protocols which will be sent to ethernet bus.
HRESULT CTxCusotmHeader::AddHeaderToData(STDATA& sData)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CUserSelectionDlg  omUserDlg;
	if(omUserDlg.DoModal() == true)
	{
		u_char*     byFinalData = new u_char[sData.unDataLength];
		string		strDataWithHeader = "";
		list<SDERIVEDPROTOCOL>::iterator itrListDerived = CTxCustomDataStore::ouGetTxEthernetDataStoreObj().m_DerivedProtocolList.begin();
		std::advance(itrListDerived, omUserDlg.m_nUserSelectedDerivedProtocol);
		for(int nBaseProtCnt = 0;nBaseProtCnt < itrListDerived->nBaseProtocolCnt ; nBaseProtCnt++)
		{
			SBASEPROTOCOL sBaseProt = itrListDerived->BaseProtocol[nBaseProtCnt];
			for(int nHeaderCnt = 0;nHeaderCnt < sBaseProt.nHeaderCount ; nHeaderCnt++)
			{
				strDataWithHeader.append( sBaseProt.sHeaders[nHeaderCnt].chValue);
			}
		}

		//Remove spaces
		int i = 0;
		while(strDataWithHeader.find(' ') != -1)
		{
			if(i >= strDataWithHeader.length())
			{
				break;
			}
			strDataWithHeader.replace(strDataWithHeader.find(' '), 1, "");
			i++;
		}

		if(sData.unDataLength < (strDataWithHeader.length()/2))
		{
			AfxMessageBox("Header length is greater than the datalength. Unable to add header");
			return S_FALSE;
		}
		//Convert 2 characters  to 1 byte  "FF" to 0xFF
		for (int i = 0, j= 0; i < strDataWithHeader.length()/2 ; i++, j+=2)
		{
			byFinalData[i] = (hex_digit_value(strDataWithHeader[j]) << 4) | ( hex_digit_value(strDataWithHeader[j+1]));            
		}
		memcpy(sData.chData, byFinalData, strDataWithHeader.length()/2);
	}
	return S_OK;
}