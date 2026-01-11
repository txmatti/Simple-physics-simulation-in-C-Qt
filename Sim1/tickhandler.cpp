
#include "constants.hh"
#include "tickhandler.hh"
#include <QPainter>
#include <thread>

using namespace std::chrono;

TickHandler::TickHandler(QObject *parent)
    : QObject(parent)
{
    buffer_ = QImage(targetSize_, QImage::Format_ARGB32_Premultiplied);
    buffer_.fill(Qt::white);
    Coord ball_location = {200, 600};
    world_.add_ball(DYNAMIC, BLACK, "ball1", 10, ball_location, 200, {3, 5});
}

void TickHandler::start() {
    if (running_.exchange(true)) return; // already running

    // Drift-free periodic schedule
    const milliseconds tick_time(TICK_DURATION); // 50 ms per tick; change to your TICK_DURATION
    auto next = s_clock::now();

    while (running_.load(std::memory_order_relaxed)) {
        tickOnce();                         // do physics + render one frame
        emit frameReady(buffer_);           // emit a copy (implicitly shared)

        next += tick_time;
        std::this_thread::sleep_until(next);
    }
}

void TickHandler::stop() {
    running_.store(false, std::memory_order_relaxed);
}



void TickHandler::tickOnce() {

    // ---- Render into the off-screen image (thread-local) ----
    QPainter p(&buffer_);
    p.setRenderHint(QPainter::Antialiasing, false);

    // Clear background
    p.fillRect(buffer_.rect(), Qt::white);

    for(auto& ball : world_.get_balls()) {
        ball.update_location();
        ball.update_momentum();



        // Draw something: a 10x10 block centered at (x_, y_)
        int size = ball.get_size();
        QRect rect(int(ball.get_location().x) - size/2, WINDOW_HEIGHT - int(ball.get_location().y) - size/2, size, size);
        p.fillRect(rect, Qt::black);

    }

}
