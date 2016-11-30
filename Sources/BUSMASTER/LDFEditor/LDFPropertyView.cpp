#include "LDFPropertyView.h"
#include "LDFDatabaseManager.h"
#include "qheaderview.h"
#include "LDFUtility.h"
#include "Defines.h"
#include "ldfadditionalview.h"
#define def_MASTER_ROW_COUNT        3

LDFPropertyView::LDFPropertyView(QWidget* parent)
    : LDFCommonTableWidget(parent)
{
    m_pouLDFDatabaseManager = LDFDatabaseManager::GetDatabaseManager();
    m_pouLdfCluster = m_pouLDFDatabaseManager->GetLDFCluster();

    /*QHeaderView* pHeader = horizontalHeader();
    if(nullptr != pHeader)
    {
        pHeader->setSectionResizeMode(QHeaderView::Stretch);
    }*/

    m_pouCurrentTable = nullptr;
    m_pouLDFAdditionalView = nullptr;

}

LDFPropertyView::~LDFPropertyView()
{

}

int LDFPropertyView::SetAdditionalPropsView(LDFAdditionalView* pouLDFAdditionalView)
{
    m_pouLDFAdditionalView = pouLDFAdditionalView;
    return 0;
}


void LDFPropertyView::onTreeItemClicked(QTreeWidgetItem* pouTreeItem, int nId)
{

    TreeItemID uidElement = pouTreeItem->data(0,  Qt::UserRole).value<TreeItemID>();
    if ( uidElement.m_eTreeElementType == eLdfItem && uidElement.m_uidElement != 0 )
    {
        ICluster* pouCluster = m_pouLDFDatabaseManager->GetLDFCluster();
        IElement* pouElement;

        VALIDATE_POINTER(pouCluster);
        pouCluster->GetElement(pouCluster->GetElementType(uidElement.m_uidElement), uidElement.m_uidElement, &pouElement);

        VALIDATE_POINTER(pouElement);
        vDisplayElement(pouElement);
    }
    else if ( uidElement.m_eTreeElementType == eTreeHeader )
    {
        switch(uidElement.m_uidElement)
        {
            case defTREE_ITEM_LINCLUSTER:
            {
                vDisplayClusterProps();
            }
            break;
            default:
                setRowCount(0);
                setColumnCount(0);
                break;
        }
    }

}

void LDFPropertyView::vDisplayClusterProps()
{
    VALIDATE_POINTER(m_pouLdfCluster);
    LIN_Settings ouLinSettings;
    m_pouLdfCluster->GetProperties(eLINClusterProperties, &ouLinSettings);
    QList<QString> strColumns;
    strColumns <<"Property Name" << "Name";
    CreateColumns(strColumns);
    setRowCount(3);
    int nRow = 0;
    QList<QVariant> ouRowList;
    ouRowList.push_back(QVariant("Baud Rate"));
    ouRowList.push_back(QVariant(ouLinSettings.m_dBaudRate));
    InsertRow(nRow++, ouRowList);

    ouRowList.clear();
    ouRowList.push_back(QVariant("LIN Protocol Version"));
    ouRowList.push_back(QVariant(ouLinSettings.m_dProtocolVers));
    InsertRow(nRow++, ouRowList);

    ouRowList.clear();
    ouRowList.push_back(QVariant("LDF Language Version"));
    ouRowList.push_back(QVariant(ouLinSettings.m_dLanguageVers));
    InsertRow(nRow++, ouRowList);

    if (bAreEqual( ouLinSettings.m_dProtocolVers ,defVersion_2_1) == true )
    {
        setRowCount(4);
        ouRowList.clear();
        ouRowList.push_back(QVariant("Channel Name"));
        ouRowList.push_back(QVariant(ouLinSettings.m_strChannelName.c_str()));
        InsertRow(nRow++, ouRowList);
    }
}

void LDFPropertyView::vDisplayElement ( IElement* pouLdfElement)
{
    if ( nullptr == pouLdfElement )
    {
        return ;
    }

    eClusterElementType eElementType = pouLdfElement->GetElementType();

    disconnect(this, SLOT( OnitemSelectionChanged()));
    m_pouCurrentTable = nullptr;

    switch (eElementType)
    {
        case eFrameElement:
            DisplayFrameElementProps(pouLdfElement);
            break;
        case eSignalElement:
            DisplaySignalElementProps(pouLdfElement);
            break;
        case eEcuElement:
            DisplayEcuElementProps(pouLdfElement);
            break;
        case eCodingElement:
            DisplayCodingElementProps(pouLdfElement);
            break;
        case eScheduleTableElement:
            DisplayScheduleTableElementProps(pouLdfElement);
            break;
        case eSignalGroupElement:
            DisplaySignalGroupDetails(pouLdfElement);
            break;
        case eLinNodeComposition:
            DisplayNodeCompositionDetails(pouLdfElement);
            break;
        default:
            break;
    }

}

