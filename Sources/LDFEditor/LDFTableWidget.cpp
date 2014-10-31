#include "LDFTableWidget.h"

LDFTableWidget::LDFTableWidget(QWidget* parent)
    : QTableWidget(parent)
{
    m_ldfFont.setFamily(QStringLiteral("Courier New"));
    m_ldfFont.setPointSize(10);
}

LDFTableWidget::~LDFTableWidget()
{

}

int LDFTableWidget::CreateColumns(QList<QString>& strColumnNames)
{
    int nColumnClount =strColumnNames.size();
    setColumnCount(2);
    int nColumn = 0;
for ( auto itr : strColumnNames )
    {
        QTableWidgetItem* pTablewidgetitem = new QTableWidgetItem();

        if(nullptr != pTablewidgetitem)
        {
            pTablewidgetitem->setText(strColumnNames.at(0));
            pTablewidgetitem->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter|Qt::AlignCenter);
            pTablewidgetitem->setFont(m_ldfFont);
            setHorizontalHeaderItem(nColumn++, pTablewidgetitem);
        }
    }
    return 0;
}

