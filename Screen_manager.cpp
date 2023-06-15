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

    //Bullet part
    shot_frame = this->my_plane.shot_frame_my_plane;
    create_frame = this->my_plane.create_frame_my_plane;
    check_frame = this->my_plane.check_frame_my_plane;
    while ((curr_frame-create_frame)/shot_frame - check_frame > 0){ //bullet create
        Bullet *bullet = new Bullet(this->my_plane.y-1+shot_frame, this->my_plane.x, check_frame);
        this->my_plane.bullet.push_back(bullet);

        for(auto iter=this->my_plane.bullet.begin(); iter<this->my_plane.bullet.end();){
            if((*iter)->y<=0){
                board[(*iter)->y][(*iter)->x]=' ';
                this->my_plane.bullet.erase(iter);
            }
            else{
                if(iter!=(this->my_plane.bullet.end()-1) && curr_frame!=1){ //방금 생성된 new bullet이나 발사 전만 아니면
                    board[(*iter)->y][(*iter)->x]=' ';
                }
                (*iter)->y -= shot_frame;
                board[(*iter)->y][(*iter)->x]= bullet->symbol[bullet->level - 1];
                bulenem((*iter)->y, (*iter)->x, bullet->damage);
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

//print when key didn't pressed
void Screen_manager::print(){
    board[this->my_plane.y][this->my_plane.x]='M';
    for (int i = 0; i < num_event; i++){
        if (curr_frame == frame_event[i]){ //object 생성
            switch (this->type_event[i]){
                case 'n': {
                    Enemy_1n *enn = new Enemy_1n(y_event[i], x_event[i], 10, 1, i, 'n');
                    this->objects.push_back(enn);
                    board[y_event[i]][x_event[i]] = 'n';
                    break;
                }
                default:
                    break;
            }
        } else if (curr_frame > frame_event[i] && this->type_event[i] != '0'){
            board[y_event[i]][x_event[i]] = type_event[i];
        }
    }
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