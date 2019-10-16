#include "SFML\Graphics.hpp"
#include <fstream> //To read the file
#include <iostream> //Debug messages
#include <iomanip> //
#include <random> //To generete random numbers
#include <vector> //To easier keep arrays


#define PI 3.14159265
#define DEG 180/PI
//Constants
const int DATASIZE = 100;
const int SCREENSIZE = 500;
//Psudo Ransom number generator
std::mt19937_64 gen;
std::uniform_int_distribution<int> rnd(0, DATASIZE-1);

void plotStreamLines(sf::Vector2f field[][DATASIZE], sf::VertexArray &stream, std::vector<sf::CircleShape>&triangles, 
                     int nrVectors = 100, float step = 1, int scale = 1);
void plotLines(int x1, int y1, float x2, float y2, sf::VertexArray &vectors);
void plotLines(sf::Vector2f from, sf::Vector2f to, sf::VertexArray &vectors);
void plotPoints(sf::Vector2f point, std::vector<sf::CircleShape>& circles);
void plotDirectionTriangles(sf::Vector2f point, sf::Vector2f direction, std::vector<sf::CircleShape>& circles);

sf::Vector2i randomPointInField();


//Operator overloading for faster work flow
//Multiply a vector with a cosntant
sf::Vector2f operator * (const sf::Vector2f rhs, const int lhs);
sf::Vector2f operator + (const sf::Vector2f rhs, const sf::Vector2f lhs);
sf::Vector2f operator + (const sf::Vector2i rhs, const sf::Vector2f lhs);
sf::Vector2f operator + (const sf::Vector2f rhs, const sf::Vector2i lhs);
sf::Vector2i operator + (const sf::Vector2i rhs, const sf::Vector2i lhs);
sf::Vector2f operator - (const sf::Vector2f rhs, const sf::Vector2f lhs);
sf::Vector2f operator - (const sf::Vector2i rhs, const sf::Vector2f lhs);
sf::Vector2f operator - (const sf::Vector2f rhs, const sf::Vector2i lhs);
sf::Vector2i operator - (const sf::Vector2i rhs, const sf::Vector2i lhs);

int main() {

    sf::RenderWindow window(sf::VideoMode(SCREENSIZE, SCREENSIZE), "VectorFields");
    std::ifstream data("data.txt");
    float xOfVec[DATASIZE][DATASIZE];
    float yOfVec[DATASIZE][DATASIZE];

    //A vector of them
    sf::Vector2f dataVectors[DATASIZE][DATASIZE];

    if (!data.is_open()) {
        std::cout << "Failed to open data.txt\n";
        char t;
        std::cin >> t;
        return 0;
    }
    for (int i = 0; i < DATASIZE; i++) {
        for (int j = 0; j < DATASIZE; j++) {
            data >> xOfVec[i][j];
        }
        std::cout << std::endl;
    }
    for (int i = 0; i < DATASIZE; i++) {
        for (int j = 0; j < DATASIZE; j++) {
            data >> yOfVec[i][j];
            dataVectors[i][j] = sf::Vector2f(xOfVec[i][j], yOfVec[i][j]);
        }
    }
    int xGrid[DATASIZE], yGrid[DATASIZE];
    sf::VertexArray vectorField(sf::Lines);
    std::vector<sf::CircleShape> triangles;
    plotStreamLines(dataVectors, vectorField, triangles, 1000, 2, SCREENSIZE/DATASIZE);
    std::cout << vectorField.getVertexCount() << std::endl;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        window.clear();
        window.draw(vectorField);
        for (auto& triangle : triangles) {
            window.draw(triangle);
        }
        window.display();
    }
    return 0;
}
void plotStreamLines(sf::Vector2f field[][DATASIZE], sf::VertexArray &stream, std::vector<sf::CircleShape>& triangles,
                     int nrVectors ,float step, int scale) {
    for (int i = 0; i < nrVectors; i++) {
       sf::Vector2i randVec = randomPointInField();
       sf::Vector2f from = (sf::Vector2f) randVec * scale;
       sf::Vector2f to = (randVec + field[randVec.x][randVec.y] * step) * scale;
       plotLines(from, to, stream);
       plotDirectionTriangles(to, to - from, triangles);

    }
}

void plotLines(int x1, int y1, float x2, float y2, sf::VertexArray &vectors) {
    plotLines(sf::Vector2f(x1, y1), sf::Vector2f(x2, y2), vectors);
}
void plotLines(sf::Vector2f from, sf::Vector2f to, sf::VertexArray &vectors) {
    sf::Vertex fromVex(from);
    sf::Vertex toVex(to);
    fromVex.color = sf::Color(64, 224, 208,255);
    toVex.color = sf::Color(64, 224, 208, 255);
    vectors.append(fromVex);

    vectors.append(toVex);
}

void plotPoints(sf::Vector2f point, std::vector<sf::CircleShape>& circles) {
    sf::CircleShape* newcircle = new sf::CircleShape;
    newcircle->setRadius(5);
    newcircle->setPosition(point);
    newcircle->setFillColor(sf::Color::Red);
    circles.push_back(*newcircle);
}

void plotDirectionTriangles(sf::Vector2f point, sf::Vector2f direction, std::vector<sf::CircleShape> &triangles) {
    double deg = atan(direction.y / direction.y);
    sf::CircleShape* triangle = new sf::CircleShape(2, 3);
    triangle->setPosition(point);
    triangle->setFillColor(sf::Color(64, 224, 208, 255));
    triangle->setRotation(deg * DEG);
    triangles.push_back(*triangle);
    std::cout << "Created triangle in (" << point.x << "," << point.y << ")\n";
}

sf::Vector2i randomPointInField() {
    return sf::Vector2i(rnd(gen), rnd(gen));
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
