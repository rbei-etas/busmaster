#ifndef LDFCOMMONTABLEWIDGET_H
#define LDFCOMMONTABLEWIDGET_H

#include <QTableWidget>
#include "IClusterProps.h"

class LDFCommonTableWidget : public QTableWidget
{
    Q_OBJECT
    QFont m_ldfFont;
public:
    LDFCommonTableWidget(QWidget* parent = 0);
    ~LDFCommonTableWidget();
    int CreateColumns(QList<QString>& strColumnNames);
    int InsertRow(int nRow, QList<QVariant> ouValueList, QString strToolTip = "");
    int nGetChecksumType(LIN_Settings ouProtocolSettings, std::string& strChecksum);
    virtual void resizeEvent(QResizeEvent* event);
    QSize GetTableWidgetSize();

signals:
    void onItemDragged(int, int , int);
private slots:
    void tableItemMoved(int, int , int);
};

#endif // LDFCOMMONTABLEWIDGET_H
