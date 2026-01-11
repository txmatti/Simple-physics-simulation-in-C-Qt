#include "mainwindow.hh"
#include "ui_mainwindow.h"
#include "constants.hh"
#include "tickhandler.hh"

#include <QThread>
#include <QPainter>
#include <QMouseEvent>


using s_clock = std::chrono::steady_clock;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    resize(WINDOW_HEIGHT, WINDOW_WIDTH);
    QImage newCanvas({WINDOW_HEIGHT, WINDOW_WIDTH}, QImage::Format_ARGB32_Premultiplied);
    newCanvas.fill(Qt::white);
    canvas_ = std::move(newCanvas);

    startWorker();

}

MainWindow::~MainWindow()
{
    stopWorker();
    delete ui;
}




void MainWindow::set_pixel(int x, int y, QColor color) {
    if (x < 0 || y < 0 || x >= canvas_.width() || y >= canvas_.height())
        return;
    canvas_.setPixelColor(x, y, color);
}


void MainWindow::mousePressEvent(QMouseEvent *event) {
    int size = 10;
    int x = event->pos().x();
    int y = event->pos().y();
    for(int ix = 0; ix < size; ix++) {
        for(int iy = 0; iy < size; iy++) {
            set_pixel(x - size/2 + ix, y - size/2 + iy);
        }
    }
    update();
}

void MainWindow::startWorker()
{
    workerThread_ = new QThread(this);
    worker_ = new TickHandler();

    worker_->moveToThread(workerThread_);

    connect(workerThread_, &QThread::started, worker_, &TickHandler::start);
    connect(worker_, &TickHandler::frameReady, this, &MainWindow::onFrameReady);


    connect(this, &QObject::destroyed, worker_, &TickHandler::stop);
    connect(workerThread_, &QThread::finished, worker_, &QObject::deleteLater);
    connect(workerThread_, &QThread::finished, workerThread_, &QObject::deleteLater);

    workerThread_->start();

}


void MainWindow::stopWorker() {
    if (!workerThread_) return;
    // Ask worker to stop and end the thread
    QMetaObject::invokeMethod(worker_, "stop", Qt::QueuedConnection);
    workerThread_->quit();
    workerThread_->wait();
    workerThread_ = nullptr;
    worker_ = nullptr;
}


void MainWindow::onFrameReady(const QImage& frame) {
    // Swap in the new frame and repaint
    canvas_ = frame;
    update();
}


void MainWindow::paintEvent(QPaintEvent * /*event*/) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, false);
    p.drawImage(0, 0, canvas_);
}





