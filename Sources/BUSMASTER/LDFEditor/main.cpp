#include <QtWidgets/QApplication>
#include <QTextCodec>

#include "LDFEditor.h"

int main(int argc, char* argv[])
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    // set app version for QSettings
    QCoreApplication::setOrganizationName("rbei-etas");
    QCoreApplication::setApplicationName("LDFEditor");
    QCoreApplication::setApplicationVersion("1.0");

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
