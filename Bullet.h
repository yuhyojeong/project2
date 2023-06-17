#ifndef BULLET_H
#define BULLET_H

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

class Bullet{
    public:
        Bullet(int y_value, int x_value, int frame_value, int level, bool powerup, char sym = '\'') \
        : y(y_value), x(x_value), create_frame_bullet(frame_value), level(level), powerup(powerup), sym(sym) {};
        Bullet(int y, int x, char sym) : y(y), x(x), sym(sym) {}
        int y, x;
        int create_frame_bullet; //initialize when it creates
        int check_frame_bullet;
        int level = 1;
        char symbol[3] = {'\'', '^', '!'};
        bool powerup;
        char getsymbol() {return sym;}
    private:
        char sym;
};

class Levelup_bullet: public Bullet{
    public:
        Levelup_bullet(int y, int x, char sym) : Bullet(y, x, sym) {}
};

class Powerup_bullet : public Bullet{
    public:
        Powerup_bullet(int y, int x, char sym) : Bullet(y, x, sym) {}
};

#endif