/*
 * legalmove.c - tests implementation of:
 *  - `isMoveLegal`
 *  - `spot_isOpen`
 * 
 * Run with `make legal` within client directory
 *
 * Will Beeson, May 2023
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../client/client.h"
#include "../support/file.h"

bool isMoveLegal(char* map, char move, int* location); 
bool spot_isOpen(char k);

/* 
 * main - for my own testing purposes, but some of the 
 * logic may be useful in game or server
 */
int 
main(int argc, char *argv[])
{
    // check arguments
    if (argc != 2) {
        fprintf(stderr, "Usage: ./legalmove map_file\n");
        return 1; // bad commandline 
    }

    // read map from file
    char* map_file = argv[1];
    FILE* ifile = fopen(map_file, "r");
    char* mapString = file_readFile(ifile);
    char* tempMap = malloc(strlen(mapString) + 1); 
    strcpy(tempMap, mapString);

    int location = 87; // valid index in main.txt
    char tempChar = mapString[location]; 
    int tempLoc = location; 

    tempMap[location] = 'M'; 
    fprintf(stdout, "%s\n", tempMap);
    fprintf(stdout, "[k] = up\n[j] = down\n[h] = left\n[l] = right\n");
    fprintf(stdout, "Press [Q] or [q] to quit\n");


    // read characters from stdin 
    char ch; 

    while ((ch = getchar()) != 'Q' && ch != 'q') {
        if ((ch == '\n') || (((ch != 'h') && (ch != 'j')) && ((ch != 'k') && (ch != 'l')))) {
            continue; 
        }

        if (isMoveLegal(mapString, ch, &location)) {
            mapString[tempLoc] = tempChar; 
            tempChar = mapString[location];  // update tempChar after move is confirmed
            mapString[location] = 'M'; 
            tempLoc = location;              // also update tempLoc after the move
            fprintf(stdout, "%s\n", mapString);
            fprintf(stdout, "\n\x1B[32mSuccessfully completed legal move!\x1B[0m\n");
            fprintf(stdout, "[k] = up\n[j] = down\n[h] = left\n[l] = right\n");
            fprintf(stdout, "Press [Q] or [q] to quit\n"); 
        } 
        else {
            fprintf(stdout, "%s\n", mapString);
            fprintf(stdout, "\x1B[31mIllegal move attempted! Unsuccessful\x1B[0m\n");
            fprintf(stdout, "[k] = up\n[j] = down\n[h] = left\n[l] = right\n");
            fprintf(stdout, "Press [Q] or [q] to quit\n");
        }
    }
    free(tempMap);
    return 0;
}

/* 
 * isMoveLegal - checks if user can move to requested location
 *
 * Returns:
 *  - true, if so
 *  - false, otherwise
 * 
 * Takes:
 *  - map string
 *  - character for move request
 *  - pointer to integer location
 *  
 * Does:
 *  - updates integer location to new position
 *    if the move is legal (using its ptr)
 *  
 */
bool 
isMoveLegal(char* map, char move, int* location)
{
    bool result; 

    // Keystrokes are ‘h’ = left, ‘l’ = right, ‘j' = down, ‘k’ = up.
    char up = 'k';
    char down = 'j';
    char left = 'h';
    char right = 'l';

    // Handle invalid inputs
    if (((move != 'k') && (move != 'j')) && ((move != 'h') && (move != 'l'))) {
        fprintf(stderr, "Error: invalid 'move' character passed to `isMoveLegal`\n");
        return false; 
    }

    // Find player's position on current line
    int linePos = 0;
    int i = *location; 

    // go backwards until '\n'
    while (map[i] != '\n') {
        linePos++; // increment line pos with each iteration
        i--; 
    }

    // Find the location's row number 
    int row = 0;
    for (int i = 0; i < *location - 1; i++) {
        if (map[i] == '\n') {
            row++; 
        }
    }
    // increment once more after last '\n'
    row++; 

    // Calculate indices for spot above & below 
    if (move == up || move == down) {
        int j = 0;
        int lineCount = 0; 
        while (lineCount < row) {
            if (map[j] == '\n') {
                lineCount++; 
            }
            j++; 
        }
        int downIdx = j + linePos - 1; 
        int lineDiff = *location - downIdx; 
        int upIdx = *location + lineDiff;

        if (move == up) {
            result = spot_isOpen(map[upIdx]);
            if (result) {
                *location = upIdx;
            }
        } else if (move == down) {
            result = spot_isOpen(map[downIdx]);
            if (result) {
                *location = downIdx;
            }
        }
    } else if (move == left) {
        result = spot_isOpen(map[*location - 1]);
        if (result) {
            *location = *location - 1; 
        }
    } else if (move == right) {
        result = spot_isOpen(map[*location + 1]);
        if (result) {
            *location = *location + 1; 
        }
    } else {
        result = false; 
    }

    return result;
}

// helper function 
bool
spot_isOpen(char k) {
    if (k == '.' || k == '#' || k == '*') {
        return true; 
    } else {
        return false; 
    }
}
