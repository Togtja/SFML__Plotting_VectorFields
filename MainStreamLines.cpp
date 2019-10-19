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
    
    ~VectorField() {
        /*
        if (vecotrfields) {
            delete[] vecotrfields;
        }
        if (vectorTriangleDirection) {
            delete[] vectorTriangleDirection;
        }
        vecotrfields = nullptr;
        vectorTriangleDirection = nullptr;
        */
        size = 0;
    }
    

};
//Psudo Random number generator
std::random_device rd;
std::mt19937_64 gen(rd());
std::uniform_int_distribution<int> rnd(0, DATASIZE-1);

sf::Vector2i randomPointInField()
;
VectorField plotStreamLines(const sf::Vector2f field[][DATASIZE], sf::Color colour = sf::Color::Cyan,
                            int nrVectors = 100, float step = 1, float scale = 1);
VectorField plotStreamBends(const sf::Vector2f field[][DATASIZE], sf::Color colour = sf::Color::Cyan,
                                int nrVectors = 100, int stepForVector = 100, float step = 0.1, float scale = 1);

void plotLines(int x1, int y1, float x2, float y2, sf::VertexArray &vectors, sf::Color colour = sf::Color::Green);
void plotLines(const sf::Vector2f from, const sf::Vector2f to, sf::VertexArray &vectors, sf::Color colour = sf::Color::Green);
void plotPoints(sf::Vector2f point, std::vector<sf::CircleShape>& circles, sf::Color colour = sf::Color::Red, float radius = 1);
void plotDirectionTriangles(sf::Vector2f point, sf::Vector2f direction, std::vector<sf::CircleShape>& circles);

sf::VertexArray getDirectionTriangles(sf::Vector2f point, sf::Vector2f direction, sf::Color colour = sf::Color::Green);



void plotVectors(const sf::Vector2f vectors[][DATASIZE], std::vector<sf::CircleShape>&points, sf::Color colour1 = sf::Color::Blue, sf::Color colour2 = sf::Color::Red, float scale = 1);




