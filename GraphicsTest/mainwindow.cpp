#include "mainwindow.h"
#include <QPoint>
#include <QMessageBox>
#include <QDebug>
#include <QRect>
#include <qSize>
#include <QThread>
#include <QString>
#include <QPointF>
#include <QTime>
#include <vector>
#include "parser.h"
#include <QFileDialog>

// To Do List
// 1. Rydde opp kanskje? ---
// 2. Fikse att renderen ikke går gjennom hele løkken DONE
// 3. Parser DONE
// 4. Bedre farge system!
// 5.


int antLevende = 0;
const int str = 50;
const int blokk = 20;
const int windowSize = str * blokk;
void tellNaboer();
void sjekkLiv();
int array[str][str];
enum Qt::GlobalColor farge[str][str];
enum Qt::GlobalColor aktivFarge = Qt::black;


int naboer[str][str];
bool brytLoop = true;


MainWindow::MainWindow(QWindow *parent) : QWindow{parent}, backingStore{this}
{

    Parser p;
    p.test();
    for(int i = 0; i < str; i++){ // gjør alle cellene tomme. det finnes en bedre måte å gjøre dette
        for(int j = 0; j < str; j++){
            farge[i][j] = Qt::white;
            array[i][j] = 0;
        }
    }

    create();

    if(windowSize < 1500){
        setGeometry(100, 100, windowSize + 70, windowSize + 60 ); // set resolution of window
    } else{
        setGeometry(100, 100, 800, 800); // set resolution of window

    }





}


void tellNaboer(){
    for(int i = 0; i < str; i++){
        for(int j = 0; j < str; j++){
            naboer[i][j] = 0;
            int sum = 0;
            bool harNabo = false;
            if(i >= 1 && i < str && j >= 1 && j < str){ // Ikke ett hjørne eller kant.
                for(int x = -1; x < 2; x++){
                    for(int y = -1; y < 2; y++){
                        if(array[x+i][y+j] == 1 ){
                            if(!(x == 0 && y == 0)){
                                harNabo = true;
                                sum++;
                            }
                        }
                    }
                }
            }
            if(harNabo){

                naboer[i][j] = sum;
            }
        }
    }
}

void sjekkLiv(){
    for(int i = 0; i < str; i++){
        for(int j = 0; j < str; j++){
            if(naboer[i][j] < 2 ){
                if(array[i][j] ==  1 && antLevende > 0){
                    antLevende--;
                }
                array[i][j] = 0;
            }
            else if(naboer[i][j] == 3){
                array[i][j] = 1;
                antLevende++;
            } else if(naboer[i][j] > 3){
                if(array[i][j] == 1 && antLevende > 0){
                    antLevende--;
                }
                array[i][j] = 0;

            }
        }
    }
}



// the main rendering method
void MainWindow::renderNow(bool super)
{
    // return if window is "hidden"
    if (!isExposed())
        return;

    // define the drawing canvas
    QRect rect(0, 0, width(), height());

    backingStore.beginPaint(rect);

    // create a painter device
    QPaintDevice *device = backingStore.paintDevice();
    QPainter painter(device);

    // draw a white background and call render
    painter.fillRect(0, 0, width(), height(), Qt::white);

    if(super){
        superRender(&painter); // render with loop
    }
    else{
        render(&painter); // main rendering method
    }


    // flush drawing canvas
    backingStore.endPaint();
    backingStore.flush(rect);


    renderLater();

    // a call to renderLater() at this point will enable animation...
}

// draw stuff inside window here

//void MainWindow::renderFargeValg(QPainter *painter){
//    painter.fillRect(0, str*blokk, width(), height(), Qt::white);
//}