void LDFPropertyView::OnitemSelectionChanged()
{
    QModelIndexList indexList = selectionModel()->selectedIndexes();
    int row = -1;

for (auto index : indexList)
    {
        row = index.row();
        if ( m_pouLDFAdditionalView != nullptr && m_pouCurrentTable != nullptr )
        {
            m_pouLDFAdditionalView->DisplayScedulingTableAdditionalProps(m_pouCurrentTable, row);
        }
        break;
    }
}

int LDFPropertyView::DisplayNodeCompositionDetails(IElement* pouLdfElement)
{
    INodeConfiguration* pouNodeConfig = (INodeConfiguration*)pouLdfElement;
    std::list<CompositeNode> lstCompNodes;
    QList<QString> strColumns;
    strColumns <<"Composite Node" << "Logical Nodes";
    CreateColumns(strColumns);

    std::string strName = "", strCompName = "";

    pouNodeConfig->GetCompositeNodes(lstCompNodes);
    pouNodeConfig->GetName(strName);

    setRowCount(lstCompNodes.size());
    int nRow = 0;
    QList<QVariant> ouRowList;
    IElement* pECU = nullptr;
for(auto itrNodeConfig : lstCompNodes)
    {
        ouRowList.clear();
        if(EC_SUCCESS == m_pouLdfCluster->GetElement(eEcuElement, itrNodeConfig.m_uidNode, &pECU))
        {
            pECU->GetName(strCompName);
            ouRowList.push_back(strCompName.c_str());
            std::string strLogicalNodes;
            int nCount = 0;
for(auto itrLogicalNode : itrNodeConfig.m_strLogicalNodes)
            {
                if(0 == nCount)
                {
                    strLogicalNodes = itrLogicalNode;
                }
                else
                {
                    strLogicalNodes += defCOMMA + itrLogicalNode;
                }
                nCount++;
            }
            ouRowList.push_back(strLogicalNodes.c_str());
        }

        InsertRow(nRow++, ouRowList);
    }
    return 0;
}

int LDFPropertyView::DisplaySignalGroupDetails(IElement* pouLdfElement)
{
    ISignalGroup* pouSignalGroup = (ISignalGroup*)pouLdfElement;
    QList<QString> strColumns;
    strColumns <<"Property" << "Value";
    CreateColumns(strColumns);

    std::string strName = "";
    LinSignalGroupProps ouSignalGroupProps;

    pouSignalGroup->GetProperties(ouSignalGroupProps);
    pouSignalGroup->GetName(strName);

    setRowCount(2);

    QList<QVariant> ouRowList;

    int nRow = 0;
    //1. Name
    ouRowList.push_back("Name");
    ouRowList.push_back(QString::fromStdString(strName));
    InsertRow(nRow++, ouRowList);
    ouRowList.clear();


    //2. Group Size
    ouRowList.push_back("Group Size");
    ouRowList.push_back(GetString(ouSignalGroupProps.m_nGroupSize, 10));
    InsertRow(nRow++, ouRowList);
    ouRowList.clear();

    return 0;
}


int LDFPropertyView::DisplayScheduleTableElementProps(IElement* pouLdfElement)
{
    connect(this, SIGNAL(itemSelectionChanged()),this, SLOT( OnitemSelectionChanged()));

    IScheduleTable* pouTable = (IScheduleTable*)pouLdfElement;

    VALIDATE_AND_RETURN(pouTable);
    m_pouCurrentTable = pouTable;
    ScheduleTableProps ouScheduleTableProps;
    pouTable->GetProperties(ouScheduleTableProps);

    QList<QString> strColumns;
    strColumns <<"Frame Name" << "Frame Type"<<"Delay [ms]";
    CreateColumns(strColumns);
    IFrame* pouMsg;
    QList<QVariant> ouColumnList;
    std::string strTemp;
    int nRow = 0;
    setRowCount(ouScheduleTableProps.m_ouCLINSheduleTableItem.size());
    LinFrameProps ouFrameProps;
	ouFrameProps.m_eLinFrameType = eLinInvalidFrame;
for ( auto itr : ouScheduleTableProps.m_ouCLINSheduleTableItem )
    {
        m_pouLdfCluster->GetElement(eFrameElement, itr.m_nFrameId, (IElement**)&pouMsg);

        if(nullptr != pouMsg)
        {
            pouMsg->GetName(strTemp);
            pouMsg->GetProperties(ouFrameProps);
        }
        std::string strName = strTemp.c_str();


        int hr = 0;
		if (ouFrameProps.m_eLinFrameType == eLinDiagnosticFrame)
        {
            hr = GetString(itr.m_eDiagType, strTemp);
        }
        else
        {
			hr = GetString(ouFrameProps.m_eLinFrameType, strTemp);
        }

        if ( 0 == hr )
        {
            if(itr.m_eDiagType == eLIN_NORMAL_FRAME_ID || itr.m_eDiagType == eLIN_MASTER_FRAME_ID
                    || itr.m_eDiagType == eLIN_SLAVE_FRAME_ID)
            {
                ouColumnList.push_back(QString::fromStdString(strName.c_str()));
                ouColumnList.push_back(QString::fromStdString(strTemp.c_str()));
                ouColumnList.push_back(itr.m_dDelay);
            }
            else
            {
                ouColumnList.push_back(QString::fromStdString(strTemp.c_str()));
                ouColumnList.push_back(QString::fromStdString(strName.c_str()));
                ouColumnList.push_back(itr.m_dDelay);
            }
            InsertRow(nRow, ouColumnList);
            nRow++;
        }
        ouColumnList.clear();
    }
    return 0;
}


