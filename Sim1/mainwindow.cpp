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
    emit click_at(event->pos());

}

void MainWindow::startWorker()
{
    worker_thread_ = new QThread(this);
    worker_ = new TickHandler();

    worker_->moveToThread(worker_thread_);

    connect(worker_thread_, &QThread::started, worker_, &TickHandler::start);
    connect(worker_, &TickHandler::frame_ready, this, &MainWindow::onFrameReady);

    connect(this, &MainWindow::click_at, worker_, &TickHandler::click_at, Qt::QueuedConnection);

    connect(this, &QObject::destroyed, worker_, &TickHandler::stop);
    connect(worker_thread_, &QThread::finished, worker_, &QObject::deleteLater);
    connect(worker_thread_, &QThread::finished, worker_thread_, &QObject::deleteLater);

    worker_thread_->start();

}


void MainWindow::stopWorker() {
    if (!worker_thread_) return;
    // Ask worker to stop and end the thread
    QMetaObject::invokeMethod(worker_, "stop", Qt::QueuedConnection);
    worker_thread_->quit();
    worker_thread_->wait();
    worker_thread_ = nullptr;
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





