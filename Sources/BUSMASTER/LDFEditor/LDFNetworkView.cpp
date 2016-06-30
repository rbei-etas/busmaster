#include "LDFNetworkView.h"
#include "Defines.h"
#include "LDFUtility.h"
LDFNetworkView::LDFNetworkView(QWidget* parent)
    : QTreeWidget(parent)
{
    m_pouLDFDatabaseManager = LDFDatabaseManager::GetDatabaseManager();
    m_pouLdfCluster = m_pouLDFDatabaseManager->GetLDFCluster();
    connect(m_pouLDFDatabaseManager, SIGNAL(DataUpdated()), this, SLOT(onDataUpdate()));
    m_ouIcons[defFRAME_ICON_INDEX].addFile(":/LDFEditorIcons/Resources/icons/16x16/Frame.png", QSize(16, 16));
    m_ouIcons[defFRAME_ICON_INDEX].addFile(":/LDFEditorIcons/Resources/icons/22x22/Frame.png", QSize(22, 22));
    m_ouIcons[defFRAME_ICON_INDEX].addFile(":/LDFEditorIcons/Resources/icons/32x32/Frame.png", QSize(32, 32));

    m_ouIcons[defECU_ICON_INDEX].addFile(":/LDFEditorIcons/Resources/icons/16x16/Ecu.png", QSize(16, 16));
    m_ouIcons[defECU_ICON_INDEX].addFile(":/LDFEditorIcons/Resources/icons/22x22/Ecu.png", QSize(22, 22));
    m_ouIcons[defECU_ICON_INDEX].addFile(":/LDFEditorIcons/Resources/icons/32x32/Ecu.png", QSize(32, 32));

    m_ouIcons[defSIGNAL_ICON_INDEX].addFile(":/LDFEditorIcons/Resources/icons/16x16/Signals.png", QSize(16, 16));
    m_ouIcons[defSIGNAL_ICON_INDEX].addFile(":/LDFEditorIcons/Resources/icons/22x22/Signals.png", QSize(22, 22));
    m_ouIcons[defSIGNAL_ICON_INDEX].addFile(":/LDFEditorIcons/Resources/icons/32x32/Signals.png", QSize(32, 32));

    m_ouIcons[defCODING_ICON_INDEX].addFile(":/LDFEditorIcons/Resources/icons/16x16/Coding.png", QSize(16, 16));
    m_ouIcons[defCODING_ICON_INDEX].addFile(":/LDFEditorIcons/Resources/icons/22x22/Coding.png", QSize(22, 22));
    m_ouIcons[defCODING_ICON_INDEX].addFile(":/LDFEditorIcons/Resources/icons/32x32/Coding.png", QSize(32, 32));

    m_ouIcons[defSCHEDULETABLE_ICON_INDEX].addFile(":/LDFEditorIcons/Resources/icons/16x16/ScheduleTable.png", QSize(16, 16));
    m_ouIcons[defSCHEDULETABLE_ICON_INDEX].addFile(":/LDFEditorIcons/Resources/icons/22x22/ScheduleTable.png", QSize(22, 22));
    m_ouIcons[defSCHEDULETABLE_ICON_INDEX].addFile(":/LDFEditorIcons/Resources/icons/32x32/ScheduleTable.png", QSize(32, 32));

    m_ouIcons[defSIGNAL_RX_ICON_INDEX].addFile(":/LDFEditorIcons/Resources/icons/16x16/Signals.png", QSize(16, 16));
    m_ouIcons[defSIGNAL_RX_ICON_INDEX].addFile(":/LDFEditorIcons/Resources/icons/22x22/Signals.png", QSize(22, 22));
    m_ouIcons[defSIGNAL_RX_ICON_INDEX].addFile(":/LDFEditorIcons/Resources/icons/32x32/Signals.png", QSize(32, 32));

    m_ouIcons[defSIGNAL_TX_ICON_INDEX].addFile(":/LDFEditorIcons/Resources/icons/16x16/Signals.png", QSize(16, 16));
    m_ouIcons[defSIGNAL_TX_ICON_INDEX].addFile(":/LDFEditorIcons/Resources/icons/22x22/Signals.png", QSize(22, 22));
    m_ouIcons[defSIGNAL_TX_ICON_INDEX].addFile(":/LDFEditorIcons/Resources/icons/32x32/Signals.png", QSize(32, 32));

    m_ouIcons[defFRAME_RX_ICON_INDEX].addFile(":/LDFEditorIcons/Resources/icons/16x16/Frame.png", QSize(16, 16));
    m_ouIcons[defFRAME_RX_ICON_INDEX].addFile(":/LDFEditorIcons/Resources/icons/22x22/Frame.png", QSize(22, 22));
    m_ouIcons[defFRAME_RX_ICON_INDEX].addFile(":/LDFEditorIcons/Resources/icons/32x32/Frame.png", QSize(32, 32));

    m_ouIcons[defFRAME_TX_ICON_INDEX].addFile(":/LDFEditorIcons/Resources/icons/16x16/Frame.png", QSize(16, 16));
    m_ouIcons[defFRAME_TX_ICON_INDEX].addFile(":/LDFEditorIcons/Resources/icons/22x22/Frame.png", QSize(22, 22));
    m_ouIcons[defFRAME_TX_ICON_INDEX].addFile(":/LDFEditorIcons/Resources/icons/32x32/Frame.png", QSize(32, 32));
}

