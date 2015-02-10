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

//To Do List
// 1. Rydde opp kanskje?
// 2. Fikse att renderen ikke går gjennom hele løkken
// 3. Parser
// 4.


int antLevende = 0;
const int str = 500;
const int blokk = 2;
const int windowSize = str * blokk;
void tellNaboer();
void sjekkLiv();
int array[str][str];

int naboer[str][str];
bool brytLoop = true;


MainWindow::MainWindow(QWindow *parent) : QWindow{parent}, backingStore{this}
{
    for(int i = 0; i < str; i++){ // gjør alle cellene tomme. det finnes en bedre måte å gjøre dette
        for(int j = 0; j < str; j++){

            array[i][j] = 0;
        }
    }

    create();

    if(windowSize < 1500){
        setGeometry(100, 100, windowSize, windowSize); // set resolution of window
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
void MainWindow::renderNow()
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

    render(&painter); // main rendering method

    // flush drawing canvas
    backingStore.endPaint();
    backingStore.flush(rect);



    // a call to renderLater() at this point will enable animation...
}

// draw stuff inside window here

void decentLoop(){


}


void MainWindow::superLoop(){ // denne kombinerer sjekk liv og tellnaboer - Det neste steget er å få med render inni denne.
    //qDebug() << "SuperLoop1";

    int arrayNy[str][str];

    for(int i = 0; i < str; i++){
        for(int j = 0; j < str; j++){
            int sum = 0;
          //  arrayNy[i][j] = 0;
            if(i >= 1 && i < str && j >= 1 && j < str){ // Ikke ett hjørne eller kant.
                for(int x = -1; x < 2; x++){ // sjekker naboer i X?
                    for(int y = -1; y < 2; y++){ // naboer i Y?
                        if(array[x+i][y+j] == 1 ){ // om naboen lever
                            if(!(x == 0 && y == 0)){ // ikke seg selv
                                sum++;
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
            }
            else if(sum == 2){

                if(array[i][j] == 1){
                // qDebug() << "superLoop2  " << i << j;
                    arrayNy[i][j] = 1;
                } else{
                    arrayNy[i][j] = 0;
                }
            }
            else if(sum == 3){
               // qDebug() << "superLoop3  " << i << j;
                arrayNy[i][j] = 1;
                antLevende++;
            } else if(sum > 3){
                if(array[i][j] == 1 && antLevende > 0){
                    antLevende--;
                }
                arrayNy[i][j] = 0;
            }


        }
    }
    //qDebug() << "superLoop5";
    std::swap(array,arrayNy);
}


const QRect* finnRekt(){

    QRect* rekt = new QRect[antLevende];
    int teller = 0;

    QSize fu;
    fu.setHeight(blokk);
    fu.setWidth(blokk);


    for(int i = 0; i <  str ; i++){
        for(int j = 0; j < str; j++){
            if(array[i][j] == 1){
                rekt[teller].setX(j*blokk);
                rekt[teller].setY(i*blokk);
                rekt[teller].setSize(fu);
                teller++;
            }
        }
    }

    return rekt;
}

void MainWindow::render(QPainter *painter)
{
    //qDebug() << "render";
    const QRect* rekts = finnRekt();


    for(int i = 0; i < antLevende; i++){
        painter->fillRect(rekts[i],Qt::black);
    }

    delete rekts;
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
        renderNow();
        tellNaboer();
        sjekkLiv();
        delay(200);
    }
}

void MainWindow::mousePressEvent(QMouseEvent *ev){
    qDebug() << "Mus";

    qDebug() << ev->localPos();


    // qDebug() << ev->button();

    if(ev->buttons() == Qt::LeftButton){

        antLevende++;
        QPointF musPos = ev->localPos();
        int y =  musPos.x();
        int x =  musPos.y();

        array[x/blokk][y/blokk] = 1;
        qDebug() << x/blokk;
        qDebug() << y/blokk;

        renderNow();

    }


}

// interface to keyboad
void MainWindow::keyPressEvent(QKeyEvent *ev)
{
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
            renderNow();
        } else {
           // qDebug() << "else";
            superLoop();
            renderNow();
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
            renderNow();
        } else {
            tellNaboer();
            sjekkLiv();
            renderNow();
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
        renderNow();

    }
    if(ev->key() == Qt::Key_1){
        qDebug() << "Skriver ut levende";
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
    }

    if(ev->key() == Qt::Key_8){
        qDebug() << "reset";
        for(int i = 0; i < str; i++){
            for(int j = 0; j < str; j++){
                array[i][j] = 0;
                naboer[i][j] = 0;
            }
        }
        renderNow();
    }

    if(ev->key() == Qt::Key_9){

        qDebug() << "100 Testen";
        array[200][200] = 1;
        array[201][200] = 1;
        array[201][199] = 1;
        array[200][201] = 1;
        array[202][200] = 1;
        antLevende = 5;
        renderNow();
//               sjekkLiv();
//                tellNaboer();

        int teller = 0;
        QTime startTid = QTime::currentTime ();
        qDebug() << startTid;
        while(teller < 500){
            //tellNaboer();
           //sjekkLiv();

            superLoop();
            renderNow();
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
        renderNow();
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

    if (isExposed()){
        renderNow();

    }

}

// draw canvas if window is exposed
void MainWindow::exposeEvent(QExposeEvent *)
{
    if (isExposed()) {
        //  renderNow();
    }
}