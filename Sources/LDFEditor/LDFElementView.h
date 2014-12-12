#ifndef LDFELEMENTVIEW_H
#define LDFELEMENTVIEW_H

#include <QTreeWidget>
#include "LDFDatabaseManager.h"
#include "Defines.h"
#include "LDFUtility.h"
#include <QMenu>

class LDFNetworkView;
class LDFElementView;
typedef void (LDFElementView::*pContextMenuAction)();

class LDFElementView : public QTreeWidget, public INotifyClusterChange
{
    Q_OBJECT

private:
    class LDFDatabaseManager* m_pouLDFDatabaseManager;
    QTreeWidgetItem* m_poUnconditionTreeItem;
    QTreeWidgetItem* m_poDiagTreeItem;
    QTreeWidgetItem* m_poEventTrigTreeItem;

    ICluster* m_pBaseCluster;
    LDFNetworkView* m_pouLDFNetworkView;
    std::map<UID_ELEMENT, QTreeWidgetItem*> m_UidTreeWidgetMap;
    QIcon m_ouIcons[5];
    QMenu m_ouHeaderPopupMenus[eTotalHeaderPopup];
    QMenu m_ouElementPopupMenus[eTotalElementPopup];
    bool m_bDiagFrames[2];

    QTreeWidgetItem* m_pDiagElementTopTreeItem[LIN_MAX_SIGNAL_TYPES];

    pContextMenuAction m_pContextMenuAction;

    void vCreateSlaveEcu(IEcu*, QTreeWidgetItem*);
    void vCreateSignal(ISignal*, QTreeWidgetItem*);
    void vCreateCoding(ICoding*, QTreeWidgetItem*);
public:

    LDFElementView(QWidget* parent);

    ~LDFElementView();
    void SelectElement(TreeItemID uidTreeElement);
    void SetLDFNetworkView(LDFNetworkView*);
private slots:
    void onDataUpdate();
    void onDisplayModeChanged();
    void onCustomContextMenu(const QPoint& point);
    void onItemDoubleClicked(QTreeWidgetItem* item, int column);
    //ContextMenu Action;
    void vOnEditLinCluster();

    void vOnEditMasterElement();
    void vOnNewSlave();
    void vOnEditSlaveElement();
    void vOnDeleteSlaveElement();

    void vOnEditUnCondFrameElement();
    void vOnNewUnCondFrameElement();
    void vOnDeleteUnCondFrameElement();

    void vOnEditEventTrigFrameElement();
    void vOnNewEventTrigFrameElement();
    void vOnDeleteEventTrigFrameElement();

    void vOnEditSporadicFrameElement();
    void vOnNewSporadicFrameElement();
    void vOnDeleteSporadicFrameElement();

    void vOnAddDiagSupport();
    void vOnRemoveDiagSupport();
    void vDeleteSignals(IFrame* pouFrame);      //Deletes from Cluster
    void vPopulateDiagSignals();

    void vOnNewSignal();
    void vOnEditSignal();
    void vOnDeleteSignal();

    void vOnEditCoding();
    void vOnDeleteCoding();
    void vOnNewCoding();

    void vOnEditScheduleTable();
    void vOnNewScheduleTable();
    void vOnDeleteScheduleTable();


    void vOnEditSignalGroup();
    void vOnDeleteSignalGroup();
    void vOnNewSignalGroup();

    void vOnNewNodeConfig();
    void vOnEditNodeConfig();
    void vOnDeleteNodeConfig();

    void vCreateNodeConfig(INodeConfiguration*, QTreeWidgetItem* pItem);


signals:
    void ElementCreated(UID_ELEMENT);
    void ElementDeleted(UID_ELEMENT);

private:
    void vPopulateElementTree();
    void vPopulateLinElement(QTreeWidgetItem* pTopTreeItem, eClusterElementType, std::string strItemTitle, QIcon& icon, int nTreeHeaderId = 0);
    void vPopulateLinEcus(QTreeWidgetItem* pElementTopTreeItem);
    void vPopulateLinFrames(QTreeWidgetItem* pElementTopTreeItem);

    void vPopulateLinUnconditionalFrames(QTreeWidgetItem* pElementTopTreeItem);
    void vPopulateLinDiagFrames(QTreeWidgetItem* pElementTopTreeItem);
    void vPopulateLinEventTrigFrames(QTreeWidgetItem* pElementTopTreeItem);

    //void vPopulateLinDiagFrames(QTreeWidgetItem* pElementTopTreeItem);
    void vPopulateLinSignals(QTreeWidgetItem* pElementTopTreeItem);
    void vPopulateLinSignalGroups(QTreeWidgetItem* pElementTopTreeItem);
    void vPopulateLinCoding(QTreeWidgetItem* pElementTopTreeItem);
    void vPopulateLinScheduleTables(QTreeWidgetItem* pElementTopTreeItem);
    void vPopulateLinNodeComposition(QTreeWidgetItem* pElementTopTreeItem);
    void showContextMenu(QMenu& ouMenu, QTreeWidgetItem* item, const QPoint& globalPos);

    void vHandleElementChange(eAction, UID_ELEMENT, void*);

    void vCreateFrameElement(IFrame*,QTreeWidgetItem*);
    void vCreateSporadicFrameElement(IFrame* pouFrame,QTreeWidgetItem* pParentItem);
    void vCreateScheduleTable(IScheduleTable* pouSchedTable, QTreeWidgetItem* pouParentTree);
    void vCreateSignalGroup(ISignalGroup* pouSignalGroup, QTreeWidgetItem* pouParentTree);

    void DisplayElementPopupMenu(const QPoint& point, QTreeWidgetItem* pTreeItem, UID_ELEMENT);
    void DisplayHeaderPopupMenu(const QPoint& point, QTreeWidgetItem* pTreeItem, UID_ELEMENT);
    void CreateHeaderPopupMenus();
    void CreateElementPopupMenus();
    ERRORCODE OnClusterChange(eAction eActionType, UID_ELEMENT nId, eClusterElementType& ouElementType, void* pAcionData, void* pUserData);

    //helpers
    int nGetDigFrameTreeWidgetItem(IFrame* pDigFrame, QTreeWidgetItem*& pouTreeWidget);
    void OnSlaveNodeDelete(UID_ELEMENT);
    void MapNodeToSignals(IFrame* , IEcu* pouEcu, eDIR);
};

#endif // LDFELEMENTVIEW_H
