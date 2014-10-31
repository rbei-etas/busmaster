#include "LDFUtility.h"
#include "LDFTagDefines.h"
#include "Defines.h"
#include "qtreewidget.h"
#include <cmath>
#include <limits>
std::string g_strEdiagType[eLIN_SID_ALL] = {" Unconditionl Frame",
        "MasterReq",
        "SlaveResp",
        "AssignFrameId",
        "UnAssignFrameId",
        "AssignNAD",
        "ConditionalChangeNAD",
        "Data Dump",
        "Save Configuration",
        "Assign Frame Range",
        "Free Format",
        "Read By Identifier"
                                           };

QLDFTableWidgetItem::QLDFTableWidgetItem(QString strName):QTableWidgetItem(strName)
{
    setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter );
    QFont font1;
    font1.setFamily(QStringLiteral("Courier New"));
    font1.setPointSize(10);
    setFont(font1);
}


QLDFViewerProcess::QLDFViewerProcess()
{
    m_ouTempFile = nullptr;
}


void QLDFViewerProcess::start()
{
    if ( LDFDatabaseManager::GetDatabaseManager()->GetLDFFilePath().size() > 0 )
    {
        m_ouTempFile = new QTemporaryFile();
        if ( m_ouTempFile->open());
        {
            int n = LDFDatabaseManager::GetDatabaseManager()->GetLDFCluster()->SaveToFile(m_ouTempFile->fileName().toStdString());
            QStringList arguments;
            arguments << m_ouTempFile->fileName();
            QProcess::start("LDFViewer.exe", arguments);
            connect( this, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(OnProcessExit()) );
            m_ouTempFile->setAutoRemove(true);
        }
    }
}

void QLDFViewerProcess::OnProcessExit()
{
    if ( nullptr != m_ouTempFile )
    {
        delete m_ouTempFile;
        m_ouTempFile = nullptr;
    }
    deleteLater();
}


QString toString( QVariant& ouVariant )
{
    QString strTemp;
    bool bHex = LDFDatabaseManager::GetDatabaseManager()->bIsDisplayHexOn();
    switch (ouVariant.type())
    {
        case QVariant::Int:
        case QVariant::LongLong:
            if ( false == bHex )
            {
                return strTemp.sprintf("%d", ouVariant.value<unsigned int>());
            }
            else
            {
                return strTemp.sprintf("0x%X", ouVariant.value<unsigned int>());
            }
            break;
        case QVariant::UInt:
        case QVariant::ULongLong:
            if ( false == bHex )
            {
                return strTemp.sprintf("%u", ouVariant.value<unsigned int>());
            }
            else
            {
                return strTemp.sprintf("0x%X", ouVariant.value<unsigned int>());
            }
            break;
        case QVariant::Double:
            return strTemp.sprintf("%lf", ouVariant.value<double>());
        case QVariant::String:
            return ouVariant.value<QString>();
        default:
            return ouVariant.toString();
    }
    return ouVariant.toString();
}

int GetString(eFrameType oueFrameType, std::string& strString)
{
    strString = "";
    int nRetVal = 0;
    switch (oueFrameType)
    {
        case eLIN_Unconditional:
            strString = "UnConditional";
            break;
        case eLIN_Sporadic:
            strString = "Sporadic";
            break;
        case eLIN_EventTriggered:
            strString = "EventTriggered";
            break;
        case eLIN_Diagnostic:
            strString = "Diagnostic";
            break;
        default:
            nRetVal = -1;
            break;
    }
    return nRetVal;
}

int GetString(eDiagType oueLinDiagType, std::string& strString)
{
    strString = "";
    if ( oueLinDiagType >= 0 && oueLinDiagType <= eLIN_SID_ALL )
    {
        strString = g_strEdiagType[oueLinDiagType];
        return 0;
    }
    return -1;
}

