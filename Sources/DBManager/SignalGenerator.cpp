#include "stdafx.h"
#include "SignalGenerator.h"
#include "LDFCluster.h"
#include "LDFTagDefines.h"
#include <iostream>
#include "Defines.h"

using namespace std;

SignalGenerator::SignalGenerator(void)
{
}


SignalGenerator::~SignalGenerator(void)
{
}

ERRORCODE SignalGenerator::nGenerateSignals(LDFCluster* pCluster, std::ostringstream& omStream)
{
    m_pLDFCluster = pCluster;

    if(nullptr == m_pLDFCluster)
    {
        return EC_FAILURE;
    }

    std::ostringstream omStringStream, omSigDiagStream;
    std::map<UID_ELEMENT, IElement*> SignalMap;
    std::list<CLINSignal*> lstNormalSignals, lstDiagSignals;

    if(EC_SUCCESS == m_pLDFCluster->GetElementList(eSignalElement, SignalMap))
    {
        if(SignalMap.size() > 0)
        {
            std::map<UID_ELEMENT, IElement*>::iterator itrSignal = SignalMap.begin();

            while( SignalMap.end() != itrSignal )
            {
                CLINSignal* pSignal = (CLINSignal*)(itrSignal->second);

                SignalProps ouSignalProps;

                if(nullptr != pSignal)
                {
                    pSignal->GetProperties(ouSignalProps);

                    if(ouSignalProps.m_ouLINSignalProps.m_ouSignalType == eSignalNormal)
                    {
                        lstNormalSignals.push_back(pSignal);
                    }
                    else if(ouSignalProps.m_ouLINSignalProps.m_ouSignalType == eSignalDiag)
                    {
                        lstDiagSignals.push_back(pSignal);
                    }
                }

                itrSignal++;
            }

            nGenerateNormalSignals(lstNormalSignals, omStringStream);
            nGenerateDiagnosticsSignals(lstDiagSignals, omSigDiagStream);

            omStream << omStringStream.str() << omSigDiagStream.str();
        }
        else
        {
            omStream << defLIN_GN_SIGNALS << defOPEN_BRACE << endl << defCLOSE_BRACE << endl << endl;
        }
    }

    return EC_SUCCESS;
}


ERRORCODE SignalGenerator::nGenerateNormalSignals(std::list<CLINSignal*> lstNormalSignals, std::ostringstream& omStream)
{
    if(lstNormalSignals.size() <= 0)
    {
        omStream << defLIN_GN_SIGNALS << defOPEN_BRACE << endl << defCLOSE_BRACE << endl << endl;
        return EC_FAILURE;
    }

    std::ostringstream omStringStream;
for( auto itrSignal : lstNormalSignals)
    {
        std::ostringstream omSignalStrmProps;

        // Generate signal properties
        nGenerateNormalSignalProperties(itrSignal, omSignalStrmProps);
        omStringStream << omSignalStrmProps.str();
    }

    omStream << defLIN_GN_SIGNALS << defOPEN_BRACE << endl << omStringStream.str() << defCLOSE_BRACE << endl << endl;

    return EC_SUCCESS;
}

ERRORCODE SignalGenerator::nGenerateDiagnosticsSignals(std::list<CLINSignal*> lstDiagSignals, std::ostringstream& omStream)
{
    if(lstDiagSignals.size() <= 0)
    {
        return EC_FAILURE;
    }

    std::ostringstream omSigDiagStream;
for( auto itrDiagSig : lstDiagSignals)
    {
        ostringstream ouSignalDiagStrmProps;

        // Generate Dignostic signal properties
        nGenerateDiagSignalProperties(itrDiagSig, ouSignalDiagStrmProps);
        omSigDiagStream << ouSignalDiagStrmProps.str();
    }

    if(omSigDiagStream.str().empty() == false)
    {
        omStream << defLIN_GN_DIAG_SIGNAL << defOPEN_BRACE << endl << omSigDiagStream.str() << defCLOSE_BRACE << endl << endl;
    }

    return EC_SUCCESS;
}


