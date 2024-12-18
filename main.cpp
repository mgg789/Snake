#include <iostream>
#include <thread>
#include <chrono>
#include <fstream>
#include <random>
#include "Windows.h"
#include <conio.h>
using namespace std;


int width = 20; // ширина и высота игрового поля
int height = 10;
int a_x = 0; // положение яблока
int a_y = 0;
int tail_x [50]; // списки с координатами хвоста (50 - максимальная длина змейки по умолчанию)
int tail_y [50];
int score = 0; // начальный счёт (AK длина хвоста)
int x = width / 2; // начальное положение головы (в центре поля)
int y = height / 2;
int flag = 0; // для функции draw()
int nx, ny, ox, oy; // для перемещения хвоста
bool game_over = false; // переменная об окончании игры
int last_move = 0; // сторона последнего перемещения (вначале - право; 0 - r, 1 - u, 2 - l, 3 - d)
int game_speed = 1200; // скорость игры (раз в столько миллисекунд обновляется поле; оптимальное - 500)


void draw() {
    cout << string( 100, '\n' );
    cout << "Score: " << score << endl; // вывод текущего счёта
    for (int i = 0; i <= height; i++) { // отрисовка каждого элемента поля
        for (int j = 0; j <= width; j++) {
            if (i == 0 || i == height || j == 0 || j == width) { // отрисовка границ
                cout  << "#";
            } else if (j == x && i == y) { // отрисовка головы
                cout  << "O";
            } else if (i == a_y && j == a_x) {
                cout  << "*";
            } else {
                flag = 0; // если не увеличится, значит в этих координатах нет ни одного сегмента хвоста
                for (int k = 0; k <= score; k++) {
                    if (i == tail_y[k] && j == tail_x[k]) {
                        cout  << "o"; // отрисовка хвоста
                        flag = 1; // найден элемент хвоста
                    }
                }
                if (flag == 0) { // если нет хвоста в этой точке, рисуем пустой сегмент (.)
                    cout  << ".";
                }
            }
        }
            cout << endl; // переход на отрисовку следующей строки
    }
} // отрисовка поля
void check() {
    random_device rd; //генератор случайных чисел
    mt19937 gen(rd());
    if (x == width || y == height || y == 0 || x == 0) { // пересечение головы с границей поля
        game_over = true; // завершение игры
    } else if (x == a_x && y == a_y) { // пересечение головы с яблоком
        score++;
        a_x = gen() % (width - 2) + 1; // генерация новой позиции яблока
        a_y = gen() % (height  - 2) + 1;
    } else {
        for (int k = 0; k <= score; k++) { // проверка на пересечение голвы с каждым элементом хвоста
            if (y == tail_y[k] && x == tail_x[k]) {
                game_over = true; // завершение игры + прерывание проверки
                break;
            }
        }
    }
} // проверка на пересечения с яблоком, телом или стенкой


void right() {
    if (score > 0) {
        nx = tail_x[0]; // записываем положение первого элемента хвоста
        ny = tail_y[0];

        tail_x[0] = x; //перемещение первого элемента хвоста на место головы
        tail_y[0] = y;
        for (int i = 1; i <= score - 1; i++) {
            ox = tail_x[i]; //запоминаем положение текущего элемента хвоста
            oy = tail_y[i];
            tail_x[i] = nx; //перемещение текущего элемента хвоста на место предыдущего (ближ. к голове)
            tail_y[i] = ny;
            nx = ox; //записываем текущий элемент как предыдущий для следующего перемещения
            ny = oy;
        }
    }
    x++; //перемещаем голову вправо
} // перемещение вправо
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
} // перемещение вверх
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

} // перемещение влево
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
} // перемещение вниз


int main() {
    std::this_thread::sleep_for(std::chrono::milliseconds(2000)); // задержка в начале игры
    random_device rd; //генератор случайных позиций яблока
    mt19937 gen(rd());
    a_x = gen() % (width - 2) + 1; // для того что бы яблоко не попала на границы (d - 2) + 1
    a_y = gen() % (height  - 2) + 1; // % - сделать так что бы число попадало в диапозон

    while (!game_over) {
        draw();
        if (GetAsyncKeyState(0x44) == -32767) { //проверка нажатий на кнопки
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
        } else { //перемещение в прошлую же сторону если кнопки не нажаты
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
        std::this_thread::sleep_for(std::chrono::milliseconds(game_speed)); // задержка в шаге игры
        if (score == 50) {
            game_over = true;
        }
    }
    cout << string( 100, '\n' );
    cout << "Game over" << endl; // окончание игры
    if (score == 50) {
        cout << "You win!" << endl;
    }
    cout << "Final score: " << score;
    return 0;
}