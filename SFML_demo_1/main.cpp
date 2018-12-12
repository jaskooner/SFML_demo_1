#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

// constexpr defines an immutable compile time value
constexpr int windowWidth(800), windowHeight(600);

// constamts for the Ball class
constexpr float ballRadius{10.f}, ballVelocity{8.f};

// constants for the paddle
constexpr float paddleWidth{60.f}, paddleHeight{20.f}, paddleVelocity{6.f};

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

// a 'Paddle' class, similar to Ball
struct Paddle
{
    RectangleShape shape;
    Vector2f velocity;
    
    Paddle(float mX, float mY)
    {
        shape.setPosition(mX, mY);
        shape.setSize({paddleWidth, paddleHeight});
        shape.setFillColor(Color::Red);
        shape.setOrigin(paddleWidth / 2.f, paddleHeight / 2.f);
    }
    
    void update()
    {
        shape.move(velocity);
        
        // to move the paddle, we check if the user is pressing
        // the left or right arrow key: if so, we change the velocity
        
        // to keep the paddle "inside the window", we change the velocity
        // of if it s inside the window
        if (Keyboard::isKeyPressed(Keyboard::Key::Left)
            && left() > 0) velocity.x = -paddleVelocity;
        else if (Keyboard::isKeyPressed(Keyboard::Key::Right)
                 && right() < windowWidth) velocity.x = paddleVelocity;
        
        // if the user isn't pressing anything. stop moving
        else velocity.x = 0;
        
    }
    
    float x()           { return shape.getPosition().x; }
    float y()           { return shape.getPosition().y; }
    float left()        { return x() - shape.getSize().x / 2.f; }
    float right()       { return  x() + shape.getSize().x / 2.f; }
    float top()         { return y() - shape.getSize().y / 2.f; }
    float bottom()      { return y() + shape.getSize().y / 2.f; }
};

int main() {
    
    // create an instance of Ball
    // positioned at the center of the window
    Ball ball{windowWidth / 2, windowHeight / 2};
    
    // create a Paddle instance
    Paddle paddle{windowWidth / 2, windowHeight - 50};
    
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
        
        // update our paddle
        paddle.update();
        
        // render the ball instance on the window
        window.draw(ball.shape);
        // render paddle
        window.draw(paddle.shape);
        window.display();
        
        // in some OS necesary to call
        Event event;
        window.pollEvent(event);
        // in the game lop to prevent window freezing
    }
    
    return 0;
}