LDFNetworkView::~LDFNetworkView()
{

}

void LDFNetworkView::OnElementDeleted(UID_ELEMENT uid)
{
    IElement* pElement;
    m_pouLdfCluster->GetElement(m_pouLdfCluster->GetElementType(uid), uid, &pElement);
    auto itr = m_UidTreeWidgetListMap.find(uid);
    if ( itr != m_UidTreeWidgetListMap.end())
    {
for ( auto itrItem : itr->second)
        {
            QTreeWidgetItem* pParent = itrItem->parent();
            if ( nullptr != pParent )
            {
                pParent->removeChild(itrItem);
            }
            else
            {
                takeTopLevelItem(indexOfTopLevelItem(itrItem));
            }
        }
        m_UidTreeWidgetListMap.erase(itr);
    }
}

void LDFNetworkView::OnElementCreated(UID_ELEMENT uid)
{
    IElement* pElement;
    m_pouLdfCluster->GetElement(m_pouLdfCluster->GetElementType(uid), uid, &pElement);
    if ( pElement == nullptr )
    {
        return vRefreshFrameAndSignals(nullptr);
    }

    switch (pElement->GetElementType())
    {
        case eEcuElement:
        {
            std::string strItemTitle;
            EcuProperties ouEcuProperties;
            ouEcuProperties.m_eEcuType = eEcuNone;
            TreeItemID ouTreeItemId;
            QVariant ouVariant;
            IEcu* pEcu = (IEcu*)pElement;
            pEcu->GetName(strItemTitle);
            pEcu->GetProperties(ouEcuProperties);
            if ( ouEcuProperties.m_eEcuType == eLIN_Master )
            {
                strItemTitle = strItemTitle + " [ Master ]";
            }
            EcuTreeWidgets ouEcuTreeWidgets;

            ouEcuTreeWidgets.m_pTreeWidgets[def_EcuTreeWidget_Index]->setText(0, QString::fromStdString(strItemTitle));
            ouEcuTreeWidgets.m_pTreeWidgets[def_EcuTreeWidget_Index]->setIcon(0, m_ouIcons[defECU_ICON_INDEX]);

            ouTreeItemId.m_uidElement = pEcu->GetUniqueId();
            ouVariant.setValue(ouTreeItemId);
            ouEcuTreeWidgets.m_pTreeWidgets[def_EcuTreeWidget_Index]->setData(0, Qt::UserRole, ouVariant);
            m_UidTreeWidgetListMap[ouTreeItemId.m_uidElement].push_back(ouEcuTreeWidgets.m_pTreeWidgets[def_EcuTreeWidget_Index]);
            addTopLevelItem(ouEcuTreeWidgets.m_pTreeWidgets[def_EcuTreeWidget_Index]);

            m_UidTreeWidgetMap[ pEcu->GetUniqueId()] = ouEcuTreeWidgets;

            vAddFrames(ouEcuTreeWidgets, pEcu);
            vAddSignals(ouEcuTreeWidgets, pEcu);
            //sortItems(0, Qt::AscendingOrder);
        }
        case eFrameElement:
        case eSignalElement:
        {
            vRefreshFrameAndSignals((IFrame*)pElement);
        }

        break;
        default:
            break;
    }
}

