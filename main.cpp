#include "enroll.h"
#include "register.h"
#include "upload.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

   // Enroll::getInstance().show();

    Upload  u;
    u.show();

//    Register r;
//    r.show();

    return a.exec();
}
