#include "stdafx.h"
#include "FrameGenerator.h"


using namespace std;

CFrameGenerator::CFrameGenerator(void)
{
}


CFrameGenerator::~CFrameGenerator(void)
{
}


ERRORCODE CFrameGenerator::nGenerateFrames(LDFCluster* ouCluster, ostringstream& omFrameStream)
{
    m_pLDFCluster = ouCluster;

    if(nullptr == m_pLDFCluster)
    {
        return EC_FAILURE;
    }

    ostringstream omStream, omStringStream, omDiagStream, omSporadicStream, omEventTriggStream;
    std::map<UID_ELEMENT, IElement*> FrameMap;
    bool bCheckIfUncondFrameExists = false, bCheckIfDiagFrameExists = false;
    std::list<CLINFrame*> lstUncondFrames, lstEventTriggFrames, lstSporadicFrames, lstDiagFrames;

    if(EC_SUCCESS == m_pLDFCluster->GetElementList(eFrameElement, FrameMap))
    {
        if(FrameMap.size() > 0)
        {
            std::map<UID_ELEMENT, IElement*>::iterator itrFrame = FrameMap.begin();

            while( FrameMap.end() != itrFrame )
            {
                CLINFrame* pFrame = (CLINFrame*)(itrFrame->second);

                if(nullptr != pFrame)
                {
                    eFrameType ouFrameType;
                    pFrame->GetFrameType(ouFrameType);

                    switch(ouFrameType)
                    {
                        case eLIN_Unconditional:
                            lstUncondFrames.push_back(pFrame);
                            break;
                        case eLIN_EventTriggered:
                            lstEventTriggFrames.push_back(pFrame);
                            break;
                        case eLIN_Sporadic:
                            lstSporadicFrames.push_back(pFrame);
                            break;
                        case eLIN_Diagnostic:
                            lstDiagFrames.push_back(pFrame);
                            break;
                    };
                }
                itrFrame++;
            }

            nGenerateUnconditionalFrames(lstUncondFrames, omStringStream);
            nGenerateSporadicFrames(lstSporadicFrames, omSporadicStream);
            nGenerateEventTriggFrames(lstEventTriggFrames, omEventTriggStream);
            nGenerateDiagFrames(lstDiagFrames, omDiagStream);

            omStream << omStringStream.str() << endl << omSporadicStream.str() << endl << omEventTriggStream.str() << endl << omDiagStream.str() << endl;
        }
        else
        {
            omStream << defLIN_GN_FRAMES << defOPEN_BRACE << endl << defCLOSE_BRACE << endl;
            omStringStream << omStream.str();
        }
    }

    if(omStream.str().empty() == false)
    {
        omFrameStream << omStringStream.str() << endl << omSporadicStream.str() << endl << omEventTriggStream.str() << endl << omDiagStream.str() << endl;
    }

    return EC_SUCCESS;
}

ERRORCODE CFrameGenerator::nGenerateUnconditionalFrames(std::list<CLINFrame*> lstUncondFrames, ostringstream& omStringStream)
{
    if(lstUncondFrames.size() <= 0)
    {
        omStringStream << defLIN_GN_FRAMES << defOPEN_BRACE << endl << defCLOSE_BRACE << endl;
        return EC_FAILURE;
    }

    omStringStream << defLIN_GN_FRAMES << defOPEN_BRACE;

for(auto itrFrame : lstUncondFrames)
    {
        nGenerateUncondFrameProperties(itrFrame, omStringStream);
    }

    omStringStream << endl <<  defCLOSE_BRACE << endl;

    return EC_SUCCESS;
}

ERRORCODE CFrameGenerator::nGenerateDiagFrames(std::list<CLINFrame*> lstDiagFrames, ostringstream& omDiagStream)
{
    if(lstDiagFrames.size() <= 0)
    {
        return EC_FAILURE;
    }

    omDiagStream << defLIN_GN_DIAG_FRAMES << defOPEN_BRACE;

for(auto itrFrame : lstDiagFrames)
    {
        nGenerateDiagFrameProperties(itrFrame, omDiagStream);
    }

    omDiagStream << endl <<  defCLOSE_BRACE << endl;

    return EC_SUCCESS;
}