void MainWindow::superLoop(){ // denne kombinerer sjekk liv og tellnaboer - Det neste steget er å få med render inni denne.
    //qDebug() << "SuperLoop1";

    int arrayNy[str][str];

    for(int i = 0; i < str; i++){
        for(int j = 0; j < str; j++){
            int sum = 0;
            Qt::GlobalColor arveFarge;
            arrayNy[i][j] = 0;
            if(i >= 1 && i < str && j >= 1 && j < str){ // Ikke ett hjørne eller kant.
                for(int x = -1; x < 2; x++){ // sjekker naboer i X?
                    for(int y = -1; y < 2; y++){ // naboer i Y?
                        if(array[x+i][y+j] == 1 ){ // om naboen lever
                            if(!(x == 0 && y == 0)){ // ikke seg selv
                                sum++;
                                arveFarge = farge[x][y];
                            }
                        }
                    }
                }
            }

            if(sum < 2 ){
                if(array[i][j] ==  1 && antLevende > 0){
                    antLevende--;
                }
                arrayNy[i][j] = 0;
                farge[i][j] = Qt::black;
            }
            else if(sum == 2){

                if(array[i][j] == 1){
                    // qDebug() << "superLoop2  " << i << j;
                    arrayNy[i][j] = 1;
                    farge[i][j] = arveFarge;
                } else{
                    arrayNy[i][j] = 0;
                }
            }
            else if(sum == 3){
                // qDebug() << "superLoop3  " << i << j;
                arrayNy[i][j] = 1;
                farge[i][j] = arveFarge;
                antLevende++;

            } else if(sum > 3){
                if(array[i][j] == 1 && antLevende > 0){
                    antLevende--;
                    farge[i][j] = Qt::black;
                }
                arrayNy[i][j] = 0;
            }


        }
    }
    //qDebug() << "superLoop5";
    std::swap(array,arrayNy);
}


void renderFargePalett(QPainter *painter){

    painter->fillRect(str*blokk,10,50,50,aktivFarge);


    enum Qt::GlobalColor farger[9];
    farger[0] = aktivFarge;
    farger[1] = Qt::red;
    farger[2] = Qt::green;
    farger[3] = Qt::darkBlue;
    farger[4] = Qt::cyan;
    farger[5] = Qt::magenta;
    farger[6] = Qt::yellow;
    farger[7] = Qt::gray;
    farger[8] = Qt::blue;
    farger[9] = Qt::black;

    for(int i = 0; i < 10; i++){
        painter->fillRect(str*blokk,(i*60) + 10,50,50,farger[i]);
    }

}



void MainWindow::superRender(QPainter *painter){

    int arrayNy[str][str];


    for(int i = 0; i < str; i++){
        for(int j = 0; j < str; j++){
            int sum = 0;
            Qt::GlobalColor arveFarge;
            //  arrayNy[i][j] = 0;
            if(i >= 1 && i < str && j >= 1 && j < str){ // Ikke ett hjørne eller kant.
                for(int x = -1; x < 2; x++){ // sjekker naboer i X?
                    for(int y = -1; y < 2; y++){ // naboer i Y?
                        if(array[x+i][y+j] == 1 ){ // om naboen lever
                            if(!(x == 0 && y == 0)){ // ikke seg selv
                                sum++;


                            }
                            if(farge[x+i][y+j] != Qt::white){
                                arveFarge = farge[x+i][y+j];
                            }

                        }
                    }
                }
            }
            //qDebug() << "Arvefarge" << arveFarge;

            if(sum < 2 ){
                if(array[i][j] ==  1 && antLevende > 0){
                    antLevende--;
                }
                arrayNy[i][j] = 0;
                farge[i][j] = Qt::white;
            }
            else if(sum == 2){

                if(array[i][j] == 1){
                    // qDebug() << "superLoop2  " << i << j;
                    arrayNy[i][j] = 1;
                    farge[i][j] = arveFarge;
                    painter->fillRect(j*blokk,i*blokk,blokk,blokk,farge[i][j]);
                } else{
                    farge[i][j] = Qt::white;
                    arrayNy[i][j] = 0;
                }
            }
            else if(sum == 3){
                // qDebug() << "superLoop3  " << i << j;
                arrayNy[i][j] = 1;
                farge[i][j] = arveFarge;
                painter->fillRect(j*blokk,i*blokk,blokk,blokk,farge[i][j]);
                antLevende++;
            } else if(sum > 3){
                if(array[i][j] == 1 && antLevende > 0){
                    antLevende--;
                }
                arrayNy[i][j] = 0;
                farge[i][j] = Qt::white;
            }


        }
    }
    //qDebug() << "superLoop5";
    std::swap(array,arrayNy);
    renderFargePalett(painter);
}


void MainWindow::render(QPainter *painter)
{
    renderFargePalett(painter);
    for(int i = 0; i <  str ; i++){
        for(int j = 0; j < str; j++){
            if(array[i][j] == 1){
                painter->fillRect(j*blokk,i*blokk,blokk,blokk,farge[i][j]);

            }
        }
    }
}

void delay( int millisecondsToWait )
{
    QTime dieTime = QTime::currentTime().addMSecs( millisecondsToWait );
    while( QTime::currentTime() < dieTime )
    {
        QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
    }
}