int GetType(std::string strString, eDiagType& ouDiagType)
{
    for(int nType = eLIN_MASTER_FRAME_ID; nType < eLIN_SID_ALL; nType++)
    {
        if(strString == g_strEdiagType[nType])
        {
            ouDiagType = (eDiagType)nType;
            return 0;
        }
    }
    return -1;
}


int CreateDefaultItems(double fLDFVersion)
{
    LDFDatabaseManager::GetDatabaseManager()->GetLDFCluster()->Clear();

    CreateDefaultPropertySettings(fLDFVersion);
    CreateDefauleNodeItems(fLDFVersion);

    return EC_SUCCESS;
}

int CreateDefaultPropertySettings(double fLDFVersion)
{
    LIN_Settings ouLINSettings;

    ouLINSettings.m_dProtocolVers = fLDFVersion;
    ouLINSettings.m_dLanguageVers = fLDFVersion;

    ouLINSettings.m_strHeaderComment = "// LIN Description File ";
    //ouLINSettings.m_strChannelName
    ouLINSettings.m_dBaudRate = 19.2;

    LDFDatabaseManager::GetDatabaseManager()->GetLDFCluster()->SetProperties(eLINClusterProperties, (void*)&ouLINSettings);

    return EC_SUCCESS;
}
int CreateMasterDiagFrame(IFrame*& pouFrame, ICluster* pBaseCluster)
{
    /*FrameProps ouFrameProps;
    pBaseCluster->CreateElement(eFrameElement, (IElement**)&pouFrame);

    pouFrame->SetName(std::string(defLINMasterFrameName));
    pouFrame->GetProperties(ouFrameProps);
    ouFrameProps.m_eFrameType = eLIN_Diagnostic;
    ouFrameProps.m_ouLINDiagnosticFrameProps.m_eDiagType = eLIN_MASTER_FRAME_ID;
    ouFrameProps.m_ouLINDiagnosticFrameProps.m_nLength = 8;
    ouFrameProps.m_ouLINDiagnosticFrameProps.m_unId = 0x3C;
    pouFrame->SetProperties(ouFrameProps);

    std::map<UID_ELEMENT, IElement*> ouEcuMap;
    pBaseCluster->GetElementList(eEcuElement, ouEcuMap);
    eEcuType oueEcuType;
    UID_ELEMENT uidElement = pouFrame->GetUniqueId();
    for ( auto itrEcu : ouEcuMap )
    {
        IEcu* pouEcu = (IEcu*)itrEcu.second;
        pouEcu->GetEcuType(oueEcuType);
        eDIR oueDir = (eLIN_Master == oueEcuType) ? eTx : eRx;
        pouEcu->MapFrame(oueDir, uidElement);
    }

    //Signals
    ISignal* pouSignal;
    char chName[33];
    SignalInstanse ouSignalInstanse;
    SignalProps ouSignalProps;
    ouSignalProps.m_ouLINSignalProps.m_nIntialValue = 0;
    ouSignalProps.m_ouLINSignalProps.m_nLength = 8;
    ouSignalProps.m_ouLINSignalProps.m_ouDataType = eUnsigned;
    ouSignalProps.m_ouLINSignalProps.m_ouEndianess = eIntel;
    ouSignalProps.m_ouLINSignalProps.m_ouValueType = eScalar;
    ouSignalProps.m_ouLINSignalProps.m_ouSignalType = eSignalDiag;

    UID_ELEMENT uid = INVALID_UID_ELEMENT;
    for ( int i = 0 ; i < 8; i++ )
    {
        pBaseCluster->CreateElement(eSignalElement, (IElement**)&pouSignal);
        ouSignalInstanse.m_nStartBit = i * 8;
        sprintf_s(chName, defLINMasterSignalName, i);
        pouSignal->SetName(std::string(chName));
        pouSignal->SetProperties(ouSignalProps);
        uid = pouSignal->GetUniqueId();
        pouFrame->MapSignal( uid, ouSignalInstanse );
    }

    return 0;*/

    //Slave
    FrameProps ouFrameProps;
    pBaseCluster->CreateElement(eFrameElement, (IElement**)&pouFrame);

    pouFrame->SetName(std::string(defLINMasterFrameName));
    pouFrame->GetProperties(ouFrameProps);
    ouFrameProps.m_eFrameType = eLIN_Diagnostic;
    ouFrameProps.m_ouLINDiagnosticFrameProps.m_eDiagType = eLIN_MASTER_FRAME_ID;
    ouFrameProps.m_ouLINDiagnosticFrameProps.m_nLength = 8;
    ouFrameProps.m_ouLINDiagnosticFrameProps.m_unId = 0x3C;
    pouFrame->SetProperties(ouFrameProps);

    std::map<UID_ELEMENT, IElement*> ouEcuMap;
    pBaseCluster->GetElementList(eEcuElement, ouEcuMap);
    eEcuType oueEcuType;
    UID_ELEMENT uidElement = pouFrame->GetUniqueId();
    UID_ELEMENT uidMaster;
    std::list<UID_ELEMENT> uidSlaveList;
for ( auto itrEcu : ouEcuMap )
    {
        eDIR oueDir;
        IEcu* pouEcu = (IEcu*)itrEcu.second;
        pouEcu->GetEcuType(oueEcuType);
        if (eLIN_Master == oueEcuType)
        {
            uidMaster = pouEcu->GetUniqueId();
            pouFrame->MapNode(eTx, uidMaster);
            oueDir = eTx;
        }
        else
        {
            UID_ELEMENT uidTemp = pouEcu->GetUniqueId();
            pouFrame->MapNode(eRx, uidTemp);
            uidSlaveList.push_back(uidTemp);
            oueDir = eRx;
        }
        pouEcu->MapFrame(oueDir, uidElement);
    }
    //Signals
    ISignal* pouSignal;
    char chName[33];
    SignalInstanse ouSignalInstanse;
    SignalProps ouSignalProps;
    ouSignalProps.m_ouLINSignalProps.m_nIntialValue = 0;
    ouSignalProps.m_ouLINSignalProps.m_nLength = 8;
    ouSignalProps.m_ouLINSignalProps.m_ouDataType = eUnsigned;
    ouSignalProps.m_ouLINSignalProps.m_ouEndianess = eIntel;
    ouSignalProps.m_ouLINSignalProps.m_ouValueType = eScalar;
    ouSignalProps.m_ouLINSignalProps.m_ouSignalType = eSignalDiag;

    UID_ELEMENT uid = INVALID_UID_ELEMENT;
    for ( int i = 0 ; i < 8; i++ )
    {
        pBaseCluster->CreateElement(eSignalElement, (IElement**)&pouSignal);
        ouSignalInstanse.m_nStartBit = i * 8;
        sprintf_s(chName, defLINMasterSignalName, i);
        pouSignal->SetName(std::string(chName));
        pouSignal->SetProperties(ouSignalProps);
        uid = pouSignal->GetUniqueId();
        pouFrame->MapSignal( uid, ouSignalInstanse );
        pouSignal->MapNode(eTx, uidMaster);
for ( auto itr : uidSlaveList )
        {
            pouSignal->MapNode(eRx, itr);
        }

    }

    return 0;

}