ERRORCODE CFrameGenerator::nGenerateEventTriggFrames(std::list<CLINFrame*> lstUncondFrames, ostringstream& omStringStream)
{
    if(lstUncondFrames.size() <= 0)
    {
        return EC_FAILURE;
    }

    omStringStream << defLIN_GN_EVENT_TRIG_FRAMES << defOPEN_BRACE << endl;

for(auto itrFrame : lstUncondFrames)
    {
        nGenerateEventTrigFrameProperties(itrFrame, omStringStream);
    }

    omStringStream << defCLOSE_BRACE << endl;

    return EC_SUCCESS;
}


ERRORCODE CFrameGenerator::nGenerateSporadicFrames(std::list<CLINFrame*> lstSporadicFrames, ostringstream& omStringStream)
{
    if(lstSporadicFrames.size() <= 0)
    {
        return EC_FAILURE;
    }

    omStringStream << defLIN_GN_SPORADIC_FRAMES << defOPEN_BRACE << endl;

for(auto itrFrame : lstSporadicFrames)
    {
        nGenerateSporadicFrameProperties(itrFrame, omStringStream);
    }

    omStringStream << defCLOSE_BRACE << endl;

    return EC_SUCCESS;
}

ERRORCODE CFrameGenerator::nGenerateUncondFrameProperties(CLINFrame* pFrame, ostringstream& omFrameProps)
{
    if(nullptr == pFrame)
    {
        return EC_FAILURE;
    }

    std::string strFrameName, strTxNode, strSignals, strSigName;
    std::list<IEcu*> ouECUlst;
    std::list<IEcu*>::iterator itrECU;
    std::map<ISignal*, SignalInstanse> mapSignals;

    // Get Signal Properties
    FrameProps ouFrameProps;
    pFrame->GetProperties(ouFrameProps);
    pFrame->GetName(strFrameName);

    // Get ECU list
    pFrame->GetEcus(eTx, ouECUlst);

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

    // Get Signal details
    pFrame->GetSignalList(mapSignals);

    //Sort signals according to start index
    map<unsigned int , ISignal*> ouSortedSignals;
for(auto itrSignal : mapSignals)
    {
        ouSortedSignals[itrSignal.second.m_nStartBit] = itrSignal.first;
    }


for(auto itrSignal : ouSortedSignals)
    {
        itrSignal.second->GetName(strSigName);
        strSignals += "\t\t" + strSigName + defCOMMA + to_string( itrSignal.first ) + ";\n";
    }

    omFrameProps << defNEWLINE << defTAB << strFrameName.c_str() << defCOLON << ouFrameProps.m_ouLINUnConditionFrameProps.m_unId
                 << defCOMMA << strTxNode.c_str() << defCOMMA << ouFrameProps.m_ouLINUnConditionFrameProps.m_nLength
                 << defOPEN_BRACE << defNEWLINE << strSignals.c_str() << defTAB << defCLOSE_BRACE;

    return EC_SUCCESS;
}

ERRORCODE CFrameGenerator::nGenerateDiagFrameProperties(CLINFrame* pFrame, ostringstream& omFrameProps)
{
    if(nullptr == pFrame)
    {
        return EC_FAILURE;
    }

    std::string strFrameName, strSignals, strSigName;
    std::map<ISignal*, SignalInstanse> mapSignals;

    // Get Signal Properties
    FrameProps ouFrameProps;
    pFrame->GetProperties(ouFrameProps);
    pFrame->GetName(strFrameName);

    // Get Signal details
    pFrame->GetSignalList(mapSignals);

    //Sort signals according to start index
    map<unsigned int , ISignal*> ouSortedSignals;
for(auto itrSignal : mapSignals)
    {
        ouSortedSignals[itrSignal.second.m_nStartBit] = itrSignal.first;
    }

    //Print Signals
for(auto itrSignal : ouSortedSignals)
    {
        itrSignal.second->GetName(strSigName);
        strSignals += "\t\t" + strSigName + defCOMMA + to_string( itrSignal.first) + ";\n";
    }

    omFrameProps << defNEWLINE << defTAB << strFrameName.c_str() << defCOLON << defHEX << std::hex << ouFrameProps.m_ouLINDiagnosticFrameProps.m_unId
                 << defSPACE << defOPEN_BRACE << defNEWLINE << strSignals.c_str() << defTAB << defCLOSE_BRACE;

    return EC_SUCCESS;
}


