// test_client.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ncurses.h>
#include <unistd.h>
#include <ctype.h>
#include "../support/file.h"
#include "client.h"

void displayGame(char* mapString);
bool checkWindowSize(int required_y, int required_x);

int 
main(int argc, char *argv[]) 
{
    // check arguments
    if (argc != 2) {
        fprintf(stderr, "Usage: ./test_client map_file\n");
        return 1; // bad commandline 
    }

    // read map from file
    char* map_file = argv[1];
    FILE* ifile = fopen(map_file, "r");
    char* mapString = file_readFile(ifile);

    if (mapString == NULL) {
        fprintf(stderr, "Failed to read from map file\n");
        return 2; // failed to read map file
    }

    // display the map
    fprintf(stderr, "\nStarting window resize test"); 
    usleep(500000);
    fprintf(stderr, ".");
    usleep(500000);
    fprintf(stderr, ".");
    usleep(500000);
    fprintf(stderr, ".");
    usleep(500000);
    displayGame(mapString);

    // free(mapString); // free the map string

    return 0;
}

void
displayGame(char* gridData) {
    int numRows = 0, numColumns = 0; 
    int j = 0; 

    // Count the numbers of rows and columns
    for (int i = 0; i < strlen(gridData); i++) {
        if (gridData[i] == '\n') {
            numRows++;
            if (j > numColumns) {
                numColumns = j;
            }
            j = 0;
        } else {
            j++;
        }
    }

    fprintf(stderr, "\nColumns: %d", numColumns);
    fprintf(stderr, "\nRows: %d\n", numRows);
    sleep(2);

    // source for ncurses functions: https://jbwyatt.com/ncurses.html 

    initscr(); // Initialize the window

    while (!checkWindowSize(numRows + 1, numColumns + 1)) {
        clear(); // Clear the screen
        printw("Please resize the window to at least %d rows and %d columns.\n", numRows + 1, numColumns + 1);
        refresh(); // Refresh the screen to show the message
        getch(); // Wait for a key press before checking again
    }

    // Print the grid data
    clear(); // clear screen one more time before printing grid
    printw("Grid from main.txt\n");
    printw("%s", gridData);
    printw("\nPress [Q] or [q] to exit window resize test.\n");

    nodelay(stdscr, TRUE); // non-blocking, getch() will not wait for user input

    int ch;
    do {
        halfdelay(1); // wait up to 0.1 seconds for input
        ch = toupper(getch());
        if (ch == ERR) { 
            // no input received, check if the window is correct size
            if (!checkWindowSize(numRows + 1, numColumns + 1)) {
                clear();
                printw("Please resize the window to at least %d rows and %d columns.\n", numRows + 1, numColumns + 1);
            } else {
                clear(); // clear screen one more time before printing grid
                printw("Grid from main.txt\n");
                printw("%s", gridData);
                printw("\nPress [Q] or [q] to exit window resize test.\n");
            }
        }
    } while (ch != 'Q'); // Wait for 'Q' or 'q' to be pressed

    endwin(); // End window mode

}

bool 
checkWindowSize(int required_y, int required_x) {
    int max_y = 0, max_x = 0;
    getmaxyx(stdscr, max_y, max_x); // Get maximum window size

    if (max_y < required_y || max_x < required_x) {
        return false;
    }

    return true;
}


