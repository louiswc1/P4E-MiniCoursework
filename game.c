//game.c Version 3.0 - Now contains fully functional startGame, movePlayer and displayWorld functions

#include "game.h"
#include <time.h> // required for seeding rand

/*__________________________________________________________________________________________________________________*/
//FUNCTION 1 - startGame

void startGame(Player *player) { /* Pointing to address of player to allow for modification of position, fuel
                                    and parts player data */

//PROLOGUE

    //Read prologue text from text file
    FILE *file = fopen("prologue.txt", "r");  //"r" mode is reading mode

    //Check if text file opened
    if (file == NULL) {
        printf("Error! Could not open prologue text file\n");
        return;  // Exit game if text file could not be opened
    }


    //Display the prologue text
    char line[150];  //creates a buffer to hold each line of prologue text, set to 150 to hold longest line.

    while (fgets(line, sizeof(line), file)) { /* Reads a line and stores it in buffer
                                                          while loop continues until 'fgets' returns nothing */

        printf("%s", line);  //Prints each line read from the text file
    }
    fclose(file);  //Close text file

    player->moves = 0;

/* ..................................................................................................................*/
//GAME MENU TO SELECT DIFFICULTY AND VIEW HIGHSCORE

    int difficulty; // Integer variable for difficulty setting

    // player can choose difficulty or view high score at start of game

    do {
        // do while loop continues until 1 or 2 is entered to choose difficulty

        printf("\n\nPlease choose an option from below...\n");
        printf("Enter '1' for Normal Mode\n"); // Starting fuel = 110 Litres
        printf("Enter '2' for Hard Mode\n"); // Starting fuel = 90 Litres
        printf("Enter '3' to view the current highscore\n");

        if (scanf("%d",&difficulty) !=1){ // states if scanf fails to read an integer
            while (getchar() != '\n'); // this is a new line buffer to prevent loop if input is not a number
            difficulty = 0; // reset difficulty input to allow while loop below to carry on.
            continue;
        }

        if (difficulty == 3){
            viewHighscore();  // show high score, then ask again for difficulty selection
        }
    } while (difficulty != 1 && difficulty != 2);

    // Set starting fuel
    if (difficulty == 1){ // double equal used to compare, not set.
        player->fuel = 110;
    } else{
        player->fuel = 90;
    }

/*...................................................................................................................*/
//WORLD STARTING POSITION

    // Sets player starting position to the center of world map (10, 10)
    player->x = world_size / 2;
    player->y = world_size / 2 -1;

/*...................................................................................................................*/
//STARTING SPACESHIP PARTS

    // Sets collected parts at start of game to 0
    player->parts = 0;

    printf("The game has started! You have %dL of fuel left. Good luck!\n", player->fuel);
}
/*__________________________________________________________________________________________________________________*/
//FUNCTION 2 - movePlayer

void movePlayer(Player *player,char move){

//PLAYER MOVEMENT

    //Switch and case statements used to determine player movement using WASD
    switch (move) {

//MOVE UP
        /* Moving up decreases y value
           Case w checks if y > 0
           If yes, move up
           If no, player is already at the top of world (row 0) and case w does not run as if statement becomes false */

        case 'w': // move up
            if (player->y > 0) {
                player->y--;
                player->fuel--; /*Decrements fuel by 1L for each attempted move. Pointer points to fuel section
                                  of player address. Line placed in each case statement to prevent fuel consumption when leaving world */
                player->moves++; // increments move highscore counter after move
            }
            break;

//MOVE DOWN
            /* Moving down increases y value
               Case s checks if y < 19 (world size - 1)
               If yes, player moves down
               If no, player is at the bottom edge of world and if statement is false */

        case 's': // move down
            if (player->y < world_size - 1) {
                player->y++;
                player->fuel--;
                player->moves++; // increments move highscore counter after move
                }
            break;

//MOVE LEFT
            /* Moving left decreases x value
               Case a checks if x > 0
               If yes, player can move left
               If no, player is already at far left of world and cannot move */

        case 'a': // move left
            if (player->x > 0) {
                player->x--;
                player->fuel--;
                player->moves++; // increments move highscore counter after move
                }
            break;

//MOVE RIGHT
            /* Moving right increases x value
              Case d checks if x < 19 (world size -1)
              If yes, player can move right
              If no, player is already at far right of world and if statement is false */

        case 'd': // move right
            if (player->x <world_size - 1) {
                player->x++;
                player->fuel--;
                player->moves++; // increments move highscore counter after move
                }
            break;

        default: // This statement is added to ignore all other input besides w, a, s and d
            break;
    }

/*...................................................................................................................*/

// CHECKING FOR COLLECTED SHIP PARTS AFTER EVERY MOVE
    for (int i = 0; i<10; i++){ // after every move, the below if statement is run 10 times for each ship part

        if (!player->shipParts[i].collected          // checks if part has been picked up
            && player->shipParts[i].x == player->x   // does player's x coordinate match part
            && player->shipParts[i].y == player->y){ // does player's y coordinate match part
            player->shipParts[i].collected = 1;      // marks the part as collected
            player->parts++;                         // increment part collected count up one
            break;                                   // ends loop if a part is found and collected
        }}}