void LDFNetworkView::vRefreshFrameAndSignals(IFrame* pouFrame)
{
    IEcu* pEuc;
    UID_ELEMENT uid = INVALID_UID_ELEMENT;
for( auto itr : m_UidTreeWidgetMap)
    {
        uid = itr.first;
        m_pouLdfCluster->GetElement(eEcuElement, uid, (IElement**)&pEuc);
        vAddFrames(itr.second, pEuc);
        vAddSignals(itr.second, pEuc);
    }
}


void LDFNetworkView::onDataUpdate()
{
    std::string strItemTitle;
    clear();
    setColumnCount(1);

    if ( LDFDatabaseManager::GetDatabaseManager()->GetLDFFilePath().length() < 0 )
    {
        return;
    }

    ICluster* pLdfCluster = m_pouLDFDatabaseManager->GetLDFCluster();

    std::map<UID_ELEMENT, IElement*> ouEcuMap;
    pLdfCluster->GetElementList(eEcuElement, ouEcuMap);

    m_UidTreeWidgetListMap.clear();
    m_UidTreeWidgetMap.clear();
    QVariant ouVariant;
    TreeItemID ouTreeItemId;
    ouTreeItemId.m_eTreeElementType = eLdfItem;
    EcuProperties ouEcuProperties;
    ouEcuProperties.m_eEcuType = eEcuNone;
for ( auto itrEcu : ouEcuMap )
    {
        itrEcu.second->GetName(strItemTitle);
        ((IEcu*)itrEcu.second)->GetProperties(ouEcuProperties);
        if ( ouEcuProperties.m_eEcuType == eLIN_Master )
        {
            strItemTitle = strItemTitle + " [ Master ]";
        }
        EcuTreeWidgets ouEcuTreeWidgets;
        m_UidTreeWidgetMap[itrEcu.second->GetUniqueId()] = ouEcuTreeWidgets;

        //QTreeWidgetItem* pElementTopTreeItem = new QTreeWidgetItem();
        ouEcuTreeWidgets.m_pTreeWidgets[def_EcuTreeWidget_Index]->setText(0, QString::fromStdString(strItemTitle));
        ouEcuTreeWidgets.m_pTreeWidgets[def_EcuTreeWidget_Index]->setIcon(0, m_ouIcons[defECU_ICON_INDEX]);

        ouTreeItemId.m_uidElement = itrEcu.second->GetUniqueId();
        ouVariant.setValue(ouTreeItemId);
        ouEcuTreeWidgets.m_pTreeWidgets[def_EcuTreeWidget_Index]->setData(0, Qt::UserRole, ouVariant);
        m_UidTreeWidgetListMap[ouTreeItemId.m_uidElement].push_back(ouEcuTreeWidgets.m_pTreeWidgets[def_EcuTreeWidget_Index]);
        addTopLevelItem(ouEcuTreeWidgets.m_pTreeWidgets[def_EcuTreeWidget_Index]);

        vAddFrames(ouEcuTreeWidgets, (IEcu*)itrEcu.second);
        vAddSignals(ouEcuTreeWidgets, (IEcu*)itrEcu.second);
    }
    m_pouLdfCluster->RegisterForChangeNotification(this);
}



