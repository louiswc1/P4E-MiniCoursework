// game.h Version 3.0

/*This header file contains structure declarations for player data,
 and prototypes for main game functions*/

#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <stdlib.h>

// World will be 0-19 wide (20 values), base and height
#define world_size 20

typedef struct{ // creates structure for randomly placed ship parts
    int x;
    int y;
    int collected; // 0 is not collected and 1 is collected
}ShipPart;

typedef struct { //structure for asteroid data
    int x;
    int y;
    int direction; // 1 for down, -1 for up
}Asteroid;

typedef struct { //declaring structures for player data
    int x, y; // will monitor player position
    int fuel; // will monitor remaining fuel
    int parts; //will monitor number of collected ship parts
    ShipPart shipParts[10]; // ShipPart struct and array of 10 ship parts stored within Player data structure
    Asteroid asteroids[10];  // Asteroid struct and array with 10 asteroids stored within player data structure
    int moves; // tracks player moves for highscore system
}Player;

//function prototypes
void startGame(Player *player);
void displayWorld(Player player);
void movePlayer(Player *player,char direction);
void placeShipParts(Player *player);
void Asteroids(Player *player, int check);
void manageHighscore(int moves, int win);
void viewHighscore();

#endif
