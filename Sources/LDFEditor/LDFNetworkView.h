#ifndef LDFNETWORKVIEW_H
#define LDFNETWORKVIEW_H

#include <QTreeWidget>
#include "LDFDatabaseManager.h"

#define def_EcuTreeWidget_Index             0
#define def_TxFrameTreeWidget_Index         1
#define def_RxFrameTreeWidget_Index         2
#define def_TxSignalTreeWidget_Index        3
#define def_RxSignalTreeWidget_Index        4
#define def_TotalEcuTreeWidgets             5

class EcuTreeWidgets
{
public:
    QTreeWidgetItem* m_pTreeWidgets[def_TotalEcuTreeWidgets];

    EcuTreeWidgets()
    {
        for ( int i = 0 ; i < def_TotalEcuTreeWidgets ; i++ )
        {
            m_pTreeWidgets[i] = new QTreeWidgetItem();
        }

        m_pTreeWidgets[def_TxFrameTreeWidget_Index]->setText(0, "Published Frames");
        m_pTreeWidgets[def_RxFrameTreeWidget_Index]->setText(0, "Subscribed Frames");
        m_pTreeWidgets[def_TxSignalTreeWidget_Index]->setText(0, "Published Signals");
        m_pTreeWidgets[def_RxSignalTreeWidget_Index]->setText(0, "Subscribed Signals");

    }
};


class LDFNetworkView : public QTreeWidget, public INotifyClusterChange
{
    Q_OBJECT
private:
    class LDFDatabaseManager* m_pouLDFDatabaseManager;
    ICluster* m_pouLdfCluster;
    QIcon m_ouIcons[9];
    std::map<UID_ELEMENT, std::list<QTreeWidgetItem*>> m_UidTreeWidgetListMap;
    std::map<UID_ELEMENT, EcuTreeWidgets> m_UidTreeWidgetMap;
public:
    LDFNetworkView(QWidget* parent);
    ~LDFNetworkView();

public slots:
    void onDataUpdate();
    void OnElementCreated(UID_ELEMENT);
    void OnElementDeleted(UID_ELEMENT);
private:
    void vHandleEcuElementChange(eAction, UID_ELEMENT, void*);
    void vHandleElementChange(eAction, UID_ELEMENT, void*);
    void vRefreshFrameAndSignals(IFrame*);


    void vAddFrames(EcuTreeWidgets& ouEcuTreeWidgets, IEcu* pEcu);
    void vAddSignals(EcuTreeWidgets& ouEcuTreeWidgets, IEcu* pEcu);



    ERRORCODE OnClusterChange(eAction eActionType, UID_ELEMENT nId, eClusterElementType& ouElementType, void* pAcionData, void* pUserData);
};

#endif // LDFNETWORKVIEW_H
