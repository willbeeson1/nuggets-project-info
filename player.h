/* 
 * header file for player module in CS50 nuggets project
 *
 * Team: BashIt Ballers
 * 
 * May 2023
 */

#ifndef __PLAYER_H
#define __PLAYER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
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
/* Creates a new player and returns a pointer to it.
 * The caller is responsible for freeing the allocated memory.
 * Returns NULL if there's an error in memory allocation.
 */
player_t* player_new(char* name, int location, char letter, addr_t* address);

/**************** player_getName ****************/
/* Returns the name of the given player.
 */
char* player_getName(player_t* player);

/**************** player_getLocation ****************/
/* Returns the current location of the given player.
 */
int player_getLocation(player_t* player);

/**************** player_getAddress ****************/
/* Returns the address of the given player.
 */
addr_t* player_getAddress(player_t* player);

/**************** player_getScore ****************/
/* Returns the current score of the given player.
 */
int player_getScore(player_t* player);

/**************** player_getLetter ****************/
/* Returns the letter associated with the given player.
 */
char player_getLetter(player_t* player);

/**************** player_updateLocation ****************/
/* Updates the location of the given player.
 */
void player_updateLocation(player_t* player, int location);

/**************** player_setMap ****************/
/* Updates the map of the given player.
 */
void player_setMap(player_t* player, char* map);

/**************** player_incrementScore ****************/
/* Increments the score of the given player by a certain
 * value, and returns the updated score. 
 */
int player_incrementScore(player_t* player, int add);

#endif // __PLAYER_H