int CreateSlaveDiagFrame(IFrame*& pouFrame, ICluster* pBaseCluster)
{
    //Slave
    FrameProps ouFrameProps;
    pBaseCluster->CreateElement(eFrameElement, (IElement**)&pouFrame);

    pouFrame->SetName(std::string(defLINSlaveFrameName));
    pouFrame->GetProperties(ouFrameProps);
    ouFrameProps.m_eFrameType = eLIN_Diagnostic;
    ouFrameProps.m_ouLINDiagnosticFrameProps.m_eDiagType = eLIN_SLAVE_FRAME_ID;
    ouFrameProps.m_ouLINDiagnosticFrameProps.m_nLength = 8;
    ouFrameProps.m_ouLINDiagnosticFrameProps.m_unId = 0x3D;
    pouFrame->SetProperties(ouFrameProps);

    std::map<UID_ELEMENT, IElement*> ouEcuMap;
    pBaseCluster->GetElementList(eEcuElement, ouEcuMap);
    eEcuType oueEcuType;
    UID_ELEMENT uidElement = pouFrame->GetUniqueId();
    UID_ELEMENT uidMaster;
    std::list<UID_ELEMENT> uidSlaveList;
for ( auto itrEcu : ouEcuMap )
    {
        eDIR oueDir;
        IEcu* pouEcu = (IEcu*)itrEcu.second;
        pouEcu->GetEcuType(oueEcuType);
        if (eLIN_Master == oueEcuType)
        {
            uidMaster = pouEcu->GetUniqueId();
            pouFrame->MapNode(eRx, uidMaster);
            oueDir = eRx;
        }
        else
        {
            UID_ELEMENT uidTemp = pouEcu->GetUniqueId();
            pouFrame->MapNode(eTx, uidTemp);
            uidSlaveList.push_back(uidTemp);
            oueDir = eTx;
        }
        pouEcu->MapFrame(oueDir, uidElement);
    }
    //Signals
    ISignal* pouSignal;
    char chName[33];
    SignalInstanse ouSignalInstanse;
    SignalProps ouSignalProps;
    ouSignalProps.m_ouLINSignalProps.m_nIntialValue = 0;
    ouSignalProps.m_ouLINSignalProps.m_nLength = 8;
    ouSignalProps.m_ouLINSignalProps.m_ouDataType = eUnsigned;
    ouSignalProps.m_ouLINSignalProps.m_ouEndianess = eIntel;
    ouSignalProps.m_ouLINSignalProps.m_ouValueType = eScalar;
    ouSignalProps.m_ouLINSignalProps.m_ouSignalType = eSignalDiag;

    UID_ELEMENT uid = INVALID_UID_ELEMENT;
    for ( int i = 0 ; i < 8; i++ )
    {
        pBaseCluster->CreateElement(eSignalElement, (IElement**)&pouSignal);
        ouSignalInstanse.m_nStartBit = i * 8;
        sprintf_s(chName, defLINSlaveSignalName, i);
        pouSignal->SetName(std::string(chName));
        pouSignal->SetProperties(ouSignalProps);
        uid = pouSignal->GetUniqueId();
        pouFrame->MapSignal( uid, ouSignalInstanse );
        pouSignal->MapNode(eRx, uidMaster);
for ( auto itr : uidSlaveList )
        {
            pouSignal->MapNode(eTx, itr);
        }

    }

    return 0;
}

