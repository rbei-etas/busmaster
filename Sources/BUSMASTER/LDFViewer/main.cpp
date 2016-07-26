#include "LDFViewer.h"
#include <QtWidgets/QApplication>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("rbei-etas");
    QCoreApplication::setApplicationName("LDFViewer");
    QCoreApplication::setApplicationVersion("1.0");

    QString strFileName = "";
    if ( argc > 1 )
    {
        strFileName = argv[1];
    }
    LDFViewer w(nullptr, strFileName);
    w.showMaximized();
    return a.exec();
}
