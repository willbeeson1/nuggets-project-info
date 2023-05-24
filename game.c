#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "mem.h" 
#include "file.h"
#include "counters.h"
#include "player.h"
#include "support/message.h" 

//Questions
//what all should i do for gameOver 
//should i keep a set linking player names to its player struct?

typedef struct game { //struct of game object;
  char* mapString;  //string of map
  char playerList[25][49]; //array of players; max 26; max name length 50
  int playerCount; //num of players currently in the game 
  counters_t* goldValueAndLocations; //will map each gold pile's location to its value 
} game_t; 

game_t* 
game_new(char* mapString) 
{
  //what gets passed in to new
  game_t* game = mem_malloc(sizeof(game_t)); //allocating memory for the overall game 
  
  game->mapString = mapString; //initialize the map string 
  game->playerCount = 0; 
  game->goldValueAndLocations = counters_new(); 

  return game;  
}

void gameOver(game_t* game) //free memory of game 
{
  mem_free(game); 
}

void setNuggets(game_t* game, int goldValue, int goldLocation) //put all nuggets on map and also update counter to include all nuggets locations with their gold value 
{
  
  game->mapString[goldLocation] = '*'; 
  counters_set(game->goldValueAndLocations, goldLocation, goldValue); //mapping gold pile location to its value 

}

bool addPlayer(game_t* game, char* name, int location, char letter, addr_t* address) //adding a new player to the array TOOK OFF ADDRESS FOR NOW
{
  if (game->playerCount == 26) {
    return false; //unsuccessful 
  } 

  player_t* newPlayer = player_new(name, location, letter, address); //create player 

  game->playerCount += 1;
  strcpy(game->playerList[game->playerCount], name); //setting the added players index in the array to the passed in name
  
  //need to replace the '.' with a player first letter
  game->mapString[location] = letter; 

  return true; //succeded 
}

void movePlayer(game_t* game, player_t* player, int newLocation) 
{
  
  int oldLocation = player_getLocation(player); //grabbing current location
  game->mapString[oldLocation] = '.'; //setting current location to an empty space 
  player_updateLocation(player, newLocation); //updating player to new location in player struct 
  game->mapString[newLocation] = player_getLetter(player); //setting new location to player letter 

}

bool hasGold(game_t* game, int newLocation) 
{
  int determiner = counters_get(game->goldValueAndLocations, newLocation); //if the location does have gold, then the counter will return the gold's value which has to be greater than 0
  if (determiner > 0) {
    return true;
  } else { 
    return false; 
  } 
}

void addGold(game_t* game, player_t* player, int newLocation)
{
  int goldValue = counters_get(game->goldValueAndLocations, newLocation); //grabs gold pile's value 
  player_incrementScore(player, goldValue); //updates player score
  counters_set(game->goldValueAndLocations, newLocation, 0); //will set this gold pile value to 0 since it has been snatched 
}