int CreateDefauleNodeItems(double fLDFVersion)
{
    IElement* pMaster, *pSlave;
    std::string strName;

    LDFDatabaseManager::GetDatabaseManager()->GetLDFCluster()->EnableEventNotofications(true);

    // Creating Master Element
    LDFDatabaseManager::GetDatabaseManager()->GetLDFCluster()->CreateElement(eEcuElement, &pMaster);

    // Set Master Properties
    strName="MasterECU";
    pMaster->SetName(strName);
    EcuProperties ouECUMasterProps;
    ouECUMasterProps.m_eEcuType = eLIN_Master;
    ouECUMasterProps.m_ouMasterProps.m_fJitter=0.0;
    ouECUMasterProps.m_ouMasterProps.m_fTimeBase= 1;

    if(nullptr != pMaster)
    {
        ((IEcu*)pMaster)->SetProperties(ouECUMasterProps);
    }

    // Create Slave Element
    LDFDatabaseManager::GetDatabaseManager()->GetLDFCluster()->CreateElement(eEcuElement, &pSlave);

    // Set Slave Properties
    strName="Slave";
    pSlave->SetName(strName);

    EcuProperties ouSlaveProps;
    ouSlaveProps.m_eEcuType = eLIN_Slave;
    ouSlaveProps.m_ouSlavePros.m_fProtocolVersion=fLDFVersion;
    ouSlaveProps.m_ouSlavePros.m_dNASTimeout = 1000;
    ouSlaveProps.m_ouSlavePros.m_dNCRTimeout=1000;
    ouSlaveProps.m_ouSlavePros.m_dP2Min=50;
    ouSlaveProps.m_ouSlavePros.m_dSTMin=0;
    ouSlaveProps.m_ouSlavePros.m_nConfiguredNAD=1;
    ouSlaveProps.m_ouSlavePros.m_nInitialNAD=1;
    ouSlaveProps.m_ouSlavePros.m_nFunctionId=0;
    ouSlaveProps.m_ouSlavePros.m_nSupplierId=0;
    ouSlaveProps.m_ouSlavePros.m_nVariant=40;
    ouSlaveProps.m_ouSlavePros.m_nFaultStateSignals.clear();
    ouSlaveProps.m_ouSlavePros.m_nRespErrSignal=INVALID_UID_ELEMENT;

    if(nullptr != pSlave)
    {
        ((IEcu*)pSlave)->SetProperties(ouSlaveProps);
    }

    return EC_SUCCESS;
}