int LDFPropertyView::DisplayCodingElementProps(IElement* pouLdfElement)
{


    ICoding* ouCoding = (ICoding*)pouLdfElement;

    VALIDATE_AND_RETURN(ouCoding);

    QList<QString> strColumns;
    strColumns <<"Encoding Type" << "Min"<<"Max" << "Factor" << "Offset" <<"Unit" <<"Value" <<"Description";
    CreateColumns(strColumns);

    if ( nullptr != ouCoding )
    {
        LINCompuMethods ouCompuMethods;
        ouCompuMethods.m_eType = eInvalidProtocol;
        ouCoding->GetProperties(ouCompuMethods);

        QList<QVariant> ouColumnList;
        int nRow = 0;
        setRowCount(ouCompuMethods.m_ouPhysicalValueList.size()+ouCompuMethods.m_ouLogicalValueList.size());

for ( auto itr : ouCompuMethods.m_ouPhysicalValueList )
        {
            ouColumnList.clear();
            ouColumnList.push_back(QVariant("Physical"));
            ouColumnList.push_back(QVariant(itr.m_unMin));
            ouColumnList.push_back(QVariant(itr.m_unMax));
            ouColumnList.push_back(QVariant(itr.m_dFactor));
            ouColumnList.push_back(QVariant(itr.m_dOffset));
            ouColumnList.push_back(QVariant(itr.m_strTextInfo.c_str()));
            ouColumnList.push_back(QVariant("-"));
            ouColumnList.push_back(QVariant("-"));
            InsertRow(nRow++, ouColumnList);
        }

for ( auto itr : ouCompuMethods.m_ouLogicalValueList)
        {
            ouColumnList.clear();
            ouColumnList.push_back(QVariant("Logical"));
            ouColumnList.push_back(QVariant("-"));
            ouColumnList.push_back(QVariant("-"));
            ouColumnList.push_back(QVariant("-"));
            ouColumnList.push_back(QVariant("-"));
            ouColumnList.push_back(QVariant("-"));
            ouColumnList.push_back(QVariant(itr.first));
            ouColumnList.push_back(QVariant(QString::fromStdString(itr.second)));
            InsertRow(nRow++, ouColumnList);
        }
    }
    return 0;

}
int LDFPropertyView::DisplaySignalElementProps(IElement* pouLdfElement)
{
    QList<QString> strColumns;
    strColumns <<"Property" << "Value";
    CreateColumns(strColumns);

    ISignal* poSignal = (ISignal*)pouLdfElement;

    VALIDATE_AND_RETURN(poSignal);

    LINSignalProps ouSignalProps;
    ouSignalProps.eType = eInvalidProtocol;
    poSignal->GetProperties(ouSignalProps);
    std::string strSignalName, strPublishers, strSubscribers;
    poSignal->GetName(strSignalName);

    strPublishers = "";
    std::list<IEcu*> lstTxEcus;
    poSignal->GetEcus(eTx, lstTxEcus);
    nGetSubscribers(lstTxEcus, strPublishers);

    strSubscribers = "";
    std::list<IEcu*> lstRxEcus;
    poSignal->GetEcus(eRx, lstRxEcus);
    nGetSubscribers(lstRxEcus, strSubscribers);

    QList<QVariant> ouRowList;
    int nRow = 0;
    setRowCount(9);

    ouRowList.push_back(QVariant("Name"));
    ouRowList.push_back(QVariant(QString::fromStdString(strSignalName)));
    InsertRow(nRow++, ouRowList);
    ouRowList.clear();

    ouRowList.push_back(QVariant("Length"));
    ouRowList.push_back(QVariant(ouSignalProps.m_unSignalSize));
    InsertRow(nRow++, ouRowList);
    ouRowList.clear();

    QString strTemp = (ouSignalProps.m_ouSignalType == eSignalNormal ) ? "Normal" : "Diagnostic";
    ouRowList.push_back(QVariant("Signal Type"));
    ouRowList.push_back(QVariant( strTemp ));
    InsertRow(nRow++, ouRowList);
    ouRowList.clear();

    ouRowList.push_back(QVariant("Initial Value"));
    ouRowList.push_back(QVariant(ouSignalProps.m_nIntialValue));
    InsertRow(nRow++, ouRowList);
    ouRowList.clear();

    strTemp = (ouSignalProps.m_ouDataType == eSigned ) ? "Signed" : "Unsigned";
    ouRowList.push_back(QVariant("Data Type"));
    ouRowList.push_back(QVariant( strTemp ));
    InsertRow(nRow++, ouRowList);
    ouRowList.clear();

    strTemp = (ouSignalProps.m_ouEndianess == eMotorola ) ? "Motorola" : "Intel";
    ouRowList.push_back(QVariant("Endianess"));
    ouRowList.push_back(QVariant( strTemp ));
    InsertRow(nRow++, ouRowList);
    ouRowList.clear();

    strTemp = (ouSignalProps.m_ouValueType == eScalar ) ? "Scalar" : "Byte Array";
    ouRowList.push_back(QVariant("Value Type"));
    ouRowList.push_back(QVariant( strTemp ));


    std::list<IEcu*> ouEcuList;
    poSignal->GetEcus(eTx, ouEcuList);
    std::list<IEcu*>::iterator itr = ouEcuList.begin();
    std::string strValue = "";

    if ( ouEcuList.end() != itr )
    {
        IEcu* pouIecu = (IEcu*)*itr;

        if(nullptr != pouIecu)
        {
            pouIecu->GetName(strValue);
        }

    }
    ouRowList.push_back(QString::fromStdString(strValue));
    InsertRow(nRow++, ouRowList);
    ouRowList.clear();

    ouRowList.push_back(QVariant("Publisher"));
    ouRowList.push_back(QVariant( QString::fromStdString(strPublishers) ));
    InsertRow(nRow++, ouRowList);
    ouRowList.clear();

    ouRowList.push_back(QVariant("Subscribers"));
    ouRowList.push_back(QVariant( QString::fromStdString(strSubscribers) ));
    InsertRow(nRow++, ouRowList);
    ouRowList.clear();

    return 0;

}

