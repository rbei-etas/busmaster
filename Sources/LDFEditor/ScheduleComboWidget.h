#ifndef ScheduleComboWidget_H
#define ScheduleComboWidget_H

#include <QComboBox>
#include "IClusterProps.h"

class ScheduleComboWidget : public QComboBox
{
    Q_OBJECT
public:
    ScheduleComboWidget(int nRow, QWidget* parent = 0);
    ~ScheduleComboWidget();

private:
    int m_nRow;

signals:
    void onScheduleComboChange(int nRow, int nIndex);

private slots:
    void onComboSelectionChange(int nIndex);

};

#endif // ScheduleComboWidget_H
