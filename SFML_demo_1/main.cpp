#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

// constexpr defines an immutable compile time value
constexpr int windowWidth(800), windowHeight(600);
constexpr float ballRadius{10.f}, ballVelocity{8.f};
constexpr float paddleWidth{60.f}, paddleHeight{20.f}, paddleVelocity{6.f};

// define constants for the bricks
constexpr float blockWidth{60.f}, blockHeight{20.f};
constexpr int countBlocksX{11}, countBlocksY{4};

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


struct Brick
{
    RectangleShape shape;
    
    // this boolean value will be used to check
    // whether a brick has been hit or not
    bool destroyed{false};
    
    Brick(float mX, float mY)
    {
        shape.setPosition(mX, mY);
        shape.setSize({blockWidth, blockHeight});
        shape.setFillColor(Color::Yellow);
        shape.setOrigin(blockWidth / 2.f, blockHeight / 2.f);
    }
    
    float x()           { return shape.getPosition().x; }
    float y()           { return shape.getPosition().y; }
    float left()        { return x() - shape.getSize().x / 2.f; }
    float right()       { return x() + shape.getSize().x / 2.f; }
    float top()         { return y() - shape.getSize().y / 2.f; }
    float bottom()      { return y() + shape.getSize().y / 2.f; }
};

// Dealing with collisions: lets define a generic function
// to check if two shapes are intersecting (colliding)
template <class T1, class T2>
bool isIntersecting(T1& mA, T2& mB) {
    return mA.right() >= mB.left() && mA.left() <= mB.right()
    && mA.bottom() >= mB.top() && mA.top() <= mB.bottom();
}

// lets defines a function that deals with paddle/ball collision
void testCollision(Paddle& mPaddle, Ball& mBall)
{
    // if there no intersection, get out of the function
    if (!isIntersecting(mPaddle, mBall)) return;
    
    // otherwise lets push the ball upwards
    mBall.velocity.y = -ballVelocity;
    
    // and lets direct it dependently on the position where the
    // paddle was hit
    if (mBall.x() < mPaddle.x()) mBall.velocity.x = -ballVelocity;
    else mBall.velocity.x = ballVelocity;
}

// ball brick collission
void testCollision(Brick& mBrick, Ball& mBall)
{
    // if there is no intersection, get out of the function
    if(!isIntersecting(mBrick, mBall)) return;
    
    // otherwide, the brik has been hit
    
}

int main() {
    
    Ball ball{windowWidth / 2, windowHeight / 2};
    Paddle paddle{windowWidth / 2, windowHeight - 50};
    
    // we will use a 'std::vector' to contain any number
    // of Bricks instances
    vector<Brick> bricks;
    
    // we will fill up our vector via a 2D for loop, creating
    // bricks in a gride-like pattern
    for (int iX{0}; iX < countBlocksX; ++iX)
        for (int iY{0}; iY < countBlocksY; ++iY)
            bricks.emplace_back((iX + 1) * (blockWidth + 3) + 32, (iY + 2) * (blockHeight + 3));
    
    
    
    // creation of the game window
    RenderWindow window{ {windowWidth, windowHeight}, "Arknoid - 7"};
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
        
        // lets test the collision every game loop
        testCollision(paddle, ball);
        
        // render the ball instance on the window
        window.draw(ball.shape);
        // render paddle
        window.draw(paddle.shape);
        
        // we must draw every brick on the window
        // let usea modern c++11 foreach loop, that allows
        // us to intuitively sat : 'draw every 'brick' in bricks
        for(auto& brick : bricks) window.draw(brick.shape);
        window.display();
        
        // in some OS necesary to call
        Event event;
        window.pollEvent(event);
        // in the game lop to prevent window freezing
    }
    
    return 0;
}