int LDFPropertyView::DisplayEcuElementProps(IElement* pouLdfElement)
{


    IEcu* pEcu = (IEcu*)pouLdfElement;

    VALIDATE_AND_RETURN(pEcu);

    LinEcuProps ouEcuProps;
    ouEcuProps.m_eEcuType = eEcuNone;
    pEcu->GetProperties(ouEcuProps);

    if ( eMaster == ouEcuProps.m_eEcuType )
    {
        DisplayMasterDetails((IEcu*)pouLdfElement);

    }
    else if ( eSlave == ouEcuProps.m_eEcuType )
    {
        DisplaySlaveDetails((IEcu*)pouLdfElement);
    }
    else
    {
    }
    return 0;
}
int LDFPropertyView::DisplaySlaveDetails(IEcu* pouLdfElement)
{
    clearContents();

    QList<QString> strColumns;
    strColumns <<"Property" << "Value";
    CreateColumns(strColumns);

    LinEcuProps ouEcuProps;
    ouEcuProps.m_eEcuType = eEcuNone;
    pouLdfElement->GetProperties(ouEcuProps);

    LIN_Settings ouLIN_Settings;
    m_pouLdfCluster->GetProperties(eLINClusterProperties, &ouLIN_Settings);

    std::string strName;
    QString strTemp;

    if(ouEcuProps.mSlaveProps.m_fProtocolVersion == defVersion_1_3)
    {
        setRowCount(3);
    }
	else if (ouEcuProps.mSlaveProps.m_fProtocolVersion == defVersion_2_0)
    {
        setRowCount(9);
    }
	else if (ouEcuProps.mSlaveProps.m_fProtocolVersion == defVersion_2_1)
    {
        setRowCount(13);
    }

    int nRow = 0;

    pouLdfElement->GetName(strName);
    QList<QVariant> ouRowList;
    std::string strFaultSignals = "";

    //1.Name
    ouRowList.push_back("Name");
    ouRowList.push_back(QString::fromStdString(strName));
    InsertRow(nRow++, ouRowList);
    ouRowList.clear();

    //2.Protocol Version
    ouRowList.push_back("Protocol Version");
	ouRowList.push_back(ouEcuProps.mSlaveProps.m_fProtocolVersion);
    InsertRow(nRow++, ouRowList);
    ouRowList.clear();

	if (ouEcuProps.mSlaveProps.m_fProtocolVersion == defVersion_1_3)
    {
        //7.Configured NAD
        ouRowList.push_back("Configured NAD");
		ouRowList.push_back(ouEcuProps.mSlaveProps.m_nConfiguredNAD);
        InsertRow(nRow++, ouRowList);
        ouRowList.clear();
    }
	else if (ouEcuProps.mSlaveProps.m_fProtocolVersion == defVersion_2_1 || ouEcuProps.mSlaveProps.m_fProtocolVersion == defVersion_2_0)
    {
		if (ouEcuProps.mSlaveProps.m_fProtocolVersion == defVersion_2_1)
        {
            //3.NAs
            ouRowList.push_back("NAs Time out");
			ouRowList.push_back(ouEcuProps.mSlaveProps.m_dNASTimeout);
            InsertRow(nRow++, ouRowList);
            ouRowList.clear();

            //4.NCr
            ouRowList.push_back("NCr Time out");
			ouRowList.push_back(ouEcuProps.mSlaveProps.m_dNCRTimeout);
            InsertRow(nRow++, ouRowList);
            ouRowList.clear();
        }
        //5.P2Min
        ouRowList.push_back("P2Min");
		ouRowList.push_back(ouEcuProps.mSlaveProps.m_dP2Min);
        InsertRow(nRow++, ouRowList);
        ouRowList.clear();

        //6.STMin
        ouRowList.push_back("STMin");
		ouRowList.push_back(ouEcuProps.mSlaveProps.m_dSTMin);
        InsertRow(nRow++, ouRowList);
        ouRowList.clear();

        //7.Configured NAD
        ouRowList.push_back("Configured NAD");
		ouRowList.push_back(ouEcuProps.mSlaveProps.m_nConfiguredNAD);
        InsertRow(nRow++, ouRowList);
        ouRowList.clear();

		if (ouEcuProps.mSlaveProps.m_fProtocolVersion == defVersion_2_1)
        {
            //8.Initial NAD
            ouRowList.push_back("Initial NAD");
			ouRowList.push_back(ouEcuProps.mSlaveProps.m_nInitialNAD);
            InsertRow(nRow++, ouRowList);
            ouRowList.clear();
        }

        //9.Function id
        ouRowList.push_back("Function ID");
		ouRowList.push_back(ouEcuProps.mSlaveProps.m_nFunctionId);
        InsertRow(nRow++, ouRowList);
        ouRowList.clear();


        //10.Supplier id
        ouRowList.push_back("Supplier ID");
		ouRowList.push_back(ouEcuProps.mSlaveProps.m_nSupplierId);
        InsertRow(nRow++, ouRowList);
        ouRowList.clear();

        //11.Variant id
        ouRowList.push_back("Variant");
		ouRowList.push_back(ouEcuProps.mSlaveProps.m_nVariant);
        InsertRow(nRow++, ouRowList);
        ouRowList.clear();

		if (ouEcuProps.mSlaveProps.m_fProtocolVersion == defVersion_2_1)
        {
            //12.Fault State Signals
            ouRowList.push_back("Fault State Signals");
			for (auto itr : ouEcuProps.mSlaveProps.m_nFaultStateSignals)
            {
                std::string strName;
                IElement* pSignal;
                m_pouLdfCluster->GetElement(eSignalElement, itr, &pSignal);

                if(nullptr != pSignal)
                {
                    pSignal->GetName(strName);

                    if(strFaultSignals.empty() == true)
                    {
                        strFaultSignals = strName;
                    }
                    else
                    {
                        strFaultSignals = strFaultSignals + defCOMMA + strName;
                    }
                }
            }
            ouRowList.push_back(QString::fromStdString(strFaultSignals));
            InsertRow(nRow++, ouRowList);
            ouRowList.clear();
        }

        //13.Configured Frame List
        /*ouRowList.push_back("Configured Frame List");
        std::string strConfigFrameList;
        QString strToolTip = "";
        for(auto itr : ouEcuProps.m_ouSlavePros.m_nConfiurableFrameIdList)
        {
            std::string strName;
            IElement *pFrame;
            m_pouLdfCluster->GetElement(eFrameElement, itr.m_uidFrame, &pFrame);
            if ( nullptr != pFrame )
            {
                pFrame->GetName(strName);
                strTemp.sprintf("%d. %s \n", itr.m_unIndex, strName.c_str());
                strToolTip +=  strTemp;
                if(strConfigFrameList.empty() == true)
                {
                    strConfigFrameList = strName;
                }
                else
                strConfigFrameList = strConfigFrameList + defCOMMA + strName;
            }

        }
        ouRowList.push_back(QString::fromStdString(strConfigFrameList));
        InsertRow(nRow++, ouRowList, strToolTip);
        ouRowList.clear();*/

        //13.Response Error Signals
        ouRowList.push_back("Response Error Signals");
        std::string strRespSig="";
		if (ouEcuProps.mSlaveProps.m_nRespErrSignal != INVALID_UID_ELEMENT)
        {
            IElement* pSignal;
			m_pouLdfCluster->GetElement(eSignalElement, ouEcuProps.mSlaveProps.m_nRespErrSignal, &pSignal);
            pSignal->GetName(strRespSig);
        }
        ouRowList.push_back(QString::fromStdString(strRespSig));
        InsertRow(nRow++, ouRowList);
        ouRowList.clear();
    }


    return 0;
}


