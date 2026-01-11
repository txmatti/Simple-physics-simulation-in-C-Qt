
#include "constants.hh"
#include "tickhandler.hh"
#include <algorithm>
#include <QDebug>
#include <QPainter>

TickHandler::TickHandler(QObject *parent)
    : QObject(parent)
{
    buffer_ = QImage({WINDOW_WIDTH, WINDOW_HEIGHT}, QImage::Format_ARGB32_Premultiplied);
    buffer_.fill(Qt::white);

    Coord ball_location = {200, 500};
    world_.add_ball(DYNAMIC, RED, "ball1", 10, ball_location, 30, {3, 5});
    world_.add_ball(DYNAMIC, GREEN, "clickball", 10, {500, 500}, 10);

}

void TickHandler::start() {
    if (running_) return; // already running


    // Create the timer in this thread’s context
    timer_ = new QTimer(this);
    timer_->setTimerType(Qt::PreciseTimer);
    timer_->setInterval(TICK_DURATION);

    connect(timer_, &QTimer::timeout, this, &TickHandler::tick_once);
    timer_->start();

}

void TickHandler::stop() {
    running_.store(false, std::memory_order_relaxed);
}

void TickHandler::click_at(QPointF pos)
{

    Coord click_location = {pos.x(), WINDOW_HEIGHT - pos.y()};
    world_.add_ball(DYNAMIC, BLUE, "clickball", 10, click_location, 20);
}



void TickHandler::tick_once() {

    // Render into the off-screen image
    QPainter p(&buffer_);
    p.setRenderHint(QPainter::Antialiasing, false);

    p.translate(0, buffer_.height()); // move origin to bottom-left
    p.scale(1, -1);

    p.fillRect(buffer_.rect(), Qt::white);
    p.setPen(Qt::NoPen);

    auto& balls = world_.get_balls();

    // Update BALLS
    for(auto& ball : balls) {
        int x = ball.get_location().x;
        int y = ball.get_location().y;

        // Remove out-of-bounds balls


        ball.update_location();
        ball.check_collision();
        ball.update_momentum();



        // Draw something: a 10x10 block centered at (x_, y_)
        int size = ball.get_size();
        p.setBrush(ball.get_color());

        QRect rect(x - size, y - size, size*2, size*2);
        p.drawEllipse(rect);
    }



    for (auto it = balls.begin(); it != balls.end(); /* no ++ */) {
        const auto loc = it->get_location();
        const bool out =
            (loc.x < 0 || loc.x > WINDOW_WIDTH ||
             loc.y < 0 || loc.y > WINDOW_HEIGHT);

        if (out) {
            // Optional debug
            // qDebug() << "Erasing ball at" << loc.x << loc.y << "size" << it->get_size();
            it = balls.erase(it); // returns next iterator
        } else {
            ++it;
        }
    }

    emit frame_ready(buffer_);

}
