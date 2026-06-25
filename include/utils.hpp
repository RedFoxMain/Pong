#ifndef UTILS_HPP
#define UTILS_HPP

#include "paddle.hpp"
#include "ball.hpp"

static bool isCollided(Paddle& paddle, Ball& ball) {
    float dx = std::abs(ball.getPosition().x - paddle.getPosition().x);
    float dy = std::abs(ball.getPosition().y - paddle.getPosition().y);
    float half_width = paddle.getSize().x / 2.0f;
    float half_height = paddle.getSize().y / 2.0f;
    if (dx > (half_width + ball.getRadius()) || dy > (half_height + ball.getRadius()))
        return false;
    if (dx <= half_width || dy <= half_height)
        return true;
    float cornerDistanceSq = std::pow(dx - half_width, 2) + std::pow(dy - half_height, 2);
    return cornerDistanceSq <= std::pow(ball.getRadius(), 2);
}
#endif // !UTILS_HPP