int LDFPropertyView::DisplayMasterDetails(IEcu* pouLdfElement)
{
    QList<QString> strColumns;
    strColumns <<"Property" << "Value";
    CreateColumns(strColumns);

    std::string strName = "";
    LinEcuProps ouEcuProps;
    ouEcuProps.m_eEcuType = eEcuNone;
    pouLdfElement->GetProperties(ouEcuProps);
    pouLdfElement->GetName(strName);

    setRowCount(def_MASTER_ROW_COUNT);

    QList<QVariant> ouRowList;

    int nRow = 0;
    //1. Name
    ouRowList.push_back("Name");
    ouRowList.push_back(QString::fromStdString(strName));
    InsertRow(nRow++, ouRowList);
    ouRowList.clear();


    //2. Jitter
    ouRowList.push_back("Jitter");
    ouRowList.push_back(ouEcuProps.mMasterProps.m_fJitter);
    InsertRow(nRow++, ouRowList);
    ouRowList.clear();


    //3. Time Base
    ouRowList.push_back("Time Base");
	ouRowList.push_back(ouEcuProps.mMasterProps.m_fTimeBase);
    InsertRow(nRow++, ouRowList);
    ouRowList.clear();

    return 0;
}

int LDFPropertyView::DisplayFrameElementProps(IElement* pouLdfElement)
{
    IFrame* pFrame = (IFrame*)pouLdfElement;

    VALIDATE_AND_RETURN(pFrame);

    LinFrameProps ouFrameProps;
    ouFrameProps.m_eLinFrameType = eLinInvalidFrame;
    pFrame->GetProperties(ouFrameProps);

    QList<QString> strColumns;
    strColumns <<"Property Name" << "Value";
    CreateColumns(strColumns);

	if (eLinUnconditionalFrame == ouFrameProps.m_eLinFrameType)
    {
        DisplayUnConditionalFrameDetails((IFrame*)pouLdfElement);

    }
	else if (eLinEventTriggeredFrame == ouFrameProps.m_eLinFrameType)
    {
        DisplayEventTriggeredFrameDetails((IFrame*)pouLdfElement);
    }
	else if (eLinSporadicFrame == ouFrameProps.m_eLinFrameType)
    {
        DisplaySporadicFrameDetails((IFrame*)pouLdfElement);
    }
	else if (eLinDiagnosticFrame == ouFrameProps.m_eLinFrameType)
    {
        DisplayDiagnosticFrameDetails((IFrame*)pouLdfElement);
    }
    return 0;
}

