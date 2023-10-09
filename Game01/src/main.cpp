#include <iostream>
#include "SFML/Graphics.hpp"
#include <fstream>


class MyRectangle
{
public:

};

class MyCircle
{
public:
    std::string name;
    sf::Text label;
    sf::CircleShape circle;
    float speedX, speedY;

    MyCircle(std::string name, float positionX,
        float positionY, float speedX, float speedY,
        int r, int g, int b, float radius,
        sf::Font &font, int rText, int gText, 
        int bText, int textSize)
        : name(name), speedX(speedX), speedY(speedY)
    {
        // set initial position of circle
        circle.setPosition(positionX, positionY);
        // set radius of circle
        circle.setRadius(radius);
        // set color of circle
        circle.setFillColor(sf::Color(r, g, b));

        // set text to options passed in constructor
        label.setString(name);
        label.setCharacterSize(textSize);
        label.setFont(font);
        label.setFillColor(sf::Color(rText, gText, bText));
        label.setOrigin(0, label.getGlobalBounds().height/2);
        //label.setPosition(positionX, positionY);
    }
};

class GameConfig {
public:
    sf::RectangleShape rect;
    sf::Font font;
    std::vector<MyCircle> myCircles;
    int wWidth;
    int wHeight;
    int rText;
    int gText;
    int bText;
    int textSize;

    GameConfig(const std::string& filename) 
    { 
        loadShapesFromFile(filename);
    }

    /* loads window, font, and shape configurations from file.
    *  creates object for window, font, and specified shapes.
    *  assigns window and font object to class attributes.
    *  adds shapes to corresponding shape vector.
    */
    void loadShapesFromFile(const std::string& filename)
    {
        // open file and characters from file to display them
        std::ifstream fin(filename);
        if (!fin)
        {
            std::cerr << "Could not load font!\n";
            exit(-1);
        };

        // make temp string variable to store current token being read from file
        std::string temp;

        // loop through tokens in file 
        while (fin >> temp)
        {
            // if encountered token is Circle we create a CircleShape and add it to CircleShape vector
            if (temp == "Circle")
            {
                std::cout << "here \n";

                std::string name;
                float positionX;
                float positionY;
                float speedX;
                float speedY;
                int r;
                int g;
                int b;
                float radius;

                fin >> name >> positionX >> positionY >> speedX
                    >> speedY >> r >> g >> b >> radius;

                myCircles.push_back(MyCircle(name, positionX,
                    positionY, speedX, speedY, r, g, b, radius,
                    font, rText, gText, bText, textSize));

            }
            // if token is Rectangle we create RectangleShape and add it to RectangleShape vector
            else if (temp == "Rectangle")
            {
                std::string name;
                int positionX;
                int positionY;
                float speedX;
                float speedY;
                int r;
                int g;
                int b;
                int width;
                int height;

                fin >> name >> positionX >> positionY >> speedX
                    >> speedY >> r >> g >> b >> width >> height;
            }
            else if (temp == "Window")
            {
                int width;
                int height;

                fin >> width >> height;

                // set member variables to width and height tokens
                wWidth = width;
                wHeight = height;

            }
            else
            {
                std::string fontFilePath;
                int tSize;
                int r;
                int g;
                int b;

                fin >> fontFilePath >> tSize >> r >> g >> b;

                // attempt to load font from file
                if (!font.loadFromFile("bin/Arial.ttf"))
                {
                    // if we can't load font, print an error to the error console and exit
                    std::cerr << "Could not load font!\n";
                    exit(-1);
                }
                rText = r;
                gText = g;
                bText = b;
                textSize = tSize;
            }
        }
    }
};

int main()
{
    // create game config object that reads game configurations from text file
    GameConfig gameConfig("bin/config.txt");

    // create window object
    sf::RenderWindow window(sf::VideoMode(gameConfig.wWidth, gameConfig.wHeight), "SFML Works");
    // set frame limit to 60 frames per second
    window.setFramerateLimit(60);

    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
            { 
                window.close();
            }

            // this event is triggered when a key is pressed
            if (event.type == sf::Event::KeyPressed)
            {
                // print key that was pressed to console
                std::cout << "Key pressed with code = " << event.key.code << "\n";

                if (event.key.code == sf::Keyboard::X)
                {
                    // reverse the direction of circle in screen
                }
            }
        }

        // clear previous window before drawing shapes
        window.clear();

        // render circles stored shapes class with basic animation
        for (auto& element : gameConfig.myCircles)
        {
            // increment or decrement circle x and y position by speedX and speedY on every frame
            element.circle.setPosition(
                element.circle.getPosition().x + element.speedX,
                element.circle.getPosition().y + element.speedY
            );

            // animate label with same x and y position of circle plus offset
            int yLabelOffset = (element.circle.getGlobalBounds().height / 2) - 
                (element.label.getGlobalBounds().height / 2);
            int xLabelOffset = (element.circle.getGlobalBounds().width -
                element.label.getGlobalBounds().width) / 2;


            element.label.setPosition(
                element.circle.getPosition().x + element.speedX +
                xLabelOffset,
                element.circle.getPosition().y + yLabelOffset
                + element.speedY
            );

            std::cout << element.circle.getGlobalBounds().height << "\n";

            // handle out of bounds for each circle on x-axis
            if (element.circle.getPosition().x < 0 || 
                element.circle.getPosition().x 
                + element.circle.getRadius() * 2 > gameConfig.wWidth)
            {
                element.speedX *= -1;
            }

            // handle out of bounds for each circle on y-axis
            if (element.circle.getPosition().y < 0 ||
                element.circle.getPosition().y
                + element.circle.getRadius() * 2 > gameConfig.wHeight)
            {
                element.speedY *= -1;
            }
            window.draw(element.circle);
            window.draw(element.label);
        }

        // basic rendering function calls
        window.display();

    }
    return 0;
}