ERRORCODE SignalGenerator::nGenerateNormalSignalProperties(CLINSignal* pSignal, ostringstream& omSigProps)
{
    if(nullptr == pSignal)
    {
        return EC_FAILURE;
    }

    std::string strSigName, strRxEcus, strTxNode, strInitVals;
    std::list<IEcu*> ouECUlst;
    std::list<IEcu*>::iterator itrECU;

    // Get Signal Properties
    SignalProps ouSignalProps;
    pSignal->GetProperties(ouSignalProps);

    pSignal->GetName(strSigName);

    // Get ECU list
    pSignal->GetEcus(eTx, ouECUlst);

    // Get Signal Publisher
    if(ouECUlst.size() > 0)
    {
        itrECU= ouECUlst.begin();
        IEcu* objEcu = (IEcu*)*itrECU;

        if(nullptr != objEcu)
        {
            objEcu->GetName(strTxNode);
        }
    }

    ouECUlst.clear();
    // Get Subscribers
    pSignal->GetEcus(eRx, ouECUlst);
    nGetSubscribers(ouECUlst, strRxEcus);

    // Generate Signal Init values as per value type
    nGetSignalInitValues(ouSignalProps, strInitVals);

    ostringstream strSigRxEcus;
    if(strRxEcus.empty() == false)
    {
        strSigRxEcus <<  defCOMMA << strRxEcus.c_str();
    }
    omSigProps << defTAB << strSigName.c_str() << defCOLON << ouSignalProps.m_ouLINSignalProps.m_nLength
               << defCOMMA << strInitVals.c_str() << defCOMMA << strTxNode.c_str() << strSigRxEcus.str() << defSEMICOLON;

    return EC_SUCCESS;
}


ERRORCODE SignalGenerator::nGenerateDiagSignalProperties(CLINSignal* pSignal, ostringstream& omSigProps)
{
    if(nullptr == pSignal)
    {
        return EC_FAILURE;
    }

    std::string strSigName, strInitVals;

    // Get Signal Properties
    SignalProps ouSignalProps;
    pSignal->GetProperties(ouSignalProps);

    pSignal->GetName(strSigName);

    // Generate Signal Init values as per value type
    nGetSignalInitValues(ouSignalProps, strInitVals);

    omSigProps << defTAB << strSigName.c_str() << defCOLON << ouSignalProps.m_ouLINSignalProps.m_nLength
               << defCOMMA << 0  << defSEMICOLON; //TODO :

    return EC_SUCCESS;
}


ERRORCODE SignalGenerator::nGetSignalInitValues(SignalProps ouSignalProps, std::string& omSigInitValues)
{
    SignalInitVal ouSigInitVal;
    ouSigInitVal.unInitVal = ouSignalProps.m_ouLINSignalProps.m_nIntialValue;

    // Generate Signal init values
    if(ouSignalProps.m_ouLINSignalProps.m_ouValueType == eByteArray)
    {
        int nSignalLength = (ouSignalProps.m_ouLINSignalProps.m_nLength/8)-1;
        for(int unIndex = nSignalLength; unIndex >= 0;)
        {
            omSigInitValues += to_string(ouSigInitVal.chInitVal[unIndex]);
            unIndex--;
            if(unIndex >= 0)
            {
                omSigInitValues += defCOMMA;
            }
        }
        if(omSigInitValues.empty() == false)
        {
            omSigInitValues = defOPEN_BRACE + omSigInitValues + defCLOSE_BRACE;
        }
    }
    else if(ouSignalProps.m_ouLINSignalProps.m_ouValueType == eScalar)
    {
        omSigInitValues = to_string(ouSignalProps.m_ouLINSignalProps.m_nIntialValue);
    }

    return EC_SUCCESS;
}

