#include <iostream>
#include <conio.h> // For keyboard input
#include <windows.h> // For Sleep
#include <vector>

using namespace std;

const int WIDTH = 30;
const int HEIGHT = 20;

class Point {
public:
    int x, y;
    Point(int x = 0, int y = 0) : x(x), y(y) {}
};

class Snake {
private:
    vector<Point> body;
    char direction; // Current direction: 'W', 'A', 'S', 'D'
    bool alive;

public:
    Snake(int startX, int startY) {
        body.push_back(Point(startX, startY)); // Initial head position
        direction = 'D'; // Default moving right
        alive = true;
    }

    bool isAlive() {
        return alive;
    }

    void setDirection(char newDirection) {
        // Prevent reverse direction
        if ((direction == 'W' && newDirection == 'S') ||
            (direction == 'S' && newDirection == 'W') ||
            (direction == 'A' && newDirection == 'D') ||
            (direction == 'D' && newDirection == 'A'))
            return;

        direction = newDirection;
    }

    void move(Point& food, bool& foodEaten) {
        Point head = body[0]; // Current head position
        switch (direction) {
        case 'W': head.y--; break; // Up
        case 'S': head.y++; break; // Down
        case 'A': head.x--; break; // Left
        case 'D': head.x++; break; // Right
        }

        // Check if the snake bites itself or hits the walls
        if (head.x < 0 || head.x >= WIDTH || head.y < 0 || head.y >= HEIGHT) {
            alive = false;
            return;
        }
        for (size_t i = 0; i < body.size(); i++) {
            if (body[i].x == head.x && body[i].y == head.y) {
                alive = false;
                return;
            }
        }

        // Add new head position
        body.insert(body.begin(), head);

        // Check if food is eaten
        if (head.x == food.x && head.y == food.y) {
            foodEaten = true;
        }
        else {
            // Remove the tail (snake moves forward)
            body.pop_back();
        }
    }

    void draw() {
        for (size_t i = 0; i < body.size(); i++) {
            if (i == 0)
                cout << "O"; // Head
            else
                cout << "o"; // Body
        }
    }

    vector<Point> getBody() {
        return body;
    }
};

class Game {
private:
    Snake snake;
    Point food;
    bool foodEaten;
    int score;

public:
    Game() : snake(WIDTH / 2, HEIGHT / 2), food(rand() % WIDTH, rand() % HEIGHT), foodEaten(false), score(0) {}

    void generateFood() {
        food.x = rand() % WIDTH;
        food.y = rand() % HEIGHT;
    }

    void draw() {
        system("cls");

        // Draw top border
        for (int i = 0; i < WIDTH + 2; i++) cout << "#";
        cout << endl;

        // Draw the field
        for (int y = 0; y < HEIGHT; y++) {
            cout << "#"; // Left border
            for (int x = 0; x < WIDTH; x++) {
                if (x == food.x && y == food.y) {
                    cout << "*"; // Food
                }
                else {
                    bool isBodyPart = false;
                    for (Point part : snake.getBody()) {
                        if (part.x == x && part.y == y) {
                            cout << "o"; // Snake part
                            isBodyPart = true;
                            break;
                        }
                    }
                    if (!isBodyPart) cout << " "; // Empty space
                }
            }
            cout << "#" << endl; // Right border
        }

        // Draw bottom border
        for (int i = 0; i < WIDTH + 2; i++) cout << "#";
        cout << endl;

        // Display score
        cout << "Score: " << score << endl;
    }

    void handleInput() {
        if (_kbhit()) {
            char input = _getch();
            snake.setDirection(toupper(input));
        }
    }

    void update() {
        snake.move(food, foodEaten);

        if (foodEaten) {
            score += 10;
            generateFood();
            foodEaten = false;
        }
    }

    bool isGameOver() {
        return !snake.isAlive();
    }
};

int main() {
    Game game;

    while (!game.isGameOver()) {
        game.draw();
        game.handleInput();
        game.update();
        Sleep(100); // Delay for smoother movement
    }

    cout << "Game Over! Final Score: " << game.isGameOver() << endl;
    return 0;
}
