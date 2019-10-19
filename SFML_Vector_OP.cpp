#include "SFML_Vector_OP.h"
float magnitude(sf::Vector2f v) {
    return sqrt(v.x*v.x + v.y*v.y);
}
sf::Vector2f normilize(sf::Vector2f v) {
    float mag = magnitude(v);
    if (mag != 0) {
        return v / mag;
    }
    return v;
}
sf::Vector2f operator * (const sf::Vector2f rhs, const int lhs) {
    return sf::Vector2f(rhs.x*lhs, rhs.y*lhs);
}

sf::Vector2f operator+(const sf::Vector2f rhs, const sf::Vector2f lhs) {
    return sf::Vector2f(rhs.x + lhs.x, rhs.y + lhs.y);
}

sf::Vector2f operator+(const sf::Vector2i rhs, const sf::Vector2f lhs) {
    return sf::Vector2f((float)rhs.x + lhs.x, (float)rhs.y + lhs.y);

}

sf::Vector2f operator+(const sf::Vector2f rhs, const sf::Vector2i lhs) {
    return sf::Vector2f(rhs.x + (float)lhs.x, rhs.y + (float)lhs.y);
}

sf::Vector2i operator+(const sf::Vector2i rhs, const sf::Vector2i lhs) {
    return sf::Vector2i(rhs.x + lhs.x, rhs.y + lhs.y);

}

sf::Vector2f operator-(const sf::Vector2f rhs, const sf::Vector2f lhs) {
    return sf::Vector2f(rhs.x - lhs.x, rhs.y - lhs.y);

}

sf::Vector2f operator-(const sf::Vector2i rhs, const sf::Vector2f lhs) {
    return sf::Vector2f((float)rhs.x - lhs.x, (float)rhs.y - lhs.y);

}

sf::Vector2f operator-(const sf::Vector2f rhs, const sf::Vector2i lhs) {
    return sf::Vector2f(rhs.x - (float)lhs.x, rhs.y - (float)lhs.y);
}

sf::Vector2i operator-(const sf::Vector2i rhs, const sf::Vector2i lhs) {
    return sf::Vector2i(rhs.x - lhs.x, rhs.y - lhs.y);
}

sf::Vector2f operator+(const sf::Vector2f rhs, const float lhs) {
    return sf::Vector2f(rhs.x + lhs, rhs.y + lhs);
}
