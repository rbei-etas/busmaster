#include "LineEditWidget.h"
#include <QMouseEvent>
#include <QTableWidget>

LineEditWidget::LineEditWidget(int nRow, QTableWidget* parent)
    : QLineEdit(parent)
{
    m_nRow = nRow;
    m_pTableWidget= parent;
    setFrame(false);
}

LineEditWidget::~LineEditWidget()
{

}

void LineEditWidget::mousePressEvent( QMouseEvent* ouEvent)
{
    if(m_nRow >=0 && nullptr != m_pTableWidget)
    {
        m_pTableWidget->selectRow(m_nRow);
    }
    this->setFocus();
}
