#include "mainwindow.h"
#include "test.h"
#include <QDebug>
#include <QString>
#include <string>
#include <QDebug>


int main(int argc, char **argv)
{


    Test t;
    int tsad = t.returner();
    qDebug() << t.returner();

   // int b = p.parser() + 2;
    QGuiApplication app(argc, argv);

    // create window and view it
    MainWindow window;
    window.setTitle("Viljars game of Life");
    window.show();





    return app.exec();





}



