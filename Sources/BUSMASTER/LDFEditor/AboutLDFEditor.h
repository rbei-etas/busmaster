#ifndef ABOUTLDFEDITOR_H
#define ABOUTLDFEDITOR_H

#include <QDialog>
#include "ui_AboutLDFEditor.h"

class AboutLDFEditor : public QDialog
{
    Q_OBJECT

public:
    AboutLDFEditor(QWidget* parent = 0);
    ~AboutLDFEditor();

private:
    Ui::AboutLDFEditor ui;
};

#endif // ABOUTLDFEDITOR_H