int LDFPropertyView::DisplayEventTriggeredFrameDetails(IFrame* pouFrame)
{
    VALIDATE_AND_RETURN(pouFrame);
    clearContents();
    LinFrameProps ouFrameProps;
	ouFrameProps.m_eLinFrameType = eLinInvalidFrame;
    pouFrame->GetProperties(ouFrameProps);
    LIN_Settings ouLIN_Settings;
    m_pouLdfCluster->GetProperties(eLINClusterProperties, &ouLIN_Settings);

    setRowCount(5);

    int nRow = 0;
    QList<QVariant> ouValueList;
    std::list<IEcu*> lstPublishers, lstSuscribers;
    std::string strName,strChecksum, strAssocFrames, strCollisionTable;
    unsigned int unId;

    nGetChecksumType(ouLIN_Settings, strChecksum);
    pouFrame->GetName(strName);

    std::list<IFrame*> lstFrames;
for ( auto itr : ouFrameProps.m_ouLINEventTrigFrameProps.m_pouUnconditionalFrame)
    {
        IFrame* pouFrame = nullptr;
        m_pouLdfCluster->GetElement(eFrameElement, UID_ELEMENT(itr.first), (IElement**)&pouFrame);
        if ( nullptr != pouFrame )
        {
            lstFrames.push_back(pouFrame);
        }
    }

    GetUnconditionalFrames(lstFrames, strAssocFrames);

    ouValueList.push_back(QVariant("Name"));
    ouValueList.push_back(QVariant(QString::fromStdString(strName)));
    InsertRow(nRow++, ouValueList);

    ouValueList.clear();
    ouValueList.push_back(QVariant("Associated Frames"));
    ouValueList.push_back(QVariant(QString::fromStdString(strAssocFrames)));
    InsertRow(nRow++, ouValueList);

    IElement* pScheduleTable = nullptr;
    m_pouLdfCluster->GetElement(eScheduleTableElement, ouFrameProps.m_ouLINEventTrigFrameProps.m_nCollisionResolveTable,&pScheduleTable);

    if(nullptr != pScheduleTable)
    {
        pScheduleTable->GetName(strCollisionTable);
    }

    ouValueList.clear();
    ouValueList.push_back(QVariant("Collision Resolution Table"));
    ouValueList.push_back(QVariant(QString::fromStdString(strCollisionTable)));
    InsertRow(nRow++, ouValueList);

    ouValueList.clear();
    ouValueList.push_back(QVariant("Checksum Model"));
    ouValueList.push_back(QVariant(QString::fromStdString(strChecksum)));
    InsertRow(nRow++, ouValueList);

    ouValueList.clear();
    ouValueList.push_back(QVariant("Frame ID"));
    ouValueList.push_back(ouFrameProps.m_nMsgId);
    InsertRow(nRow++, ouValueList);
    return 0;
}

