/* client.c - client module for CS50 Nuggets project
*
* Team: BashIt ballers
* May 2023
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ncurses.h>
#include <unistd.h>
#include <ctype.h>
#include "client.h"
#include "../support/message.h"
#include "../support/file.h"

void displayGame(char* mapString);
bool handleInput(void* arg);
bool handleMessage(void* arg, const addr_t from, const char* buf);
bool checkWindowSize(int required_y, int required_x);


int 
main(int argc, char *argv[]) 
{
    // initialize the message module (without logging)
    if (message_init(NULL) == 0) {
        return 2; // failure to initialize message module
    }

    bool spectator = false; 
    char* playerName = NULL; 

    // check arguments
    const char* program = argv[0];
    if (argc == 3) {
        spectator = true; 
    } else if (argc == 4) {
        playerName = argv[3];
    } else { 
        fprintf(stderr, "Usage: ./%s hostname port [playername]\n", program);
        return 3; // bad commandline 
    }

    // commandline provides address for server 
    char* serverHost = argv[1];
    char* serverPort = argv[2];
    addr_t server; // address of server

    if (!message_setAddr(serverHost, serverPort, &server)) {
        fprintf(stderr, "Failed to form address from %s %s\n", serverHost, serverPort);
        return 4; // bad hostname/port
    }

    // tell server whether client is player or spectator
    char* message; 
    if (spectator) {
        message = "SPECTATE";
    } else if (playerName != NULL) { 
        message = malloc(strlen(playerName) + 6); 
        sprintf(message, "PLAY %s", playerName); 
    }

    message_send(server, message); // send msg to server
    // free(message); // free alloc'd memory for message

    bool loopResult = message_loop(&server, 0, NULL, handleInput, handleMessage);

    if (!loopResult) {
        fprintf(stderr, "Error in the message_loop call\n"); 
        return 5; // failed loop
    }

    message_done();

    return loopResult? 0 : 1; // status code depends on result of message_loop
}

//      Typical client sequence looks like this:
//  *   message_init(stderr);
//  *   message_setAddr(serverHost, serverPort, &serverAddress);
//  *   message_send(serverAddress, message); // client speaks first
//  *   message_loop(arg, timeout, handleTimeout, handleStdin, handleMessage);
//  *   message_done();



/***************** handleInput *****************/
/* function to handle stdin from client and send
 * to server as message 
 */
bool
handleInput(void* arg)
{
    addr_t* serverp = arg;
    if (serverp == NULL) {
        fprintf(stderr, "handleInput called with arg=NULL");
        return true;
    }
    if (!message_isAddr(*serverp)) {
        fprintf(stderr, "handleInput called without a correspondent.");
        return true;
    }

    char line[message_MaxBytes];

    // read a line from stdin
    if (fgets(line, message_MaxBytes, stdin) == NULL) {
        // stop looping if EOF
        return true;
    } else {
        // strip trailing newline
        const int len = strlen(line);
        if (len > 0 && line[len-1] == '\n') {
            line[len-1] = '\0';
        }

        // send as message to server
        message_send(*serverp, line);

        // normal case: keep looping
        return false;
    }
}

/***************** handleMessage *****************/
/* function to handle messages from server to client
 */
bool 
handleMessage(void* arg, const addr_t from, const char* buf) {
        // will handle incoming message from server

        // Check if message is a GRID msg
        if (strncmp(buf, "GRID", 4) == 0) {
            // extract data after "GRID "
            char* gridData = (char*) buf + 5;

            // initialize display with the grid data
            displayGame(gridData);
            return true;
        }
        return false;

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