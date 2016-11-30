#ifndef LineEditWidget_H
#define LineEditWidget_H

#include <QLineEdit>
#include "LINDefines.h"
class QTableWidget;
class LineEditWidget : public QLineEdit
{
    Q_OBJECT
public:
    LineEditWidget(int nRow, QTableWidget* parent = 0);
    ~LineEditWidget();

private:
    int m_nRow;
    QTableWidget* m_pTableWidget;

protected:
    virtual void mousePressEvent( QMouseEvent* );
};

#endif // LineEditWidget_H
