#include "SFML\Graphics.hpp"
#include <fstream> //To read the file
#include <iostream> //Debug messages
#include <iomanip>

//Constants
const int DATASIZE = 100;

void plot(int x1, int y1, float x2, float y2, sf::VertexArray &vectors);
void plot(sf::Vector2f from, sf::Vector2f to, sf::VertexArray &vectors);

int main() {

    sf::RenderWindow window(sf::VideoMode(1200, 1200), "VectorFields");
    std::ifstream data("data.txt");
    if (!data.is_open()) {
        std::cout << "Failed to open data.txt\n";
        char t;
        std::cin >> t;
        return 0;
    }
    float xOfVec[DATASIZE][DATASIZE];
    for (int i = 0; i < DATASIZE; i++) {
        for (int j = 0; j < DATASIZE; j++) {
            data >> xOfVec[i][j];
        }
        std::cout << std::endl;
    }
   float yOfVec[DATASIZE][DATASIZE];
    for (int i = 0; i < DATASIZE; i++) {
        for (int j = 0; j < DATASIZE; j++) {
            data >> yOfVec[i][j];
        }
    }
    int xGrid[DATASIZE], yGrid[DATASIZE];
    sf::VertexArray vectorField(sf::Lines);
    for (int i = 0; i < DATASIZE; i++) {
        xGrid[i] = yGrid[i] = i * 100;
        for (int j = 0; j < DATASIZE; j++) {
            plot(i*1000, j*1000, xOfVec[i][j]*1000, yOfVec[i][j]*1000, vectorField);
        }
    }
    std::cout << vectorField.getVertexCount();

    /*
    // A sample data plotting to test my plotting device
    int simplePlotData[10][10] = {
    { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
    { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
    { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
    { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
    { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
    { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
    { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
    { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
    { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
    { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 }
    };
    */

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        window.clear();
        window.draw(vectorField);
        window.display();
    }
    return 0;
}

void plot(int x1, int y1, float x2, float y2, sf::VertexArray &vectors) {
    plot(sf::Vector2f(x1, y1), sf::Vector2f(x2, y2), vectors);
}
void plot(sf::Vector2f from, sf::Vector2f to, sf::VertexArray &vectors) {
    sf::Vertex fromVex(from);
    sf::Vertex toVex(to);
    fromVex.color = sf::Color(64, 224, 208,255);
    toVex.color = sf::Color(64, 224, 208, 255);
    vectors.append(fromVex);
    vectors.append(toVex);
}