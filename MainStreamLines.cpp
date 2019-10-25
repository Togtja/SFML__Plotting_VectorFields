#include "SFML\Graphics.hpp"
#include <fstream> //To read the file
#include <iostream> //Debug messages
#include <random> //To generete random numbers
#include <vector> //To easier keep arrays
//I know SFML has vector OP's but they for some reason does not work for me
#include "SFML_Vector_OP.h" //Help with vector operators


#define PI 3.14159265
#define DEG 180/PI
//Constants
const int DATASIZE = 100;
const int DERRIVED = DATASIZE - 2;
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
//Get vector, where x is from 0 to maxX -1 and y is 0 to maxY - 1
sf::Vector2i randomPointInField(int maxX, int maxY);
//Plot vectorfield's stream with lines of the size of step
VectorField plotStreamLines(const sf::Vector2f* field, int X, int Y,
                            sf::Color colour = sf::Color::Cyan, int nrVectors = 100,
                            float step = 1, float scale = 1);
//Plot vector field stream line where we take some step for each vector with the size of step
VectorField plotStreamBends(const sf::Vector2f* field, int X, int Y, 
                            sf::Color colour = sf::Color::Cyan, int nrVectors = 100, 
                            int stepForVector = 100, float step = 0.1, float scale = 1);
//Plot a line from (x1,y1) to (x2, y2), and append it to the VertexArray
void plotLines(int x1, int y1, float x2, float y2, sf::VertexArray &vectors,
               sf::Color colour = sf::Color::Green);
//Plot a line from 'from' to 'to' and append to the VertexArray
void plotLines(const sf::Vector2f from, const sf::Vector2f to,
               sf::VertexArray &vectors, sf::Color colour = sf::Color::Green);
//Plot a point at point and push it to the points std::vector
void plotPoints(sf::Vector2f point, std::vector<sf::CircleShape>& points,
                sf::Color colour = sf::Color::Red, float radius = 1);
//create a direction triangle as a VertexArtay at point in the direction of direction
sf::VertexArray getDirectionTriangles(sf::Vector2f point, sf::Vector2f direction,
                                      sf::Color colour = sf::Color::Green);
//Plot vectors based on their position, and with colour based on magnitute
//X is the width of the plotting
//Y is the height of the plotting
void plotVectors(const sf::Vector2f* vectors, const int X, const int Y,
                 std::vector<sf::CircleShape>& points, sf::Color colour1, sf::Color colour2, float scale);

//TEMP/DEBUG

void plotVectors(const float* vectors, int X, int Y,
                 std::vector<sf::CircleShape>& points, sf::Color colour1, sf::Color colour2, float scale);
void plotVectors(const std::vector<float>& vectors, int X, int Y,
                 std::vector<sf::CircleShape>& points, sf::Color colour1, sf::Color colour2, float scale) {

    plotVectors(vectors.data(), X, Y, points, colour1, colour2, scale);
}
int main() {

    sf::RenderWindow windowbend(sf::VideoMode(SCREENSIZE, SCREENSIZE), "VectorFields with bends");
    sf::RenderWindow windowspeed(sf::VideoMode(SCREENSIZE, SCREENSIZE), "VectorFields speed");
    sf::RenderWindow windowdiv(sf::VideoMode(SCREENSIZE, SCREENSIZE), "VectorFields divergence");
    sf::RenderWindow windowcurl(sf::VideoMode(SCREENSIZE, SCREENSIZE), "VectorFields curl");
    
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
    }
    float speed[DATASIZE*DATASIZE];
    for (int i = 0; i < DATASIZE; i++) {
        for (int j = 0; j < DATASIZE; j++) {
            data >> yOfVec[i][j];
            dataVectors[i][j] = sf::Vector2f(xOfVec[i][j], yOfVec[i][j]);
            speed[i*DATASIZE + j] = magnitude(dataVectors[i][j]);
        }
    }
    data.close();
    //The curl of the vectorfield
    //DATASIZE == 100, and DERRIVED == 98
    float curl[DERRIVED*DERRIVED];
    //Derivativ of the vectorfield
    float divergence[DERRIVED*DERRIVED];
    //from 1 to and including 98 (< 99)
    for (int i = 1; i < DATASIZE-1; i++) {
        for (int j = 1; j < DATASIZE-1; j++) {   
            //px is P/dx
            float px = (dataVectors[i][j + 1].x - dataVectors[i][j - 1].x) / 2;
            //qy is Q/dy
            float qy = (dataVectors[i + 1][j].y - dataVectors[i - 1][j].y) / 2;
            //py is P/dy
            float py = (dataVectors[i + 1][j].x - dataVectors[i - 1][j].x) / 2;
            //qx is Q/dx
            float qx = (dataVectors[i][j + 1].y - dataVectors[i][j - 1].y) / 2;
            
            //Subtract -1 so that i goes from 0 to and including 97
            divergence[(i - 1) * DERRIVED + (j - 1)] = px + qy;
            curl[(i - 1) * DERRIVED + (j - 1)] = qx - py;
        }
    }
    std::vector<sf::CircleShape> speedShape;
    std::vector<sf::CircleShape> derivedShape;
    std::vector<sf::CircleShape> curlShape;
    VectorField vfieldbend = plotStreamBends(&dataVectors[0][0], DATASIZE, DATASIZE,
                                sf::Color::Blue, 1000, 100, 0.5, SCREENSIZE / DATASIZE);

    plotVectors(&speed[0], DATASIZE, DATASIZE,
                speedShape, sf::Color::Blue, sf::Color::Yellow, SCREENSIZE/DATASIZE);

    plotVectors(&divergence[0], DERRIVED, DERRIVED,
                derivedShape, sf::Color::Blue, sf::Color::Yellow, SCREENSIZE / DERRIVED);
    plotVectors(&curl[0], DERRIVED, DERRIVED,
                curlShape, sf::Color::Blue, sf::Color::Yellow, SCREENSIZE / DERRIVED);


    while (windowspeed.isOpen() || windowbend.isOpen()
           || windowcurl.isOpen() || windowdiv.isOpen()) {
        sf::Event event;
        while (windowspeed.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                windowspeed.close();
            }
        }
        while (windowbend.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                windowbend.close();
            }
        }
        while (windowdiv.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                windowdiv.close();
            }
        }
        while (windowcurl.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                windowcurl.close();
            }
        }

        windowdiv.clear();
        for (auto& point : derivedShape) {
            windowdiv.draw(point);
        }

        windowdiv.display();

        windowcurl.clear();
        for (auto& point : curlShape) {
            windowcurl.draw(point);
        }

        windowcurl.display();

        windowbend.clear();
        for (int i = 0; i < vfieldbend.size; i++) {
            windowbend.draw(vfieldbend.vecotrfields[i]);
            windowbend.draw(vfieldbend.vectorTriangleDirection[i]);
        }
        windowbend.display();
        windowspeed.clear();
        for (auto& point : speedShape) {
            windowspeed.draw(point);
        }

        windowspeed.display();
    }
    return 0;
}

