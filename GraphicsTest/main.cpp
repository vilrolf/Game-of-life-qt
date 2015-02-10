#include "mainwindow.h"
#include "parser.h"
#include <QDebug>
#include <QString>
#include <string>


int main(int argc, char **argv)
{
        QGuiApplication app(argc, argv);

        // create window and view it
        MainWindow window;
        window.setTitle("Viljars game of Life");
        window.show();



        return app.exec();





}



