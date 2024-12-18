#include <iostream>
#include <thread>
#include <chrono>
#include <fstream>
#include <random>
#include "Windows.h"
#include <conio.h>
using namespace std;

int width = 20; // width and height of the game field
int height = 10;
int a_x = 0; // position of the apple
int a_y = 0;
int tail_x[50]; // arrays with coordinates of the tail (50 is the maximum default length of the snake)
int tail_y[50];
int score = 0; // initial score (also the length of the tail)
int x = width / 2; // initial position of the head (in the center of the field)
int y = height / 2;
int flag = 0; // for the draw() function
int nx, ny, ox, oy; // for moving the tail
bool game_over = false; // variable for the end of the game
int last_move = 0; // side of the last move (initially - right; 0 - right, 1 - up, 2 - left, 3 - down)
int game_speed = 1200; // game speed (field updates every this many milliseconds; optimal is 500)

void draw() {
    cout << string(100, '\n');
    cout << "Score: " << score << endl; // display the current score
    for (int i = 0; i <= height; i++) { // draw each element of the field
        for (int j = 0; j <= width; j++) {
            if (i == 0 || i == height || j == 0 || j == width) { // draw the boundaries
                cout << "#";
            } else if (j == x && i == y) { // draw the head
                cout << "O";
            } else if (i == a_y && j == a_x) {
                cout << "*";
            } else {
                flag = 0; // if it doesn't increase, it means there's no tail segment at these coordinates
                for (int k = 0; k <= score; k++) {
                    if (i == tail_y[k] && j == tail_x[k]) {
                        cout << "o"; // draw the tail
                        flag = 1; // tail element found
                    }
                }
                if (flag == 0) { // if there's no tail at this point, draw an empty segment (.)
                    cout << ".";
                }
            }
        }
        cout << endl; // move to the next line for drawing
    }
} // draw the field

void check() {
    random_device rd; // random number generator
    mt19937 gen(rd());
    if (x == width || y == height || y == 0 || x == 0) { // head intersects with the field boundary
        game_over = true; // end the game
    } else if (x == a_x && y == a_y) { // head intersects with the apple
        score++;
        a_x = gen() % (width - 2) + 1; // generate new apple position
        a_y = gen() % (height - 2) + 1;
    } else {
        for (int k = 0; k <= score; k++) { // check if the head intersects with any tail element
            if (y == tail_y[k] && x == tail_x[k]) {
                game_over = true; // end the game + interrupt the check
                break;
            }
        }
    }
} // check for intersections with the apple, body, or wall

void right() {
    if (score > 0) {
        nx = tail_x[0]; // record the position of the first tail segment
        ny = tail_y[0];

        tail_x[0] = x; // move the first tail segment to the head's position
        tail_y[0] = y;
        for (int i = 1; i <= score - 1; i++) {
            ox = tail_x[i]; // remember the position of the current tail segment
            oy = tail_y[i];
            tail_x[i] = nx; // move the current tail segment to the previous one's position (closer to the head)
            tail_y[i] = ny;
            nx = ox; // record the current segment as the previous for the next move
            ny = oy;
        }
    }
    x++; // move the head to the right
} // move right

void up() {
    if (score > 0) {
        nx = tail_x[0];
        ny = tail_y[0];
        tail_x[0] = x;
        tail_y[0] = y;
        for (int i = 1; i <= score - 1; i++) {
            ox = tail_x[i];
            oy = tail_y[i];
            tail_x[i] = nx;
            tail_y[i] = ny;
            nx = ox;
            ny = oy;
        }
    }
    y--;
} // move up

void down() {
    if (score > 0) {
        nx = tail_x[0];
        ny = tail_y[0];
        tail_x[0] = x;
        tail_y[0] = y;
        for (int i = 1; i <= score - 1; i++) {
            ox = tail_x[i];
            oy = tail_y[i];
            tail_x[i] = nx;
            tail_y[i] = ny;
            nx = ox;
            ny = oy;
        }
    }
    y++;
} // move down

void left() {
    if (score > 0) {
        nx = tail_x[0];
        ny = tail_y[0];
        tail_x[0] = x;
        tail_y[0] = y;
        for (int i = 1; i <= score - 1; i++) {
            ox = tail_x[i];
            oy = tail_y[i];
            tail_x[i] = nx;
            tail_y[i] = ny;
            nx = ox;
            ny = oy;
        }
    }
    x--;
} // move left

int main() {
    std::this_thread::sleep_for(std::chrono::milliseconds(2000)); // delay at the start of the game
    random_device rd; // random position generator for the apple
    mt19937 gen(rd());
    a_x = gen() % (width - 2) + 1; // ensure the apple doesn't land on the boundaries (width - 2) + 1
    a_y = gen() % (height - 2) + 1; // % - ensure the number falls within the range

    while (!game_over) {
        draw();
        if (GetAsyncKeyState(0x44) == -32767) { // check key presses
            right();
            last_move = 0;
        } else if (GetAsyncKeyState(0x57) == -32767) {
            up();
            last_move = 1;
        } else if (GetAsyncKeyState(0x41) == -32767) {
            left();
            last_move = 2;
        } else if (GetAsyncKeyState(0x53) == -32767) {
            down();
            last_move = 3;
        } else { // move in the last direction if no key is pressed
            if (last_move == 0) {
                right();
            } else if (last_move == 1) {
                up();
            } else if (last_move == 2) {
                left();
            } else if (last_move == 3) {
                down();
            }
        }
        check();
        std::this_thread::sleep_for(std::chrono::milliseconds(game_speed)); // delay in game step
        if (score == 50) {
            game_over = true;
        }
    }
    cout << string(100, '\n');
    cout << "Game over" << endl; // end of the game
    if (score == 50) {
        cout << "You win!" << endl;
    }
    cout << "Final score: " << score;
    return 0;
}
