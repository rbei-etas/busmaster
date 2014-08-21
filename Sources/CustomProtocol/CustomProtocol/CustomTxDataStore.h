#pragma once
#include "CustomProtocol_stdafx.h"

using namespace std;

typedef HRESULT (* CHECKSUM_CALCULATION)(u_char* ucData, int nLength);
 //CHECKSUM_CALCULATION pfChecksumCalculation = NULL;
//Structure to store all headers
typedef struct sHeader
{
	int							nHeaderLength;
	string						strName;
	string						strValue;
	sHeader()
	{
		nHeaderLength			= 0;
		strName					= "";
		strValue				= "";
	}
}SHEADER;

typedef struct sBaseProtocol
{
	int							nHeaderCount;
	string						strName;
	SHEADER						sHeaders[10];			//Max 10 headers per protocol
	CHECKSUM_CALCULATION		pfChecksumCalculation; 
	sBaseProtocol()
	{
		nHeaderCount			= 0;
		strName					= "";
		pfChecksumCalculation	= NULL;
	}
}SBASEPROTOCOL;

typedef struct sDerivedProtocol
{
	string						strName;
	int							nBaseProtocolCnt;
	SBASEPROTOCOL				BaseProtocol[10];   //Max allowed base protocol
	sDerivedProtocol()
	{
		strName					= "";
		nBaseProtocolCnt		= 0;
	}
}SDERIVEDPROTOCOL;

class CTxCustomDataStore
{
public:
	static CTxCustomDataStore m_sTxEthernetDataStoreObj;
	static CTxCustomDataStore& ouGetTxEthernetDataStoreObj()
	{
		 return m_sTxEthernetDataStoreObj;
	}
    
	list<SBASEPROTOCOL> m_BaseProtocolList;
	list<SDERIVEDPROTOCOL> m_DerivedProtocolList;
};

//CTxCustomDataStore CTxCustomDataStore::m_sTxEthernetDataStoreObj;