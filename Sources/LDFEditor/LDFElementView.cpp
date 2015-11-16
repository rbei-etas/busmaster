#include "LDFElementView.h"
#include "Defines.h"
#include "LDFUtility.h"
#include <qmenu.h>
#include "MasterEditDlg.h"
#include "UnconditionalFrameEditDlg.h"
#include "EventFrameEditDlg.h"
#include "SporadicFrameEditDlg.h"
#include "LDFTagDefines.h"
#include "SlaveDlg.h"
#include "LDFNetworkView.h"
#include "qmessagebox.h"
#include "SignalDlg.h"
#include "CodingDlg.h"
#include "ScheduleTableDlg.h"
#include "SignalGroupDlg.h"
#include "LDFCLusterPropsDlg.h"
#include "NodeConfigurationDlg.h"

LDFElementView::LDFElementView(QWidget* parent)
    : QTreeWidget(parent)
{

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

    //we can use directly ecu icon.But fin feature if it reqired to chaange icon...
    m_ouIcons[defNODE_COMPOSITION_INDEX].addFile(":/LDFEditorIcons/Resources/icons/16x16/Ecu.png", QSize(16, 16));
    m_ouIcons[defNODE_COMPOSITION_INDEX].addFile(":/LDFEditorIcons/Resources/icons/22x22/Ecu.png", QSize(22, 22));
    m_ouIcons[defNODE_COMPOSITION_INDEX].addFile(":/LDFEditorIcons/Resources/icons/32x32/Ecu.png", QSize(32, 32));


    m_pouLDFDatabaseManager = LDFDatabaseManager::GetDatabaseManager();
    m_pBaseCluster = m_pouLDFDatabaseManager->GetLDFCluster();
    VALIDATE_POINTER(m_pouLDFDatabaseManager);
    connect(m_pouLDFDatabaseManager, SIGNAL(DataUpdated()), this, SLOT(onDataUpdate()));
    connect(m_pouLDFDatabaseManager, SIGNAL(DisplayModeChanged()), this, SLOT(onDisplayModeChanged()));

    setSelectionMode(SingleSelection);

    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(onCustomContextMenu(const QPoint&)));
    connect(this, SIGNAL(itemDoubleClicked(QTreeWidgetItem* , int )), this, SLOT(onItemDoubleClicked(QTreeWidgetItem* , int )));
    CreateHeaderPopupMenus();
    CreateElementPopupMenus();

}

LDFElementView::~LDFElementView()
{

}
void LDFElementView::onDataUpdate()
{
    clear();
    setColumnCount(1);
    m_UidTreeWidgetMap.clear();
    m_bDiagFrames[0] = false;
    m_bDiagFrames[1] = false;
    m_poUnconditionTreeItem = nullptr;
    m_poDiagTreeItem = nullptr;
    m_poEventTrigTreeItem = nullptr;
    if ( LDFDatabaseManager::GetDatabaseManager()->GetLDFFilePath().length() > 0 )
    {
        vPopulateElementTree();
    }

}
void LDFElementView::onDisplayModeChanged()
{
    if ( currentItem() != nullptr )
    {

        TreeItemID ouTreeItemId = currentItem()->data(0, Qt::UserRole).value<TreeItemID>();
        vPopulateLinUnconditionalFrames(nullptr);
        vPopulateLinDiagFrames(nullptr);
        vPopulateLinEventTrigFrames(nullptr);

        SelectElement(ouTreeItemId);
    }
}


void LDFElementView::showContextMenu(QMenu& ouMenu,QTreeWidgetItem* item, const QPoint& globalPos)
{
    ouMenu.exec(globalPos);
}

void LDFElementView::vPopulateElementTree()
{
    QTreeWidgetItem* pElementTopTreeItem = new QTreeWidgetItem();
    pElementTopTreeItem->setText(0, "Lin Cluster");

    setContextMenuPolicy(Qt::CustomContextMenu);

    vPopulateLinEcus(pElementTopTreeItem);
    vPopulateLinFrames(pElementTopTreeItem);
    vPopulateLinSignals(pElementTopTreeItem);
    vPopulateLinSignalGroups(pElementTopTreeItem);
    vPopulateLinCoding(pElementTopTreeItem);
    vPopulateLinScheduleTables(pElementTopTreeItem);
    vPopulateLinNodeComposition(pElementTopTreeItem);

    TreeItemID ouTreeItemID;
    QVariant ouVariant;
    ouTreeItemID.m_eTreeElementType = eTreeHeader;
    ouTreeItemID .m_uidElement = defTREE_ITEM_LINCLUSTER;
    ouVariant.setValue(ouTreeItemID);
    pElementTopTreeItem->setData(0, Qt::UserRole, ouVariant);

    addTopLevelItem(pElementTopTreeItem);
    pElementTopTreeItem->setExpanded(true);
}

void LDFElementView::vPopulateLinElement(QTreeWidgetItem* pTopTreeItem, eClusterElementType ouClusterElement, std::string strItemTitle, QIcon& icon, int nTreeHeaderId )
{
    VALIDATE_POINTER(pTopTreeItem)
    ICluster* pouLDFCluster = m_pouLDFDatabaseManager->GetLDFCluster();
    VALIDATE_POINTER(pouLDFCluster);

    std::map< UID_ELEMENT, IElement*> ouElementMap;
    pouLDFCluster->GetElementList(ouClusterElement, ouElementMap);

    TreeItemID ouTreeItemID;
    QVariant ouVariant;

    QTreeWidgetItem* pElementTopTreeItem = new QTreeWidgetItem();
    pElementTopTreeItem->setText(0, QString::fromStdString(strItemTitle));
    QList<QTreeWidgetItem*> LinElementItem;
    std::string strName;
    pElementTopTreeItem->setIcon(0, icon);

    ouTreeItemID.m_eTreeElementType = eTreeHeader;
    ouTreeItemID.m_uidElement = nTreeHeaderId;
    ouVariant.setValue(ouTreeItemID);
    pElementTopTreeItem->setData(0, Qt::UserRole, ouVariant);

    ouTreeItemID.m_eTreeElementType = eLdfItem;
for ( auto itrEcu : ouElementMap )
    {
        itrEcu.second->GetName(strName);
        QTreeWidgetItem* pElementItem = new QTreeWidgetItem();
        pElementItem->setText(0, QString::fromStdString(strName));
        pElementItem->setIcon(0, icon);
        ouTreeItemID.m_uidElement = itrEcu.second->GetUniqueId();
        m_UidTreeWidgetMap[ouTreeItemID.m_uidElement] = pElementItem;
        ouVariant.setValue(ouTreeItemID);
        pElementItem->setData(0, Qt::UserRole, ouVariant);
        LinElementItem.push_back(pElementItem);
    }


    pElementTopTreeItem->addChildren(LinElementItem);

    pTopTreeItem->addChild(pElementTopTreeItem);
    pElementTopTreeItem->sortChildren(0, Qt::AscendingOrder);
}

void LDFElementView::vPopulateLinEcus(QTreeWidgetItem* pTopTreeItem)
{
    VALIDATE_POINTER(pTopTreeItem);

    ICluster* pouLDFCluster = m_pouLDFDatabaseManager->GetLDFCluster();
    VALIDATE_POINTER(pouLDFCluster);

    std::map< UID_ELEMENT, IElement*> ouElementMap;
    pouLDFCluster->GetElementList(eEcuElement, ouElementMap);

    QTreeWidgetItem* pElementTopTreeItem[LIN_MAX_ECU_TYPES];
    QTreeWidgetItem* pElementItem[LIN_MAX_ECU_TYPES];

    TreeItemID ouTreeItemID;
    QVariant ouVariant;

    std::string strLinFrameTpes[] = { "Master", "Slaves"};
    for ( int i = 0 ; i < LIN_MAX_ECU_TYPES; i++ )
    {
        ouTreeItemID.m_eTreeElementType = eTreeHeader;
        ouTreeItemID.m_uidElement = defTREE_ITEM_MASTER+i;
        ouVariant.setValue(ouTreeItemID);

        pElementTopTreeItem[i] = new QTreeWidgetItem();
        pElementTopTreeItem[i]->setText(0, QString::fromStdString(strLinFrameTpes[i]));
        pElementTopTreeItem[i]->setIcon(0, m_ouIcons[defECU_ICON_INDEX]);
        pElementTopTreeItem[i]->setData(0, Qt::UserRole, ouVariant);
    }

    QList<QTreeWidgetItem*> LinElementItem[LIN_MAX_ECU_TYPES];
    std::string strName;
    EcuProperties ouECUProps;

for ( auto itrSignal : ouElementMap )
    {
        itrSignal.second->GetName(strName);

        ((IEcu*)itrSignal.second)->GetProperties(ouECUProps);

        switch (ouECUProps.m_eEcuType)
        {
            case eLIN_Master:
                pElementItem[LIN_MASTER_ECU] = new QTreeWidgetItem();
                pElementItem[LIN_MASTER_ECU]->setText(0, QString::fromStdString(strName));
                pElementItem[LIN_MASTER_ECU]->setIcon(0, m_ouIcons[defECU_ICON_INDEX]);

                ouTreeItemID.m_eTreeElementType = eLdfItem;

                ouTreeItemID.m_uidElement = itrSignal.second->GetUniqueId();
                m_UidTreeWidgetMap[ouTreeItemID.m_uidElement] = pElementItem[LIN_MASTER_ECU];
                ouVariant.setValue(ouTreeItemID);

                pElementItem[LIN_MASTER_ECU]->setData(0, Qt::UserRole, ouVariant);
                LinElementItem[LIN_MASTER_ECU].push_back(pElementItem[LIN_MASTER_ECU]);
                break;
            case eLIN_Slave:
                pElementItem[LIN_SLAVE_ECU] = new QTreeWidgetItem();
                pElementItem[LIN_SLAVE_ECU]->setText(0, QString::fromStdString(strName));
                pElementItem[LIN_SLAVE_ECU]->setIcon(0, m_ouIcons[defECU_ICON_INDEX]);

                ouTreeItemID.m_eTreeElementType = eLdfItem;

                ouTreeItemID.m_uidElement = itrSignal.second->GetUniqueId();
                m_UidTreeWidgetMap[ouTreeItemID.m_uidElement] = pElementItem[LIN_SLAVE_ECU];
                ouVariant.setValue(ouTreeItemID);

                pElementItem[LIN_SLAVE_ECU]->setData(0, Qt::UserRole, ouVariant);
                LinElementItem[LIN_SLAVE_ECU].push_back(pElementItem[LIN_SLAVE_ECU]);
                break;
            default:
                break;
        }
    }

    for ( int i = 0 ; i < LIN_MAX_ECU_TYPES; i++ )
    {
        pElementTopTreeItem[i]->addChildren(LinElementItem[i]);
        pTopTreeItem->addChild(pElementTopTreeItem[i]);
        pElementTopTreeItem[i]->sortChildren(0, Qt::AscendingOrder);
    }

}
void LDFElementView::SetLDFNetworkView(LDFNetworkView* pouLDFNetworkView)
{
    m_pouLDFNetworkView = pouLDFNetworkView;
    connect(this, SIGNAL(ElementCreated(UID_ELEMENT)), m_pouLDFNetworkView, SLOT(OnElementCreated(UID_ELEMENT)));

}

