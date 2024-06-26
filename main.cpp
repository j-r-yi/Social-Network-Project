#include "socialnetworkwindow.h"
#include "network.h"
#include "user.h"
#include "post.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SocialNetworkWindow w;


    w.show();

    return a.exec();


}