ERRORCODE SignalGenerator::nGetSubscribers(std::list<IEcu*> ouECUlst, std::string& strRxEcus)
{
    strRxEcus = "";
    std::string strNodeName;
    std::list<IEcu*>::iterator itrECU;

    // Generate Subscribers
    if(ouECUlst.size() > 0)
    {
        itrECU = ouECUlst.begin();

        while(ouECUlst.end() != itrECU)
        {
            IEcu* objEcu = (IEcu*)*itrECU;

            if(nullptr != objEcu)
            {
                objEcu->GetName(strNodeName);
            }
            strRxEcus += strNodeName.c_str();
            itrECU++;
            if(ouECUlst.end() != itrECU)
            {
                strRxEcus += defCOMMA;
            }
        }
    }
    else
    {
        return EC_FAILURE;
    }

    return EC_SUCCESS;
}

ERRORCODE SignalGenerator::nGenerateSignalEncodingTypes(LDFCluster* ouCluster, std::ostringstream& omSigEncoding)
{
    m_pLDFCluster = ouCluster;

    std::map<UID_ELEMENT, IElement*> ouCodingMap;
    std::string strCodingName;

    ICoding* pCoding;
    CompuMethodProps ouCompuProps;

    m_pLDFCluster->GetElementList(eCodingElement, ouCodingMap);
    std::ostringstream omEncodings;
for(auto itrCoding : ouCodingMap)
    {
        pCoding = (ICoding*)itrCoding.second;

        if(nullptr != pCoding)
        {
            pCoding->GetName(strCodingName);
            pCoding->GetProperties(ouCompuProps);
        }

        std::ostringstream omSigEncodings;
        nGeneratePhysicalEncodings(strCodingName, ouCompuProps, omSigEncodings);
        nGenerateLogicalEncodings(strCodingName, ouCompuProps, omSigEncodings);

        if(ouCompuProps.m_ouLinCompuMethods.m_bBCD == true)
        {
            omSigEncodings << defBCD_VALUE;
        }

        if(ouCompuProps.m_ouLinCompuMethods.m_bASCII == true)
        {
            omSigEncodings << defASCII_VALUE;
        }

        //if(omSigEncodings.str().empty() == false)
        {
            omEncodings << defNEWLINE <<  defTAB << strCodingName.c_str() << defOPEN_BRACE << endl
                        << omSigEncodings.str() << defTAB << defCLOSE_BRACE;
        }
    }

    if(omEncodings.str().empty() == false)
    {
        omSigEncoding << defSIGNAL_ENCODING << defOPEN_BRACE << omEncodings.str() << endl << defCLOSE_BRACE << endl;
    }
    /*else
    {
        if(strCodingName.empty() == false)
        {
            omSigEncoding << defSIGNAL_ENCODING << defOPEN_BRACE << endl << strCodingName.c_str() << defOPEN_BRACE << endl
                << defCLOSE_BRACE << endl << defCLOSE_BRACE << endl;
        }
    }*/

    return EC_SUCCESS;
}

ERRORCODE SignalGenerator::nGeneratePhysicalEncodings(std::string strCodingName, CompuMethodProps ouCompuProps, std::ostringstream& omEncodings)
{
for(auto itrPhysical : ouCompuProps.m_ouLinCompuMethods.m_ouPhysicalValueList)
    {
        omEncodings << defTWOTAB << defPHYSICAL_VALUE << defCOMMA << itrPhysical.m_unMin << defCOMMA << itrPhysical.m_unMax << defCOMMA
                    << itrPhysical.m_dFactor << defCOMMA << itrPhysical.m_dOffset ;

        if(itrPhysical.m_strTextInfo.empty() == false)
        {
            omEncodings << defCOMMA << defDOUBLEQUOTE << itrPhysical.m_strTextInfo.c_str() << defDOUBLEQUOTE;
        }

        omEncodings << defSEMICOLON;
    }
    return EC_SUCCESS;
}

ERRORCODE SignalGenerator::nGenerateLogicalEncodings(std::string strCodingName, CompuMethodProps ouCompuProps, std::ostringstream& omEncodings)
{
for(auto itrLogical : ouCompuProps.m_ouLinCompuMethods.m_ouLogicalValueList)
    {
        omEncodings << defTWOTAB << defLOGICAL_VALUE << defCOMMA << itrLogical.first << defCOMMA << defDOUBLEQUOTE
                    << itrLogical.second << defDOUBLEQUOTE << defSEMICOLON;
    }

    return EC_SUCCESS;
}