void LDFElementView::OnSlaveNodeDelete(UID_ELEMENT uid)
{
    IEcu* pEcu = nullptr;

    if(EC_SUCCESS == m_pBaseCluster->GetElement(eEcuElement, uid, (IElement**)&pEcu))
    {
        std::list<IFrame*> lstFrames;
        std::list<IFrame*>::iterator itrFrame;
        std::map<ISignal*, ISignal*> m_mapSignals;
        pEcu->GetFrameList(eTx,  lstFrames);

        itrFrame = lstFrames.begin();
        FrameProps ouFrameProps;
        while(itrFrame != lstFrames.end())
        {
            UID_ELEMENT uidFrame = (*itrFrame)->GetUniqueId();
            (*itrFrame)->GetProperties(ouFrameProps);

            if(ouFrameProps.m_eFrameType == eLIN_Unconditional)
            {
                pEcu->UnMapFrame(eTx, uidFrame);
                itrFrame = lstFrames.erase(itrFrame);
                m_pBaseCluster->DeleteElement(eFrameElement, uidFrame);

                auto itr = m_UidTreeWidgetMap.find(uidFrame);
                if ( itr != m_UidTreeWidgetMap.end() )
                {
                    itr->second->parent()->removeChild(itr->second);
                }
            }
            else
            {
                itrFrame++;
            }
        }

        pEcu->GetSignalList(eTx,  m_mapSignals);
for ( auto itr : m_mapSignals)
        {
            UID_ELEMENT uidSignal = itr.first->GetUniqueId();
            pEcu->UnMapSignal(eTx, uidSignal);
            m_pBaseCluster->DeleteElement(eSignalElement, uidSignal);

            auto itrSignal = m_UidTreeWidgetMap.find(uidSignal);
            if ( itrSignal != m_UidTreeWidgetMap.end() )
            {
                itrSignal->second->parent()->removeChild(itrSignal->second);
            }
        }


    }
}


void LDFElementView::SelectElement(TreeItemID uidTreeElement)
{
    if ( eLdfItem == uidTreeElement.m_eTreeElementType )
    {
        auto itr = m_UidTreeWidgetMap.find(uidTreeElement.m_uidElement);
        if ( m_UidTreeWidgetMap.end() != itr )
        {
            auto selItems = selectedItems();
for ( auto selItem : selItems )
            {
                setItemSelected(selItem, false);
            }

            setItemSelected(itr->second, true);
            setCurrentItem(itr->second);
            scrollToItem(itr->second);
        }
    }
    else if ( eTreeHeader == uidTreeElement.m_eTreeElementType )
    {

    }
}



void LDFElementView::vPopulateLinFrames(QTreeWidgetItem* pTopTreeItem)
{
    VALIDATE_POINTER(pTopTreeItem);

    ICluster* pouLDFCluster = m_pouLDFDatabaseManager->GetLDFCluster();
    VALIDATE_POINTER(pouLDFCluster);

    LIN_Settings ouLINSettings;
    pouLDFCluster->GetProperties(eLINClusterProperties, &ouLINSettings);

    std::map< UID_ELEMENT, IElement*> ouFrameMap;
    pouLDFCluster->GetElementList(eFrameElement, ouFrameMap);

    QTreeWidgetItem* pElementTopTreeItem[LIN_MAX_FRAME_TYPES] = {nullptr};
    QTreeWidgetItem* pElementItem;

    TreeItemID ouTreeItemID;
    QVariant ouVariant;
    QString omstrFrameId;

    std::string strLinFrameTpes[] = { "Unconditional Frames", "Event Triggered", "Sporadic Frames", "Diagnostic Frames" };

    int nTreeId[] = {defTRRE_ITEM_UNCONDITIONAL, defTREE_ITEM_EVENT_TRIGGERED, defTREE_ITEM_SPORADIC, defTREE_ITEM_DIAGNOSTIC};

    for ( int i = 0 ; i < LIN_MAX_FRAME_TYPES; i++ )
    {
        if(ouLINSettings.m_dProtocolVers <= defVersion_1_3 && nTreeId[i] == defTREE_ITEM_SPORADIC)
        {
            continue;
        }

        pElementTopTreeItem[i] = new QTreeWidgetItem();
        pElementTopTreeItem[i]->setText(0, QString::fromStdString(strLinFrameTpes[i]));
        pElementTopTreeItem[i]->setIcon(0, m_ouIcons[defFRAME_ICON_INDEX]);

        ouTreeItemID.m_eTreeElementType = eTreeHeader;
        ouTreeItemID.m_uidElement = nTreeId[i];
        ouVariant.setValue(ouTreeItemID);

        pElementTopTreeItem[i]->setData(0, Qt::UserRole, ouVariant);
    }
    m_poUnconditionTreeItem = pElementTopTreeItem[0];
    m_poDiagTreeItem = pElementTopTreeItem[3];
    m_poEventTrigTreeItem = pElementTopTreeItem[1];
    QList<QTreeWidgetItem*> LinElementItem[LIN_MAX_FRAME_TYPES];
    std::string strName;

    ouTreeItemID.m_eTreeElementType = eLdfItem;
for ( auto itrFrame : ouFrameMap )
    {
        itrFrame.second->GetName(strName);
        eFrameType ouLinFrameType;
        ((IFrame*)(itrFrame.second))->GetFrameType(ouLinFrameType);
        unsigned int unId = -1;
        char chText[33] = "";
        switch (ouLinFrameType)
        {
            case eLIN_Unconditional:
            {
                pElementItem = new QTreeWidgetItem();
                ((IFrame*)(itrFrame.second))->GetFrameId(unId);
                /*sprintf_s(chText, "0x%X", unId);*/
                omstrFrameId = GetString(unId);
                strName = strName + " [ " + omstrFrameId.toStdString() + " ] ";
                pElementItem->setText(0, QString::fromStdString(strName));
                pElementItem->setIcon(0, m_ouIcons[defFRAME_ICON_INDEX]);

                ouTreeItemID.m_uidElement = itrFrame.second->GetUniqueId();
                m_UidTreeWidgetMap[ouTreeItemID.m_uidElement] = pElementItem;
                ouVariant.setValue(ouTreeItemID);
                pElementItem->setData(0, Qt::UserRole, ouVariant);

                LinElementItem[LIN_UNCONDITIONAL_FRAME_INDEX].push_back(pElementItem);

            }
            break;
            case eLIN_Sporadic:
            {
                if(ouLINSettings.m_dProtocolVers <= defVersion_1_3)
                {
                    break;
                }

                pElementItem = new QTreeWidgetItem();
                pElementItem->setText(0, QString::fromStdString(strName));
                pElementItem->setIcon(0, m_ouIcons[defFRAME_ICON_INDEX]);

                ouTreeItemID.m_uidElement = itrFrame.second->GetUniqueId();
                m_UidTreeWidgetMap[ouTreeItemID.m_uidElement] = pElementItem;
                ouVariant.setValue(ouTreeItemID);
                pElementItem->setData(0, Qt::UserRole, ouVariant);

                LinElementItem[LIN_SPORADIC_FRAME_INDEX].push_back(pElementItem);
            }
            break;
            case eLIN_EventTriggered:
            {
                pElementItem = new QTreeWidgetItem();
                ((IFrame*)(itrFrame.second))->GetFrameId(unId);
                omstrFrameId = GetString(unId);

                strName = strName + " [ " + omstrFrameId.toStdString() + " ] ";
                pElementItem->setText(0, QString::fromStdString(strName));
                pElementItem->setIcon(0, m_ouIcons[defFRAME_ICON_INDEX]);

                ouTreeItemID.m_uidElement = itrFrame.second->GetUniqueId();
                m_UidTreeWidgetMap[ouTreeItemID.m_uidElement] = pElementItem;
                ouVariant.setValue(ouTreeItemID);
                pElementItem->setData(0, Qt::UserRole, ouVariant);

                LinElementItem[LIN_EVENT_FRAME_INDEX].push_back(pElementItem);
            }
            break;
            case eLIN_Diagnostic:
            {
                nGetDigFrameTreeWidgetItem(((IFrame*)(itrFrame.second)), pElementItem);
                LinElementItem[LIN_DIAGNOSTIC_FRAME_INDEX].push_back(pElementItem);
            }
            default:
                break;
        }
    }

    for ( int nFrameIndex = 0 ; nFrameIndex < LIN_MAX_FRAME_TYPES; nFrameIndex++ )
    {
        if(nFrameIndex == LIN_SPORADIC_FRAME_INDEX && ouLINSettings.m_dProtocolVers <= defVersion_1_3)
        {
            continue;
        }
        pElementTopTreeItem[nFrameIndex]->addChildren(LinElementItem[nFrameIndex]);
        pTopTreeItem->addChild(pElementTopTreeItem[nFrameIndex]);
        //addTopLevelItem(pElementTopTreeItem[nFrameIndex]);
        pElementTopTreeItem[nFrameIndex]->sortChildren(0, Qt::AscendingOrder);
    }
}

void LDFElementView::vPopulateLinEventTrigFrames(QTreeWidgetItem* pElementTopTreeItem)
{
    std::map<UID_ELEMENT, IElement*> ouFrameMap;
    m_pBaseCluster->GetElementList(eFrameElement, ouFrameMap);
    QString omstrFrameId;
    std::string strName;
    TreeItemID ouTreeItemID;
    QVariant ouVariant;
    eFrameType oueFrameType;
    unsigned int unId;
    m_poEventTrigTreeItem->takeChildren();
    ouTreeItemID.m_eTreeElementType = eLdfItem;
for ( auto itrFrame : ouFrameMap )
    {
        ((IFrame*)(itrFrame.second))->GetFrameType(oueFrameType);
        if ( oueFrameType != eLIN_EventTriggered )
        {
            continue;
        }
        QTreeWidgetItem* pElementItem = new QTreeWidgetItem();
        itrFrame.second->GetName(strName);
        ((IFrame*)(itrFrame.second))->GetFrameId(unId);

        omstrFrameId = GetString(unId);
        strName = strName + " [ " + omstrFrameId.toStdString() + " ] ";
        pElementItem->setText(0, QString::fromStdString(strName));
        pElementItem->setIcon(0, m_ouIcons[defFRAME_ICON_INDEX]);

        ouTreeItemID.m_uidElement = itrFrame.second->GetUniqueId();
        m_UidTreeWidgetMap[ouTreeItemID.m_uidElement] = pElementItem;
        ouVariant.setValue(ouTreeItemID);
        pElementItem->setData(0, Qt::UserRole, ouVariant);
        m_poEventTrigTreeItem->addChild(pElementItem);
    }
}

