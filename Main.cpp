// Filename: snake_game.cpp
// Description: Classic Snake Game for Windows Console
// Controls: W (Up), S (Down), A (Left), D (Right), X (Exit)

#include <iostream>
#include <conio.h>   // for _kbhit() and _getch()
#include <windows.h> // for Sleep() and gotoxy()
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

// Console dimensions
const int WIDTH = 40;
const int HEIGHT = 20;

// Game variables
bool gameOver;
int score;
int x, y;           // Snake head position
int fruitX, fruitY; // Fruit position
int dir;            // Current direction
vector<int> tailX, tailY; // Snake tail coordinates
int tailLength;

// Direction constants
enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
eDirection currentDir;

// Function to move cursor to (x, y) in console
void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// Hide cursor to prevent flickering
void hideCursor() {
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

// Initial setup
void Setup() {
    gameOver = false;
    dir = STOP;
    currentDir = STOP;
    x = WIDTH / 2;
    y = HEIGHT / 2;
    
    // Random fruit position
    srand(time(0));
    fruitX = rand() % (WIDTH - 2) + 1;
    fruitY = rand() % (HEIGHT - 2) + 1;
    
    score = 0;
    tailLength = 0;
    tailX.clear();
    tailY.clear();
}

// Draw the game board
void Draw() {
    gotoxy(0, 0);
    
    // Top border
    for (int i = 0; i < WIDTH + 2; i++)
        cout << "#";
    cout << endl;
    
    // Middle area
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH + 2; j++) {
            if (j == 0 || j == WIDTH + 1)
                cout << "#"; // Side borders
            
            // Snake head
            else if (i == y && j == x)
                cout << "O";
            
            // Fruit
            else if (i == fruitY && j == fruitX)
                cout << "@";
            
            // Tail
            else {
                bool printTail = false;
                for (int k = 0; k < tailLength; k++) {
                    if (tailX[k] == j && tailY[k] == i) {
                        cout << "o";
                        printTail = true;
                        break;
                    }
                }
                if (!printTail)
                    cout << " ";
            }
        }
        cout << endl;
    }
    
    // Bottom border
    for (int i = 0; i < WIDTH + 2; i++)
        cout << "#";
    cout << endl;
    
    // Score display
    cout << "Score: " << score << endl;
    cout << "Controls: W (Up)  S (Down)  A (Left)  D (Right)  X (Exit)" << endl;
}

// Handle keyboard input
void Input() {
    if (_kbhit()) {
        char key = _getch();
        key = tolower(key);
        
        switch (key) {
            case 'a':
                if (currentDir != RIGHT) currentDir = LEFT;
                break;
            case 'd':
                if (currentDir != LEFT) currentDir = RIGHT;
                break;
            case 'w':
                if (currentDir != DOWN) currentDir = UP;
                break;
            case 's':
                if (currentDir != UP) currentDir = DOWN;
                break;
            case 'x':
                gameOver = true;
                break;
        }
    }
}

// Update game logic
void Logic() {
    // Store previous head position for tail movement
    int prevX = tailX.empty() ? x : tailX[0];
    int prevY = tailY.empty() ? y : tailY[0];
    int prev2X, prev2Y;
    
    // Move tail
    if (tailLength > 0) {
        tailX[0] = x;
        tailY[0] = y;
        for (int i = 1; i < tailLength; i++) {
            prev2X = tailX[i];
            prev2Y = tailY[i];
            tailX[i] = prevX;
            tailY[i] = prevY;
            prevX = prev2X;
            prevY = prev2Y;
        }
    }
    
    // Move head based on direction
    switch (currentDir) {
        case LEFT:  x--; break;
        case RIGHT: x++; break;
        case UP:    y--; break;
        case DOWN:  y++; break;
        default: break;
    }
    
    // Wall collision (Game Over)
    if (x <= 0 || x >= WIDTH + 1 || y < 0 || y >= HEIGHT) {
        gameOver = true;
    }
    
    // Tail collision (Game Over)
    for (int i = 0; i < tailLength; i++) {
        if (tailX[i] == x && tailY[i] == y) {
            gameOver = true;
        }
    }
    
    // Fruit collision
    if (x == fruitX && y == fruitY) {
        score += 10;
        tailLength++;
        
        // Add new tail segment
        if (tailLength == 1) {
            tailX.push_back(prevX);
            tailY.push_back(prevY);
        } else {
            tailX.push_back(prevX);
            tailY.push_back(prevY);
        }
        
        // Generate new fruit (ensure it's not on snake)
        bool validPosition;
        do {
            validPosition = true;
            fruitX = rand() % (WIDTH - 1) + 1;
            fruitY = rand() % (HEIGHT - 1) + 1;
            
            // Check if fruit spawns on tail
            for (int i = 0; i < tailLength; i++) {
                if (tailX[i] == fruitX && tailY[i] == fruitY) {
                    validPosition = false;
                    break;
                }
            }
        } while (!validPosition);
    }
}

int main() {
    hideCursor();
    Setup();
    
    cout << "=== SNAKE GAME ===" << endl;
    cout << "Press any key to start..." << endl;
    _getch();
    
    while (!gameOver) {
        Draw();
        Input();
        Logic();
        Sleep(70); // Game speed (lower = faster)
    }
    
    system("cls");
    cout << "=== GAME OVER ===" << endl;
    cout << "Final Score: " << score << endl;
    cout << "Press any key to exit..." << endl;
    _getch();
    
    return 0;
}
