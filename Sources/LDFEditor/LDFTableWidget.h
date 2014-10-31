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

public:
    int CreateColumns(QList<QString>& strColumnNames);
};

#endif // LDFTABLEWIDGET_H