void LDFElementView::vPopulateLinDiagFrames(QTreeWidgetItem* pElementTopTreeItem)
{
    std::map<UID_ELEMENT, IElement*> ouFrameMap;
    m_pBaseCluster->GetElementList(eFrameElement, ouFrameMap);
    QString omstrFrameId;
    std::string strName;
    TreeItemID ouTreeItemID;
    QVariant ouVariant;
    eFrameType oueFrameType;
    unsigned int unId;
    m_poDiagTreeItem->takeChildren();
    ouTreeItemID.m_eTreeElementType = eLdfItem;
for ( auto itrFrame : ouFrameMap )
    {
        ((IFrame*)(itrFrame.second))->GetFrameType(oueFrameType);
        if ( oueFrameType != eLIN_Diagnostic )
        {
            continue;
        }
        QTreeWidgetItem* pElementItem = new QTreeWidgetItem();
        itrFrame.second->GetName(strName);
        ((IFrame*)(itrFrame.second))->GetFrameId(unId);
        /*sprintf_s(chText, "0x%X", unId);*/
        omstrFrameId = GetString(unId);
        strName = strName + " [ " + omstrFrameId.toStdString() + " ] ";
        pElementItem->setText(0, QString::fromStdString(strName));
        pElementItem->setIcon(0, m_ouIcons[defFRAME_ICON_INDEX]);

        ouTreeItemID.m_uidElement = itrFrame.second->GetUniqueId();
        m_UidTreeWidgetMap[ouTreeItemID.m_uidElement] = pElementItem;
        ouVariant.setValue(ouTreeItemID);
        pElementItem->setData(0, Qt::UserRole, ouVariant);
        m_poDiagTreeItem->addChild(pElementItem);
    }

}
void LDFElementView::vPopulateLinUnconditionalFrames(QTreeWidgetItem* pElementTopTreeItem)
{
    std::map<UID_ELEMENT, IElement*> ouFrameMap;
    m_pBaseCluster->GetElementList(eFrameElement, ouFrameMap);
    QString omstrFrameId;
    std::string strName;
    TreeItemID ouTreeItemID;
    QVariant ouVariant;
    eFrameType oueFrameType;
    unsigned int unId;
    ouTreeItemID.m_eTreeElementType = eLdfItem;
    m_poUnconditionTreeItem->takeChildren();
for ( auto itrFrame : ouFrameMap )
    {
        ((IFrame*)(itrFrame.second))->GetFrameType(oueFrameType);
        if ( oueFrameType != eLIN_Unconditional )
        {
            continue;
        }
        QTreeWidgetItem* pElementItem = new QTreeWidgetItem();
        itrFrame.second->GetName(strName);
        ((IFrame*)(itrFrame.second))->GetFrameId(unId);
        /*sprintf_s(chText, "0x%X", unId);*/
        omstrFrameId = GetString(unId);
        strName = strName + " [ " + omstrFrameId.toStdString() + " ] ";
        pElementItem->setText(0, QString::fromStdString(strName));
        pElementItem->setIcon(0, m_ouIcons[defFRAME_ICON_INDEX]);

        ouTreeItemID.m_uidElement = itrFrame.second->GetUniqueId();
        m_UidTreeWidgetMap[ouTreeItemID.m_uidElement] = pElementItem;
        ouVariant.setValue(ouTreeItemID);
        pElementItem->setData(0, Qt::UserRole, ouVariant);
        m_poUnconditionTreeItem->addChild(pElementItem);
    }
}


int LDFElementView::nGetDigFrameTreeWidgetItem(IFrame* pDigFrame, QTreeWidgetItem*& pouTreeWidget)
{

    unsigned int unId = 0;
    std::string strName;
    char chText[33];
    TreeItemID ouTreeItemID;
    QVariant ouVariant;
    QString strTemp;
    pouTreeWidget = new QTreeWidgetItem();
    pDigFrame->GetFrameId(unId);
    pDigFrame->GetName(strName);
    if ( unId == 0x3C )
    {
        m_bDiagFrames[0] = true;
    }
    if ( unId == 0x3D )
    {
        m_bDiagFrames[1] = true;
    }
    strTemp = GetString(unId);
    strName = strName + " [ " + strTemp.toStdString() + " ] ";
    pouTreeWidget->setText(0, QString::fromStdString(strName));
    pouTreeWidget->setIcon(0, m_ouIcons[defFRAME_ICON_INDEX]);
    pouTreeWidget->setData(0, Qt::UserRole, pDigFrame->GetUniqueId());
    ouTreeItemID.m_eTreeElementType = eLdfItem;
    ouTreeItemID.m_uidElement = pDigFrame->GetUniqueId();
    m_UidTreeWidgetMap[ouTreeItemID.m_uidElement] = pouTreeWidget;
    ouVariant.setValue(ouTreeItemID);
    pouTreeWidget->setData(0, Qt::UserRole, ouVariant);
    return 0;
}

void LDFElementView::vPopulateLinSignalGroups(QTreeWidgetItem* pElementTopTreeItem)
{
    vPopulateLinElement(pElementTopTreeItem, eSignalGroupElement, "Signal Groups", m_ouIcons[defSIGNAL_ICON_INDEX], defTREE_ITEM_SIGNALGROUP);
}


void LDFElementView::vPopulateLinSignals(QTreeWidgetItem* pTopTreeItem)
{
    VALIDATE_POINTER(pTopTreeItem);
    ICluster* pouLDFCluster = m_pouLDFDatabaseManager->GetLDFCluster();
    VALIDATE_POINTER(pouLDFCluster);

    std::map< UID_ELEMENT, IElement*> ouElementMap;
    pouLDFCluster->GetElementList(eSignalElement, ouElementMap);


    QTreeWidgetItem* pElementItem[LIN_MAX_SIGNAL_TYPES];

    TreeItemID ouTreeItemID;
    QVariant ouVariant;

    std::string strLinFrameTpes[] = { "Signals", "Diagnostic Signals"};
    int nTreeId[] = {defTREE_ITEM_NORMAL_SIGNAL, defTREE_ITEM_DIAG_SIGNAL};
    for ( int i = 0 ; i < LIN_MAX_SIGNAL_TYPES; i++ )
    {
        m_pDiagElementTopTreeItem[i] = new QTreeWidgetItem();
        m_pDiagElementTopTreeItem[i]->setText(0, QString::fromStdString(strLinFrameTpes[i]));
        m_pDiagElementTopTreeItem[i]->setIcon(0, m_ouIcons[defSIGNAL_ICON_INDEX]);

        ouTreeItemID.m_eTreeElementType = eTreeHeader;
        ouTreeItemID.m_uidElement = nTreeId[i];
        ouVariant.setValue(ouTreeItemID);
        m_pDiagElementTopTreeItem[i]->setData(0, Qt::UserRole, ouVariant);


    }

    QList<QTreeWidgetItem*> LinElementItem[LIN_MAX_SIGNAL_TYPES];
    std::string strName;
    SignalProps ouSignalProps;
    ouTreeItemID.m_eTreeElementType = eLdfItem;

for ( auto itrSignal : ouElementMap )
    {
        itrSignal.second->GetName(strName);

        ((ISignal*)itrSignal.second)->GetProperties(ouSignalProps);

        switch (ouSignalProps.m_ouLINSignalProps.m_ouSignalType)
        {
            case eSignalNormal:
                pElementItem[LIN_NORMAL_SIGNAL] = new QTreeWidgetItem();
                pElementItem[LIN_NORMAL_SIGNAL]->setText(0, QString::fromStdString(strName));
                pElementItem[LIN_NORMAL_SIGNAL]->setIcon(0, m_ouIcons[defSIGNAL_ICON_INDEX]);

                ouTreeItemID.m_uidElement = itrSignal.second->GetUniqueId();
                m_UidTreeWidgetMap[ouTreeItemID.m_uidElement] = pElementItem[LIN_NORMAL_SIGNAL];
                ouVariant.setValue(ouTreeItemID);
                pElementItem[LIN_NORMAL_SIGNAL]->setData(0, Qt::UserRole, ouVariant);


                LinElementItem[LIN_NORMAL_SIGNAL].push_back(pElementItem[LIN_NORMAL_SIGNAL]);
                break;
            case eSignalDiag:
                pElementItem[LIN_DIAGNOSTICS_SIGNAL] = new QTreeWidgetItem();
                pElementItem[LIN_DIAGNOSTICS_SIGNAL]->setText(0, QString::fromStdString(strName));
                pElementItem[LIN_DIAGNOSTICS_SIGNAL]->setIcon(0, m_ouIcons[defSIGNAL_ICON_INDEX]);


                ouTreeItemID.m_uidElement = itrSignal.second->GetUniqueId();
                m_UidTreeWidgetMap[ouTreeItemID.m_uidElement] = pElementItem[LIN_DIAGNOSTICS_SIGNAL];
                ouVariant.setValue(ouTreeItemID);
                pElementItem[LIN_DIAGNOSTICS_SIGNAL]->setData(0, Qt::UserRole, ouVariant);


                LinElementItem[LIN_DIAGNOSTICS_SIGNAL].push_back(pElementItem[LIN_DIAGNOSTICS_SIGNAL]);
                break;
            default:
                break;
        }
    }

    for ( int i = 0 ; i < LIN_MAX_SIGNAL_TYPES; i++ )
    {

        pTopTreeItem->addChild(m_pDiagElementTopTreeItem[i]);
        m_pDiagElementTopTreeItem[i]->addChildren(LinElementItem[i]);
        m_pDiagElementTopTreeItem[i]->sortChildren(0, Qt::AscendingOrder);
    }
}

void LDFElementView::vPopulateLinCoding(QTreeWidgetItem* pTopTreeItem)
{
    vPopulateLinElement(pTopTreeItem, eCodingElement, "Codings", m_ouIcons[defCODING_ICON_INDEX], defTREE_ITEM_CODING);
}

void LDFElementView::vPopulateLinScheduleTables(QTreeWidgetItem* pTopTreeItem)
{
    vPopulateLinElement(pTopTreeItem, eScheduleTableElement, "Schedule Tables", m_ouIcons[defSCHEDULETABLE_ICON_INDEX], defTREE_ITEM_SCHEDULETABLE);
}

void LDFElementView::vPopulateLinNodeComposition(QTreeWidgetItem* pTopTreeItem)
{
    LIN_Settings ouLINSettings;
    m_pBaseCluster->GetProperties(eLINClusterProperties, &ouLINSettings);
    if(ouLINSettings.m_dProtocolVers <= defVersion_1_3)
    {
        return;
    }
    std::map<UID_ELEMENT, IElement*> mapNodeComps;
    m_pBaseCluster->GetElementList(eLinNodeComposition, mapNodeComps);
    std::string omstrConfigName;
    TreeItemID ouTreeItemID;
    QVariant ouVariant;

    QList<QTreeWidgetItem*> LinElementItem;

    QTreeWidgetItem* pElementTopTreeItem = new QTreeWidgetItem();
    pElementTopTreeItem->setText(0, "Node Compositions");
    pElementTopTreeItem->setIcon(0, m_ouIcons[defECU_ICON_INDEX]);

    ouTreeItemID.m_eTreeElementType = eTreeHeader;
    ouTreeItemID.m_uidElement = defTREE_ITEM_NODECOMP;
    ouVariant.setValue(ouTreeItemID);
    pElementTopTreeItem->setData(0, Qt::UserRole, ouVariant);

    ouTreeItemID.m_eTreeElementType = eLdfItem;
for(auto itrNodeConfig : mapNodeComps)
    {
        itrNodeConfig.second->GetName(omstrConfigName);

        QTreeWidgetItem* pElementItem = new QTreeWidgetItem();
        pElementItem->setText(0, QString::fromStdString(omstrConfigName));
        pElementItem->setIcon(0, m_ouIcons[defECU_ICON_INDEX]);
        ouTreeItemID.m_uidElement = itrNodeConfig.second->GetUniqueId();
        m_UidTreeWidgetMap[ouTreeItemID.m_uidElement] = pElementItem;
        ouVariant.setValue(ouTreeItemID);
        pElementItem->setData(0, Qt::UserRole, ouVariant);
        LinElementItem.push_back(pElementItem);
    }

    pElementTopTreeItem->addChildren(LinElementItem);

    pTopTreeItem->addChild(pElementTopTreeItem);
    pElementTopTreeItem->sortChildren(0, Qt::AscendingOrder);
}

