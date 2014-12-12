#include "ldfeditor.h"
#include <QtWidgets/QApplication>
#include <QTextCodec>

int main(int argc, char* argv[])
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    QApplication a(argc, argv);
    std::string omstrLDFFile = "";
    if(argc > 1)
    {
        omstrLDFFile = argv[1];
    }
    LDFEditor w(omstrLDFFile);
    w.showMaximized();
    return a.exec();
}
