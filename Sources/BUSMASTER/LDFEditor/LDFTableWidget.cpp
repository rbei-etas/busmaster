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
