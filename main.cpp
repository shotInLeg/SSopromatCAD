#include "ssopromatcad.h"
#include "startform.h"
#include <QApplication>
#include <QTimer>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    StartForm s;
    s.show();

    QEventLoop loop; QTimer::singleShot(3000, &loop, SLOT(quit())); loop.exec();
    s.close();

    SSopromatCAD w;
    w.show();

    return a.exec();
}