void LDFElementView::CreateHeaderPopupMenus()
{
    QAction* pouAction;
    for(unsigned int unIndex = eLinClusterHeaderPopup; unIndex < eTotalHeaderPopup; unIndex++)
    {
        switch(unIndex)
        {
            case eLinClusterHeaderPopup:
                pouAction = m_ouHeaderPopupMenus[unIndex].addAction(defEDIT);
                connect(pouAction, SIGNAL(triggered()), this, SLOT(vOnEditLinCluster()));
                break;
            case eSlaveHeaderPopup:
                pouAction = m_ouHeaderPopupMenus[unIndex].addAction(defNEW);
                connect(pouAction, SIGNAL(triggered()), this, SLOT(vOnNewSlave()));
                break;
            case eUnconditionalFrameHeaderPopup:
            {
                pouAction = m_ouHeaderPopupMenus[unIndex].addAction(defNEW);
                connect(pouAction, SIGNAL(triggered()), this, SLOT(vOnNewUnCondFrameElement()));
            }
            break;
            case eEventTriggeredFrameHeaderPopup:
            {
                pouAction = m_ouHeaderPopupMenus[unIndex].addAction(defNEW);
                connect(pouAction, SIGNAL(triggered()), this, SLOT(vOnNewEventTrigFrameElement()));
            }
            break;
            case eSporadicFrameHeaderPopup:
                pouAction = m_ouHeaderPopupMenus[unIndex].addAction(defNEW);
                connect(pouAction, SIGNAL(triggered()), this, SLOT(vOnNewSporadicFrameElement()));
                break;
            case eSignalHeaderPopup:
            {
                pouAction = m_ouHeaderPopupMenus[unIndex].addAction(defNEW);
                connect(pouAction, SIGNAL(triggered()), this, SLOT(vOnNewSignal()));
            }
            break;
            case eCodingHeaderPopup:
            {
                pouAction = m_ouHeaderPopupMenus[unIndex].addAction(defNEW);
                connect(pouAction, SIGNAL(triggered()), this, SLOT(vOnNewCoding()));
            }
            break;
            case eScheduleTableHeaderPopup:
            {
                pouAction = m_ouHeaderPopupMenus[unIndex].addAction(defNEW);
                connect(pouAction, SIGNAL(triggered()), this, SLOT(vOnNewScheduleTable()));
            }
            break;
            case eDiagnosticFrameHeaderPopup:
                pouAction = m_ouHeaderPopupMenus[unIndex].addAction(defADD_DIAGSUPPORT);
                connect(pouAction, SIGNAL(triggered()), this, SLOT(vOnAddDiagSupport()));
                pouAction = m_ouHeaderPopupMenus[unIndex].addAction(defREMOVE_DIAGSUPPORT);
                connect(pouAction, SIGNAL(triggered()), this, SLOT(vOnRemoveDiagSupport()));
                break;
            case eSignalGroupHeaderPopUp:
                pouAction = m_ouHeaderPopupMenus[unIndex].addAction(defNEW);
                connect(pouAction, SIGNAL(triggered()), this, SLOT(vOnNewSignalGroup()));
                break;
            case eNodeConfigHeaderPopup:
                pouAction = m_ouHeaderPopupMenus[unIndex].addAction(defNEW);
                connect(pouAction, SIGNAL(triggered()), this, SLOT(vOnNewNodeConfig()));
                break;
        };
    }
}

void LDFElementView::CreateElementPopupMenus()
{
    QAction* pouAction;
    for(unsigned int unIndex = eMasterElementPopup; unIndex < eTotalElementPopup; unIndex++)
    {
        switch(unIndex)
        {
            case eMasterElementPopup:
            {
                pouAction = m_ouElementPopupMenus[unIndex].addAction(defEDIT);
                connect(pouAction, SIGNAL(triggered()), this, SLOT(vOnEditMasterElement()));
                break;
            }
            case eSlaveElementPopup:
            {
                pouAction = m_ouElementPopupMenus[unIndex].addAction(defEDIT);
                connect(pouAction, SIGNAL(triggered()), this, SLOT(vOnEditSlaveElement()));

                pouAction = m_ouElementPopupMenus[unIndex].addAction(defDELETE);
                connect(pouAction, SIGNAL(triggered()), this, SLOT(vOnDeleteSlaveElement()));
                break;
            }
            case eUnconditionalFrameElementPopup:
            {
                pouAction = m_ouElementPopupMenus[unIndex].addAction(defEDIT);
                connect(pouAction, SIGNAL(triggered()), this, SLOT(vOnEditUnCondFrameElement()));

                pouAction = m_ouElementPopupMenus[unIndex].addAction(defDELETE);
                connect(pouAction, SIGNAL(triggered()), this, SLOT(vOnDeleteUnCondFrameElement()));
            }
            break;
            case eEventTriggeredFrameElementPopup:
            {
                pouAction = m_ouElementPopupMenus[unIndex].addAction(defEDIT);
                connect(pouAction, SIGNAL(triggered()), this, SLOT(vOnEditEventTrigFrameElement()));

                pouAction = m_ouElementPopupMenus[unIndex].addAction(defDELETE);
                connect(pouAction, SIGNAL(triggered()), this, SLOT(vOnDeleteEventTrigFrameElement()));
            }
            break;
            case eSporadicFrameElementPopup:
            {
                pouAction = m_ouElementPopupMenus[unIndex].addAction(defEDIT);
                connect(pouAction, SIGNAL(triggered()), this, SLOT(vOnEditSporadicFrameElement()));

                pouAction = m_ouElementPopupMenus[unIndex].addAction(defDELETE);
                connect(pouAction, SIGNAL(triggered()), this, SLOT(vOnDeleteSporadicFrameElement()));
            }
            break;
            case eSignalElementPopup:
            {
                pouAction = m_ouElementPopupMenus[unIndex].addAction(defEDIT);
                connect(pouAction, SIGNAL(triggered()), this, SLOT(vOnEditSignal()));

                pouAction = m_ouElementPopupMenus[unIndex].addAction(defDELETE);
                connect(pouAction, SIGNAL(triggered()), this, SLOT(vOnDeleteSignal()));
            }
            break;
            case eCodingElementPopup:
            {
                pouAction = m_ouElementPopupMenus[unIndex].addAction(defEDIT);
                connect(pouAction, SIGNAL(triggered()), this, SLOT(vOnEditCoding()));

                pouAction = m_ouElementPopupMenus[unIndex].addAction(defDELETE);
                connect(pouAction, SIGNAL(triggered()), this, SLOT(vOnDeleteCoding()));
            }
            break;
            case eScheduleTableElementPopup:
            {
                pouAction = m_ouElementPopupMenus[unIndex].addAction(defEDIT);
                connect(pouAction, SIGNAL(triggered()), this, SLOT(vOnEditScheduleTable()));

                pouAction = m_ouElementPopupMenus[unIndex].addAction(defDELETE);
                connect(pouAction, SIGNAL(triggered()), this, SLOT(vOnDeleteScheduleTable()));
            }
            break;
            case eDiagnosticSignalElementPopup:
                m_ouElementPopupMenus[unIndex].addAction(defEDIT);
                break;
            case eSignalGroupElementPopup:
                pouAction = m_ouElementPopupMenus[unIndex].addAction(defEDIT);
                connect(pouAction, SIGNAL(triggered()), this, SLOT(vOnEditSignalGroup()));

                pouAction = m_ouElementPopupMenus[unIndex].addAction(defDELETE);
                connect(pouAction, SIGNAL(triggered()), this, SLOT(vOnDeleteSignalGroup()));
                break;
            case eNodeConfigElementPopup:
                pouAction = m_ouElementPopupMenus[unIndex].addAction(defEDIT);
                connect(pouAction, SIGNAL(triggered()), this, SLOT(vOnEditNodeConfig()));

                pouAction = m_ouElementPopupMenus[unIndex].addAction(defDELETE);
                connect(pouAction, SIGNAL(triggered()), this, SLOT(vOnDeleteNodeConfig()));
                break;
        };
    }
}

void LDFElementView::vOnNewNodeConfig()
{
    INodeConfiguration* pouNodeConfig = nullptr;
    NodeConfigurationDlg ouDlg(&pouNodeConfig, eNew, this);
    if ( QDialog::Accepted == ouDlg.exec() && pouNodeConfig != nullptr )
    {
        vCreateNodeConfig(pouNodeConfig, currentItem());
    }
}

void LDFElementView::vCreateNodeConfig(INodeConfiguration* pNodeConfig, QTreeWidgetItem* pCurrItem)
{
    if ( nullptr == pNodeConfig )
    {
        return;
    }

    QTreeWidgetItem* pTreeNodeConfig = new QTreeWidgetItem();
    TreeItemID ouTreeItemID;
    QVariant ouVariant;

    std::string strName;
    pNodeConfig->GetName(strName);
    pTreeNodeConfig->setText(0, QString::fromStdString(strName));
    pTreeNodeConfig->setIcon(0, m_ouIcons[defECU_ICON_INDEX]);

    ouTreeItemID.m_eTreeElementType = eLdfItem;
    ouTreeItemID.m_uidElement = pNodeConfig->GetUniqueId();
    m_UidTreeWidgetMap[ouTreeItemID.m_uidElement] = pTreeNodeConfig;
    ouVariant.setValue(ouTreeItemID);
    pTreeNodeConfig->setData(0, Qt::UserRole, ouVariant);

    pCurrItem->addChild(pTreeNodeConfig);
    pCurrItem->sortChildren(0, Qt::AscendingOrder);
    LDFDatabaseManager::GetDatabaseManager()->setDocumentModified(true);
    emit ElementCreated(pNodeConfig->GetUniqueId());
}

void LDFElementView::vOnEditNodeConfig()
{
    QTreeWidgetItem* pItem = currentItem();
    if ( nullptr != pItem )
    {
        TreeItemID ouTreeItemId = pItem->data(0, Qt::UserRole).value<TreeItemID>();
        INodeConfiguration* pouNodeConfig = nullptr;
        m_pBaseCluster->GetElement(eLinNodeComposition, ouTreeItemId.m_uidElement, (IElement**)&pouNodeConfig);
        if ( nullptr != pouNodeConfig )
        {
            pouNodeConfig->RegisterForChangeNotification(this);
            NodeConfigurationDlg ouDlg(&pouNodeConfig, eEdit, this);
            if ( QDialog::Accepted == ouDlg.exec() && pouNodeConfig != nullptr )
            {
                SelectElement(ouTreeItemId);
                emit ElementCreated(pouNodeConfig->GetUniqueId());
            }
        }
    }
}

void LDFElementView::vOnDeleteNodeConfig()
{
    INodeConfiguration* pouNodeConfig = nullptr;
    QTreeWidgetItem* pItem = currentItem();
    if ( nullptr != pItem  )
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Delete?", "Do you Really Want to Delete Item", QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::No)
        {
            return;
        }
        TreeItemID uid = pItem->data(0, Qt::UserRole).value<TreeItemID>();
        m_pBaseCluster->GetElement(eLinNodeComposition, uid.m_uidElement, (IElement**)&pouNodeConfig);
        if ( nullptr != pouNodeConfig )
        {

            auto itr = m_UidTreeWidgetMap.find(uid.m_uidElement);
            if ( itr != m_UidTreeWidgetMap.end() )
            {
                itr->second->parent()->removeChild(itr->second);
                m_UidTreeWidgetMap.erase(itr);
                //emit ElementDeleted(uid.m_uidElement);
            }
            LDFDatabaseManager::GetDatabaseManager()->setDocumentModified(true);
            m_pBaseCluster->DeleteElement(eLinNodeComposition, uid.m_uidElement);
        }
    }
}

void LDFElementView::vOnNewCoding()
{
    ICoding* pouCoding = nullptr;
    CodingDlg ouDlg(m_pBaseCluster, &pouCoding, true, this);
    if ( QDialog::Accepted == ouDlg.exec() && pouCoding != nullptr )
    {
        vCreateCoding(pouCoding, currentItem());
    }
}