void MainWindow::loop(){

    while(brytLoop)

    {
        renderNow(false);
        tellNaboer();
        sjekkLiv();
        delay(20);
    }
}

void MainWindow::mousePressEvent(QMouseEvent *ev){
    qDebug() << "Mus";

    qDebug() << ev->localPos();
    if(ev->buttons() == Qt::MiddleButton){
        QPointF musPos = ev->localPos();
        int y =  musPos.x();

        for(int i = 0; i < str; i++){
            if(array[i][y/blokk] == 0){
                farge[i][y/blokk] = aktivFarge;
                array[i][y/blokk] = 1;
                antLevende++;
            }

        }
        renderNow(false);
    }


    if(ev->buttons() == Qt::LeftButton){



        QPointF musPos = ev->localPos();

        int y =  musPos.x();
        int x =  musPos.y();

        if(y >= str*blokk && y < str*blokk +50){
            qDebug() << "inne i farge";
            enum Qt::GlobalColor farger[9];
            farger[0] = aktivFarge;
            farger[1] = Qt::red;
            farger[2] = Qt::green;
            farger[3] = Qt::darkBlue;
            farger[4] = Qt::cyan;
            farger[5] = Qt::magenta;
            farger[6] = Qt::yellow;
            farger[7] = Qt::gray;
            farger[8] = Qt::blue;
            farger[9] = Qt::black;

            // painter->fillRect(str*blokk,(i*60) + 10,50,50,farger[i]);
            for(int i = 1; i < 10; i++){
                if(x >= i*60 +10 && x <= i*60 +60){
                    aktivFarge = farger[i];
                }
            }
        } else{
            farge[x/blokk][y/blokk] = aktivFarge;
            array[x/blokk][y/blokk] = 1;
            antLevende++;
            qDebug() << x/blokk;
            qDebug() << y/blokk;

            qDebug() << "farge "  << farge[x/blokk][y/blokk];
        }



        renderNow(false);

    }
    if(ev->buttons() == Qt::RightButton){


        QPointF musPos = ev->localPos();
        int y =  musPos.x();
        int x =  musPos.y();

        if(array[x/blokk][y/blokk] == 1){
            antLevende--;
            array[x/blokk][y/blokk] = 0;
        }

        qDebug() << x/blokk;
        qDebug() << y/blokk;

        renderNow(false);

    }


}