int GetString(double dVal, QString& strText)
{
    strText = GetString(dVal);
    return 0;
}

int GetString(int nVal, QString& strText)
{
    strText = GetString(nVal);
    return 0;
}

unsigned int GetUnsignedInt(QString& strText, int nBase)
{
    return strtoul(strText.toStdString().c_str(), nullptr, nBase);
}

unsigned int GetUnsignedInt(const QString& strText)
{
    unsigned int unVal = 0;
    bool bHex = LDFDatabaseManager::GetDatabaseManager()->bIsDisplayHexOn();
    if ( true == bHex )
    {
        unVal = strtoul(strText.toStdString().c_str(), nullptr, 16);
    }
    else
    {
        unVal = strtoul(strText.toStdString().c_str(), nullptr, 10);
    }
    return unVal;
}

QString GetString(unsigned __int64 nVal, bool bPrintSymbol)
{
    QString strText = "";
    bool bHex = LDFDatabaseManager::GetDatabaseManager()->bIsDisplayHexOn();
    if ( false == bHex )
    {
        strText.sprintf("%llu", nVal);
    }
    else
    {
        if ( true == bPrintSymbol )
        {
            strText.sprintf("0x%llX", nVal);
        }
        else
        {
            strText.sprintf("%llX", nVal);
        }
    }
    return strText;
}

QString GetString(int nVal, bool bPrintHex)
{
    QString strText = "";
    bool bHex = LDFDatabaseManager::GetDatabaseManager()->bIsDisplayHexOn();
    if ( false == bHex )
    {
        strText.sprintf("%d", nVal);
    }
    else
    {
        if ( true == bPrintHex )
        {
            strText.sprintf("0x%X", nVal);
        }
        else
        {
            strText.sprintf("%X", nVal);
        }
    }
    return strText;
}

QString GetString(unsigned int nVal, bool bPrintHex)
{
    QString strText = "";
    bool bHex = LDFDatabaseManager::GetDatabaseManager()->bIsDisplayHexOn();
    if(bHex == false)
    {
        strText.sprintf("%d", nVal);
    }
    else
    {
        if ( true == bPrintHex )
        {
            strText.sprintf("0x%X", nVal);
        }
        else
        {
            strText.sprintf("%X", nVal);
        }
    }
    return strText;
}

QString GetString(double dVal)
{
    QString strText = "";
    strText.sprintf("%lf", dVal);
    return strText;
}

