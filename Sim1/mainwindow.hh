#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QMainWindow>
#include <QImage>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class QThread;
class TickHandler;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void set_pixel(int x, int y, QColor color = Qt::black);

signals:
    void canvasSizeChanged(const QSize&);


protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private slots:
    void onFrameReady(const QImage &frame);

private:
    Ui::MainWindow *ui;
    QImage canvas_;

    QThread* workerThread_ = nullptr;
    TickHandler* worker_ = nullptr;

    void startWorker();
    void stopWorker();


};
#endif // MAINWINDOW_HH