// interface to keyboad
void MainWindow::keyPressEvent(QKeyEvent *ev)
{
    if(ev->key() == Qt::Key_O){
        std::vector<int> punkter;
        for(int i = 0; i <str; i++){
            for(int j = 0; j < str; j++){
                if(array[i][j] == 1){
                    punkter.push_back(j);
                    qDebug() << "i : " << i;
                    punkter.push_back(i);
                    qDebug() << "j :" << j;
                }
            }
        }
    }

    if(ev->key() == Qt::Key_P){

        QPoint pos = QCursor::pos();
        int x = pos.x() / blokk;
        int y = pos.y() / blokk;
        Parser p;
        std::vector<int> punkter = p.parseFil();
        for(int i = 0; i < punkter.size(); i = i+2){
            array[punkter[i+1]+y][punkter[i]+x] = 1;
            farge[punkter[i+1]+y][punkter[i]+x] = aktivFarge;
            antLevende++;
            qDebug() << "X: " << punkter[i] << "Y: " << punkter[i+1];
        }
        renderNow(false);

    }
    if(ev->key() == Qt::Key_S){
        //qDebug() << "knappSuper";

        int teller = 0;
        if(antLevende ==  -1){
            for(int i = 0; i <str; i++){
                for(int j = 0; j < str; j++){
                    if(array[i][j] == 1){
                        qDebug() << teller << " : " << i << j;
                        teller++;
                        antLevende = teller;
                    }
                }
            }
            renderNow(true);
        } else {
            // qDebug() << "else";
            renderNow(true);
        }



    }
    if(ev->key() == Qt::Key_Space){
        brytLoop = false;
    }

    if(ev->key() == Qt::Key_V){
        brytLoop = true;


        qDebug() << "KnappViljar";
        int teller = 0;
        for(int i = 0; i <str; i++){
            for(int j = 0; j < str; j++){
                if(array[i][j] == 1){
                    qDebug() << teller << " : " << i << j;
                    teller++;
                    antLevende = teller;
                }
            }
        }
        loop();

    }

    if(ev->key() == Qt::Key_H){

        QPoint p = QCursor::pos();
        int x =  p.y() ;


        for(int i = 0; i < str; i++){
            if(array[x/blokk][i] == 0){
                farge[x/blokk][i] = aktivFarge;
                array[x/blokk][i] = 1;
                antLevende++;
            }

        }
        renderNow(false);

    }

    if(ev->key() == Qt::Key_K){
        qDebug() << "KnappKine";
        int teller = 0;

        if(antLevende ==  -1){
            for(int i = 0; i <str; i++){
                for(int j = 0; j < str; j++){
                    if(array[i][j] == 1){
                        qDebug() << teller << " : " << i << j;
                        teller++;
                        antLevende = teller;
                    }
                }
            }
            renderNow(true);
        } else {
            renderNow(true);
        }



    }


    if(ev->key() == Qt::Key_R){
        qDebug() << "tellerNaboer";
        tellNaboer();

    }
    if(ev->key() == Qt::Key_T){
        qDebug() << "SjekkLiv";
        qDebug() << "antLevende: " << antLevende;
        sjekkLiv();

    }
    if(ev->key() == Qt::Key_B){
        qDebug() << "Inne i b";
        renderNow(false);

    }
    if(ev->key() == Qt::Key_1){
        qDebug() << "Skriver ut levende";
        int teller = 0;
        for(int i = 0; i <str; i++){
            for(int j = 0; j < str; j++){
                if(array[i][j] == 1){
                    qDebug() << teller << " : " << i << j;
                    qDebug() << "Farge: " << farge[i][j];
                    teller++;
                    antLevende = teller;
                }
            }
        }
    }

    if(ev->key() == Qt::Key_8){
        qDebug() << "reset";
        for(int i = 0; i < str; i++){
            for(int j = 0; j < str; j++){
                array[i][j] = 0;
                naboer[i][j] = 0;
                farge[i][j] = Qt::black;
            }
        }
        renderNow(false);
    }

    if(ev->key() == Qt::Key_9){

        qDebug() << "100 Testen";
//        array[200][200] = 1;
//        array[201][200] = 1;
//        array[201][199] = 1;
//        array[200][201] = 1;
//        array[202][200] = 1;
//        antLevende = 5;
        renderNow(false);
        int teller = 0;
        QTime startTid = QTime::currentTime ();
        qDebug() << startTid;
        while(teller < 500){
            //tellNaboer();
            //sjekkLiv();

            //superLoop();
            renderNow(true);
            teller++;
            //delay(10);
        }
        QTime sluttTid = QTime::currentTime ();

        //sluttTid.addMSecs(-startTid.msec());
        //QTime finalTime = sluttTid - startTid;

        qDebug() << "StartTid: " << startTid;
        qDebug() << "SluttTid: " << sluttTid;



    }
    if(ev->key() == Qt::Key_2){
        //tellNaboer();
        qDebug() << "Skriver ut naboer";
        for(int i = 0; i <str; i++){
            QString linje;
            for(int j = 0; j < str; j++){
                linje += QString::number( naboer[i][j]);
                linje += "  ";
            }
            qDebug() << linje;
            qDebug() << "\n";
        }
    }
    if(ev->key() == Qt::Key_3){
        qDebug() << "Skriver ut levende:";
        for(int i = 0; i <str; i++){
            QString linje;
            for(int j = 0; j < str; j++){
                linje += QString::number( array[i][j]);
                linje += "  ";
            }
            qDebug() << linje;
            qDebug() << "\n";
        }
    }



    if(ev->key() == Qt::Key_Q)
        QGuiApplication::quit();
}

/**************
 * The following methods are "standard" Qt methods to enable drawing
 *************/

bool MainWindow::event(QEvent *event)
{
    // draw on canvas on update request
    if (event->type() == QEvent::UpdateRequest) {
        update_pending = false;
        renderNow(false);
        return true;
    }
    return QWindow::event(event);
}

// implement the "standard" Qt renderLater method
// call this method if you wish to trigger rendering of a new image
void MainWindow::renderLater()
{
    // postEvent needs a QEvent allocated on the heap.
    // it will treat this object 'properly', so this is safe
    if (!update_pending) {
        update_pending = true;
        QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
    }
}

// implement the "standard" Qt resize event
void MainWindow::resizeEvent(QResizeEvent *resizeEvent)
{
    backingStore.resize(resizeEvent->size());

    //    if (isExposed()){
    //        renderNow(false);

    //    }

}

// draw canvas if window is exposed
void MainWindow::exposeEvent(QExposeEvent *)
{
    if (isExposed()) {
        //  renderNow(false);
    }
}
