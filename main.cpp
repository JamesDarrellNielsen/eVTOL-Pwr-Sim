#include "evtolpwrsim.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    eVtolPwrSim w;
    w.resize(750,280);
    w.show();
    return a.exec();
}
