#pragma once
#include "SFML\Graphics.hpp"
//Operator overloading for faster work flow
//Multiply a vector with a cosntant
float magnitude(sf::Vector2f vector);
sf::Vector2f normilize(sf::Vector2f vector);
sf::Vector2f operator * (const sf::Vector2f rhs, const int lhs);
sf::Vector2f operator + (const sf::Vector2f rhs, const sf::Vector2f lhs);
sf::Vector2f operator + (const sf::Vector2i rhs, const sf::Vector2f lhs);
sf::Vector2f operator + (const sf::Vector2f rhs, const sf::Vector2i lhs);
sf::Vector2i operator + (const sf::Vector2i rhs, const sf::Vector2i lhs);
sf::Vector2f operator - (const sf::Vector2f rhs, const sf::Vector2f lhs);
sf::Vector2f operator - (const sf::Vector2i rhs, const sf::Vector2f lhs);
sf::Vector2f operator - (const sf::Vector2f rhs, const sf::Vector2i lhs);
sf::Vector2i operator - (const sf::Vector2i rhs, const sf::Vector2i lhs);