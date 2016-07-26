#ifndef LDFVIEWER_H
#define LDFVIEWER_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QTextEdit>

#include "ui_LDFViewer.h"

class LDFViewer : public QMainWindow
{
    Q_OBJECT
    QTextEdit* m_pouLdfEditor;
public:
    LDFViewer(QWidget* parent = 0, QString strFileName = QString());
    ~LDFViewer();

private:
    class LDFHighlighter* m_pouLDFHighlighter;
    Ui::LDFViewerClass ui;
    void vSetupEditor();
    void vSetupFileMenu();

public slots:
    void OnOpenFile(const QString& path = QString());
};

#endif // LDFVIEWER_H
