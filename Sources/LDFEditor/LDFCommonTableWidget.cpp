#include "LDFCommonTableWidget.h"
#include "qheaderview.h"
#include "LDFUtility.h"
#include "LDFTagDefines.h"
#include "Defines.h"

LDFCommonTableWidget::LDFCommonTableWidget(QWidget* parent)
    : QTableWidget(parent)
{
    m_ldfFont.setFamily(QStringLiteral("Courier New"));
    m_ldfFont.setPointSize(10);

    QHeaderView* pHeader = horizontalHeader();
    //pHeader->setSectionResizeMode(QHeaderView::Stretch);
    pHeader->setSectionResizeMode(QHeaderView::Interactive);
    pHeader->setStretchLastSection(true);
    setEditTriggers(NoEditTriggers);
    verticalHeader()->setVisible(true);
    setAlternatingRowColors(true);
    setSelectionBehavior(QAbstractItemView::SelectRows);

    connect(verticalHeader(), SIGNAL(sectionMoved(int,int, int)), this, SLOT(tableItemMoved(int,int,int)));
}

LDFCommonTableWidget::~LDFCommonTableWidget()
{

}

QSize LDFCommonTableWidget::GetTableWidgetSize()
{
    int w = verticalHeader()->width() + 4; // +4 seems to be needed
    for (int i = 0; i < columnCount(); i++)
    {
        w += columnWidth(i);    // seems to include gridline (on my machine)
    }
    int h = horizontalHeader()->height() + 4;
    for (int i = 0; i < rowCount(); i++)
    {
        h += rowHeight(i);
    }
    return QSize(w, h);
}

void LDFCommonTableWidget::resizeEvent(QResizeEvent* event)
{
    int nColumnCount = columnCount();
    if ( nColumnCount > 0 )
    {
        int x = verticalHeader()->height();
        int y = verticalHeader()->width();
        int z = verticalHeader()->length();
        QSize s = verticalHeader()->size();
        int nWidth = (width()-50)/ nColumnCount;
        for ( int i = 0 ; i < nColumnCount; i++ )
        {
            setColumnWidth(i, nWidth);
        }
    }
}
int LDFCommonTableWidget::CreateColumns(QList<QString>& strColumnNames)
{
    clear();
    int nColumnClount =strColumnNames.size();
    setColumnCount(nColumnClount);
    setRowCount(0);
    //int nColumn = 0;
    int nWidth = (width()-10)/strColumnNames.size();
    for ( int nColumn = 0; nColumn < strColumnNames.size(); nColumn++)// auto itr : strColumnNames )
    {
        QTableWidgetItem* pTablewidgetitem = new QTableWidgetItem();
        pTablewidgetitem->setText(strColumnNames.at(nColumn));
        pTablewidgetitem->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter|Qt::AlignCenter);
        pTablewidgetitem->setFont(m_ldfFont);
        setColumnWidth(nColumn, nWidth);
        setHorizontalHeaderItem(nColumn, pTablewidgetitem);
    }
    return 0;
}

int LDFCommonTableWidget::InsertRow(int nRow, QList<QVariant> ouValueList, QString strToolTip)
{
    int nColumn = 0;
for ( auto itr : ouValueList )
    {
        QVariant ouVariant = ouValueList.at(nColumn);
        QString str = toString(ouVariant);

        QLDFTableWidgetItem* col1 = new QLDFTableWidgetItem(str);

        if( strToolTip.size() <= 0 )
        {
            col1->setToolTip(str);
        }
        else
        {
            col1->setToolTip(strToolTip);
        }
        setItem(nRow, nColumn, col1);
        nColumn++;
    }

    return 0;
}


int LDFCommonTableWidget::nGetChecksumType(LIN_Settings ouProtocolSettings, std::string& strChecksum)
{
    if(ouProtocolSettings.m_dProtocolVers == defVersion_1_3)
    {
        strChecksum = defCLASSIC_CHECKSUM;
    }
    else if(ouProtocolSettings.m_dProtocolVers == defVersion_2_0)
    {
        strChecksum = defENHANCED_CHECKSUM;
    }
    else if(ouProtocolSettings.m_dProtocolVers == defVersion_2_1)
    {
        strChecksum = defENHANCED_CHECKSUM;
    }
    else
    {
        return EC_FAILURE;
    }
    return EC_SUCCESS;

}

void LDFCommonTableWidget::tableItemMoved(int nLogical, int nOld, int nNew)
{
    emit onItemDragged(nLogical, nOld, nNew);
}