ERRORCODE CFrameGenerator::nGenerateEventTrigFrameProperties(CLINFrame* pFrame, ostringstream& omFrameProps)
{
    if(nullptr == pFrame)
    {
        return EC_FAILURE;
    }

    std::string strEventTrigName, strFrameName, strUnCondFrames;

    // Get Signal Properties
    FrameProps ouFrameProps;
    pFrame->GetProperties(ouFrameProps);
    pFrame->GetName(strEventTrigName);

    unsigned int unCount = 0;

for( auto itrFrame : ouFrameProps.m_ouLINEventTrigFrameProps.m_pouUnconditionalFrame)
    {
        if( (EC_SUCCESS == m_pLDFCluster->GetElement(eFrameElement, UID_ELEMENT(itrFrame.first), (IElement**)&pFrame)) & ( nullptr != pFrame ))
        {
            pFrame->GetName(strFrameName);
        }
        strUnCondFrames += strFrameName;

        unCount++;
        if(unCount < ouFrameProps.m_ouLINEventTrigFrameProps.m_pouUnconditionalFrame.size())
        {
            strUnCondFrames += defCOMMA;
        }
    }

    std::ostringstream  strUncond;
    IElement* pCollisionTable = nullptr;
    std::string strCollisionTable = "";
    if(EC_SUCCESS == m_pLDFCluster->GetElement(eScheduleTableElement, ouFrameProps.m_ouLINEventTrigFrameProps.m_nCollisionResolveTable, &pCollisionTable))
    {
        pCollisionTable->GetName(strCollisionTable);
        strCollisionTable += defCOMMA;
    }

    if(false == strUnCondFrames.empty())
    {
        strUncond << defCOMMA << strUnCondFrames;
    }

    omFrameProps << defTAB << strEventTrigName.c_str() << defCOLON << strCollisionTable.c_str() << ouFrameProps.m_ouLINEventTrigFrameProps.m_unId
                 << strUncond.str() << defSEMICOLON;

    return EC_SUCCESS;
}

ERRORCODE CFrameGenerator::nGenerateSporadicFrameProperties(CLINFrame* pFrame, ostringstream& omFrameProps)
{
    if(nullptr == pFrame)
    {
        return EC_FAILURE;
    }

    std::string strSporadicFrameName, strSporadicFrames;

    // Get Signal Properties
    FrameProps ouFrameProps;
    pFrame->GetProperties(ouFrameProps);
    pFrame->GetName(strSporadicFrameName);

    unsigned int unCount = 0;
    std::string strFrameName;
for( auto itrFrame : ouFrameProps.m_ouLINSporadicFrameProps.m_pouUnconditionalFrame)
    {
        if((EC_SUCCESS == m_pLDFCluster->GetElement(eFrameElement, UID_ELEMENT(itrFrame.first), (IElement**)&pFrame)) && ( nullptr != pFrame ))
        {
            pFrame->GetName(strFrameName);
        }
        strSporadicFrames += strFrameName;

        unCount++;
        if(unCount < ouFrameProps.m_ouLINSporadicFrameProps.m_pouUnconditionalFrame.size())
        {
            strSporadicFrames += defCOMMA;
        }
    }

    omFrameProps << defTAB << strSporadicFrameName.c_str() << defCOLON << strSporadicFrames << defTAB << defSEMICOLON;

    return EC_SUCCESS;
}