void LDFNetworkView::vAddSignals(EcuTreeWidgets& ouEcuTreeWidgets, IEcu* pEcu)
{
    std::list<IFrame*> ouFrameList;
    std::map<UID_ELEMENT, std::string> m_uidName;

    TreeItemID ouTreeItemID;
    QVariant ouVariant;

    ouTreeItemID.m_eTreeElementType = eTreeHeader;
    ouTreeItemID.m_uidElement = -1;

    std::map<ISignal*, ISignal*> ouSignalMap;
    pEcu->GetSignalList(eTx, ouSignalMap);
    QTreeWidgetItem* pouTxTableItem = ouEcuTreeWidgets.m_pTreeWidgets[def_TxSignalTreeWidget_Index];
    pouTxTableItem->takeChildren();
    pouTxTableItem->setIcon(0, m_ouIcons[defSIGNAL_ICON_INDEX]);
    ouVariant.setValue(ouTreeItemID);
    pouTxTableItem->setData(0, Qt::UserRole, ouVariant);

    std::string strText;

for( auto itrSignal : ouSignalMap )
    {
        itrSignal.first->GetName(strText);

        m_uidName[itrSignal.first->GetUniqueId()] =  strText;
    }


    ouTreeItemID.m_eTreeElementType = eLdfItem;
for ( auto uidSignal: m_uidName  )
    {
        QTreeWidgetItem* pouFrameItem = new QTreeWidgetItem();
        pouFrameItem->setText(0, QString::fromStdString(uidSignal.second));
        pouFrameItem->setIcon(0, m_ouIcons[defSIGNAL_ICON_INDEX]);
        ouTreeItemID.m_uidElement = uidSignal.first;
        ouVariant.setValue(ouTreeItemID);
        m_UidTreeWidgetListMap[ouTreeItemID.m_uidElement].push_back(pouFrameItem);
        pouFrameItem->setData(0, Qt::UserRole, ouVariant);

        pouTxTableItem->addChild(pouFrameItem);
    }
    m_uidName.clear();

    ouEcuTreeWidgets.m_pTreeWidgets[def_EcuTreeWidget_Index]->addChild(pouTxTableItem);
    pouTxTableItem->sortChildren(0, Qt::AscendingOrder);


    ouSignalMap.clear();


    pEcu->GetSignalList(eRx, ouSignalMap);
    QTreeWidgetItem* pouRxTableItem = ouEcuTreeWidgets.m_pTreeWidgets[def_RxSignalTreeWidget_Index];
    pouRxTableItem->takeChildren();

    pouRxTableItem->setText(0, "Subscribed Signals");
    pouRxTableItem->setIcon(0, m_ouIcons[defSIGNAL_ICON_INDEX]);
    ouTreeItemID.m_eTreeElementType = eTreeHeader;
    ouTreeItemID.m_uidElement = -1;
    ouVariant.setValue(ouTreeItemID);
    pouTxTableItem->setData(0, Qt::UserRole, ouVariant);


for( auto itrSignal : ouSignalMap )
    {
        itrSignal.first->GetName(strText);
        m_uidName[itrSignal.first->GetUniqueId()] =  strText;
    }

    ouTreeItemID.m_eTreeElementType = eLdfItem;
for ( auto uidSignal: m_uidName  )
    {
        QTreeWidgetItem* pouFrameItem = new QTreeWidgetItem();
        pouFrameItem->setText(0, QString::fromStdString(uidSignal.second));
        pouFrameItem->setIcon(0, m_ouIcons[defSIGNAL_ICON_INDEX]);
        ouTreeItemID.m_uidElement = uidSignal.first;
        ouVariant.setValue(ouTreeItemID);
        m_UidTreeWidgetListMap[ouTreeItemID.m_uidElement].push_back(pouFrameItem);
        pouFrameItem->setData(0, Qt::UserRole, ouVariant);
        pouRxTableItem->addChild(pouFrameItem);
    }
    m_uidName.clear();

    ouEcuTreeWidgets.m_pTreeWidgets[def_EcuTreeWidget_Index]->addChild(pouRxTableItem);
    pouRxTableItem->sortChildren(0, Qt::AscendingOrder);
}