int LDFPropertyView::DisplayDiagnosticFrameDetails(IFrame* pouFrame)
{
    VALIDATE_AND_RETURN(pouFrame);
    clearContents();
    LinFrameProps ouFrameProps;
	ouFrameProps.m_eLinFrameType = eLinInvalidFrame;
    pouFrame->GetProperties(ouFrameProps);
    LIN_Settings ouLIN_Settings;
    m_pouLdfCluster->GetProperties(eLINClusterProperties, &ouLIN_Settings);

    setRowCount(6);

    int nRow = 0;
    QList<QVariant> ouValueList;
    std::list<IEcu*> lstPublishers, lstSuscribers;
    std::string strName,strChecksum, strPublisher, strSubscriber;
    unsigned int unId;

    strPublisher = "";
    pouFrame->GetEcus(eTx, lstPublishers);
    lstPublishers.unique();
    nGetSubscribers(lstPublishers, strPublisher);

    pouFrame->GetEcus(eRx, lstSuscribers);
    nGetChecksumType(ouLIN_Settings, strChecksum);

    strSubscriber = "";
    lstSuscribers.unique();
    nGetSubscribers(lstSuscribers, strSubscriber);
    pouFrame->GetName(strName);

    nGetChecksumType(ouLIN_Settings, strChecksum);
    pouFrame->GetName(strName);

    ouValueList.push_back(QVariant("Name"));
    ouValueList.push_back(QVariant(QString::fromStdString(strName)));
    InsertRow(nRow++, ouValueList);

    ouValueList.clear();
    ouValueList.push_back(QVariant("Checksum Model"));
    ouValueList.push_back(QVariant(defCLASSIC_CHECKSUM));
    InsertRow(nRow++, ouValueList);

    ouValueList.clear();
    ouValueList.push_back(QVariant("Frame ID"));
    ouValueList.push_back(ouFrameProps.m_nMsgId);
    InsertRow(nRow++, ouValueList);

    ouValueList.clear();
    ouValueList.push_back(QVariant("Publisher"));
    ouValueList.push_back(QVariant(QString::fromStdString(strPublisher)));
    InsertRow(nRow++, ouValueList);

    ouValueList.clear();
    ouValueList.push_back(QVariant("Size (Byte(s))"));
    ouValueList.push_back(QVariant(GetString(ouFrameProps.m_unMsgSize, 10)));
    InsertRow(nRow++, ouValueList);

    ouValueList.clear();
    ouValueList.push_back(QVariant("Subscribers"));
    ouValueList.push_back(QVariant(QString::fromStdString(strSubscriber)));
    InsertRow(nRow++, ouValueList);

    return 0;
}

