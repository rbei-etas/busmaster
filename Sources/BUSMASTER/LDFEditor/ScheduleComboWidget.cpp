#include "ScheduleComboWidget.h"

ScheduleComboWidget::ScheduleComboWidget(int nRow, QWidget* parent)
    : QComboBox(parent)
{
    m_nRow = nRow;

    connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboSelectionChange(int)));
}

ScheduleComboWidget::~ScheduleComboWidget()
{

}

void ScheduleComboWidget::onComboSelectionChange(int nIndex)
{
    emit onScheduleComboChange(m_nRow, nIndex);
}