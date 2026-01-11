#include <QObject>
#include <QImage>
#include <atomic>
#include <chrono>
#include "world.hh"
#include "constants.hh"

class TickHandler : public QObject {
    Q_OBJECT
public:
    explicit TickHandler(QObject *parent = nullptr);

public slots:
    void start();                    // starts the tick loop
    void stop();                     // stops the tick loop (safe on quit)

signals:
    void frameReady(const QImage& frame); // emitted after each tick

private:
    void tickOnce();                 // one physics+render step

    std::atomic<bool> running_{false};
    QImage buffer_;                  // off-screen buffer (only touched in worker thread)
    QSize targetSize_{WINDOW_WIDTH, WINDOW_HEIGHT};

    World world_;

    using s_clock = std::chrono::steady_clock;
};