int LDFPropertyView::DisplaySporadicFrameDetails(IFrame* pouFrame)
{
    VALIDATE_AND_RETURN(pouFrame);
    clearContents();
    LinFrameProps ouFrameProps;
	ouFrameProps.m_eLinFrameType = eLinInvalidFrame;
    pouFrame->GetProperties(ouFrameProps);
    LIN_Settings ouLIN_Settings;
    m_pouLdfCluster->GetProperties(eLINClusterProperties, &ouLIN_Settings);

    setRowCount(3);

    int nRow = 0;
    QList<QVariant> ouValueList;
    std::list<IEcu*> lstPublishers, lstSuscribers;
    std::string strName,strChecksum, strAssocFrames, strCollisionTable;
    unsigned int unId;

    nGetChecksumType(ouLIN_Settings, strChecksum);
    pouFrame->GetName(strName);

    std::list<IFrame*> lstFrames;
for ( auto itr : ouFrameProps.m_ouLINSporadicFrameProps.m_pouUnconditionalFrame)
    {
        IFrame* pouFrame = nullptr;
        m_pouLdfCluster->GetElement(eFrameElement, UID_ELEMENT(itr.first), (IElement**)&pouFrame);
        if ( nullptr != pouFrame )
        {
            lstFrames.push_back(pouFrame);
        }
    }

    GetUnconditionalFrames(lstFrames, strAssocFrames);

    ouValueList.push_back(QVariant("Name"));
    ouValueList.push_back(QVariant(QString::fromStdString(strName)));
    InsertRow(nRow++, ouValueList);

    ouValueList.clear();
    ouValueList.push_back(QVariant("Associated Frames"));
    ouValueList.push_back(QVariant(QString::fromStdString(strAssocFrames)));
    InsertRow(nRow++, ouValueList);

    ouValueList.clear();
    ouValueList.push_back(QVariant("Checksum Model"));
    ouValueList.push_back(QVariant(QString::fromStdString(strChecksum)));
    InsertRow(nRow++, ouValueList);
    return 0;
}

int LDFPropertyView::GetUnconditionalFrames(std::list<IFrame*>& lstFrames, std::string& strFrames)
{
    unsigned int unFrameId, unCount = 0;
    std::string strFrameName;
for(auto itr : lstFrames)
    {
        itr->GetName(strFrameName);
        strFrames += strFrameName;
        unCount++;
        if(unCount < lstFrames.size())
        {
            strFrames += defCOMMA;
        }
    }

    return 0;
}


int LDFPropertyView::DisplayUnConditionalFrameDetails(IFrame* pouFrame)
{
    VALIDATE_AND_RETURN(pouFrame);
    clearContents();
    LinFrameProps ouFrameProps;
	ouFrameProps.m_eLinFrameType = eLinInvalidFrame;
    pouFrame->GetProperties(ouFrameProps);

    LIN_Settings ouLIN_Settings;
    m_pouLdfCluster->GetProperties(eLINClusterProperties, &ouLIN_Settings);

    setRowCount(6);

    int nRow = 0;
    QList<QVariant> ouValueList;
    std::list<IEcu*> lstPublishers, lstSuscribers;
    std::string strName,strChecksum, strPublisher, strSubscriber;
    unsigned int unId;

    pouFrame->GetEcus(eTx, lstPublishers);
    lstPublishers.unique();
    nGetSubscribers(lstPublishers, strPublisher);

    pouFrame->GetEcus(eRx, lstSuscribers);
    nGetChecksumType(ouLIN_Settings, strChecksum);

    lstSuscribers.unique();
    nGetSubscribers(lstSuscribers, strSubscriber);
    pouFrame->GetName(strName);

    ouValueList.push_back(QVariant("Name"));
    ouValueList.push_back(QVariant(QString::fromStdString(strName)));
    InsertRow(nRow++, ouValueList);

    ouValueList.clear();
    ouValueList.push_back(QVariant("Checksum Model"));
    ouValueList.push_back(QVariant(QString::fromStdString(strChecksum)));
    InsertRow(nRow++, ouValueList);

    ouValueList.clear();
    ouValueList.push_back(QVariant("Frame Id"));
    ouValueList.push_back(QVariant(ouFrameProps.m_nMsgId));
    InsertRow(nRow++, ouValueList);

    ouValueList.clear();
    ouValueList.push_back(QVariant("Publisher"));
    ouValueList.push_back(QVariant(QString::fromStdString(strPublisher)));
    InsertRow(nRow++, ouValueList);

    ouValueList.clear();
    ouValueList.push_back(QVariant("Length (Byte(s))"));
    ouValueList.push_back(QVariant(GetString(ouFrameProps.m_unMsgSize, 10)));        //Size always dec
    InsertRow(nRow++, ouValueList);

    ouValueList.clear();
    ouValueList.push_back(QVariant("Subscribers"));
    ouValueList.push_back(QString::fromStdString(strSubscriber));
    InsertRow(nRow++, ouValueList);
    return 0;
}