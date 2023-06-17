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
#include "Screen_manager.h"
#include <algorithm>

using namespace std;

//move cursor
void cursorYX(int y, int x)
{
    COORD pos;
    pos.X = x;        
    pos.Y = y;            
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void Screen_manager::render(){
    for(int j=0; j<height; j++){
        for(int i=0; i<width; i++){
            cursorYX(j, i);
            printf("%c", board[j][i]);
        }
    }
}

//print sharing things.
void Screen_manager::print_share(){
    //wall
    for(int i=0; i<width; i++){board[height-1][i]='w';}
    for(int j=0; j<height; j++){board[j][0]='w'; board[j][width-1]='w';}

    int shot_frame, create_frame, check_frame;

    for (int i = 0; i < num_event; i++){
        if (curr_frame == frame_event[i]){ //object 생성
            switch (this->type_event[i]){
                case 'n': {
                    Enemy_1n *enn = new Enemy_1n(y_event[i], x_event[i], 10, 1, i, 'n');
                    this->objects.push_back(enn);
                    board[y_event[i]][x_event[i]] = 'n';
                    break;
                }
                case 'L': {
                    board[y_event[i]][x_event[i]] = 'L';
                    break;
                }
                case 'P': {
                    board[y_event[i]][x_event[i]] = 'P';
                    break;
                }
                case 'r': {
                    Enemy_2r *enn = new Enemy_2r(y_event[i], x_event[i], 5, 2, i, 'r', 3);
                    enn->seton();
                    this->objects.push_back(enn);
                    board[y_event[i]][x_event[i]] = 'r';
                    break;
                }
                default:
                    break;
            }
        } else if (curr_frame > frame_event[i] && this->type_event[i] != '0'){
            switch(this->type_event[i]){
                case 'n':{
                    board[y_event[i]][x_event[i]] = 'n';
                    plnenem(this->my_plane.y, this->my_plane.x);
                    break;
                }
                case 'r':{
                    board[y_event[i]][x_event[i]] = 'r';
                    plnenem(this->my_plane.y, this->my_plane.x);
                    break;
                }
                case 'L':{
                    if (this->my_plane.y == y_event[i] && this->my_plane.x == x_event[i]){
                        type_event[i] = '0';
                        this->my_plane.level ++;
                    } else{
                        board[y_event[i]][x_event[i]] = 'L';
                    }
                    break;
                }
                case 'P':{
                    if (this->my_plane.y == y_event[i] && this->my_plane.x == x_event[i]){
                        type_event[i] = '0';
                        this->my_plane.powerup = true;
                    } else{
                        board[y_event[i]][x_event[i]] = 'P';
                    }
                    break;
                }
                default:
                    break;
            }
        }
    }

    //Bullet part
    shot_frame = this->my_plane.shot_frame_my_plane;
    create_frame = this->my_plane.create_frame_my_plane;
    check_frame = this->my_plane.check_frame_my_plane;

    for (auto elem : this->objects){
        if (elem->getsymbol() == 'r'){
            movenemy(elem);
        }
    }

    while ((curr_frame-create_frame)/shot_frame - check_frame > 0){ //bullet create
        Bullet *bullet = new Bullet(this->my_plane.y-1+shot_frame, this->my_plane.x, check_frame, this->my_plane.level, this->my_plane.powerup);
        this->my_plane.bullet.push_back(bullet);

        for(auto iter=this->my_plane.bullet.begin(); iter<this->my_plane.bullet.end();){
            if((*iter)->y<=0){
                extrabullet((*iter)->y, (*iter)->x, (*iter)->powerup);
                board[(*iter)->y][(*iter)->x]=' ';
                this->my_plane.bullet.erase(iter);
            }
            else{
                if(iter!=(this->my_plane.bullet.end()-1) && curr_frame!=1){ //방금 생성된 new bullet이나 발사 전만 아니면
                    extrabullet((*iter)->y, (*iter)->x, (*iter)->powerup);
                    board[(*iter)->y][(*iter)->x]=' ';
                }
                (*iter)->y -= shot_frame;
                if ((*iter)->powerup == true){
                    if ((*iter)->x >=2){
                        board[(*iter)->y][(*iter)->x - 1]= (*iter)->symbol[(*iter)->level - 1];
                        bulenem((*iter)->y, (*iter)->x-1, (*iter)->level);
                    }
                    if ((*iter)->x <= 57){
                        board[(*iter)->y][(*iter)->x + 1]= (*iter)->symbol[(*iter)->level - 1];
                        bulenem((*iter)->y, (*iter)->x+1, (*iter)->level);
                    }
                }
                board[(*iter)->y][(*iter)->x]= (*iter)->symbol[(*iter)->level - 1];
                bulenem((*iter)->y, (*iter)->x, (*iter)->level);
                iter++;
            }
        }

        this->my_plane.check_frame_my_plane+=1;
        check_frame++;
    }
    //Bullet part ends
}

void Screen_manager::bulenem(int y, int x, int damage){
    for (auto iter = this->objects.begin(); iter < this->objects.end(); iter++){
        if (y == (*iter)->gety() && x == (*iter)->getx()){
            (*iter)->sethp((*iter)->gethp() - damage);
            if ((*iter)->gethp() <= 0){
                this->my_plane.score += (*iter)->getscore();
                this->type_event[(*iter)->getorder()] = '0';
                this->objects.erase(iter);
            }
        }
    }
}

void Screen_manager::plnenem(int y, int x){
    for (auto iter = this->objects.begin(); iter < this->objects.end(); iter++){
        if (y == (*iter)->gety() && x == (*iter)->getx()){
            this->my_plane.hp --;
            board[y][x] = 'M';
        }
    }
}

void Screen_manager::extrabullet(int y, int x, bool p){
    if (p == true){
        if (x >=2){
            board[y][x - 1]=' ';
        }
        if (x <= 57){
            board[y][x + 1]=' ';
        }
    }
}

void Screen_manager::movenemy(Enemy* enem){
    int create_frame = this->frame_event[enem->getorder()];
    int check_frame = 0;
    while ((curr_frame-create_frame)/enem->getcellspeed() - check_frame > 0){
        if (enem->gety() >= 29){
            board[enem->gety()][enem->getx()] = 'w';
            this->objects.erase(find(this->objects.begin(), this->objects.end(), enem));
        } else{
            board[enem->gety()][enem->getx()] = ' ';
            enem->sety(enem->gety() + 1);
            board[enem->gety()][enem->getx()] = enem->getsymbol();
        }
        check_frame++;
    }
}

//print when key didn't pressed
void Screen_manager::print(){
    board[this->my_plane.y][this->my_plane.x]='M';
    print_share();
}

//print when key pressed
void Screen_manager::print(int ch){ //ascii
    if(ch=='d'){ //right
        if(this->my_plane.y >=0 && this->my_plane.y <(height-1) && this->my_plane.x >0 && this->my_plane.x < (width-2)){
            board[this->my_plane.y][this->my_plane.x]=' ';
            this->my_plane.x +=1;
            board[this->my_plane.y][this->my_plane.x]='M';
        }
    }
    else if(ch=='a'){ //left
        if(this->my_plane.y >=0 && this->my_plane.y <(height-1) && this->my_plane.x >1 && this->my_plane.x < (width-1)){
            board[this->my_plane.y][this->my_plane.x]=' ';
            this->my_plane.x -=1;
            board[this->my_plane.y][this->my_plane.x]='M';
        }
    }
    else if(ch=='w'){ //up
        if(this->my_plane.y >0 && this->my_plane.y <(height-1) && this->my_plane.x >0 && this->my_plane.x < (width-1)){
            board[this->my_plane.y][this->my_plane.x]=' ';
            this->my_plane.y -=1;
            board[this->my_plane.y][this->my_plane.x]='M';
        }
    }
    else if(ch=='s'){ //down
        if(this->my_plane.y >=0 && this->my_plane.y <(height-2) && this->my_plane.x >0 && this->my_plane.x < (width-1)){
            board[this->my_plane.y][this->my_plane.x]=' ';
            this->my_plane.y +=1;
            board[this->my_plane.y][this->my_plane.x]='M';
        }    
    }

    print_share();
}