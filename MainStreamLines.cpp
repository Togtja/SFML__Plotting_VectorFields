#include "SFML\Graphics.hpp"
#include <fstream> //To read the file
#include <iostream> //Debug messages
#include <iomanip> //
#include <random> //To generete random numbers
#include <vector> //To easier keep arrays
#include "SFML_Vector_OP.h" //Help with vector


#define PI 3.14159265
#define DEG 180/PI
//Constants
const int DATASIZE = 100;
const int SCREENSIZE = 500;

struct VectorField {
    int size;
    sf::VertexArray* vecotrfields;
    sf::VertexArray* vectorTriangleDirection;

};
//Psudo Ransom number generator
std::random_device rd;
std::mt19937_64 gen(rd());
std::uniform_int_distribution<int> rnd(0, DATASIZE-1);

sf::Vector2i randomPointInField()
;
VectorField plotStreamLines(sf::Vector2f field[][DATASIZE], sf::Color colour = sf::Color::Cyan,
                            int nrVectors = 100, float step = 1, int scale = 1);
VectorField plotStreamBends(sf::Vector2f field[][DATASIZE], sf::Color colour = sf::Color::Cyan,
                                int nrVectors = 100, int stepForVector = 100, float step = 0.1, int scale = 1);

void plotLines(int x1, int y1, float x2, float y2, sf::VertexArray &vectors, sf::Color colour = sf::Color::Green);
void plotLines(sf::Vector2f from, sf::Vector2f to, sf::VertexArray &vectors, sf::Color colour = sf::Color::Green);
void plotPoints(sf::Vector2f point, std::vector<sf::CircleShape>& circles);
void plotDirectionTriangles(sf::Vector2f point, sf::Vector2f direction, std::vector<sf::CircleShape>& circles);

sf::VertexArray getDirectionTriangles(sf::Vector2f point, sf::Vector2f direction, sf::Color colour = sf::Color::Green);






//TEMP/DEBUG

int main() {

    //sf::RenderWindow debugwindow(sf::VideoMode(SCREENSIZE, SCREENSIZE), "VectorFields DEBUG");
    sf::RenderWindow windowline(sf::VideoMode(SCREENSIZE, SCREENSIZE), "VectorFields lines");
    sf::RenderWindow windowbend(sf::VideoMode(SCREENSIZE, SCREENSIZE), "VectorFields with bends");
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
    std::vector<sf::CircleShape> triangles;
    int nrVec = 1000;

    VectorField vfieldbend = plotStreamBends(dataVectors, sf::Color::Blue, 1000, 100, 0.5, SCREENSIZE / DATASIZE);
    VectorField vfieldLine = plotStreamLines(dataVectors, sf::Color::Cyan, 1000, 2, SCREENSIZE / DATASIZE);


    while (windowline.isOpen()) {
        sf::Event eventline;
        while (windowline.pollEvent(eventline)) {
            if (eventline.type == sf::Event::Closed) {
                windowline.close();
            }
        }
        sf::Event eventbend;
        while (windowbend.pollEvent(eventbend)) {
            if (eventbend.type == sf::Event::Closed) {
                windowbend.close();
            }
        }

        windowbend.clear();
        for (int i = 0; i < vfieldbend.size; i++) {
            windowbend.draw(vfieldbend.vecotrfields[i]);
            windowbend.draw(vfieldbend.vectorTriangleDirection[i]);
        }
        windowbend.display();
        windowline.clear();
        for (int i = 0; i < vfieldbend.size; i++) {
            windowline.draw(vfieldLine.vecotrfields[i]);
            windowline.draw(vfieldLine.vectorTriangleDirection[i]);
        }
        windowline.display();

    }
    return 0;
}

VectorField plotStreamLines(sf::Vector2f field[][DATASIZE], sf::Color colour,
                                  int nrVectors, float step, int scale) {
    return plotStreamBends(field, colour, nrVectors, 1, step, scale);
}

VectorField plotStreamBends(sf::Vector2f field[][DATASIZE], sf::Color colour,
                                int nrVectors , int stepForVector, float step, int scale) {
    VectorField stream;
    stream.vecotrfields = new sf::VertexArray[nrVectors];
    stream.size = nrVectors;
    stream.vectorTriangleDirection = new sf::VertexArray[nrVectors];
    for (int i = 0; i < nrVectors; i++) {
        stream.vecotrfields[i] = sf::VertexArray(sf::LinesStrip);
        sf::Vector2i randVec = randomPointInField();
        sf::Vector2f from = (sf::Vector2f) randVec * scale;
        sf::Vector2f to = (randVec + field[randVec.x][randVec.y] * step) * scale;
        for (int j = 0; j < stepForVector; j++) {
            plotLines(from, to, stream.vecotrfields[i], colour);
            from = to;
            int x = (from.x / (float)scale);
            int y = (from.y / (float)scale);
            if (x < 0 || y < 0 || x >= DATASIZE || y >= DATASIZE) {
                break;
            }
            to = from + field[x][y] * step;

        }
        stream.vectorTriangleDirection[i] = getDirectionTriangles(to, to - from, colour);
    }
    return stream;
}
void plotLines(int x1, int y1, float x2, float y2, sf::VertexArray &vectors, sf::Color colour) {
    plotLines(sf::Vector2f((float)x1, (float)y1), sf::Vector2f(x2, y2), vectors);
}
void plotLines(sf::Vector2f from, sf::Vector2f to, sf::VertexArray &vectors, sf::Color colour) {
    sf::Vertex fromVex(from);
    sf::Vertex toVex(to);
    fromVex.color =colour;
    toVex.color = colour;
    vectors.append(fromVex);

    vectors.append(toVex);
}

void plotPoints(sf::Vector2f point, std::vector<sf::CircleShape>& circles) {
    sf::CircleShape* newcircle = new sf::CircleShape;
    newcircle->setRadius(2);
    newcircle->setPosition(point);
    newcircle->setFillColor(sf::Color::Red);
    circles.push_back(*newcircle);
}

void plotDirectionTriangles(sf::Vector2f point, sf::Vector2f direction, std::vector<sf::CircleShape> &triangles) {
    double deg = atan(direction.y / direction.y);
    sf::CircleShape* triangle = new sf::CircleShape(2, 3);
    triangle->setOrigin(triangle->getRadius(), triangle->getRadius());
    triangle->setPosition(point);
    triangle->setFillColor(sf::Color(64, 224, 208, 255));
    triangle->rotate(deg * DEG);
    triangles.push_back(*triangle);
    std::cout << "Created triangle in (" << point.x << "," << point.y << ")\n";
}

sf::VertexArray getDirectionTriangles(sf::Vector2f point, sf::Vector2f direction, sf::Color colour) {
    float dx = direction.x - point.x, dy = direction.y - point.y;
    sf::Vector2f normal1(-dy, dx), normal2(dy, -dx);
    sf::VertexArray triangles(sf::Triangles, 3);
    triangles[0].position = point - (normilize(normal1) * 2);
    triangles[1].position = point + (normilize(normal1) * 2);
    triangles[2].position = point + (normilize(direction) * 2);


    triangles[0].color = triangles[1].color =  triangles[2].color = colour;

    return triangles;
}

sf::Vector2i randomPointInField() {
    return sf::Vector2i(rnd(gen), rnd(gen));
}