/*__________________________________________________________________________________________________________________*/
//FUNCTION 3 - displayWorld

void displayWorld(Player player){    /* no pointer here as no changes need to be made, just reading position, fuel and parts etc
                                        from player structure */

// PRINTING THE GAME WORLD (20X20) INCLUDING PLAYER (O), SHIP PARTS (P), ASTEROIDS (A) AND EMPTY SPACE (.)

    // Function starts by checking all 400 cells of the game world

    for (int y = 0; y < world_size; y++) { // works through columns 0-19

        for (int x = 0; x < world_size; x++){ // Nested 'for' loop - works through rows 0-19

            if (x == player.x && y == player.y) { // if xy coordinate matches player position, print player 'O'
                printf("O ");  // character represented by 'O'

            } else {
                int isShipPart = 0; // created a check which is set to 0 to begin with before we check if a part has been allocated to a coordinate or empty space.

                // isShipPart looks at every cell in world and checks if any of the 10 ship parts have been assigned there.

                //Checks if the current xy coordinates have been occupied by a ship part
                for (int i = 0; i<10; i++) { // goes through the loop 10 times (10 ship parts)

                    if (!player.shipParts[i].collected && // only checks uncollected parts to be displayed (!0 is true as collected = 0)
                            player.shipParts[i].x == x && // checks if x coord of part matches coord of cell being printed
                            player.shipParts[i].y == y){  // checks if x coord of part matches coord of cell being printed
                            isShipPart = 1;               // Sets check to 1 if part is found at a space. All 10 parts across 400 spaces (20x20 grid) are checked
                            break;
                    }}

                if (isShipPart) { // if a ship part is found at a space
                    printf("P ");  // print 'P' for ship part

 // CHECK FOR ASTEROIDS
                } else{

                    int asteroidCheck = 0; // check for asteroids is initially set to 0

                    for (int i = 0; i<10; i++){ // repeats loop 10 times for each asteroid

                        if (player.asteroids[i].x == x && player.asteroids[i].y == y) {  // if asteroid is at the cell being checked (function checks all 400)
                            asteroidCheck = 1;  // Set check to 1 if asteroid is found at a space
                            break;
                        }
                    }

                    if (asteroidCheck) {  // if asteroid is found at a space
                        printf("A ");  // print 'A' on this space
                    } else{
                        printf(". ");  // empty space represented by '.'
                    }}}
        }

        printf("\n"); // this line ensures the loop moves on to next row and prints world columns/ rows correctly
    }
/*...................................................................................................................*/
// FUEL AND GAME PROGRESS MONITORING
    printf("\nFuel Remaining: %dL | Parts Collected: %d of 10\n", player.fuel, player.parts); // display fuel and collected parts
}

/*______________________________________________________________________________________________________________________*/
//FUNCTION 4 - placeShipParts