//TEMP/DEBUG
void plotVectors(const float vectors[DATASIZE*DATASIZE], std::vector<sf::CircleShape>& points, sf::Color colour1, sf::Color colour2, float scale) {
    float max, min;
    max = min = vectors[0];
    for (int i = 0; i < DATASIZE; i++) {
        for (int j = 0; j < DATASIZE; j++) {
            float mag = vectors[i*DATASIZE + j];
            if (mag > max) {
                max = mag;
            }
            if (mag < min) {
                min = mag;
            }
        }
    }
    for (int i = 0; i < DATASIZE; i++) {
        for (int j = 0; j < DATASIZE; j++) {
            float mag = vectors[i*DATASIZE + j];
            float frac = ((mag - min) / (max - min));;
            int r = (colour2.r - colour1.r) * frac + colour1.r;
            int g = (colour2.g - colour1.g) * frac + colour1.g;
            int b = (colour2.b - colour1.b) * frac + colour1.b;
            plotPoints(sf::Vector2f(i * scale, j * scale), points, sf::Color(r, g, b), scale / 2 + 1);
        }
    }
}
int main() {

    //sf::RenderWindow debugwindow(sf::VideoMode(SCREENSIZE, SCREENSIZE), "VectorFields DEBUG");
    sf::RenderWindow windowspeed(sf::VideoMode(SCREENSIZE, SCREENSIZE), "VectorFields speed");
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
    //Derivativ of the vectorfield
    float derived[DATASIZE*DATASIZE];
    for (int i = 0; i < DATASIZE; i++) {
        for (int j = 0; j < DATASIZE; j++) {
            float h = 0.0001f;// (sqrt(DBL_EPSILON));
            sf::Vector2f f = dataVectors[i][j];
            sf::Vector2f df = sf::Vector2f(i, j);
            derived[i*DATASIZE + j] =  (((f.x + df.x * h) - (f.x - df.x * h)) / 2 * h) + (((f.y + df.y * h) - (f.y - df.y * h)) / 2 * h);
        }
    }


    float curl[DATASIZE*DATASIZE];
    for (int i = 0; i < DATASIZE; i++) {
        for (int j = 0; j < DATASIZE; j++) {
            float h = 0.0001f;// (sqrt(DBL_EPSILON));
            sf::Vector2f f = dataVectors[i][j];
            sf::Vector2f df = sf::Vector2f(i, j);
            curl[i*DATASIZE + j] = (((f.y + df.x * h) - (f.y - df.x * h)) / 2 * h) - (((f.x + df.y * h) - (f.x - df.y * h)) / 2 * h);
        }
    }

    std::vector<sf::CircleShape> speedShape;
    std::vector<sf::CircleShape> derivedShape;
    std::vector<sf::CircleShape> curlShape;
    int nrVec = 1000;

    VectorField vfieldbend = plotStreamBends(dataVectors, sf::Color::Blue, 1000, 100, 0.5, SCREENSIZE / DATASIZE);
    VectorField vfieldLine = plotStreamLines(dataVectors, sf::Color::Cyan, 1000, 2, SCREENSIZE / DATASIZE);
    plotVectors(dataVectors, speedShape, sf::Color::Yellow, sf::Color::Blue, SCREENSIZE/DATASIZE);
    plotVectors(derived, derivedShape, sf::Color::Yellow, sf::Color::Blue, SCREENSIZE / DATASIZE);
    plotVectors(curl, curlShape, sf::Color::Yellow, sf::Color::Blue, SCREENSIZE / DATASIZE);






    while (windowspeed.isOpen()) {
        sf::Event eventspeed;
        while (windowspeed.pollEvent(eventspeed)) {
            if (eventspeed.type == sf::Event::Closed) {
                windowspeed.close();
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
        windowspeed.clear();
        for (auto& point : curlShape) {
            windowspeed.draw(point);
        }

        windowspeed.display();

    }
    return 0;
}

VectorField plotStreamLines(const sf::Vector2f field[][DATASIZE], sf::Color colour,
                                  int nrVectors, float step, float scale) {
    return plotStreamBends(field, colour, nrVectors, 1, step, scale);
}

VectorField plotStreamBends(const sf::Vector2f field[][DATASIZE], sf::Color colour,
                                int nrVectors , int stepForVector, float step, float scale) {
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
            int x = (from.x / scale);
            int y = (from.y / scale);
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

void plotPoints(sf::Vector2f point, std::vector<sf::CircleShape>& circles, sf::Color colour, float radius) {
    sf::CircleShape* newcircle = new sf::CircleShape(radius, 4);
    newcircle->setOrigin(radius, radius);
    newcircle->rotate(45);
    newcircle->setPosition(point + (radius/2));
    newcircle->setFillColor(colour);
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

void plotVectors(const sf::Vector2f vectors[][DATASIZE], std::vector<sf::CircleShape>& points, sf::Color colour1, sf::Color colour2, float scale) {
    float max, min;
    max = min = magnitude(vectors[0][0]);
    for (int i = 0; i < DATASIZE; i++) {
        for (int j = 0; j < DATASIZE; j++) {
            float mag = magnitude(vectors[i][j]);
            if (mag > max) {
                max = mag;
            }
            if (mag < min) {
                min = mag;
            }
        }
    }
    for (int i = 0; i < DATASIZE; i++) {
        for (int j = 0; j < DATASIZE; j++) {
            float mag = magnitude(vectors[i][j]);
            float frac = ((mag - min) / (max - min));;
            int r = (colour2.r - colour1.r) * frac + colour1.r;
            int g = (colour2.g - colour1.g) * frac + colour1.g;
            int b = (colour2.b - colour1.b) * frac + colour1.b;
            plotPoints(sf::Vector2f(i * scale, j * scale), points, sf::Color(r, g, b), scale/2 + 1);
        }
    }
}

sf::Vector2i randomPointInField() {
    return sf::Vector2i(rnd(gen), rnd(gen));
}

