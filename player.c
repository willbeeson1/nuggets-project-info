/*
 * player module - contains player_t struct and related functions
 * 
 * See player.h for documentation. 
 * 
 * Team 5: BashIt Ballers - May 2023
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "player.h"
#include "../support/message.h"
#include "../support/file.h"

typedef struct {
    const char* name;       // name inputted as client
    int location;           // index on map string
    const char letter;      // display alias within game
    addr_t* address;        // IP address for message handling
    char* visibleMap;       // map of game visible to player
    int score;              // amount of gold collected
} player_t; 

/**************** player_new ****************/
/* See player.h for documentation. */
player_t* 
player_new(char* name, int location, char letter, addr_t* address) 
{
    // allocate memory for new player
    player_t* player = malloc(sizeof(player_t));

    // handle malloc errors
    if (player == NULL) {
        fprintf(stderr, "Error allocating memory for player\n");
        return NULL; 
    }

    // set its variables to given arguments
    player->name = name; 
    player->location = location; 
    player->letter = letter; 
    player->address = address; 

    // initialize its other variables
    player->visibleMap = NULL;
    player->score = 0; 

    return player; 
}

/**************** player_getName ****************/
/* See player.h for documentation. */
char*
player_getName(player_t* player) 
{
    return player->name; 
}

/**************** player_getLocation ****************/
/* See player.h for documentation. */
int
player_getLocation(player_t* player) 
{
    return player->location; 
}

/**************** player_getAddress ****************/
/* See player.h for documentation. */
addr_t*
player_getAddress(player_t* player) 
{
    return player->address; 
}

/**************** player_getScore ****************/
/* See player.h for documentation. */
int
player_getScore(player_t* player) 
{
    return player->score; 
}

/**************** player_getLetter ****************/
/* See player.h for documentation. */
char
player_getLetter(player_t* player) 
{
    return player->letter;
}

/**************** player_updateLocation ****************/
/* See player.h for documentation. */
void
player_updateLocation(player_t* player, int location) {
    player->location = location; 
}

/**************** player_setMap ****************/
/* See player.h for documentation. */
void
player_setMap(player_t* player, char* map)
{
    player->visibleMap = map; 
}

/**************** player_incrementScore ****************/
/* See player.h for documentation. */
int
player_incrementScore(player_t* player, int add) {
    player->score += add;
    return player->score;  
}
