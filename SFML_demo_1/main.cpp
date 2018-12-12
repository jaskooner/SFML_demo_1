#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

// constexpr defines an immutable compile time value
constexpr int windowWidth(800), windowHeight(600);
constexpr float ballRadius{10.f};

struct Ball
{
    CircleShape shape;
    
    // Ball constructor
    // argument mX -> starting x coord
    // argument mY -> starting y coord
    Ball(float mX, float mY)
    {
        shape.setPosition(mX, mY);
        shape.setRadius(ballRadius);
        shape.setFillColor(Color::Red);
        shape.setOrigin(ballRadius, ballRadius);
    }
};

int main() {
    
    // create an instance of Ball
    // positioned at the center of the window
    Ball ball{windowWidth / 2, windowHeight / 2};
    
    // creation of the game window
    RenderWindow window{ {windowWidth, windowHeight}, "Arknoid - 1"};
    window.setFramerateLimit(60);
    
    // game loop
    while (true) {
        // clear the window from previously drwan graphics
        window.clear(Color::Black);
        
        // if 'escape' is pressed, break out of the loop
        if(Keyboard::isKeyPressed(Keyboard::Key::Escape)) break;
    
        // render the ball instance on the window
        window.draw(ball.shape);
        window.display();
        
        // in some OS necesary to call
        Event event;
        window.pollEvent(event);
        // in the game lop to prevent window freezing
    }
    
    return 0;
}
