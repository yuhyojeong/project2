#ifndef MY_PLANE_H
#define MY_PLANE_H

#include <stdio.h>
#include <locale.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <Windows.h>
#include <conio.h>
#include <chrono>
#include <vector>
#include "Bullet.h"

using namespace std;

class My_plane{
    public:
        My_plane(int y_value, int x_value, int frame_value) \
        : y(y_value), x(x_value), create_frame_my_plane(frame_value) {};
        int y, x;
        int shot_frame_my_plane=1; //몇 frame마다 bullet 생성하는지
        int create_frame_my_plane; //initialize when it creates
        int check_frame_my_plane=0; //previous value of (curr-create) / shot
        vector<Bullet*> bullet;
        int hp = 5;
        int score = 0;
        int level = 1;
        bool powerup = false;
        vector<Bullet*> buff;
};

class Enemy{
    public:
        Enemy(int y, int x, int hp, int score, int order, char symbol, int cellspeed) : y(y), x(x), hp(hp), score(score), order(order), symbol(symbol), cellspeed(cellspeed) {}
        Enemy(int y, int x, int hp, int score, int order, char symbol) : y(y), x(x), hp(hp), score(score), order(order), symbol(symbol) {}
        int gety() {return y;}
        void sety(int i) {y = i;}
        int getx() {return x;}
        int gethp() {return hp;}
        void sethp(int i) {hp = i;}
        int getorder() {return order;}
        int getscore() {return score;}
        char getsymbol() {return symbol;}
        void seton() {on = true;}
        bool geton() {return on;}
        int getcellspeed() {return cellspeed;}

    private:
    int y, x, hp, order, score, cellspeed;
    bool on = false;
    char symbol;
};

class Enemy_1n : public Enemy{
    public:
        Enemy_1n(int y, int x, int hp, int score, int order, char symbol) : Enemy(y, x, hp, score, order, symbol) {}

};

class Enemy_2r : public Enemy{
    public:
        Enemy_2r(int y, int x, int hp, int score, int order, char symbol, int cellspeed) : Enemy(y, x, hp, score, order, symbol, cellspeed) {}
};
#endif