void LDFElementView::vOnNewScheduleTable()
{
    IScheduleTable* pouShedTable = nullptr;
    ScheduleTableDlg ouDlg(&pouShedTable, eNew, this);
    if ( QDialog::Accepted == ouDlg.exec() && pouShedTable != nullptr )
    {
        vCreateScheduleTable(pouShedTable, currentItem());
    }
}

void LDFElementView::vOnEditScheduleTable()
{
    QTreeWidgetItem* pItem = currentItem();
    if ( nullptr != pItem )
    {
        TreeItemID ouTreeItemId = pItem->data(0, Qt::UserRole).value<TreeItemID>();
        IScheduleTable* pouScheduleTable = nullptr;
        m_pBaseCluster->GetElement(eScheduleTableElement, ouTreeItemId.m_uidElement, (IElement**)&pouScheduleTable);
        if ( nullptr != pouScheduleTable )
        {
            pouScheduleTable->RegisterForChangeNotification(this);
            ScheduleTableDlg ouDlg(&pouScheduleTable, eEdit, this);
            if ( QDialog::Accepted == ouDlg.exec() && pouScheduleTable != nullptr )
            {
                SelectElement(ouTreeItemId);
                LDFDatabaseManager::GetDatabaseManager()->setDocumentModified(true);
                emit ElementCreated(pouScheduleTable->GetUniqueId());
            }
        }
    }
}

void LDFElementView::vOnEditSignalGroup()
{
    QTreeWidgetItem* pItem = currentItem();
    if ( nullptr != pItem )
    {
        TreeItemID ouTreeItemId = pItem->data(0, Qt::UserRole).value<TreeItemID>();
        ISignalGroup* pouScheduleTable = nullptr;
        m_pBaseCluster->GetElement(eSignalGroupElement, ouTreeItemId.m_uidElement, (IElement**)&pouScheduleTable);
        if ( nullptr != pouScheduleTable )
        {
            pouScheduleTable->RegisterForChangeNotification(this);
            SignalGroupDlg ouDlg(m_pBaseCluster, &pouScheduleTable, false, this);
            if ( QDialog::Accepted == ouDlg.exec() && pouScheduleTable != nullptr )
            {
                SelectElement(ouTreeItemId);
                LDFDatabaseManager::GetDatabaseManager()->setDocumentModified(true);
                emit ElementCreated(pouScheduleTable->GetUniqueId());
            }
        }
    }
}

void LDFElementView::vOnDeleteSignalGroup()
{
    IElement* pouElement = nullptr;
    QTreeWidgetItem* pItem = currentItem();
    if ( nullptr != pItem  )
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Delete?", "Do you Really Want to Delete Item", QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::No)
        {
            return;
        }
        TreeItemID uid = pItem->data(0, Qt::UserRole).value<TreeItemID>();
        m_pBaseCluster->GetElement(eSignalGroupElement, uid.m_uidElement, (IElement**)&pouElement);
        if ( nullptr != pouElement )
        {

            auto itr = m_UidTreeWidgetMap.find(uid.m_uidElement);
            if ( itr != m_UidTreeWidgetMap.end() )
            {
                itr->second->parent()->removeChild(itr->second);
                m_UidTreeWidgetMap.erase(itr);
            }
            LDFDatabaseManager::GetDatabaseManager()->setDocumentModified(true);
            m_pBaseCluster->DeleteElement(eSignalGroupElement, uid.m_uidElement);
        }
    }
}

void LDFElementView::vOnNewSignalGroup()
{
    ISignalGroup* pouSignalGroup = nullptr;
    SignalGroupDlg ouDlg(m_pBaseCluster, &pouSignalGroup, true, this);
    if ( QDialog::Accepted == ouDlg.exec() && pouSignalGroup != nullptr )
    {
        vCreateSignalGroup(pouSignalGroup, currentItem());
    }
}



void LDFElementView::vOnDeleteScheduleTable()
{
    IElement* pouElement = nullptr;
    QTreeWidgetItem* pItem = currentItem();
    if ( nullptr != pItem  )
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Delete?", "Do you Really Want to Delete Item", QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::No)
        {
            return;
        }
        TreeItemID uid = pItem->data(0, Qt::UserRole).value<TreeItemID>();
        m_pBaseCluster->GetElement(eScheduleTableElement, uid.m_uidElement, (IElement**)&pouElement);
        if ( nullptr != pouElement )
        {

            auto itr = m_UidTreeWidgetMap.find(uid.m_uidElement);
            if ( itr != m_UidTreeWidgetMap.end() )
            {
                itr->second->parent()->removeChild(itr->second);
                m_UidTreeWidgetMap.erase(itr);
            }
            LDFDatabaseManager::GetDatabaseManager()->setDocumentModified(true);
            m_pBaseCluster->DeleteElement(eScheduleTableElement, uid.m_uidElement);
        }
    }
}
void LDFElementView::vCreateSignalGroup(ISignalGroup* pouSignalGroup, QTreeWidgetItem* pouParentTree)
{
    if ( nullptr == pouSignalGroup )
    {
        return;
    }

    QTreeWidgetItem* pCodingItem = new QTreeWidgetItem();
    TreeItemID ouTreeItemID;
    QVariant ouVariant;

    std::string strName;
    pouSignalGroup->GetName(strName);
    pCodingItem->setText(0, QString::fromStdString(strName));
    pCodingItem->setIcon(0, m_ouIcons[defSIGNAL_ICON_INDEX]);

    ouTreeItemID.m_eTreeElementType = eLdfItem;
    ouTreeItemID.m_uidElement = pouSignalGroup->GetUniqueId();
    m_UidTreeWidgetMap[ouTreeItemID.m_uidElement] = pCodingItem;
    ouVariant.setValue(ouTreeItemID);
    pCodingItem->setData(0, Qt::UserRole, ouVariant);

    pouParentTree->addChild(pCodingItem);
    pouParentTree->sortChildren(0, Qt::AscendingOrder);
    LDFDatabaseManager::GetDatabaseManager()->setDocumentModified(true);
    emit ElementCreated(pouSignalGroup->GetUniqueId());
}


void LDFElementView::vCreateScheduleTable(IScheduleTable* pouSchedTable, QTreeWidgetItem* pouParentTree)
{
    if ( nullptr == pouSchedTable )
    {
        return;
    }

    QTreeWidgetItem* pCodingItem = new QTreeWidgetItem();
    TreeItemID ouTreeItemID;
    QVariant ouVariant;

    std::string strName;
    pouSchedTable->GetName(strName);
    pCodingItem->setText(0, QString::fromStdString(strName));
    pCodingItem->setIcon(0, m_ouIcons[defSCHEDULETABLE_ICON_INDEX]);

    ouTreeItemID.m_eTreeElementType = eLdfItem;
    ouTreeItemID.m_uidElement = pouSchedTable->GetUniqueId();
    m_UidTreeWidgetMap[ouTreeItemID.m_uidElement] = pCodingItem;
    ouVariant.setValue(ouTreeItemID);
    pCodingItem->setData(0, Qt::UserRole, ouVariant);

    pouParentTree->addChild(pCodingItem);
    pouParentTree->sortChildren(0, Qt::AscendingOrder);
    LDFDatabaseManager::GetDatabaseManager()->setDocumentModified(true);
    emit ElementCreated(pouSchedTable->GetUniqueId());
}

void LDFElementView::vOnEditCoding()
{
    QTreeWidgetItem* pItem = currentItem();
    if ( nullptr != pItem )
    {
        TreeItemID ouTreeItemId = pItem->data(0, Qt::UserRole).value<TreeItemID>();
        ICoding* pouCoding = nullptr;
        m_pBaseCluster->GetElement(eCodingElement, ouTreeItemId.m_uidElement, (IElement**)&pouCoding);
        if ( nullptr != pouCoding )
        {
            pouCoding->RegisterForChangeNotification(this);
            CodingDlg ouDlg(m_pBaseCluster, &pouCoding, false, this);
            if ( QDialog::Accepted == ouDlg.exec() && pouCoding != nullptr )
            {
                SelectElement(ouTreeItemId);
                LDFDatabaseManager::GetDatabaseManager()->setDocumentModified(true);
                emit ElementCreated(pouCoding->GetUniqueId());
            }
        }
    }
}


void LDFElementView::vOnDeleteCoding()
{
    ICoding* pouCoding = nullptr;
    QTreeWidgetItem* pItem = currentItem();
    if ( nullptr != pItem  )
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Delete?", "Do you Really Want to Delete Item", QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::No)
        {
            return;
        }
        TreeItemID uid = pItem->data(0, Qt::UserRole).value<TreeItemID>();
        m_pBaseCluster->GetElement(eCodingElement, uid.m_uidElement, (IElement**)&pouCoding);
        if ( nullptr != pouCoding )
        {

            auto itr = m_UidTreeWidgetMap.find(uid.m_uidElement);
            if ( itr != m_UidTreeWidgetMap.end() )
            {
                itr->second->parent()->removeChild(itr->second);
                m_UidTreeWidgetMap.erase(itr);
                emit ElementDeleted(uid.m_uidElement);
            }
            LDFDatabaseManager::GetDatabaseManager()->setDocumentModified(true);
            m_pBaseCluster->DeleteElement(eCodingElement, uid.m_uidElement);
        }
    }
}

void LDFElementView::vOnNewSignal()
{
    ISignal* pouSignal = nullptr;
    SignalDlg ouDlg(m_pBaseCluster, &pouSignal, true, this);
    if ( QDialog::Accepted == ouDlg.exec() && pouSignal != nullptr )
    {
        vCreateSignal(pouSignal, currentItem());
    }
}

void LDFElementView::vOnEditSignal()
{
    QTreeWidgetItem* pItem = currentItem();
    if ( nullptr != pItem )
    {
        TreeItemID ouTreeItemId = pItem->data(0, Qt::UserRole).value<TreeItemID>();
        ISignal* pouSignal = nullptr;
        m_pBaseCluster->GetElement(eSignalElement, ouTreeItemId.m_uidElement, (IElement**)&pouSignal);
        if ( nullptr != pouSignal )
        {
            pouSignal->RegisterForChangeNotification(this);
            SignalDlg ouDlg(m_pBaseCluster, &pouSignal, false, this);
            if ( QDialog::Accepted == ouDlg.exec() && pouSignal != nullptr )
            {
                LDFDatabaseManager::GetDatabaseManager()->setDocumentModified(true);
                emit ElementCreated(pouSignal->GetUniqueId());
                SelectElement(ouTreeItemId);
            }
        }
    }
}

void LDFElementView::vOnDeleteSignal()
{
    ISignal* pouSignal = nullptr;
    QTreeWidgetItem* pItem = currentItem();
    if ( nullptr != pItem  )
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Delete?", "Do you Really Want to Delete Item", QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::No)
        {
            return;
        }
        TreeItemID uid = pItem->data(0, Qt::UserRole).value<TreeItemID>();
        m_pBaseCluster->GetElement(eSignalElement, uid.m_uidElement, (IElement**)&pouSignal);
        if ( nullptr != pouSignal )
        {

            auto itr = m_UidTreeWidgetMap.find(uid.m_uidElement);
            if ( itr != m_UidTreeWidgetMap.end() )
            {
                itr->second->parent()->removeChild(itr->second);
                m_UidTreeWidgetMap.erase(itr);
                //emit ElementDeleted(uid.m_uidElement);
            }
            LDFDatabaseManager::GetDatabaseManager()->setDocumentModified(true);
            m_pBaseCluster->DeleteElement(eSignalElement, uid.m_uidElement);
            emit ElementCreated(uid.m_uidElement);
        }
    }
}

