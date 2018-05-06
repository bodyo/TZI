#include "decoder.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Decoder w;
    w.setMinimumSize({800, 500});
    w.show();
    w.setWindowState(Qt::WindowMaximized);

    return a.exec();
}