VectorField plotStreamLines(const sf::Vector2f* field, int X, int Y,
                            sf::Color colour, int nrVectors,
                            float step, float scale) {
    return plotStreamBends(field, X, Y, colour, nrVectors, 1, step, scale);
}
VectorField plotStreamBends(const sf::Vector2f* field, int X, int Y,
                            sf::Color colour, int nrVectors,
                            int stepForVector, float step, float scale) {
    VectorField stream;
    stream.vecotrfields = new sf::VertexArray[nrVectors];
    stream.size = nrVectors;
    stream.vectorTriangleDirection = new sf::VertexArray[nrVectors];
    for (int i = 0; i < nrVectors; i++) {
        stream.vecotrfields[i] = sf::VertexArray(sf::LinesStrip);
        sf::Vector2i randVec = randomPointInField(X,Y);
        sf::Vector2f from = (sf::Vector2f) randVec * scale;
        sf::Vector2f to = (randVec + field[X*randVec.x + randVec.y] * step) * scale;
        for (int j = 0; j < stepForVector; j++) {
            plotLines(from, to, stream.vecotrfields[i], colour);
            from = to;
            int x = (from.x / scale);
            int y = (from.y / scale);
            if (x < 0 || y < 0 || x >= X || y >= Y) {
                break;
            }
            to = from + field[X*x + y] * step;

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

void plotVectors(const sf::Vector2f* vectors, int X, int Y, 
                 std::vector<sf::CircleShape>& points, sf::Color colour1, sf::Color colour2, float scale) {
    float max, min;
    max = min = magnitude(vectors[0]);
    for (int i = 0; i < Y; i++) {
        for (int j = 0; j < X; j++) {
            float mag = magnitude(vectors[i*X + j]);
            if (mag > max) {
                max = mag;
            }
            if (mag < min) {
                min = mag;
            }
        }
    }
    for (int i = 0; i < Y; i++) {
        for (int j = 0; j < X; j++) {
            float mag = magnitude(vectors[i*X + j]);
            float frac = ((mag - min) / (max - min));;
            int r = (colour2.r - colour1.r) * frac + colour1.r;
            int g = (colour2.g - colour1.g) * frac + colour1.g;
            int b = (colour2.b - colour1.b) * frac + colour1.b;
            plotPoints(sf::Vector2f(j * scale, i * scale), points, sf::Color(r, g, b), scale/2 + 1);
        }
    }
}
void plotVectors(const float* vectors, int X, int Y,
                 std::vector<sf::CircleShape>& points, sf::Color colour1, sf::Color colour2, float scale) {
    float max, min;
    //Find the min in the array, and the max in the array
    max = min = vectors[0];
    for (int i = 0; i < Y; i++) {
        for (int j = 0; j < X; j++) {
            float mag = vectors[i*X + j];
            if (mag > max) {
                max = mag;
            }
            if (mag < min) {
                min = mag;
            }
        }
    }
    //Find  suitable values for their colour based on max and min
    for (int i = 0; i < Y; i++) {
        for (int j = 0; j < X; j++) {
            float mag = vectors[i*X + j];
            float frac = ((mag - min) / (max - min));;
            int r = (colour2.r - colour1.r) * frac + colour1.r;
            int g = (colour2.g - colour1.g) * frac + colour1.g;
            int b = (colour2.b - colour1.b) * frac + colour1.b;
            plotPoints(sf::Vector2f(j * scale, i * scale),
                       points, sf::Color(r, g, b), scale / 2 + 1);
        }
    }
}

sf::Vector2i randomPointInField(int x, int y) {
    std::uniform_int_distribution<int> rndx(0, x - 1);
    std::uniform_int_distribution<int> rndy(0, y - 1);
    return sf::Vector2i(rndx(gen), rndy(gen));
}