void LDFElementView::vOnEditLinCluster()
{
    LDFCLusterPropsDlg dlg(m_pBaseCluster, this);
    if ( QDialog::Accepted == dlg.exec() )
    {
        setItemSelected(currentItem(), false);
        setItemSelected(currentItem(), true);
        LDFDatabaseManager::GetDatabaseManager()->setDocumentModified(true);
    }
}


void LDFElementView::vOnEditMasterElement()
{
    if ( selectedItems().size() > 0 )
    {
        QTreeWidgetItem* item = selectedItems().at(0);
        TreeItemID ouItemId = item->data(0, Qt::UserRole).value<TreeItemID>();
        IElement* pIECu;

        m_pouLDFDatabaseManager->GetLDFCluster()->GetElement(eEcuElement, ouItemId.m_uidElement, &pIECu);
        pIECu->RegisterForChangeNotification(this);
        if ( nullptr != pIECu )
        {
            MasterEditDlg ouDlg((IEcu*)pIECu, this);
            if (QDialog::Accepted == ouDlg.exec() )
            {
                LDFDatabaseManager::GetDatabaseManager()->setDocumentModified(true);
                SelectElement(ouItemId);
            }
        }
    }
}

ERRORCODE LDFElementView::OnClusterChange(eAction eActionType, UID_ELEMENT nId, eClusterElementType& ouElementType, void* pAcionData, void* pUserData)
{
    switch (ouElementType)
    {
        case eEcuElement:
        case eSignalElement:
        case eCodingElement:
        case eFrameElement:
        case eScheduleTableElement:
        case eSignalGroupElement:
        case eLinNodeComposition:
            vHandleElementChange(eActionType, nId, pAcionData);
        default:
            break;
    }
    return EC_NA;
}

void LDFElementView::vHandleElementChange(eAction eActionType, UID_ELEMENT nId, void* pAcionData)
{
    if ( eDeleted == eActionType ||  eBeforeDeletion == eActionType )
    {
    }
    else
    {
        IElement* pElement;

        std::string strName;
        TreeItemID uidTreeElement;
        m_pBaseCluster->GetElement(m_pBaseCluster->GetElementType(nId), nId, &pElement);
        if ( nullptr != pElement )
        {
            pElement->GetName(strName);
            m_UidTreeWidgetMap[nId]->setText(0, QString::fromStdString(strName));

            uidTreeElement.m_eTreeElementType = eLdfItem;
            uidTreeElement.m_uidElement = nId;
            SelectElement(uidTreeElement);
        }
    }
    return;
}
void LDFElementView::vCreateCoding(ICoding* pouCoding, QTreeWidgetItem* pouParentTree)
{
    if ( nullptr == pouCoding )
    {
        return;
    }

    QTreeWidgetItem* pCodingItem = new QTreeWidgetItem();
    TreeItemID ouTreeItemID;
    QVariant ouVariant;

    std::string strName;
    pouCoding->GetName(strName);
    pCodingItem->setText(0, QString::fromStdString(strName));
    pCodingItem->setIcon(0, m_ouIcons[defCODING_ICON_INDEX]);

    ouTreeItemID.m_eTreeElementType = eLdfItem;
    ouTreeItemID.m_uidElement = pouCoding->GetUniqueId();
    m_UidTreeWidgetMap[ouTreeItemID.m_uidElement] = pCodingItem;
    ouVariant.setValue(ouTreeItemID);
    pCodingItem->setData(0, Qt::UserRole, ouVariant);

    pouParentTree->addChild(pCodingItem);
    pouParentTree->sortChildren(0, Qt::AscendingOrder);
    LDFDatabaseManager::GetDatabaseManager()->setDocumentModified(true);
    emit ElementCreated(pouCoding->GetUniqueId());
}


void LDFElementView::vCreateSignal(ISignal* pouSignal, QTreeWidgetItem* pouParentTree)
{
    if ( nullptr == pouSignal )
    {
        return;
    }

    QTreeWidgetItem* pSignalItem = new QTreeWidgetItem();
    TreeItemID ouTreeItemID;
    QVariant ouVariant;

    std::string strName;
    pouSignal->GetName(strName);
    pSignalItem->setText(0, QString::fromStdString(strName));
    pSignalItem->setIcon(0, m_ouIcons[defSIGNAL_ICON_INDEX]);

    ouTreeItemID.m_eTreeElementType = eLdfItem;
    ouTreeItemID.m_uidElement = pouSignal->GetUniqueId();
    m_UidTreeWidgetMap[ouTreeItemID.m_uidElement] = pSignalItem;
    ouVariant.setValue(ouTreeItemID);
    pSignalItem->setData(0, Qt::UserRole, ouVariant);

    pouParentTree->addChild(pSignalItem);
    pouParentTree->sortChildren(0, Qt::AscendingOrder);
    LDFDatabaseManager::GetDatabaseManager()->setDocumentModified(true);
    emit ElementCreated(pouSignal->GetUniqueId());
}


void LDFElementView::vCreateSlaveEcu(IEcu* pouEcu, QTreeWidgetItem* pouParentTree)
{
    VALIDATE_POINTER(pouEcu);

    QTreeWidgetItem* pSlaveWidget = new QTreeWidgetItem();
    std::string strName = "";
    TreeItemID ouTreeItemID;
    QVariant ouVariant;
    pouEcu->GetName(strName);


    pSlaveWidget->setText(0, QString::fromStdString(strName));
    pSlaveWidget->setIcon(0, m_ouIcons[defECU_ICON_INDEX]);

    ouTreeItemID.m_eTreeElementType = eLdfItem;

    ouTreeItemID.m_uidElement = pouEcu->GetUniqueId();
    m_UidTreeWidgetMap[ouTreeItemID.m_uidElement] = pSlaveWidget;
    ouVariant.setValue(ouTreeItemID);
    pSlaveWidget->setData(0, Qt::UserRole, ouVariant);

    pouParentTree->addChild(pSlaveWidget);
    pouParentTree->sortChildren(0, Qt::AscendingOrder);
    m_UidTreeWidgetMap[pouEcu->GetUniqueId()] = pSlaveWidget;



    UID_ELEMENT uidEcu = pouEcu->GetUniqueId();
    UID_ELEMENT uidFrame = INVALID_DATA;



    std::map <UID_ELEMENT, IElement*> pouFrameList;
    m_pBaseCluster->GetElementList(eFrameElement, pouFrameList);

    eFrameType ouFrameType;
    UID_ELEMENT uId;
    unsigned int unFramId = 0;
    unsigned int unMasterFrameId = 0x3C;
    unsigned int unSlaveFrameId = 0x3D;
for ( auto itr : pouFrameList )
    {
        ((IFrame*)itr.second)->GetFrameId(unFramId);
        if ( unMasterFrameId == unFramId )
        {
            //add master frame as rx
            ((IFrame*)itr.second)->MapNode(eRx, uidEcu);
            uidFrame = ((IFrame*)itr.second)->GetUniqueId();
            pouEcu->MapFrame(eRx, uidFrame);
            MapNodeToSignals(((IFrame*)itr.second), pouEcu, eRx);
        }
        else if ( unSlaveFrameId == unFramId )
        {
            //add slave as tx
            ((IFrame*)itr.second)->MapNode(eTx, uidEcu);
            uidFrame = ((IFrame*)itr.second)->GetUniqueId();
            pouEcu->MapFrame(eTx, uidFrame);
            MapNodeToSignals(((IFrame*)itr.second), pouEcu, eTx);
        }

    }
    LDFDatabaseManager::GetDatabaseManager()->setDocumentModified(true);
    emit ElementCreated(pouEcu->GetUniqueId());
}

void LDFElementView::MapNodeToSignals(IFrame* pouFrame, IEcu* pouEcu, eDIR oueDir)
{
    VALIDATE_POINTER(pouFrame);
    VALIDATE_POINTER(pouEcu);

    UID_ELEMENT uidEcu = pouEcu->GetUniqueId();
    UID_ELEMENT uidSignal = INVALID_UID_ELEMENT;

    std::map<ISignal*, SignalInstanse> ouSignalList;
    pouFrame->GetSignalList(ouSignalList);
for ( auto itrSignal : ouSignalList )
    {
        uidSignal = itrSignal.first->GetUniqueId();
        itrSignal.first->MapNode( oueDir, uidEcu );
        pouEcu->MapSignal(oueDir, uidSignal);
    }
}

void LDFElementView::vOnNewSlave()
{
    IEcu* pouEcu = nullptr;
    SlaveDlg ouDlg(m_pBaseCluster, &pouEcu, true, this);
    if ( QDialog::Accepted == ouDlg.exec() && pouEcu != nullptr )
    {
        vCreateSlaveEcu(pouEcu, currentItem());
    }
}

void LDFElementView::vPopulateDiagSignals()
{
    QList<QTreeWidgetItem*> LinElementItem[LIN_MAX_SIGNAL_TYPES];
    std::string strName;
    SignalProps ouSignalProps;
    TreeItemID ouTreeItemID;
    QVariant ouVariant;
    std::map< UID_ELEMENT, IElement*> ouElementMap;
    m_pBaseCluster->GetElementList(eSignalElement, ouElementMap);
    QTreeWidgetItem* pElementItem;
    ouTreeItemID.m_eTreeElementType = eLdfItem;

for ( auto itrSignal : ouElementMap )
    {
        itrSignal.second->GetName(strName);

        ((ISignal*)itrSignal.second)->GetProperties(ouSignalProps);

        if (ouSignalProps.m_ouLINSignalProps.m_ouSignalType == eSignalDiag)
        {
            pElementItem = new QTreeWidgetItem();
            pElementItem->setText(0, QString::fromStdString(strName));
            pElementItem->setIcon(0, m_ouIcons[defSIGNAL_ICON_INDEX]);


            ouTreeItemID.m_uidElement = itrSignal.second->GetUniqueId();
            m_UidTreeWidgetMap[ouTreeItemID.m_uidElement] = pElementItem;
            ouVariant.setValue(ouTreeItemID);
            pElementItem->setData(0, Qt::UserRole, ouVariant);

            m_pDiagElementTopTreeItem[1]->addChild(pElementItem);
        }
    }
    m_pDiagElementTopTreeItem[1]->sortChildren(0, Qt::AscendingOrder);

}


void LDFElementView::vOnAddDiagSupport()
{
    IFrame* pouFrame;
    QTreeWidgetItem* pouTreeItem;

    CreateMasterDiagFrame(pouFrame, m_pBaseCluster);
    nGetDigFrameTreeWidgetItem(pouFrame, pouTreeItem);
    currentItem()->addChild(pouTreeItem);

    CreateSlaveDiagFrame(pouFrame, m_pBaseCluster);
    nGetDigFrameTreeWidgetItem(pouFrame, pouTreeItem);
    currentItem()->addChild(pouTreeItem);

    vPopulateDiagSignals();
    LDFDatabaseManager::GetDatabaseManager()->setDocumentModified(true);
    emit ElementCreated(pouFrame->GetUniqueId());

    m_bDiagFrames[0] = true;
    m_bDiagFrames[1] = true;

}

void LDFElementView::vOnRemoveDiagSupport()
{
    m_bDiagFrames[0] = false;
    m_bDiagFrames[1] = false;
    std::map<UID_ELEMENT, IElement*> pouFrameList;
    m_pBaseCluster->GetElementList(eFrameElement, pouFrameList);

    eFrameType ouFrameType;
    UID_ELEMENT uId;
for ( auto itr : pouFrameList )
    {
        ((IFrame*)itr.second)->GetFrameType(ouFrameType);
        if ( eLIN_Diagnostic == ouFrameType )
        {
            uId = itr.second->GetUniqueId();
            vDeleteSignals((IFrame*)itr.second);
            m_pBaseCluster->DeleteElement(eFrameElement, uId);
            auto itr = m_UidTreeWidgetMap.find(uId);
            if ( itr != m_UidTreeWidgetMap.end())
            {
                QTreeWidgetItem* pParent =  itr->second->parent();
                if ( nullptr != pParent )
                {
                    pParent->removeChild(itr->second);
                }
            }

            emit ElementDeleted(uId);
        }
    }
    LDFDatabaseManager::GetDatabaseManager()->setDocumentModified(true);
}