void placeShipParts(Player *player){

    srand(time(NULL)); // seeds random number generator

for (int i = 0; i<10; i++){ // 'for' loop to repeat the below 10 times for the 10 ship parts
    int emptySpace=0;

    while (!emptySpace){ //nested while loop
        emptySpace =1; // assume it's a new space unless found otherwise

        int x = rand() % world_size; // generates random x coordinate within world
        int y = rand() % world_size; // generates random y coordinate within world
        // rand() generates a random integer and % world_size takes modulus of that rand number and ensures it is between 0-19

        // for loop makes sure parts are not placed on duplicate spaces, j used as an index in nested loop.
        for (int j = 0; j < i; j++) {/* loops through already placed parts, e.g if trying to place part i, then program knows
                                                                                              parts 0 to i-1 have been placed. */

            if (player->shipParts[j].x==x && player->shipParts[j].y==y){ /* checks if ran generated coord matches position of any
                                                                             other existing coordinates */

                emptySpace = 0; // If space is already used program tries again and marks spot as taken
                break;
            }}

        if (emptySpace){ // if space does not already have a part in it, place a part and assign xy coord to array.
            player->shipParts[i].x = x;
            player->shipParts[i].y = y;
            player->shipParts[i].collected = 0; // once part is placed, mark it as not collected
        }
    }}}

/*______________________________________________________________________________________________________________________*/
//FUNCTION 5 - Asteroids

void Asteroids(Player *player, int check){ // check created to check if it is first time asteroid is being called for initial placement

// INITIAL PLACEMENT OF ASTEROID AT START OF EACH GAME
    if (check){

        for (int i = 0; i<10; i++){ // for loop will repeat 10 times for each asteroid

            player->asteroids[i].x = i * 2 + 1; // On top row, place asteroid in columns 1, 3, 5, 7, 9, 11, 13, 15, 17, 19
                                                // (first column is column 0, last column is column 19)

            player->asteroids[i].y = 0; // ensures all asteroids start along top row

            player->asteroids[i].direction = 1; // Direction set to 1 will result in asteroids moving downwards
                                                // Direction set to -1 will mean asteroid is moving upwards.
        }
        return; // ends the function after the first call
    }

/*...................................................................................................................*/
// ASTEROID MOVEMENT
    for (int i = 0; i<10; i++) {
        // 'for' loop runs continuously after every player move

        // Update y position (row) depending on direction
        player->asteroids[i].y = player->asteroids[i].y + player->asteroids[i].direction;

        // Flip direction if asteroid reaches top or bottom rows
        if (player->asteroids[i].y == world_size - 1){ // if asteroid is at bottom row of world
            player->asteroids[i].direction = -1;        // set direction to -1 (upwards)

        } else if (player->asteroids[i].y == 0){      // or if asteroid is on top row of world
            player->asteroids[i].direction = 1;         // set direction to 1 (downwards)
        }

/*...................................................................................................................*/
// COLLISION WITH PLAYER CHECK (LOSS CONDITION)

        // if asteroid xy position matches player position
        if (player->asteroids[i].x == player->x && player->asteroids[i].y == player->y){

            // end game and print game over message
            printf("You collided with an asteroid! Your ship has been destroyed, all hope of rescue is lost. Game Over.\n");
            exit(0); // end the game
        }
    }
}

/*______________________________________________________________________________________________________________________*/
//FUNCTION 6 - manageHighscore

void manageHighscore(int moves, int win){ // win should be 1 if player won and 0 if player lost

    FILE *file;

    int highScore;

    // read from text file
    file = fopen("highscore.txt","r"); //"r" mode is reading mode

    if (file == NULL){
        highScore = -1; // If file doesn't exist yet, no highscore exists
    } else{
        fscanf(file, "%d", &highScore);  // scan text file and reads highscore number
        fclose(file);
    }

    // only update high score if the player has won the game
    if (win == 1 && (highScore == -1 || moves < highScore)){ /* logic states: if player won game AND (highscore doesn't exist
                                                                              OR current moves is less than highscore) */

        file = fopen("highscore.txt","w"); // open text file in write mode

            fprintf(file, "%d", moves);
            fclose(file);
        }}
/*______________________________________________________________________________________________________________________*/
//FUNCTION 7 - viewHighscore

void viewHighscore(int score){ // score created to print highscore within menu

    FILE *file = fopen("highscore.txt", "r"); // set file to read mode

    if (file == NULL){
        printf("\nNo high score set\n\n");
    } else{
        fscanf(file, "%d", &score);
        fclose(file);
        printf("\nCurrent best game was completed in %d moves\n\n", score);
    }}

/*______________________________________________________________________________________________________________________*/
