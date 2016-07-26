#include <QFileDialog>
#include <QTextOption>

#include "LDFViewer.h"
#include "LDFHighlighter.h"

LDFViewer::LDFViewer(QWidget* parent, QString strFileName)
    : QMainWindow(parent)
{
    vSetupFileMenu();
    vSetupEditor();
    m_pouLdfEditor->setReadOnly(true);
    setCentralWidget(m_pouLdfEditor);
    m_pouLdfEditor->setWordWrapMode(QTextOption::WrapMode::NoWrap);
    setWindowTitle("BUSMASTER LDF Viewer");
    if ( strFileName.isEmpty() == false )
    {
        OnOpenFile(strFileName);
    }
}

LDFViewer::~LDFViewer()
{

}

void LDFViewer::vSetupEditor()
{
    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(10);

    m_pouLdfEditor = new QTextEdit(this);
    m_pouLdfEditor->setFont(font);

    m_pouLDFHighlighter = new LDFHighlighter(m_pouLdfEditor->document());
}

void LDFViewer::vSetupFileMenu()
{
    QMenu* pouFileMenu = new QMenu(tr("&File"), this);
    menuBar()->addMenu(pouFileMenu);

    pouFileMenu->addAction(tr("&Open..."), this, SLOT(OnOpenFile()), QKeySequence::Open);
    pouFileMenu->addSeparator();
    pouFileMenu->addAction(tr("E&xit"), qApp, SLOT(quit()), QKeySequence::Quit);

    /*QMenu *pouHelpMenu = new QMenu(tr("&Help"), this);
    menuBar()->addMenu(pouHelpMenu);

    pouHelpMenu->addAction(tr("&About..."), this, SLOT(OnOpenFile()));*/
}

void LDFViewer::OnOpenFile(const QString& path)
{
    QString fileName = path;

    if (fileName.isNull())
    {
        fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", "Lin Description Files(*.ldf)");
    }

    if (!fileName.isEmpty())
    {
        QFile file(fileName);
        if (file.open(QFile::ReadOnly | QFile::Text))
        {
            m_pouLdfEditor->setPlainText(file.readAll());
            fileName.replace('/', '\\');
            setWindowTitle(fileName);
        }
    }
}