void LDFNetworkView::vAddFrames(EcuTreeWidgets& ouEcuTreeWidgets, IEcu* pEcu)
{
    std::list<IFrame*> ouFrameList;
    TreeItemID ouTreeItemID;
    QVariant ouVariant;

    ouTreeItemID.m_eTreeElementType = eTreeHeader;
    ouTreeItemID.m_uidElement = -1;


    pEcu->GetFrameList(eTx, ouFrameList);
    QTreeWidgetItem* pouTxTableItem = ouEcuTreeWidgets.m_pTreeWidgets[def_TxFrameTreeWidget_Index];
    pouTxTableItem->setIcon(0, m_ouIcons[defFRAME_TX_ICON_INDEX]);
    pouTxTableItem->takeChildren();
    ouVariant.setValue(ouTreeItemID);
    pouTxTableItem->setData(0, Qt::UserRole, ouVariant);

    std::string strText;

    ouTreeItemID.m_eTreeElementType = eLdfItem;
    eFrameType oueFrameType;
for ( auto itrFrame : ouFrameList )
    {
        itrFrame->GetFrameType(oueFrameType);
        if ( eLIN_Unconditional == oueFrameType )
        {
            itrFrame->GetName(strText);
            QTreeWidgetItem* pouFrameItem = new QTreeWidgetItem();
            pouFrameItem->setText(0, QString::fromStdString(strText));
            pouFrameItem->setIcon(0, m_ouIcons[defFRAME_TX_ICON_INDEX]);
            ouTreeItemID.m_uidElement = itrFrame->GetUniqueId();
            ouVariant.setValue(ouTreeItemID);
            m_UidTreeWidgetListMap[ouTreeItemID.m_uidElement].push_back(pouFrameItem);
            pouFrameItem->setData(0, Qt::UserRole, ouVariant);
            pouTxTableItem->addChild(pouFrameItem);
        }
    }
    pouTxTableItem->sortChildren(0, Qt::AscendingOrder);
    ouEcuTreeWidgets.m_pTreeWidgets[def_EcuTreeWidget_Index]->addChild(pouTxTableItem);

    ouFrameList.clear();
    pEcu->GetFrameList(eRx, ouFrameList);
    QTreeWidgetItem* pouRxTableItem = ouEcuTreeWidgets.m_pTreeWidgets[def_RxFrameTreeWidget_Index];
    pouRxTableItem->setIcon(0, m_ouIcons[defFRAME_RX_ICON_INDEX]);
    pouRxTableItem->takeChildren();


for ( auto itrFrame : ouFrameList )
    {
        itrFrame->GetFrameType(oueFrameType);
        if ( eLIN_Unconditional == oueFrameType )
        {
            itrFrame->GetName(strText);
            QTreeWidgetItem* pouFrameItem = new QTreeWidgetItem();
            ouTreeItemID.m_uidElement = itrFrame->GetUniqueId();
            pouFrameItem->setText(0, QString::fromStdString(strText));
            pouFrameItem->setIcon(0, m_ouIcons[defFRAME_RX_ICON_INDEX]);
            ouVariant.setValue(ouTreeItemID);
            m_UidTreeWidgetListMap[ouTreeItemID.m_uidElement].push_back(pouFrameItem);
            pouFrameItem->setData(0, Qt::UserRole, ouVariant);
            pouRxTableItem->addChild(pouFrameItem);
        }
    }
    pouRxTableItem->sortChildren(0, Qt::AscendingOrder);
    ouEcuTreeWidgets.m_pTreeWidgets[def_EcuTreeWidget_Index]->addChild(pouRxTableItem);
}



ERRORCODE LDFNetworkView::OnClusterChange(eAction eActionType, UID_ELEMENT nId, eClusterElementType& ouElementType, void* pAcionData, void* pUserData)
{
    switch (ouElementType)
    {
        case eEcuElement:
        case eFrameElement:
            vHandleEcuElementChange(eActionType, nId, pAcionData);
        default:
            vHandleElementChange(eActionType, nId, pAcionData);
            break;
    }
    return EC_NA;
}

void LDFNetworkView::vHandleElementChange(eAction eActionType, UID_ELEMENT nId, void* pAcionData)
{
    if ( eDeleted == eActionType )
    {
        OnElementDeleted(nId);
    }

}

void LDFNetworkView::vHandleEcuElementChange(eAction eActionType, UID_ELEMENT nId, void* pAcionData)
{
    if ( eDeleted == eActionType )
    {
        OnElementDeleted(nId);
        auto itr = m_UidTreeWidgetMap.find(nId);
        if ( m_UidTreeWidgetMap.end() != itr)
        {
            m_UidTreeWidgetMap.erase(itr);
        }

    }
    else
    {
        IEcu* pEcu;

        std::string strName;
        TreeItemID uidTreeElement;
        EcuProperties ouEcuProps;
        ouEcuProps.m_eEcuType = eEcuNone;
        m_pouLdfCluster->GetElement(eEcuElement, nId, (IElement**)&pEcu);
        if ( nullptr != pEcu )
        {
            pEcu->GetName(strName);
            auto itr = m_UidTreeWidgetListMap[nId];
            pEcu->GetProperties(ouEcuProps);
            if ( eLIN_Master == ouEcuProps.m_eEcuType )
            {
                strName = strName + " [ Master ]";
            }
for ( auto itrTree : itr )
            {
                itrTree->setText(0, QString::fromStdString(strName));
            }

        }
    }
    return;
}