void LDFElementView::vDeleteSignals(IFrame* pouFrame)
{
    std::map<ISignal*, SignalInstanse> ouSignalMap;
    pouFrame->GetSignalList(ouSignalMap);
    UID_ELEMENT uid = INVALID_UID_ELEMENT;
for ( auto itr : ouSignalMap )
    {
        uid = itr.first->GetUniqueId();
        m_pBaseCluster->DeleteElement(eSignalElement, uid);
        emit ElementDeleted(uid);
        auto itr2 = m_UidTreeWidgetMap.find(uid);
        if ( itr2 != m_UidTreeWidgetMap.end())
        {
            itr2->second->parent()->removeChild(itr2->second);
        }
    }
}


void LDFElementView::vOnEditSlaveElement()
{
    IEcu* pouEcu = nullptr;
    QTreeWidgetItem* pItem = currentItem();
    if ( nullptr != pItem  )
    {
        TreeItemID uid = pItem->data(0, Qt::UserRole).value<TreeItemID>();
        m_pBaseCluster->GetElement(eEcuElement, uid.m_uidElement, (IElement**)&pouEcu);
        if ( nullptr != pouEcu )
        {
            pouEcu->RegisterForChangeNotification(this);
            SlaveDlg ouDlg(m_pBaseCluster, &pouEcu, false, this);
            if ( QDialog::Accepted == ouDlg.exec() && pouEcu != nullptr )
            {
                setItemSelected(pItem, false);
                setItemSelected(pItem, true);
                SelectElement(uid);
            }
        }
    }



}

void LDFElementView::vOnDeleteSlaveElement()
{
    IEcu* pouEcu = nullptr;
    QTreeWidgetItem* pItem = currentItem();
    if ( nullptr != pItem  )
    {
        QMessageBox::StandardButton reply;
        if(pItem->parent()->childCount() <= 1)
        {
            reply = QMessageBox::critical(this, "Delete?", "Alteast one Slave should exist");
            return;
        }


        reply = QMessageBox::question(this, "Delete?", "All frames and signals mapped to the Slave will be deleted. Do you want to proceed?", QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::No)
        {
            return;
        }
        TreeItemID uid = pItem->data(0, Qt::UserRole).value<TreeItemID>();
        m_pBaseCluster->GetElement(eEcuElement, uid.m_uidElement, (IElement**)&pouEcu);

        OnSlaveNodeDelete(uid.m_uidElement);
        if ( nullptr != pouEcu )
        {

            auto itr = m_UidTreeWidgetMap.find(uid.m_uidElement);
            if ( itr != m_UidTreeWidgetMap.end() )
            {
                itr->second->parent()->removeChild(itr->second);
                emit ElementDeleted(uid.m_uidElement);
            }
            LDFDatabaseManager::GetDatabaseManager()->setDocumentModified(true);
            m_pBaseCluster->DeleteElement(eEcuElement, uid.m_uidElement);
        }

    }
}
void LDFElementView::onItemDoubleClicked(QTreeWidgetItem* pTreeItem, int column)
{
    if ( nullptr != pTreeItem)
    {
        QVariant qVar = pTreeItem->data(0, Qt::UserRole);

        TreeItemID objTreeItemData = qVar.value<TreeItemID>();

        if ( eLdfItem == objTreeItemData.m_eTreeElementType)
        {
            IElement* pElement;
            eClusterElementType eType = m_pBaseCluster->GetElementType(objTreeItemData.m_uidElement);
            m_pBaseCluster->GetElement(eType, objTreeItemData.m_uidElement, &pElement);

            switch (eType)
            {
                case eFrameElement:
                {
                    eFrameType oueFrameType;
                    ((IFrame*)pElement)->GetFrameType(oueFrameType);
                    if ( oueFrameType == eLIN_Unconditional)
                    {
                        vOnEditUnCondFrameElement();
                    }
                    else if (oueFrameType == eLIN_EventTriggered)
                    {
                        vOnEditEventTrigFrameElement();
                    }
                    else if (oueFrameType == eLIN_Sporadic)
                    {
                        vOnEditSporadicFrameElement();
                    }
                }
                break;
                case eSignalElement:
                {
                    SignalProps ouSignalProps;
                    ((ISignal*)pElement)->GetProperties(ouSignalProps);
                    if ( ouSignalProps.m_ouLINSignalProps.m_ouSignalType == eSignalNormal )
                    {
                        vOnEditSignal();
                    }
                }
                break;
                case eEcuElement:
                {
                    eEcuType ouEcuType;
                    ((IEcu*)pElement)->GetEcuType(ouEcuType);
                    if ( ouEcuType == eLIN_Master )
                    {
                        vOnEditMasterElement();
                    }
                    else if ( ouEcuType == eLIN_Slave )
                    {
                        vOnEditSlaveElement();
                    }
                }
                break;
                case eCodingElement:
                    vOnEditCoding();
                    break;
                case eScheduleTableElement:
                    vOnEditScheduleTable();
                    break;
                case eSignalGroupElement:
                    vOnEditSignalGroup();
                    break;
                case eLinNodeComposition:
                    vOnEditNodeConfig();
                    break;
                default:
                    break;
            }
        }
        else
        {
            if ( defTREE_ITEM_LINCLUSTER == objTreeItemData.m_uidElement )
            {
                vOnEditLinCluster();
            }
        }
    }
}

void LDFElementView::onCustomContextMenu(const QPoint& point)
{
    QTreeWidgetItem* pTreeItem = itemAt(point);

    if ( nullptr != pTreeItem)
    {
        if ( currentItem() != nullptr )
        {
            setItemSelected(currentItem(), false);
        }

        setCurrentItem(pTreeItem);
        setItemSelected(pTreeItem, true);
        QVariant qVar = pTreeItem->data(0, Qt::UserRole);

        TreeItemID objTreeItemData = qVar.value<TreeItemID>();

        switch(objTreeItemData.m_eTreeElementType)
        {
            case eTreeHeader:
                DisplayHeaderPopupMenu(viewport()->mapToGlobal(point), pTreeItem, objTreeItemData.m_uidElement);
                break;
            case eLdfItem:
                DisplayElementPopupMenu(viewport()->mapToGlobal(point), pTreeItem, objTreeItemData.m_uidElement);
                break;
        };
    }
}

void LDFElementView::DisplayHeaderPopupMenu(const QPoint& point, QTreeWidgetItem* pTreeItem, UID_ELEMENT uidElement)
{
    switch(uidElement)
    {
        case defTREE_ITEM_LINCLUSTER:
            showContextMenu(m_ouHeaderPopupMenus[eLinClusterHeaderPopup],pTreeItem, point);
            break;
        case defTREE_ITEM_MASTER:
            break;
        case defTREE_ITEM_SLAVE:
            showContextMenu(m_ouHeaderPopupMenus[eSlaveHeaderPopup],pTreeItem, point);
            break;
        case defTRRE_ITEM_UNCONDITIONAL:
            showContextMenu(m_ouHeaderPopupMenus[eUnconditionalFrameHeaderPopup],pTreeItem, point);
            break;
        case defTREE_ITEM_SPORADIC:
            showContextMenu(m_ouHeaderPopupMenus[eSporadicFrameHeaderPopup],pTreeItem, point);
            break;
        case defTREE_ITEM_EVENT_TRIGGERED:
            showContextMenu(m_ouHeaderPopupMenus[eEventTriggeredFrameHeaderPopup],pTreeItem, point);
            break;
        case defTREE_ITEM_DIAGNOSTIC:
        {
            QMenu ouMenu;
            if ( m_bDiagFrames[0] == false || m_bDiagFrames[0] == false )
            {
                ouMenu.addAction(m_ouHeaderPopupMenus[eDiagnosticFrameHeaderPopup].actions().at(0));
            }
            else
            {
                ouMenu.addAction(m_ouHeaderPopupMenus[eDiagnosticFrameHeaderPopup].actions().at(1));
            }
            showContextMenu(ouMenu,pTreeItem, point);
        }
        break;
        case defTREE_ITEM_NORMAL_SIGNAL:
            showContextMenu(m_ouHeaderPopupMenus[eSignalHeaderPopup],pTreeItem, point);
            break;
        case defTREE_ITEM_DIAG_SIGNAL:
            break;
        case defTREE_ITEM_CODING:
            showContextMenu(m_ouHeaderPopupMenus[eCodingHeaderPopup],pTreeItem, point);
            break;
        case defTREE_ITEM_SCHEDULETABLE:
            showContextMenu(m_ouHeaderPopupMenus[eScheduleTableHeaderPopup],pTreeItem, point);
            break;
        case defTREE_ITEM_SIGNALGROUP:
            showContextMenu(m_ouHeaderPopupMenus[eSignalGroupHeaderPopUp],pTreeItem, point);
            break;
        case defTREE_ITEM_NODECOMP:
            showContextMenu(m_ouHeaderPopupMenus[eNodeConfigHeaderPopup],pTreeItem, point);
            break;
    };
}

void LDFElementView::DisplayElementPopupMenu(const QPoint& point, QTreeWidgetItem* pTreeItem, UID_ELEMENT uidElement)
{
    eClusterElementType eType = m_pouLDFDatabaseManager->GetLDFCluster()->GetElementType(uidElement);
    IElement* pElement = nullptr;
    EcuProperties ouEcuProps;
    FrameProps ouFrameProps;
    SignalProps ouSignalProps;

    switch(eType)
    {
        case eEcuElement:

            m_pouLDFDatabaseManager->GetLDFCluster()->GetElement(eEcuElement, uidElement, &pElement);

            if(nullptr != pElement)
            {
                if(EC_SUCCESS == ((IEcu*)pElement)->GetProperties(ouEcuProps))
                {
                    if(ouEcuProps.m_eEcuType == eLIN_Master)
                    {
                        showContextMenu(m_ouElementPopupMenus[eMasterElementPopup],pTreeItem, point);
                    }
                    else if(ouEcuProps.m_eEcuType == eLIN_Slave)
                    {
                        showContextMenu(m_ouElementPopupMenus[eSlaveElementPopup],pTreeItem, point);
                    }
                }
            }
            break;
        case eFrameElement:
            m_pouLDFDatabaseManager->GetLDFCluster()->GetElement(eFrameElement, uidElement, &pElement);

            if(nullptr != pElement)
            {
                if(EC_SUCCESS == ((IFrame*)pElement)->GetProperties(ouFrameProps))
                {
                    if(ouFrameProps.m_eFrameType == eLIN_Unconditional)
                    {
                        showContextMenu(m_ouElementPopupMenus[eUnconditionalFrameElementPopup],pTreeItem, point);
                    }
                    else if(ouFrameProps.m_eFrameType == eLIN_EventTriggered)
                    {
                        showContextMenu(m_ouElementPopupMenus[eEventTriggeredFrameElementPopup],pTreeItem, point);
                    }
                    else if(ouFrameProps.m_eFrameType == eLIN_Sporadic)
                    {
                        showContextMenu(m_ouElementPopupMenus[eSporadicFrameElementPopup],pTreeItem, point);
                    }
                }
            }

            break;
        case eSignalElement:

            m_pouLDFDatabaseManager->GetLDFCluster()->GetElement(eSignalElement, uidElement, &pElement);

            if(nullptr != pElement)
            {
                if(EC_SUCCESS == ((ISignal*)pElement)->GetProperties(ouSignalProps))
                {
                    if(ouSignalProps.m_ouLINSignalProps.m_ouSignalType == eSignalNormal)
                    {
                        showContextMenu(m_ouElementPopupMenus[eSignalElementPopup],pTreeItem, point);
                    }
                }
            }
            break;
        case eCodingElement:
            showContextMenu(m_ouElementPopupMenus[eCodingElementPopup],pTreeItem, point);
            break;
        case eScheduleTableElement:
            showContextMenu(m_ouElementPopupMenus[eScheduleTableElementPopup],pTreeItem, point);
            break;
        case eSignalGroupElement:
            showContextMenu(m_ouElementPopupMenus[eSignalGroupElementPopup],pTreeItem, point);
            break;
        case eLinNodeComposition:
            showContextMenu(m_ouElementPopupMenus[eNodeConfigElementPopup],pTreeItem, point);
            break;
    };
}