ERRORCODE SignalGenerator::nGenerateSignalRepresentation(LDFCluster* ouCluster, std::ostringstream& omSigRepresentation)
{

    m_pLDFCluster = ouCluster;

    if(nullptr == m_pLDFCluster)
    {
        return EC_FAILURE;
    }

    std::map<UID_ELEMENT, IElement*> ouCodingMap;
    std::map<UID_ELEMENT, ISignal*> ouSignalMap;
    std::string strCodingName, strSignalName;
    m_pLDFCluster->GetElementList(eCodingElement, ouCodingMap);
    ICoding* pCoding = nullptr;
    std::ostringstream omCodings;
for(auto itrCoding : ouCodingMap)
    {
        pCoding = (ICoding*)itrCoding.second;
        ouSignalMap.clear();

        if(nullptr != pCoding)
        {
            pCoding->GetName(strCodingName);
            pCoding->GetSignalList(ouSignalMap);
        }

        std::ostringstream omSignals;
        unsigned int nCount = 0;
for(auto itrSignal : ouSignalMap)
        {
            itrSignal.second->GetName(strSignalName);
            omSignals << strSignalName.c_str();

            nCount++;
            if(nCount < ouSignalMap.size())
            {
                omSignals << defCOMMA;
            }
        }

        if(omSignals.str().empty() == false)
        {
            omCodings << defTAB << strCodingName.c_str() << defCOLON
                      << omSignals.str() << defSEMICOLON;
        }
    }

    if(omCodings.str().empty() == false)
    {
        omSigRepresentation << defSIGNAL_REPRESENTATION << defOPEN_BRACE << endl << omCodings.str() << defCLOSE_BRACE;
    }
    return EC_SUCCESS;
}

ERRORCODE SignalGenerator::nGenerateSignalGroups(LDFCluster* ouCluster, std::ostringstream& omSigGroupsDetails)
{
    m_pLDFCluster = ouCluster;

    if(nullptr == m_pLDFCluster)
    {
        return EC_FAILURE;
    }

    ISignalGroup* pSignalGroup = nullptr;
    std::map<UID_ELEMENT, IElement*> mapSignalGroups;
    std::map<ISignal*, SignalInstanse> mapSignalList;
    ISignal* pISignal = nullptr;
    SignalGroupProps omSigGrpProps;
    std::string omSigGrpName, omSignalName;
    std::ostringstream omSigGroups;
    m_pLDFCluster->GetElementList(eSignalGroupElement, mapSignalGroups);

for(auto itrSignalGrp : mapSignalGroups)
    {
        std::ostringstream omSignals;
        mapSignalList.clear();
        pSignalGroup = (ISignalGroup*)itrSignalGrp.second;
        pSignalGroup->GetName(omSigGrpName);

        pSignalGroup->GetSignalList(mapSignalList);

        pSignalGroup->GetProperties(omSigGrpProps);

for(auto itrSignal : mapSignalList )
        {
            pISignal = (ISignal*)itrSignal.first;

            if(nullptr != pISignal)
            {
                pISignal->GetName(omSignalName);
            }

            omSignals << defTWOTAB << omSignalName.c_str() <<  defCOMMA <<  itrSignal.second.m_nStartBit << defSEMICOLON;
        }

        if(false == omSignals.str().empty())
        {
            omSigGroups << defTAB << omSigGrpName.c_str() << defCOLON << omSigGrpProps.m_ouLinSignalGroupProps.m_nGroupSize
                        << defOPEN_BRACE << endl << omSignals.str() << defTAB << defCLOSE_BRACE << endl;
        }
    }

    if(false == omSigGroups.str().empty())
    {
        omSigGroupsDetails << defSIGNAL_GROUPS << defOPEN_BRACE << endl << omSigGroups.str() << defCLOSE_BRACE << endl << endl;
    }

    return EC_SUCCESS;
}