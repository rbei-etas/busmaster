#ifndef PROPERTYVIEW_H
#define PROPERTYVIEW_H

#include <QWidget>
#include <QTreeWidget>
#include <QTableWidget>

#include "LDFDefines.h"
#include "LDFCommonTableWidget.h"

#include "ICluster.h"

class LDFAdditionalView;
class LDFPropertyView : public LDFCommonTableWidget
{
    Q_OBJECT

public:
    LDFPropertyView(QWidget* parent);
    ~LDFPropertyView();
    int SetAdditionalPropsView(class LDFAdditionalView*);
private:
    IScheduleTable* m_pouCurrentTable;
    class LDFDatabaseManager* m_pouLDFDatabaseManager;
    ICluster* m_pouLdfCluster;
    LDFAdditionalView* m_pouLDFAdditionalView;
private:
    void vDisplayClusterProps();
    void vDisplayElement ( IElement* pouLdfElement);
    int DisplayEcuElementProps(IElement* pouLdfElement);
    int DisplayMasterDetails(IEcu* pouLdfElement);
    int DisplaySlaveDetails(IEcu* pouLdfElement);
    int DisplaySignalElementProps(IElement* pouLdfElement);
    int DisplayCodingElementProps(IElement* pouLdfElement);
    int DisplayScheduleTableElementProps(IElement* pouLdfElement);
    int DisplayFrameElementProps(IElement* pouLdfElement);
    int DisplayUnConditionalFrameDetails(IFrame* pouLdfElement);
    int DisplayEventTriggeredFrameDetails(IFrame* pouFrame);
    int DisplayDiagnosticFrameDetails(IFrame* pouFrame);
    int DisplaySporadicFrameDetails(IFrame* pouFrame);
    int DisplaySignalGroupDetails(IElement* pouGroup);
    int DisplayNodeCompositionDetails(IElement* pouLdfElement);
    int GetUnconditionalFrames(std::list<IFrame*>& lstFrames, std::string& strFrames);

public slots:
    void onTreeItemClicked(QTreeWidgetItem* pouTreeItem, int nId);
    void OnitemSelectionChanged();
private:

};

#endif // PROPERTYVIEW_H