void LDFElementView::vOnEditUnCondFrameElement()
{
    if ( selectedItems().size() > 0 )
    {
        QTreeWidgetItem* item = selectedItems().at(0);
        TreeItemID ouItemId = item->data(0, Qt::UserRole).value<TreeItemID>();
        IFrame* pIFrame;
        m_pouLDFDatabaseManager->GetLDFCluster()->GetElement(eFrameElement, ouItemId.m_uidElement, (IElement**)&pIFrame);
        if ( nullptr != pIFrame )
        {
            UnconditionalFrameEditDlg ouDlg(&pIFrame, eEdit);
            pIFrame->RegisterForChangeNotification(this);
            if(QDialog::Accepted == ouDlg.exec())
            {
                unsigned int unFrameId;
                std::string strName;
                QString omstrFrameId = "";
                pIFrame->GetFrameId(unFrameId);
                pIFrame->GetName(strName);
                omstrFrameId = GetString(unFrameId);
                strName = strName + " [ " + omstrFrameId.toStdString() + " ] ";
                currentItem()->setText(0, QString::fromStdString(strName));
                emit ElementCreated(pIFrame->GetUniqueId());
                SelectElement(ouItemId);
            }

        }
    }
}

void LDFElementView::vOnNewUnCondFrameElement()
{
    IFrame* pIFrame = nullptr;
    UnconditionalFrameEditDlg ouDlg(&pIFrame, eNew, this);
    if ( QDialog::Accepted == ouDlg.exec() && pIFrame != nullptr )
    {
        vCreateFrameElement(pIFrame, currentItem());
    }
}

void LDFElementView::vCreateFrameElement(IFrame* pouFrame,QTreeWidgetItem* pParentItem)
{
    QTreeWidgetItem* pFrameItem = new QTreeWidgetItem();
    UID_ELEMENT uidFrame = pouFrame->GetUniqueId();
    unsigned int unFrameId;
    QString omstrFrameId;
    std::string strName;
    TreeItemID ouTreeItemID;
    QVariant ouVariant;

    pouFrame->GetFrameId(unFrameId);
    pouFrame->GetName(strName);
    /*sprintf_s(chText, "0x%X", unId);*/
    omstrFrameId = GetString(unFrameId);
    strName = strName + " [ " + omstrFrameId.toStdString() + " ] ";
    pFrameItem->setText(0, QString::fromStdString(strName));
    pFrameItem->setIcon(0, m_ouIcons[defFRAME_ICON_INDEX]);
    ouTreeItemID.m_eTreeElementType = eLdfItem;
    ouTreeItemID.m_uidElement = uidFrame;
    m_UidTreeWidgetMap[ouTreeItemID.m_uidElement] = pFrameItem;
    ouVariant.setValue(ouTreeItemID);
    pFrameItem->setData(0, Qt::UserRole, ouVariant);

    pParentItem->addChild(pFrameItem);
    pParentItem->sortChildren(0, Qt::AscendingOrder);
    m_UidTreeWidgetMap[pouFrame->GetUniqueId()] = pFrameItem;
    emit ElementCreated(pouFrame->GetUniqueId());
}

void LDFElementView::vOnDeleteUnCondFrameElement()
{
    IFrame* pouFrame = nullptr;
    QTreeWidgetItem* pItem = currentItem();
    if ( nullptr != pItem  )
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Delete?", "Do you Really Want to Delete Item", QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::No)
        {
            return;
        }
        TreeItemID uid = pItem->data(0, Qt::UserRole).value<TreeItemID>();
        m_pBaseCluster->GetElement(eFrameElement, uid.m_uidElement, (IElement**)&pouFrame);
        if ( nullptr != pouFrame )
        {

            auto itr = m_UidTreeWidgetMap.find(uid.m_uidElement);
            if ( itr != m_UidTreeWidgetMap.end() )
            {
                itr->second->parent()->removeChild(itr->second);
            }
            LDFDatabaseManager::GetDatabaseManager()->setDocumentModified(true);
            m_pBaseCluster->DeleteElement(eFrameElement, uid.m_uidElement);
        }
    }
}


void LDFElementView::vOnNewEventTrigFrameElement()
{
    if ( selectedItems().size() > 0 )
    {
        IElement* pIFrame = nullptr;
        EventFrameEditDlg ouDlg(m_pBaseCluster, (IFrame**)&pIFrame, true, this);
        if ( QDialog::Accepted == ouDlg.exec() && pIFrame != nullptr )
        {
            vCreateFrameElement((IFrame*)pIFrame, currentItem());
        }

    }
}

void LDFElementView::vOnEditEventTrigFrameElement()
{
    if ( selectedItems().size() > 0 )
    {
        QTreeWidgetItem* item = selectedItems().at(0);
        TreeItemID ouItemId = item->data(0, Qt::UserRole).value<TreeItemID>();
        IFrame* pIFrame;
        m_pouLDFDatabaseManager->GetLDFCluster()->GetElement(eFrameElement, ouItemId.m_uidElement, (IElement**)&pIFrame);
        if ( nullptr != pIFrame )
        {
            pIFrame->RegisterForChangeNotification(this);
            EventFrameEditDlg ouDlg(m_pBaseCluster, &pIFrame, false, this);
            if ( QDialog::Accepted == ouDlg.exec() && pIFrame != nullptr )
            {
                //Move to some function
                unsigned int unFrameId;
                std::string strName;
                QString omstrFrameId = "";
                pIFrame->GetFrameId(unFrameId);
                pIFrame->GetName(strName);
                /*sprintf_s(chText, "0x%X", unId);*/
                omstrFrameId = GetString(unFrameId);
                strName = strName + " [ " + omstrFrameId.toStdString() + " ] ";
                currentItem()->setText(0, QString::fromStdString(strName));
                SelectElement(ouItemId);
            }
        }
    }
}

void LDFElementView::vOnDeleteEventTrigFrameElement()
{
    IFrame* pouFrame = nullptr;
    QTreeWidgetItem* pItem = currentItem();
    if ( nullptr != pItem  )
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Delete?", "Do you Really Want to Delete Item", QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::No)
        {
            return;
        }
        TreeItemID uid = pItem->data(0, Qt::UserRole).value<TreeItemID>();
        m_pBaseCluster->GetElement(eFrameElement, uid.m_uidElement, (IElement**)&pouFrame);
        if ( nullptr != pouFrame )
        {

            auto itr = m_UidTreeWidgetMap.find(uid.m_uidElement);
            if ( itr != m_UidTreeWidgetMap.end() )
            {
                itr->second->parent()->removeChild(itr->second);
            }
            LDFDatabaseManager::GetDatabaseManager()->setDocumentModified(true);
            m_pBaseCluster->DeleteElement(eFrameElement, uid.m_uidElement);
        }
    }
}

void LDFElementView::vOnEditSporadicFrameElement()
{
    if ( selectedItems().size() > 0 )
    {
        QTreeWidgetItem* item = selectedItems().at(0);
        TreeItemID ouItemId = item->data(0, Qt::UserRole).value<TreeItemID>();
        IElement* pIFrame;
        m_pouLDFDatabaseManager->GetLDFCluster()->GetElement(eFrameElement, ouItemId.m_uidElement, &pIFrame);
        if ( nullptr != pIFrame )
        {
            pIFrame->RegisterForChangeNotification(this);
            SporadicFrameEditDlg ouDlg((IFrame**)&pIFrame,eEdit, this);
            if ( QDialog::Accepted == ouDlg.exec() && pIFrame != nullptr )
            {
                //Move to some function
                unsigned int unFrameId;
                std::string strName;
                pIFrame->GetName(strName);
                currentItem()->setText(0, QString::fromStdString(strName));
                SelectElement(ouItemId);
            }
        }
    }
}

void LDFElementView::vOnNewSporadicFrameElement()
{
    if ( selectedItems().size() > 0 )
    {
        IElement* pIFrame = nullptr;
        SporadicFrameEditDlg ouDlg((IFrame**)&pIFrame, eNew, this);
        if ( QDialog::Accepted == ouDlg.exec() && pIFrame != nullptr )
        {
            vCreateSporadicFrameElement((IFrame*)pIFrame, currentItem());
        }

    }
}

void LDFElementView::vOnDeleteSporadicFrameElement()
{
    IFrame* pouFrame = nullptr;
    QTreeWidgetItem* pItem = currentItem();
    if ( nullptr != pItem  )
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Delete?", "Do you Really Want to Delete Item", QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::No)
        {
            return;
        }
        TreeItemID uid = pItem->data(0, Qt::UserRole).value<TreeItemID>();
        m_pBaseCluster->GetElement(eFrameElement, uid.m_uidElement, (IElement**)&pouFrame);
        if ( nullptr != pouFrame )
        {

            auto itr = m_UidTreeWidgetMap.find(uid.m_uidElement);
            if ( itr != m_UidTreeWidgetMap.end() )
            {
                itr->second->parent()->removeChild(itr->second);
            }
            LDFDatabaseManager::GetDatabaseManager()->setDocumentModified(true);
            m_pBaseCluster->DeleteElement(eFrameElement, uid.m_uidElement);
        }
    }
}


void LDFElementView::vCreateSporadicFrameElement(IFrame* pouFrame,QTreeWidgetItem* pParentItem)
{
    QTreeWidgetItem* pFrameItem = new QTreeWidgetItem();
    UID_ELEMENT uidFrame = pouFrame->GetUniqueId();
    unsigned int unFrameId;
    QString omstrFrameId;
    std::string strName;
    TreeItemID ouTreeItemID;
    QVariant ouVariant;

    pouFrame->GetFrameId(unFrameId);
    pouFrame->GetName(strName);
    /*sprintf_s(chText, "0x%X", unId);*/
    omstrFrameId = GetString(unFrameId);
    strName = strName;
    pFrameItem->setText(0, QString::fromStdString(strName));
    pFrameItem->setIcon(0, m_ouIcons[defFRAME_ICON_INDEX]);
    ouTreeItemID.m_eTreeElementType = eLdfItem;
    ouTreeItemID.m_uidElement = uidFrame;
    m_UidTreeWidgetMap[ouTreeItemID.m_uidElement] = pFrameItem;
    ouVariant.setValue(ouTreeItemID);
    pFrameItem->setData(0, Qt::UserRole, ouVariant);

    pParentItem->addChild(pFrameItem);
    pParentItem->sortChildren(0, Qt::AscendingOrder);
    m_UidTreeWidgetMap[pouFrame->GetUniqueId()] = pFrameItem;
}
