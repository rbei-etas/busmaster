#ifndef LDFADDITIONALVIEW_H
#define LDFADDITIONALVIEW_H

#include <QTableWidget>
#include <qtreewidget.h>
#include <qlabel.h>
#include "ICluster.h"
#include "LDFDatabaseManager.h"
#include "LDFCommonTableWidget.h"

Q_DECLARE_METATYPE(ConfigFrameDetails);

class LDFAdditionalView : public LDFCommonTableWidget
{
    Q_OBJECT
private:

    class LDFDatabaseManager* m_pouLDFDatabaseManager;
    ICluster* m_pouLdfCluster;
    IEcu* m_pCurrentEcu;
    QLabel* m_pHelpLabel;

public:
    LDFAdditionalView(QWidget* parent);
    ~LDFAdditionalView();
private slots:
    void tableItemMoved(int, int , int);
    void onConfigFrameEditChange(const QString&);
public:
    void onTreeItemClicked(QTreeWidgetItem* pouTreeItem, int nColumn);
    void SetInfoLable(QLabel* m_pHelpLabel);
    int DisplayScedulingTableAdditionalProps(IScheduleTable* pouTable, int nItemIndex = 0 );
private:
    void vDisplayElement ( IElement* pouLdfElement);
    int DisplayEcuAdditionalProps(IEcu* pouLdfElement);
    int DisplayMasterAdditionalProps(IEcu* pouLdfElement);
    int DisplaySlaveAdditionalProps(IEcu* pouLdfElement);
    int DisplaySignalAdditionalProps(ISignal* pouLdfElement);
    int DisplayFrameAdditionalProps( IFrame* pouFrame);
    int DisplayUnConditionalFrameProps(IFrame* pouFrame);
    int DisplayEventTrigProps( IFrame* pouFrame);
    int DisplaySporadicProps( IFrame* pouFrame);
    int DisplayDiagProps( IFrame* pouFrame);
    int DisplayCodingAdditionalProps(ICoding* pouLdfElement);
    int DisplaySignalGroupAdditionalProps(ISignalGroup* pouLdfElement);
    int nGetEncodingnUnit(CompuMethodProps ouCompuProps, std::string& strEncoding, std::string& strUnit);
    int DisplayScheduleTableFrameProps(CSheduleTableItem& ouTableItem);

    int DisplayAssignFrameIdProps(CSheduleTableItem& ouTableItem);
    int DisplayDiagProps(CSheduleTableItem& ouTableItem);
    int DisplayConfigFrames(IEcu* pouEcu, std::list<ConfigFrameDetails>& ouFrameUidList);
};

#endif // LDFADDITIONALVIEW_H
