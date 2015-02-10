#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>

class MainWindow : public QWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWindow *parent = nullptr); // do not allow implicit conversions
    ~MainWindow() {}

    void render(QPainter *painter);

public slots:
    void renderLater();
    void renderNow();

protected:

    void MainWindow::superLoop();
    void MainWindow::loop();
    bool event(QEvent *event) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
    void exposeEvent(QExposeEvent *event) Q_DECL_OVERRIDE;
    void MainWindow::mousePressEvent(QMouseEvent *ev);

  //  void MainWindow::mouseMoveEvent(QMouseEvent *ev);
    void keyPressEvent(QKeyEvent* ev);

private:
    QBackingStore backingStore; // drawing area
    bool        update_pending;
};


#endif // MAINWINDOW_H
