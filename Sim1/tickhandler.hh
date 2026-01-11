#include <QObject>
#include <QImage>
#include <QTimer>
#include <atomic>

#include "world.hh"

class TickHandler : public QObject {
    Q_OBJECT
public:
    explicit TickHandler(QObject *parent = nullptr);

public slots:
    void start();                    // starts the tick loop
    void stop();                     // stops the tick loop (safe on quit)
    void click_at(QPointF pos);

signals:
    void frame_ready(const QImage& frame); // emitted after each tick

private slots:
    void tick_once();

private:
    std::atomic<bool> running_{false};
    QTimer* timer_ = nullptr;
    QImage buffer_;                  // off-screen buffer (only touched in worker thread)

    World world_;
};
