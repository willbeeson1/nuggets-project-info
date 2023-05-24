#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <stdbool.h>
#include "../support/message.h"

void displayGame(char* mapString);
bool handleInput(void* arg);
bool handleMessage(void* arg, const addr_t from, const char* buf);

#endif /* CLIENT_H */