ERRORCODE nGetSubscribers(std::list<IEcu*> ouECUlst, std::string& strRxEcus)
{
    strRxEcus = "";
    std::string strNodeName;
    std::list<IEcu*>::iterator itrECU;
    std::map<UID_ELEMENT, UID_ELEMENT> mapElements;
    UID_ELEMENT uidEcu;
    // Generate Subscribers
    if(ouECUlst.size() > 0)
    {
        itrECU = ouECUlst.begin();

        while(ouECUlst.end() != itrECU)
        {
            IEcu* objEcu = (IEcu*)*itrECU;

            if(nullptr != objEcu)
            {
                uidEcu = objEcu->GetUniqueId();
                objEcu->GetName(strNodeName);
            }
            if(mapElements.find(uidEcu) != mapElements.end())
            {
                itrECU++;
                continue;
            }

            if(mapElements.size() <= 0)
            {
                strRxEcus += strNodeName.c_str();
            }
            else
            {
                strRxEcus = strRxEcus + defCOMMA + strNodeName;
            }

            itrECU++;

            mapElements[objEcu->GetUniqueId()] = objEcu->GetUniqueId();
        }
    }
    else
    {
        return EC_FAILURE;
    }

    return EC_SUCCESS;
}

void DeleteChildren(QTreeWidgetItem* pouTreeItem)
{
    int nCount = pouTreeItem->childCount();
    for ( int i = 0 ; i < nCount; i++ )
    {
        pouTreeItem->takeChildren();
    }

}
QString GetString(int nVal, int nBase)
{
    QString strText = "";
    bool bHex = LDFDatabaseManager::GetDatabaseManager()->bIsDisplayHexOn();
    if ( 10 == nBase )
    {
        strText.sprintf("%d", nVal);
    }
    else if ( 16 == nBase )
    {
        strText.sprintf("0x%X", nVal);
    }
    return strText;
}

QString GetString(unsigned int nVal, int nBase)
{
    QString strText = "";
    bool bHex = LDFDatabaseManager::GetDatabaseManager()->bIsDisplayHexOn();
    if ( 10 == nBase )
    {
        strText.sprintf("%d", nVal);
    }
    else if ( 16 == nBase )
    {
        strText.sprintf("0x%X", nVal);
    }
    return strText;
}

QString GetString(unsigned __int64 nVal, int nBase)
{
    QString strText = "";
    bool bHex = LDFDatabaseManager::GetDatabaseManager()->bIsDisplayHexOn();
    if ( 10 == nBase )
    {
        strText.sprintf("%llu", nVal);
    }
    else if ( 16 == nBase )
    {
        strText.sprintf("0x%llu", nVal);
    }
    return strText;
}

unsigned char ucCalculatePID(unsigned char ucId)
{
    char chChecksum = ucId;
    char chMask[8], byte[8];
    for(int nIndex = 0; nIndex < 8; nIndex++)
    {
        chMask[nIndex] = nIndex+1;
        byte[nIndex] = ((chChecksum) >> nIndex) & 1;
    }

    char chP0, chP1;
    // chP0 = ID0+ID1+ID2+ID4
    chP0 = byte[0] ^ byte[1] ^ byte[2] ^ byte[4];


    // chP1 = ID1+ID3+ID4+ID5
    chP1 = byte[1] + byte[3] + byte[4] + byte[5];

    chP1 = ~chP1;

    //00010110 -> p00010110
    chP0 = chP0 << 6;
    chP1 = chP1 << 7;
    chChecksum = chChecksum + chP0 + chP1;
    ucId = chChecksum;
    return ucId;
}

QLDFElementDialog::QLDFElementDialog(QWidget* pParent):QDialog(pParent)
{

}

void QLDFElementDialog::accept()
{

}

bool bAreEqual(double a, double b)
{
    return fabs(a - b) < std::numeric_limits<double>::epsilon();
}