#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

// constexpr defines an immutable compile time value
constexpr int windowWidth(800), windowHeight(600);

// constamts for the Ball class
constexpr float ballRadius{10.f}, ballVelocity{8.f};

struct Ball
{
    CircleShape shape;
    
    // 2D vector that stores Ball's velocity
    Vector2f velocity{-ballVelocity, -ballVelocity};
    
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
    
    // lets "update" the ball: move its shape
    // by the current velocity
    void update()
    {
        shape.move(velocity);
        
        // we need to keep the ball "inside the screen"
        
        // if it's leaving toward the left, we need to set
        // horizontal velocity to a positive value (towards the right)
        if (left() < 0) velocity.x = ballVelocity;
        
        // otherwise, if its leaving towards the right, we need to
        // set horizontal velocity to a negative value (towards the left)
        else if (right() > windowWidth) velocity.x = -ballVelocity;
        
        // the same idea can be applied for top/botto collisions
        if (top() < 0) velocity.y = ballVelocity;
        else if (bottom() > windowHeight) velocity.y = -ballVelocity;
    }
    
    // we can create "property" methods to easily
    // get commonly used values
    float x()       { return shape.getPosition().x; }
    float y()       { return shape.getPosition().y; }
    float left()    { return x() - shape.getRadius(); }
    float right()   { return x() + shape.getRadius(); }
    float top()     { return y() - shape.getRadius(); }
    float bottom()  { return y() + shape.getRadius(); }
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
    
        // every loop iteraton, we need to update the ball
        ball.update();
        
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
