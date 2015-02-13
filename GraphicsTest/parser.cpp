#include "parser.h"
#include "QDebug"
#include "QFile"
#include "QPoint"
#include <string>
#include <vector>


Parser::Parser()
{

}

Parser::~Parser()
{

}
void Parser::test(){
    qDebug() << "Test godkjent";
}


void Parser::skrivTilFil(std::vector<int> punkter){


    for(int i = 0; i < punkter.size()/2; i = i+2){


    }


}


std::vector<int> Parser::parseFil(){
   // QString test = QFileDialog::getOpenFileName();
    QFile minFil("C:/Users/Viljar/Documents/QT/dieHard.txt");


    if(!minFil.open(QFile::ReadOnly | QFile::Text)){
        qDebug() << "kan ikke åpne";
    }
    QTextStream in(&minFil);

   // QString mText = in.readAll();

    std::vector<int> punkter;

    QString life = "#Life 1.06";
    QString linje = in.readLine();
    qDebug() << linje;
    if(linje != life){
        qDebug() << "ulik";
    }
    while(!in.atEnd()){

        QString ord;
        //in >> QString::number(test);
        in >> ord;
        int via = ord.toInt();
        punkter.push_back(via);
        qDebug() << via;
    }


    minFil.close();


    return punkter;
}

