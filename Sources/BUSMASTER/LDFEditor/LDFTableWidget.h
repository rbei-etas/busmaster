#ifndef LDFTABLEWIDGET_H
#define LDFTABLEWIDGET_H

#include <QTableWidget>


class LDFTableWidget : public QTableWidget
{
    Q_OBJECT
private:
    QFont m_ldfFont;

public:
    LDFTableWidget(QWidget* parent = 0);
    ~LDFTableWidget();

};

#endif // LDFTABLEWIDGET_H
