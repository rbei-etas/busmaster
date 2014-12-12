#include "AboutLDFEditor.h"

AboutLDFEditor::AboutLDFEditor(QWidget* parent)
    : QDialog(parent)
{
    ui.setupUi(this);

    connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(accept()));
    QIcon ouWindowIcon;
    ouWindowIcon.addFile(":/LDFEditorIcons/Resources/icons/16x16/Application.png", QSize(16, 16));
    ouWindowIcon.addFile(":/LDFEditorIcons/Resources/icons/22x22/Application.png", QSize(16, 16));
    ouWindowIcon.addFile(":/LDFEditorIcons/Resources/icons/32x32/Application.png", QSize(16, 16));
    setWindowIcon(ouWindowIcon);
}

AboutLDFEditor::~AboutLDFEditor()
{

}
