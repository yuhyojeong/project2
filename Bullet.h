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
        Bullet(int y_value, int x_value, int frame_value, int level) \
        : y(y_value), x(x_value), create_frame_bullet(frame_value), level(level) {};
        int y, x;
        int create_frame_bullet; //initialize when it creates
        int check_frame_bullet;
        int level;
        char symbol[3] = {'\'', '^', '!'};
};

class Levelup_bullet: public Bullet{
    public:
        Levelup_bullet(int y, int x, int frame, int level) : Bullet(y, x, frame, level) {}
};
#endif