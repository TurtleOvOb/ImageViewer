#include "ImageViewer.h"
#include <QtWidgets/QApplication>
#include<qstylefactory.h>
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    //QStyle *sty = QStyleFactory::create("fusion");
    //app.setStyle(sty);
    ImageViewer window;
    window.show();
    return app.exec();
}
